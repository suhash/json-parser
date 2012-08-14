//
//  Object.h
//
//  Created by suhash on 13/08/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <map>
#include <string>
#include <vector>
#include <hash_map.h>
#include <hash_set.h>

#include "Util.h"

using namespace std;

class Value;
class JsonObject;

enum object_type{ 
    _str,
    _json,
    _real,
    _array,
    _empty,
    _boolean,
    _integer
};

typedef hash_map<string, Value, key_comparer> Object_map;

class Value{
    
private:
    object_type cur_type;
    string val;
    Object_map *map;
    vector <Value> *arr;

public:
    Value();
    ~Value();
    void free();
    Value(int value);
    Value(long long value);
    Value(string value);
    Value(double value);
    Value(bool value);
    Value(string &json_str, int i, int j);
    Value(JsonObject value);
    Value(Object_map value);
    Value(vector<Value> value);
    string get_val(string &json_str, int i, int j);
    object_type get_type();
    vector<Value> get_arr(string &json_str, int i, int j);
    Object_map get_map(string &json_str, int i, int j);
    bool operator<(Value cur);
    bool operator>(Value cur);
    bool operator==(Value cur);
    bool operator!=(Value cur);
    void operator=(Value cur);
    string to_string();
};

class JsonObject{
    
private:
    Object_map json;

public:
    JsonObject();
    ~JsonObject();
    JsonObject(string &json_str);
    Value get_value(string key);
    void free();
    void put(string key, JsonObject &val);
    void put(string key, int val);
    void put(string key, long long val);
    void put(string key, string val);
    void put(string key, const char *val);
    void put(string key, bool val);
    void put(string key, double val);
    void put(string key, vector<Value> &val);
    string get(string key);
    string to_string();
    Object_map get_json();
};
