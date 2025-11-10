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


    int reservar(Nodo<Asiento>* estadio, Asiento asiento);
    bool estaLlenoElEstadio(Nodo<Asiento> estadio);
    bool eliminarReserva(Asiento asiento);
    void buscarAsiento(std::string cedula);
    void mostrarAsientoReservado(std::string cedula);
};

#include "AsientosController.hpp"
