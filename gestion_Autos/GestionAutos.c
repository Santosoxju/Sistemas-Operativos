/*usando l atabla de telefonos como u menu independiete*/


#include <stdlib.h>
#include <libpq-fe.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

char cad[1000];

void AltaPropietario();
void ConsulPropietario();
void ActualiPropietario();
void EliminarPropietario();
void MenuPropietario();

void AltaAuto();
void ConsulAuto();
void ActualiAuto();
void EliminarAuto();
void MenuAuto();

void AltaAccidente();
void ConsulAccidente();
void ActualiAccidente();
void EliminarAccidente();
void MenuAccidente();

void AltaTelefono();
void ConsulTelefono();
void ActualiTelefono();
void EliminarTelefono();
void MenuTelefono();

void MenuPrincipal();

int main(int argc, char *argv[]) {
    MenuPrincipal();
    return 0;
}

// Menú principal
void MenuPrincipal() {
    int opcion;
    while (1) {
        printf("\n----------Menú Principal----------\n");
        printf("1. Menú Propietario\n");
        printf("2. Menú Auto\n");
        printf("3. Menú Accidente\n");
        printf("4. Menú Teléfono de Propietario\n");
        printf("5. Salir\n");
        printf("\nElija una opción: ");
        scanf("%d", &opcion);

        pid_t pid;

        switch (opcion) {
            case 1:
                pid = fork();
                if (pid == -1) {
                    perror("Error al crear el proceso hijo");
                    exit(EXIT_FAILURE);
                } else if (pid == 0) {
                    // En el proceso hijo
                    printf("\n----------Soy el proceso hijo (Menú Propietario) con PID %d----------\n", getpid());
                    MenuPropietario();
                    exit(EXIT_SUCCESS);
                }
                wait(&pid);
                break;
            case 2:
                pid = fork();
                if (pid == -1) {
                    perror("Error al crear el proceso hijo");
                    exit(EXIT_FAILURE);
                } else if (pid == 0) {
                    printf("\n----------Soy el proceso hijo (Menú Auto) con PID %d----------\n", getpid());
                    MenuAuto();
                    exit(EXIT_SUCCESS);
                }
                wait(&pid);
                break;
            case 3:
                pid = fork();
                if (pid == -1) {
                    perror("Error al crear el proceso hijo");
                    exit(EXIT_FAILURE);
                } else if (pid == 0) {
                    printf("\n----------Soy el proceso hijo (Menú Accidente) con PID %d----------\n", getpid());
                    MenuAccidente();
                    exit(EXIT_SUCCESS);
                }
                wait(&pid);
                break;
            case 4:
                pid = fork();
                if (pid == -1) {
                    perror("Error al crear el proceso hijo");
                    exit(EXIT_FAILURE);
                } else if (pid == 0) {
                    printf("\n----------Soy el proceso hijo (Menú Teléfono) con PID %d----------\n", getpid());
                    MenuTelefono();
                    exit(EXIT_SUCCESS);
                }
                wait(&pid);
                break;
            case 5:
                printf("Saliendo del programa...\n");
                exit(0);
            default:
                printf("Opción no válida\n");
                break;
        }
    }
}

// Menú de Propietario
void MenuPropietario() {
    int opcion;
    while (1) {
        printf("\n----------Menú Propietario----------\n");
        printf("1. Alta Propietario\n");
        printf("2. Consultar todos los Propietarios\n");
        printf("3. Actualizar datos de un Propietario\n");
        printf("4. Eliminar Propietario\n");
        printf("5. Salir\n");
        printf("\nElija una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                AltaPropietario();
                break;
            case 2:
                ConsulPropietario();
                break;
            case 3:
                ActualiPropietario();
                break;
            case 4:
                EliminarPropietario();
                break;
            case 5:
                printf("Saliendo del menú Propietario\n");
                exit(0);
            default:
                printf("Opción no válida\n");
                break;
        }
    }
}

// Funciones de Propietario
void AltaPropietario() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    char nombre[255], ap_paterno[255], ap_materno[255], fecha_nacimiento[11];
    
    printf("Ingrese los datos del Propietario:\n");
    printf("Nombre: ");
    scanf("%s", nombre);
    printf("Apellido Paterno: ");
    scanf("%s", ap_paterno);
    printf("Apellido Materno: ");
    scanf("%s", ap_materno);
    printf("Fecha de Nacimiento (YYYY-MM-DD): ");
    scanf("%s", fecha_nacimiento);
    

    char query[1000];
    sprintf(query, "INSERT INTO propietario (nombre, ap_paterno, ap_materno, fecha_nacimiento) VALUES ('%s', '%s', '%s', '%s')",
            nombre, ap_paterno, ap_materno, fecha_nacimiento);
            
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error en la ejecución de la consulta: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }
    PQclear(res);
    PQfinish(conn);
    printf("Propietario insertado exitosamente.\n");
}

