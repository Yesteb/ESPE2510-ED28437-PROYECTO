#pragma once
#include <iostream>
#include <string>

class Asiento {
private:
    char[2] nombreAsiento;
    std::string tipoAsiento;
    Usuario usuario;
public: 
    Asiento();
    ~Asiento();
    
    //Getters
    char getNombreAsiento();
    std::string getTipoAsiento();
    Usuario getUsuario();

    //Setters
    void setNombreAsiento(char[2]);
    void setTipoAsiento(std::string);
    void setUsuario(Usuario);

}

