#pragma once
#include <iostream>
#include <string>
#include "Validacion.h"
#include "FechaHora.h"

using namespace std;

class Usuario {
private:
    string nombre;
    string cedula;
    string numeroCelular;
    string correo;
    FechaHora fechaRegistro; 

public:
    Usuario() = default;
    Usuario(const string& nombre, const string& cedula,
             const string& numeroCelular, const string& correo);

    void setNombre(const string& nombre);
    void setCedula(const string& cedula);
    void setNumeroCelular(const string& numeroCelular);
    void setCorreo(const string& correo);

    string getNombre() const { return nombre; }
    string getCedula() const { return cedula; }
    string getNumeroCelular() const { return numeroCelular; }
    string getCorreo() const { return correo; }
    FechaHora getFechaRegistro() const { return fechaRegistro; }

    void mostrarDatos() const;
    void ingresarDatos();
};

