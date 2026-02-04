#ifndef ARBOLBUSQUEDA_H
#define ARBOLBUSQUEDA_H

#include "Asiento.h"
#include <iostream>
#include <string>

struct NodoArbol {
    Asiento dato;
    NodoArbol* izq;
    NodoArbol* der;

    NodoArbol(const Asiento& a) : dato(a), izq(nullptr), der(nullptr) {}
};

class ArbolBusqueda {
private:
    NodoArbol* raiz;

    NodoArbol* insertarRec(NodoArbol* nodo, const Asiento& a);
    NodoArbol* buscarRec(NodoArbol* nodo, const std::string& cedula);
    void inOrdenRec(NodoArbol* nodo);

public:
    ArbolBusqueda();
    ~ArbolBusqueda();

    void insertar(const Asiento& a);
    NodoArbol* buscar(const std::string& cedula);
    void mostrarEnOrden();

    // Nueva función para buscar y mostrar la reserva por cédula
    void buscarYMostrarPorCedula(const std::string& cedula);
};

#endif
