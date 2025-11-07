#pragma once

template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    Nodo<T>* anterior;

    NodoDobleCircular(const T& valor) : dato(valor), siguiente(nullptr), anterior(nullptr) {}
};

template <typename T>
class ListaDobleCircular {
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    size_t tamaño;
public:
    ListaDobleCircular() : cabeza(nullptr), cola(nullptr), tamaño(0) {}
    ~ListaDobleCircular() {
        while (tamaño > 0) {
            eliminarInicio();
        }
    }

    void insertarInicio(Nodo<T> *&lista, T valor);
    void insertarFinal(Nodo<T> *&lista, T valor);
    void eliminarInicio(Nodo<T> *&lista);
    void eliminarFinal(Nodo<T> *&lista);
    bool buscar(Nodo<T> * lista, T valor);
    void mostrar(Nodo<T>* lista);
    void eliminarLista(Nodo<T> *&lista, T valor);

};
