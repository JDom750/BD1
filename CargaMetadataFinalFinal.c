#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//-------------------------------------------------
//Alumnos:
//Nombre y Apellido: Facundo Schwab.
//Legajo: 171801.
//Nombre y Apellido: Joaquin Dominguez.
//Legajo: 182901
//Nombre y Apellido: Luciano Scialchi
//Legajo: 166719.
//Nombre y Apellido: Lucas Emanuelli
//Legajo: 175646.

//-------------------------------------------------


int tamRegistro;

// Estructura para el metadato de un campo
typedef struct {
    char nombre[50];
    int longitud;
} MetadatoCampo;

// Función para buscar un registro por campo
void buscarRegistroPorPosicion(const char *nombreArchivo, MetadatoCampo* metadatos, int numCampos, int posicionBuscada) {
    // Validar si la posición buscada es menor a 0 o mayor que la cantidad de registros

    posicionBuscada = posicionBuscada - 1;

    if (posicionBuscada < 0) {
        printf("Posicion de registro no valida. Debe ser mayor o igual a 0.\n");
        return;
    }

    FILE *archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de registros");
        return;
    }

    // Calcular el tamaño de un registro completo
    long tamRegistro = 0;
    for (int i = 0; i < numCampos; i++) {
        tamRegistro += metadatos[i].longitud;
    }

    // Calcular la cantidad de registros en el archivo
    fseek(archivo, 0, SEEK_END);
    long longitudArchivo = ftell(archivo);
    long cantidadRegistros = longitudArchivo / tamRegistro;

    // Validar si la posición buscada es mayor que la cantidad de registros
    if (posicionBuscada >= cantidadRegistros) {
        printf("Posicion de registro no valida. No existe un registro en la posicion %d.\n", posicionBuscada);
        fclose(archivo);
        return;
    }

    // Calcular la posición de inicio del registro deseado
    long posicionInicio = tamRegistro * (long)posicionBuscada;

    // Moverse a la posición deseada
    fseek(archivo, posicionInicio, SEEK_SET);

    // Mostrar la posición real del registro sumando 1
    printf("Registro encontrado en la posicion %d:\n", posicionBuscada + 1);
    
    for (int i = 0; i < numCampos; i++) {
        char valor[100];
        fread(valor, 1, metadatos[i].longitud, archivo);
        valor[metadatos[i].longitud] = '\0';  // Asegurar que la cadena esté terminada correctamente
        printf("%s: %s\n", metadatos[i].nombre, valor);
    }

    fclose(archivo);
}
// Función para mostrar el contenido de un archivo binario
// Función para mostrar el contenido de un archivo binario con todos los registros y su contenido
void mostrarArchivo(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos) {
    FILE *archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    char valor[100];
    int contadorRegistros = 0;
    printf("Mostramos los registros del archivo: \n");
    printf("---------------------------------------------------------------- \n");
    while (1) {
        int leidos = 0;
        printf("Registro %d:\n", contadorRegistros + 1);
        for (int i = 0; i < numCampos; i++) {
            leidos = fread(valor, 1, metadatos[i].longitud, archivo);
            if (leidos != metadatos[i].longitud) {
                // Si no se leen la cantidad esperada de bytes, se asume que llegamos al final del archivo
                fclose(archivo);
                return;
            }
            valor[metadatos[i].longitud] = '\0';  // Asegurar que la cadena esté terminada correctamente
            
            printf("%s: %s\n", metadatos[i].nombre, valor);
        }
        printf("\n");
        contadorRegistros++;
        printf("---------------------------------------------------------------- \n");
    }

    fclose(archivo);
}





void buscarRegistro(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos) {
    int posicion;
    printf("Ingrese la posición del registro a buscar: ");
    scanf("%d", &posicion);


    buscarRegistroPorPosicion(nombreArchivo, metadatos, numCampos, posicion);
}



