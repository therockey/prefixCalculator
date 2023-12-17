#include "../include/Util.h"
#include <sstream>
#include <cmath>
#include <iostream>

using namespace std;

vector<string> Util::splitExpression(const string &expression) {
    vector<string> result;

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
    map<string, int>::iterator it = variables->find(key);

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

// Funkcja usuwająca ostatnie wystąpienie podciągu 'second' z ciągu znaków 'first'.
string Util::strSub(string first, const string& second) {
    // Sprawdź, czy podciąg 'second' występuje w ciągu 'first'.
    if (first.find(second) == string::npos)
        return first;  // Jeśli nie, zwróć niezmieniony ciąg 'first'.

    // Znajdź pozycję ostatniego wystąpienia podciągu 'second' w ciągu 'first'.
    size_t pos = first.rfind(second);

    // Usuń podciąg 'second' z ciągu 'first', zaczynając od jego ostatniego wystąpienia.
    first.erase(pos, second.length());

    // Zwróć zmodyfikowany ciąg 'first' po usunięciu ostatniego wystąpienia 'second'.
    return first;
}

// Funkcja, która dla każdego wystąpienia pierwszego znaku z 'second' w ciągu 'first',
// wstawia resztę ciągu 'second' za tym znakiem w ciągu wynikowym.
string Util::strMult(const string& first, const string& second) {
    // Zmienna do przechowywania wynikowego ciągu.
    string result;

    // Pętla przechodząca przez każdy znak ciągu 'first'.
    for (int i = 0; i < first.length(); ++i) {
        // Sprawdź, czy obecny znak w 'first' jest równy pierwszemu znakowi w 'second'.
        if (first[i] == second[0])
            result += second;  // Jeśli tak, dodaj cały ciąg 'second' do wynikowego ciągu.
        else {
            result += first[i];  // Jeśli nie, dodaj obecny znak z 'first' do wynikowego ciągu.
        }
    }

    // Zwróć wynikowy ciąg po przetworzeniu wszystkich znaków w 'first'.
    return result;
}

// Funkcja, która odwraca działanie funkcji 'strMult'.
// Usuwa każde wystąpienie reszty ciągu 'second' po pierwszym znaku z 'second' w ciągu 'first'.
string Util::strDiv(string first, const string& second) {
    size_t pos;

    // Powtarzaj, dopóki znajdowane są kolejne wystąpienia ciągu 'second' w ciągu 'first'.
    while ((pos = first.find(second)) != string::npos) {
        // Usuń fragment ciągu 'first' zaczynając od pozycji po pierwszym znaku z 'second'
        // do końca ciągu 'second'.
        first.erase(pos + 1, second.length() - 1);
    }

    // Zwróć zmodyfikowany ciąg 'first' po usunięciu wszystkich wystąpień ciągu 'second'.
    return first;
}
