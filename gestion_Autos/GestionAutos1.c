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
        printf("4. Salir\n");
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
                    printf("Soy el proceso hijo (Menú Propietario) con PID %d y mi proceso padre tiene PID %d\n", getpid(), getppid());
                    MenuPropietario();
                    exit(EXIT_SUCCESS);
                }
                wait(NULL);  // Espera a que el proceso hijo termine
                break;
            case 2:
                pid = fork();
                if (pid == -1) {
                    perror("Error al crear el proceso hijo");
                    exit(EXIT_FAILURE);
                } else if (pid == 0) {
                    // En el proceso hijo
                    printf("Soy el proceso hijo (Menú Auto) con PID %d y mi proceso padre tiene PID %d\n", getpid(), getppid());
                    MenuAuto();
                    exit(EXIT_SUCCESS);
                }
                wait(NULL);  // Espera a que el proceso hijo termine
                break;
            case 3:
                pid = fork();
                if (pid == -1) {
                    perror("Error al crear el proceso hijo");
                    exit(EXIT_FAILURE);
                } else if (pid == 0) {
                    // En el proceso hijo
                    printf("Soy el proceso hijo (Menú Accidente) con PID %d y mi proceso padre tiene PID %d\n", getpid(), getppid());
                    MenuAccidente();
                    exit(EXIT_SUCCESS);
                }
                wait(NULL);  // Espera a que el proceso hijo termine
                break;
            case 4:
                printf("Saliendo del programa...\n");
                exit(0);
            default:
                printf("Opción no válida\n");
                break;
        }
    }

    return 0;
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
// Función para establecer la conexión a la base de datos
PGconn* conectarBD() {
    PGconn *conn = PQconnectdb("dbname=gestion_autos user=postgres password=postgres host=localhost port=5432");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Error de conexión: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    return conn;
}

// Función para cerrar la conexión a la base de datos
void cerrarBD(PGconn* conn) {
    PQfinish(conn);
}

// Función para dar de alta un propietario
void AltaPropietario() {
    PGconn *conn = conectarBD();

    char nombre[255], ap_paterno[255], ap_materno[255], fecha_nacimiento[11], telefono[255];
    int id_propietario;

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
    sprintf(query, "INSERT INTO propietario (nombre, ap_paterno, ap_materno, fecha_nacimiento) VALUES ('%s', '%s', '%s', '%s') RETURNING id_propietario",
            nombre, ap_paterno, ap_materno, fecha_nacimiento);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Error en la ejecución de la consulta: %s\n", PQerrorMessage(conn));
        PQclear(res);
        cerrarBD(conn);
        exit(1);
    }

    id_propietario = atoi(PQgetvalue(res, 0, 0));
    PQclear(res);

    printf("Ingrese el Teléfono del Propietario: ");
    scanf("%s", telefono);

    sprintf(query, "INSERT INTO telefono (id_propietario, telefono) VALUES (%d, '%s')", id_propietario, telefono);
    res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error al insertar el teléfono: %s\n", PQerrorMessage(conn));
        PQclear(res);
        cerrarBD(conn);
        exit(1);
    }
    PQclear(res);
    cerrarBD(conn);

    printf("Propietario y Teléfono insertados exitosamente.\n");
}

// Función para consultar propietarios
void ConsulPropietario() {
    PGconn *conn = conectarBD();

    PGresult *resultado;
    char query[1000];
    sprintf(query, "SELECT p.id_propietario, p.nombre, p.ap_paterno, p.ap_materno, p.fecha_nacimiento, t.telefono "
                   "FROM propietario p LEFT JOIN telefono t ON p.id_propietario = t.id_propietario");

    resultado = PQexec(conn, query);
    if (PQresultStatus(resultado) == PGRES_TUPLES_OK) {
        int fila = PQntuples(resultado);
        int columna = PQnfields(resultado);
        for (int i = 0; i < fila; i++) {
            for (int j = 0; j < columna; j++) {
                printf("%s  ", PQgetvalue(resultado, i, j));
            }
            printf("\n");
        }
    } else {
        printf("Error al ejecutar la consulta: %s\n", PQerrorMessage(conn));
    }
    PQclear(resultado);
    cerrarBD(conn);
}

