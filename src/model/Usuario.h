#pragma once
#include <iostream>
#include <string>
#include "Validacion.h"

using namespace std;

class Usuario {
private:
    string nombre;
    string cedula;
    string numeroCelular;
    string correo;

public:
    Usuario() = default;
    Usuario(const string& nombre, const string& cedula,
             const string& numeroCelular, const string& correo);

    // --- Setters ---
    void setNombre(const string& nombre);
    void setCedula(const string& cedula);
    void setNumeroCelular(const string& numeroCelular);
    void setCorreo(const string& correo);

    // --- Métodos principales ---
    void mostrarDatos() const;
    void ingresarDatos();
};

