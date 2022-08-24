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


string get_pipe_dir(string::iterator x,string::iterator x_end)
{
    string t;

    while (x!=x_end) {
	if (*x=='/') {
	    break;
	}
	t.push_back(*x);
	++x;
    }
    return t;
}


inline
string get_pipe_dir(string& x)
{
    return get_pipe_dir(x.begin(),x.end());
}


string get_pipe_file(string::iterator x,string::iterator x_end)
{

    string t;


//  skip over "V4_invitae" first :
    while (x!=x_end) {
	if (*x=='/') {
	    break;
	}
	++x;
    }
    while (x!=x_end) {
	if (isdigit(*x)) {
	    break;
	}
	++x;
    }
    while (x!=x_end) {
	t.push_back(*x);
	++x;
    }
    return t;
}


inline
string get_pipe_file(string& x)
{
    return get_pipe_file(x.begin(),x.end());
}


int main(int argc, char* argv[])
{
//  *.variants.tsv file list :
    string f=argv[1];
 
    string_list l;


    read(f,l);


    for (string_list_iter i=l.begin(); i!=l.end(); ++i) {


	cout << "./revise-variants.exe "
	     << *i
	     << " > "
	     << get_pipe_dir( *i)
             << "/revised/"
	     << get_pipe_file(*i)
	     << '\n';


    }


}



