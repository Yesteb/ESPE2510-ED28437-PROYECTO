#include "Server.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <atomic>

#include "../controlador/ListaCircular.h"
#include "../controlador/hash.h"
#include "../modelo/Asiento.h"
#include "../modelo/FechaHora.h"
#include "../modelo/Usuario.h"
#include "../modelo/ArbolBusqueda.h"
#include "xdr_serialization.h"

namespace {
    int serverSock = -1;
    std::atomic<bool> running(false);
    std::mutex mtx; // protege lista, hash y arbol
    ListaCircular listaGlobal;
    HashAsientos hashGlobal(10);
    ArbolBusqueda arbolGlobal;
    std::string archivoGlobal = "reservas.toon";

    static std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> out;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) out.push_back(item);
        return out;
    }

    bool asientoOcupado(const std::string& codigo) {
        Nodo* aux = listaGlobal.getCabeza();
        if (!aux) return false;
        do {
            std::string c = aux->dato.getNombreAsiento();
            c.erase(remove_if(c.begin(), c.end(), ::isspace), c.end());
            if (c == codigo) return true;
            aux = aux->siguiente;
        } while (aux != listaGlobal.getCabeza());
        return false;
    }

    void handleClient(int clientSock) {
        char buf[4096];
        while (true) {
            ssize_t n = recv(clientSock, buf, sizeof(buf)-1, 0);
            if (n <= 0) break;
            buf[n] = '\0';
            std::string req(buf);
            // Trim newline
            if (!req.empty() && (req.back()=='\n' || req.back()=='\r')) req.erase(req.find_last_not_of("\r\n") + 1);

            if (req == "PING") {
                std::string resp = "PONG\n";
                send(clientSock, resp.c_str(), resp.size(), 0);
                continue;
            }

            // LIST - listar todas las reservas
            if (req == "LIST") {
                std::lock_guard<std::mutex> lock(mtx);
                Nodo* aux = listaGlobal.getCabeza();
                std::string resp = "START_LIST\n";
                if (aux) {
                    do {
                        resp += aux->dato.getNombreAsiento() + "|";
                        resp += aux->dato.getTipoAsiento() + "|";
                        resp += aux->dato.getUsuario().getNombre() + "|";
                        resp += aux->dato.getUsuario().getCedula() + "\n";
                        aux = aux->siguiente;
                    } while (aux != listaGlobal.getCabeza());
                }
                resp += "END_LIST\n";
                send(clientSock, resp.c_str(), resp.size(), 0);
                continue;
            }

            // SEARCH|cedula - buscar por cédula
            if (req.rfind("SEARCH|", 0) == 0) {
                std::string cedula = req.substr(7);
                std::lock_guard<std::mutex> lock(mtx);
                Nodo* aux = listaGlobal.getCabeza();
                std::string resp = "START_SEARCH\n";
                if (aux) {
                    do {
                        if (aux->dato.getUsuario().getCedula() == cedula) {
                            resp += aux->dato.getNombreAsiento() + "|";
                            resp += aux->dato.getTipoAsiento() + "|";
                            resp += aux->dato.getUsuario().getNombre() + "\n";
                        }
                        aux = aux->siguiente;
                    } while (aux != listaGlobal.getCabeza());
                }
                resp += "END_SEARCH\n";
                send(clientSock, resp.c_str(), resp.size(), 0);
                continue;
            }

            // AVAILABILITY - mostrar disponibilidad
            if (req == "AVAILABILITY") {
                std::lock_guard<std::mutex> lock(mtx);
                std::string resp = "GENERAL|";
                int gUsed = 0, pUsed = 0, tUsed = 0;
                Nodo* aux = listaGlobal.getCabeza();
                if (aux) {
                    do {
                        if (aux->dato.getTipoAsiento() == "General") gUsed++;
                        else if (aux->dato.getTipoAsiento() == "Palco") pUsed++;
                        else tUsed++;
                        aux = aux->siguiente;
                    } while (aux != listaGlobal.getCabeza());
                }
                resp += std::to_string(10 - gUsed) + "|PALCO|";
                resp += std::to_string(10 - pUsed) + "|TRIBUNA|";
                resp += std::to_string(10 - tUsed) + "\n";
                send(clientSock, resp.c_str(), resp.size(), 0);
                continue;
            }

            // MATRIX_XDR - enviar matriz completa serializada en XDR
            if (req == "MATRIX_XDR") {
                std::lock_guard<std::mutex> lock(mtx);
                MatrixInfo m;
                m.filas = 3;
                m.columnas = 10;
                m.seats.reserve(m.filas * m.columnas);

                for (int i = 0; i < m.filas; ++i) {
                    char letra = 'A' + i;
                    for (int j = 0; j < m.columnas; ++j) {
                        std::string codigo = std::string(1, letra) + std::to_string(j + 1);
                        SeatInfo s;
                        s.codigo = codigo;
                        s.ocupado = false;

                        Nodo* aux = listaGlobal.getCabeza();
                        if (aux) {
                            do {
                                std::string c = aux->dato.getNombreAsiento();
                                c.erase(remove_if(c.begin(), c.end(), ::isspace), c.end());
                                if (c == codigo) {
                                    s.ocupado = true;
                                    s.nombre = aux->dato.getUsuario().getNombre();
                                    s.cedula = aux->dato.getUsuario().getCedula();
                                    break;
                                }
                                aux = aux->siguiente;
                            } while (aux != listaGlobal.getCabeza());
                        }

                        m.seats.push_back(std::move(s));
                    }
                }

                std::vector<char> buf;
                if (!encode_matrix_xdr(m, buf)) {
                    std::string err = "ERR|XDR_FAIL\n";
                    send(clientSock, err.c_str(), err.size(), 0);
                    continue;
                }

                // Send header with length then raw bytes
                std::string header = "XDR|" + std::to_string(buf.size()) + "\n";
                send(clientSock, header.c_str(), header.size(), 0);
                size_t sent = 0;
                while (sent < buf.size()) {
                    ssize_t sret = send(clientSock, buf.data() + sent, buf.size() - sent, 0);
                    if (sret <= 0) break;
                    sent += (size_t)sret;
                }
                continue;
            }

            // DELETE|cedula - eliminar por cédula
            if (req.rfind("DELETE|", 0) == 0) {
                std::string cedula = req.substr(7);
                std::lock_guard<std::mutex> lock(mtx);
                
                Nodo* aux = listaGlobal.getCabeza();
                bool encontrado = false;
                if (aux) {
                    do {
                        if (aux->dato.getUsuario().getCedula() == cedula) {
                            encontrado = true;
                            break;
                        }
                        aux = aux->siguiente;
                    } while (aux != listaGlobal.getCabeza());
                }
                
                if (!encontrado) {
                    std::string resp = "ERR|NO_ENCONTRADO\n";
                    send(clientSock, resp.c_str(), resp.size(), 0);
                    continue;
                }
                
                hashGlobal.eliminarporcedula(cedula);
                // Reconstruir lista sin esa cédula
                ListaCircular listaTemp;
                aux = listaGlobal.getCabeza();
                if (aux) {
                    do {
                        if (aux->dato.getUsuario().getCedula() != cedula) {
                            listaTemp.insertarFinal(aux->dato);
                        }
                        aux = aux->siguiente;
                    } while (aux != listaGlobal.getCabeza());
                }
                listaGlobal = listaTemp;
                listaGlobal.guardarEnArchivo(archivoGlobal);
                
                std::string resp = "OK|ELIMINADO\n";
                send(clientSock, resp.c_str(), resp.size(), 0);
                continue;
            }

            // RESERVE|cedula|nombre|tipo|codigo
            if (req.rfind("RESERVE|", 0) == 0) {
                auto parts = split(req.substr(8), '|');
                if (parts.size() < 4) {
                    std::string resp = "ERR|BAD_FORMAT\n";
                    send(clientSock, resp.c_str(), resp.size(), 0);
                    continue;
                }
                std::string cedula = parts[0];
                std::string nombre = parts[1];
                std::string tipo = parts[2];
                std::string codigo = parts[3];

                std::lock_guard<std::mutex> lock(mtx);
                // primero validar cedula ya no exista
                if (hashGlobal.existecedula(cedula)) {
                    std::string resp = "ERR|CEDULA_EXISTE\n";
                    send(clientSock, resp.c_str(), resp.size(), 0);
                    continue;
                }
                // luego verificar asiento
                if (asientoOcupado(codigo)) {
                    std::string resp = "ERR|ASIENTO_OCUPADO\n";
                    send(clientSock, resp.c_str(), resp.size(), 0);
                    continue;
                }

                // Crear asiento y agregar usando las estructuras existentes
                Asiento a;
                a.setNombreAsiento(codigo);
                a.setTipoAsiento(tipo);
                Usuario u;
                u.setNombre(nombre);
                u.setCedula(cedula);
                a.setUsuario(u);

                listaGlobal.insertarFinal(a);
                hashGlobal.insertar(a);
                arbolGlobal.insertar(a);
                listaGlobal.guardarEnArchivo(archivoGlobal);

                std::string resp = "OK|" + codigo + "\n";
                send(clientSock, resp.c_str(), resp.size(), 0);
                continue;
            }

            // unknown
            std::string resp = "ERR|UNKNOWN_CMD\n";
            send(clientSock, resp.c_str(), resp.size(), 0);
        }
        close(clientSock);
    }

    void acceptLoop(int port) {
        serverSock = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSock < 0) {
            perror("socket");
            return;
        }

        int opt = 1;
        setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(serverSock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("bind");
            close(serverSock);
            return;
        }

        if (listen(serverSock, 10) < 0) {
            perror("listen");
            close(serverSock);
            return;
        }

        std::cout << "[Server] Escuchando en puerto " << port << "\n";

        while (running.load()) {
            sockaddr_in cli;
            socklen_t clilen = sizeof(cli);
            int clientSock = accept(serverSock, (struct sockaddr*)&cli, &clilen);
            if (clientSock < 0) {
                if (!running.load()) break;
                perror("accept");
                continue;
            }
            std::thread t(handleClient, clientSock);
            t.detach();
        }

        close(serverSock);
        serverSock = -1;
    }

}

namespace Server {
    void iniciar(int puerto, const std::string& archivoReservas) {
        if (running.load()) return;
        archivoGlobal = archivoReservas;
        // cargar archivo existente
        listaGlobal.cargarDesdeArchivo(archivoGlobal);
        // sincronizar hash y arbol con lista
        Nodo* aux = listaGlobal.getCabeza();
        if (aux) {
            do {
                hashGlobal.insertar(aux->dato);
                arbolGlobal.insertar(aux->dato);
                aux = aux->siguiente;
            } while (aux != listaGlobal.getCabeza());
        }

        running.store(true);
        std::thread(acceptLoop, puerto).detach();
    }

    void detener() {
        if (!running.load()) return;
        running.store(false);
        if (serverSock >= 0) close(serverSock);
    }
}
