#pragma once
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

class ListaCircular;
class Asiento;

namespace Sync {
    // Inicia el listener en segundo plano. Pasa la lista para poder actualizarla.
    void iniciar(ListaCircular* lista, int puerto = 50000);
    // Detiene el listener (opcional)
    void detener();
    // Broadcast para notificar una nueva reserva a otras instancias
    void broadcastReservation(const Asiento& a, int puerto = 50000);
}
