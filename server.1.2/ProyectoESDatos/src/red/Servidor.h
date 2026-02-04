#pragma once
#include <string>
using namespace std;

class ListaCircular;
class HashAsientos;

namespace Servidor {
    void iniciar(int puerto, const string& archivoReservas = "reservas.toon");
    void detener();
}
