#!/usr/bin/env bash
set -euo pipefail

# sync_file.sh
# Vigila un archivo y lo sincroniza automáticamente al host remoto usando rsync sobre SSH.
# Requisitos en la máquina origen: rsync, ssh y (opcional) inotifywait (inotify-tools).
# Uso: ./sync_file.sh <archivo_local> <usuario_remoto> <host_remoto> <ruta_remota> [puerto_ssh]

if [ "$#" -lt 4 ]; then
  echo "Usage: $0 <archivo_local> <usuario_remoto> <host_remoto> <ruta_remota> [puerto_ssh]"
  exit 1
fi

SRC_FILE="$1"
REMOTE_USER="$2"
REMOTE_HOST="$3"
REMOTE_PATH="$4"
SSH_PORT="${5:-22}"

if [ ! -f "$SRC_FILE" ]; then
  echo "Error: archivo '$SRC_FILE' no existe." >&2
  exit 2
fi

RSYNC_CMD=(rsync -az --progress -e "ssh -p ${SSH_PORT}")

push_file() {
  echo "[sync] Enviando '$SRC_FILE' -> ${REMOTE_USER}@${REMOTE_HOST}:${REMOTE_PATH}"
  "${RSYNC_CMD[@]}" "$SRC_FILE" "${REMOTE_USER}@${REMOTE_HOST}:$REMOTE_PATH" && echo "[sync] OK: $(date)"
}

if command -v inotifywait >/dev/null 2>&1; then
  echo "[sync] inotifywait detectado. Vigilar cambios en: $SRC_FILE"
  # Enviar primero al iniciar
  push_file
  while inotifywait -e close_write,modify "$SRC_FILE" >/dev/null 2>&1; do
    push_file
  done
else
  echo "[sync] inotifywait no encontrado. Usando comprobación por checksum cada 2s. Instala inotify-tools para mejor eficiencia."
  lastsum=""
  push_file
  while true; do
    if [ -f "$SRC_FILE" ]; then
      sum=$(sha1sum "$SRC_FILE" | awk '{print $1}')
      if [ "$sum" != "$lastsum" ]; then
        lastsum="$sum"
        push_file
      fi
    fi
    sleep 2
  done
fi
