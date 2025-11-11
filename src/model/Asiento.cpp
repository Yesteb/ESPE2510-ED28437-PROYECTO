#include "Asiento.h"

int Asiento::disponiblesGeneral = 150;
int Asiento::disponiblesPalco = 100;
int Asiento::disponiblesTribuna = 50;


Asiento::Asiento() = default;
Asiento::~Asiento() = default;


std::string Asiento::getNombreAsiento() const { return nombreAsiento; }
std::string Asiento::getTipoAsiento() const { return tipoAsiento; }
Usuario Asiento::getUsuario() const { return duenioAsiento; }


void Asiento::setNombreAsiento(const std::string& nombre) { nombreAsiento = nombre; }
void Asiento::setTipoAsiento(const std::string& tipo) { tipoAsiento = tipo; }
void Asiento::setUsuario(const Usuario& u) { duenioAsiento = u; }


void Asiento::mostrarDisponibilidad() {
    cout << "\nDisponibilidad de asientos:\n";
    cout << "General: " << disponiblesGeneral << endl;
    cout << "Palco:   " << disponiblesPalco << endl;
    cout << "Tribuna: " << disponiblesTribuna << endl;
}

bool Asiento::reservarAsiento(const std::string& tipo, const Usuario& u, const std::string& nombre) {
    if(tipo == "General" && disponiblesGeneral > 0) {
        setTipoAsiento(tipo);
        setUsuario(u);
        setNombreAsiento(nombre);
        disponiblesGeneral--;
        return true;
    } 
    else if(tipo == "Palco" && disponiblesPalco > 0) {
        setTipoAsiento(tipo);
        setUsuario(u);
        setNombreAsiento(nombre);
        disponiblesPalco--;
        return true;
    } 
    else if(tipo == "Tribuna" && disponiblesTribuna > 0) {
        setTipoAsiento(tipo);
        setUsuario(u);
        setNombreAsiento(nombre);
        disponiblesTribuna--;
        return true;
    } 
    else {
        cout << "No hay asientos disponibles en la sección " << tipo << ".\n";
        return false;
    }
}
