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


class ml_group {
public:
//  lineages :
    string l;
//  mutation :
    string m;

    ml_group() {}
    ml_group(string ll,string mm) : l(ll), m(mm) {}
};


typedef list<ml_group> ml_group_list;
typedef list<ml_group>::iterator ml_group_list_iter;


void append_list(ml_group_list_iter x,ml_group_list_iter x_end,
                 ml_group_list& f,string l,string m)
{
    while (x!=x_end) {
	if ((*x).m==m) {
	    (*x).l=(*x).l + ',' + l;
	    break;
	}
	++x;
    }


if (x!=x_end) {
    goto line_1;
}


    f.push_back(ml_group(l,m));


line_1 : {}


}
//  checked.


inline
void append_list(ml_group_list& x,string l,string m)
{
    append_list(x.begin(),x.end(),x,l,m);
}


string gene_string(string::iterator x,string::iterator x_end)
{
    string t;
    
    while (x!=x_end) {
	if (*x==':') {
	    break;
	}
	t.push_back(*x);
	++x;
    }
    return t;
}
//  checked.


inline
string gene_string(string& x)
{
    return gene_string(x.begin(),x.end());
}


inline
string gene_string(ml_group& x)
{
    return gene_string(x.m);
}


void get_gene_list(ml_group_list_iter x,ml_group_list_iter x_end,
                   ml_group_list& y,string gene)
{
    while (x!=x_end) {
	if (gene_string(*x)==gene) {
	    y.push_back(*x);
	}
	++x;
    }
}
//  checked.


inline
void get_gene_list(ml_group_list& x,ml_group_list& y,string gene)
{
    get_gene_list(x.begin(),x.end(),y,gene);
}


#define read_indels
//#define read_synonymous
//#define read_non_synonymous


int main()
{
    string_list l_1;
    string_list l_2;
    string_list l_3;

    ml_group_list x;
    ml_group_list y;


    l_1.push_back("alpha");
    l_1.push_back("beta");
    l_1.push_back("delta");
    l_1.push_back("gamma");
    l_1.push_back("mu");
    l_1.push_back("BA.1");
    l_1.push_back("BA.2");


    for (string_list_iter i=l_1.begin(); i!=l_1.end(); ++i) {


	read(*i + ".txt",l_2);


	for (string_list_iter j=l_2.begin(); j!=l_2.end(); ++j) {


#ifdef read_indels
if ((*j).find('-')==std::string::npos) {
    goto line_1;
}
#endif


#ifdef read_synonymous 
if ((*j).find(':')!=std::string::npos || 
     *j=="Synonymous" || 
     *j=="Nonsynonymous") {
    goto line_1;
}
#endif


#ifdef read_non_synonymous 
if ((*j).find(':')==std::string::npos ||
    (*j).find('-')!=std::string::npos) {
    goto line_1;
}
#endif


	    append_list(x,*i,*j);


line_1 : {}


	}


	l_2.clear();
    }


/*
    for (ml_group_list_iter i=x.begin(); i!=x.end(); ++i) {
	cout << (*i).m
	     << '\t'
	     << (*i).l
	     << '\n';
    }    
*/


#ifndef read_synonymous
    l_3.push_back("E");
    l_3.push_back("M");
    l_3.push_back("N");
    l_3.push_back("ORF10");
    l_3.push_back("ORF1a");
    l_3.push_back("ORF1b");
    l_3.push_back("ORF3a");
    l_3.push_back("ORF6");
    l_3.push_back("ORF7a");
    l_3.push_back("ORF7b");
    l_3.push_back("ORF8");
    l_3.push_back("S");


    for (string_list_iter i=l_3.begin(); i!=l_3.end(); ++i) {
	get_gene_list(x,y,*i);
    }


    for (ml_group_list_iter i=y.begin(); i!=y.end(); ++i) {
	cout << (*i).m
	     << '\t'
	     << (*i).l
	     << '\n';
    }
#else
    for (ml_group_list_iter i=x.begin(); i!=x.end(); ++i) {
	cout << "snt:" + (*i).m
	     << '\t'
	     << (*i).l
	     << '\n';
    }    
#endif


}



