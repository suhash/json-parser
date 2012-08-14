//
//  Object.cpp
//  kwiq
//
//  Created by walmart on 14/08/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Object.h"

using namespace std;

Value::Value(){
    cur_type = _empty;
    val = "";
    map = NULL;
    arr = NULL;
}

Value::~Value(){}

void Value::free(){
    if(map != NULL){
        for(__typeof((*map).begin()) it = (*map).begin(); it != (*map).end(); ++it){
            (it->second).free();
        }
        delete map;
    }
    if(arr != NULL){
        for(__typeof((*arr).begin()) it = (*arr).begin(); it != (*arr).end(); ++it){
            it->free();
        }
        delete arr;
    }
}

Value::Value(int value){
    cur_type = _integer;
    val = get_string(value), map = NULL, arr = NULL;
}

Value::Value(long long value){
    cur_type = _integer;
    val = get_string(value), map = NULL, arr = NULL;
}

Value::Value(string value){
    normalize(value);
    cur_type = _str;
    val = get_string(value), map = NULL, arr = NULL;
}

Value::Value(double value){
    cur_type = _real;
    val = get_string(value), map = NULL, arr = NULL;
}

Value::Value(bool value){
    cur_type = _boolean;
    string v = (value == 0) ? "false" : "true";
    val = get_string(v), map = NULL, arr = NULL;
}

string Value::get_val(string &json_str, int i, int j){
    string str = "";
    for(int k = i; k <= j; k++) str += json_str[k];
    return str;
}

Object_map Value::get_map(string &json_str, int i, int j){
    string str = "";
    for(int k = i; k <= j; k++) str += json_str[k];
    JsonObject obj(str);
    return obj.get_json();
}

vector<Value> Value::get_arr(string &json_str, int i, int j){
    i++, j--;
    int k = i, cnt1 = 0;
    vector<Value> ret;
    while(k <= j){
        if(json_str[k] == '"'){
            string val = "";
            k++, cnt1 = 1;
            while(cnt1 == 1 && k <= j){
                if(json_str[k] == '\\') val += json_str[k] + json_str[k+1], k += 2;
                else if(json_str[k] == '"') cnt1--, k += 2;
                else val += json_str[k], k++;
            }
            Value value(val);
            ret.push_back(value);
        }
        else if(json_str[k] == '['){
            int cnt2 = 1, st = k;
            cnt1 = 0, k++;
            while(cnt2 > 0 && k <= j){
                if(json_str[k] == '\\') k++;
                else if(json_str[k] == '"'){
                    if(cnt1 == 0)   cnt1++;
                    else cnt1--;
                }
                else if(json_str[k] == '[' || json_str[k] == ']'){
                    if(!cnt1){
                        if(json_str[k] == '[')     cnt2++;
                        else cnt2--;
                    }
                }
                k++;
            }
            Value value(json_str, st, k-1);
            ret.push_back(value);
            k++;
        }
        else if(json_str[k] == '{'){
            int cnt2 = 1, st = k;
            cnt1 = 0, k++;
            while(cnt2 > 0 && k <= j){
                if(json_str[k] == '\\') k++;
                else if(json_str[k] == '"'){
                    if(cnt1 == 0)   cnt1++;
                    else cnt1--;
                }
                else if(json_str[k] == '{' || json_str[k] == '}'){
                    if(!cnt1){
                        if(json_str[k] == '{')     cnt2++;
                        else cnt2--;
                    }
                }
                k++;
            }
            Value value(json_str, st, k-1);
            ret.push_back(value);
            k++;
        }
    }
    return ret;
}

bool isequal(Object_map *A, Object_map *B){
    if(A == NULL && B == NULL)  return true;
    if((A == NULL && B != NULL) || (A != NULL && B == NULL))  return false;
    Object_map a = *A, b = *B;
    if((int)a.size() != (int)b.size())  return false;
    for(Object_map::iterator it = a.begin(); it != a.end(); ++it){
        string key = it -> first;
        if(a[key] != b[key])    return false;
    }
    return true;
}

