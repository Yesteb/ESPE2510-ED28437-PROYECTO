

#include "Validacion.h"
#include <iostream>
#include <regex>
#include <stdexcept>
#include <limits>
#include <conio.h>

using namespace std;

void Validacion::validar(const string& texto, const string& tipo) {
    if (tipo == "nombre" || tipo == "apellido") {
        regex patron("^[A-Za-zÁÉÍÓÚáéíóúÑñ ]+$");
        if (!regex_match(texto, patron)) {
            throw invalid_argument("Error: " + tipo + " contiene caracteres invalidos.");
        }
    }
    else if (tipo == "telefono") {
        regex patron("^[0-9]{7,15}$");
        if (!regex_match(texto, patron)) {
            throw invalid_argument("Error, el  telefono debe contener solo números (7-15 dígitos).");
        }
    }
    else if (tipo == "correo") {

        regex patron(R"(^[A-Za-z0-9][\w\.-]*[A-Za-z0-9]@[A-Za-z0-9][\w\.-]*\.[A-Za-z]{2,}$)");

        if (!regex_match(texto, patron)) {
            throw invalid_argument("Error; formato de correo electronico invalido.");
        }

        if (texto.find("..") != string::npos || texto.back() == '.' || texto.back() == '/' ||
            texto.back() == '"' || texto.back() == '|' || texto.front() == '.' || texto.front() == '-') {
            throw invalid_argument("Error: el correo contiene caracteres invalidos o mal ubicados.");
        }
    }
    else if (tipo == "cedula") {
        if (!validarCedulaEcuatoriana(texto)) {
            throw invalid_argument("Error: cedula  ecuatoriana invalida.");
        }
    }
    else {
        throw invalid_argument("Error: tipo de validacion desconocido.");
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
