#pragma once
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
using namespace std;

class ListaCircular;
class Asiento;

namespace Sincronizacion {
    void iniciar(ListaCircular* lista, int puerto = 50000);
    void detener();
    void enviarReserva(const Asiento& asiento, int puerto = 50000);
}
