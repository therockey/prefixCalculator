#include <iostream>
#include "../include/CTree.h"
#include "../include/Util.h"
using namespace Util;
using namespace std;

bool cli(const string& command, CTree& drzewo) {
    // String command z argumentu zamieniamy na wektor ciągów znaków oddzielonych spacją
    vector<string> input = splitExpression(command);

    // Odczytujemy, a następnie usuwamy z wektora pierwszy string i interpretujemy go jako komendę
    string instruction = input.back();
    input.pop_back();

    if(instruction != "exit"){ // Jeśli użytkownik nie chce wyjść z programu, kontynuuj wykonywanie, w innym przypadku FALSE
        if(instruction == "enter"){

            // Jeśli użytkownik nie podał wyrażenia, wypisz błąd
            if(input.empty()){
                cout << "ERROR: missing arguments." << endl;
            }else{
                drzewo.enter(input);
            }

        }else if(instruction == "comp"){

            // Jeśli użytkownik przekazał mniej argumentów, niż drzewo ma zmiennych, wypisz błąd
            if(input.size() != drzewo.getArgCount()){
                cout << "ERROR: incorrect number of arguments." << endl;
            }else{
                cout << drzewo.comp(input) << endl;
            }

        }else if(instruction == "vars"){

            drzewo.vars();

        }else if(instruction == "join"){

            // Jeśli użytkownik nie podał wyrażenia do przyłączenia, wypisz błąd
            if(input.empty()){
                cout << "ERROR: missing arguments." << endl;
            }else{
                CTree drzewo2;
                drzewo2.enter(input);
                drzewo = drzewo+drzewo2;
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




