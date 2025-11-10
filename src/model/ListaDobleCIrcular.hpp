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

template <typename T>
void ListaDobleCircular<T>::insertarInicio(T valor){
    Nodo<T>* nuevo = new Nodo<T>(valor);
    nuevo->siguiente = nullptr;
    nuevo->anterior = nullptr;

    if(cabeza == nullptr){
        cabeza = cola = nuevo;
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

template <typename T>
void ListaDobleCircular<T>::eliminarInicio(){
    if(cabeza == nullptr){
        cout<<"lista vacia, no hay nodo que eliminar"<<endl; 
        return; 
    }

    Nodo<T>* aux = cabeza;

    if(cabeza == cola){
        delete cabeza; 
        cabeza = cola = nullptr; 
    }else{
        cabeza = cabeza->siguiente;
        cabeza->anterior = cola;
        cola->siguiente = cabeza;
        delete aux;
    }

    cout<<"nodo eliminado al inicio"<<endl; 
}

template <typename T>
void ListaDobleCircular<T>::buscar(T n){
    Nodo<T>* aux = cabeza; 

    if(aux == nullptr){
        cout<<"lista vacia"<<endl; 
        return; 
    }

    do{
        if(aux->dato == n){
            cout<<"nodo encontrado exitosamente"<<endl; 
            return; 
        }
        aux = aux->siguiente;
    }while(aux != cabeza);

    cout<<"no se encontro el nodo"<<endl; 
}

template <typename T>
void ListaDobleCircular<T>::eliminarLista(T n){
    if(cabeza == nullptr){
        cout<<"lista vacia o nodo no encontrado"<<endl; 
        return; 
    }

    Nodo<T>* aux1 = cabeza;
    Nodo<T>* aux2 = nullptr;

    do{
        if(aux1->dato == n){
            if(aux1 == cabeza && aux1 == cola){
                delete aux1;
                cabeza = cola = nullptr;
            }else if(aux1 == cabeza){
                cabeza = cabeza->siguiente;
                cabeza->anterior = cola;
                cola->siguiente = cabeza;
                delete aux1;
            }else if(aux1 == cola){
                cola = cola->anterior;
                cola->siguiente = cabeza;
                cabeza->anterior = cola;
                delete aux1;
            }else{
                aux1->anterior->siguiente = aux1->siguiente;
                aux1->siguiente->anterior = aux1->anterior;
                delete aux1;
            }

            cout<<"nodo eliminado correctamente"<<endl; 
            return; 
        }

        aux2 = aux1;
        aux1 = aux1->siguiente;
    }while(aux1 != cabeza);

    cout<<"nodo no encontrado"<<endl; 
}

template <typename T>
void ListaDobleCircular<T>::mostrar(){
    if(cabeza == nullptr){
        cout<<"lista vacia"<<endl; 
        return; 
    }

    Nodo<T>* aux = cabeza;
    do{
        cout<<aux->dato<<" -> ";
        aux = aux->siguiente;
    }while(aux != cabeza);

    cout<<" (regreso al inicio)"<<endl; 
}

