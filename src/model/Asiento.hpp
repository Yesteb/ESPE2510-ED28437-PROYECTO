#pragma once
#include "Asiento.h"

Asiento::Asiento(){}
Asiento::~Asiento(){}

//Getters
char[2] Asiento::getNombreAsiento(){ return nombreAsiento; }
std::string Asiento::getTipoAsiento(){ return tipoAsiento; }
Usuario Asiento::getUsuario(){ return usuario; }

//Setters
void Asiento::setNombreAsiento(char[2] nombre) : nombreAsiento(nombre){}
void Asiento::setTipoAsiento(std::string asiento) : tipoAsiento(asiento){}
void Asiento::setUsuario(Usuario usuario) : duenioAsiento(usuario){}
