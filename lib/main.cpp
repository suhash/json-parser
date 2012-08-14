//
//  main.cpp
//  kwiq
//
//  Created by walmart on 12/08/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Object.h"

using namespace std;

/*
 Testing for JsonObject.
 Be sure to call free() method on the object, when done using.
 Default destructor has been disabled. Manual memory management implemented.
 */

int main(int argc, const char * argv[]){
    JsonObject a, b, c;
    a.put("wow", 32);
    a.put("haha", "works");
    b.put("awesome", a);
    b.put("but", a);
    vector <Value> s;
    Value tmp(2.34);
    s.push_back(a);
    s.push_back(b);
    s.push_back(tmp);
    c.put("fantastic", s);
    c.put("marvellous", s);
    s.push_back(Value("hoho"));
    s.push_back(c);
    c.put("mind-blowing", s);
    cout << a.to_string() << endl;
    cout << b.to_string() << endl;
    cout << c.to_string() << endl;
    string json_str = c.to_string();
    JsonObject d(json_str);
    cout << endl;
    cout << d.to_string() << endl << endl;
    Value vv = d.get_json()["fantastic"];
    cout << vv.to_string() << endl;
    a.free();
    b.free();
    c.free();
    d.free();
    return 0;
}