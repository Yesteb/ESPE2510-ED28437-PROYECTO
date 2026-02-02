#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <algorithm>
#include <regex>
#include <limits>
#include <cstring>

using namespace std;

int sock = -1;

bool enviarComando(const string& cmd, string& resp) {
    if (sock < 0) return false;
    string msg = cmd + "\n";
    send(sock, msg.c_str(), msg.size(), 0);
    
    char buf[4096];
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
    cout << "\n=== SISTEMA DE RESERVAS (CLIENTE) ===\n";
    cout << "1. Probar conexion (PING)\n";
    cout << "2. Reservar asiento\n";
    cout << "3. Salir\n";
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
        close(sock);
        return 1;
    }
    
    cout << "Conectado al servidor de boleteria!\n";
    
    char opcion;
    while (true) {
        menuPrincipal();
        cin >> opcion;
        
        switch (opcion) {
            case '1':
                pruebaConexion();
                break;
            case '2':
                reservarAsiento();
                break;
            case '3':
                cout << "Desconectando...\n";
                close(sock);
                return 0;
            default:
                cout << "Opcion invalida.\n";
        }
    }
    
    return 0;
}