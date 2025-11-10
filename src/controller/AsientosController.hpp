#pragma once
#include "AsientosController.h"

AsientosController::AsientosController(){}
AsientosController::~AsientosController(){}

int AsientosController::reservar(Nodo<Asiento> estadio, Asiento asiento){
    estadio.insertarFinal(estadio, asiento) 
    return 1;
}

bool AsientosController::estaLlenoElEstadio(Nodo<Asiento>* estadio){
  Nodo<Asiento>* actual = estadio//300;
  int count = 0;
  if(actual == nullptr) {
  std::cout << "Esta vacio el estadio" << std::endl;
    return false;
  }

  while(actual != estadio){
    count += 1;
    actual = actual->siguiente;
  }
  
  if(count == 300) std::cout << "Se encuentran " << 300 - cout << " disponibles.\n" return true; 
  if(count < 300) return false;
}
