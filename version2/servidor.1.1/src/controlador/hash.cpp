#include "hash.h"
#include <iostream>
#include <string>
#include "../../includes/Validacion.h"
using namespace std;

HashAsientos::HashAsientos(int tam) {
    tamanio = tam;
    inicio = nullptr;

    for (int i = 0; i < tamanio; i++) {
        cubeta* nueva = new cubeta;
        nueva->indice = i;
        nueva->primero = nullptr;
        nueva->sig = inicio;
        inicio = nueva;
    }
}

/* ================= DESTRUCTOR ================= */
HashAsientos::~HashAsientos() {
    cubeta* c = inicio;
    while (c) {
        nodohash* n = c->primero;
        while (n) {
            nodohash* borrar = n;
            n = n->sig;
            delete borrar;
        }
        cubeta* borrarC = c;
        c = c->sig;
        delete borrarC;
    }
}

/* ================= FUNCION HASH ================= */
int HashAsientos::funcionhash(const string& clave) const {
    int suma = 0;
    for (char c : clave)
        suma += c;
    return suma % tamanio;
}

/* ================= OBTENER CUBETA ================= */
cubeta* HashAsientos::obtenercubeta(int indice) {
    cubeta* aux = inicio;
    while (aux) {
        if (aux->indice == indice)
            return aux;
        aux = aux->sig;
    }
    return nullptr;
}
/* ================= INSERTAR ================= */
void HashAsientos::insertar(const Asiento& a) {
    Validacion v;

    // Validar cédula antes de insertar
    if (!v.validarCedulaEcuatoriana(a.getUsuario().getCedula())) {
        std::cout << "CEDULA INVALIDA. NO SE PUEDE INSERTAR EL ASIENTO.\n";
        return;
    }

    int idx = funcionhash(a.getUsuario().getCedula());
    cubeta* c = obtenercubeta(idx);
    if (!c) return;

    nodohash* nuevo = new nodohash;
    nuevo->dato = a;
    nuevo->sig = c->primero;
    nuevo->ant = nullptr;

    if (c->primero)
        c->primero->ant = nuevo;

    c->primero = nuevo;
}

/* ================= EXISTE CEDULA ================= */
bool HashAsientos::existecedula(const std::string& cedula) {
    Validacion v;
    if (!v.validarCedulaEcuatoriana(cedula)) return false;

    int idx = funcionhash(cedula);
    cubeta* c = obtenercubeta(idx);
    if (!c) return false;

    nodohash* n = c->primero;
    while (n) {
        if (n->dato.getUsuario().getCedula() == cedula)
            return true;
        n = n->sig;
    }
    return false;
}

/* ================= ELIMINAR POR CEDULA ================= */
void HashAsientos::eliminarporcedula(const std::string& cedula) {
    Validacion v;
    if (!v.validarCedulaEcuatoriana(cedula)) {
        std::cout << "CEDULA INVALIDA. NO SE PUEDE ELIMINAR.\n";
        return;
    }

    int idx = funcionhash(cedula);
    cubeta* c = obtenercubeta(idx);
    if (!c) return;

    nodohash* n = c->primero;
    while (n) {
        if (n->dato.getUsuario().getCedula() == cedula) {
            if (n->ant)
                n->ant->sig = n->sig;
            else
                c->primero = n->sig;

            if (n->sig)
                n->sig->ant = n->ant;

            nodohash* borrar = n;
            n = n->sig;
            delete borrar;
        } else {
            n = n->sig;
        }
    }
}
