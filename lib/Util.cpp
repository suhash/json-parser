//
//  Util.cpp
//  kwiq
//
//  Created by walmart on 14/08/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Util.h"

using namespace std;

unsigned long get_hash(string str){
    unsigned long hash_val = 5381;
    
    for(int i = 0; i < (int)str.size(); ++i){
        int c = str[i];
        hash_val = ((hash_val << 5) + hash_val) + c;
    }
    return hash_val;
}

size_t key_comparer::operator()(const string &a) const{
    return get_hash(a);
}

bool key_comparer::operator()(const string &a, const string &b) const{
    return a < b;
}

string get_string(string &val){
    ostringstream sout;
    sout << val;
    return sout.str();
}

string get_string(double &val){
    ostringstream sout;
    sout << val;
    return sout.str();
}

string get_string(int &val){
    ostringstream sout;
    sout << val;
    return sout.str();
}

string get_string(long long &val){
    ostringstream sout;
    sout << val;
    return sout.str();
}

void normalize(string &val){
    string ret = "";
    int len = (int)val.size();
    for(int i = 0; i < len; i++){
        if(val[i] == '"' || val[i] == '\\' || val[i] == '/')    ret += '\\';
        ret += val[i];
    }
    val = ret;
}