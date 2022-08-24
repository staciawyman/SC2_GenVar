//  read a file to a string list / print a string list to a file. 

/*------------------------------------------------------------------------*/


typedef list<string> string_list;
typedef list<string>::iterator string_list_iter;


void read(std::istream& is,list<string>& x)
{
    string s;
    while (getline(is,s)) {
	x.push_back(s);
    }
}


void read(string f,list<string>& x)
{
    std::ifstream ifs(f.c_str());
    if (!ifs) {
	cerr << "error. read (unable to open file " << f << ")\n";
	exit(1);
    }
    else {
	read(ifs,x);
    }
}


string int_to_string(int i)
{
    std::ostringstream os;
    os << i;
    return os.str();
}


inline
int string_to_int(string x)
{
    return atoi(x.c_str());
}


inline
double string_to_float(string x)
{
    return atof(x.c_str());
}


string file_string(string prefix,int i,string ext)
{
    std::ostringstream os;
    os << prefix
       << i
       << "."
       << ext;
    return os.str();
}


void print(list<string>::iterator x,list<string>::iterator x_end,
	std::ostream& os)
{
    while (x!=x_end) {
	os << *x
	   << '\n';
	++x;
    }
}

inline
void print(list<string>& x,std::ostream& os)
{
    print(x.begin(),x.end(),os);
}


void print(list<string>& x,string f)
{
    std::ofstream ofs(f.c_str());
    if (!ofs) {
	cerr << "error. print (unable to open file " << f << ")\n";
	exit(1);
    }
    else {
	print(x,ofs);
    }
}



