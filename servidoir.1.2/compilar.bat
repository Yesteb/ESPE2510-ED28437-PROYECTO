@echo off
echo Compilando...

g++ -std=c++17 ^
    .\Main.cpp ^
    .\src\modelo\FechaHora.cpp ^
    .\src\modelo\Usuario.cpp ^
    .\src\modelo\Asiento.cpp ^
    .\src\vista\VistaReservarAsiento.cpp ^
    .\src\controlador\ListaCircular.cpp ^
    .\src\controlador\ordenamiento.cpp ^
    .\src\controlador\hash.cpp ^
    .\includes\Include.cpp ^
    .\includes\Validacion.cpp ^
    .\src\modelo\ArbolBusqueda.cpp ^
    -o Main.exe

if %ERRORLEVEL% NEQ 0 (
    echo Hubo errores de compilacion.
    pause
    exit /b %ERRORLEVEL%
)

echo Compilacion exitosa.
echo Ejecutando programa...
Main.exe

pause
