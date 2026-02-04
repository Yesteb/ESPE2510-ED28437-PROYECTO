#include <iostream>
#include <string>
#include <algorithm>
#include <regex>
#include <limits>
#include <cstring>
#include <sstream>
#include <vector>
#include "xdr_serialization.h"

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
    cout << "| 5 | Mostrar disponibilidad de asientos (detalle XDR) |\n";
    cout << "+---+------------------------------------+\n";
    cout << "| 6 | Probar conexion (PING)             |\n";
    cout << "+---+------------------------------------+\n";
    cout << "| 7 | Salir                              |\n";
    cout << "+---+------------------------------------+\n";
    cout << ">> ";
}

void pruebaConexion() {
    cout << "Enviando PING...\n";
    string resp;
    if (!enviarComando("PING", resp)) {
        cout << "ERROR: No se pudo conectar con el servidor.\n";
        return;
    }
    if (resp.find("PONG") != string::npos) {
        cout << "Conexion exitosa!\n";
    } else {
        cout << "ERROR: Respuesta inesperada del servidor.\n";
    }
}

void mostrarAsientosReservados() {
    cout << "\n--- Asientos Reservados ---\n";
    string resp;
    if (!enviarComando("LIST", resp)) {
        cout << "ERROR: Conexion perdida.\n";
        return;
    }
    
    if (resp.find("START_LIST") == string::npos) {
        cout << "ERROR: Respuesta inesperada.\n";
        return;
    }
    
    size_t start = resp.find("START_LIST") + 10;
    size_t end = resp.find("END_LIST");
    string lista = resp.substr(start, end - start);
    
    if (lista.empty() || lista == "\n") {
        cout << "No hay asientos reservados.\n";
        return;
    }
    
    cout << "Asiento | Tipo      | Usuario\n";
    cout << "--------|-----------|--------------------\n";
    
    size_t pos = 0;
    while (pos < lista.size()) {
        size_t newline = lista.find('\n', pos);
        if (newline == string::npos) break;
        
        string linea = lista.substr(pos, newline - pos);
        if (!linea.empty()) {
            auto parts = split(linea, '|');
            if (parts.size() >= 3) {
                cout << parts[0] << " | " << parts[1] << " | " << parts[2] << "\n";
            }
        }
        pos = newline + 1;
    }
}

void buscarAsiento() {
    string cedula;
    cout << "\nIngrese cedula a buscar: ";
    cin >> cedula;
    
    string resp;
    if (!enviarComando("SEARCH|" + cedula, resp)) {
        cout << "ERROR: Conexion perdida.\n";
        return;
    }
    
    if (resp.find("START_SEARCH") == string::npos) {
        cout << "ERROR: Respuesta inesperada.\n";
        return;
    }
    
    size_t start = resp.find("START_SEARCH") + 12;
    size_t end = resp.find("END_SEARCH");
    string lista = resp.substr(start, end - start);
    
    if (lista.empty() || lista == "\n") {
        cout << "No se encontraron asientos para esa cedula.\n";
        return;
    }
    
    cout << "Asiento | Tipo      | Usuario\n";
    cout << "--------|-----------|--------------------\n";
    
    size_t pos = 0;
    while (pos < lista.size()) {
        size_t newline = lista.find('\n', pos);
        if (newline == string::npos) break;
        
        string linea = lista.substr(pos, newline - pos);
        if (!linea.empty()) {
            auto parts = split(linea, '|');
            if (parts.size() >= 3) {
                cout << parts[0] << " | " << parts[1] << " | " << parts[2] << "\n";
            }
        }
        pos = newline + 1;
    }
}

void mostrarDisponibilidad() {
    cout << "\n--- Disponibilidad de Asientos (detalle XDR) ---\n";
    string cmd = "MATRIX_XDR\n";
    if (sock < 0) { cout << "ERROR: Socket inválido.\n"; return; }
    if (send(sock, cmd.c_str(), cmd.size(), 0) <= 0) { cout << "ERROR: envio fallido.\n"; return; }

    // Read header line (e.g., "XDR|<len>\n") and then read exactly <len> bytes
    string acc; // accum for header and any payload
    vector<char> bin;
    char tmp[4096];
    size_t expected = 0;

    while (true) {
        ssize_t n = recv(sock, tmp, sizeof(tmp), 0);
        if (n <= 0) { cout << "ERROR: Conexion perdida.\n"; return; }
        acc.append(tmp, tmp + n);
        size_t pos = acc.find('\n');
        if (pos != string::npos) {
            string header = acc.substr(0, pos);
            if (header.rfind("XDR|", 0) != 0) {
                cout << "ERROR: Encabezado inesperado: " << header << "\n";
                return;
            }
            // parse length
            try {
                expected = (size_t)stoul(header.substr(4));
            } catch (...) { cout << "ERROR: Header corrupto.\n"; return; }

            // move any payload already received (after newline)
            size_t payload_start = pos + 1;
            if (acc.size() > payload_start) {
                bin.insert(bin.end(), acc.begin() + payload_start, acc.end());
            }

            // read remaining bytes
            while (bin.size() < expected) {
                ssize_t m = recv(sock, tmp, sizeof(tmp), 0);
                if (m <= 0) { cout << "ERROR: Conexion perdida durante transferencia.\n"; return; }
                bin.insert(bin.end(), tmp, tmp + m);
            }
            break;
        }
        // else continue receiving until header line found
    }

    // decode XDR
    MatrixInfo m;
    if (!decode_matrix_xdr(bin.data(), bin.size(), m)) {
        cout << "ERROR: No se pudo decodificar XDR.\n";
        return;
    }

    // Print matrix with format similar to Asiento::mostrarDisponibilidad
    for (int i = 0; i < m.filas; ++i) {
        if (i == 0) cout << "General: ";
        else if (i == 1) cout << "Palco:   ";
        else cout << "Tribuna: ";

        for (int j = 0; j < m.columnas; ++j) {
            const SeatInfo& s = m.seats[i * m.columnas + j];
            char flag = s.ocupado ? 'X' : ' ';
            cout << s.codigo << "[" << flag << "] ";
        }
        cout << '\n';
    }

    // Additionally print occupied seats with user info
    cout << "\n--- Reservas (ocupados) ---\n";
    cout << "Asiento | Usuario | Cedula\n";
    cout << "--------|---------|--------\n";
    for (const SeatInfo& s : m.seats) {
        if (s.ocupado) {
            cout << s.codigo << " | " << s.nombre << " | " << s.cedula << "\n";
        }
    }
}

