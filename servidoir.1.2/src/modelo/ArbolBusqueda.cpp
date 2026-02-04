#include "ArbolBusqueda.h"
#include "../../includes/Validacion.h"
#include <iostream>

// Constructor
ArbolBusqueda::ArbolBusqueda() : raiz(nullptr) {}

// Funcion recursiva para insertar
NodoArbol* ArbolBusqueda::insertarRec(NodoArbol* nodo, const Asiento& a) {
    Validacion v;

    // Validar cedula antes de insertar
    if (!v.validarCedulaEcuatoriana(a.getUsuario().getCedula())) {
        std::cout << "CEDULA INVALIDA NO SE PUEDE INSERTAR EN EL ARBOL.\n";
        return nodo; // Retorna el nodo sin insertar
    }

    if (!nodo) return new NodoArbol(a);

    if (a.getUsuario().getCedula() < nodo->dato.getUsuario().getCedula())
        nodo->izq = insertarRec(nodo->izq, a);
    else if (a.getUsuario().getCedula() > nodo->dato.getUsuario().getCedula())
        nodo->der = insertarRec(nodo->der, a);

    return nodo;
}

// Funcion publica para insertar
void ArbolBusqueda::insertar(const Asiento& a) {
    raiz = insertarRec(raiz, a);
}

// Funcion recursiva para buscar
NodoArbol* ArbolBusqueda::buscarRec(NodoArbol* nodo, const std::string& cedula) {
    Validacion v;
    if (!v.validarCedulaEcuatoriana(cedula)) return nullptr;

    if (!nodo) return nullptr;
    if (nodo->dato.getUsuario().getCedula() == cedula)
        return nodo;
    if (cedula < nodo->dato.getUsuario().getCedula())
        return buscarRec(nodo->izq, cedula);
    else
        return buscarRec(nodo->der, cedula);
}

// Funcion publica para buscar
NodoArbol* ArbolBusqueda::buscar(const std::string& cedula) {
    return buscarRec(raiz, cedula);
}

// Funcion recursiva para mostrar en orden
void ArbolBusqueda::inOrdenRec(NodoArbol* nodo) {
    if (!nodo) return;
    inOrdenRec(nodo->izq);
    std::cout << nodo->dato.getNombreAsiento() << " - "
              << nodo->dato.getUsuario().getCedula() << std::endl;
    inOrdenRec(nodo->der);
}

// Funcion publica para mostrar
void ArbolBusqueda::mostrarEnOrden() {
    inOrdenRec(raiz);
}

// Destructor (libera memoria)
ArbolBusqueda::~ArbolBusqueda() {
    // Recorrer post-order para liberar nodos
    std::function<void(NodoArbol*)> destruir = [&](NodoArbol* nodo) {
        if (!nodo) return;
        destruir(nodo->izq);
        destruir(nodo->der);
        delete nodo;
    };
    destruir(raiz);
}

void ArbolBusqueda::buscarYMostrarPorCedula(const std::string& cedula) {
    Validacion v;
    if (!v.validarCedulaEcuatoriana(cedula)) {
        std::cout << "CEDULA INVALIDA NO SE PUEDE BUSCAR EN EL ARBOL.\n";
        return;
    }

    NodoArbol* resultado = buscar(cedula); // usa tu funcion buscar existente
    if (resultado) {
        std::cout << "Reserva encontrada en el arbol.\n";
        std::cout << "Usuario: " << resultado->dato.getUsuario().getNombre() << " | "
                  << "Cedula: " << resultado->dato.getUsuario().getCedula() << "\n";
        std::cout << "Asiento reservado: " << resultado->dato.getNombreAsiento()
                  << " | Tipo: " << resultado->dato.getTipoAsiento() << std::endl;
    } else {
        std::cout << "No existe ninguna reserva con esa cedula en el arbol.\n";
    }
}
