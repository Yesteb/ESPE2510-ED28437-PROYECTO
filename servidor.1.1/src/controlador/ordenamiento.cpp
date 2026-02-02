#include "ordenamiento.h"
#include <iostream>
using namespace std;

/* ======================================================
 *  ORDENAMIENTO POR INSERCIÓN (INSERTION SORT)
 * ====================================================== */

void ordenamiento::ordenarPorInsercion(ListaCircular& lista) {
    Nodo* cabeza = lista.getCabeza();
    if (!cabeza || cabeza->siguiente == cabeza) {
        cout << "No se puede ordenar lista vacia o con un solo elemento.\n";
        return;
    }

    Nodo* actual = cabeza->siguiente;

    while (actual != cabeza) {
        Nodo* temp = actual;
        Nodo* anterior = actual->anterior;

        while (anterior != cabeza->anterior &&
               temp->dato.getNombreAsiento() < anterior->dato.getNombreAsiento()) {

            swap(temp->dato, anterior->dato);
            temp = anterior;
            anterior = anterior->anterior;
        }
        actual = actual->siguiente;
    }

    cout << "\n Lista ordenada por insercion:\n";
    lista.mostrar();
}

/* ======================================================
 *  ORDENAMIENTO BURBUJA (BUBBLE SORT)
 * ====================================================== */
void ordenamiento::ordenarPorBurbuja(ListaCircular& lista) {
    Nodo* cabeza = lista.getCabeza();
    if (!cabeza || cabeza->siguiente == cabeza) {
        cout << "No se puede ordenar lista vacia o con un solo elemento.\n";
        return;
    }

    bool cambio;
    Nodo* fin = cabeza->anterior;  

    do {
        cambio = false;
        Nodo* actual = cabeza;

        while (actual != fin) {
            Nodo* sig = actual->siguiente;

            if (actual->dato.getNombreAsiento() > sig->dato.getNombreAsiento()) {
                swap(actual->dato, sig->dato);
                cambio = true;
            }
            actual = sig;
        }

        fin = fin->anterior;  // reducir el rango de comparación en cada pasada

    } while (cambio);

    cout << "\n lista ordenada por burbuja:\n";
    lista.mostrar();
}

/* ======================================================
 *  FUNCIONES AUXILIARES PARA QUICKSORT
 * ====================================================== */
Nodo* ordenamiento::partition(Nodo* low, Nodo* high) {
    string pivot = high->dato.getNombreAsiento();
    Nodo* i = low->anterior;

    for (Nodo* j = low; j != high; j = j->siguiente) {
        if (j->dato.getNombreAsiento() <= pivot) {
            i = (i == nullptr ? low : i->siguiente);
            swap(i->dato, j->dato);
        }
    }

    i = (i == nullptr ? low : i->siguiente);
    swap(i->dato, high->dato);
    return i;
}

void ordenamiento::quickSortRec(Nodo* low, Nodo* high) {
    if (high != nullptr && low != high && low != high->siguiente) {
        Nodo* pivot = partition(low, high);
        quickSortRec(low, pivot->anterior);
        quickSortRec(pivot->siguiente, high);
    }
}

/* ======================================================
 *  QUICKSORT PRINCIPAL
 * ====================================================== */
void ordenamiento::ordenarPorQuickSort(ListaCircular& lista) {
    Nodo* cabeza = lista.getCabeza();
    if (!cabeza || cabeza->siguiente == cabeza) {
        cout << "No se puede ordenar lista vacia o con un solo elemento.\n";
        return;
    }

    Nodo* fin = cabeza->anterior;
    quickSortRec(cabeza, fin);

    cout << "\n L¿lista ordenada por quickshort:\n";
    lista.mostrar();
}


