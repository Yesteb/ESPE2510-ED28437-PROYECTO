#pragma once
#include "Usuario.h"
#include <iostream>
#include <string>
#include <fstream>
//#include "../controlador/ListaCircular.h"  //Ojo
using namespace std;

class ListaCircular;  

class Asiento {
private:
    string nombreAsiento;   
    string tipoAsiento;    
    Usuario duenioAsiento;

    static int disponiblesGeneral;
    static int disponiblesPalco;
    static int disponiblesTribuna;

public: 
    Asiento();
    ~Asiento();

    string getNombreAsiento() const;
    string getTipoAsiento() const;
    Usuario getUsuario() const;

    void setNombreAsiento(const string& nombre);
    void setTipoAsiento(const string& tipo);
    void setUsuario(const Usuario& u);

void mostrarDisponibilidad(ListaCircular& listaReservas);
bool reservarAsiento(ListaCircular& listaReservas, const string& tipo, const Usuario& u, const string& nombre);

    void guardarEnArchivo(ofstream &archivo) const;
    void cargarDesdeArchivo(const string &linea);
    int buscarDistanciaBinaria(Asiento* arr, int n, const string& origen, const string& destino);

};
