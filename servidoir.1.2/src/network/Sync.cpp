#include "Sync.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <sstream>
#include <vector>

#include "../controlador/ListaCircular.h"
#include "../modelo/Asiento.h"
#include "../controlador/hash.h"
#include "../modelo/ArbolBusqueda.h"

namespace {
    std::thread listenerThread;
    std::atomic<bool> running{false};
    ListaCircular* sharedLista = nullptr;
    HashAsientos* sharedHash = nullptr;
    ArbolBusqueda* sharedArbol = nullptr;
    std::string archivoLocal = "reservas.toon";
    std::mutex listaMutex;
    int listenPort = 50000;

    // Split helper
    static std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> out;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) out.push_back(item);
        return out;
    }

    // Comprueba si un asiento ya existe en la lista (por codigo)
    static bool asientoExiste(ListaCircular* lista, const std::string& codigo) {
        Nodo* aux = lista->getCabeza();
        if (!aux) return false;
        do {
            std::string codigoAsiento = aux->dato.getNombreAsiento();
            // normalizar mayúsculas y espacios
            codigoAsiento.erase(remove_if(codigoAsiento.begin(), codigoAsiento.end(), ::isspace), codigoAsiento.end());
            std::string c = codigo;
            c.erase(remove_if(c.begin(), c.end(), ::isspace), c.end());
            if (codigoAsiento == c) return true;
            aux = aux->siguiente;
        } while (aux != lista->getCabeza());
        return false;
    }

    void runListener(int puerto) {
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            perror("socket");
            return;
        }

        sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(puerto);

        int reuse = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

        if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("bind");
            close(sockfd);
            return;
        }

        char buffer[4096];
        while (running.load()) {
            sockaddr_in src;
            socklen_t srclen = sizeof(src);
            ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&src, &srclen);
            if (n <= 0) continue;
            buffer[n] = '\0';
            std::string msg(buffer);

            // Mensaje esperado: RESERVE|codigo|tipo|nombre|cedula
            if (msg.rfind("RESERVE|", 0) == 0) {
                auto parts = split(msg.substr(8), '|');
                if (parts.size() >= 4) {
                    std::string codigo = parts[0];
                    std::string tipo = parts[1];
                    std::string nombre = parts[2];
                    std::string cedula = parts[3];

                    std::lock_guard<std::mutex> lock(listaMutex);
                    if (!sharedLista) continue;

                    // si ya existe la cedula en el hash, ignorar
                    if (sharedHash && sharedHash->existecedula(cedula)) continue;

                    // si el asiento ya existe por codigo, ignorar
                    if (asientoExiste(sharedLista, codigo)) continue;

                    Asiento a;
                    a.setNombreAsiento(codigo);
                    a.setTipoAsiento(tipo);
                    Usuario u;
                    u.setNombre(nombre);
                    u.setCedula(cedula);
                    a.setUsuario(u);
                    // insertar en la lista local
                    sharedLista->insertarFinal(a);
                    // actualizar hash y arbol si están disponibles
                    if (sharedHash) sharedHash->insertar(a);
                    if (sharedArbol) sharedArbol->insertar(a);
                    // guardar en archivo local
                    sharedLista->guardarEnArchivo(archivoLocal);

                    std::cout << "[Sync] Reserva sincronizada: " << codigo << " -> " << nombre << "\n";
                }
            }
        }

        close(sockfd);
    }
}

namespace Sync {

    void iniciar(ListaCircular* lista, HashAsientos* hash, ArbolBusqueda* arbol, const std::string& archivoReservas, int puerto) {
        if (running.load()) return;
        sharedLista = lista;
        sharedHash = hash;
        sharedArbol = arbol;
        archivoLocal = archivoReservas;
        listenPort = puerto;
        running.store(true);
        listenerThread = std::thread(runListener, puerto);
        listenerThread.detach();
    }

    void detener() {
        if (!running.load()) return;
        running.store(false);
    }

    void broadcastReservation(const Asiento& a, int puerto) {
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
            perror("socket");
            return;
        }

        int broadcastEnable = 1;
        setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

        sockaddr_in dest;
        memset(&dest, 0, sizeof(dest));
        dest.sin_family = AF_INET;
        dest.sin_port = htons(puerto);
        dest.sin_addr.s_addr = inet_addr("255.255.255.255");

        // Construir mensaje simple
        std::string msg = "RESERVE|";
        msg += a.getNombreAsiento();
        msg += "|";
        msg += a.getTipoAsiento();
        msg += "|";
        msg += a.getUsuario().getNombre();
        msg += "|";
        msg += a.getUsuario().getCedula();

        ssize_t sent = sendto(sock, msg.c_str(), msg.size(), 0, (struct sockaddr*)&dest, sizeof(dest));
        if (sent < 0) perror("sendto");
        close(sock);
    }
}
