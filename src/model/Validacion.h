#pragma once
#include <string>
#include <regex>
#include <stdexcept>
#include <limits>
#include <iostream>
#include <sstream>

class Validacion {
public:
    static bool validar(const std::string& texto, const std::string& tipo); 
    static bool validarCedulaEcuatoriana(const std::string& cedula);
    static std::string validarCantidad();
    static bool validarFecha(const std::string& fecha); 
};
