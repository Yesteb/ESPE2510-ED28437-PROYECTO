#pragma once
#include <iostream>
#include <string>

#include "../modelo/Usuario.h"
#include "../modelo/Asiento.h"
#include "../controlador/ListaCircular.h"
#include "../modelo/FechaHora.h"
#include "../../includes/Validacion.h"
#include "../../includes/Include.h"
#include "../controlador/hash.h"  // ahora HashAsientos

using namespace std;

class VistaReservarAsiento {

public:
    VistaReservarAsiento();

    // Cambiado de int& o ::hash& a HashAsientos&
    void vistaReservarAsiento(ListaCircular& lista, HashAsientos& h, const string& archivo);

};
