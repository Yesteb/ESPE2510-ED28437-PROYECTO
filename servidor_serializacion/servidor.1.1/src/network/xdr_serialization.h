#pragma once

#include <string>
#include <vector>

struct SeatInfo {
    std::string codigo; // e.g., "A1"
    bool ocupado;
    std::string nombre; // propietario si ocupado
    std::string cedula;
};

struct MatrixInfo {
    int filas;
    int columnas;
    std::vector<SeatInfo> seats; // length filas*columnas, row-major
};

// Encode MatrixInfo into XDR into `out` (binary buffer)
// Returns true on success
bool encode_matrix_xdr(const MatrixInfo& m, std::vector<char>& out);

// Decode XDR buffer into MatrixInfo
bool decode_matrix_xdr(const char* buf, size_t len, MatrixInfo& out);
