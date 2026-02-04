#include "Servidor.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <atomic>
#include <fstream>
using namespace std;

#include "../controlador/ListaCircular.h"
#include "../controlador/hash.h"
#include "../modelo/Asiento.h"
#include "../modelo/FechaHora.h"
#include "../modelo/Usuario.h"
#include "../modelo/ArbolBusqueda.h"

namespace {
    atomic<bool> enEjecucion(false);
    mutex protectorDatos;
    ListaCircular listaGlobal;
    HashAsientos hashGlobal(10);
    ArbolBusqueda arbolGlobal;
    string archivoGlobal = "reservas.toon";

    vector<string> dividir(const string& texto, char separador) {
        vector<string> resultado;
        stringstream flujo(texto);
        string parte;
        while (getline(flujo, parte, separador)) 
            resultado.push_back(parte);
        return resultado;
    }

    string leerMenu() {
        string menu = "";
        ifstream archivo("resources/MenuPrincipal.txt");
        if (archivo.is_open()) {
            string linea;
            while (getline(archivo, linea)) 
                menu += linea + "\n";
            archivo.close();
        }
        return menu;
    }

    bool existeAsiento(const string& codigo) {
        Nodo* nodo = listaGlobal.getCabeza();
        if (!nodo) return false;
        
        do {
            string c = nodo->dato.getNombreAsiento();
            c.erase(remove_if(c.begin(), c.end(), ::isspace), c.end());
            if (c == codigo) return true;
            nodo = nodo->siguiente;
        } while (nodo != listaGlobal.getCabeza());
        return false;
    }

    void atenderCliente(int socketCliente) {
        string menu = leerMenu();
        send(socketCliente, menu.c_str(), menu.size(), 0);
        
        char buffer[4096];
        while (true) {
            ssize_t bytesRecibidos = recv(socketCliente, buffer, sizeof(buffer)-1, 0);
            if (bytesRecibidos <= 0) break;
            
            buffer[bytesRecibidos] = '\0';
            string solicitud(buffer);
            
            if (!solicitud.empty() && (solicitud.back() == '\n' || solicitud.back() == '\r')) 
                solicitud.erase(solicitud.find_last_not_of("\r\n") + 1);

            string respuesta = "";

            if (solicitud == "PING") {
                respuesta = "PONG\n";
            } 
            else if (solicitud == "LISTAR") {
                lock_guard<mutex> bloqueo(protectorDatos);
                Nodo* nodo = listaGlobal.getCabeza();
                respuesta = "INICIO_LISTA\n";
                if (nodo) {
                    do {
                        respuesta += nodo->dato.getNombreAsiento() + "|";
                        respuesta += nodo->dato.getTipoAsiento() + "|";
                        respuesta += nodo->dato.getUsuario().getNombre() + "|";
                        respuesta += nodo->dato.getUsuario().getCedula() + "\n";
                        nodo = nodo->siguiente;
                    } while (nodo != listaGlobal.getCabeza());
                }
                respuesta += "FIN_LISTA\n";
            }
            else if (solicitud.rfind("BUSCAR|", 0) == 0) {
                string cedula = solicitud.substr(7);
                lock_guard<mutex> bloqueo(protectorDatos);
                Nodo* nodo = listaGlobal.getCabeza();
                respuesta = "INICIO_BUSQUEDA\n";
                if (nodo) {
                    do {
                        if (nodo->dato.getUsuario().getCedula() == cedula) {
                            respuesta += nodo->dato.getNombreAsiento() + "|";
                            respuesta += nodo->dato.getTipoAsiento() + "|";
                            respuesta += nodo->dato.getUsuario().getNombre() + "\n";
                        }
                        nodo = nodo->siguiente;
                    } while (nodo != listaGlobal.getCabeza());
                }
                respuesta += "FIN_BUSQUEDA\n";
            }
            else if (solicitud == "DISPONIBILIDAD") {
                lock_guard<mutex> bloqueo(protectorDatos);
                respuesta = "GENERAL|";
                int generalUsados = 0, palcoUsados = 0, tribunaUsados = 0;
                
                Nodo* nodo = listaGlobal.getCabeza();
                if (nodo) {
                    do {
                        if (nodo->dato.getTipoAsiento() == "General") generalUsados++;
                        else if (nodo->dato.getTipoAsiento() == "Palco") palcoUsados++;
                        else tribunaUsados++;
                        nodo = nodo->siguiente;
                    } while (nodo != listaGlobal.getCabeza());
                }
                respuesta += to_string(10 - generalUsados) + "|PALCO|";
                respuesta += to_string(10 - palcoUsados) + "|TRIBUNA|";
                respuesta += to_string(10 - tribunaUsados) + "\n";
            }
            else if (solicitud.rfind("ELIMINAR|", 0) == 0) {
                string cedula = solicitud.substr(9);
                lock_guard<mutex> bloqueo(protectorDatos);
                
                Nodo* nodo = listaGlobal.getCabeza();
                bool encontrado = false;
                if (nodo) {
                    do {
                        if (nodo->dato.getUsuario().getCedula() == cedula) {
                            encontrado = true;
                            break;
                        }
                        nodo = nodo->siguiente;
                    } while (nodo != listaGlobal.getCabeza());
                }
                
                if (!encontrado) {
                    respuesta = "ERROR|NO_ENCONTRADO\n";
                } else {
                    hashGlobal.eliminarporcedula(cedula);
                    ListaCircular listaTemp;
                    nodo = listaGlobal.getCabeza();
                    if (nodo) {
                        do {
                            if (nodo->dato.getUsuario().getCedula() != cedula) 
                                listaTemp.insertarFinal(nodo->dato);
                            nodo = nodo->siguiente;
                        } while (nodo != listaGlobal.getCabeza());
                    }
                    listaGlobal = listaTemp;
                    listaGlobal.guardarEnArchivo(archivoGlobal);
                    respuesta = "OK|ELIMINADO\n";
                }
            }
            else if (solicitud.rfind("RESERVAR|", 0) == 0) {
                auto partes = dividir(solicitud.substr(9), '|');
                if (partes.size() < 4) {
                    respuesta = "ERROR|FORMATO_INVALIDO\n";
                } else {
                    string cedula = partes[0];
                    string nombre = partes[1];
                    string tipo = partes[2];
                    string codigo = partes[3];

                    lock_guard<mutex> bloqueo(protectorDatos);
                    
                    if (hashGlobal.existecedula(cedula)) {
                        respuesta = "ERROR|CEDULA_EXISTE\n";
                    } else if (existeAsiento(codigo)) {
                        respuesta = "ERROR|ASIENTO_OCUPADO\n";
                    } else {
                        Asiento asiento;
                        asiento.setNombreAsiento(codigo);
                        asiento.setTipoAsiento(tipo);
                        Usuario usuario;
                        usuario.setNombre(nombre);
                        usuario.setCedula(cedula);
                        asiento.setUsuario(usuario);

                        listaGlobal.insertarFinal(asiento);
                        hashGlobal.insertar(asiento);
                        arbolGlobal.insertar(asiento);
                        listaGlobal.guardarEnArchivo(archivoGlobal);

                        respuesta = "OK|" + codigo + "\n";
                    }
                }
            }
            else {
                respuesta = "ERROR|COMANDO_DESCONOCIDO\n";
            }

            send(socketCliente, respuesta.c_str(), respuesta.size(), 0);
        }
        close(socketCliente);
    }

