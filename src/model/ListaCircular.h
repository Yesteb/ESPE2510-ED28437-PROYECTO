#pragma once
#include <iostream>
#include <string>
#include "Usuario.h"

using namespace std;


struct Nodo {
    Usuario dato;
    Nodo* siguiente;
    Nodo* anterior;
};


class ListaDobleCircular {
private:
    Nodo* cabeza;

public:
    ListaDobleCircular();  

    void insertarFinal(const Usuario& u);
    void mostrar() const;
    Nodo* buscar(const string& cedula);
    void eliminarPorCedula(const string& cedula);
};
