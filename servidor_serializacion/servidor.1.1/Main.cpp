#include <iostream>
#include <string>
#include <sstream>
#include <algorithm> // para find_if

#include "src/modelo/Usuario.h"
#include "src/modelo/Asiento.h"
#include "src/controlador/ListaCircular.h"
#include "src/modelo/FechaHora.h"
#include "includes/Validacion.h"
#include "includes/Include.h"
#include "src/vista/VistaReservarAsiento.h"
#include "src/controlador/ordenamiento.h"
#include "src/controlador/hash.h"
#include "src/modelo/ArbolBusqueda.h"
#include "src/network/Sync.h"
#include "src/network/Server.h"

using namespace std;

int main() {
    VistaReservarAsiento vistaReservar;
    ListaCircular lista;
    HashAsientos h(10);

    const string archivo = "reservas.toon";

    // ====== CARGAR ARCHIVO ======
    lista.cargarDesdeArchivo(archivo);

    // ====== INICIAR SYNC (escucha broadcast de otras instancias) ======
    Sync::iniciar(&lista);

    // ====== INICIAR SERVIDOR TCP (opcional) ======
    // Iniciar servidor central en puerto 60000; si esta máquina es el servidor, habilitar.
    Server::iniciar(60000, archivo);

    // ====== LIMPIAR CADENAS ======
    auto limpiarCadena = [](const string& s) {
        string res = s;
        res.erase(res.begin(), find_if(res.begin(), res.end(),
            [](unsigned char c) { return !isspace(c); }));
        res.erase(find_if(res.rbegin(), res.rend(),
            [](unsigned char c) { return !isspace(c); }).base(), res.end());
        return res;
    };

    // ====== SINCRONIZAR LISTA → HASH → ARBOL ======
    Nodo* aux = lista.getCabeza();
    ArbolBusqueda arbol;

    if (aux) {
        do {
            Usuario u = aux->dato.getUsuario();
            u.setNombre(limpiarCadena(u.getNombre()));
            u.setCedula(limpiarCadena(u.getCedula()));

            Asiento a = aux->dato;
            a.setUsuario(u);
            aux->dato = a;

            h.insertar(a);
            arbol.insertar(a);

            aux = aux->siguiente;
        } while (aux != lista.getCabeza());
    }

    // ====== MENU PRINCIPAL ======
    char opcion;

    do {
        ImprimirArchivo::imprimirArchivo("resources/MenuPrincipal.txt", 696);
        cout << ">> ";
        cin >> opcion;

        if (opcion < '1' || opcion > '8') {
            cout << "Opcion invalida. Intente de nuevo.\n";
            continue;
        }

        switch (opcion) {

        case '1': {
            vistaReservar.vistaReservarAsiento(lista, h, archivo);

            // reconstruir hash y arbol
            h = HashAsientos(10);
            arbol = ArbolBusqueda();
            aux = lista.getCabeza();

            if (aux) {
                do {
                    h.insertar(aux->dato);
                    arbol.insertar(aux->dato);
                    aux = aux->siguiente;
                } while (aux != lista.getCabeza());
            }
            break;
        }

        case '2':
            lista.mostrar();
            break;

        case '3': {
            string cedula;
            cout << "Ingrese la cedula: ";
            cin >> cedula;

            if (!h.existecedula(cedula)) {
                cout << "No existe ninguna reserva con esa cedula.\n";
                break;
            }

            lista.buscarPorCedula(cedula);
            break;
        }

        case '4':
            lista.eliminarPorCedula();
            lista.guardarEnArchivo(archivo);

            h = HashAsientos(10);
            arbol = ArbolBusqueda();
            aux = lista.getCabeza();

            if (aux) {
                do {
                    h.insertar(aux->dato);
                    arbol.insertar(aux->dato);
                    aux = aux->siguiente;
                } while (aux != lista.getCabeza());
            }
            break;

        case '5': {
            Asiento a;
            a.mostrarDisponibilidad(lista);
            break;
        }
        case '6': {
            char op2;
            do {
                cout << "\n--- ORDENAR ASIENTOS ---\n";
                cout << "1. Burbuja\n";
                cout << "2. Insercion\n";
                cout << "3. QuickSort\n";
                cout << "4. Buscar distancia (Busqueda binaria)\n";
                cout << "5. Ordenar letras de cada nombre\n";
                cout << "6. Salir\n";
                cout << "Seleccione una opcion: ";
        
                cin >> op2;
        
                if (op2 < '1' || op2 > '6') {
                    cout << "Opcion invalida.\n";
                    continue;
                }
        
                switch (op2) {
                case '1':
                    ordenamiento::ordenarPorBurbuja(lista);
                    break;
        
                case '2':
                    ordenamiento::ordenarPorInsercion(lista);
                    break;
        
                case '3':
                    ordenamiento::ordenarPorQuickSort(lista);
                    break;
        
                case '4': {   // 🔍 BUSQUEDA BINARIA
                    if (lista.getCabeza() == nullptr) {
                        cout << "No hay asientos registrados.\n";
                        break;
                    }
        
                    int n = 0;
                    Nodo* aux = lista.getCabeza();
                    do {
                        n++;
                        aux = aux->siguiente;
                    } while (aux != lista.getCabeza());
        
                    Asiento* arr = new Asiento[n];
                    aux = lista.getCabeza();
                    for (int i = 0; i < n; i++) {
                        arr[i] = aux->dato;
                        aux = aux->siguiente;
                    }
        
                    sort(arr, arr + n, [](const Asiento& a, const Asiento& b) {
                        return a.getNombreAsiento() < b.getNombreAsiento();
                    });
        
                    string origen, destino;
                    cout << "Ingrese asiento origen (ej: A3): ";
                    cin >> origen;
                    cout << "Ingrese asiento destino (ej: C7): ";
                    cin >> destino;
        
                    transform(origen.begin(), origen.end(), origen.begin(), ::toupper);
                    transform(destino.begin(), destino.end(), destino.begin(), ::toupper);
        
                    Asiento a;
                    int distancia = a.buscarDistanciaBinaria(arr, n, origen, destino);
        
                    if (distancia != -1)
                        cout << "Distancia entre " << origen << " y " << destino
                             << " = " << distancia << endl;
                    else
                        cout << "Uno o ambos asientos no existen.\n";
        
                    delete[] arr;
                    break;
                }
        
                case '5': {
                    int n;
                    cout << "Ingrese cuantas letras iniciales quiere ordenar: ";
                    cin >> n;
                    lista.ordenarLetrasNombres(n);
                    break;
                }
        
                case '6':
                    cout << "Saliendo...\n";
                    break;
                }
        
            } while (op2 != '6');
        
            break;
        }

        

        case '7': {
            string cedula;
            cout << "Ingrese la cedula a buscar en el arbol: ";
            cin >> cedula;
            arbol.buscarYMostrarPorCedula(cedula);
            break;
        }

        case '8':
            cout << "Saliendo del programa...\n";
            break;
        }

    } while (opcion != '8');

    lista.guardarEnArchivo(archivo);
    return 0;
}
