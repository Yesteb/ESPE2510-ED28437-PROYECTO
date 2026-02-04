#ifndef ORDENAMIENTOS_H
#define ORDENAMIENTOS_H

#include "../modelo/Asiento.h"
#include "../controlador/ListaCircular.h"

class ordenamiento {
public:
    static void ordenarPorInsercion(ListaCircular& lista);
    static void ordenarPorBurbuja(ListaCircular& lista);
    static void ordenarPorQuickSort(ListaCircular& lista);

private:
    static Nodo* partition(Nodo* low, Nodo* high);
    static void quickSortRec(Nodo* low, Nodo* high);
};

#endif
