#ifndef LIST_3_TREE_STATIC_HPP
#define LIST_3_TREE_STATIC_HPP

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
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
    static bool isValue(const string& str, T &val);

    CNode root;
    vector<string>* varNames;
public:
    CTree();
    CTree(const CTree<T>& other);
    CTree(CTree<T>&& other) noexcept;
    ~CTree();
    int getArgCount();
    void enter(const vector<string>& formula);
    T comp(const vector<string>& args);
    void print();
    void vars();

    CTree& operator=(const CTree<T>& other);
    CTree& operator=(CTree<T>&& other) noexcept;
    CTree<T> operator+(const CTree<T>& other) const&;
    CTree<T> operator+(const CTree<T>& other) &&;

};

template<typename T>
bool CTree<T>::isValue(const string &str, T &val) {
    return false;
}

template<>
bool CTree<int>::isValue(const string &str, int &val) {
    // Konwertujemy stringa z argumentu na cString, czyli taki zakończony dodatkowo Null character
    const char* cStr = str.c_str();
    char* endPtr;

    // Korzystamy z metody std::strtol, która konwertuje cString na long; po próbie konwersji endPtr będzie wskazywać na pierwszy znak z cStr którego nie udało się zamienić na wartość liczbową
    long result = strtol(cStr, &endPtr, 10);

    // Sprawdzamy, czy konwersja zakończyła się na znaku Null, co oznacza, że całość string'a została poprawnie zamieniona na int
    if (endPtr != cStr && *endPtr == '\0') {
        // Konwersja udana, ustawiamy val na wynik i zwracamy TRUE
        val = result;
        return true;
    }
    // Konwersja nieudana, ustawiamy val na 0 i zwracamy FALSE
    val = 0;
    return false;

}

template<>
bool CTree<double>::isValue(const string &str, double &val) {
    // Konwertujemy stringa z argumentu na cString, czyli taki zakończony dodatkowo Null character
    const char* cStr = str.c_str();
    char* endPtr;

    // Korzystamy z metody std::strtod, która konwertuje cString na double; po próbie konwersji endPtr będzie wskazywać na pierwszy znak z cStr którego nie udało się zamienić na wartość liczbową
    double result  = strtod(cStr, &endPtr);

    // Sprawdzamy, czy konwersja zakończyła się na końcu stringa, jeśli tak, to konwersja odbyła się poprawnie
    if (endPtr == cStr + str.size()) {
        // Konwersja udana, ustawiamy val na wynik i zwracamy TRUE
        val = result;
        return true;

    }
    // Konwersja nieudana, ustawiamy val na 0 i zwracamy FALSE
    val = 0;
    return false;
}

template<>
bool CTree<string>::isValue(const std::string &str, string& val) {
    if(
            str != "+" &&
            str != "-" &&
            str != "*" &&
            str != "/" &&
            str != "sin" &&
            str != "cos" &&
            str[0] == '\"' &&
            str[str.length()-1] == '\"'
            ) {
        val = str;
        return true;
    }
    val = "";
    return false;
}

template <typename T>
CTree<T>::CTree(){
    varNames = new vector<string>;
}

// Konstruktor kopiujący
template <typename T>
CTree<T>::CTree(const CTree<T>& other){
    root=other.root;
    varNames = new vector<string>;

    // Tworzymy głęboką kopię wartości w wektorze
    *varNames = *other.varNames;
}

// Konstruktor przenoszący
template <typename T>
CTree<T>::CTree(CTree<T>&& other) noexcept {
    root=other.root;
    varNames = other.varNames;

    // Zmieniamy wskaźnik z drugim drzewie, aby jego konstruktor nie zdealokował pamięci przepisanej do drzewa, w którym się znajdujemy
    other.varNames = nullptr;
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
        case 0: return value; // Węzeł jest wartością
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

template<>
string CTree<string>::CNode::getValue(map<std::string,string> &variables) {
    switch(type) {
        case 0: return value.substr(1,value.length()-2); // Węzeł jest wartością
        case 1: return children[0].getValue(variables)+children[1].getValue(variables); //Węzeł jest operacją +
        case 2: return strSub(children[0].getValue(variables),children[1].getValue(variables)); //Węzeł jest operacją -
        case 3: return strMult(children[0].getValue(variables),children[1].getValue(variables)); //Węzeł jest operacją *
        case 4: return strDiv(children[0].getValue(variables),children[1].getValue(variables)); //Węzeł jest operacją /
        case 7: return variables[varName]; //węzeł jest zmienną
    }
    return "";
}

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

        // Sprawdzam, czy aktualny ciąg znaków jest liczbą, jeśli jest, to przypisz typ 0 do węzła i wartość liczbową z 'eval' to pola value
        if (isValue(eval,value)) {
            type = 0;
        } else if (eval =="+") { // Jeśli 'eval' to nie liczba, to sprawdź, czy nie jest jedną z możliwych operacji (jeśli tak to przypisz odpowiedni typ do węzła i utwórz odpowiednią liczbę dzieci)
            type = 1;
            children.push_back(CNode(expr, variableNames, flag));
            children.push_back(CNode(expr, variableNames, flag));
        } else if (eval == "-") {
            type = 2;
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
    varNames->clear();

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
        isValue(args[i], vars[(*varNames)[varNames->size()-i-1]]);
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
CTree<T>& CTree<T>::operator=(const CTree<T> &other) {
    if(this!=&other) {
        root = other.root;

        // Usuwamy dotychczasową listę nazw zmiennych i rezerwujemy miejsce na stercie
        delete varNames;
        varNames = new vector<string>;

        // Głęboka kopia wartości z wektora znajdującego się pod adresem we wskaźniku other.varNames
        *varNames = *other.varNames;
    }
    return *this;
}


// Operator przypisania dla argumentu będącego wartością tymczasową
template <typename T>
CTree<T>& CTree<T>::operator=(CTree<T>&& other) noexcept {
    if(this!=&other){

        // Przepisujemy roota z drugiego drzewa
        root=other.root;

        // Usuwamy dotychczasowy vector nazw zmiennych i przepisujemy go z drugiego drzewa
        delete varNames;
        varNames=other.varNames;

        // Zmieniamy wskaźnik w drugim drzewie tak, aby jego destruktor nie usunął vectora w "this"
        other.varNames=nullptr;
    }
    return *this;
}

template <typename T>
CTree<T> CTree<T>::operator+(const CTree<T> &other) const& {

    // Tworzymy drzewo reprezentujące wynik, korzystając z konstruktora kopiującego
    CTree result(*this);

    // Rozpoczynamy poszukiwanie liścia od roota
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

    // Zwracamy wynik poprzez wartość
    return move(result);
}

// Operator dodawania, dla przypadku gdy został wywołany w drzewie będącym wartością tymczasową
template <typename T>
CTree<T> CTree<T>::operator+(const CTree<T>& other) &&{

    // Tworzymy drzewo reprezentujące wynik, korzystając z konstruktora przenoszącego, ponieważ funkcja została wywołana w wartości tymczasowej
    CTree result(move(*this));

    // Rozpoczynamy poszukiwanie liścia od roota
    CNode* currNode;
    currNode= &result.root;

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

    // Zwracamy wynik poprzez wartość
    return move(result);
}



#endif //LIST_3_TREE_STATIC_HPP
