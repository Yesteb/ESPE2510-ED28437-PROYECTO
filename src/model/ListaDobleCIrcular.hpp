#pragma once
#include "ListaDobleCircular.h"

template <typename T>
Nodo<T>::Nodo(T valor): dato(valor), siguiente(nullptr), anterior(nullptr) {}

template <typename T>
Nodo<T>::~Nodo() {}

template <typename T>
ListaDobleCircular<T>::ListaDobleCircular(): cabeza(nullptr), cola(nullptr) {}

template <typename T>
ListaDobleCircular<T>::~ListaDobleCircular() {}

// ---------------------------------------------------------
// insertar al inicio
/*
template <typename T>
void ListaDobleCircular<T>::insertarInicio(T valor){
    Nodo<T>* nuevo = new Nodo<T>(valor);
    nuevo->siguiente = nullptr;
    nuevo->anterior = nullptr;

    if(cabeza == nullptr){
        cabeza = nuevo;
        cola = nuevo;
        nuevo->siguiente = nuevo;
        nuevo->anterior = nuevo;
    }else{
        nuevo->siguiente = cabeza;
        nuevo->anterior = cola;
        cabeza->anterior = nuevo;
        cola->siguiente = nuevo;
        cabeza = nuevo;
    }

    cout<<"nodo insertado correctamente"<<endl; 
}
*/
// ---------------------------------------------------------
// insertar al final
template <typename T>
bool ListaDobleCircular<T>::insertarFinal(Nodo<T>*& lista, T valor){
    Nodo<T>* nuevo = new Nodo<T>(valor);
    nuevo->siguiente = nullptr;
    nuevo->anterior = nullptr;

    if(lista == nullptr){
        lista = nuevo;
        lista->siguiente = lista;
        lista->anterior = lista;
        cout<<"nodo insertado al final correctamente"<<endl; 
        return false;
    }

    Nodo<T>* aux = lista;
    while(aux->siguiente != lista && aux->siguiente != nullptr){
        aux = aux->siguiente;
    }

    nuevo->siguiente = lista;
    nuevo->anterior = aux;
    aux->siguiente = nuevo;
    lista->anterior = nuevo;

    cout<<"nodo insertado al final correctamente"<<endl; 
    return true;
}

// ---------------------------------------------------------
// eliminar al inicio
template <typename T>
void ListaDobleCircular<T>::eliminarInicio(Nodo<T>*& lista){
    if(lista == nullptr){
        cout<<"lista vacia, no hay nodo que eliminar"<<endl;
        return;
    }

    Nodo<T>* aux = lista;

    if(lista->siguiente == lista){
        delete lista;
        lista = nullptr;
    }else{
        Nodo<T>* cola = lista->anterior;
        lista = lista->siguiente;
        lista->anterior = cola;
        cola->siguiente = lista;
        delete aux;
    }

    cout<<"nodo eliminado al inicio"<<endl;
}

// ---------------------------------------------------------
// eliminar al final
template <typename T>
void ListaDobleCircular<T>::eliminarFinal(Nodo<T>*& lista){
    if(lista == nullptr){
        cout<<"lista vacia, no hay nodo que eliminar"<<endl;
        return;
    }

    if(lista->siguiente == lista){
        delete lista;
        lista = nullptr;
        cout<<"nodo eliminado al final"<<endl;
        return;
    }

    Nodo<T>* cola = lista->anterior;
    Nodo<T>* nuevo_final = cola->anterior;

    nuevo_final->siguiente = lista;
    lista->anterior = nuevo_final;

    delete cola;

    cout<<"nodo eliminado al final"<<endl;
}

// ---------------------------------------------------------
// buscar un nodo
template <typename T>
Nodo<T> ListaDobleCircular<T>::buscar(Nodo<T>* lista, T valor){
    Nodo<T>* aux = lista;

    if(aux == nullptr){
        cout<<"lista vacia"<<endl;
        return Nodo<T>(-1); // valor dummy para indicar no encontrado
    }

    do{
        if(aux->dato == valor){
            cout<<"nodo encontrado exitosamente"<<endl;
            return *aux;
        }
        aux = aux->siguiente;
    }while(aux != lista);

    cout<<"no se encontro el nodo"<<endl;
    return Nodo<T>(-1);
}

// ---------------------------------------------------------
// mostrar todos los nodos
template <typename T>
void ListaDobleCircular<T>::mostrar(Nodo<T>* lista){
    if(lista == nullptr){
        cout<<"lista vacia"<<endl;
        return;
    }

    Nodo<T>* aux = lista;
    do{
        cout<<aux->dato<<" -> ";
        aux = aux->siguiente;
    }while(aux != lista);

    cout<<" (regreso al inicio)"<<endl;
}

// ---------------------------------------------------------
// eliminar nodo especifico
template <typename T>
void ListaDobleCircular<T>::eliminarLista(Nodo<T>*& lista, T valor){
    if(lista == nullptr){
        cout<<"lista vacia o nodo no encontrado"<<endl;
        return;
    }

    Nodo<T>* aux1 = lista;
    Nodo<T>* aux2 = nullptr;

    do{
        if(aux1->dato == valor){
            if(aux1->siguiente == aux1){
                delete aux1;
                lista = nullptr;
            }else{
                Nodo<T>* anterior = aux1->anterior;
                Nodo<T>* siguiente = aux1->siguiente;

                anterior->siguiente = siguiente;
                siguiente->anterior = anterior;

                if(aux1 == lista){
                    lista = siguiente;
                }

                delete aux1;
            }

            cout<<"nodo eliminado correctamente"<<endl;
            return;
        }

        aux2 = aux1;
        aux1 = aux1->siguiente;
    }while(aux1 != lista);

    cout<<"nodo no encontrado"<<endl;
}
