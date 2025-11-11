#pragma once
#include <iostream>
#include <string>
#include "Asiento.h"

using namespace std;

struct Nodo {
    Asiento dato;
    Nodo* siguiente;
    Nodo* anterior;
};

class ListaCircular {
private:
    Nodo* cabeza;

public:
    ListaCircular();  
    Nodo* getCabeza() const { return cabeza; }

    void insertarFinal(const Asiento& a);
    void mostrar() const;

    void buscarPorCedula(const string& cedula);

    void eliminarPorCedula(const string& cedula);

    Nodo* buscar(const string& nombreAsiento); 
    void eliminarPorNombre(const string& nombreAsiento);
};