// Función para actualizar un propietario
void ActualiPropietario() {
    PGconn *conn = conectarBD();
    int id_propietario;
    char nombre[255] = "", ap_paterno[255] = "", ap_materno[255] = "", fecha_nacimiento[11] = "", telefono[255] = "";
    int opcion;
    int modificar_nombre = 0, modificar_ap_paterno = 0, modificar_ap_materno = 0, modificar_fecha_nacimiento = 0, modificar_telefono = 0;

    printf("Ingrese el ID del Propietario: ");
    scanf("%d", &id_propietario);

    // Menú para seleccionar los campos a modificar
    do {
        printf("\nSeleccione qué campo desea modificar:\n");
        printf("1. Nombre\n");
        printf("2. Apellido Paterno\n");
        printf("3. Apellido Materno\n");
        printf("4. Fecha de Nacimiento\n");
        printf("5. Teléfono\n");
        printf("6. Finalizar selección y guardar cambios\n");
        printf("Opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Nuevo Nombre: ");
                scanf("%s", nombre);
                modificar_nombre = 1;
                break;
            case 2:
                printf("Nuevo Apellido Paterno: ");
                scanf("%s", ap_paterno);
                modificar_ap_paterno = 1;
                break;
            case 3:
                printf("Nuevo Apellido Materno: ");
                scanf("%s", ap_materno);
                modificar_ap_materno = 1;
                break;
            case 4:
                printf("Nueva Fecha de Nacimiento (YYYY-MM-DD): ");
                scanf("%s", fecha_nacimiento);
                modificar_fecha_nacimiento = 1;
                break;
            case 5:
                printf("Nuevo Teléfono: ");
                scanf("%s", telefono);
                modificar_telefono = 1;
                break;
            case 6:
                printf("Finalizando selección...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
                break;
        }
    } while (opcion != 6);

    char query[1000] = "UPDATE propietario SET ";
    int first = 1; // Para controlar la inclusión de comas entre los campos

    if (modificar_nombre) {
        sprintf(query + strlen(query), "%snombre = '%s'", first ? "" : ", ", nombre);
        first = 0;
    }
    if (modificar_ap_paterno) {
        sprintf(query + strlen(query), "%sap_paterno = '%s'", first ? "" : ", ", ap_paterno);
        first = 0;
    }
    if (modificar_ap_materno) {
        sprintf(query + strlen(query), "%sap_materno = '%s'", first ? "" : ", ", ap_materno);
        first = 0;
    }
    if (modificar_fecha_nacimiento) {
        sprintf(query + strlen(query), "%sfecha_nacimiento = '%s'", first ? "" : ", ", fecha_nacimiento);
        first = 0;
    }

    if (first == 1) {
        // No se seleccionó ningún campo para modificar
        printf("No se seleccionó ningún campo para modificar en la tabla 'propietario'.\n");
    } else {
        // Ejecutar la consulta solo si hay campos para modificar
        sprintf(query + strlen(query), " WHERE id_propietario = %d", id_propietario);
        PGresult *res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Error al actualizar el propietario: %s\n", PQerrorMessage(conn));
            PQclear(res);
            cerrarBD(conn);
            exit(1);
        }
        PQclear(res);
        printf("Propietario actualizado correctamente.\n");
    }

    // Actualizar el teléfono si se seleccionó modificarlo
    if (modificar_telefono) {
        sprintf(query, "UPDATE telefono SET telefono = '%s' WHERE id_propietario = %d", telefono, id_propietario);
        PGresult *res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Error al actualizar el teléfono: %s\n", PQerrorMessage(conn));
            PQclear(res);
            cerrarBD(conn);
            exit(1);
        }
        PQclear(res);
        printf("Teléfono actualizado correctamente.\n");
    }

    cerrarBD(conn);
    printf("Actualización completada.\n");
}


