//
//  utility.cpp
//  CS32 P4
//
//  Created by Shenran Chen on 3/14/23.
//

#include "utility.h"
#include <string>
using namespace std;

//makes the string lowercase
string toLower(string s)
{
    for(int i = 0; i< s.size(); i++)
    {
        s[i] = tolower(s[i]);
    }
    return s;
}
