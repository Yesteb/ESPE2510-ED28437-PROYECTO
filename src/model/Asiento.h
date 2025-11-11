#pragma once
#include "Usuario.h"
#include <iostream>
#include <string>

using namespace std;

class Asiento {
private:
    std::string nombreAsiento;   
    std::string tipoAsiento;    
    Usuario duenioAsiento;

    
    static int disponiblesGeneral;
    static int disponiblesPalco;
    static int disponiblesTribuna;

public: 
   
    Asiento();
    ~Asiento();


    std::string getNombreAsiento() const;
    std::string getTipoAsiento() const;
    Usuario getUsuario() const;

    
    void setNombreAsiento(const std::string& nombre);
    void setTipoAsiento(const std::string& tipo);
    void setUsuario(const Usuario& u);

  
    static void mostrarDisponibilidad();
    bool reservarAsiento(const std::string& tipo, const Usuario& u, const std::string& nombre);
};

