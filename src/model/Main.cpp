#include <iostream>
#include <string>
#include <sstream>
#include "Usuario.h"
#include "Asiento.h"
#include "ListaCircular.h"
#include "FechaHora.h"
#include "../../includes/Include.h"
using namespace std;



int main() {
    ListaCircular lista;   
    int opcion;
    string fechaEvento;

      do {
        cout << "--- Reservar Fecha del Evento ---\n";
        cout << "Ingrese la fecha del evento (DD/MM/AAAA): ";
        cin >> fechaEvento;

        
        if (!Validacion::validarFecha(fechaEvento)) {
            cout << "Fecha invalida, ontente nuevamente.\n";
        }

    } while (!Validacion::validarFecha(fechaEvento));

    


    cout << "Fecha del evento registrada: " << fechaEvento << endl;

    do {
        /*
        cout << "\n--- Menu de Reservas de Asientos ---\n";
        cout << "1. Reservar asiento\n";
        cout << "2. Mostrar todos los asientos reservados\n";
        cout << "3. Buscar asientos por cedula\n";
        cout << "4. Eliminar asientos por cedula\n";
        cout << "5. Mostrar disponibilidad de asientos\n";
        cout << "6. Salir\n";
        cout << "Ingrese opcion: ";*/
        ImprimirArchivo::imprimirArchivo("../resources/MenuPrincipal.txt", 654);
        cin >> opcion;

        switch(opcion) {
            case 1: {
                Usuario u;
                u.ingresarDatos();

                int cantidad;
                cout << "Cuantos asientos desea reservar? (maximo 10): ";
                cin >> cantidad;
                if(cantidad < 1 || cantidad > 10) {
                    cout << "Cantidad invalida.\n";
                    break;
                }

                for(int i = 0; i < cantidad; i++) {
                    Asiento a;
                    string nombreAsiento, tipo;

                    Asiento::mostrarDisponibilidad();

                    cout << "Ingrese nombre del asiento #" << (i+1) << " (Ej: A1): ";
                    cin >> nombreAsiento;

                    cout << "Ingrese tipo de asiento (General/Palco/Tribuna): ";
                    cin >> tipo;

                    if(a.reservarAsiento(tipo, u, nombreAsiento)) {
                        lista.insertarFinal(a);
                    } else {
                        cout << "No se pudo reservar el asiento #" << (i+1) << ".\n";
                    }
                }
                break;
            }

            case 2:
                lista.mostrar();
                break;

            case 3: {
                string cedula;
                cout << "Ingrese la cedula para buscar asientos: ";
                cin >> cedula;
                lista.buscarPorCedula(cedula);
                break;
            }

            case 4: {
                string cedula;
                cout << "Ingrese la cedula para eliminar todos sus asientos: ";
                cin >> cedula;
                lista.eliminarPorCedula(cedula);
                break;
            }

            case 5:
                Asiento::mostrarDisponibilidad();
                break;

            case 6:
                cout << "Saliendo del programa...\n";
                break;

            default:
                cout << "Opcion invalida. Intente nuevamente.\n";
        }
    } while(opcion != 6);

    return 0;
}
