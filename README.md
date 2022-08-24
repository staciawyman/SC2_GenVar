# SC2_GenVar
Code and data for SARS-CoV-2 sequencing in Northern California

## Files in this repository
- sc2_pipeline.sh: this is the driving script for analyzing fastq data which takes as input a PAUI (identifier assigned by CDPH) and the source lab (data is stored based on which facility sequenced the data).
- metadata.csv: metadata file contains more PAUI's than there exists fasta and mutation files because sequencing failed, was ClearLabs sequenced (too poor quality for our use), or sequencing data has not been received yet. 

## Output files
fasta and mutation files for this repository can be found at DOI https://doi.org/10.6078/D1JQ5C

## Mutation matrices
Matrices were constructed for all genomes at any frequency above 2% of reads (to as to not capture sequencing errors). Initially this was just performed for delta using ```delta_aa_indel_matrix.pl```.

## Low frequency mutation consensus files
We use ivar for consensus files, but ivar doesn't have a frequency cutoff for calling mutations and uses majority rule or more (not less), so we needed to create low frequency consensus files by introducing LFMs (low frequency mutations) into the reference genome. To do this we took the output of ivar variants, converted it to a VCF, then used bcftools to introduce the variants to the reference genome.

```python ivar2vcf.py -ic -po -af 0.02 mutations/500030432.variants.tsv 500030432.vcf
bcftools consensus --fasta-ref ../../../sc2/references/sars-cov-2.fa -H A -o 500030432.consensus.bcftools.fa 500030432.vcf.gz```
