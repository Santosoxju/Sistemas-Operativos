#include <unistd.h>   
#include <stdlib.h>
#include <stdio.h>    
#include <string.h>
#include <sys/wait.h>

int main() {
    char cad[50];   
    int tubo[2];//hijo a padre 
    int tubo2[2];//padre a hijo

    // Crear los pipes
    if (pipe(tubo) == -1 || pipe(tubo2) == -1) {
        perror("Error al crear pipes");
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) {
        // Proceso hijo
        close(tubo[0]); // Cierra el extremo de lectura del primer pipe
        close(tubo2[1]); // Cierra el extremo de escritura del segundo pipe

        while (1) {
            // Hijo lee respuesta del padre
            read(tubo2[0], cad, sizeof(cad));
            printf("Mensaje de papa a hijo (PID: %d): %s\n", getppid(), cad); // Mostrar el mensaje del padre

            // Terminar la conversación si se recibe "adios"
            if (strcmp(cad, "adios") == 0) {
                break; // Salir del bucle
            }

            // Mensaje para que el hijo escriba a papá
            printf("Hijo (PID: %d), ingrese un mensaje para papá: ", getpid());
            fgets(cad, sizeof(cad), stdin);
            cad[strcspn(cad, "\n")] = '\0'; // Eliminar el salto de línea
            write(tubo[1], cad, strlen(cad) + 1); // +1 para incluir el carácter nulo
        }

        // Cerrar los extremos restantes del hijo
        close(tubo[1]);
        close(tubo2[0]);
        exit(0);
    } else {
        // Proceso padre
        close(tubo[1]); // Cierra el extremo de escritura del primer pipe
        close(tubo2[0]); // Cierra el extremo de lectura del segundo pipe

        while (1) {
            // Mensaje para que el padre escriba al hijo
            printf("Padre (PID: %d), ingrese un mensaje para el hijo: ", getpid());
            fgets(cad, sizeof(cad), stdin);
            cad[strcspn(cad, "\n")] = '\0'; // Eliminar el salto de línea
            write(tubo2[1], cad, strlen(cad) + 1); // +1 para incluir el carácter nulo

            // Padre lee mensaje del hijo
            read(tubo[0], cad, sizeof(cad));
            printf("Mensaje de Hijo a papa (PID: %d): %s\n", getpid(), cad);

            // Terminar la conversación si el hijo dice "adios"
            if (strcmp(cad, "adios") == 0) {
                printf("Padre (PID: %d): Saliendo del programa.\n", getpid());
                break; // Salir del bucle
            }
        }

        // Cerrar los extremos restantes del padre
        close(tubo[0]);
        close(tubo2[1]);
        wait(NULL); // Esperar al proceso hijo
    }

    return 0;
}
