#pragma once
#include <string>

class ListaCircular;
class HashAsientos;

namespace Server {
    // Inicia el servidor TCP en el puerto indicado. Ruta de archivo de reservas.
    void iniciar(int puerto, const std::string& archivoReservas="reservas.toon");
    void detener();
}
