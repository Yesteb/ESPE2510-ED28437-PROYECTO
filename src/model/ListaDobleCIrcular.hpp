#pragma once
#include "ListaDobleCircular.h"

template <typename T>
Nodo<T>::Nodo(T valor): dato(valor), siguiente(nullptr), anterior(nullptr){}

template <typename T>
Nodo<T>::~Nodo() {}

template <typename T>
ListaDobleCircular<T>::ListaDobleCircular(): cabeza(nullptr), cola(nullptr), tamaño(0) {}

template <typename T>
ListaDobleCircular<T>::~ListaDobleCircular(){}

template <typename T>
void ListaDobleCircular<T>::insertarInicio(Nodo<T> *& lista, T valor){
    Nodo<T>* nuevo_nodo = new Nodo<T>();
    



}