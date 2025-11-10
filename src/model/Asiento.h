#pragma once
#include "Usuario.h"
#include <iostream>
#include <string>

class Asiento {
private:
    char[2] nombreAsiento;
    std::string tipoAsiento;
    Usuario duenioAsiento;
public: 
    Asiento();
    ~Asiento();
    
    //Getters
    char[2] getNombreAsiento();
    std::string getTipoAsiento();
    Usuario getUsuario();

    //Setters
    void setNombreAsiento(char[2]);
    void setTipoAsiento(std::string);
    void setUsuario(Usuario);

}




