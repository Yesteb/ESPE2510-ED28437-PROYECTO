#pragma once
#include <string>
#include <regex>
#include <stdexcept>

class Validacion {
public:
    
    static void validar(const std::string& texto, const std::string& tipo);

    
    static bool validarCedulaEcuatoriana(const std::string& cedula);


    static std::string validarCantidad();
};
