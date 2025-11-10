#include "Usuario.h"

// --- Constructor ---
Usuario::Usuario(const string& nombre, const string& cedula,
                 const string& numeroCelular, const string& correo) {
    setNombre(nombre);
    setCedula(cedula);
    setNumeroCelular(numeroCelular);
    setCorreo(correo);
}

// --- Setters ---
void Usuario::setNombre(const string& nombre) {
    Validacion::validar(nombre, "nombre");
    this->nombre = nombre;
}

void Usuario::setCedula(const string& cedula) {
    Validacion::validar(cedula, "cedula");
    this->cedula = cedula;
}

void Usuario::setNumeroCelular(const string& numeroCelular) {
    Validacion::validar(numeroCelular, "telefono");
    this->numeroCelular = numeroCelular;
}

void Usuario::setCorreo(const string& correo) {
    Validacion::validar(correo, "correo");
    this->correo = correo;
}

// --- Mostrar datos ---
void Usuario::mostrarDatos() const {
    cout << "\n Datos del Usuario:\n";
    cout << "Nombre: " << nombre << endl;
    cout << "Cedula: " << cedula << endl;
    cout << "Celular: " << numeroCelular << endl;
    cout << "Correo: " << correo << endl;
}

void Usuario::ingresarDatos() {
    string auxiliar;
    bool valido = false;

    // Nombre
    do {
        cout << "Ingrese nombre: ";
        cin >> auxiliar;
        valido = Validacion::validar(auxiliar, "nombre");
        if (valido == false)
            cout << "Error: el nombre debe contener solo letras y espacios. Intente nuevamente.\n";
    } while (valido == false);
    nombre = auxiliar;

    // Cedula
    do {
        cout << "Ingrese cedula: ";
        cin >> auxiliar;
        valido = Validacion::validar(auxiliar, "cedula");
        if (valido == false)
            cout << "Error: la cedula no es valida. Debe ser una cedula ecuatoriana de 10 digitos. Intente nuevamente.\n";
    } while (valido == false);
    cedula = auxiliar;

    // Numero de celular
    do {
        cout << "Ingrese numero de celular: ";
        cin >> auxiliar;
        valido = Validacion::validar(auxiliar, "telefono");
        if (valido == false)
            cout << "Error: el numero de celular no es valido. Debe empezar con 0 y tener 10 digitos. Intente nuevamente.\n";
    } while (valido == false);
    numeroCelular = auxiliar;

    // Correo electronico
    do {
        cout << "Ingrese correo electronico: ";
        cin >> auxiliar;
        valido = Validacion::validar(auxiliar, "correo");
        if (valido == false) {
            cout << "Error: el correo no es valido. Debe tener formato correcto, por ejemplo ivan@espe.edu.ec. Intente nuevamente.\n";
        }
    } while (valido == false);
    correo = auxiliar;

    cout << "\nUsuario registrado correctamente.\n";
}