void ConsulPropietario() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    PGresult *resultado;
    int i, j, columna, fila;
    sprintf(cad, "SELECT * FROM propietario");
    resultado = PQexec(conn, cad);

    if (PQresultStatus(resultado) == PGRES_TUPLES_OK) {
        fila = PQntuples(resultado);
        columna = PQnfields(resultado);
        for (i = 0; i < fila; i++) {
            for (j = 0; j < columna; j++) {
                printf("%s  ", PQgetvalue(resultado, i, j));
            }
            printf("\n");
        }
    } else {
        printf("Error al ejecutar la consulta: %s\n", PQerrorMessage(conn));
    }
    PQclear(resultado);
    PQfinish(conn);
}

void ActualiPropietario() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    char id_propietario[20], nombre[255], ap_paterno[255], ap_materno[255], fecha_nacimiento[11];
    printf("Ingrese el ID del Propietario: ");
    scanf("%s", id_propietario);
    printf("Nombre: ");
    scanf("%s", nombre);
    printf("Apellido Paterno: ");
    scanf("%s", ap_paterno);
    printf("Apellido Materno: ");
    scanf("%s", ap_materno);
    printf("Fecha de Nacimiento (YYYY-MM-DD): ");
    scanf("%s", fecha_nacimiento);
    

    char query[1000];
    sprintf(query, "UPDATE propietario SET nombre = '%s', ap_paterno = '%s', ap_materno = '%s', fecha_nacimiento = %d WHERE id_propietario = %s",
            nombre, ap_paterno, ap_materno, fecha_nacimiento, id_propietario);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Update failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }
    PQclear(res);
    PQfinish(conn);
    printf("Actualización exitosa.\n");
}

void EliminarPropietario() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    char id_propietario_str[100];
    int id_propietario;

    printf("Ingrese el ID del Propietario a eliminar: ");
    scanf("%s", id_propietario_str);

    id_propietario = atoi(id_propietario_str);

    char query[1000];
    sprintf(query, "DELETE FROM propietario WHERE id_propietario = %d", id_propietario);

    PGresult *result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error al eliminar el propietario: %s\n", PQerrorMessage(conn));
        PQclear(result);
        PQfinish(conn);
        exit(1);
    }

    printf("Propietario eliminado exitosamente\n");
    PQclear(result);
    PQfinish(conn);
}

// Menú de Auto
void MenuAuto() {
    int opcion;
    while (1) {
        printf("\n----------Menú Auto----------\n");
        printf("1. Alta Auto\n");
        printf("2. Consultar todos los Autos\n");
        printf("3. Actualizar datos de un Auto\n");
        printf("4. Eliminar Auto\n");
        printf("5. Salir\n");
        printf("\nElija una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                AltaAuto();
                break;
            case 2:
                ConsulAuto();
                break;
            case 3:
                ActualiAuto();
                break;
            case 4:
                EliminarAuto();
                break;
            case 5:
                printf("Saliendo del menú Auto\n");
                exit(0);
            default:
                printf("Opción no válida\n");
                break;
        }
    }
}

// Funciones de Auto
void AltaAuto() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    int id_propietario;
    char marca[255], tipo[255], color[255], placa[255];
    int modelo;

    printf("Ingrese los datos del Auto:\n");
    printf("ID Propietario: ");
    scanf("%d", &id_propietario);
    printf("Marca: ");
    scanf("%s", marca);
    printf("Modelo (año): ");
    scanf("%d", &modelo);
    printf("Tipo: ");
    scanf("%s", tipo);
    printf("Color: ");
    scanf("%s", color);
    printf("Placa: ");
    scanf("%s", placa);

    char query[1000];
    sprintf(query, "INSERT INTO auto (id_propietario, marca, modelo, tipo, color, placa) VALUES (%d, '%s', %d, '%s', '%s', '%s')",
            id_propietario, marca, modelo, tipo, color, placa);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error en la ejecución de la consulta: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }
    PQclear(res);
    PQfinish(conn);
    printf("Auto insertado exitosamente.\n");
}

