#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Estructuras
struct Nodo {
    char nombre[50];
    char apellidoP[30];
    char apellidoM[30];
    char direccion[100];
    char correo[50];
    char telefono[20];
    char contrasenina[20];
    char id[20];
    struct Nodo* aptNodoSig;
    struct Nodo* aptNodoPrev;
};

struct Cola {
    struct Nodo* aptFrente;
    struct Nodo* aptAtras;
};

struct NodoM {
    char especie[50];
    char nombre[20];
    int edad_aproximada;
    char tamanio_segun_su_especie[20];
    float peso;
    char descripcion_general[100];
    struct NodoM* aptNodoSig;
    struct NodoM* aptNodoPrev;
};

struct ListaDoblementeLigadaCircular {
    struct NodoM* aptFrente;
    struct NodoM* aptAtras;
};

// Prototipos
void inicializarCola(struct Cola* laCola);
void encolar(struct Cola* laCola);
int desencolar(struct Cola* laCola);
int estaVaciaCola(struct Cola* laCola);

void menuMascotas();
void inicializarLista(struct ListaDoblementeLigadaCircular* laLista);
int estaVaciaLista(struct ListaDoblementeLigadaCircular* laLista);
void insertarMascota(struct ListaDoblementeLigadaCircular* laLista);
void mostrarLista(struct ListaDoblementeLigadaCircular* laLista);

void convertirNumeroADosDigitos(int numero, char texto[3]);
void generarID(char id[], char nombre[], char apellidoP[], char apellidoM[], int contador);
void validarContrasenina(struct Cola* laCola);

// Lista global de mascotas para que no se pierdan los datos entre menus
struct ListaDoblementeLigadaCircular listaMascotas;

