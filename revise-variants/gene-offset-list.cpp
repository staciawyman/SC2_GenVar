//  sars CoV 2 gene offsets..
  
/*------------------------------------------------------------------------*/

class gene_offset {
public:
//  gene name :
    string gene;

//  nt offset :
    int offset;
//  nt length :
    int length;


    gene_offset(string s,int i,int j);
};


gene_offset::gene_offset(string s,int i,int j)
{
    gene=s;

    offset=i;
    length=j;
}


typedef list<gene_offset>::iterator gene_offset_list_iter;


class gene_offset_list : public list<gene_offset> {
public:
    gene_offset_list();
};


gene_offset_list::gene_offset_list()
{
    list<gene_offset>& x=*this;


    x.push_back(gene_offset("E"    ,26242,  228));
    x.push_back(gene_offset("M"    ,26520,  669));
    x.push_back(gene_offset("N"    ,28271, 1260));
    x.push_back(gene_offset("ORF10",29555,  117));
    x.push_back(gene_offset("ORF1a",  263,13218));
    x.push_back(gene_offset("ORF1b",13465, 8072));
    x.push_back(gene_offset("ORF3a",25390,  828));
    x.push_back(gene_offset("ORF6" ,27199,  186));
    x.push_back(gene_offset("ORF7a",27391,  366));
    x.push_back(gene_offset("ORF7b",27753,  132));
    x.push_back(gene_offset("ORF8" ,27891,  318));
    x.push_back(gene_offset("S"    ,21560, 3822));


/*
    NOTE.


    it turned out that NSP gene offsets and lengths were not needed.

    i have commented them out to avoid picking them up in any search

    calls.
*/


/*
//  ORF1a sub-genes :
    x.push_back(gene_offset("NSP1" ,  263,  540));
    x.push_back(gene_offset("NSP2" ,  803, 1914));
    x.push_back(gene_offset("NSP3" , 2717, 5835));
    x.push_back(gene_offset("NSP4" , 8552, 1500));
    x.push_back(gene_offset("NSP5" ,10052,  918));
    x.push_back(gene_offset("NSP6" ,10970,  870));
    x.push_back(gene_offset("NSP7" ,11840,  249));
    x.push_back(gene_offset("NSP8" ,12086,  594));
    x.push_back(gene_offset("NSP9" ,12683,  339));
    x.push_back(gene_offset("NSP10",13022,  417));
    x.push_back(gene_offset("NSP12",13439, 2795));
    x.push_back(gene_offset("NSP13",16235, 1803));


//  ORF1b sub-genes :
    x.push_back(gene_offset("NSP14",18037, 1581));
    x.push_back(gene_offset("NSP15",19618, 1038));
    x.push_back(gene_offset("NSP16",20656,  894));
*/
}
//  checked



/*
    NOTE.


    all the offsets have been checked using the code in registry/
 

    all offsets are nt start positions of the genes measured from

    the beginning of the genome, nt positions i in [0, N].



gene    offset (nt) length (nt)


E       26242    228
M       26520    669
N       28271   1260
ORF10   29555    117
ORF1a     263  13218
ORF1b   13465   8072    (=21290 - 13218)
ORF3a   25390    828
ORF6    27199    186
ORF7a   27391    366
ORF7b   27753    132
ORF8    27891    318
S       21560   3822


offsets above were obtained from codon2nucleotide at :


                https://codon2nucleotide.theo.io/


NSP offsets below are computed as e.g. :


      offset (nt) = (ORF1a offset (nt)) + 3 * offset (codon)


all lengths are obtained from the ucsc genome browser at :


            https://genome.ucsc.edu/cgi-bin/hgGateway


by clicking the blue strip corresponding the apropriate gene

label for the sars cov 2 genome :


gene    offset (codon) offset (nt) length (nt)


ORF1b, 263 :

"NSP1" ,    0    263    540
"NSP2" ,  180    803   1914
"NSP3" ,  818   2717   5835  
"NSP4" , 2763   8552   1500
"NSP5" , 3263  10052    918
"NSP6" , 3569  10970    870
"NSP7" , 3859  11840    249
"NSP8" , 3941  12086    594
"NSP9" , 4140  12683    339
"NSP10", 4253  13022    417
"NSP11",    ?      ?     39
"NSP12", 4392  13439   2795
"NSP13", 5324  16235   1803


ORF1b, 13465 :

"NSP14", 1524  18037   1581
"NSP15", 2051  19618   1038
"NSP16", 2397  20656    894
 
 
 
 
   note the problem with these numbers for NSP13; codon2nucleotide
 
   says that NSP13 is a sub-gene of ORF1a, however, the ucsc genome
 
   browser gives a length for ORF1a that is too small to include
 
   the NSP13 gene.


   .  .  .



*/



gene_offset_list_iter find_gene(gene_offset_list_iter x,
                                gene_offset_list_iter x_end,string& s)
{
    while (x!=x_end) {
	if ((*x).gene==s) {
	    break;
	}
	++x;
    }
    return x;
}
//  checked


inline
gene_offset_list_iter find_gene(gene_offset_list& x,string s)
{
    return find_gene(x.begin(),x.end(),s);
}


inline
gene_offset_list_iter find_gene_for_pos(gene_offset_list_iter x,
                                        gene_offset_list_iter x_end,int i)
{
    while (x!=x_end) {
	if (i>=(*x).offset && i<(*x).offset + (*x).length) {
	    break;
	}
	++x;
    }
    return x;
}
//  checked


inline
gene_offset_list_iter find_gene_for_pos(gene_offset_list& x,int i)
{
    return find_gene_for_pos(x.begin(),x.end(),i); 
}



