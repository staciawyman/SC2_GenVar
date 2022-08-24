
# USAGE: sh sc2.sh <PAUI> <lab>
# e.g.  sh sc2.sh 500013006 ucsf
# Stacia Wyman 10 June 2020

name=$1
run=$2
run_dir=pipeline_output/$2
if [ ! -d $run_dir ] 
then
    mkdir $run_dir
fi
cd ${run_dir}
if [ -f mutations/${name}.variants.tsv ]  && [ -f fasta/${name}.consensus.fasta ];
then
    echo "$name.variants.tsv and consensus.fasta exists: bailing"
    exit
fi

if [ ! -d  bams ]; then mkdir bams; fi
if [ ! -d  mutations ]; then mkdir mutations; fi
if [ ! -d  fasta ]; then mkdir fasta; fi
if [ ! -d  logs ]; then mkdir logs; fi

genome=/groups/wyman/projects/sc2/references/sars-cov-2.fa
gff=/groups/wyman/projects/sc2/references/sars.gff3

# figure out which primer bed file to use since different sequencing labs use different protocols
primer_bed_file=/groups/wyman/projects/sc2/references/nCoV-2019.bed
if [ $run = "vpd" ];
then
echo "swift_sc2_V2.bed being used for trimming" 
echo "swift_sc2_V2.bed being used for trimming" >${name}.log
primer_bed_file=/groups/wyman/projects/sc2/references/swift_sc2_V2.bed
fi
if [ $run = "ucsd" ];
then
echo "swift_sc2_V2.bed being used for trimming" 
echo "swift_sc2_V2.bed being used for trimming" >>${name}.log
primer_bed_file=/groups/wyman/projects/sc2/references/swift_sc2_V2.bed
fi
if [ $run = "ucsf" ];
then
echo "ARTICv3_275.bed being used for trimming" 
echo "ARTICv3_275.bed being used for trimming" >>${name}.log
primer_bed_file=/groups/wyman/projects/sc2/references/ARTICv3_275.bed
fi
if [ $run = "ucsc" ];
then
echo "neb_vss1a.primer.bed.bed being used for trimming" 
echo "neb_vss1a.primer.bed.bed being used for trimming" >>${name}.log
primer_bed_file=/groups/wyman/projects/sc2/references/neb_vss1a.primer.bed
fi
if [ $run = "V4_invitae" ] || [ $run = "V4_ucsc" ] || [ $run = "V4_ucb" ];
then
echo "ARTICv4.bed being used for trimming" >>${name}.log
primer_bed_file=/groups/wyman/projects/sc2/references/ARTICv4.bed
fi

if [ ! -e /groups/wyman/projects/kpnc_fastq/${run}*/*/*${name}*R1*q.gz ];
then
    if [ ! -e /groups/wyman/projects/kpnc_fastq/${run}*/*/${name}*barc*q.gz ];
    then
        echo "can't find fastq file: /groups/wyman/projects/kpnc_fastq/${run}*/*/*${name}*R1*q.gz"
        exit
    else 
	echo "sh clearlabs_sc2.sh ${name} ${locale}" >> /groups/wyman/projects/genvar/run_clearlabs.sh
	exit
    fi
fi