void ConsulAuto() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    PGresult *resultado;
    int i, j, columna, fila;
    sprintf(cad, "SELECT * FROM auto");
    resultado = PQexec(conn, cad);

    if (PQresultStatus(resultado) == PGRES_TUPLES_OK) {
        fila = PQntuples(resultado);
        columna = PQnfields(resultado);
        for (i = 0; i < fila; i++) {
            for (j = 0; j < columna; j++) {
                printf("%s  ", PQgetvalue(resultado, i, j));
            }
            printf("\n");
        }
    } else {
        printf("Error al ejecutar la consulta: %s\n", PQerrorMessage(conn));
    }
    PQclear(resultado);
    PQfinish(conn);
}

void ActualiAuto() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    int id_auto, id_propietario;
    char marca[255], tipo[255], color[255], placa[255];
    int modelo;

    printf("Ingrese el ID del Auto: ");
    scanf("%d", &id_auto);
    printf("ID Propietario: ");
    scanf("%d", &id_propietario);
    printf("Marca: ");
    scanf("%s", marca);
    printf("Modelo (año): ");
    scanf("%d", &modelo);
    printf("Tipo: ");
    scanf("%s", tipo);
    printf("Color: ");
    scanf("%s", color);
    printf("Placa: ");
    scanf("%s", placa);

    char query[1000];
    sprintf(query, "UPDATE auto SET id_propietario = %d, marca = '%s', modelo = %d, tipo = '%s', color = '%s', placa = '%s' WHERE id_auto = %d",
            id_propietario, marca, modelo, tipo, color, placa, id_auto);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Update failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }
    PQclear(res);
    PQfinish(conn);
    printf("Actualización exitosa.\n");
}

void EliminarAuto() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    int id_auto;
    printf("Ingrese el ID del Auto a eliminar: ");
    scanf("%d", &id_auto);

    char query[1000];
    sprintf(query, "DELETE FROM auto WHERE id_auto = %d", id_auto);

    PGresult *result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error al eliminar el auto: %s\n", PQerrorMessage(conn));
        PQclear(result);
        PQfinish(conn);
        exit(1);
    }

    printf("Auto eliminado exitosamente\n");
    PQclear(result);
    PQfinish(conn);
}

// Menú de Accidente
void MenuAccidente() {
    int opcion;
    while (1) {
        printf("\n----------Menú Accidente----------\n");
        printf("1. Alta Accidente\n");
        printf("2. Consultar todos los Accidentes\n");
        printf("3. Actualizar datos de un Accidente\n");
        printf("4. Eliminar Accidente\n");
        printf("5. Salir\n");
        printf("\nElija una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                AltaAccidente();
                break;
            case 2:
                ConsulAccidente();
                break;
            case 3:
                ActualiAccidente();
                break;
            case 4:
                EliminarAccidente();
                break;
            case 5:
                printf("Saliendo del menú Accidente\n");
                exit(0);
            default:
                printf("Opción no válida\n");
                break;
        }
    }
}

// Funciones de Accidente
void AltaAccidente() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    int id_auto;
    char fecha[11], hora[6];

    printf("Ingrese los datos del Accidente:\n");
    printf("ID Auto: ");
    scanf("%d", &id_auto);
    printf("Fecha (YYYY-MM-DD): ");
    scanf("%s", fecha);
    printf("Hora (HH:MM): ");
    scanf("%s", hora);
    //printf("Lugar: ");
    //scanf("%s", lugar);

    char query[1000];
    sprintf(query, "INSERT INTO accidente (id_auto, fecha, hora) VALUES (%d, '%s', '%s')",
            id_auto, fecha, hora);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error en la ejecución de la consulta: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }
    PQclear(res);
    PQfinish(conn);
    printf("Accidente insertado exitosamente.\n");
}

void ConsulAccidente() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    PGresult *resultado;
    int i, j, columna, fila;
    sprintf(cad, "SELECT * FROM accidente");
    resultado = PQexec(conn, cad);

    if (PQresultStatus(resultado) == PGRES_TUPLES_OK) {
        fila = PQntuples(resultado);
        columna = PQnfields(resultado);
        for (i = 0; i < fila; i++) {
            for (j = 0; j < columna; j++) {
                printf("%s  ", PQgetvalue(resultado, i, j));
            }
            printf("\n");
        }
    } else {
        printf("Error al ejecutar la consulta: %s\n", PQerrorMessage(conn));
    }
    PQclear(resultado);
    PQfinish(conn);
}

