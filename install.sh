#!/bin/sh
set -e

BIN=bin/gluon
DEST=/usr/local/bin/gluon

if [ ! -f "$BIN" ]; then
    echo "Error: $BIN no existe. Corre 'make' primero." >&2
    exit 1
fi

echo "Instalando gluon en $DEST (necesita sudo)..."
sudo install -Dm755 "$BIN" "$DEST"
echo "Listo. Prueba: gluon --help"