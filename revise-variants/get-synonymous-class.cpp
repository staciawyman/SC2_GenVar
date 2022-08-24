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


ml_group string_to_ml_group(string::iterator x,string::iterator x_end)
{
    ml_group y;
    string t;


    while (x!=x_end) {
    if (*x=='\t') {
        break;
    }
    t.push_back(*x);
    ++x;
    }


    y.m=t;
    t.clear();


//  skip '\t' :
    while (x!=x_end) {
    ++x;
    break;
    }


    while (x!=x_end) {
    t.push_back(*x);
    ++x;
    }


    y.l=t;

 
    return y;
}


inline
ml_group string_to_ml_group(string& x)
{
    return string_to_ml_group(x.begin(),x.end());
}


ml_group_list_iter find_ml_group_m(ml_group_list_iter x,
                                   ml_group_list_iter x_end,string s)
{
    while (x!=x_end) {
	if ((*x).m==s) {
	    break;
	}
	++x;
    }
    return x;
}


inline
ml_group_list_iter find_ml_group_m(ml_group_list& x,string s)
{
    return find_ml_group_m(x.begin(),x.end(),s);
}


int main(int argc, char* argv[])
{
//  group list :
    string f="synonymous.txt";

    string_list l;
    ml_group t;


    read(f,l);

    
//  marker
    

    cout << "class synonymous : public ml_group_list {"
         << '\n'
         << "public:"
         << '\n'
         << "    synonymous();"
         << '\n'
         << "};"
         << '\n'
         << '\n'
         << '\n'
         << "synonymous::synonymous()"
         << '\n'
         << "{"
         << '\n'
         << "    ml_group_list& l=*this;"
         << '\n'
         << '\n'
         << '\n';


//  get group list :
    for (string_list_iter i=l.begin(); i!=l.end(); ++i) {


	t=string_to_ml_group(*i);


	cout << "    l.push_back(ml_group("
	     << '"'
	     << t.l
	     << '"'
	     << ","
	     << '"'
	     << t.m
	     << '"'
	     << "));"
	     << '\n'; 

    }


    cout << "}"
         << '\n';

}



