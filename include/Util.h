#ifndef LISTA_3_UTIL_H
#define LISTA_3_UTIL_H

#include <string>
#include <vector>
#include <map>
using namespace std;

namespace Util {

    vector<string> splitExpression(const string& expression);
    bool mapContains(map<string,int>* variables, const string& key);
    bool vectorContains(const vector<string>& varNames, const string& key);
    void printVector(const vector<string>& elements);
    string strSub(string first, const string& second);
    string strMult(const string& first, const string& second);
    string strDiv(string first, const string& second);

}


#endif //LISTA_3_UTIL_H
