# Arquitectura Cliente-Servidor - Sistema de Boletería

## Descripción General
El sistema original de boletería se ha convertido en una **arquitectura cliente-servidor centralizada** usando sockets TCP:

- **Servidor (Main.cpp)**: Máquina central que ejecuta el sistema de boletería completo
- **Cliente (cliente)**: Máquinas remotas que envían solicitudes de reserva al servidor

---

## Requisitos
- Sistema Linux/Unix o Windows con compilador g++
- Conectividad TCP entre máquinas (puerto 60000)

---

## Paso 1: Compilar

### Servidor (Máquina Principal)
```bash
cd ProyectoESDatos
g++ -std=c++17 -pthread $(find . -name '*.cpp' | grep -v client_tcp.cpp) -I. -o ProyectoESDatos
```

### Cliente (Máquinas Remotas)
```bash
cd ProyectoESDatos
g++ -std=c++17 -pthread src/network/client_tcp.cpp -I. -o cliente
```

---

## Paso 2: Ejecutar el Servidor

**En la máquina principal (con la IP conocida):**
```bash
./ProyectoESDatos
```

**Salida esperada:**
```
[Server] Escuchando en puerto 60000
>> [menú interactivo del sistema de boletería]
```

El servidor:
- Levanta en **puerto TCP 60000** (centralizado)
- Encapsula toda la lógica de boletería existente
- Protege datos con **mutex** para concurrencia
- Acepta múltiples clientes simultáneos en hilos independientes

---

## Paso 3: Ejecutar el Cliente

**En máquinas remotas:**
```bash
./cliente <IP_DEL_SERVIDOR> <PUERTO>
```

### Ejemplos:
```bash
# Conectar a servidor en IP local
./cliente 127.0.0.1 60000

# Conectar a servidor remoto
./cliente 192.168.1.100 60000

# Conectar a localhost (por defecto)
./cliente
```

**Menú del cliente:**
```
=== SISTEMA DE RESERVAS (CLIENTE) ===
1. Probar conexion (PING)
2. Reservar asiento
3. Salir
>> 
```

---

## Protocolo TCP (Cliente-Servidor)

### Comandos Soportados

#### PING (Verificar conectividad)
```
→ Cliente envía: PING
← Servidor responde: PONG
```

#### RESERVE (Hacer una reserva)
```
→ Cliente envía: RESERVE|cedula|nombre|tipo|codigo
← Servidor responde:
   - OK|codigo             (Éxito)
   - ERR|CEDULA_EXISTE     (Cédula ya reservada)
   - ERR|ASIENTO_OCUPADO   (Asiento ya reservado)
   - ERR|BAD_FORMAT        (Formato incorrecto)
```

**Ejemplo:**
```
RESERVE|1234567890|Juan Perez|GENERAL|A5
→ OK|A5
```

---

## Lógica de Protección contra Duplicados

El servidor valida en este orden (con **mutex**):

1. **¿Existe la cédula?** → Si sí, rechaza (`CEDULA_EXISTE`)
2. **¿Está el asiento ocupado?** → Si sí, rechaza (`ASIENTO_OCUPADO`)
3. Si ambas validaciones pasan → **inserta en lista, hash y árbol**
4. Guarda cambios en `reservas.toon`

Todo esto ocurre **atómicamente** con mutex, evitando race conditions.

---

## Flujo de Uso Típico

### Servidor (Máquina A: 192.168.1.10)
```bash
$ ./ProyectoESDatos
[Server] Escuchando en puerto 60000
>> [esperando clientes...]
```

### Cliente 1 (Máquina B: 192.168.1.20)
```bash
$ ./cliente 192.168.1.10 60000
Conectando a 192.168.1.10:60000...
Conectado al servidor de boleteria!

=== SISTEMA DE RESERVAS (CLIENTE) ===
1. Probar conexion (PING)
2. Reservar asiento
3. Salir
>> 1
Enviando PING...
Conexion exitosa!

>> 2
--- Formulario de Reserva ---
Ingrese cedula (10 digitos): 1234567890
Ingrese nombre completo: Juan Perez
Tipo de asiento (GENERAL/PALCO/TRIBUNA): GENERAL
Codigo de asiento (ej: A1): A5
Enviando solicitud...
EXITO! Asiento reservado: A5
```

### Cliente 2 (Máquina C: 192.168.1.30)
```bash
$ ./cliente 192.168.1.10 60000
...
>> 2
--- Formulario de Reserva ---
Ingrese cedula (10 digitos): 9876543210
Ingrese nombre completo: Maria Lopez
Tipo de asiento (GENERAL/PALCO/TRIBUNA): GENERAL
Codigo de asiento (ej: A1): A5  ← mismo asiento
Enviando solicitud...
ERROR: El asiento ya esta ocupado.  ← rechazado!
```

---

## Arquitectura de Archivos

```
ProyectoESDatos/
├── Main.cpp                          (servidor principal)
├── src/network/
│   ├── Server.h / Server.cpp         (servidor TCP)
│   ├── Sync.h / Sync.cpp             (sincronización UDP - opcional)
│   └── client_tcp.cpp                (cliente TCP)
├── src/modelo/
│   ├── Asiento.h/cpp, Usuario.h/cpp
│   ├── FechaHora.h/cpp, ArbolBusqueda.h/cpp
├── src/controlador/
│   ├── ListaCircular.h/cpp
│   ├── hash.h/cpp
│   ├── ordenamiento.h/cpp
├── src/vista/
│   └── VistaReservarAsiento.h/cpp
├── includes/
│   └── Validacion.h/cpp
└── reservas.toon                     (archivo de datos centralizado)
```

---

## Consideraciones de Seguridad

⚠️ **Nota Importante:** Este sistema es de demostración educativa. Para producción:

1. Agregar autenticación (tokens, certificados)
2. Encriptar comunicación (TLS/SSL)
3. Validar que `reservas.toon` se sincronice correctamente
4. Implementar timeouts de conexión
5. Usar base de datos centralizada en lugar de archivo

---

## Troubleshooting

### "Connection refused"
- Verificar que el servidor esté ejecutándose: `netstat -tlnp | grep 60000`
- Verificar firewall: `ufw allow 60000`
- Verificar IP correcta del servidor

### "Address already in use"
```bash
# Liberar el puerto
lsof -i :60000 | grep LISTEN
kill -9 <PID>
```

### Clientedisconectado sin guardar
- Normalmente el servidor guarda en `reservas.toon` tras cada operación exitosa
- Verificar que el archivo tenga permisos de escritura

---

## Próximas Mejoras Sugeridas

- [ ] Agregar opción "Ver todas las reservas" (solo lectura remota)
- [ ] Agregar cancelación de reservas desde cliente
- [ ] Implementar persistencia redundante (backup)
- [ ] Agregar logs en servidor
- [ ] Implementar reconexión automática en cliente
