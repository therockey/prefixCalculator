#include <iostream>
#include "../include/CTree.h"
#include "../include/Util.h"
using namespace Util;
using namespace std;

template <typename T>
bool cli(const string& command, CTree<T>& drzewo) {
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
                CTree<T> drzewo2;
                drzewo2.enter(input);
                //CTree<T> drzewo3;
                //drzewo3.enter(splitExpression("+ 2 2"));
                //drzewo = drzewo+drzewo2+drzewo3;
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

template <typename T>
void loop(CTree<T>& ctree){

    string input;
    bool exit = true;

    // Kontynuuj odczytywanie wejścia od użytkownika, dopóki metoda 'cli' odpowiedzialna za interpretację komend użytkownika nie zwróci false
    do{
        cout << "CTree>";
        input = "";
        getline(cin, input);
        if(!input.empty()){
            exit = cli(input, ctree);
        }
    }while(exit);
}

int main() {

    string input;

    bool check = true;
    do {
        cout << "Choose the tree type: " << endl;
        cout << "1. Int" << endl;
        cout << "2. Double" << endl;
        cout << "3. String" << endl;
        cout << "Choice>";
        getline(cin, input);

        if(input=="1"){
            check=false;
            CTree<int> drzewo;
            loop(drzewo);
        }else if(input=="2"){
            check=false;
            CTree<double> drzewo;
            loop(drzewo);
        }else if(input=="3"){
            check=false;
            CTree<string> drzewo;
            loop(drzewo);
        }else{
            cout<<"ERROR: no such option.\n"<<endl;
        }
    }while(check);
}




