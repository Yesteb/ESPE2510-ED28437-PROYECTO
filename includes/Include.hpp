#pragma once
#include "Include.h"

void ImprimirArchivo::imprimirArchivo(const char* rutaArchivo, int charMax) {
    FILE *fptr = fopen(rutaArchivo, "r");
    if (fptr == nullptr) {
        std::cout << "No se ha encontrado el archivo!\n";
        return;
    }

    char* buffer = new char[charMax];

    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, charMax - 1, fptr)) > 0) {
        buffer[bytesRead] = '\0';
        std::cout << buffer;
    }

    delete[] buffer;
    fclose(fptr);
}