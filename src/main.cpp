#include <iostream>
#include "../include/CTree.h"
#include "../include/Util.h"
using namespace Util;
using namespace std;

bool cli(const string& command, CTree& drzewo) {
    // String command z argmuentu zamieniamy na wektor ciągów znaków oddzielonych spacją
    vector<string> input = splitExpression(command);

    // Odczytujemy, a następnie usuwamy z wektora pierwszy string i interpretujemy go jako komendę
    string instruction = input.back();
    input.pop_back();

    if(instruction != "exit"){ // Jeśli użytkownik nie chce wyjść z programu, kontynuuj wykonywanie, w innym przypadku FALSE
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
    bool exit = true;

    // Kontynuuj odczytywanie wejścia od użytkownika, dopóki metoda 'cli' odpowiedzialna za interpretację komend użytkownika nie zwróci false
    do{
        cout << "CTree>";
        input = "";
        getline(cin, input);
        if(!input.empty()){
             exit = cli(input, drzewo);
        }
    }while(exit);

}




