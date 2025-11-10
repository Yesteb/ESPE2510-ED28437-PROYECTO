#pragma once
#include "FechaNacimiento.h"

FechaHora::FechaHora(){
    actualizar();
}

void FechaHora::actualizar(){
    time(&t);
    tmInfo = localtime(&t);
    strftime(fecha, sizeof(fecha), "%Y/%m %d a las %H:%M h", tmInfo);
}

const char* FechaHora::getFechaHora() const {
    return fecha;
}

void FechaHora::imprimir() const {
    std::cout << fecha << std::endl;
}

bool FechaHora::esMenorEdad(int diaNacimiento, int mesNacimiento, int anioNacimiento) const {
    time_t tiempoActual = time(nullptr);
    struct tm *fechaActual = localtime(&tiempoActual);

    int diaActual = fechaActual->tm_mday;
    int mesActual = fechaActual->tm_mon + 1;
    int anioActual = fechaActual->tm_year + 1900;

    int edad = anioActual - anioNacimiento;
    if(mesActual < mesNacimiento || (mesActual == mesNacimiento && diaActual < diaNacimiento)) edad--;
    if(edad <= 2) std::cout "La edad es menor o igual a 24 meses de edad" return false;
    if(edad >= 2) std::cout "La edad es mayor o igual a 65 años de edad" return false;
    return edad =<18;
}