int main() {
    struct Cola laCola;
    inicializarCola(&laCola);
    inicializarLista(&listaMascotas);

    char opcion;
    printf("Bienvenido\n");
    do {
        printf("\nA. Registro de adoptante.\nB. Atender a un adoptante.\nC. Cerrar el sistema\nD. Registro de mascotas\n\n\tOpcion: ");
        scanf(" %c", &opcion);
        getchar();
        switch (opcion) {
            case 'A':
                encolar(&laCola);
                break;
            case 'B':
                validarContrasenina(&laCola);
                break;
            case 'C':
                printf("Cerrando sistema...\n");
                break;
            case 'D':
                insertarMascota(&listaMascotas);
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 'C');
    return 0;
}

// Cola de adoptantes
void inicializarCola(struct Cola* laCola) {
    laCola->aptFrente = NULL;
    laCola->aptAtras = NULL;
}

int estaVaciaCola(struct Cola* laCola) {
    return (laCola->aptFrente == NULL && laCola->aptAtras == NULL);
}

void encolar(struct Cola* laCola) {
    static int contador = 1;
    char confirmacion[8];
    struct Nodo* aptNuevoNodo = NULL;

    while (1) {
        if (aptNuevoNodo != NULL) {
            free(aptNuevoNodo);
        }
        aptNuevoNodo = (struct Nodo*)calloc(1, sizeof(struct Nodo));
        if (aptNuevoNodo == NULL) {
            printf("\nNo se pudo reservar memoria.");
            return;
        }

        printf("\n=== REGISTRO DE ADOPTANTE ===\n");

        printf("Primer apellido: ");
        fgets(aptNuevoNodo->apellidoP, sizeof(aptNuevoNodo->apellidoP), stdin);
        aptNuevoNodo->apellidoP[strcspn(aptNuevoNodo->apellidoP, "\n")] = '\0';

        printf("Segundo apellido: ");
        fgets(aptNuevoNodo->apellidoM, sizeof(aptNuevoNodo->apellidoM), stdin);
        aptNuevoNodo->apellidoM[strcspn(aptNuevoNodo->apellidoM, "\n")] = '\0';

        printf("Nombre: ");
        fgets(aptNuevoNodo->nombre, sizeof(aptNuevoNodo->nombre), stdin);
        aptNuevoNodo->nombre[strcspn(aptNuevoNodo->nombre, "\n")] = '\0';

        printf("Direccion: ");
        fgets(aptNuevoNodo->direccion, sizeof(aptNuevoNodo->direccion), stdin);
        aptNuevoNodo->direccion[strcspn(aptNuevoNodo->direccion, "\n")] = '\0';

        printf("Correo: ");
        fgets(aptNuevoNodo->correo, sizeof(aptNuevoNodo->correo), stdin);
        aptNuevoNodo->correo[strcspn(aptNuevoNodo->correo, "\n")] = '\0';

        printf("Telefono: ");
        fgets(aptNuevoNodo->telefono, sizeof(aptNuevoNodo->telefono), stdin);
        aptNuevoNodo->telefono[strcspn(aptNuevoNodo->telefono, "\n")] = '\0';

        printf("Contrasenina: ");
        fgets(aptNuevoNodo->contrasenina, sizeof(aptNuevoNodo->contrasenina), stdin);
        aptNuevoNodo->contrasenina[strcspn(aptNuevoNodo->contrasenina, "\n")] = '\0';

        // Mostrar resumen de datos ingresados
        printf("\n--- Resumen de datos ingresados ---\n");
        printf("Nombre completo: %s %s %s\n", aptNuevoNodo->nombre, aptNuevoNodo->apellidoP, aptNuevoNodo->apellidoM);
        printf("Direccion: %s\n", aptNuevoNodo->direccion);
        printf("Correo: %s\n", aptNuevoNodo->correo);
        printf("Telefono: %s\n", aptNuevoNodo->telefono);
        printf("Contrasenina: %s\n", aptNuevoNodo->contrasenina);

        printf("\n¿Son correctos estos datos? (SI/NO): ");
        fgets(confirmacion, sizeof(confirmacion), stdin);
        confirmacion[strcspn(confirmacion, "\n")] = '\0';

        // Convertir a mayusculas para comparar
        for (int i = 0; confirmacion[i]; i++) {
            if (confirmacion[i] >= 'a' && confirmacion[i] <= 'z') {
                confirmacion[i] = confirmacion[i] - ('a' - 'A');
            }
        }

        if (strcmp(confirmacion, "SI") == 0) {
            break;
        } else if (strcmp(confirmacion, "NO") == 0) {
            printf("\nRegistro cancelado. Puedes volver a intentarlo.\n");
            continue;
        } else {
            printf("\nRespuesta no valida. Por favor escribe SI o NO.\n");
            continue;
        }
    }

    generarID(aptNuevoNodo->id, aptNuevoNodo->nombre, aptNuevoNodo->apellidoP, aptNuevoNodo->apellidoM, contador);
    contador++;

    printf("\n¡Registro exitoso!\nTu ID es: %s\nTu contrasenina es: %s\n", aptNuevoNodo->id, aptNuevoNodo->contrasenina);

    aptNuevoNodo->aptNodoSig = NULL;
    aptNuevoNodo->aptNodoPrev = NULL;

    if (estaVaciaCola(laCola)) {
        laCola->aptFrente = aptNuevoNodo;
        laCola->aptAtras = aptNuevoNodo;
    } else {
        laCola->aptAtras->aptNodoSig = aptNuevoNodo;
        aptNuevoNodo->aptNodoPrev = laCola->aptAtras;
        laCola->aptAtras = aptNuevoNodo;
    }

    printf("\nDatos agregados correctamente. Espera a ser atendido.\n");
}

int desencolar(struct Cola* laCola) {
    if (estaVaciaCola(laCola)) {
        printf("\nLa cola esta vacia.\n");
        return 0;
    }
    struct Nodo* temp = laCola->aptFrente;
    if (laCola->aptFrente == laCola->aptAtras) {
        laCola->aptFrente = NULL;
        laCola->aptAtras = NULL;
    } else {
        laCola->aptFrente = laCola->aptFrente->aptNodoSig;
        laCola->aptFrente->aptNodoPrev = NULL;
    }
    free(temp);
    return 1;
}

// Lista de mascotas
void inicializarLista(struct ListaDoblementeLigadaCircular* laLista) {
    laLista->aptFrente = NULL;
    laLista->aptAtras = NULL;
}

int estaVaciaLista(struct ListaDoblementeLigadaCircular* laLista) {
    return (laLista->aptFrente == NULL && laLista->aptAtras == NULL);
}

void insertarMascota(struct ListaDoblementeLigadaCircular* laLista) {
    struct NodoM* aptNuevoNodoM = (struct NodoM*)calloc(1, sizeof(struct NodoM));
    if (aptNuevoNodoM == NULL) {
        printf("\nNo se pudo reservar memoria.");
        return;
    }
    printf("Especie:\n");
    fgets(aptNuevoNodoM->especie, sizeof(aptNuevoNodoM->especie), stdin);
    aptNuevoNodoM->especie[strcspn(aptNuevoNodoM->especie, "\n")] = '\0';

    printf("Nombre:\n");
    fgets(aptNuevoNodoM->nombre, sizeof(aptNuevoNodoM->nombre), stdin);
    aptNuevoNodoM->nombre[strcspn(aptNuevoNodoM->nombre, "\n")] = '\0';

    printf("Edad aproximada:\n");
    scanf("%d", &aptNuevoNodoM->edad_aproximada);
    getchar();

    printf("Tamanio segun su especie:\n");
    fgets(aptNuevoNodoM->tamanio_segun_su_especie, sizeof(aptNuevoNodoM->tamanio_segun_su_especie), stdin);
    aptNuevoNodoM->tamanio_segun_su_especie[strcspn(aptNuevoNodoM->tamanio_segun_su_especie, "\n")] = '\0';

    printf("Peso:\n");
    scanf("%f", &aptNuevoNodoM->peso);
    getchar();

    printf("Descripcion general:\n");
    fgets(aptNuevoNodoM->descripcion_general, sizeof(aptNuevoNodoM->descripcion_general), stdin);
    aptNuevoNodoM->descripcion_general[strcspn(aptNuevoNodoM->descripcion_general, "\n")] = '\0';

    // Insercion circular doblemente ligada
    if (estaVaciaLista(laLista)) {
        laLista->aptFrente = aptNuevoNodoM;
        laLista->aptAtras = aptNuevoNodoM;
        aptNuevoNodoM->aptNodoSig = aptNuevoNodoM;
        aptNuevoNodoM->aptNodoPrev = aptNuevoNodoM;
    } else {
        aptNuevoNodoM->aptNodoPrev = laLista->aptAtras;
        aptNuevoNodoM->aptNodoSig = laLista->aptFrente;
        laLista->aptAtras->aptNodoSig = aptNuevoNodoM;
        laLista->aptFrente->aptNodoPrev = aptNuevoNodoM;
        laLista->aptAtras = aptNuevoNodoM;
    }
    printf("\nMascota registrada correctamente.\n");
}

void mostrarLista(struct ListaDoblementeLigadaCircular* laLista) {
    if (estaVaciaLista(laLista)) {
        printf("\n\tLa lista esta vacia. No se pueden mostrar las mascotas.\n");
        return;
    }
    struct NodoM* aptNodoActualM = laLista->aptAtras;
    struct NodoM* aptInicio = aptNodoActualM;
    int num = 1;
    printf("\nContenido de la lista de mascotas:\n");
    do {
        printf("%i.\nEspecie: %s\nNombre: %s\nEdad aprox: %i\nTamanio: %s\nPeso: %.2f\nDescripcion: %s\n",
               num,
               aptNodoActualM->especie,
               aptNodoActualM->nombre,
               aptNodoActualM->edad_aproximada,
               aptNodoActualM->tamanio_segun_su_especie,
               aptNodoActualM->peso,
               aptNodoActualM->descripcion_general);
        aptNodoActualM = aptNodoActualM->aptNodoPrev;
        num++;
    } while (aptNodoActualM != aptInicio);
}

// Menu de mascotas
void menuMascotas() {
    char opcion;
    do {
        printf("Selecciona alguna de estas opciones:\n1. Mostrar mascotas de todas las especies\n2. Mostrar mascotas por especie\n3. Buscar una mascota en especifico por nombre\n4. Buscar mascotas indicando especie y un intervalo de edad\n5. Salir\n");
        scanf(" %c", &opcion);
        getchar();
        switch (opcion) {
            case '1':
                mostrarLista(&listaMascotas);
                break;
            case '2':
                // Implementar si lo necesitas
                break;
            case '3':
                // Implementar si lo necesitas
                break;
            case '4':
                mostrarLista(&listaMascotas);
                break;
        }
    } while (opcion != '5');
}

// Utilidades
void convertirNumeroADosDigitos(int numero, char texto[3]) {
    texto[0] = (numero / 10) + '0';
    texto[1] = (numero % 10) + '0';
    texto[2] = '\0';
}

void generarID(char id[], char nombre[], char apellidoP[], char apellidoM[], int contador) {
    char fecha[7];//ddmmaa\0
    char numero[3];
    char letras[4];

    time_t t = time(NULL);
    struct tm hoy = *localtime(&t);

    char dia[3], mes[3], anio[3];
    convertirNumeroADosDigitos(hoy.tm_mday, dia);
    convertirNumeroADosDigitos(hoy.tm_mon + 1, mes);
    convertirNumeroADosDigitos(hoy.tm_year % 100, anio);

    strcpy(fecha, "");
    strcat(fecha, dia);
    strcat(fecha, mes);
    strcat(fecha, anio);

    convertirNumeroADosDigitos(contador, numero);

    letras[0] = (nombre[0] != '\0') ? nombre[0] : 'X';
    int lenP = strlen(apellidoP);
    letras[1] = (lenP >= 2) ? apellidoP[lenP - 2] : 'X';
    letras[2] = (strlen(apellidoM) >= 3) ? apellidoM[2] : 'X';
    letras[3] = '\0';

    strcpy(id, "");
    strcat(id, fecha);
    strcat(id, numero);
    strcat(id, letras);
}

// Validacion de contrasenina
void validarContrasenina(struct Cola* laCola) {
    int len;
    if (estaVaciaCola(laCola)) {
        printf("\nNo hay adoptantes en espera.\n");
        return;
    }

    struct Nodo* aptAdoptante = laCola->aptFrente;
    char contraseninaIngresada[20];
    int intentos = 3;

    printf("\nAdoptante al frente: ID = %s\n", aptAdoptante->id);

    while (intentos > 0) {
        printf("Introduce tu contrasenina: ");
        fgets(contraseninaIngresada, sizeof(contraseninaIngresada), stdin);

        // Eliminar salto de linea si existe
        len = strlen(contraseninaIngresada);
        if (len > 0 && contraseninaIngresada[len - 1] == '\n') {
            contraseninaIngresada[len - 1] = '\0';
        }

        if (strcmp(contraseninaIngresada, aptAdoptante->contrasenina) == 0) {
            printf("\nContrasenina correcta. Accediendo al menu de adopcion...\n");
            menuMascotas();
            desencolar(laCola);
            return;
        } else {
            intentos--;
            if (intentos > 0) {
                printf("Contrasenina incorrecta. Te quedan %d intento(s).\n", intentos);
            }
        }
    }

    printf("\nHas fallado los 3 intentos. Pierdes tu lugar en la fila.\n");
    desencolar(laCola);
}
