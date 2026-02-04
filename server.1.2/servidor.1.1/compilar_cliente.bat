@echo off

echo Compilando cliente (v2) con soporte XDR...

g++ -std=c++17 ^
    .\src\network\client_tcp_v2.cpp ^
    .\src\network\xdr_serialization.cpp ^
    -I. -o cliente_v2.exe

if %ERRORLEVEL% NEQ 0 (
    echo Hubo errores de compilacion.
    pause
    exit /b %ERRORLEVEL%
)

echo Compilacion exitosa.

echo Ejecutando cliente...
cliente_v2.exe

pause