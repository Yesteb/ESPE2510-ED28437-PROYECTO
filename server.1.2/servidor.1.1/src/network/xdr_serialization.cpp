#include "xdr_serialization.h"
#if defined(__has_include)
  #if __has_include(<rpc/xdr.h>)
    #include <rpc/xdr.h>
  #elif __has_include(<tirpc/rpc/xdr.h>)
    #include <tirpc/rpc/xdr.h>
  #else
    #error "XDR header not found. Install libtirpc-dev or ensure rpc/xdr.h is available."
  #endif
#else
  /* Fallback: try the traditional header; build may still fail and produce a clear error */
  #include <rpc/xdr.h>
#endif

#include <cstring>
#include <cstdlib>

// Note: simple, small fixed limits for strings in XDR to avoid unbounded sizes
static const unsigned int MAX_CODE_LEN = 8;
static const unsigned int MAX_NAME_LEN = 256;
static const unsigned int MAX_CEDULA_LEN = 32;

bool encode_matrix_xdr(const MatrixInfo& m, std::vector<char>& out) {
    // allocate buffer
    size_t bufsize = 16384; // should be enough for 3x10 with some strings
    char* buf = (char*)malloc(bufsize);
    if (!buf) return false;

    XDR xdrs;
    xdrmem_create(&xdrs, buf, bufsize, XDR_ENCODE);

    int filas = m.filas;
    int columnas = m.columnas;
    if (!xdr_int(&xdrs, &filas)) goto fail;
    if (!xdr_int(&xdrs, &columnas)) goto fail;

    for (const SeatInfo& s : m.seats) {
        // code
        char* code_c = strdup(s.codigo.c_str());
        if (!xdr_string(&xdrs, &code_c, MAX_CODE_LEN)) { free(code_c); goto fail; }
        free(code_c);

        int ocupado = s.ocupado ? 1 : 0;
        if (!xdr_int(&xdrs, &ocupado)) goto fail;

        if (ocupado) {
            char* name_c = strdup(s.nombre.c_str());
            if (!xdr_string(&xdrs, &name_c, MAX_NAME_LEN)) { free(name_c); goto fail; }
            free(name_c);

            char* ced_c = strdup(s.cedula.c_str());
            if (!xdr_string(&xdrs, &ced_c, MAX_CEDULA_LEN)) { free(ced_c); goto fail; }
            free(ced_c);
        }
    }

    // get used size
    size_t used = xdr_getpos(&xdrs);
    out.assign(buf, buf + used);
    xdr_destroy(&xdrs);
    free(buf);
    return true;

fail:
    xdr_destroy(&xdrs);
    free(buf);
    return false;
}

bool decode_matrix_xdr(const char* buf, size_t len, MatrixInfo& out) {
    XDR xdrs;
    xdrmem_create(&xdrs, (char*)buf, (int)len, XDR_DECODE);

    int filas = 0, columnas = 0;
    if (!xdr_int(&xdrs, &filas)) goto fail;
    if (!xdr_int(&xdrs, &columnas)) goto fail;

    out.filas = filas;
    out.columnas = columnas;
    out.seats.clear();

    for (int i = 0; i < filas*columnas; ++i) {
        char* code_c = nullptr;
        if (!xdr_string(&xdrs, &code_c, MAX_CODE_LEN)) { if (code_c) free(code_c); goto fail; }
        SeatInfo s;
        s.codigo = code_c ? std::string(code_c) : std::string();
        if (code_c) free(code_c);

        int ocupado = 0;
        if (!xdr_int(&xdrs, &ocupado)) goto fail;
        s.ocupado = (ocupado != 0);

        if (s.ocupado) {
            char* name_c = nullptr;
            char* ced_c = nullptr;
            if (!xdr_string(&xdrs, &name_c, MAX_NAME_LEN)) { if (name_c) free(name_c); goto fail; }
            if (!xdr_string(&xdrs, &ced_c, MAX_CEDULA_LEN)) { if (ced_c) free(ced_c); if (name_c) free(name_c); goto fail; }
            s.nombre = name_c ? std::string(name_c) : std::string();
            s.cedula = ced_c ? std::string(ced_c) : std::string();
            if (name_c) free(name_c);
            if (ced_c) free(ced_c);
        }

        out.seats.push_back(std::move(s));
    }

    xdr_destroy(&xdrs);
    return true;

fail:
    xdr_destroy(&xdrs);
    return false;
}