// Función para eliminar un propietario
void EliminarPropietario() {
    PGconn *conn = conectarBD();

    int id_propietario;
    printf("Ingrese el ID del Propietario a eliminar: ");
    scanf("%d", &id_propietario);

    char query[1000];

    sprintf(query, "DELETE FROM telefono WHERE id_propietario = %d", id_propietario);
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error al eliminar el teléfono: %s\n", PQerrorMessage(conn));
        PQclear(res);
        cerrarBD(conn);
        exit(1);
    }
    PQclear(res);

    sprintf(query, "DELETE FROM propietario WHERE id_propietario = %d", id_propietario);
    res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error al eliminar el propietario: %s\n", PQerrorMessage(conn));
        PQclear(res);
        cerrarBD(conn);
        exit(1);
    }
    PQclear(res);
    cerrarBD(conn);

    printf("Propietario y su Teléfono eliminados exitosamente.\n");
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
    PGconn *conn = conectarBD();

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
    PGconn *conn = conectarBD();

    PGresult *resultado;
    char query[1000];
    sprintf(query, "SELECT * FROM auto");

    resultado = PQexec(conn, query);
    if (PQresultStatus(resultado) == PGRES_TUPLES_OK) {
        int fila = PQntuples(resultado);
        int columna = PQnfields(resultado);
        for (int i = 0; i < fila; i++) {
            for (int j = 0; j < columna; j++) {
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

// Función para actualizar un auto
void ActualiAuto() {
    PGconn *conn = conectarBD();

    int id_auto;
    int id_propietario = 0, modelo = 0;
    char marca[255] = "", tipo[255] = "", color[255] = "", placa[255] = "";
    int opcion;
    int modificar_id_propietario = 0, modificar_marca = 0, modificar_modelo = 0, modificar_tipo = 0, modificar_color = 0, modificar_placa = 0;

    printf("Ingrese el ID del Auto: ");
    scanf("%d", &id_auto);

    // Menú para seleccionar los campos a modificar
    do {
        printf("\nSeleccione qué campo desea modificar:\n");
        printf("1. ID Propietario\n");
        printf("2. Marca\n");
        printf("3. Modelo\n");
        printf("4. Tipo\n");
        printf("5. Color\n");
        printf("6. Placa\n");
        printf("7. Finalizar selección y guardar cambios\n");
        printf("Opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Nuevo ID Propietario: ");
                scanf("%d", &id_propietario);
                modificar_id_propietario = 1;
                break;
            case 2:
                printf("Nueva Marca: ");
                scanf("%s", marca);
                modificar_marca = 1;
                break;
            case 3:
                printf("Nuevo Modelo (año): ");
                scanf("%d", &modelo);
                modificar_modelo = 1;
                break;
            case 4:
                printf("Nuevo Tipo: ");
                scanf("%s", tipo);
                modificar_tipo = 1;
                break;
            case 5:
                printf("Nuevo Color: ");
                scanf("%s", color);
                modificar_color = 1;
                break;
            case 6:
                printf("Nueva Placa: ");
                scanf("%s", placa);
                modificar_placa = 1;
                break;
            case 7:
                printf("Finalizando selección...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
                break;
        }
    } while (opcion != 7);

    char query[1000] = "UPDATE auto SET ";
    int first = 1; // Para controlar la inclusión de comas entre los campos

    if (modificar_id_propietario) {
        sprintf(query + strlen(query), "%sid_propietario = %d", first ? "" : ", ", id_propietario);
        first = 0;
    }
    if (modificar_marca) {
        sprintf(query + strlen(query), "%smarca = '%s'", first ? "" : ", ", marca);
        first = 0;
    }
    if (modificar_modelo) {
        sprintf(query + strlen(query), "%smodelo = %d", first ? "" : ", ", modelo);
        first = 0;
    }
    if (modificar_tipo) {
        sprintf(query + strlen(query), "%stipo = '%s'", first ? "" : ", ", tipo);
        first = 0;
    }
    if (modificar_color) {
        sprintf(query + strlen(query), "%scolor = '%s'", first ? "" : ", ", color);
        first = 0;
    }
    if (modificar_placa) {
        sprintf(query + strlen(query), "%splaca = '%s'", first ? "" : ", ", placa);
        first = 0;
    }

    if (first == 1) {
        // No se seleccionó ningún campo para modificar
        printf("No se seleccionó ningún campo para modificar en la tabla 'auto'.\n");
    } else {
        // Ejecutar la consulta solo si hay campos para modificar
        sprintf(query + strlen(query), " WHERE id_auto = %d", id_auto);
        PGresult *res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Error al actualizar el auto: %s\n", PQerrorMessage(conn));
            PQclear(res);
            PQfinish(conn);
            exit(1);
        }
        PQclear(res);
        printf("Auto actualizado correctamente.\n");
    }

    PQfinish(conn);
    printf("Actualización completada.\n");
}


void EliminarAuto() {
    PGconn *conn = conectarBD();

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
    PGconn *conn = conectarBD();

    int id_auto;
    char fecha[11], hora[6];

    printf("Ingrese los datos del Accidente:\n");
    printf("ID Auto: ");
    scanf("%d", &id_auto);
    printf("Fecha (YYYY-MM-DD): ");
    scanf("%s", fecha);
    printf("Hora (HH:MM): ");
    scanf("%s", hora);

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
    PGconn *conn = conectarBD();

    PGresult *resultado;
    char query[1000];
    sprintf(query, "SELECT * FROM accidente");

    resultado = PQexec(conn, query);
    if (PQresultStatus(resultado) == PGRES_TUPLES_OK) {
        int fila = PQntuples(resultado);
        int columna = PQnfields(resultado);
        for (int i = 0; i < fila; i++) {
            for (int j = 0; j < columna; j++) {
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

// Función para actualizar un accidente
void ActualiAccidente() {
    PGconn *conn = conectarBD();

    int no_accidente;
    int id_auto = 0;
    char fecha[11] = "", hora[6] = "";
    int opcion;
    int modificar_id_auto = 0, modificar_fecha = 0, modificar_hora = 0;

    printf("Ingrese el ID del Accidente: ");
    scanf("%d", &no_accidente);

    // Menú para seleccionar los campos a modificar
    do {
        printf("\nSeleccione qué campo desea modificar:\n");
        printf("1. ID Auto\n");
        printf("2. Fecha (YYYY-MM-DD)\n");
        printf("3. Hora (HH:MM)\n");
        printf("4. Finalizar selección y guardar cambios\n");
        printf("Opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Nuevo ID Auto: ");
                scanf("%d", &id_auto);
                modificar_id_auto = 1;
                break;
            case 2:
                printf("Nueva Fecha (YYYY-MM-DD): ");
                scanf("%s", fecha);
                modificar_fecha = 1;
                break;
            case 3:
                printf("Nueva Hora (HH:MM): ");
                scanf("%s", hora);
                modificar_hora = 1;
                break;
            case 4:
                printf("Finalizando selección...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
                break;
        }
    } while (opcion != 4);

    char query[1000] = "UPDATE accidente SET ";
    int first = 1; // Para controlar la inclusión de comas entre los campos

    if (modificar_id_auto) {
        sprintf(query + strlen(query), "%sid_auto = %d", first ? "" : ", ", id_auto);
        first = 0;
    }
    if (modificar_fecha) {
        sprintf(query + strlen(query), "%sfecha = '%s'", first ? "" : ", ", fecha);
        first = 0;
    }
    if (modificar_hora) {
        sprintf(query + strlen(query), "%shora = '%s'", first ? "" : ", ", hora);
        first = 0;
    }

    if (first == 1) {
        // No se seleccionó ningún campo para modificar
        printf("No se seleccionó ningún campo para modificar en la tabla 'accidente'.\n");
    } else {
        // Ejecutar la consulta solo si hay campos para modificar
        sprintf(query + strlen(query), " WHERE no_accidente = %d", no_accidente);
        PGresult *res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Error al actualizar el accidente: %s\n", PQerrorMessage(conn));
            PQclear(res);
            PQfinish(conn);
            exit(1);
        }
        PQclear(res);
        printf("Accidente actualizado correctamente.\n");
    }

    PQfinish(conn);
    printf("Actualización completada.\n");
}


void EliminarAccidente() {
    PGconn *conn = conectarBD();

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
