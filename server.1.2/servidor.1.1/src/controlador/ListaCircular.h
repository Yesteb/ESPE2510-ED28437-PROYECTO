#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "../modelo/Asiento.h"
#include "../../includes/Validacion.h"
using namespace std;

class ListaCircular;

class Asiento; 
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
    void eliminarPorCedula(); 
    void guardarEnArchivo(const string &nombreArchivo) const;
    void cargarDesdeArchivo(const string &nombreArchivo);
    void ordenarLetrasNombres(int n);

   

};
