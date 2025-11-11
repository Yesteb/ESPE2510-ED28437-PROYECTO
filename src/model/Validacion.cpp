

#include "Validacion.h"
#include <iostream>
#include <regex>
#include <stdexcept>
#include <limits>
#include <conio.h>
#include "FechaHora.h"

using namespace std;

bool Validacion::validar(const std::string& texto, const std::string& tipo) {
    if (tipo == "nombre" || tipo == "apellido") {
        // Solo letras y espacios
        std::regex patron("^[A-Za-zÁÉÍÓÚáéíóúÑñ ]+$");
        return std::regex_match(texto, patron);
    }
    else if (tipo == "telefono") {
        // Celulares ecuatorianos: empiezan con 0 y 10 dígitos
        std::regex patron("^0[0-9]{9}$");
        return std::regex_match(texto, patron);
    }
    else if (tipo == "correo") {
        // Permitir correos como ivan@espe.edu.ec
        std::regex patron(R"(^[A-Za-z0-9][\w\.-]*@[A-Za-z0-9][\w\.-]*\.[A-Za-z]{2,}(\.[A-Za-z]{2,})?$)");
        if (!std::regex_match(texto, patron)) return false;

        // Evitar puntos dobles, caracteres al inicio o al final
        if (texto.find("..") != std::string::npos || 
            texto.back() == '.' || texto.back() == '/' ||
            texto.back() == '"' || texto.back() == '|' ||
            texto.front() == '.' || texto.front() == '-') {
            return false;
        }
        return true;
    }
    else if (tipo == "cedula") {
        return validarCedulaEcuatoriana(texto);
    }
    else {
        return false;
    }
}

bool Validacion::validarCedulaEcuatoriana(const std::string& cedula) {
    static const std::regex regexNumeros("^[0-9]{10}$");
    if (!std::regex_match(cedula, regexNumeros)) {
        return false;
    }

    int provincia = std::stoi(cedula.substr(0, 2));
    if (provincia < 1 || provincia > 24) return false;

    int tercerDigito = cedula[2] - '0';
    if (tercerDigito > 5) return false;

    int coeficientes[9] = {2,1,2,1,2,1,2,1,2};
    int suma = 0;

    for (int i = 0; i < 9; i++) {
        int valor = (cedula[i] - '0') * coeficientes[i];
        suma += (valor >= 10) ? valor - 9 : valor;
    }

    int digitoVerificador = cedula[9] - '0';
    int resultado = (suma % 10 == 0) ? 0 : 10 - (suma % 10);

    return resultado == digitoVerificador;
}

std::string Validacion::validarCantidad() {
    std::string monto;
    char c;
    bool puntoUsado = false;
    int decimales = 0;

    std::cout << "Ingrese monto: ";

    while (true) {
        c = getch();
        if (c >= '0' && c <= '9') {
            if (puntoUsado) {
                if (decimales < 2) {
                    std::cout << c;
                    monto += c;
                    decimales++;
                }
            } else {
                std::cout << c;
                monto += c;
            }
        }
        else if (c == '.' && !puntoUsado && !monto.empty()) {
            std::cout << c;
            monto += c;
            puntoUsado = true;
        }
        else if (c == 8 && !monto.empty()) {
            if (monto.back() == '.') {
                puntoUsado = false;
            } else if (puntoUsado && decimales > 0) {
                decimales--;
            }
            monto.pop_back();
            std::cout << "\b \b";
        }
        else if (c == 13 && !monto.empty() && monto != ".") {
            std::cout << std::endl;
            break;
        }
    }

    return monto;
}
