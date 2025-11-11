#include "ListaCircular.h"

// Constructor
ListaCircular::ListaCircular() {
    cabeza = nullptr;
}

// Insertar al final
void ListaCircular::insertarFinal(const Usuario& u) {
    Nodo* nuevo = new Nodo;
    nuevo->dato = u;
    nuevo->siguiente = nullptr;
    nuevo->anterior = nullptr;

    if (cabeza == nullptr) {
        cabeza = nuevo;
        cabeza->siguiente = cabeza;
        cabeza->anterior = cabeza;
        cout << "Usuario insertado correctamente.\n";
        return;
    }

    Nodo* cola = cabeza->anterior;
    cola->siguiente = nuevo;
    nuevo->anterior = cola;
    nuevo->siguiente = cabeza;
    cabeza->anterior = nuevo;

    cout << "Usuario insertado correctamente al final.\n";
}

// Mostrar todos los usuarios
void ListaCircular::mostrar() const {
    if (cabeza == nullptr) {
        cout << "Lista vacía.\n";
        return;
    }

    Nodo* aux = cabeza;
    do {
        aux->dato.mostrarDatos();
        aux = aux->siguiente;
    } while (aux != cabeza);
}

// Buscar usuario por cédula
Nodo* ListaCircular::buscar(const string& cedula) {
    if (cabeza == nullptr) {
        cout << "Lista vacía.\n";
        return nullptr;
    }

    Nodo* aux = cabeza;
    do {
        if (aux->dato.getCedula() == cedula) {
            cout << "Usuario encontrado.\n";
            return aux;
        }
        aux = aux->siguiente;
    } while (aux != cabeza);

    cout << "Usuario no encontrado.\n";
    return nullptr;
}

// Eliminar usuario por cédula
void ListaCircular::eliminarPorCedula(const string& cedula) {
    if (cabeza == nullptr) {
        cout << "Lista vacía, no hay usuarios que eliminar.\n";
        return;
    }

    Nodo* aux = cabeza;
    do {
        if (aux->dato.getCedula() == cedula) {
            if (aux->siguiente == aux) {
                // Solo hay un nodo
                delete aux;
                cabeza = nullptr;
                cout << "Usuario eliminado correctamente.\n";
                return;
            }

            Nodo* anterior = aux->anterior;
            Nodo* siguiente = aux->siguiente;

            anterior->siguiente = siguiente;
            siguiente->anterior = anterior;

            if (aux == cabeza) {
                cabeza = siguiente;
            }

            delete aux;
            cout << "Usuario eliminado correctamente.\n";
            return;
        }
        aux = aux->siguiente;
    } while (aux != cabeza);

    cout << "Usuario no encontrado.\n";
}

