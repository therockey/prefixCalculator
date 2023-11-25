#ifndef LISTA_3_UTIL_H
#define LISTA_3_UTIL_H

#include <string>
#include <vector>
#include <map>
using namespace std;

namespace Util {
    vector<string> splitExpression(const string& expression);
    bool isInteger(const string& str);
    int stringToInt(const std::string& str);
    bool mapContains(map<string,int>* variables, const string& key);
    bool vectorContains(const vector<string>& varNames, const string& key);
    void printVector(const vector<string>& elements);
}


#endif //LISTA_3_UTIL_H