bool isequal(vector<Value> *A, vector<Value> *B){
    if(A == NULL && B == NULL)  return true;
    if((A == NULL && B != NULL) || (A != NULL && B == NULL))  return false;
    vector<Value> a = *A, b = *B;
    if((int)a.size() != (int)b.size())  return false;
    for(int i = 0; i < (int)a.size(); ++i){
        if(a[i] != b[i])    return false;
    }
    return true;
}

Value::Value(Object_map value){
    this->cur_type = _json;
    val = "";
    map = new Object_map(value);
    for(Object_map::iterator it = (*map).begin(); it != (*map).end(); ++it){
        if((it->second).get_type() == _json){
            Object_map temp = *((it->second).map);
            (it->second).map = NULL;
            Value val(temp);
            it->second = val;
        }
        else if((it->second).get_type() == _array){
            vector<Value> temp = *((it->second).arr);
            (it->second).arr = NULL;
            Value val(temp);
            it->second = val;
        }
    }
    arr = NULL;
}

Value::Value(JsonObject value){
    this->cur_type = _json;
    val = "";
    map = new Object_map(value.get_json());
    for(Object_map::iterator it = (*map).begin(); it != (*map).end(); ++it){
        if((it->second).get_type() == _json){
            Object_map temp = *((it->second).map);
            (it->second).map = NULL;
            Value val(temp);
            it->second = val;
        }
        else if((it->second).get_type() == _array){
            vector<Value> temp = *((it->second).arr);
            (it->second).arr = NULL;
            Value val(temp);
            it->second = val;
        }
    }
    arr = NULL;
}

Value::Value(vector<Value> value){
    this->cur_type = _array;
    val = "";
    map = NULL;
    arr = new vector<Value>(value);
    for(vector<Value>::iterator it = (*arr).begin(); it != (*arr).end(); ++it){
        if(it->get_type() == _json){
            Object_map temp = *(it->map);
            it->map = NULL;
            Value val(temp);
            (*it) = val;
        }
        else if(it->get_type() == _array){
            vector<Value> temp = *(it->arr);
            it->arr = NULL;
            Value val(temp);
            (*it) = val;
        }
    }
}

Value::Value(string &json_str, int i, int j){
    if(json_str[i] == '{'){
        this->cur_type = _json;
        val = "";
        map = new Object_map(get_map(json_str, i, j));
        arr = NULL;
    }
    else if(json_str[i] == '['){
        this->cur_type = _array;
        val = "";
        map = NULL;
        arr = new vector<Value>(get_arr(json_str, i, j));
    }
    else{
        this->cur_type = _str;
        val = get_val(json_str, i, j);
        map = NULL;
        arr = NULL;
    }
}

object_type Value::get_type(){
    return cur_type;
}

bool Value::operator<(Value cur){
    return val < cur.val;
}

bool Value::operator>(Value cur){
    return val > cur.val;
}

bool Value::operator==(Value cur){
    return cur_type == cur.cur_type &&
    val == cur.val &&
    isequal(map, cur.map) &&
    isequal(arr, cur.arr);
}

bool Value::operator!=(Value cur){
    return !(*this == cur);
}

void Value::operator=(Value cur){
    cur_type = cur.cur_type;
    val = cur.val;
    map = cur.map;
    arr = cur.arr;
}

string Value::to_string(){
    string ret = "";
    if(cur_type == _empty){
        ret += "null";
    }
    else if(cur_type == _json){
        Object_map json = *map;
        ret += "{";
        for(__typeof(json.begin()) it = json.begin(); it != json.end(); ++it){
            string key = it -> first;
            Value val = it -> second;
            if(it != json.begin())  ret += ",";
            ret += "\"" + key + "\":" + val.to_string();
        }
        ret += "}";
    }
    else if(cur_type == _array){
        vector<Value> val = *arr;
        ret += "[";
        for(int i = 0; i < (int)val.size(); i++){
            if(i > 0)   ret += ",";
            ret += val[i].to_string();
        }
        ret += "]";
    }
    else{
        ret += "\"" + val + "\"";
    }
    return ret;
}

