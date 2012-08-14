//
//  Util.h
//
//  Created by suhash on 13/08/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <string>
#include <sstream>
#include <hash_map.h>

using namespace std;

// djb2 hash function
unsigned long get_hash(string str);


struct key_comparer{
    size_t operator()(const string &a) const;
    bool operator()(const string &a, const string &b) const;
};

string get_string(string &val);
string get_string(double &val);
string get_string(int &val);
string get_string(long long &val);
void normalize(string &val);
