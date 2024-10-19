#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    int fd;
    char cad[50];

    // Crea el FIFO si no existe
    mkfifo("canal", 0666); // Permiso de lectura y escritura

    while (1) {
        // Amigo 2 lee el mensaje del amigo 1
        fd = open("canal", O_RDONLY);
        read(fd, cad, sizeof(cad)); // Lee del FIFO
        
        write(1, "Amigo 1 dice: ", 14); // Muestra el indicador de Amigo 1
        write(1, cad, strlen(cad)); // Muestra el mensaje recibido
        write(1, "\n", 1); // Nueva línea
        close(fd);

        // Verifica si el mensaje recibido es "adios"
        if (strcmp(cad, "adios") == 0) {
            break; // Sale del bucle si Amigo 1 dice "adios"
        }

        // Amigo 2 escribe su mensaje
        write(1, "Amigo 2: ", 9); // Muestra el indicador de Amigo 2
        read(0, cad, sizeof(cad)); // Lee desde la entrada estándar (descriptor 0)
        cad[strcspn(cad, "\n")] = 0; // Elimina el salto de línea

        fd = open("canal", O_WRONLY);
        write(fd, cad, strlen(cad) + 1); // Escribe el mensaje en el FIFO
        close(fd);

        // Verifica si se ha escrito "adios"
        if (strcmp(cad, "adios") == 0) {
            break; // Sale del bucle si se dice "adios"
        }
    }

    return 0;
}

