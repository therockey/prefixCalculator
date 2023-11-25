#include <iostream>
#include "../include/CTree.h"
#include "../include/Util.h"
using namespace Util;
using namespace std;

bool cli(const string& command, CTree& drzewo) {
    vector<string> input = splitExpression(command);
    string instruction = input.back();
    input.pop_back();

    if(instruction != "exit"){
        if(instruction == "enter"){

            if(input.empty()){
                cout << "ERROR: missing arguments." << endl;
            }else{
                drzewo.enter(input);
            }

        }else if(instruction == "comp"){

            if(input.size() != drzewo.getArgCount()){
                cout << "ERROR: incorrect number of arguments." << endl;
            }else{
                cout << drzewo.comp(input) << endl;
            }

        }else if(instruction == "vars"){

            drzewo.vars();

        }else if(instruction == "join"){

            if(input.empty()){
                cout << "ERROR: missing arguments." << endl;
            }else{
                drzewo = drzewo+input;
            }

        }else if(instruction == "print"){

            drzewo.print();

        }else {

            cout << "ERROR: no such command \"" << instruction << "\"." << endl;

        }
        return true;
    }
    return false;
}


int main() {

    string input;
    CTree drzewo;


    do{
        cout << "CTree>";
        input = "";
        getline(cin, input);
    }while(cli(input, drzewo));

}