//Modificar Con FGest.
void modificarRegistroPorPosicion(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos, int posicion) {
    FILE *archivo = fopen(nombreArchivo, "rb+");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de registros");
        return;
    }

    posicion = posicion - 1;  // Ajustar posición para comenzar desde 0
    char *registro = (char *)malloc(tamRegistro);  // Espacio para un registro completo
    int contadorRegistros = 0;

    while (1) {
        int leidos = fread(registro, 1, tamRegistro, archivo);

        if (leidos != tamRegistro) {
            break;  // Fin del archivo
        }

        if (contadorRegistros == posicion) {
            // Este es el registro a modificar
            printf("Registro encontrado:\n");
            for (int i = 0; i < numCampos; i++) {
                char nuevoValor[100];
                if (i == 0) {
                    // Caso especial cuando i es 0, acceder al inicio del registro
                    printf("%s: %s\n", metadatos[i].nombre, registro);
                } else {
                    // Acceder al campo dentro del registro usando la fórmula corregida
                    printf("%s: %s\n", metadatos[i].nombre, registro + ((i - 1) * metadatos[i - 1].longitud + metadatos[i - 1].longitud));
                }
                printf("Ingrese el nuevo valor para %s: ", metadatos[i].nombre);
                fflush(stdin); // Limpia el búfer de entrada antes de usar fgets
                fgets(nuevoValor, sizeof(nuevoValor), stdin);
                nuevoValor[strcspn(nuevoValor, "\n")] = '\0'; // Elimina el carácter de nueva línea al final

                //Valida si el dato es mayor a la longitud o si es campo es vacia.
                while (strlen(nuevoValor) > metadatos[i].longitud - 1 || strlen(nuevoValor) == 0) {
                    printf("Error: El valor es demasiado largo para %s\n", metadatos[i].nombre);
                    printf("Ingrese el nuevo valor para %s: ", metadatos[i].nombre);
                    fflush(stdin); // Limpia el búfer de entrada antes de usar fgets
                    fgets(nuevoValor, sizeof(nuevoValor), stdin);
                    nuevoValor[strcspn(nuevoValor, "\n")] = '\0'; // Elimina el carácter de nueva línea al final
                }

                // Copiar el nuevo valor al campo correspondiente dentro del registro
                strncpy(registro + ((i - 1) * metadatos[i - 1].longitud + metadatos[i - 1].longitud), nuevoValor, metadatos[i].longitud);
            }

            // Posicionarse en el inicio del registro y escribir el registro modificado
            fseek(archivo, posicion * tamRegistro, SEEK_SET);
            fwrite(registro, 1, tamRegistro, archivo);

            fflush(archivo); // Forzar la escritura inmediata al archivo
            free(registro);
            fclose(archivo);
            printf("Registro modificado con éxito.\n");
            return;
        }

        contadorRegistros++;
    }

    free(registro);
    fclose(archivo);
    printf("Registro no encontrado.\n");
}
void modificarRegistro(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos) {
    int posicion;
    printf("Ingrese la posicion del registro a modificar: ");
    scanf("%d", &posicion);

    modificarRegistroPorPosicion(nombreArchivo, metadatos, numCampos, posicion);
}


// Función para eliminar un registro por campo
void eliminarRegistroPorPosicion(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos, int posicion) {
    FILE *archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de registros");
        return;
    }

    FILE *tempArchivo = fopen("temp.bin", "wb");
    if (tempArchivo == NULL) {
        perror("Error al crear archivo temporal");
        fclose(archivo);
        return;
    }

    posicion = posicion - 1;  // Ajustar posición para comenzar desde 0
    char *registro = (char *)malloc(tamRegistro);  // Espacio para un registro completo
    int contadorRegistros = 0;
    int eliminado = 0;

    while (1) {
        int leidos = fread(registro, 1, tamRegistro, archivo);

        if (leidos != tamRegistro) {
            break;  // Fin del archivo
        }

        if (contadorRegistros != posicion) {
            // No es el registro a eliminar, copiarlo al archivo temporal
            fwrite(registro, 1, tamRegistro, tempArchivo);
        } else {
            eliminado = 1;
        }

        contadorRegistros++;
    }

    fclose(archivo);
    fclose(tempArchivo);
    free(registro);

    // Verificar si se eliminó el registro
    if (eliminado) {
        // Reemplazar el archivo original con el archivo temporal
        remove(nombreArchivo);
        rename("temp.bin", nombreArchivo);
        printf("Registro en la posicion %d eliminado con exito.\n", posicion + 1);  // Mostrar posición real
    } else {
        printf("Registro en la posicion %d no encontrado o no pudo ser eliminado.\n", posicion + 1);  // Mostrar posición real
        remove("temp.bin"); // Eliminar el archivo temporal sin cambios
    }
}

