# Gluon

Verificador de hashes SHA-256 en C. Calcula o compara la huella digital de cualquier archivo usando OpenSSL EVP, con lectura en streaming (sin cargar el archivo entero en RAM).

Parte del ecosistema de herramientas de sistema Atom: **Quark**, **Neutron**, **Photon**, **Tachyon**, **Gluon**, **Electron**, **Átomo**.

## ¿Qué hace?

Gluon tiene dos modos:

```bash
# Calcular el SHA-256 de un archivo
gluon <archivo>

# Verificar un archivo contra un hash esperado
gluon <archivo> <hash_hex>

compilacion
gcc -Wall -Wextra src/gluon.c -o bin/gluon $(pkg-config --cflags --libs openssl)