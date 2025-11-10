#pragma once

#include <string>

class Validador {
public:
    static void validar(const std::string& texto, const std::string& tipo);
    static bool validarCedulaEcuatoriana(const std::string& cedula);
    static std::string validarCantidad();
};
