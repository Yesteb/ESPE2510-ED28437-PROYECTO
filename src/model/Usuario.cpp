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
void Usuario::mostrarDatos() const {
    cout << "\nDatos del Usuario:\n";
    cout << "Nombre: " << nombre << endl;
    cout << "Cedula: " << cedula << endl;
    cout << "Celular: " << numeroCelular << endl;
    cout << "Correo: " << correo << endl;
    fechaRegistro.imprimir(); 
}
bool Validacion::validarNombre(const string& nombre) {
    // No puede estar vacío
    if (nombre.empty()) return false;

    // Debe tener al menos un espacio (nombre y apellido)
    if (nombre.find(' ') == string::npos) return false;

    // Verificar que todos los caracteres sean letras o espacios
    for (char c : nombre) {
        if (!isalpha(c) && c != ' ')
            return false;
    }

    int letras = 0;
    for (char c : nombre)
        if (isalpha(c)) letras++;

    if (letras < 4) return false;

    return true;
}

// ingreso de datos
void Usuario::ingresarDatos() {
    string auxiliar;
    bool valido = false;

    // 🔹 NOMBRE
    do {
        cout << "Ingrese nombre: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, auxiliar); 

        
        valido = Validacion::validarNombre(auxiliar);

        if (!valido)
            cout << "Error: el nombre debe contener solo letras y al menos un espacio (nombre y apellido). Intente nuevamente.\n";

    } while (!valido);
    nombre = auxiliar;

    //  CEDULA
    do {
        cout << "Ingrese cedula: ";
        cin >> auxiliar;
        valido = Validacion::validar(auxiliar, "cedula");
        if (!valido)
            cout << "Error: la cedula no es valida. Debe ser una cedula ecuatoriana de 10 digitos.\n";
    } while (!valido);
    cedula = auxiliar;

    //  CELULAR
    do {
        cout << "Ingrese numero de celular: ";
        cin >> auxiliar;
        valido = Validacion::validar(auxiliar, "telefono");
        if (!valido)
            cout << "Error: el numero de celular no es valido. Debe empezar con 0 y tener 10 digitos.\n";
    } while (!valido);
    numeroCelular = auxiliar;

    //  CORREO
    do {
        cout << "Ingrese correo electronico: ";
        cin >> auxiliar;
        valido = Validacion::validar(auxiliar, "correo");
        if (!valido)
            cout << "Error: el correo no es valido. Debe tener formato correcto (ej. ivan@espe.edu.ec).\n";
    } while (!valido);
    correo = auxiliar;

    fechaRegistro.actualizar(); 
    cout << "\n Usuario registrado correctamente.\n";
}

