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

bool Util::isInteger(const string& str) {
    // Konwertujemy stringa z argumentu na cString, czyli taki zakończony dodatkowo Null character
    const char* cStr = str.c_str();
    char* endPtr;

    // Korzystamy z metody std::strtol, która konwertuje cString na long; po próbie konwersji endPtr będzie wskazywać na pierwszy znak cStr którego nie udało się zamienić na wartość liczbową; ostatni argmuent strtol oznacza system liczbowy w którym zapisana jest liczba w cStr
    long result = strtol(cStr, &endPtr, 10);

    // Sprawdzamy, czy konwersja zakończyła się na znaku Null, co oznacza, że całość string'a została poprawnie zamieniona na int
    return (endPtr != cStr && *endPtr == '\0');
}

int Util::stringToInt(const std::string& str) {
    // Konwertujemy stringa z argumentu na cString, czyli taki zakończony dodatkowo Null character
    const char* cStr = str.c_str();
    char* endPtr;

    // Korzystamy z metody std::strtol, która konwertuje cString na long; po próbie konwersji endPtr będzie wskazywać na pierwszy znak z cStr którego nie udało się zamienić na wartość liczbową
    long result = strtol(cStr, &endPtr, 10);

    // Sprawdzamy, czy konwersja zakończyła się na znaku Null, co oznacza, że całość string'a została poprawnie zamieniona na int
    if (endPtr != cStr && *endPtr == '\0') {
        // Konwersja udana, zwracamy przekonwertowaną wartość
        return result;
    } else {
        // Konwersja nieudana, zwracamy wartość domyślną
        return 0;
    }
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
