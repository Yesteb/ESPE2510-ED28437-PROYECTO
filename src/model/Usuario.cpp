#include "Usuario.h"
#include "FechaHora.h"

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

// Daatos del usuario
void Usuario::mostrarDatos() const {
    cout << "\nDatos del Usuario:\n";
    cout << "Nombre: " << nombre << endl;
    cout << "Cedula: " << cedula << endl;
    cout << "Celular: " << numeroCelular << endl;
    cout << "Correo: " << correo << endl;
    fechaRegistro.imprimir(); 
}

// ingreso de datos
void Usuario::ingresarDatos() {
    string auxiliar;
    bool valido = false;

    // Nombre
    do {
        cout << "Ingrese su nombre: ";
        cin >> auxiliar;
        valido = Validacion::validar(auxiliar, "nombre");
        if (!valido)
            cout << "Error: el nombre debe contener solo letras y espacios. Intente nuevamente.\n";
    } while (!valido);
    nombre = auxiliar;

    // Cedula
    do {
        cout << "Ingrese su numero de cedula (10 digitos): ";
        cin >> auxiliar;
        valido = Validacion::validar(auxiliar, "cedula");
        if (!valido)
            cout << "Error: la cedula no es valida. Debe ser una cedula ecuatoriana de 10 digitos. Intente nuevamente.\n";
    } while (!valido);
    cedula = auxiliar;

    // Numero de celular
    do {
        cout << "Ingrese numero de celular (10 digitos): ";
        cin >> auxiliar;
        valido = Validacion::validar(auxiliar, "telefono");
        if (!valido)
            cout << "Error: el numero de celular no es valido. Debe empezar con 0 y tener 10 digitos. Intente nuevamente.\n";
    } while (!valido);
    numeroCelular = auxiliar;

    // Correo electronico
    do {
        cout << "Ingrese correo electronico: ";
        cin >> auxiliar;
        valido = Validacion::validar(auxiliar, "correo");
        if (!valido)
            cout << "Error: el correo no es valido. Debe tener formato correcto, por ejemplo ivan@espe.edu.ec. Intente nuevamente.\n";
    } while (!valido);
    correo = auxiliar;

    fechaRegistro.actualizar(); 
    cout << "\nUsuario registrado correctamente.\n";
}

