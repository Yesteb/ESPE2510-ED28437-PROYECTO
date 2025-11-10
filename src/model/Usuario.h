#pragma once
#include ".h"
#include <iostream>
#include <string>

class Usuario {
private:
    std::string nombre;
    std::string cedula;
    std::string numeroCelular;
    std::string fechaNacimiento;
    

public:
    Usuario();
    ~Usuario();

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
