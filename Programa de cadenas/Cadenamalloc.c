#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Prototipos de funciones
int contarVocalesStrchr(const char *cadena);
int esPalindromo(const char *cadena);

int main() {
    char *cadena1;
    char *cadena2;
    char *concatenacion;
    size_t max_len;
    int opcion;

    // Solicitar al usuario la longitud máxima esperada
    printf("Escribe la longitud máxima esperada para las cadenas: ");
    scanf("%zu", &max_len);
    getchar(); // Consumir el carácter de nueva línea que deja scanf

    // Asignar memoria dinámica para las cadenas y la concatenación
    cadena1 = (char *)malloc((max_len + 1) * sizeof(char));
    cadena2 = (char *)malloc((max_len + 1) * sizeof(char));
    concatenacion = (char *)malloc((2 * max_len + 1) * sizeof(char));

    if (cadena1 == NULL || cadena2 == NULL || concatenacion == NULL) {
        printf("Error en la asignación de memoria.\n");
        return 1;
    }

    // Solicitar cadenas al usuario
    printf("Escribe la primera cadena: ");
    fgets(cadena1, max_len + 1, stdin);
    cadena1[strcspn(cadena1, "\n")] = '\0'; // Eliminar el salto de línea

    printf("Escribe la segunda cadena: ");
    fgets(cadena2, max_len + 1, stdin);
    cadena2[strcspn(cadena2, "\n")] = '\0'; // Eliminar el salto de línea

    do {
        // Mostrar el menú de opciones
        printf("\n--- Menú de Operaciones ---\n");
        printf("1. Longitud de las cadenas\n");
        printf("2. Concatenar las cadenas\n");
        printf("3. Comparar las cadenas\n");
        printf("4. Copiar las cadenas\n");
        printf("5. Contar el número de vocales\n");
        printf("6. Verificar si las cadenas son palíndromos\n");
        printf("7. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        getchar(); // Consumir el carácter de nueva línea que deja scanf

        switch (opcion) {
            case 1: // A) Longitud
                printf("Longitud de la primera cadena: %lu\n", strlen(cadena1));
                printf("Longitud de la segunda cadena: %lu\n", strlen(cadena2));
                break;

            case 2: // B) Concatenación
                strcpy(concatenacion, cadena1);
                strcat(concatenacion, cadena2);
                printf("Cadena concatenada: %s\n", concatenacion);
                break;

            case 3: // C) Comparar
                if (strcmp(cadena1, cadena2) == 0) {
                    printf("Las cadenas son iguales.\n");
                } else if (strcmp(cadena1, cadena2) < 0) {
                    printf("La primera cadena es menor que la segunda.\n");
                } else {
                    printf("La primera cadena es mayor que la segunda.\n");
                }
                break;

            case 4: // D) Copiar
                {
                    char *copia1 = (char *)malloc((strlen(cadena1) + 1) * sizeof(char));
                    char *copia2 = (char *)malloc((strlen(cadena2) + 1) * sizeof(char));
                    if (copia1 == NULL || copia2 == NULL) {
                        printf("Error en la asignación de memoria para las copias.\n");
                        return 1;
                    }
                    strcpy(copia1, cadena1);
                    strcpy(copia2, cadena2);
                    printf("Copia de la primera cadena: %s\n", copia1);
                    printf("Copia de la segunda cadena: %s\n", copia2);
                    free(copia1);
                    free(copia2);
                }
                break;

            case 5: // Contar el número de vocales
                printf("Número de vocales en la primera cadena: %d\n", contarVocalesStrchr(cadena1));
                printf("Número de vocales en la segunda cadena: %d\n", contarVocalesStrchr(cadena2));
                break;

            case 6: // Verificar si es un palíndromo
                printf("La primera cadena %s un palíndromo.\n", esPalindromo(cadena1) ? "es" : "no es");
                printf("La segunda cadena %s un palíndromo.\n", esPalindromo(cadena2) ? "es" : "no es");
                break;

            case 7: // Salir
                printf("Saliendo del programa...\n");
                break;

            default:
                printf("Opción no válida. Por favor, seleccione una opción del 1 al 7.\n");
        }
    } while (opcion != 7); // Repetir hasta que el usuario elija salir

    // Liberar la memoria asignada
    free(cadena1);
    free(cadena2);
    free(concatenacion);

    return 0;
}

// Función para contar el número de vocales en una cadena
int contarVocalesStrchr(const char *cadena) {
    int conteo = 0;
    const char *vocales = "aeiouAEIOU";
    while (*cadena) {
        if (strchr(vocales, *cadena)) {
            conteo++;
        }
        cadena++;
    }
    return conteo;
}

// Función para verificar si una cadena es un palíndromo
int esPalindromo(const char *cadena) {
    int inicio = 0;
    int fin = strlen(cadena) - 1;
    while (inicio < fin) {
        if (cadena[inicio] != cadena[fin]) {
            return 0; // No es un palíndromo
        }
        inicio++;
        fin--;
    }
    return 1; // Es un palíndromo
}
