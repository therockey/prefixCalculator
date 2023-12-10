#include "../include/Util.h"
#include <sstream>
#include <cmath>
#include <iostream>

using namespace std;

vector<string> Util::splitExpression(const string &expression) {
    vector<std::string> result;

    // Utworzenie strumienia formatującego dane ze stringa
    istringstream iss(expression);

    // Tymczasowa zmienna potrzebna, aby przechować następne ciągi znaków
    string token;

    // Odczytaj każdy znak ze strumienia wejścia i dodaj do wektora
    while (iss >> token) {
        result.push_back(token);
    }

    // Odwracamy kolejność elementów w wektorze, aby móc je potem szybko usuwać metodą pop_back();
    if (!result.empty()) {
        // Zamiana elementów za pomocą std::swap
        for (size_t i = 0, j = result.size() - 1; i < j; ++i, --j) {
            swap(result[i], result[j]);
        }
    }

    return result;
}


bool Util::mapContains(map<string,int>* variables, const string& key) {

    // Sprawdź, czy zmienna "key" występuje w mapie zmiennych
    map<std::string, int>::iterator it = variables->find(key);

    // Iterator map::find nie zakończył pracy poza końcem mapy, zatem klucz występuje w mapie
    if (it != variables->end()) {
        return true;
    }
    return false;

}

bool Util::vectorContains(const vector<string>& varNames, const string& key){
    // Iterujemy po elementach wektora, sprawdzając, czy wartość elementu jest równa poszukiwanemu kluczowi; jeśli jest, zwróć TRUE
    for(int i=0; i<varNames.size(); i++){
        if(varNames[i] == key){
            return true;
        }
    }
    return false;
}

void Util::printVector(const vector<string>& elements){
    // Iterujemy po elementach wektora, drukując każdy z nich wraz ze spacją odstępu przed następnym
    for (int i = 0; i < elements.size(); i++) {
        cout << elements[i] << " ";
    }
    cout << endl;
}

string Util::strSub(string first, const string& second){
    if (first.find(second) == std::string::npos)
        return first;

    first.erase(first.rfind(second), second.length());
    return first;
}

string Util::strMult(const string& first, const string& second) {
    std::string result;
    int counter = 0;
    for (int i = 0; i < first.length(); ++i) {
        if (first[i] == second[0])
            result += second;
        else {
            result += first[i];
        }
    }
    return result;
}

string Util::strDiv(string first, const string& second){
    size_t pos;

    while ((pos = first.find(second)) != string::npos) {
        first.erase(pos+1, second.length()-1);
    }

    return first;
}
