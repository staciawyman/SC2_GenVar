//  variants.tsv file interpeter.


#define verbose


/*
    NOTE!!!


    below, tsv fields are indexed with i in [1, ..], so that the first

    tsv field in a tsv file line has index 1.
*/

  
/*------------------------------------------------------------------------*/

string get_tsv_field(string::iterator x,string::iterator x_end,
                     int i)
{
    string s;

    int j;
    j=0;


line_1 : {}


    s.clear();


    while (x!=x_end) {
	if (*x=='\t') {
	    ++j;
	    break;
	}
	else {
	    s.push_back(*x);
	}
	++x;
    }
    

//  skip '\t' :
    while (x!=x_end) {
	++x;
	break;
    }


if (j<i && x!=x_end) {
    goto line_1;
}


    return s;
/*
    get tsv field number i. if there are too few fields, the function
 
    returns the last field in the input string.
*/
}
//  checked


inline
string get_tsv_field(string& x,int i)
{
    return get_tsv_field(x.begin(),x.end(),i);
}


void erase_tsv_field(string::iterator x,string::iterator x_end,
                     string& l,int i)
{
    string::iterator s;
    string::iterator t;

    int j;
    j=0;


line_1 : {}


    s=x;


    while (x!=x_end) {
	if (*x=='\t') {
	    ++j;
	    break;
	}
	++x;
    }


//  skip '\t' :
    while (x!=x_end) {
	++x;
	break;
    }


    t=x;


if (j<i && t!=x_end) {
    goto line_1;
}


    if (j==i || (t==x_end && j==i - 1)) {
	l.erase(s,t);
    }


/*
    erase tsv field number i, if it exists; otherwise do nothing.
*/
}
//  checked


inline
void erase_tsv_field(string& x,int i)
{
    erase_tsv_field(x.begin(),x.end(),x,i);
}


void insert_tsv_field(string::iterator x,string::iterator x_end,
                      string& l,string& r,int i)
{
//  r is the entry to a field, not includeing a '\t'.
    string::iterator s;
    string::iterator t;


    r.push_back('\t');
    

    int j;
    j=0;


line_1 : {}


    s=x;


    while (x!=x_end) {
	if (*x=='\t') {
	    ++j;
	    break;
	}
	++x;
    }


//  skip '\t' :
    while (x!=x_end) {
	++x;
	break;
    }


    t=x;


if (j<i && t!=x_end) {
    goto line_1;
}


    if (j==i || (t==x_end && j==i - 1)) {
	l.insert(t,r.begin(),r.end());
    }


/*
    insert a new tsv field after tsv field number i, if it exists;

    otherwise, do nothing.


    note that if the inserted field is the last field, then a '\t'
 
    will need to be removed from the end.
*/
}
//  checked


inline
void insert_tsv_field(string& x,string r,int i)
{
    insert_tsv_field(x.begin(),x.end(),x,r,i);
}



#undef verbose





