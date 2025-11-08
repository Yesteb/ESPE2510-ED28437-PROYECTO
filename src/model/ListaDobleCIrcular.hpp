#pragma once
#include "ListaDobleCircular.h"

template <typename T>
Nodo<T>::Nodo(T valor): dato(valor), siguiente(nullptr), anterior(nullptr){}

template <typename T>
Nodo<T>::~Nodo() {}

template <typename T>
ListaDobleCircular<T>::ListaDobleCircular(): cabeza(nullptr), cola(nullptr){}

template <typename T>
ListaDobleCircular<T>::~ListaDobleCircular(){}

template <typename T>
void ListaDobleCircular<T>::insertarInicio(T valor){
    Nodo<T>* nuevo_nodo = new Nodo<T>(valor);

    if(cabeza == nullptr){
        cola = cabeza = nuevo_nodo;
        nuevo_nodo->siguiente = nuevo_nodo;
        nuevo_nodo->anterior = nuevo_nodo;
    } else {
        nuevo_nodo->siguiente = cabeza;
        nuevo_nodo->anterior = cola;
        cabeza->anterior = nuevo_nodo;
        cola->siguiente = nuevo_nodo;
        cabeza = nuevo_nodo;
    }


    
}
