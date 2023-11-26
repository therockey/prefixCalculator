#ifndef LIST_3_TREE_STATIC_HPP
#define LIST_3_TREE_STATIC_HPP

#include <vector>
#include <map>
using namespace std;

class CTree{
private:
    class CNode{
        public:
            CNode();
            CNode(vector<string>* expr, vector<string>* variableNames, bool* flag);
            double getValue(map<string,int>& variables);
            void printBelow();
            void rediscoverVariables(vector<string>* variableNames);
            friend class CTree;

        private:

            int type;
            int value;
            string varName;
            std::vector<CNode> children;

    };

    CNode root;
    vector<string>* varNames;
public:
    CTree();
    ~CTree();
    int getArgCount();
    void enter(const vector<string>& formula);
    double comp(const vector<string>& args);
    void print();
    void vars();

    void operator=(const CTree& other);
    CTree operator+(const vector<string> &formula);
};


#endif //LIST_3_TREE_STATIC_HPP
