#include "ListaCircular.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../modelo/Asiento.h"
#include <algorithm>
#include <cctype>
using namespace std;



ListaCircular::ListaCircular() {
    cabeza = nullptr;
   
}

void ListaCircular::insertarFinal(const Asiento& a) {
    Nodo* nuevo = new Nodo{a, nullptr, nullptr};

    if (!cabeza) {
        cabeza = nuevo;
        cabeza->siguiente = cabeza;
        cabeza->anterior = cabeza;
    } else {
        Nodo* cola = cabeza->anterior;
        cola->siguiente = nuevo;
        nuevo->anterior = cola;
        nuevo->siguiente = cabeza;
        cabeza->anterior = nuevo;
    }
}

void ListaCircular::mostrar() const {
    if (!cabeza) {
        cout << "Lista vacia.\n";
        return;
    }
    Nodo* aux = cabeza;
    do {
        cout << "Asiento: " << aux->dato.getNombreAsiento()
             << " | Tipo: " << aux->dato.getTipoAsiento()
             << " | Usuario: " << aux->dato.getUsuario().getNombre()
             << " | Cedula: " << aux->dato.getUsuario().getCedula() << endl;
        aux = aux->siguiente;
    } while (aux != cabeza);
}
void ListaCircular::buscarPorCedula(const string& cedulaInicial) {
    if (!cabeza) {
        cout << "Lista vacia.\n";
        return;
    }

    string cedula = cedulaInicial;
    Validacion v;

    while (true) {
        if (cedula.empty()) {
            cout << "Ingrese la cedula del usuario a buscar: ";
            cin >> cedula;
        }

        bool soloDigitos = true;
        for (char c : cedula) {
            if (!isdigit(c)) {
                soloDigitos = false;
                break;
            }
        }

        if (!soloDigitos) {
            cout << "Error: la cedula solo puede contener numeros positivos sin decimales.\n";
            cedula.clear();
            continue; 
        }

        if (!v.validarCedulaEcuatoriana(cedula)) {
            cout << "Error: cedula ecuatoriana invalida.\n";
            cedula.clear();
            continue; 
        }

        Nodo* aux = cabeza;
        bool encontrado = false;
        bool infoUsuarioMostrada = false;

        do {
            if (aux->dato.getUsuario().getCedula() == cedula) {
                if (!infoUsuarioMostrada) {
                    cout << "\nUsuario: " << aux->dato.getUsuario().getNombre()
                         << " | Cedula: " << aux->dato.getUsuario().getCedula() << endl;
                    infoUsuarioMostrada = true;
                }
                cout << "Asiento reservado: " << aux->dato.getNombreAsiento()
                     << " | Tipo: " << aux->dato.getTipoAsiento() << endl;
                encontrado = true;
            }
            aux = aux->siguiente;
        } while (aux != cabeza);

        if (!encontrado) {
            cout << "No se encontraron asientos reservados con esa cedula.\n";
            cedula.clear(); 
            continue;
        }

        break;
    }
}


void ListaCircular::guardarEnArchivo(const string &nombreArchivo) const {
    ofstream archivo(nombreArchivo, ios::trunc);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir archivo para guardar.\n";
        return;
    }
    if (!cabeza) return;

    Nodo* aux = cabeza;
    do {
        aux->dato.guardarEnArchivo(archivo);
        aux = aux->siguiente;
    } while (aux != cabeza);
}

void ListaCircular::cargarDesdeArchivo(const string &nombreArchivo) {

    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) return;

    string linea;
    while (getline(archivo, linea)) {
        Asiento a;
        a.cargarDesdeArchivo(linea);
        insertarFinal(a);
    }
    archivo.close();
}

void ListaCircular::eliminarPorCedula() {
    string cedula;
    
    do {
        cout << "Ingrese la cedula: ";
        cin >> cedula;
        if (!Validacion::validar(cedula, "cedula"))
            cout << "Cedula no valida.\n";
    } while (!Validacion::validar(cedula, "cedula"));

    if (!cabeza) {
        cout << "Lista vacia.\n";
        return;
    }

    
    Nodo* aux = cabeza;
    int contador = 0;
    do {
        if (aux->dato.getUsuario().getCedula() == cedula) {
            cout << static_cast<char>('a' + contador) << ") Asiento: "
                 << aux->dato.getNombreAsiento() << " | Tipo: "
                 << aux->dato.getTipoAsiento() << " | Usuario: "
                 << aux->dato.getUsuario().getNombre() << endl;
            contador++;
        }
        aux = aux->siguiente;
    } while (aux != cabeza);

    if (!contador) {
        cout << "No se encontraron reservas con esa cedula.\n";
        return;
    }

    
    char opcion;
    cout << "Ingrese la letra de la reserva a eliminar (x para cancelar): ";
    cin >> opcion;
    if (opcion == 'x' || opcion == 'X') return;

    int indice = opcion - 'a';
    if (indice < 0 || indice >= contador) {
        cout << "Opcion invalida.\n";
        return;
    }

    aux = cabeza;
    Nodo* anterior = nullptr;
    int actual = 0;
    do {
        if (aux->dato.getUsuario().getCedula() == cedula) {
            if (actual == indice) {
                Nodo* sig = aux->siguiente;
                Nodo* ant = aux->anterior;
                if (aux == cabeza && aux->siguiente == cabeza) {
                    delete aux;
                    cabeza = nullptr;
                } else {
                    ant->siguiente = sig;
                    sig->anterior = ant;
                    if (aux == cabeza) cabeza = sig;
                    delete aux;
                }
                cout << "Reserva eliminada correctamente.\n";
                guardarEnArchivo("reservas.txt"); 
                return;
            }
            actual++;
        }
        aux = aux->siguiente;
    } while (aux != cabeza);
}



/*---------------------------------------------------------------------------------------------------------------------------*/
void ListaCircular::ordenarLetrasNombres(int n) {
    if (!cabeza) {
        cout << "Lista vacía.\n";
        return;
    }

    // Lambda para ordenar solo las primeras n letras
    auto ordenarLetrasParcial = [](string &s, int n) {
        if (n > s.length()) n = s.length();

        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (s[j] > s[j + 1]) {
                    char temp = s[j];
                    s[j] = s[j + 1];
                    s[j + 1] = temp;
                }
            }
        }
    };

    Nodo* aux = cabeza;
    cout << "\nNombres con letras ordenadas:\n";

    do {
        string nombreCompleto = aux->dato.getUsuario().getNombre();

        string primerNombre = nombreCompleto.substr(0, nombreCompleto.find(' '));

        for (char &c : primerNombre) c = tolower(c);

        ordenarLetrasParcial(primerNombre, n);

        cout << primerNombre << endl;

        aux = aux->siguiente;
    } while (aux != cabeza);
}




//hacer nuedtro propio sort 
//consultar la importan y poeque visual sutido genera un  reat me
//crear un dll para limpiar la memoria
//verificar  el campo clave 
/*ordenar un nombre ANITA y parametrozar toda la estructura y ordenar el nnombre y el numero de caracteres a ordenar, ejemplo si
envio 3 solo debeb ordenar ani los otros dos se deben quedar estaticos*/
//investigar cunado hay que ordenar una mayuscyla y minuscula como se comporta el algoritmo de ordenamiento
//leer hasg y busquedas binarias y busquedas secuenciales(opcion) mas busquedas binarias