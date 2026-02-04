#!/usr/bin/env bash
# compilar_server.sh - Script de compilación para el servidor (Linux/WSL)
# Ubique este archivo en la raíz del proyecto y ejecútelo: ./compilar_server.sh

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
OUT="ProyectoESDatos"
CXX="g++"
CXXFLAGS="-std=c++17 -pthread -I."
# Excluir clientes (cliente TCP y cliente_v2)
EXCLUDE_PATTERN='client_tcp(.*)\\.cpp|cliente\\.cpp'

echo "[build] Directorio del proyecto: $ROOT_DIR"

if ! command -v "$CXX" >/dev/null 2>&1; then
    echo "ERROR: '$CXX' no encontrado en PATH. Instale g++ y vuelva a intentar." >&2
    exit 1
fi

# Recolectar fuentes (excluye archivos de cliente)
mapfile -t SRCS < <(find "$ROOT_DIR" -name "*.cpp" | grep -Ev "$EXCLUDE_PATTERN")
if [ ${#SRCS[@]} -eq 0 ]; then
    echo "ERROR: No se encontraron archivos fuente .cpp para compilar." >&2
    exit 1
fi

echo "[build] Archivos fuente a compilar:"
for s in "${SRCS[@]}"; do echo "  - $s"; done

# Intento de compilación (1): sin librerías adicionales
echo "[build] Compilando (intento 1, sin librerías XDR adicionales)..."
if "$CXX" $CXXFLAGS "${SRCS[@]}" -o "$OUT"; then
    echo "[build] Compilación exitosa: ./$OUT"
    exit 0
fi

# Si falla, intentar con pkg-config para libtirpc (mejor compatibilidad)
echo "[build] Compilación fallida (intento 1). Intentando con 'pkg-config --cflags --libs libtirpc'..."
PKGFLAGS=""
if command -v pkg-config >/dev/null 2>&1 && pkg-config --exists libtirpc; then
    PKGFLAGS="$(pkg-config --cflags --libs libtirpc)"
    echo "[build] Usando flags de pkg-config: $PKGFLAGS"
    echo "[build] Comando: $CXX $CXXFLAGS $PKGFLAGS ${SRCS[*]} -o $OUT"
    if "$CXX" $CXXFLAGS $PKGFLAGS "${SRCS[@]}" -o "$OUT"; then
        echo "[build] Compilación exitosa con pkg-config libtirpc: ./$OUT"
        exit 0
    fi
else
    echo "[build] pkg-config no disponible o libtirpc no encontrado via pkg-config. Intentando detectar headers manualmente..."
fi

# Intentar detectar header en rutas comunes
if [ -f /usr/include/rpc/xdr.h ]; then
    echo "[build] Encontrado /usr/include/rpc/xdr.h, compilando enlazando -ltirpc..."
    echo "[build] Comando: $CXX $CXXFLAGS ${SRCS[*]} -o $OUT -ltirpc"
    if "$CXX" $CXXFLAGS "${SRCS[@]}" -o "$OUT" -ltirpc; then
        echo "[build] Compilación exitosa enlazando -ltirpc: ./$OUT"
        exit 0
    fi
fi

if [ -f /usr/include/tirpc/rpc/xdr.h ]; then
    echo "[build] Encontrado /usr/include/tirpc/rpc/xdr.h, compilando con -I/usr/include/tirpc -ltirpc..."
    echo "[build] Comando: $CXX $CXXFLAGS -I/usr/include/tirpc ${SRCS[*]} -o $OUT -ltirpc"
    if "$CXX" $CXXFLAGS -I/usr/include/tirpc "${SRCS[@]}" -o "$OUT" -ltirpc; then
        echo "[build] Compilación exitosa con -I/usr/include/tirpc -ltirpc: ./$OUT"
        exit 0
    fi
fi

# Por último, intentar con -ltirpc directo (sin verificar header). Esto puede fallar si falta el header.
echo "[build] Intentando con -ltirpc (sin verificar header)..."
echo "[build] Comando: $CXX $CXXFLAGS ${SRCS[*]} -o $OUT -ltirpc"
if "$CXX" $CXXFLAGS "${SRCS[@]}" -o "$OUT" -ltirpc; then
    echo "[build] Compilación exitosa con -ltirpc: ./$OUT"
    exit 0
fi

# Si aún falla, proveer instrucciones de instalación
cat <<'EOF' >&2
ERROR: La compilación falló incluso intentando enlazar con libtirpc.
Sugerencias:
  - En Debian/Ubuntu: sudo apt update && sudo apt install g++ libtirpc-dev pkg-config
  - En Fedora: sudo dnf install gcc-c++ libtirpc-devel
  - En Arch: sudo pacman -S gcc libtirpc

Comandos de depuración sugeridos:
  - Verifique si existe el header: ls /usr/include/rpc/xdr.h || ls /usr/include/tirpc/rpc/xdr.h
  - Intente compilar manualmente con pkg-config (si está instalado):
      g++ -std=c++17 -pthread $(pkg-config --cflags --libs libtirpc) src/*.cpp -o ProyectoESDatos
  - O con include explícito y enlace:
      g++ -std=c++17 -pthread -I/usr/include/tirpc src/*.cpp -o ProyectoESDatos -ltirpc

Si está en Windows, use WSL o un entorno Linux para compilar la parte XDR.
EOF

exit 1
