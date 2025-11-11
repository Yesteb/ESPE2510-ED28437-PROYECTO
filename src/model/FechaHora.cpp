#include "FechaHora.h"

FechaHora::FechaHora() {
    actualizar();
}

void FechaHora::actualizar() {
    time(&t);
    tmInfo = localtime(&t);
    strftime(fecha, sizeof(fecha), "Fecha: %Y/%m/%d a las %H:%M", tmInfo);
}

const char* FechaHora::getFechaHora() const {
    return fecha;
}

void FechaHora::imprimir() const {
    std::cout << fecha << std::endl;
}
