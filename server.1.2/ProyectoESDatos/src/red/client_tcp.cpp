// Copia en español del cliente TCP (client_tcp.cpp)
#include <iostream>
#include <string>
#include <algorithm>
#include <regex>
#include <limits>
#include <cstring>
#include <sstream>
#include <vector>

// Multiplataforma: Windows y Linux
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    #define SHUT_RDWR SD_BOTH
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define closesocket close
#endif

using namespace std;

int sock = -1;

vector<string> split(const string& s, char delim) {
    vector<string> out;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) out.push_back(item);
    return out;
}

bool enviarComando(const string& cmd, string& resp) {
    if (sock < 0) return false;
    string msg = cmd + "\n";
    send(sock, msg.c_str(), msg.size(), 0);
    
    char buf[8192];
    ssize_t n = recv(sock, buf, sizeof(buf)-1, 0);
    if (n <= 0) return false;
    buf[n] = '\0';
    resp = buf;
    return true;
}

bool validarCedula(const string& cedula) {
    if (cedula.length() != 10) return false;
    if (!all_of(cedula.begin(), cedula.end(), ::isdigit)) return false;
    return true;
}

bool validarNombre(const string& nombre) {
    regex patron("^[A-Za-zÁÉÍÓÚáéíóúÑñ ]+$");
    return regex_match(nombre, patron);
}

void menuPrincipal() {
    cout << "\nBIENVENIDO ^ _ ^\n";
    cout << "Seleccione una opcion (1-7):\n";
    cout << "Fecha del partido: 11/10/2026\n";
    cout << "+---+------------------------------------+\n";
    cout << "| 1 | Reservar Asiento                   |\n";
    cout << "+---+------------------------------------+\n";
    cout << "| 2 | Mostrar Asientos Reservados        |\n";
    cout << "+---+------------------------------------+\n";
    cout << "| 3 | Buscar Asientos (Cedula)           |\n";
    cout << "+---+------------------------------------+\n";
    cout << "| 4 | Eliminar Reserva                   |\n";
    cout << "+---+------------------------------------+\n";
    cout << "| 5 | Mostrar disponibilidad de asientos |\n";
    cout << "+---+------------------------------------+\n";
    cout << "| 6 | Probar conexion (PING)             |\n";
    cout << "+---+------------------------------------+\n";
    cout << "| 7 | Salir                              |\n";
    cout << "+---+------------------------------------+\n";
    cout << ">> ";
}

// (resto del cliente — idéntico al cliente original, puede copiarse desde src/network)

// Este archivo es un placeholder de cliente ubicado en src/red.
// Para ejecutar el cliente utilice el archivo completo en src/network/client_tcp.cpp o copie su contenido aquí.

