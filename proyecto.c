#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX 100

struct Mascota {
    char especie[30];
    char nombre[30];
    int edad;
    char tamano[20];
    float peso;
    char descripcion[100];
};

struct Adoptante {
    int id;
    char password[20];
};

struct Adopcion {
    struct Adoptante adoptante;
    struct Mascota mascota;
    char fechaHora[30];
};

struct Mascota mascotas[MAX];
int totalMascotas = 0;

struct Adoptante adoptantes[MAX];
int inicioFila = 0, finFila = 0;

struct Adopcion adopciones[MAX];
int totalAdopciones = 0;

// Crear admin.txt si no existe y registrar admin la primera vez
void crearAdminSiNoExiste() {
    FILE *file = fopen("admin.txt", "r");
    if (!file) {
        char usuario[20], contrasena[20];
        printf("\n=== REGISTRO DE ADMINISTRADOR ===\n");
        printf("Cree un usuario de administrador: ");
        scanf("%19s", usuario);
        printf("Cree una contrasenina de administrador: ");
        scanf("%19s", contrasena);
        file = fopen("admin.txt", "w");
        if (file) {
            fprintf(file, "%s:%s\n", usuario, contrasena);
            fclose(file);
            printf("Administrador registrado correctamente.\n");
        } else {
            printf("No se pudo crear el archivo de administrador.\n");
            exit(1);
        }
    } else {
        fclose(file);
    }
}

// Guardar todas las mascotas en mascotas.txt (sobrescribe)
void guardarMascotasArchivo() {
    FILE *file = fopen("mascotas.txt", "w");
    if (!file) {
        printf("No se pudo abrir el archivo mascotas.txt para guardar.\n");
        return;
    }
    for (int i = 0; i < totalMascotas; i++) {
        fprintf(file, "%s %s %d %s %.2f %s\n",
            mascotas[i].especie, mascotas[i].nombre, mascotas[i].edad,
            mascotas[i].tamano, mascotas[i].peso, mascotas[i].descripcion);
    }
    fclose(file);
}

// Guardar todos los adoptantes en adoptantes.txt (sobrescribe)
void guardarAdoptantesArchivo() {
    FILE *file = fopen("adoptantes.txt", "w");
    if (!file) {
        printf("No se pudo abrir el archivo adoptantes.txt para guardar.\n");
        return;
    }
    for (int i = 0; i < finFila; i++) {
        fprintf(file, "%d %s\n", adoptantes[i].id, adoptantes[i].password);
    }
    fclose(file);
}

// Cargar mascotas desde archivo (crea si no existe)
void cargarMascotas() {
    FILE *file = fopen("mascotas.txt", "a+");
    if (file == NULL) {
        printf("No se pudo abrir el archivo mascotas.txt\n");
        return;
    }
    rewind(file);
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (totalMascotas >= MAX) {
            printf("Advertencia: Se alcanzo el limite maximo de mascotas\n");
            break;
        }
        int result = sscanf(line, "%29s %29s %d %19s %f %99[^\n]",
                  mascotas[totalMascotas].especie,
                  mascotas[totalMascotas].nombre,
                  &mascotas[totalMascotas].edad,
                  mascotas[totalMascotas].tamano,
                  &mascotas[totalMascotas].peso,
                  mascotas[totalMascotas].descripcion);
        if (result == 6) {
            totalMascotas++;
        }
    }
    fclose(file);
}

// Cargar adoptantes desde archivo (crea si no existe)
void cargarAdoptantes() {
    FILE *file = fopen("adoptantes.txt", "a+");
    if (file == NULL) {
        printf("No se pudo abrir el archivo adoptantes.txt\n");
        return;
    }
    rewind(file);
    int id;
    char password[20];
    while (fscanf(file, "%d %19s", &id, password) == 2) {
        if (finFila >= MAX) {
            printf("Advertencia: Se alcanzo el limite maximo de adoptantes\n");
            break;
        }
        adoptantes[finFila].id = id;
        strcpy(adoptantes[finFila].password, password);
        finFila++;
    }
    fclose(file);
}

void guardarAdopciones() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char filename[50];
    sprintf(filename, "%04d%02d%02d-adopciones.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("No se pudo crear el archivo de adopciones\n");
        return;
    }
    for (int i = 0; i < totalAdopciones; i++) {
        fprintf(file, "ID: %d, Mascota: %s, Fecha: %s\n",
                adopciones[i].adoptante.id,
                adopciones[i].mascota.nombre,
                adopciones[i].fechaHora);
    }
    fclose(file);
}