void eliminarRegistro(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos) {
    int posicion;
    printf("Ingrese la posicion del registro a eliminar: ");
    scanf("%d", &posicion);

    eliminarRegistroPorPosicion(nombreArchivo, metadatos, numCampos, posicion);
}

void limpiarBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}



//Agregar Con Fgest-
void agregarRegistro(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos) {
    FILE *archivo = fopen(nombreArchivo, "ab");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de registros");
        return;
    }

    for (int i = 0; i < numCampos; i++) {
        char valor[100];
        printf("Ingrese el valor para %s: ", metadatos[i].nombre);
        fflush(stdin); // Limpia el búfer de entrada antes de usar fgets
        fgets(valor, sizeof(valor), stdin);
        valor[strcspn(valor, "\n")] = '\0'; // Elimina el carácter de nueva línea al final

        // Elimina el carácter de nueva línea al final de la cadena
        if (valor[strlen(valor) - 1] == '\n') {
            valor[strlen(valor) - 1] = '\0';
        }

        while (strlen(valor) > metadatos[i].longitud - 1 || strlen(valor) == 0) {
            printf("Error: El valor es demasiado largo o es vacio para %s\n", metadatos[i].nombre);
            printf("Ingrese el valor para %s: ", metadatos[i].nombre);
            fflush(stdin); // Limpia el búfer de entrada antes de usar fgets
            fgets(valor, sizeof(valor), stdin);
            valor[strcspn(valor, "\n")] = '\0'; // Elimina el carácter de nueva línea al final
        }
        fwrite(valor, metadatos[i].longitud, 1, archivo);
    }

    fclose(archivo);
    printf("Registro agregado con exito.\n");
}

bool esEntero(const char *cadena) {
    // Verifica si la cadena contiene solo dígitos y, opcionalmente, un signo negativo al principio.
    if (*cadena == '-' || (*cadena >= '0' && *cadena <= '9')) {
        for (int i = 1; cadena[i]; i++) {
            if (cadena[i] < '0' || cadena[i] > '9') {
                return false; // Caracter no válido encontrado.
            }
        }
        return true; // La cadena es un número entero.
    }
    return false; // No comienza con '-' o un dígito.
}

// Función para crear un archivo de metadatos con el formato de registro
void crearMetadatos(const char *nombreArchivo, MetadatoCampo **metadatos, int *numCampos) {

 char input[50]; // Tamaño de la cadena de entrada según tus necesidades.

    printf("Ingrese el numero de campos en el formato de registro (no debe superar 10): ");
    scanf("%s", input);

    while (!esEntero(input) || atoi(input) <= 0 || atoi(input) > 10) {
        printf("Entrada invalida. Por favor, ingrese un número entero entre 1 y 10: ");
        scanf("%s", input);
    }

    *numCampos = atoi(input); // Convertir la cadena a un número entero.

    printf("Ha ingresado el valor válido: %d\n", numCampos);


    *metadatos = (MetadatoCampo *)malloc(sizeof(MetadatoCampo) * (*numCampos));
    if (*metadatos == NULL) {
        perror("Error al asignar memoria para metadatos");
        return;
    }

    for (int i = 0; i < *numCampos; i++) {
        printf("Ingrese el nombre del campo %d: ", i + 1);
        scanf("%s", (*metadatos)[i].nombre);
        printf("Ingrese la longitud máxima para %s: ", (*metadatos)[i].nombre);
        scanf("%d", &(*metadatos)[i].longitud);

        (*metadatos)[i].longitud ++;
        tamRegistro += (*metadatos)[i].longitud;
    }

    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        perror("Error al crear el archivo de metadatos");
        return;
    }

    fprintf(archivo, "%d\n", *numCampos);

    for (int i = 0; i < *numCampos; i++) {
        fprintf(archivo, "%s %d\n", (*metadatos)[i].nombre, (*metadatos)[i].longitud);
    }

    fclose(archivo);
    printf("Metadatos guardados con exito en %s.\n", nombreArchivo);


    FILE *archivo2 = fopen("registros.bin","wb");
    fclose(archivo2);
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
}


