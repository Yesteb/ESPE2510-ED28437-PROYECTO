#include "VistaReservarAsiento.h"

VistaReservarAsiento::VistaReservarAsiento() {}

void VistaReservarAsiento::vistaReservarAsiento(ListaCircular& lista, HashAsientos& h, const string& archivo) {
    Usuario u;
    u.ingresarDatos();

    // Verificar si ya existe la cedula antes de continuar
    if (h.existecedula(u.getCedula())) {
        cout << "\n" << endl; 
        cout << "YA EXISTE UNA RESERVA CON ESTA CEDULA NO SE PUEDE RESERVAR DE NUEVO\n";
        return;
    }

    int cantidad;
    // Pedir cantidad de asientos a reservar
    while (true) {
        cout << "\nCUANTOS ASIENTOS DESEA RESERVAR MAXIMO 10: ";
        string entrada;
        cin >> entrada;

        bool soloDigitos = all_of(entrada.begin(), entrada.end(), ::isdigit);
        if (!soloDigitos) {
            cout << "ENTRADA INVALIDA INGRESE UN NUMERO ENTERO POSITIVO\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cantidad = stoi(entrada);
        if (cantidad < 1 || cantidad > 10) {
            cout << "CANTIDAD INVALIDA DEBE ESTAR ENTRE 1 Y 10\n";
            continue;
        }
        break;
    }

    // Reservar cada asiento
    for (int i = 0; i < cantidad; i++) {
        Asiento a;
        string tipo, codigoAsiento;

        a.mostrarDisponibilidad(lista);

        while (true) {
            cout << "INGRESE TIPO DE ASIENTO GENERAL PALCO TRIBUNA: ";
            cin >> tipo;
            string tipoLower = tipo;
            transform(tipoLower.begin(), tipoLower.end(), tipoLower.begin(), ::tolower);

            if (tipoLower == "general") { codigoAsiento = "A1"; break; }
            else if (tipoLower == "palco") { codigoAsiento = "B1"; break; }
            else if (tipoLower == "tribuna") { codigoAsiento = "C1"; break; }
            else { cout << "TIPO INVALIDO INGRESE GENERAL PALCO O TRIBUNA\n"; }
        }

        if (a.reservarAsiento(lista, tipo, u, codigoAsiento)) {
            lista.insertarFinal(a);

            // Insertar en hash
            h.insertar(a);

            cout << "ASIENTO RESERVADO CORRECTAMENTE CODIGO " << codigoAsiento << "\n";
        } else {
            cout << "NO SE PUDO RESERVAR EL ASIENTO #" << (i + 1) << "\n";
        }
    }

    lista.guardarEnArchivo(archivo);
}