    void aceptarConexiones(int puerto) {
        int socketServidor = socket(AF_INET, SOCK_STREAM, 0);
        if (socketServidor < 0) {
            perror("socket");
            return;
        }

        int opcion = 1;
        setsockopt(socketServidor, SOL_SOCKET, SO_REUSEADDR, &opcion, sizeof(opcion));

        sockaddr_in direccion;
        memset(&direccion, 0, sizeof(direccion));
        direccion.sin_family = AF_INET;
        direccion.sin_addr.s_addr = INADDR_ANY;
        direccion.sin_port = htons(puerto);

        if (bind(socketServidor, (struct sockaddr*)&direccion, sizeof(direccion)) < 0) {
            perror("bind");
            close(socketServidor);
            return;
        }

        if (listen(socketServidor, 10) < 0) {
            perror("listen");
            close(socketServidor);
            return;
        }

        cout << "[Servidor] Escuchando en puerto " << puerto << "\n";

        while (enEjecucion.load()) {
            sockaddr_in cliente;
            socklen_t tamanoCliente = sizeof(cliente);
            int socketCliente = accept(socketServidor, (struct sockaddr*)&cliente, &tamanoCliente);
            
            if (socketCliente < 0) {
                if (!enEjecucion.load()) break;
                perror("accept");
                continue;
            }
            
            thread hiloCliente(atenderCliente, socketCliente);
            hiloCliente.detach();
        }

        close(socketServidor);
    }
}

namespace Servidor {
    void iniciar(int puerto, const string& archivoReservas) {
        if (enEjecucion.load()) return;
        
        archivoGlobal = archivoReservas;
        listaGlobal.cargarDesdeArchivo(archivoGlobal);
        
        Nodo* nodo = listaGlobal.getCabeza();
        if (nodo) {
            do {
                hashGlobal.insertar(nodo->dato);
                arbolGlobal.insertar(nodo->dato);
                nodo = nodo->siguiente;
            } while (nodo != listaGlobal.getCabeza());
        }

        enEjecucion.store(true);
        thread(::aceptarConexiones, puerto).detach();
    }

    void detener() {
        if (!enEjecucion.load()) return;
        enEjecucion.store(false);
    }
}
