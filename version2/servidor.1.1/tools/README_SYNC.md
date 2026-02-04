# Sincronización de un único archivo (sync_file.sh)

Este pequeño utilitario permite vigilar un archivo local y sincronizarlo automáticamente a otra máquina remota usando `rsync` sobre `ssh`.

Requisitos (origen):
- `rsync` y `ssh` instalados.
- (Opcional) `inotifywait` (paquete `inotify-tools`) para detección instantánea de cambios.

Uso básico:

```bash
# en la máquina origen (ej. WSL)
./tools/sync_file.sh /ruta/al/archivo.txt usuario_remoto ip_remota /ruta/remota/en/servidor [puerto_ssh]
```

Ejemplo:

```bash
./tools/sync_file.sh reservas.toon ivan 10.9.5.230 /home/ivan/reservas 22
```

Qué hace:
- Envía inmediatamente el archivo al iniciar.
- Si `inotifywait` está disponible, espera eventos `close_write` o `modify` y vuelve a enviar el archivo cada vez que cambie.
- Si no hay `inotifywait`, usa un bucle de comprobación por checksum cada 2s.

Notas:
- En el host remoto debe existir el usuario y tener permisos para escribir en `ruta_remota`.
- Para uso desde Windows (WSL) el comando es idéntico si ejecutas dentro de WSL.
- Si no quieres usar `ssh`/`rsync`, puedo añadir alternativa con `nc`.
