#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include "../include/CTree.h"
#include "../include/Util.h"

using namespace std;
using namespace Util;

CTree::CTree(){
    varNames = new vector<string>;
}

CTree::~CTree() {
    delete varNames;
}

int CTree::getArgCount(){
    return varNames->size();
}

CTree::CNode::CNode() : type(), value(){}

double CTree::CNode::getValue(map<string,int>& variables) {
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

CTree::CNode::CNode(vector<string>* expr, vector<string>* variableNames) {

    // Pobieramy następny ciąg znaków i usuwamy go z wektora wyrażenia
    string eval = expr->back();
    expr->pop_back();

    // Sprawdzam, czy aktualny ciąg znaków jest liczbą, jeśli jest to przypisz typ 0 do węzła i wartość liczbową z 'eval' to pola value
    if(isInteger(eval)){
        type=0;
        value = stringToInt(eval);
    }else if(eval=="+"){ // Jeśli 'eval' to nie liczba, to sprawdź, czy nie jest jedną z możliwych operacji (jeśli tak to przypisz odpowiedni typ do węzła i utwórz odpowiednią liczbę dzieci)
        type=1;
        children.push_back(CNode(expr, variableNames));
        children.push_back(CNode(expr, variableNames));
    }else if(eval=="-"){
        type=2;;
        children.push_back(CNode(expr, variableNames));
        children.push_back(CNode(expr, variableNames));
    }else if(eval=="*"){
        type=3;
        children.push_back(CNode(expr, variableNames));
        children.push_back(CNode(expr, variableNames));
    }else if(eval=="/"){
        type=4;
        children.push_back(CNode(expr, variableNames));
        children.push_back(CNode(expr, variableNames));
    }else if(eval=="sin"){
        type=5;
        children.push_back(CNode(expr, variableNames));
    }else if(eval=="cos"){
        type=6;
        children.push_back(CNode(expr, variableNames));
    }else{ // Jeśli 'eval' to nie liczba i nie operacja, to potraktuj jako zmienną
        type=7;
        varName = eval; // Przechowujemy nazwę zmiennej w Węźle

        // Sprawdzamy, czy nazwa nie występuje już w wektorze nazw zmiennych; jeśli nie, to go tam dodajemy
        if(!vectorContains(*variableNames, eval)){
            variableNames->push_back(eval);
        }
    }
}

void CTree::CNode::printBelow() { //drukowanie węzłów w kolejności preorder, aby uzyskać odpowiednią kolejność znaków w wyrażeniu

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

void CTree::CNode::rediscoverVariables(vector<string> *variableNames) {

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

void CTree::enter(const vector<string>& formula) {

    // Tworzymy kopię wektora wyrażenia na stercie, aby coraz głębsze węzły mogły pobierać i usuwać następne ciągi znaków z wektora
    vector<string> *expr = new vector<string>;
    *expr = formula;

    // Wywołujemy konstruktor przeciążony dla root'a
    root = CNode(expr, varNames);
}

double CTree::comp(const vector<string>& args) {
    map<string, int> vars;
    for(int i=0; i<varNames->size(); i++){
        vars[(*varNames)[i]] = stringToInt(args[i]);
    }
    return root.getValue(vars);
}

void CTree::print() {
    root.printBelow(); // Drukowanie drzewa preorder, począwszy od root'a
    cout << endl;
}

void CTree::vars() {
    if(varNames->empty()){
        cout << "This expression contains no variables." << endl;
    }else{
        printVector(*varNames); // Drukujemy wektor zawierający nazwy zmiennych
    }
}

void CTree::operator=(const CTree &other) {

    root = other.root;

    // Usuwamy dotychczasową listę nazw zmiennych i rezerwujemy miejsce na stercie
    delete varNames;
    varNames = new vector<string>;

    // Głęboka kopia wartości z wektora znajdującego się pod adresem we wskaźniku other.varNames
    *varNames = *other.varNames;
}

CTree CTree::operator+(const vector<string> &formula) {

    // Tworzymy kopię pierwszego drzewa
    CTree result;
    result = *this;

    // Tworzymy drugie drzewo
    CTree other;
    other.enter(formula);

    // Zaczynamy poszukiwanie liścia od roota w kopii pierwszego drzewa
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