void agregarAdopcion(struct Adoptante adoptante, struct Mascota mascota) {
    if (totalAdopciones >= MAX) {
        printf("No se pueden registrar mas adopciones. Limite alcanzado.\n");
        return;
    }
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(adopciones[totalAdopciones].fechaHora, "%04d-%02d-%02d %02d:%02d:%02d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    adopciones[totalAdopciones].adoptante = adoptante;
    adopciones[totalAdopciones].mascota = mascota;
    totalAdopciones++;
}

void mostrarMascota(struct Mascota m) {
    printf("\n--- Informacion de la Mascota ---\n");
    printf("Especie: %s\n", m.especie);
    printf("Nombre: %s\n", m.nombre);
    printf("Edad aproximada: %d\n", m.edad);
    printf("Tamano segun su especie: %s\n", m.tamano);
    printf("Peso: %.2f kg\n", m.peso);
    printf("Descripcion general: %s\n", m.descripcion);
    printf("---------------------------------\n");
}

// Opcion 5: Registrar mascota (adoptante)
void registrarMascotaPorAdoptante() {
    if (totalMascotas >= MAX) {
        printf("No se pueden registrar mas mascotas. Limite alcanzado.\n");
        return;
    }
    struct Mascota nueva;
    printf("\n=== REGISTRO DE NUEVA MASCOTA ===\n");
    printf("Especie: "); scanf("%29s", nueva.especie);
    printf("Nombre: "); scanf("%29s", nueva.nombre);
    printf("Edad aproximada: "); scanf("%d", &nueva.edad);
    printf("Tamano segun su especie: "); scanf("%19s", nueva.tamano);
    printf("Peso: "); scanf("%f", &nueva.peso);
    getchar();
    printf("Descripcion general: "); fgets(nueva.descripcion, 100, stdin);
    nueva.descripcion[strcspn(nueva.descripcion, "\n")] = '\0';
    mascotas[totalMascotas++] = nueva;
    guardarMascotasArchivo();
    printf("\nMascota registrada con exito!\n");
}

// Opcion 1: Mostrar todas las mascotas con navegacion S/A/M/Q
void mostrarMascotasTodas(struct Adoptante actual) {
    if (totalMascotas == 0) {
        printf("No hay mascotas disponibles para mostrar.\n");
        return;
    }
    int pos = 0;
    char accion;
    do {
        mostrarMascota(mascotas[pos]);
        printf("\nOpciones:\n");
        printf("S = Siguiente, A = Anterior, M = Adoptar, Q = Salir: ");
        scanf(" %c", &accion);
        if (accion == 'S' || accion == 's') {
            if (pos == totalMascotas - 1) {
                printf("Actualmente se muestran los datos de la ultima mascota registrada.\n");
                printf("Vuelva a escribir S para mostrar los datos de la primera mascota registrada.\n");
                scanf(" %c", &accion);
                if (accion == 'S' || accion == 's') pos = 0;
            } else {
                pos++;
            }
        } else if (accion == 'A' || accion == 'a') {
            if (pos == 0) {
                printf("Actualmente se muestran los datos de la primera mascota.\n");
                printf("Vuelva a escribir A para mostrar los datos de la ultima mascota registrada.\n");
                scanf(" %c", &accion);
                if (accion == 'A' || accion == 'a') pos = totalMascotas - 1;
            } else {
                pos--;
            }
        } else if (accion == 'M' || accion == 'm') {
            printf("\n--- Datos de la mascota a adoptar ---\n");
            mostrarMascota(mascotas[pos]);
            printf("Escriba CONFIRMAR para adoptar o CANCELAR para regresar: ");
            char decision[20];
            scanf("%s", decision);
            if (strcasecmp(decision, "CONFIRMAR") == 0) {
                agregarAdopcion(actual, mascotas[pos]);
                printf("\nFelicidades por adoptar a %s!\n", mascotas[pos].nombre);
                printf("Pase al area donde tienen a las mascotas bajo resguardo.\n");
                sleep(3);
                inicioFila++;
                return;
            } else {
                printf("Esta bien, puede seguir revisando la lista de mascotas.\n");
            }
        } else if (accion == 'Q' || accion == 'q') {
            break;
        } else {
            printf("Opcion no valida. Intente nuevamente.\n");
        }
    } while (1);
}

// Opcion 2: Mostrar mascotas por especie
void mostrarMascotasPorEspecie(struct Adoptante actual) {
    if (totalMascotas == 0) {
        printf("No hay mascotas disponibles.\n");
        return;
    }
    // Listar especies unicas
    char especies[MAX][30];
    int numEspecies = 0;
    for (int i = 0; i < totalMascotas; i++) {
        int existe = 0;
        for (int j = 0; j < numEspecies; j++) {
            if (strcmp(mascotas[i].especie, especies[j]) == 0) {
                existe = 1;
                break;
            }
        }
        if (!existe) {
            strcpy(especies[numEspecies++], mascotas[i].especie);
        }
    }
    printf("\nEspecies disponibles:\n");
    for (int i = 0; i < numEspecies; i++) {
        printf("%d. %s\n", i + 1, especies[i]);
    }
    printf("Seleccione el numero de la especie: ");
    int opcion;
    scanf("%d", &opcion);
    if (opcion < 1 || opcion > numEspecies) {
        printf("Opcion no valida.\n");
        return;
    }
    char especieSeleccionada[30];
    strcpy(especieSeleccionada, especies[opcion - 1]);
    // Filtrar mascotas de esa especie
    int indices[MAX], total = 0;
    for (int i = 0; i < totalMascotas; i++) {
        if (strcmp(mascotas[i].especie, especieSeleccionada) == 0) {
            indices[total++] = i;
        }
    }
    if (total == 0) {
        printf("No hay mascotas de esa especie.\n");
        return;
    }
    int pos = 0;
    char accion;
    do {
        mostrarMascota(mascotas[indices[pos]]);
        printf("\nOpciones:\n");
        printf("S = Siguiente, A = Anterior, M = Adoptar, Q = Salir: ");
        scanf(" %c", &accion);
        if (accion == 'S' || accion == 's') {
            if (pos == total - 1) {
                printf("Actualmente se muestran los datos de la ultima mascota registrada.\n");
                printf("Vuelva a escribir S para mostrar los datos de la primera mascota registrada.\n");
                scanf(" %c", &accion);
                if (accion == 'S' || accion == 's') pos = 0;
            } else {
                pos++;
            }
        } else if (accion == 'A' || accion == 'a') {
            if (pos == 0) {
                printf("Actualmente se muestran los datos de la primera mascota.\n");
                printf("Vuelva a escribir A para mostrar los datos de la ultima mascota registrada.\n");
                scanf(" %c", &accion);
                if (accion == 'A' || accion == 'a') pos = total - 1;
            } else {
                pos--;
            }
        } else if (accion == 'M' || accion == 'm') {
            printf("\n--- Datos de la mascota a adoptar ---\n");
            mostrarMascota(mascotas[indices[pos]]);
            printf("Escriba CONFIRMAR para adoptar o CANCELAR para regresar: ");
            char decision[20];
            scanf("%s", decision);
            if (strcasecmp(decision, "CONFIRMAR") == 0) {
                agregarAdopcion(actual, mascotas[indices[pos]]);
                printf("\nFelicidades por adoptar a %s!\n", mascotas[indices[pos]].nombre);
                printf("Pase al area donde tienen a las mascotas bajo resguardo.\n");
                sleep(3);
                inicioFila++;
                return;
            } else {
                printf("Esta bien, puede seguir revisando la lista de mascotas.\n");
            }
        } else if (accion == 'Q' || accion == 'q') {
            break;
        } else {
            printf("Opcion no valida. Intente nuevamente.\n");
        }
    } while (1);
}

// Opcion 3: Buscar mascota por nombre
void buscarMascotaPorNombre(struct Adoptante actual) {
    if (totalMascotas == 0) {
        printf("No hay mascotas disponibles.\n");
        return;
    }
    char nombreBuscado[30];
    printf("Ingrese el nombre de la mascota a buscar: ");
    scanf("%29s", nombreBuscado);
    int indices[MAX], total = 0;
    for (int i = 0; i < totalMascotas; i++) {
        if (strcmp(mascotas[i].nombre, nombreBuscado) == 0) {
            indices[total++] = i;
        }
    }
    if (total == 0) {
        printf("No existe ese nombre en la lista de mascotas.\n");
        return;
    }
    int pos = 0;
    char accion;
    do {
        mostrarMascota(mascotas[indices[pos]]);
        printf("\nOpciones:\n");
        printf("S = Siguiente, A = Anterior, M = Adoptar, Q = Salir: ");
        scanf(" %c", &accion);
        if (accion == 'S' || accion == 's') {
            if (pos == total - 1) {
                printf("Actualmente se muestran los datos de la ultima mascota registrada.\n");
                printf("Vuelva a escribir S para mostrar los datos de la primera mascota registrada.\n");
                scanf(" %c", &accion);
                if (accion == 'S' || accion == 's') pos = 0;
            } else {
                pos++;
            }
        } else if (accion == 'A' || accion == 'a') {
            if (pos == 0) {
                printf("Actualmente se muestran los datos de la primera mascota.\n");
                printf("Vuelva a escribir A para mostrar los datos de la ultima mascota registrada.\n");
                scanf(" %c", &accion);
                if (accion == 'A' || accion == 'a') pos = total - 1;
            } else {
                pos--;
            }
        } else if (accion == 'M' || accion == 'm') {
            printf("\n--- Datos de la mascota a adoptar ---\n");
            mostrarMascota(mascotas[indices[pos]]);
            printf("Escriba CONFIRMAR para adoptar o CANCELAR para regresar: ");
            char decision[20];
            scanf("%s", decision);
            if (strcasecmp(decision, "CONFIRMAR") == 0) {
                agregarAdopcion(actual, mascotas[indices[pos]]);
                printf("\nFelicidades por adoptar a %s!\n", mascotas[indices[pos]].nombre);
                printf("Pase al area donde tienen a las mascotas bajo resguardo.\n");
                sleep(3);
                inicioFila++;
                return;
            } else {
                printf("Esta bien, puede seguir revisando la lista de mascotas.\n");
            }
        } else if (accion == 'Q' || accion == 'q') {
            break;
        } else {
            printf("Opcion no valida. Intente nuevamente.\n");
        }
    } while (1);
}

// Opcion 4: Buscar mascotas por especie e intervalo de edad (recursivo)
void buscarMascotasRecursivo(int *indices, int total, int pos, struct Adoptante actual);

void buscarMascotasPorEspecieYEdad(struct Adoptante actual) {
    if (totalMascotas == 0) {
        printf("No hay mascotas disponibles.\n");
        return;
    }
    char especie[30];
    int edadMin, edadMax;
    printf("Ingrese la especie: ");
    scanf("%29s", especie);
    printf("Ingrese la edad minima: ");
    scanf("%d", &edadMin);
    printf("Ingrese la edad maxima: ");
    scanf("%d", &edadMax);
    int indices[MAX], totalFiltradas = 0;
    for (int i = 0; i < totalMascotas; i++) {
        if (strcmp(mascotas[i].especie, especie) == 0 &&
            mascotas[i].edad >= edadMin && mascotas[i].edad <= edadMax) {
            indices[totalFiltradas++] = i;
        }
    }
    if (totalFiltradas == 0) {
        printf("No hay mascotas que coincidan con el criterio de busqueda.\n");
        printf("Presione cualquier letra para regresar al menu del adoptante.\n");
        char dummy;
        scanf(" %c", &dummy);
        return;
    }
    buscarMascotasRecursivo(indices, totalFiltradas, 0, actual);
}

void buscarMascotasRecursivo(int *indices, int total, int pos, struct Adoptante actual) {
    mostrarMascota(mascotas[indices[pos]]);
    printf("\nOpciones:\n");
    printf("S = Siguiente, A = Anterior, M = Adoptar, Q = Salir: ");
    char accion;
    scanf(" %c", &accion);
    if (accion == 'S' || accion == 's') {
        if (pos == total - 1) {
            printf("Actualmente se muestran los datos de la ultima mascota registrada.\n");
            printf("Vuelva a escribir S para mostrar los datos de la primera mascota registrada.\n");
            scanf(" %c", &accion);
            if (accion == 'S' || accion == 's') buscarMascotasRecursivo(indices, total, 0, actual);
            else buscarMascotasRecursivo(indices, total, pos, actual);
        } else {
            buscarMascotasRecursivo(indices, total, pos + 1, actual);
        }
    } else if (accion == 'A' || accion == 'a') {
        if (pos == 0) {
            printf("Actualmente se muestran los datos de la primera mascota.\n");
            printf("Vuelva a escribir A para mostrar los datos de la ultima mascota registrada.\n");
            scanf(" %c", &accion);
            if (accion == 'A' || accion == 'a') buscarMascotasRecursivo(indices, total, total - 1, actual);
            else buscarMascotasRecursivo(indices, total, pos, actual);
        } else {
            buscarMascotasRecursivo(indices, total, pos - 1, actual);
        }
    } else if (accion == 'M' || accion == 'm') {
        printf("\n--- Datos de la mascota a adoptar ---\n");
        mostrarMascota(mascotas[indices[pos]]);
        printf("Escriba CONFIRMAR para adoptar o CANCELAR para regresar: ");
        char decision[20];
        scanf("%s", decision);
        if (strcasecmp(decision, "CONFIRMAR") == 0) {
            agregarAdopcion(actual, mascotas[indices[pos]]);
            printf("\nFelicidades por adoptar a %s!\n", mascotas[indices[pos]].nombre);
            printf("Pase al area donde tienen a las mascotas bajo resguardo.\n");
            sleep(3);
            inicioFila++;
            return;
        } else {
            printf("Esta bien, puede seguir revisando la lista de mascotas.\n");
            buscarMascotasRecursivo(indices, total, pos, actual);
        }
    } else if (accion == 'Q' || accion == 'q') {
        return;
    } else {
        printf("Opcion no valida. Intente nuevamente.\n");
        buscarMascotasRecursivo(indices, total, pos, actual);
    }
}

void editarMascota() {
    if (totalMascotas == 0) {
        printf("No hay mascotas para editar.\n");
        return;
    }
    int idx;
    printf("\nMascotas registradas:\n");
    for (int i = 0; i < totalMascotas; i++) {
        printf("%d. %s (%s)\n", i + 1, mascotas[i].nombre, mascotas[i].especie);
    }
    printf("Seleccione el numero de la mascota a editar: ");
    scanf("%d", &idx);
    idx--;
    if (idx < 0 || idx >= totalMascotas) {
        printf("Mascota no valida.\n");
        return;
    }
    printf("Editando mascota %s:\n", mascotas[idx].nombre);
    printf("Especie (%s): ", mascotas[idx].especie);
    scanf("%29s", mascotas[idx].especie);
    printf("Nombre (%s): ", mascotas[idx].nombre);
    scanf("%29s", mascotas[idx].nombre);
    printf("Edad (%d): ", mascotas[idx].edad);
    scanf("%d", &mascotas[idx].edad);
    printf("Tamano (%s): ", mascotas[idx].tamano);
    scanf("%19s", mascotas[idx].tamano);
    printf("Peso (%.2f): ", mascotas[idx].peso);
    scanf("%f", &mascotas[idx].peso);
    getchar();
    printf("Descripcion (%s): ", mascotas[idx].descripcion);
    fgets(mascotas[idx].descripcion, 100, stdin);
    mascotas[idx].descripcion[strcspn(mascotas[idx].descripcion, "\n")] = '\0';
    printf("Mascota editada correctamente.\n");
}

void editarAdoptante() {
    if (finFila == 0) {
        printf("No hay adoptantes para editar.\n");
        return;
    }
    int idx;
    printf("\nAdoptantes registrados:\n");
    for (int i = 0; i < finFila; i++) {
        printf("%d. ID: %d\n", i + 1, adoptantes[i].id);
    }
    printf("Seleccione el numero de adoptante a editar: ");
    scanf("%d", &idx);
    idx--;
    if (idx < 0 || idx >= finFila) {
        printf("Adoptante no valido.\n");
        return;
    }
    printf("Editando adoptante ID %d:\n", adoptantes[idx].id);
    printf("Nueva contrasenina: ");
    scanf("%19s", adoptantes[idx].password);
    printf("Adoptante editado correctamente.\n");
}

void menuAdoptante(struct Adoptante actual) {
    int opcion;
    do {
        printf("\n=== MENU DEL ADOPTANTE ===\n");
        printf("1. Mostrar mascotas de todas las especies\n");
        printf("2. Mostrar mascotas por especie\n");
        printf("3. Buscar una mascota por nombre\n");
        printf("4. Buscar mascotas por especie y rango de edad\n");
        printf("5. Registrar mascota\n");
        printf("6. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        if (opcion == 1) {
            mostrarMascotasTodas(actual);
        } else if (opcion == 2) {
            mostrarMascotasPorEspecie(actual);
        } else if (opcion == 3) {
            buscarMascotaPorNombre(actual);
        } else if (opcion == 4) {
            buscarMascotasPorEspecieYEdad(actual);
        } else if (opcion == 5) {
            registrarMascotaPorAdoptante();
        } else if (opcion == 6) {
            break;
        } else {
            printf("Opcion no valida. Intente nuevamente.\n");
        }
    } while (opcion != 6);
}

void atenderAdoptante() {
    if (inicioFila == finFila) {
        printf("\nNo hay adoptantes en la fila.\n");
        return;
    }
    struct Adoptante actual = adoptantes[inicioFila];
    char input[20];
    int intentos = 0;
    printf("\n=== ATENCION AL ADOPTANTE ===\n");
    printf("ID de adoptante: %d\n", actual.id);
    while (intentos < 3) {
        printf("Ingrese su contrasenina: ");
        scanf("%s", input);
        if (strcmp(input, actual.password) == 0) {
            menuAdoptante(actual);
            return;
        } else {
            printf("Contrasenina incorrecta. Intentos restantes: %d\n", 2 - intentos);
            intentos++;
        }
    }
    printf("\nLimite de intentos alcanzado. Registrate de nuevo.\n");
    inicioFila++;
}

// Registrar un nuevo adoptante
void registrarAdoptante() {
    if (finFila >= MAX) {
        printf("No se pueden registrar mas adoptantes. Limite alcanzado.\n");
        return;
    }
    int id = (finFila == 0) ? 1 : adoptantes[finFila-1].id + 1;
    char password[20];
    printf("\n=== REGISTRO DE ADOPTANTE ===\n");
    printf("Asigne una contrasenina: ");
    scanf("%19s", password);
    adoptantes[finFila].id = id;
    strcpy(adoptantes[finFila].password, password);
    printf("Registro exitoso. Su ID es: %d y su contrasenina es: %s\n", id, password);
    finFila++;
}

void menuPrincipal() {
    int opcion;
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Registro de mascota (admin)\n");
        printf("2. Atender a un adoptante\n");
        printf("3. Registrar adoptante\n");
        printf("4. Editar mascota\n");
        printf("5. Editar adoptante\n");
        printf("6. Cerrar sistema\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        if (opcion == 1) {
            char nickname[20], contrasena[20];
            FILE *file = fopen("admin.txt", "r");
            if (!file) {
                printf("Error al abrir el archivo de administrador.\n");
                continue;
            }
            char nick[20], pass[20];
            fscanf(file, "%[^:]:%s", nick, pass);
            fclose(file);
            printf("\n=== ACCESO ADMINISTRADOR ===\n");
            printf("Nickname: ");
            scanf("%s", nickname);
            printf("Contrasenina: ");
            scanf("%s", contrasena);
            if (strcmp(nick, nickname) == 0 && strcmp(pass, contrasena) == 0) {
                if (totalMascotas >= MAX) {
                    printf("No se pueden registrar mas mascotas. Limite alcanzado.\n");
                    continue;
                }
                struct Mascota nueva;
                printf("\n=== REGISTRO DE NUEVA MASCOTA ===\n");
                printf("Especie: "); scanf("%29s", nueva.especie);
                printf("Nombre: "); scanf("%29s", nueva.nombre);
                printf("Edad: "); scanf("%d", &nueva.edad);
                printf("Tamano: "); scanf("%19s", nueva.tamano);
                printf("Peso: "); scanf("%f", &nueva.peso);
                getchar();
                printf("Descripcion: "); fgets(nueva.descripcion, 100, stdin);
                nueva.descripcion[strcspn(nueva.descripcion, "\n")] = '\0';
                mascotas[totalMascotas++] = nueva;
                guardarMascotasArchivo();
                printf("\nMascota registrada con exito!\n");
            } else {
                printf("\nAcceso denegado. Credenciales incorrectas.\n");
            }
        } else if (opcion == 2) {
            atenderAdoptante();
        } else if (opcion == 3) {
            registrarAdoptante();
        } else if (opcion == 4) {
            editarMascota();
        } else if (opcion == 5) {
            editarAdoptante();
        } else if (opcion == 6) {
            guardarAdoptantesArchivo();
            guardarMascotasArchivo();
            guardarAdopciones();
            printf("\nSistema cerrado. Datos guardados.\n");
            break;
        } else {
            printf("\nOpcion no valida. Intente nuevamente.\n");
        }
    } while (1);
}

int main() {
    printf("=== SISTEMA DE ADOPCION DE MASCOTAS ===\n");
    crearAdminSiNoExiste();
    cargarAdoptantes();
    cargarMascotas();
    menuPrincipal();
    return 0;
}