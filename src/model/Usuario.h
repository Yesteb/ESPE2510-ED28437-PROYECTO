#pragma once
#include ".h"
#include <iostream>
#include <string>
using namespace std; 
class Usuario {
private:
    std::string nombre;
    std::string cedula;
    std::string numeroCelular;
    std::string fechaNacimiento;
    

public:
    Usuario();
    ~Usuario();

    void ingresarUsuario(){
        cout<<"Ingrese su Nombre y Apellido: "<<; 
        cin>>nombre; 
        cout<<"Ingrese su numero de cedula: <<; 
        cin>>cedula; 
        cout<<"Ingrese su numero de telefono: "<<;
        cin>>numeroCelular;       
    }

    void mostrarDatos(){
       
    
    }

    //Getters
    std::string getNombre();
    std::string getCedula();
    std::string getNumeroCelular();
    std::string getFechaNacimiento();

    //Setters
    void setNombre(std::string);
    void setCedula(std::string);
    void setNumeroCelular(std::string);
    void setFechaNacimiento(std::string);

};
