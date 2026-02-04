#include "Sincronizacion.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <sstream>
using namespace std;

#include "../controlador/ListaCircular.h"
#include "../modelo/Asiento.h"

namespace {
    atomic<bool> enEjecucion(false);
    ListaCircular* listaCompartida = nullptr;
    mutex protectorLista;
    int puertoSincro = 50000;

    vector<string> dividir(const string& texto, char separador) {
        vector<string> resultado;
        stringstream flujo(texto);
        string parte;
        while (getline(flujo, parte, separador)) 
            resultado.push_back(parte);
        return resultado;
    }

    bool asientoYaExiste(ListaCircular* lista, const string& codigo) {
        Nodo* nodo = lista->getCabeza();
        if (!nodo) return false;
        
        do {
            string c = nodo->dato.getNombreAsiento();
            c.erase(remove_if(c.begin(), c.end(), ::isspace), c.end());
            if (c == codigo) return true;
            nodo = nodo->siguiente;
        } while (nodo != lista->getCabeza());
        return false;
    }

    void escucharReservas(int puerto) {
        int socketUDP = socket(AF_INET, SOCK_DGRAM, 0);
        if (socketUDP < 0) {
            perror("socket");
            return;
        }

        int reutilizable = 1;
        setsockopt(socketUDP, SOL_SOCKET, SO_REUSEADDR, &reutilizable, sizeof(reutilizable));

        sockaddr_in direccion;
        memset(&direccion, 0, sizeof(direccion));
        direccion.sin_family = AF_INET;
        direccion.sin_addr.s_addr = INADDR_ANY;
        direccion.sin_port = htons(puerto);

        if (bind(socketUDP, (struct sockaddr*)&direccion, sizeof(direccion)) < 0) {
            perror("bind");
            close(socketUDP);
            return;
        }

        char buffer[4096];
        while (enEjecucion.load()) {
            sockaddr_in origen;
            socklen_t tamanoOrigen = sizeof(origen);
            ssize_t bytesRecibidos = recvfrom(socketUDP, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&origen, &tamanoOrigen);
            
            if (bytesRecibidos <= 0) continue;
            buffer[bytesRecibidos] = '\0';
            string mensaje(buffer);

            if (mensaje.rfind("RESERVAR|", 0) == 0) {
                auto partes = dividir(mensaje.substr(9), '|');
                if (partes.size() >= 4) {
                    string codigo = partes[0];
                    string tipo = partes[1];
                    string nombre = partes[2];
                    string cedula = partes[3];

                    lock_guard<mutex> bloqueo(protectorLista);
                    if (!listaCompartida || asientoYaExiste(listaCompartida, codigo)) continue;

                    Asiento asiento;
                    asiento.setNombreAsiento(codigo);
                    asiento.setTipoAsiento(tipo);
                    Usuario usuario;
                    usuario.setNombre(nombre);
                    usuario.setCedula(cedula);
                    asiento.setUsuario(usuario);
                    
                    listaCompartida->insertarFinal(asiento);
                    cout << "[Sincronizacion] Reserva recibida: " << codigo << " -> " << nombre << "\n";
                }
            }
        }

        close(socketUDP);
    }
}

namespace Sincronizacion {
    void iniciar(ListaCircular* lista, int puerto) {
        if (enEjecucion.load()) return;
        
        listaCompartida = lista;
        puertoSincro = puerto;
        enEjecucion.store(true);
        thread(::escucharReservas, puerto).detach();
    }

    void detener() {
        if (!enEjecucion.load()) return;
        enEjecucion.store(false);
    }

    void enviarReserva(const Asiento& asiento, int puerto) {
        int socketUDP = socket(AF_INET, SOCK_DGRAM, 0);
        if (socketUDP < 0) {
            perror("socket");
            return;
        }

        int habilitarBroadcast = 1;
        setsockopt(socketUDP, SOL_SOCKET, SO_BROADCAST, &habilitarBroadcast, sizeof(habilitarBroadcast));

        sockaddr_in destino;
        memset(&destino, 0, sizeof(destino));
        destino.sin_family = AF_INET;
        destino.sin_port = htons(puerto);
        destino.sin_addr.s_addr = inet_addr("255.255.255.255");

        string mensaje = "RESERVAR|";
        mensaje += asiento.getNombreAsiento() + "|";
        mensaje += asiento.getTipoAsiento() + "|";
        mensaje += asiento.getUsuario().getNombre() + "|";
        mensaje += asiento.getUsuario().getCedula();

        ssize_t enviados = sendto(socketUDP, mensaje.c_str(), mensaje.size(), 0, (struct sockaddr*)&destino, sizeof(destino));
        if (enviados < 0) perror("sendto");
        close(socketUDP);
    }
}
