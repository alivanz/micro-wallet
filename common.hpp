#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>
#include <map>
#include <vector>
using namespace std;

string GetValue(std::map<string,string>, string);

vector<char> HexToBytes(string);
string BytesToHex(vector<char>);

#endif
