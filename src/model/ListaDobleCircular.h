#pragma once
#include <iostream>
#include <functional>

template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    Nodo<T>* anterior;

    NodoDobleCircular(T valor);
};

template <typename T>
class ListaDobleCircular {
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    size_t tamaño;
public:
    ListaDobleCircular();
    ~ListaDobleCircular();

    void insertarInicio(Nodo<T> *&lista, T valor);
    void insertarFinal(Nodo<T> *&lista, T valor);
    void eliminarInicio(Nodo<T> *&lista);
    void eliminarFinal(Nodo<T> *&lista);
    Nodo<T> buscar(Nodo<T> * lista, T valor);
    void mostrar(Nodo<T>* lista);
    void eliminarLista(Nodo<T> *&lista, T valor);

};

#include "ListaDobleCIrcular.hpp"
