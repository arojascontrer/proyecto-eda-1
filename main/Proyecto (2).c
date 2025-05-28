#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct Nodo{
	char nombre[50];
    char apellidoP[30];
    char apellidoM[30];
    char direccion[100];
    char correo[50];
    char telefono[20];
    char contrasenia[20];
    char id[20];
	struct Nodo* aptNodoSig;
	struct Nodo* aptNodoPrev;

};

struct Cola{
	struct Nodo* aptFrente;
	struct Nodo* aptAtras;
	int tamanioMaximo;
	int tamanioActual;		
};

struct NodoM{
	char especie[50]; 
	char nombre[20]; 
	int edad_aproximada;
	char tamanio_segun_su_especie [20];
	float peso;
	char descripcion_general[100];
	struct NodoM* aptNodoSig;
	struct NodoM* aptNodoPrev;
};

struct ListaDoblementeLigadaCircular{
	struct Nodo* aptFrente;
	struct Nodo* aptAtras;	
};

void inicializar(struct Cola* laCola);
void encolar(struct Cola* laCola); 
int desencolar(struct Cola* laCola);
int estaVacia(struct Cola* laCola);
void vaciar(struct Cola* laCola ); 
void mostrarCola(struct Cola* laCola );

void menuMascotas();

void inicializar(struct ListaDoblementeLigadaCircular*  laListaDoblementeLigadaCircular);
int estaVacia(struct ListaDoblementeLigadaCircular*  laListaDoblementeLigadaCircular);
void insertar(struct ListaDoblementeLigadaCircular* laListaDoblementeLigadaCircular);
struct NodoM* buscar(struct ListaDoblementeLigadaCircular* laListaDoblementeLigadaCircular);
void eliminar(struct ListaDoblementeLigadaCircular* laListaDoblementeLigadaCircular);
void mostrarLista(struct ListaDoblementeLigadaCircular* laListaDoblementeLigadaCircular);
void seleccionar(struct ListaDoblementeLigadaCircular* laListaDoblementeLigadaCircular);

void convertirNumeroADosDigitos(int numero, char texto[3]);
void generarID(char id[], char nombre[], char apellidoP[], char apellidoM[], int contador);
void validarContraseña(struct Cola* laCola);


int main(){
	struct Cola laCola;
	inicializar(&laCola);
	char opcion;
	printf("Bienvenido\n");
	do{
		printf("A. Registro de adoptante.\nB. Atender a un adoptante.\nC. Cerrar el sistema\nD. Registro de mascotas\n\n\n\tID:");
		scanf("%c",&opcion);
		getchar();
		switch(opcion){
			case 'A':encolar(&laCola);
					break;
			case 'B':
					//Primero se pide la contraseña y solo si es correcta se accede a menuMascota()
					validarContraseña(&laCola);
					break;
			case 'C':
					break;
			case 'D':
					break;
		}
	} while(opcion!='C');	
	return 1;
}

void inicializar(struct Cola* laCola)
{
	laCola->aptFrente = NULL;
	laCola->aptAtras = NULL;
}


int estaVacia(struct Cola*  laCola){
	int bandera=0;
	if ( laCola->aptFrente == NULL && laCola->aptAtras == NULL)
	{
		bandera=1;
	}
	else{
		bandera=0;
	}
	return bandera;
}
//En teoria tambien se tendria en cuenta agregar los usuarios desde algun archivo externo, pero no le se a eso
void encolar(struct Cola* laCola)
{
    static int contador = 1; // se usa static para que no se reinicie el contador cada que se mande a llamar a encolar()
    char confirmacion[3];
    struct Nodo* aptNuevoNodo = NULL;

    do {
        if (aptNuevoNodo != NULL) {
            free(aptNuevoNodo);
        }

        aptNuevoNodo = (struct Nodo*)calloc(1, sizeof(struct Nodo));
        if (aptNuevoNodo == NULL) {
            printf("\nNo se pudo reservar memoria.");
            return;
        }

        getchar();
        printf("Primer apellido:\n");
        fgets(aptNuevoNodo->apellidoP, 50, stdin);
        printf("Segundo apellido:\n");
        fgets(aptNuevoNodo->apellidoM, 50, stdin);
        printf("Nombre:\n");
        fgets(aptNuevoNodo->nombre, 50, stdin);
        printf("Direccion:\n");
        fgets(aptNuevoNodo->direccion, 100, stdin);
        printf("Correo:\n");
        fgets(aptNuevoNodo->correo, 100, stdin);
        printf("Telefono:\n");
        fgets(aptNuevoNodo->telefono, 100, stdin);
        printf("Contrasenia:\n");
        fgets(aptNuevoNodo->contrasenia, 100, stdin);

        printf("\n¿Son correctos estos datos? (SI/NO): ");
        scanf("%s", confirmacion);

    } while (strcmp(confirmacion, "SI") != 0);

    //Generar y asignar ID
    generarID(aptNuevoNodo->id, aptNuevoNodo->nombre, aptNuevoNodo->apellidoP, aptNuevoNodo->apellidoM, contador);
    contador++; // siguiente adoptante

    printf("\nTu ID es: %s", aptNuevoNodo->id);
    printf("\nTu contraseña es: %s", aptNuevoNodo->contrasenia);

    aptNuevoNodo->aptNodoSig = NULL;
    aptNuevoNodo->aptNodoPrev = NULL;

    if (estaVacia(laCola) == 1) {
        laCola->aptFrente = aptNuevoNodo;
        laCola->aptAtras = aptNuevoNodo;
    } else {
        laCola->aptAtras->aptNodoSig = aptNuevoNodo;
        aptNuevoNodo->aptNodoPrev = laCola->aptAtras;
        laCola->aptAtras = aptNuevoNodo;
    }

    printf("\nDatos agregados correctamente.\n");
}

