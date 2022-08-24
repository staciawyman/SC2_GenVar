//  characteristic mutations (including indels).

/*------------------------------------------------------------------------*/

//  characteristic mutation, and its lineage(s).

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


class m_equal_to {
    string s;
public:
    m_equal_to(string& x) : s(x) {}
    bool operator () (ml_group& x) const
    {
	return x.m==s;
    }
};


class indel_match {

    bool f(string::iterator x,string::iterator x_end) const;

    string s;

    int i;

public:
    indel_match(string& ss,int ii);
    bool operator () (ml_group& x) const;
};


indel_match::indel_match(string& ss,int ii)
{
    s=ss;
    i=ii;

/*
    here, s is the GFF_FEATURE field (gene string) for an indel and

    i is the first reference codon position in gene s obtained from

    the POS field and the gene offset list.
*/
}


bool indel_match::f(string::iterator x,
                    string::iterator x_end) const
{
    string t;

    int j;


    while (x!=x_end) {
	if (*x==':') {
	    break;
	}
	t.push_back(*x);
	++x;
    } 


if (t!=s) {
    goto line_1;
}


//  skip ':' :
    while (x!=x_end) {
	++x;
	break;
    }


    t.clear();


    while (x!=x_end) {
	if (isdigit(*x)) {
	    break;
	}
	++x;
    }


//  get j :
    while (x!=x_end) {
	if (!isdigit(*x)) {
	    break;
	}
	t.push_back(*x);
	++x;
    }


    j=atoi(t.c_str());


if (j!=i) {
    goto line_1;
} 


    return true;


line_1 : {}


    return false;
/*
    true if the input gene string, s, and gene position, i, are equal

    to that in the m string of an indels ml_group.
*/
}
//  checked


inline
bool indel_match::operator () (ml_group& x) const
{
    return f(x.m.begin(),x.m.end());
}


template<class F>
ml_group_list_iter find_if(ml_group_list_iter x,
                           ml_group_list_iter x_end,F f)
{
    while (x!=x_end) {
	if (f(*x)) {
	    break;
	}
	++x;
    }
    return x;
}
//  checked


template<class F>
inline
ml_group_list_iter find_if(ml_group_list& x,F f)
{
    return find_if(x.begin(),x.end(),f);
}


void get_indel_list(ml_group_list& x,ml_group_list& y,
                    string& s,int i,int i_end)
{
    ml_group_list_iter t;


    y.clear();


    while (i!=i_end) {


	t=find_if(x,indel_match(s,i));


	if (t!=x.end()) { 
	    y.push_back(*t);
	}
	else {


/*
    all codons in [i,i_end[ must have a match in x otherwise the 

    deletion is not characteristic :
*/


	    y.clear();
	    break;
	}


	++i;
    }
/*
    get the list of ml_groups corresponding to a multi-site deletion

    affecting codons [i,i_end[ in gene s.


    by requiring all codons in [i,i_end[ to have a match, we exclude
 
    longer indels that might include codons [i,i_end[ as a subset.
*/
}
//  checked


void get_l_strings(string::iterator x,string::iterator x_end,
                   string_list& y)
{
    string t;


    y.clear();


line_1 : {}


    t.clear();


    while (x!=x_end) {
	if (*x==',') {
	    break;
	}
	t.push_back(*x);
	++x;
    }


//  skip ',' :
    while (x!=x_end) {
	++x;
	break;
    }


    y.push_back(t);


if (x!=x_end) {
    goto line_1;
} 


/*
    parse the individual lineage strings (alpha, beta, etc.) from

    the l string of an ml_group and record them in a string_list.
*/
}
//  checked.


inline
void get_l_strings(string& x,string_list& y)
{
    get_l_strings(x.begin(),x.end(),y);
}


void intersect(string_list_iter x,string_list_iter x_end,
               string_list_iter y,string_list_iter y_end,
               string_list& l)
{
    string_list_iter f;
    f=y;
    
    
    l.clear();


    while (x!=x_end) {
	y=f;
	while (y!=y_end) {
	    if (*y==*x) {
		break;
	    }
	    ++y;
	}
	if (y!=y_end) {
	    l.push_back(*x);
	}
	++x;
    }
/*
    generate a list of the strings shared by lists x and y.
*/
}
//  checked.


inline
void intersect(string_list& x,string_list& y,string_list& l)
{
    intersect(x.begin(),x.end(),y.begin(),y.end(),l);
}


string join_l_strings(string_list_iter x,string_list_iter x_end)
{
    string t;


    while (x!=x_end) {
	t=t + *x + ',';
	++x;
    }


    if (t.back()==',') {
	t.pop_back();
    }


    return t;
/*
    join the strings in a string list into a single comma separated
 
    string.
*/
}
//  checked.


inline
string join_l_strings(string_list& x)
{
    return join_l_strings(x.begin(),x.end());
}


string get_l_string(ml_group_list_iter x,ml_group_list_iter x_end) 
{
    string_list r;
    string_list s;
    string_list t;


    get_l_strings((*x).l,r);


    while (x!=x_end) {
	++x;
	break;
    }


    while (x!=x_end) {


	get_l_strings((*x).l,s);


	intersect(r,s,t);


	r.clear();
	r.splice(r.end(),t);


	++x;
    }


    return join_l_strings(r);
/*
    generate an ml_group l string that is the intersect() of the

    lists of variant type strings for a list of ml_groups.
*/
}
//  checked.


inline
string get_l_string(ml_group_list& x)
{   
    return get_l_string(x.begin(),x.end());
}


string get_m_string(ml_group_list_iter x,ml_group_list_iter x_end)
{
    string m;


    while (x!=x_end) {
	m=m + (*x).m + ',';
	++x;
    }


    if (m.back()==',') {
	m.pop_back();
    }


    return m;
/*
    join the m strings for a list of ml_groups into a single comma
 
    seprated string.
*/
}
//  checked.


inline
string get_m_string(ml_group_list& x)
{   
    return get_m_string(x.begin(),x.end());
}



