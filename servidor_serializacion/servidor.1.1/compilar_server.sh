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

# Si falla, intentar con -ltirpc y mostrar recomendaciones
echo "[build] Compilación fallida (intento 1). Intentando con '-ltirpc' (común para XDR en Linux)..."
if "$CXX" $CXXFLAGS "${SRCS[@]}" -o "$OUT" -ltirpc; then
    echo "[build] Compilación exitosa con -ltirpc: ./$OUT"
    exit 0
fi

# Si aún falla, proveer instrucciones de instalación
cat <<'EOF' >&2
ERROR: La compilación falló incluso intentando enlazar con -ltirpc.
Sugerencias:
  - En Debian/Ubuntu: sudo apt update && sudo apt install g++ libtirpc-dev pkg-config
  - En Fedora: sudo dnf install gcc-c++ libtirpc-devel
  - En Arch: sudo pacman -S gcc libtirpc

También puede intentar compilar manualmente con flags adicionales:
  g++ -std=c++17 -pthread src/*.cpp src/network/xdr_serialization.cpp -I. -o ProyectoESDatos -ltirpc

Si está en Windows, use WSL o un entorno Linux para compilar la parte XDR.
EOF

exit 1