void menuMascotas(){
	struct ListaDoblementeLigadaCircular laListaDoblementeLigadaCircular;
	inicializar(&laListaDoblementeLigadaCircular);
	char opcion;
	do{
		printf("Selecciona alguna de estas opciones:\n1.Mostrar mascotas de todas las especies\n2. Mostrar mascotas por especie \n3. Buscar una mascota en específico por nombre\n4. Buscar mascotas indicando especie y un intervalo de edad\n5. Salir\n");
		scanf("%c",&opcion);
		getchar();
		switch(opcion){
			case '1':mostrarLista(&laListaDoblementeLigadaCircular);
					break;
			case '2':
				
					break;
			case '3':
					break;
			case '4':mostrarLista(&laListaDoblementeLigadaCircular);
					break;			
		}
	} while(opcion!='5');	
}

void inicializar(struct ListaDoblementeLigadaCircular*  laListaDoblementeLigadaCircular)
{
	laListaDoblementeLigadaCircular->aptFrente=NULL;
	laListaDoblementeLigadaCircular->aptAtras=NULL;
}
void insertar(struct ListaDoblementeLigadaCircular* laListaDoblementeLigadaCircular){
	struct NodoM* aptNuevoNodoM = (struct NodoM*) calloc(1, sizeof(struct NodoM));
	if(aptNuevoNodoM!=NULL){
		//No se como ingresar los datos desde un archivo
	}
	else{

		printf("\nNo se pudo reservar memoria.");
	}
}
void mostrarLista(struct ListaDoblementeLigadaCircular* laListaDoblementeLigadaCircular){
    if (estaVacia(laListaDoblementeLigadaCircular) == 1) {
        printf("\n\tLa lista esta vacia. No se pueden mostrar las mascotas.\n");
        return;
    }
	
    struct Nodo* aptNodoActualM = laListaDoblementeLigadaCircular->aptAtras;
    struct Nodo* aptInicio = aptNodoActualM;
	int num=1;
    printf("\nContenido de la lista de mascotas:\n");

    do {
        printf("%i.\nEspecie:%sNombre:%sEdad aprox:%iTamanio:%s.\nPeso:%f.\nDescripcion:%s.\n",num,aptNodoActualM,aptNodoActual->tipo,aptNodoActual->poderEspecial,aptNodoActual->nivel,aptNodoActual->resistencia,aptNodoActual->poder);
        aptNodoActualM = aptNodoActualM->aptNodoPrev;
		num=num+1;
    } while (aptNodoActualM != aptInicio);
}


void convertirNumeroADosDigitos(int numero, char texto[3]) {
    texto[0] = (numero / 10) + '0';
    texto[1] = (numero % 10) + '0';
    texto[2] = '\0';
}

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

    if (nombre[0] != '\0') {
        letras[0] = nombre[0];
    } else {
        letras[0] = 'X';
    }

    int lenP = strlen(apellidoP);
    if (lenP >= 2) {
        letras[1] = apellidoP[lenP - 2];
    } else {
        letras[1] = 'X';
    }

    if (strlen(apellidoM) >= 3) {
        letras[2] = apellidoM[2];
    } else {
        letras[2] = 'X';
    }

    letras[3] = '\0';

    strcpy(id, "");
    strcat(id, fecha);
    strcat(id, numero);
    strcat(id, letras);
}

void validarContraseña(struct Cola* laCola) {
    
    int len;
    if (estaVacia(laCola)) {
        printf("\nNo hay adoptantes en espera.\n");
        return;
    }

    struct Nodo* aptAdoptante = laCola->aptFrente;
    char contrasenaIngresada[20];
    int intentos = 3;

    printf("\nAdoptante al frente: ID = %s\n", aptAdoptante->id);

    while (intentos > 0) {
        printf("Introduce tu contraseña: ");
        fgets(contrasenaIngresada, sizeof(contrasenaIngresada), stdin);

        // Eliminar salto de línea si existe
        len = strlen(contrasenaIngresada);
        if (len > 0 && contrasenaIngresada[len - 1] == '\n') {
            contrasenaIngresada[len - 1] = '\0';
        }

  
        char contrasenaReal[20];
        strcpy(contrasenaReal, aptAdoptante->contrasenia);
        len = strlen(contrasenaReal);
        if (len > 0 && contrasenaReal[len - 1] == '\n') {
            contrasenaReal[len - 1] = '\0';
        }

        if (strcmp(contrasenaIngresada, contrasenaReal) == 0) {
            printf("\nContraseña correcta. Accediendo al menú de adopción...\n");
            
            //Unicamente si la contraseña es correcta accedemos al menú
            menuMascotas();
          
            desencolar(laCola);//Una vez atendido el usuario, se despacha
            return;
        } else {
            intentos--;
            if (intentos > 0) {
                printf("Contraseña incorrecta. Te quedan %d intento(s).\n", intentos);
            }
        }
    }

    printf("\nHas fallado los 3 intentos. Pierdes tu lugar en la fila.\n");
    desencolar(laCola);
}