# Annoying bbmap won't extrapolate the input file name, make temp fastq files
cp /groups/wyman/projects/kpnc_fastq/${run}*/*/*${name}_*R1*q.gz ${name}_R1.fastq.gz
cp /groups/wyman/projects/kpnc_fastq/${run}*/*/*${name}_*R2*q.gz ${name}_R2.fastq.gz

# Illumina Adapter Removal with BBDuK:
bbduk.sh -Xmx2g in1=${name}_R1.fastq.gz in2=${name}_R2.fastq.gz \
    out1=${name}.rmadpt_1.fastq.gz out2=${name}.rmadpt_2.fastq.gz \
    ref=/shared/software/bbmap/latest/resources/adapters.fa \
    stats=${name}.adapters.stats.txt ktrim=r k=23 mink=11 hdist=1 tpe tbo 2>&1 | tee -a ${name}.log 

# PhiX removal with bbduk: No PhiX added to our samples so removes nothing 
bbduk.sh -Xmx2g in1=${name}.rmadpt_1.fastq.gz in2=${name}.rmadpt_2.fastq.gz \
    out1=${name}_1.clean.fastq.gz out2=${name}_2.clean.fastq.gz \
    outm=${name}.matched_phix.fq ref=/shared/software/bbmap/latest/resources/phix174_ill.ref.fa.gz \
    k=31 hdist=1 stats=${name}.phix.stats.txt 2>&1 | tee -a ${name}.log

# Reference alignment with minimap2:
# minimap2 params: -K minibatch size for mapping -x sr genomic short read mapping -a output SAM format (instead of PAF)
echo "minimap2 -K 20M -x sr -a $genome ${name}_1.clean.fastq.gz ${name}_2.clean.fastq.gz " >>  ${name}.log  
minimap2 -K 20M -x sr -a $genome ${name}_1.clean.fastq.gz ${name}_2.clean.fastq.gz 2>>  ${name}.log  | \
    samtools view -u -h -q 20 -F 4 - | \
    samtools sort > ${name}.sorted.bam 

samtools index ${name}.sorted.bam
samtools flagstat ${name}.sorted.bam 2>&1 | tee -a ${name}.log

# Trim primers with iVar for amplicon/ARTIC sequencing
# -e to keep reads with no primers!
# ivar trim -i SC2.bam -b ${primer_bed_file} -p ivar -e
echo "ivar  trim  -e  -q 0  -i ${name}.sorted.bam  -b ${primer_bed_file}  -p ${name}.primer_trimmed" >> ${name}.log 
ivar  trim  -e  -q 0  -i ${name}.sorted.bam  -b ${primer_bed_file}  -p ${name}.primer_trimmed 2>&1 | tee -a ${name}.log

# Or bbduk for primer trimming --> syntax issue here
#bbduk.sh in=raw.fq.gz out=trimmed.fq.gz ref=artic3.fa ktrim=l restrictleft=30 k=22 hdist=3 qhdist=1 rcomp=f

# Create pileup file for iVar to create consensus and call variants from
if [ -f ${name}.primer_trimmed.bam ]; then 
    samtools sort ${name}.primer_trimmed.bam -o ${name}.trim.sorted.bam
    samtools mpileup -f ${genome} -d 1000000 -q 20  -B -Q 0 ${name}.trim.sorted.bam > ${name}.pileup
else
    samtools mpileup -f ${genome} -d 1000000 -q 20  -B -Q 0 ${name}.sorted.bam > ${name}.pileup
fi

# Create consenus sequence for GISAID, GenBank 
ivar consensus -p ${name}.cons -m 10 -n N < ${name}.pileup >> ${name}.log 2>&1

echo ">${name}" > fasta/${name}.consensus.fasta
seqtk seq -U -l 50 ${name}.cons.fa | tail -n +2 >> fasta/${name}.consensus.fasta

# Do we want to pipe mpileup into ivar? Is that faster?
#ivar variants -p ${name}.variants -g SC2.gff -r ncbi_wuhan.fa.fasta -t 0.6 < ${name}.pileup
echo "ivar variants -p ${name}.muts -r ${genome} -g ${gff} -t 0.01 < ${name}.pileup" >> ${name}.log
ivar variants -p ${name}.muts -r ${genome} -g ${gff} -t 0.01 < ${name}.pileup

# Add column of amino acid mutations
echo "/groups/wyman/projects/genvar/pipeline_output/revise-variants.exe  ${name}.muts.tsv >  mutations/${name}.variants.tsv" >> ${name}.log
/groups/wyman/projects/genvar/pipeline_output/revise-variants.exe  ${name}.muts.tsv >  mutations/${name}.variants.tsv

# change SARS... in variant file to ID name. 
echo "sed -i 's/^SARS-CoV-2|NC_045512.2|/${name}/' mutations/${name}.variants.tsv" >> ${name}.log
sed -i "s/^SARS-CoV-2|NC_045512.2|/${name}/" mutations/${name}.variants.tsv


# Cleanup!
echo "Cleaning up--removing intermediate files" >> ${name}.log
mv $name.trim.sorted.bam bams
mv $name.log logs

rm ${name}.sorted.bam 
rm ${name}_R1.fastq.gz
rm ${name}_R2.fastq.gz
rm ${name}_1.clean.fastq.gz
rm ${name}_2.clean.fastq.gz
rm ${name}.pileup
rm ${name}.cons.fa
rm ${name}.sorted.bam.bai
rm ${name}.primer_trimmed.bam
rm ${name}.cons.qual.txt
rm ${name}.adapters.stats.txt
rm ${name}.phix.stats.txt
rm ${name}.matched_phix.fq
rm ${name}.muts.tsv
rm ${name}.rmadpt_1.fastq.gz
rm ${name}.rmadpt_2.fastq.gz

#
cd -
