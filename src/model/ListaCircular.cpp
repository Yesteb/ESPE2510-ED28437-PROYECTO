#include "ListaCircular.h"
#include <iostream>
using namespace std;

// Constructor
ListaCircular::ListaCircular() {
    cabeza = nullptr;
}

// Insertar al final
void ListaCircular::insertarFinal(const Asiento& a) {
    Nodo* nuevo = new Nodo;
    nuevo->dato = a;

    if (cabeza == nullptr) {
        cabeza = nuevo;
        cabeza->siguiente = cabeza;
        cabeza->anterior = cabeza;
        cout << "Asiento insertado correctamente.\n";
        return;
    }

    Nodo* cola = cabeza->anterior;
    cola->siguiente = nuevo;
    nuevo->anterior = cola;
    nuevo->siguiente = cabeza;
    cabeza->anterior = nuevo;

    cout << "Asiento insertado correctamente al final.\n";
}

// Mostrar todos los asientos
void ListaCircular::mostrar() const {
    if (cabeza == nullptr) {
        cout << "Lista vacia.\n";
        return;
    }

    Nodo* aux = cabeza;
    do {
        cout << "Asiento: " << aux->dato.getNombreAsiento()
             << " | Tipo: " << aux->dato.getTipoAsiento()
             << " | Usuario: " << aux->dato.getUsuario().getNombre()
             << " | Cedula: " << aux->dato.getUsuario().getCedula()
             << endl;
        aux = aux->siguiente;
    } while (aux != cabeza);
}

// Buscar todos los asientos por cedula
void ListaCircular::buscarPorCedula(const string& cedula) {
    if (cabeza == nullptr) {
        cout << "Lista vacia.\n";
        return;
    }

    Nodo* aux = cabeza;
    bool encontrado = false;
    do {
        if (aux->dato.getUsuario().getCedula() == cedula) {
            cout << "Asiento: " << aux->dato.getNombreAsiento()
                 << " | Tipo: " << aux->dato.getTipoAsiento()
                 << " | Usuario: " << aux->dato.getUsuario().getNombre()
                 << " | Cedula: " << aux->dato.getUsuario().getCedula()
                 << endl;
            encontrado = true;
        }
        aux = aux->siguiente;
    } while (aux != cabeza);

    if (!encontrado) {
        cout << "No se encontraron asientos reservados con esa cedula.\n";
    }
}

// Eliminar todos los asientos por cedula
void ListaCircular::eliminarPorCedula(const string& cedula) {
    if (cabeza == nullptr) {
        cout << "Lista vacia, no hay asientos que eliminar.\n";
        return;
    }

    Nodo* aux = cabeza;
    bool eliminado = false;

    do {
        Nodo* siguiente = aux->siguiente; // guardamos siguiente antes de eliminar
        if (aux->dato.getUsuario().getCedula() == cedula) {
            if (aux->siguiente == aux) {
                // Solo hay un nodo
                delete aux;
                cabeza = nullptr;
                cout << "Todos los asientos de esa cedula eliminados.\n";
                return;
            }

            Nodo* anterior = aux->anterior;
            anterior->siguiente = siguiente;
            siguiente->anterior = anterior;

            if (aux == cabeza) {
                cabeza = siguiente;
            }

            delete aux;
            eliminado = true;
        }
        aux = siguiente;
    } while (aux != cabeza);

    if (eliminado) {
        cout << "Todos los asientos de esa cedula eliminados.\n";
    } else {
        cout << "No se encontraron asientos con esa cedula.\n";
    }
}
