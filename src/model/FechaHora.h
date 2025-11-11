#pragma once
#include <ctime>
#include <iostream>

class FechaHora{
private:

    time_t t;
    struct tm *tmInfo;
    char fecha[32];

public:

    FechaHora();
    void actualizar();
    const char* getFechaHora() const;
    void imprimir() const;

};

#include "FechaHora.hpp"