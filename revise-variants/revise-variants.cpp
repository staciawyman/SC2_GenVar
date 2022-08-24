#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <valarray>
#include <cstdlib>
using namespace std;


#include <limits>


#include <stdlib.h>
#include <math.h>


#include "plain-file-io.cpp"


#include "ml-group-list.cpp"


//  characteristic mutations :
#include "non-synonymous.cpp"
#include "synonymous.cpp"
#include "indels.cpp"


#include "gene-offset-list.cpp"


#include "tsv-file-tools.cpp"



#define revert_NSP_to_ORF
#define add_summary_field



int n_commas(string&);
int n_omicron(string&);



int main(int argc, char* argv[])
{
//  input *.variants.tsv input file name :
    string f_1=argv[1];
 
    string_list l_1;


//  read *.variants file :
    read(f_1,l_1);


//  gene offset list :
    gene_offset_list_iter t;


    gene_offset_list l;


//  gff feature, nt_position :
    string gff;
    string pos;

//  reference aa, mutant aa :
    string aa1;
    string aa2;
    string s;

    int p;
    int q;
    
    
/*
    NOTE!!


    this entire program assumes that the number of fields is <= 21;  if

    there are more fields, the program will need to be revised.


    the easiest way to revise the program would be to have it erase and

    revise all the all of the fields to be revised, including fields 20,

    and 21 (see below).
DONE

    .  .  .


    below, we add field # 22 in the last loop, to summarize information
 
    in field # 21.


    the only revision to the previous code is to 'pre-erase' field # 22
 
    in the first loop.


    .  .  .


*/



//  append AA_MUT field :
    for (string_list_iter i=l_1.begin(); i!=l_1.end(); ++i) {


	erase_tsv_field(*i,22);
	erase_tsv_field(*i,21);
	erase_tsv_field(*i,20);


	if ((*i).back()=='\t') {
	    (*i).pop_back();
	}


if (i==l_1.begin()) {


    *i=*i + '\t' + "AA_MUT";


/*
    cout << *i
         << '\n';
*/


    goto line_1;
}


	gff=get_tsv_field(*i,15);


	if (gff.substr(0,3)!="NSP") {
//  !NSP :

	    t=find_gene(l,gff);
	}
	else {
//   NSP :

	    pos=get_tsv_field(*i, 2);


	    p=atoi(pos.c_str());


	    t=find_gene_for_pos(l,p);
	}


	if (t!=l.end()) {
//   found :

	    pos=get_tsv_field(*i, 2);


	    p=atoi(pos.c_str());


	    q=p - (*t).offset;
	    q/=3;


	    aa1=get_tsv_field(*i,17);
	    aa2=get_tsv_field(*i,19);


	    s=(*t).gene + ':' + aa1 + int_to_string(q) + aa2;


	    *i=*i + '\t' + s;


/*
	    cout << *i
	         << '\n';
*/


	}
	else {
//  !found :

	    *i=*i + '\t' + "NA";


/*
	    cout << *i
	         << '\n';
*/


	}


line_1 : {}


    }
//  checked


//  characteristic non_synonymous :
    non_synonymous ns;


//  append VAR_LIST :
    ml_group_list_iter r;

    string aa_mut;


    for (string_list_iter i=l_1.begin(); i!=l_1.end(); ++i) {


if (i==l_1.begin()) {


	*i=*i + '\t' + "VAR_LIST";


/*
	cout << *i
	     << '\n';
*/


    goto line_2;
}


	aa_mut=get_tsv_field(*i,20);


/*
cerr << aa_mut
     << '\n';
*/


	r=find_if(ns,m_equal_to(aa_mut));


	if (r!=ns.end()) {
//   characteristic :


	    *i=*i + '\t' + (*r).l;


/*
	    cout << *i
	         << '\n';
*/


	}
	else {
//  !characteristic :

	    *i=*i + '\t' + "NA";


/*
	    cout << *i
	         << '\n';
*/


	}


line_2 : {}


    }
//  checked


/*
    NOTE!!! :


    the code below affects indel lines only. it re-writes the field 

    for GFF_FEATURE, and the AA_MUT, and VAR_LIST fields.
*/


//  characteristic indels :
    indels x;

    ml_group_list y;

    string ref;
    string alt;

    int len;


    for (string_list_iter i=l_1.begin(); i!=l_1.end(); ++i) {


if (i==l_1.begin()) {
    goto line_3;
}


	alt=get_tsv_field(*i,4);


 	if (alt[0]=='+' || alt[0]=='-') {
//  indel :


	    erase_tsv_field(*i,21);
	    erase_tsv_field(*i,20);


	    if ((*i).back()=='\t') {
		(*i).pop_back();
	    }


	    pos=get_tsv_field(*i,2);


//  nt position :
	    p=atoi(pos.c_str());


	    if ((t=find_gene_for_pos(l,p))!=l.end()) {
//  p in [offset,offset + length[ for a gene :


		gff=(*t).gene;


//  re--write gene name :
		 erase_tsv_field(*i,    15);
		insert_tsv_field(*i,gff,14);


//  # of deleted nt's :
		len=alt.size();
		--len;


//  nt position in gene :
		q =p - (*t).offset;


		if (len % 3==0 && q % 3==0) {
//  amino acid string indel :


		    len/=3;
		      q/=3;


		    get_indel_list(x,y,gff,q,q + len);


		    if (y.size()!=0) {
//   characteristic :


			*i=*i + '\t' + get_m_string(y) + '\t' + get_l_string(y);
		    }
		    else {
//  !characteristic :


			ref=get_tsv_field(*i,3);


			s=(*t).gene + ';' + ref + int_to_string(p) + alt;


			*i=*i + '\t' + s + '\t' + "NA";
		    }

		}
		else {
//  !amino acid string indel :


		    ref=get_tsv_field(*i,3);


		    s=(*t).gene + ';' + ref + int_to_string(p) + alt;


		    *i=*i + '\t' + s + '\t' + "NA";
		}


	    }
	    else {
//  p !in [offset,offset + length[ for a gene :

		*i=*i + '\t' + "NA" + '\t' + "NA";
	    }


//  : end if indel
	}


line_3 : {}


    }
//  checked


/*
    revert NSP's to ORF's in the GFF_FEATURE field using the ORF strings

    obtained for the AA_MUT field of NSP's in the first loop above.
*/


#ifdef revert_NSP_to_ORF
    for (string_list_iter i=l_1.begin(); i!=l_1.end(); ++i) {


//  GFF_FEATURE :
        s=get_tsv_field(*i,15).substr(0,3);


if (s!="NSP") {
    goto line_4;
}


//  AA_MUT :
        s=get_tsv_field(*i,20).substr(0,5);


        if (s=="ORF1a" || s=="ORF1b") {
             erase_tsv_field(*i,  15);
            insert_tsv_field(*i,s,14);
        }


line_4 : {}


    }
#endif
//  checked


#ifdef add_summary_field
    for (string_list_iter i=l_1.begin(); i!=l_1.end(); ++i) {


if (i==l_1.begin()) {


	*i=*i + '\t' + "UNIQUE";


    goto line_5;
}


	s=get_tsv_field(*i,21);


	if (s!="NA") {
//   characteristic :


	    if (n_commas(s)==0) {
//   unique :


		*i=*i + '\t' + 'Y';
	    }
	    else {
		if (n_commas(s)==1 && n_omicron(s)==2) {
//   unique (omicron) :


		    *i=*i + '\t' + 'M';
		}
		else {
//  !unique :


		    *i=*i + '\t' + 'N';
		}
	    }
	}
	else {
//  !characteristic :


	    *i=*i + '\t' + "NA";
	}


line_5 : {}


    }
#endif


    print(l_1,cout);


}


int n_commas(string::iterator x,string::iterator x_end)
{
    int i;


    i=0;
    while (x!=x_end) {
	if (*x==',') {
	    ++i;
	}
	++x;
    }
    return i;
}


inline
int n_commas(string& x)
{
    return n_commas(x.begin(),x.end());
}


int n_omicron(string::iterator x,string::iterator x_end)
{
    string s;

    int j;


    j=0;


line_1 : {}


    s.clear();


    while (x!=x_end) {
	if (*x==',') {
	    break;
	}
	s.push_back(*x);
	++x;
    }


    if (s=="BA.1" || 
        s=="BA.2") {
	++j;
    }


//  skip ',' :
    while (x!=x_end) {
	++x;
	break;
    }


if (x!=x_end) {
    goto line_1;
}


    return j;
}


inline
int n_omicron(string& x)
{
    return n_omicron(x.begin(),x.end());
}





