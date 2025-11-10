#pragma once
#include <string>
#include "FechaNacimiento.h"
#include "Usuario.h"

class Boleto{
private:
    Usuario duenioBoleto;
    std::string id;
    std::string fechaHora;
    float precioIVA;
    float precioSinIVA;

public:
    Boleto();
    ~Boleto();

    //Getters
    Usuario getDuenioBoleto();
    std::string getId();
    std::string getFechaHora();
    float getPrecioIVA();
    float getPrecioSinIVA();

    //Setters
    void setDuenioBoleto(Usuario);
    void setId(std::string id);
    void setFechaHora(std::string);
    void setPrecioIVA(float);
    void setPrecioSinIVA(float);
    


};