int cargarMetadatos(const char *nombreArchivo, MetadatoCampo **metadatos, int *numCampos) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de metadatos");
        return 0;
    }

    fscanf(archivo, "%d", numCampos);
    *metadatos = (MetadatoCampo *)malloc(sizeof(MetadatoCampo) * (*numCampos));

    for (int i = 0; i < *numCampos; i++) {
        fscanf(archivo, "%s %d", (*metadatos)[i].nombre, &((*metadatos)[i].longitud));
        // Utiliza la notación de flecha para acceder a la longitud
        tamRegistro += (*metadatos)[i].longitud;
    }

    fclose(archivo);
    return 1;
}


// Función principal
int main() {
    const char *nombreMetadatos = "metadatos.txt";
    const char *nombreRegistros = "registros.bin";
    MetadatoCampo *metadatos;
    int numCampos;

    
    int cargarNuevoFormato; // Declara cargarNuevoFormato como un número entero

    // Utiliza un bucle do-while para solicitar entrada repetidamente hasta que se proporcione un número entero válido
    do {
        printf("Desea cargar un nuevo formato de registro? (1: Si / 0: No): ");
        if (scanf("%d", &cargarNuevoFormato) != 1) {
            // Si scanf no lee un número entero, limpia el búfer de entrada y vuelve a intentarlo
            printf("Entrada no valida. Introduzca 0 o 1.\n");
            while (getchar() != '\n'); // Limpia el búfer de entrada
        }
        else if (cargarNuevoFormato < 0 || cargarNuevoFormato > 1) {
            // Si el número entero introducido no es 0 o 1, solicita la entrada nuevamente
            printf("Por favor, introduzca 0 o 1.\n");
        }
    } while (cargarNuevoFormato < 0 || cargarNuevoFormato > 1);


    if (cargarNuevoFormato) {
        crearMetadatos(nombreMetadatos, &metadatos, &numCampos);
    } else {
        if (!cargarMetadatos(nombreMetadatos, &metadatos, &numCampos)) {
            return 1;
        }
    }
    //printf("\n tamanio registro:%d\n",tamRegistro);

    while (1) {
        printf("\n Elija una opcion:\n");
        printf("1. Agregar registro\n");
        printf("2. Buscar registro\n");
        printf("3. Modificar registro\n");
        printf("4. Eliminar registro\n");
        printf("5. Mostrar registro\n");
        printf("6. Salir\n");
        int opcion;
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                agregarRegistro(nombreRegistros, metadatos, numCampos);
                break;
            case 2:
                buscarRegistro(nombreRegistros,metadatos,numCampos);
                break;
            case 3:
                modificarRegistro(nombreRegistros,metadatos, numCampos);
                break;
            case 4:
                eliminarRegistro(nombreRegistros, metadatos, numCampos);
                break;
            case 5:
                mostrarArchivo(nombreRegistros, metadatos, numCampos);
                break;
            case 6:
                free(metadatos);
                return 0;
            default:
                printf("Opcion no valida.\n");
        }
    }

    return 0;
}





