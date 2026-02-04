#pragma once
#include <iostream>
#include <string>
#include "../modelo/Asiento.h"



struct nodohash {
    Asiento dato;
    nodohash* sig;
    nodohash* ant;
};

struct cubeta {
    int indice;
    nodohash* primero;
    cubeta* sig;
    cubeta* ant;
};

class HashAsientos {
public:
    cubeta* inicio;
    int tamanio;

    HashAsientos(int tam);
    ~HashAsientos();

    void insertar(const Asiento& a);
    bool existecedula(const std::string& cedula);
    bool existeasiento(const std::string& codigo);
    void eliminarporcedula(const std::string& cedula);
    void mostrar();

private:
    int funcionhash(const std::string& cedula) const;
    cubeta* obtenercubeta(int indice);
};
