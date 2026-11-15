/*
 * gluon.c - Verificador de hashes SHA-256 en C
 * Copyright (C) 2026 Julio <tu-correo@ejemplo.com>
 *
 * Este programa es software libre: puedes redistribuirlo y/o modificarlo
 * bajo los términos de la GNU General Public License publicada por la
 * Free Software Foundation, ya sea la versión 3 de la Licencia, o
 * (a tu elección) cualquier versión posterior.
 *
 * Este programa se distribuye con la esperanza de que sea útil,
 * pero SIN NINGUNA GARANTÍA; ni siquiera la garantía implícita de
 * COMERCIABILIDAD o IDONEIDAD PARA UN PROPÓSITO PARTICULAR. Véase la
 * GNU General Public License para más detalles.
 *
 * Deberías haber recibido una copia de la GNU General Public License
 * junto con este programa. Si no, consulta <https://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <openssl/evp.h>

static int hex_nibble(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }

    if (c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }

    if (c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }

    return -1;
}

int hex_to_bytes(const char* hex, unsigned char out[32])
{
    if (strlen(hex) != 64)
    {
        return -1;
    }

    for (int i = 0; i < 32; i++)
    {
        int high = hex_nibble(hex[2 * i]);
        int low = hex_nibble(hex[2 * i + 1]);
        if (high < 0 || low < 0)
        {
            return -1;
        }

        out[i] = (unsigned char)((high << 4) | low);
    }

    return 0;
}

int sha256_file(const char* path, unsigned char out[32])
{
    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return -1;
    }

    EVP_MD_CTX* contexto = EVP_MD_CTX_new();
    if (!contexto)
    {
        close(fd);
        return -1;
    }

    if (EVP_DigestInit_ex(contexto, EVP_sha256(), NULL) != 1)
    {
        EVP_MD_CTX_free(contexto);
        close(fd);
        return -1;
    }

    unsigned char buffer[65536];
    ssize_t n;
    while ((n = read(fd, buffer, sizeof(buffer))) > 0)
    {
        if (EVP_DigestUpdate(contexto, buffer, (size_t) n) != 1)
        {
            EVP_MD_CTX_free(contexto);
            close(fd);
            return -1;
        }
    }

    if (n < 0)
    {
        perror("read");
        EVP_MD_CTX_free(contexto);
        close(fd);
        return -1;
    }

    unsigned int len = 0;
    if (EVP_DigestFinal_ex(contexto, out, &len) != 1)
    {
        EVP_MD_CTX_free(contexto);
        close(fd);
        return -1;
    }

    EVP_MD_CTX_free(contexto);

    close(fd);
    return 0;
}

int main(int argc, char * argv[])
{
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "uso:\n");
        fprintf(stderr, "  %s <archivo>              # calcula hash\n", argv[0]);
        fprintf(stderr, "  %s <archivo> <hash_hex>   # verifica\n", argv[0]);
        return 1;
    }
    else if (argc == 3)
    {

        unsigned char calculado[32];
        if (sha256_file(argv[1], calculado) != 0)
        {
            return 1;
        }

        unsigned char esperado[32];
        if (hex_to_bytes(argv[2], esperado) != 0)
        {
            fprintf(stderr, "hash hex inválido\n");
            return 1;
        }

        if (memcmp(calculado, esperado, 32) == 0)
        {
            printf("OK: %s\n", argv[1]);
            return 0;
        }
        else
        {
            printf("FALLO: %s\n", argv[1]);
            return 1;
        }
    }
    else if (argc == 2)
    {
        unsigned char hash[32];
        if (sha256_file(argv[1], hash) != 0)
        {
            return 1;
        }

        for (int i = 0; i < 32; i++)
        {
            printf("%02x", hash[i]);
        }

        printf("\n");

    }
    return 0;
}