JsonObject::JsonObject(){
    json.clear();
}

JsonObject::~JsonObject(){
    json.clear();
}

JsonObject::JsonObject(string &json_str){
    int i = 2, j = (int)json_str.size() - 2, k = i, cnt1 = 0;
    while(k <= j){
        string key = "";
        cnt1 = 1;
        while(cnt1 == 1 && k <= j){
            if(json_str[k] == '\\') key += json_str[k] + json_str[k+1], k += 2;
            else if(json_str[k] == '"') cnt1--, k += 2;
            else key += json_str[k], k++;
        }
        if(json_str[k] == '"'){
            string val = "";
            k++, cnt1 = 1;
            while(cnt1 == 1 && k <= j){
                if(json_str[k] == '\\') val += json_str[k] + json_str[k+1], k += 2;
                else if(json_str[k] == '"') cnt1--, k += 2;
                else val += json_str[k], k++;
            }
            k++;
            Value value(val);
            json[key] = value;
        }
        else if(json_str[k] == '['){
            int cnt2 = 1, st = k;
            cnt1 = 0, k++;
            while(cnt2 > 0 && k <= j){
                if(json_str[k] == '\\') k++;
                else if(json_str[k] == '"'){
                    if(cnt1 == 0)   cnt1++;
                    else cnt1--;
                }
                else if(json_str[k] == '[' || json_str[k] == ']'){
                    if(!cnt1){
                        if(json_str[k] == '[')     cnt2++;
                        else cnt2--;
                    }
                }
                k++;
            }
            Value value(json_str, st, k-1);
            json[key] = value;
            k += 2;
        }
        else if(json_str[k] == '{'){
            int cnt2 = 1, st = k;
            cnt1 = 0, k++;
            while(cnt2 > 0 && k <= j){
                if(json_str[k] == '\\') k++;
                else if(json_str[k] == '"'){
                    if(cnt1 == 0)   cnt1++;
                    else cnt1--;
                }
                else if(json_str[k] == '{' || json_str[k] == '}'){
                    if(!cnt1){
                        if(json_str[k] == '{')     cnt2++;
                        else cnt2--;
                    }
                }
                k++;
            }
            Value value(json_str, st, k-1);
            json[key] = value;
            k += 2;
        }
    }
}

void JsonObject::put(string key, JsonObject &val){
    normalize(key);
    Value value(val);
    json[key] = value;
}

void JsonObject::put(string key, int val){
    normalize(key);
    Value value(val);
    json[key] = value;
}

void JsonObject::put(string key, long long val){
    normalize(key);
    Value value(val);
    json[key] = value;
}

void JsonObject::put(string key, string val){
    normalize(key);
    Value value(val);
    json[key] = value;
}

void JsonObject::put(string key, const char *val){
    put(key, string(val));
}

void JsonObject::put(string key, bool val){
    normalize(key);
    Value value(val);
    json[key] = value;
}

void JsonObject::put(string key, double val){
    normalize(key);
    Value value(val);
    json[key] = value;
}

void JsonObject::put(string key, vector<Value> &val){
    normalize(key);
    Value value(val);
    json[key] = value;
}

string JsonObject::to_string(){
    string ret = "";
    ret += "{";
    for(__typeof(json.begin()) it = json.begin(); it != json.end(); ++it){
        string key = it -> first;
        Value val = it -> second;
        if(it != json.begin())  ret += ",";
        ret += "\"" + key + "\":" + val.to_string();
    }
    ret += "}";
    return ret;
}

void JsonObject::free(){
    for(__typeof(json.begin()) it = json.begin(); it != json.end(); ++it){
        (it->second).free();
    }
    json.clear();
}

string JsonObject::get(string key){
    normalize(key);
    return json[key].to_string();
}

Object_map JsonObject::get_json(){
    return json;
}

Value JsonObject::get_value(string key){
    normalize(key);
    return json[key];
}