void eliminarReserva() {
    string cedula;
    cout << "\nIngrese cedula a eliminar: ";
    cin >> cedula;
    
    string resp;
    if (!enviarComando("DELETE|" + cedula, resp)) {
        cout << "ERROR: Conexion perdida.\n";
        return;
    }
    
    if (resp.find("OK|") != string::npos) {
        cout << "Reserva eliminada correctamente.\n";
    } else if (resp.find("NO_ENCONTRADO") != string::npos) {
        cout << "ERROR: No se encontro reserva con esa cedula.\n";
    } else {
        cout << "ERROR: " << resp;
    }
}

void reservarAsiento() {
    string cedula, nombre, tipo, codigo;
    
    cout << "\n--- Formulario de Reserva ---\n";
    
    // Cedula
    while (true) {
        cout << "Ingrese cedula (10 digitos): ";
        cin >> cedula;
        if (validarCedula(cedula)) break;
        cout << "Cedula invalida. Intente de nuevo.\n";
    }
    
    // Nombre
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (true) {
        cout << "Ingrese nombre completo: ";
        getline(cin, nombre);
        if (validarNombre(nombre) && !nombre.empty()) break;
        cout << "Nombre invalido. Use solo letras.\n";
    }
    
    // Tipo de asiento
    while (true) {
        cout << "Tipo de asiento (GENERAL/PALCO/TRIBUNA): ";
        cin >> tipo;
        transform(tipo.begin(), tipo.end(), tipo.begin(), ::toupper);
        if (tipo == "GENERAL" || tipo == "PALCO" || tipo == "TRIBUNA") break;
        cout << "Tipo invalido.\n";
    }
    
    // Codigo de asiento
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (true) {
        cout << "Codigo de asiento (ej: A1): ";
        cin >> codigo;
        transform(codigo.begin(), codigo.end(), codigo.begin(), ::toupper);
        if (codigo.length() >= 2 && isalpha(codigo[0])) break;
        cout << "Codigo invalido.\n";
    }
    
    // Construir comando
    string cmd = "RESERVE|" + cedula + "|" + nombre + "|" + tipo + "|" + codigo;
    cout << "\nEnviando solicitud...\n";
    
    string resp;
    if (!enviarComando(cmd, resp)) {
        cout << "ERROR: Conexion perdida.\n";
        return;
    }
    
    if (resp.find("OK|") != string::npos) {
        cout << "EXITO! Asiento reservado: " << codigo << "\n";
    } else if (resp.find("CEDULA_EXISTE") != string::npos) {
        cout << "ERROR: Ya existe una reserva con esa cedula.\n";
    } else if (resp.find("ASIENTO_OCUPADO") != string::npos) {
        cout << "ERROR: El asiento ya esta ocupado.\n";
    } else if (resp.find("BAD_FORMAT") != string::npos) {
        cout << "ERROR: Formato de solicitud invalido.\n";
    } else {
        cout << "ERROR: " << resp;
    }
}

int main(int argc, char** argv) {
    string host = "localhost";
    int port = 60000;
    
    if (argc >= 2) host = argv[1];
    if (argc >= 3) port = atoi(argv[2]);
    
    // Inicializar Winsock en Windows
    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            cout << "ERROR: WSAStartup falló\n";
            return 1;
        }
    #endif
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); return 1; }
    
    sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &serv.sin_addr);
    
    cout << "Conectando a " << host << ":" << port << "...\n";
    if (connect(sock, (struct sockaddr*)&serv, sizeof(serv)) < 0) {
        perror("ERROR: No se pudo conectar");
        closesocket(sock);
        #ifdef _WIN32
            WSACleanup();
        #endif
        return 1;
    }
    
    cout << "Conectado al servidor de boleteria!\n";
    
    char opcion;
    while (true) {
        menuPrincipal();
        cin >> opcion;
        
        switch (opcion) {
            case '1':
                reservarAsiento();
                break;
            case '2':
                mostrarAsientosReservados();
                break;
            case '3':
                buscarAsiento();
                break;
            case '4':
                eliminarReserva();
                break;
            case '5':
                mostrarDisponibilidad();
                break;
            case '6':
                pruebaConexion();
                break;
            case '7':
                cout << "Desconectando...\n";
                closesocket(sock);
                #ifdef _WIN32
                    WSACleanup();
                #endif
                return 0;
            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
        }
    }
    
    return 0;
}
