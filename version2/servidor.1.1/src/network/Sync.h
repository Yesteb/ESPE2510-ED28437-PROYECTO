#pragma once
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

class ListaCircular;
class Asiento;
class HashAsientos;
class ArbolBusqueda;

namespace Sync {
    // Inicia el listener en segundo plano. Pasa la lista, hash y arbol para poder sincronizar reservas.
    void iniciar(ListaCircular* lista, HashAsientos* hash, ArbolBusqueda* arbol, const std::string& archivoReservas, int puerto = 50000);
    // Detiene el listener (opcional)
    void detener();
    // Broadcast para notificar una nueva reserva a otras instancias
    void broadcastReservation(const Asiento& a, int puerto = 50000);
}
