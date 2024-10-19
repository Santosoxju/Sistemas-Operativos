#include <unistd.h>   
#include <stdlib.h>
#include <stdio.h>    
#include <string.h>
#include <sys/wait.h>

int main() {
    char cad[50];   
    int tubo[2];  
    int tubo2[2]; 

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
            printf("Mensaje de papá a hijo (PID: %d): %s\n", getppid(), cad); // Mostrar el mensaje del padre

            // Terminar la conversación si se recibe "adios hijo"
            if (strcmp(cad, "adios hijo") == 0) {
                break; // Salir del bucle
            }

            // Si el padre envía "tabla", el hijo solicita el número
            if (strcmp(cad, "tabla") == 0) {
                int numero;
                // Solicitar el número para la tabla de multiplicar
                printf("Hijo (PID: %d), ¿De qué número quieres la tabla de multiplicar? ", getpid());
                fgets(cad, sizeof(cad), stdin);
                cad[strcspn(cad, "\n")] = '\0'; // Eliminar el salto de línea
                numero = atoi(cad); // Convertir la cadena a un entero

                // Generar la tabla de multiplicar
                printf("Tabla de multiplicar del %d:\n", numero);
                for (int i = 1; i <= 10; i++) {
                    printf("%d x %d = %d\n", numero, i, numero * i);
                }

                // Enviar un mensaje al padre indicando que la tabla fue generada
                char mensaje[50];
                snprintf(mensaje, sizeof(mensaje), "Tabla de %d generada.", numero);
                write(tubo[1], mensaje, strlen(mensaje) + 1); // +1 para incluir el carácter nulo
            } else {
                // Mensaje para que el hijo escriba a papá
                printf("Hijo (PID: %d), ingrese un mensaje para papá: ", getpid());
                fgets(cad, sizeof(cad), stdin);
                cad[strcspn(cad, "\n")] = '\0'; // Eliminar el salto de línea
                write(tubo[1], cad, strlen(cad) + 1); // +1 para incluir el carácter nulo
            }
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
            printf("Mensaje de hijo a papá (PID: %d): %s\n", getpid(), cad);

            // Terminar la conversación si el hijo dice "adios hijo"
            if (strcmp(cad, "adios hijo") == 0) {
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
