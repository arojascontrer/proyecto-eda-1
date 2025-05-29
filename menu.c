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

void cargarMascotas() {
    FILE *file = fopen("mascotas.txt", "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo mascotas.txt\n");
        return;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (totalMascotas >= MAX) {
            printf("Advertencia: Se alcanzó el límite máximo de mascotas\n");
            break;
        }
        
        // Formato esperado: especie nombre edad tamaño peso descripción
        int result = sscanf(line, "%29s %29s %d %19s %f %99[^\n]",
                  mascotas[totalMascotas].especie,
                  mascotas[totalMascotas].nombre,
                  &mascotas[totalMascotas].edad,
                  mascotas[totalMascotas].tamano,
                  &mascotas[totalMascotas].peso,
                  mascotas[totalMascotas].descripcion);
        
        if (result == 6) {
            totalMascotas++;
        } else {
            printf("Error al leer la línea: %s", line);
        }
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
        printf("No se pueden registrar más adopciones. Límite alcanzado.\n");
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
    printf("\n--- Información de la Mascota ---\n");
    printf("Especie: %s\n", m.especie);
    printf("Nombre: %s\n", m.nombre);
    printf("Edad: %d años\n", m.edad);
    printf("Tamaño: %s\n", m.tamano);
    printf("Peso: %.2f kg\n", m.peso);
    printf("Descripción: %s\n", m.descripcion);
    printf("---------------------------------\n");
}

void menuAdoptante(struct Adoptante actual) {
    int opcion, pos = 0;
    do {
        printf("\n=== MENÚ DEL ADOPTANTE ===\n");
        printf("1. Mostrar mascotas de todas las especies\n");
        printf("2. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        
        if (opcion == 1) {
            if (totalMascotas == 0) {
                printf("No hay mascotas disponibles para mostrar.\n");
                continue;
            }
            
            char accion;
            do {
                mostrarMascota(mascotas[pos]);
                printf("\nOpciones:\n");
                printf("S = Siguiente, A = Anterior, M = Adoptar, Q = Salir: ");
                scanf(" %c", &accion);
                
                if (accion == 'S' || accion == 's') {
                    pos = (pos + 1) % totalMascotas;
                } else if (accion == 'A' || accion == 'a') {
                    pos = (pos - 1 + totalMascotas) % totalMascotas;
                } else if (accion == 'M' || accion == 'm') {
                    printf("\n¿Desea CONFIRMAR o CANCELAR la adopción?: ");
                    char decision[10];
                    scanf("%s", decision);
                    if (strcasecmp(decision, "CONFIRMAR") == 0) {
                        agregarAdopcion(actual, mascotas[pos]);
                        printf("\n¡Felicidades por adoptar a %s!\n", mascotas[pos].nombre);
                        sleep(3);
                        inicioFila++;
                        return;
                    } else {
                        printf("\nPuede seguir revisando mascotas.\n");
                    }
                }
            } while (accion != 'Q' && accion != 'q');
        } else if (opcion == 2) {
            char salida[10];
            printf("\n¿Desea SALIR o REGRESAR?: ");
            scanf("%s", salida);
            if (strcasecmp(salida, "SALIR") == 0) {
                inicioFila++;
                return;
            }
        }
    } while (1);
}

void atenderAdoptante() {
    if (inicioFila == finFila) {
        printf("\nNo hay adoptantes en la fila.\n");
        return;
    }
    
    struct Adoptante actual = adoptantes[inicioFila];
    char input[20];
    int intentos = 0;
    
    printf("\n=== ATENCIÓN AL ADOPTANTE ===\n");
    printf("ID de adoptante: %d\n", actual.id);
    
    while (intentos < 3) {
        printf("Ingrese su contraseña: ");
        scanf("%s", input);
        
        if (strcmp(input, actual.password) == 0) {
            menuAdoptante(actual);
            return;
        } else {
            printf("Contraseña incorrecta. Intentos restantes: %d\n", 2 - intentos);
            intentos++;
        }
    }
    
    printf("\nLímite de intentos alcanzado. Regístrese de nuevo.\n");
    inicioFila++;
}

void menuPrincipal() {
    int opcion;
    do {
        printf("\n=== MENÚ PRINCIPAL ===\n");
        printf("1. Registro de mascota (admin)\n");
        printf("2. Atender a un adoptante\n");
        printf("3. Cerrar sistema\n");
        printf("Seleccione una opción: ");
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
            printf("Contraseña: ");
            scanf("%s", contrasena);
            
            if (strcmp(nick, nickname) == 0 && strcmp(pass, contrasena) == 0) {
                if (totalMascotas >= MAX) {
                    printf("No se pueden registrar más mascotas. Límite alcanzado.\n");
                    continue;
                }
                
                struct Mascota nueva;
                printf("\n=== REGISTRO DE NUEVA MASCOTA ===\n");
                printf("Especie: "); scanf("%29s", nueva.especie);
                printf("Nombre: "); scanf("%29s", nueva.nombre);
                printf("Edad: "); scanf("%d", &nueva.edad);
                printf("Tamaño: "); scanf("%19s", nueva.tamano);
                printf("Peso: "); scanf("%f", &nueva.peso);
                getchar();
                printf("Descripción: "); fgets(nueva.descripcion, 100, stdin);
                nueva.descripcion[strcspn(nueva.descripcion, "\n")] = '\0';
                
                mascotas[totalMascotas++] = nueva;
                printf("\nMascota registrada con éxito!\n");
            } else {
                printf("\nAcceso denegado. Credenciales incorrectas.\n");
            }
        } else if (opcion == 2) {
            atenderAdoptante();
        } else if (opcion == 3) {
            guardarAdopciones();
            printf("\nSistema cerrado. Adopciones guardadas.\n");
            break;
        } else {
            printf("\nOpción no válida. Intente nuevamente.\n");
        }
    } while (1);
}

int main() {
    printf("=== SISTEMA DE ADOPCIÓN DE MASCOTAS ===\n");
    
    // Cargar datos iniciales
    cargarMascotas();
    
    // Datos de prueba
    adoptantes[0].id = 1;
    strcpy(adoptantes[0].password, "1234");
    finFila++;
    
    menuPrincipal();
    
    return 0;
}