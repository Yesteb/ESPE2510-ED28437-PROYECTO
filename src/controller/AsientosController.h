#pragma once
#include <iostream>
#include "Asiento.h"
#include "FechaHora.h"
#include "ListaDobleCircular.h"
#include "Usuario.h"
#include "Validacion.h"
#include <string>

class AsientosController{
  public:
    AsientosController();
    ~AsientosController();


    bool reservar(Asiento asiento);
    bool eliminarReserva(Asiento asiento);
    void buscarAsiento(std::string cedula);
    void mostrarAsientoReservado(std::string cedula);
};

#include "AsientosController.hpp"