void ActualiAccidente() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    int no_accidente, id_auto;
    char fecha[11], hora[6];

    printf("Ingrese el ID del Accidente: ");
    scanf("%d", &no_accidente);
    printf("ID Auto: ");
    scanf("%d", &id_auto);
    printf("Fecha (YYYY-MM-DD): ");
    scanf("%s", fecha);
    printf("Hora (HH:MM): ");
    scanf("%s", hora);


    char query[1000];
    sprintf(query, "UPDATE accidente SET id_auto = %d, fecha = '%s', hora = '%s' WHERE no_accidente = %d",
            id_auto, fecha, hora, no_accidente);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Update failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }
    PQclear(res);
    PQfinish(conn);
    printf("Actualización exitosa.\n");
}

void EliminarAccidente() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    int no_accidente;
    printf("Ingrese el ID del Accidente a eliminar: ");
    scanf("%d", &no_accidente);

    char query[1000];
    sprintf(query, "DELETE FROM accidente WHERE no_accidente = %d", no_accidente);

    PGresult *result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error al eliminar el accidente: %s\n", PQerrorMessage(conn));
        PQclear(result);
        PQfinish(conn);
        exit(1);
    }

    printf("Accidente eliminado exitosamente\n");
    PQclear(result);
    PQfinish(conn);
}

// Menú de Teléfono
void MenuTelefono() {
    int opcion;
    while (1) {
        printf("\n----------Menú Teléfono de Propietario----------\n");
        printf("1. Alta Teléfono\n");
        printf("2. Consultar todos los Teléfonos\n");
        printf("3. Actualizar datos de un Teléfono\n");
        printf("4. Eliminar Teléfono\n");
        printf("5. Salir\n");
        printf("\nElija una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                AltaTelefono();
                break;
            case 2:
                ConsulTelefono();
                break;
            case 3:
                ActualiTelefono();
                break;
            case 4:
                EliminarTelefono();
                break;
            case 5:
                printf("Saliendo del menú Teléfono\n");
                exit(0);
            default:
                printf("Opción no válida\n");
                break;
        }
    }
}

// Funciones de Teléfono
void AltaTelefono() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    int id_propietario;
    char telefono[255];

    printf("Ingrese los datos del Teléfono:\n");
    printf("ID Propietario: ");
    scanf("%d", &id_propietario);
    printf("Teléfono: ");
    scanf("%s", telefono);

    char query[1000];
    sprintf(query, "INSERT INTO telefono (id_propietario, telefono) VALUES (%d, '%s')",
            id_propietario, telefono);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error en la ejecución de la consulta: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }
    PQclear(res);
    PQfinish(conn);
    printf("Teléfono insertado exitosamente.\n");
}

void ConsulTelefono() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    PGresult *resultado;
    int i, j, columna, fila;
    sprintf(cad, "SELECT * FROM telefono");
    resultado = PQexec(conn, cad);

    if (PQresultStatus(resultado) == PGRES_TUPLES_OK) {
        fila = PQntuples(resultado);
        columna = PQnfields(resultado);
        for (i = 0; i < fila; i++) {
            for (j = 0; j < columna; j++) {
                printf("%s  ", PQgetvalue(resultado, i, j));
            }
            printf("\n");
        }
    } else {
        printf("Error al ejecutar la consulta: %s\n", PQerrorMessage(conn));
    }
    PQclear(resultado);
    PQfinish(conn);
}

void ActualiTelefono() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    int id_telefono, id_propietario;
    char telefono[255];

    printf("Ingrese el ID del Teléfono: ");
    scanf("%d", &id_telefono);
    printf("ID Propietario: ");
    scanf("%d", &id_propietario);
    printf("Teléfono: ");
    scanf("%s", telefono);

    char query[1000];
    sprintf(query, "UPDATE telefono SET id_propietario = %d, telefono = '%s' WHERE id_telefono = %d",
            id_propietario, telefono, id_telefono);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Update failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }
    PQclear(res);
    PQfinish(conn);
    printf("Actualización exitosa.\n");
}

void EliminarTelefono() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    int id_telefono;
    printf("Ingrese el ID del Teléfono a eliminar: ");
    scanf("%d", &id_telefono);

    char query[1000];
    sprintf(query, "DELETE FROM telefono WHERE id_telefono = %d", id_telefono);

    PGresult *result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error al eliminar el teléfono: %s\n", PQerrorMessage(conn));
        PQclear(result);
        PQfinish(conn);
        exit(1);
    }

    printf("Teléfono eliminado exitosamente\n");
    PQclear(result);
    PQfinish(conn);
}

