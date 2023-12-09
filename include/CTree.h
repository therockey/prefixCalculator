#ifndef LIST_3_TREE_STATIC_HPP
#define LIST_3_TREE_STATIC_HPP

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include "../include/Util.h"
using namespace std;
using namespace Util;

template <typename T>
class CTree{
private:
    class CNode{
        public:
            CNode();
            CNode(vector<string>* expr, vector<string>* variableNames, bool* flag);
            T getValue(map<string,T>& variables);
            void printBelow();
            void rediscoverVariables(vector<string>* variableNames);
            friend class CTree;

        private:

            int type;
            T value;
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
    T comp(const vector<string>& args);
    void print();
    void vars();

    void operator=(const CTree<T>& other);
    CTree<T> operator+(const vector<string> &formula) const;
    CTree<T> operator+(const CTree<T>& other) const;
};

template <typename T>
CTree<T>::CTree(){
    varNames = new vector<string>;
}

template <typename T>
CTree<T>::~CTree() {
    delete varNames;
}

template <typename T>
int CTree<T>::getArgCount(){
    return varNames->size();
}

template <typename T>
CTree<T>::CNode::CNode() : type(), value(){}

template <typename T>
T CTree<T>::CNode::getValue(map<string,T>& variables) {
    switch(type){
        case 0: return value; // Węzeł jest wartością liczbową
        case 1: return children[0].getValue(variables)+children[1].getValue(variables); //Węzeł jest operacją +
        case 2: return children[0].getValue(variables)-children[1].getValue(variables); //Węzeł jest operacją -
        case 3: return children[0].getValue(variables)*children[1].getValue(variables); //Węzeł jest operacją *
        case 4: return children[0].getValue(variables)/children[1].getValue(variables); //Węzeł jest operacją /
        case 5: return sin(children[0].getValue(variables)); //Węzeł jest operacją sin
        case 6: return cos(children[0].getValue(variables)); //Węzeł jest operacją cos
        case 7: return variables[varName]; // Węzeł jest zmienną

    }
    return 0;
}

/*
template <>
double CTree<int>::CNode::getValue(map<string,int>& variables) {
    switch(type){
        case 0: return value; // Węzeł jest wartością liczbową
        case 1: return children[0].getValue(variables)+children[1].getValue(variables); //Węzeł jest operacją +
        case 2: return children[0].getValue(variables)-children[1].getValue(variables); //Węzeł jest operacją -
        case 3: return children[0].getValue(variables)*children[1].getValue(variables); //Węzeł jest operacją *
        case 4: return children[0].getValue(variables)/children[1].getValue(variables); //Węzeł jest operacją /
        case 5: return sin(children[0].getValue(variables)); //Węzeł jest operacją sin
        case 6: return cos(children[0].getValue(variables)); //Węzeł jest operacją cos
        case 7: return variables[varName]; // Węzeł jest zmienną

    }
    return 0;
}
*/

template <typename T>
CTree<T>::CNode::CNode(vector<string>* expr, vector<string>* variableNames, bool* flag) {

    if(expr->empty()) // Jeśli w wyrażeniu brakuje argumentów, to ustawiamy domyślny węzeł z wartością 1 i ustawiamy flagę błędu na TRUE;
    {
        type=0;
        value=1;
        *flag = true;

    }else{

        // Pobieramy następny ciąg znaków i usuwamy go z wektora wyrażenia
        string eval = expr->back();
        expr->pop_back();

        // Sprawdzam, czy aktualny ciąg znaków jest liczbą, jeśli jest to przypisz typ 0 do węzła i wartość liczbową z 'eval' to pola value
        if (isInteger(eval)) {
            type = 0;
            value = stringToInt(eval);
        } else if (eval =="+") { // Jeśli 'eval' to nie liczba, to sprawdź, czy nie jest jedną z możliwych operacji (jeśli tak to przypisz odpowiedni typ do węzła i utwórz odpowiednią liczbę dzieci)
            type = 1;
            children.push_back(CNode(expr, variableNames, flag));
            children.push_back(CNode(expr, variableNames, flag));
        } else if (eval == "-") {
            type = 2;;
            children.push_back(CNode(expr, variableNames, flag));
            children.push_back(CNode(expr, variableNames, flag));
        } else if (eval == "*") {
            type = 3;
            children.push_back(CNode(expr, variableNames, flag));
            children.push_back(CNode(expr, variableNames, flag));
        } else if (eval == "/") {
            type = 4;
            children.push_back(CNode(expr, variableNames, flag));
            children.push_back(CNode(expr, variableNames, flag));
        } else if (eval == "sin") {
            type = 5;
            children.push_back(CNode(expr, variableNames, flag));
        } else if (eval == "cos") {
            type = 6;
            children.push_back(CNode(expr, variableNames, flag));
        } else { // Jeśli 'eval' to nie liczba i nie operacja, to potraktuj jako zmienną
            type = 7;
            varName = eval; // Przechowujemy nazwę zmiennej w Węźle

            // Sprawdzamy, czy nazwa nie występuje już w wektorze nazw zmiennych; jeśli nie, to go tam dodajemy
            if (!vectorContains(*variableNames, eval)) {
                variableNames->push_back(eval);
            }
        }
    }
}

template <typename T>
void CTree<T>::CNode::printBelow() { //drukowanie węzłów w kolejności preorder, aby uzyskać odpowiednią kolejność znaków w wyrażeniu

    // Drukujemy odpowiednią tekstową reprezentację węzła, w zależności od jego typu
    switch (type) {
        case 0: cout << value << " "; break;
        case 1: cout << "+ "; break;
        case 2: cout << "- "; break;
        case 3: cout << "* "; break;
        case 4: cout << "/ "; break;
        case 5: cout << "sin "; break;
        case 6: cout << "cos "; break;
        case 7: cout << varName << " ";
    }

    // Następnie powtarzamy operację dla dzieci węzła
    for(int i=0; i<children.size(); i++){
        children[i].printBelow();
    }
}

//Metoda odszukująca rekursywnie zmienne w drzewie i umieszczająca ich nazwy w vecotrze 'variableNames'
template <typename T>
void CTree<T>::CNode::rediscoverVariables(vector<string> *variableNames) {

    // Jeśli typ węzła (Node) jest inny niż 7, czyli nie jest zmienną, to szukamy zmiennych dalej, schodząc poziom niżej w drzewie do dzieci węzła
    if(type!=7){
        for(int i=0; i<children.size(); i++){
            children[i].rediscoverVariables(variableNames);
        }
    }else{

        // Jeśli natrafiono na zmienną, sprawdzamy, czy jej nazwa nie występuje już w wektorze nazw zmiennych; jeśli nie, to go tam dodajemy
        if(!vectorContains(*variableNames,varName)){
            variableNames->push_back(varName);
        }
    }
}

template <typename T>
void CTree<T>::enter(const vector<string>& formula) {

    // Tworzymy kopię wektora wyrażenia na stercie, aby coraz głębsze węzły mogły pobierać i usuwać następne ciągi znaków z wektora
    vector<string> *expr = new vector<string>;
    *expr = formula;
    bool* flag = new bool();
    *flag = false;

    // Wywołujemy konstruktor przeciążony dla root'a
    root = CNode(expr, varNames, flag);

    if(!expr->empty() && !*flag){
        cout << "ERROR: excess arguments: ";
        printVector(*expr);
        cout << "Parsed the following expression: ";
        print();
    }else if(*flag){
        cout << "ERROR: incorrect expression.\nParsed the following corrected expression: ";
        print();
    }

    delete expr;
}

template <typename T>
T CTree<T>::comp(const vector<string>& args) {
    // Tworzymy mapę zmiennych, gdzie kluczami są nazwy zmiennych, a wartościami są kolejne liczby z wektora args
    map<string, T> vars;
    for(int i=0; i<varNames->size(); i++){
        vars[(*varNames)[i]] = stringToInt(args[i]);
    }
    return root.getValue(vars);
}

template <typename T>
void CTree<T>::print() {
    root.printBelow(); // Drukowanie drzewa preorder, począwszy od root'a
    cout << endl;
}

template <typename T>
void CTree<T>::vars() {
    if(varNames->empty()){
        cout << "This expression contains no variables." << endl;
    }else{
        printVector(*varNames); // Drukujemy wektor zawierający nazwy zmiennych
    }
}

template <typename T>
void CTree<T>::operator=(const CTree<T> &other) {

    root = other.root;

    // Usuwamy dotychczasową listę nazw zmiennych i rezerwujemy miejsce na stercie
    delete varNames;
    varNames = new vector<string>;

    // Głęboka kopia wartości z wektora znajdującego się pod adresem we wskaźniku other.varNames
    *varNames = *other.varNames;
}

template <typename T>
CTree<T> CTree<T>::operator+(const vector<string> &formula) const {

    // Tworzymy kopię pierwszego drzewa
    CTree result;
    result = *this;

    // Tworzymy drugie drzewo
    CTree other;
    other.enter(formula);

    return result+other;
}

template <typename T>
CTree<T> CTree<T>::operator+(const CTree<T> &other) const {
    CTree result;
    result = *this;

    CNode* currNode;
    currNode = &result.root;

    // Jeśli znaleźliśmy Node, który nie ma dzieci, to jest to liść
    while(!currNode->children.empty()){
        currNode = &currNode->children[0];
    }

    // Podstawiamy root nowo utworzonego drzewa za liścia pierwszego drzewa
    *currNode = other.root;

    // Usuwamy dotychczasową listę nazw zmiennych i rezerwujemy miejsce na stercie
    delete result.varNames;
    result.varNames = new vector<string>;

    // Odszukujemy na nowo zmienne w drzewie
    result.root.rediscoverVariables(result.varNames);

    return result;
}



#endif //LIST_3_TREE_STATIC_HPP
