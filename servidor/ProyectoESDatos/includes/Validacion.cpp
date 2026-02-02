#include "Validacion.h"
#include "../src/modelo/FechaHora.h"
#include <conio.h>
using namespace std;

bool Validacion::validar(const std::string& texto, const std::string& tipo) {
    if (tipo == "nombre" || tipo == "apellido") {
        // Solo letras y espacios
        std::regex patron("^[A-Za-zÁÉÍÓÚáéíóúÑñ ]+$");
        return std::regex_match(texto, patron);
    }
    else if (tipo == "telefono") {
        
        std::regex patron("^0[0-9]{9}$");
        return std::regex_match(texto, patron);
    }
    else if (tipo == "correo") {
        //  correos como ivan@espe.edu.ec
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

bool Validacion::validarFecha(const string& fecha) {
    int dia, mes, anio;
    char aux1, aux2;  

    stringstream ss(fecha);
    if (!(ss >> dia >> aux1 >> mes >> aux2 >> anio)) {
        return false; 
    }

    if (aux1 != '/' || aux2 != '/') return false; 

    if (anio < 1900 || anio > 2100) return false;
    if (mes < 1 || mes > 12) return false;

    int diasMes[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  
    if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0))
        diasMes[1] = 29;

    if (dia < 1 || dia > diasMes[mes - 1]) return false;

    return true;
}

bool Validacion::validarNumeroEnRango(const std::string& texto, int min, int max) {
    
    std::regex patron("^[0-9]+$");
    if (!std::regex_match(texto, patron)) {
        return false;
    }

   
    int numero = std::stoi(texto);

    // Verificar el rango
    return numero >= min && numero <= max;
}

bool Validacion::validarNumeroEnRango(int numero, int min, int max) {
    return numero >= min && numero <= max;
}

bool Validacion::validarHora(const string& hora) {
    int horas, minutos;
    char separador;

    stringstream ss(hora);
    if (!(ss >> horas >> separador >> minutos)) return false;

    if (separador != ':') return false;
    if (horas < 0 || horas > 23) return false;
    if (minutos < 0 || minutos > 59) return false;

    return true;
}


string Validacion::ingresar_email(const std::string& mensaje) {
    char c;
    string palabra;
    cout << mensaje;
    int arroba_count = 0;
    bool arroba_encontrado = false;

    while (true) {
        c = getch();

        if (isalnum(c) || c == '-' || c == '_' || c == '+' ||
            (c == '.' && !palabra.empty() && !arroba_encontrado && palabra.back() != '.')) 
        {
            // '.' solo antes del @ y no al inicio o doble punto
            palabra += c;
            cout << c;
        }
        else if (c == '@' && arroba_count == 0 && !palabra.empty() && palabra.back() != '.') {
            palabra += c;
            cout << c;
            arroba_count++;
            arroba_encontrado = true;
        }
        else if (c == '.' && arroba_encontrado && palabra.back() != '@' && palabra.back() != '.') {
            palabra += c;
            cout << c;
        }
        else if (c == 13 && palabra.length() >= 6 && Validacion::validar(palabra, "correo")) { 
            // ENTER + validación final usando tu método
            transform(palabra.begin(), palabra.end(), palabra.begin(), ::tolower);
            return palabra;
        }
        else if (c == 8 && !palabra.empty()) { // Backspace
            if (palabra.back() == '@') {
                arroba_count--;
                arroba_encontrado = false;
            }
            palabra.pop_back();
            cout << "\b \b";
        }
        else if (c == 13) { 
            // ENTER pero correo no válido → devolver vacío
            palabra.clear();
            return palabra;
        }
    }
}