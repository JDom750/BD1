#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int tamRegistro;

// Estructura para el metadato de un campo
typedef struct {
    char nombre[50];
    int longitud;
} MetadatoCampo;

// Función para buscar un registro por campo
void buscarRegistroPorPosicion(const char *nombreArchivo, MetadatoCampo* metadatos, int numCampos, int posicionBuscada) {
    FILE *archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de registros");
        return;
    }

    fseek(archivo, 0, SEEK_END);  // Moverse al final del archivo para obtener la longitud total
    long longitudArchivo = ftell(archivo); //ftell me devuelve el tamaño del archivo con los registros cargados
    long longitudRegistro = 0;

    if (longitudArchivo == 0) {
        printf("El archivo de registros está vacío.\n");
        fclose(archivo);
        return;
    }
    printf("\nlongArchivo: %li\t tamRegistro: %d\n",longitudArchivo, tamRegistro);
    if (longitudArchivo % (tamRegistro) != 0) {
        printf("Error: Tamaño de registro no válido.\n");
        fclose(archivo);
        return;
    }

    longitudRegistro = longitudArchivo / tamRegistro;

    if (posicionBuscada < 0 || posicionBuscada >= longitudRegistro) {
        printf("Posición de registro no válida.\n");
        fclose(archivo);
        return;
    }


 //   fseek(archivo, posicionBuscada *numCampos * sizeof(MetadatoCampo), SEEK_SET);  // Moverse a la posición deseada
    fseek(archivo, tamRegistro*(posicionBuscada-1), SEEK_SET);  // Moverse a la posición deseada
    

    printf("Registro encontrado en la posición %d:\n", posicionBuscada);
    for (int i = 0; i < numCampos; i++) {
        char valor[100];
        fread(valor, 1, metadatos[i].longitud, archivo);
        valor[metadatos[i].longitud] = '\0';  // Asegurar que la cadena esté terminada correctamente
        printf("%s: %s\n", metadatos[i].nombre, valor);
    }

    fclose(archivo);
}


/* void buscarRegistroPorPosicion(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos, int posicion) {
    FILE *archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de registros");
        return;
    }

    // Calcular la posición física del registro
    long posicionFisica = sizeof(MetadatoCampo) * numCampos * posicion;

    // Mover el puntero del archivo a la posición deseada
    fseek(archivo, posicionFisica, SEEK_SET);

    char valor[100];
    for (int i = 0; i < numCampos; i++) {
        fread(valor, 1, metadatos[i].longitud, archivo);
        valor[metadatos[i].longitud] = '\0'; // Asegurar que la cadena esté terminada correctamente
        printf("%s: %s\n", metadatos[i].nombre, valor);
    }

    fclose(archivo);
} */


void buscarRegistro(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos) {
    int posicion;
    printf("Ingrese la posición del registro a buscar: ");
    scanf("%d", &posicion);


    buscarRegistroPorPosicion(nombreArchivo, metadatos, numCampos, posicion);
}


// Función para modificar un registro por campo
void modificarRegistroPorPosicion(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos, int posicion) {
    FILE *archivo = fopen(nombreArchivo, "rb+");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de registros");
        return;
    }

    char valor[100];
    int contadorRegistros = 0;

    while (1) {
        long posicionFisica = sizeof(MetadatoCampo) * numCampos * contadorRegistros;
        fseek(archivo, posicionFisica, SEEK_SET);

        int leidos = 0;
        for (int i = 0; i < numCampos; i++) {
            leidos += fread(valor, 1, metadatos[i].longitud, archivo);
            valor[metadatos[i].longitud] = '\0';  // Asegurar que la cadena esté terminada correctamente
        }

        if (leidos == 0) {
            break;  // Fin del archivo
        }

        if (contadorRegistros == posicion) {
            // Este es el registro a modificar
            printf("Registro encontrado:\n");
            for (int i = 0; i < numCampos; i++) {
                printf("%s: %s\n", metadatos[i].nombre, valor);
                printf("Ingrese el nuevo valor para %s: ", metadatos[i].nombre);
                scanf("%s", valor);

                if (strlen(valor) > metadatos[i].longitud) {
                    printf("Error: El valor es demasiado largo para %s\n", metadatos[i].nombre);
                    fclose(archivo);
                    return;
                }

                fseek(archivo, posicionFisica + i * metadatos[i].longitud, SEEK_SET);
                fwrite(valor, metadatos[i].longitud, 1, archivo);
            }
            fclose(archivo);
            printf("Registro modificado con éxito.\n");
            return;
        }

        contadorRegistros++;
    }

    fclose(archivo);
    printf("Registro no encontrado.\n");
}

void modificarRegistro(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos) {
    int posicion;
    printf("Ingrese la posición del registro a modificar: ");
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

    char valor[100];
    int contadorRegistros = 0;

    while (1) {
        //long posicionFisica = sizeof(MetadatoCampo) * numCampos * contadorRegistros;
        fseek(archivo, (posicion-1)*tamRegistro, SEEK_SET);

        int leidos = 0;
        for (int i = 0; i < numCampos; i++) {
            leidos += fread(valor, 1, metadatos[i].longitud, archivo);
            valor[metadatos[i].longitud] = '\0';  // Asegurar que la cadena esté terminada correctamente
        }

        if (leidos == 0) {
            break;  // Fin del archivo
        }

        if (contadorRegistros != posicion) {
            // No es el registro a eliminar, copiarlo al archivo temporal
            for (int i = 0; i < numCampos; i++) {
                fwrite(valor, metadatos[i].longitud, 1, tempArchivo);
            }
        }

        contadorRegistros++;
    }

    fclose(archivo);
    fclose(tempArchivo);

    // Reemplazar el archivo original con el archivo temporal
    remove(nombreArchivo);
    rename("temp.bin", nombreArchivo);
}

void eliminarRegistro(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos) {
    int posicion;
    printf("Ingrese la posición del registro a eliminar: ");
    scanf("%d", &posicion);

    eliminarRegistroPorPosicion(nombreArchivo, metadatos, numCampos, posicion);
}


// Función para agregar un registro en un archivo binario
void agregarRegistro(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos) {
    FILE *archivo = fopen(nombreArchivo, "ab");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de registros");
        return;
    }

    for (int i = 0; i < numCampos; i++) {
        char valor[100];
        printf("Ingrese el valor para %s: ", metadatos[i].nombre);
        scanf("%s", valor);

        if (strlen(valor) > metadatos[i].longitud) {
            printf("Error: El valor es demasiado largo para %s\n", metadatos[i].nombre);
            fclose(archivo);
            return;
        }

        fwrite(valor, metadatos[i].longitud, 1, archivo);
        fflush(archivo);  // Forzar escritura inmediata de datos al archivo
    }

    fclose(archivo);
    printf("Registro agregado con éxito.\n");
}
 


// Función para crear un archivo de metadatos con el formato de registro
void crearMetadatos(const char *nombreArchivo, MetadatoCampo **metadatos, int *numCampos) {
    printf("Ingrese el número de campos en el formato de registro: ");
    scanf("%d", numCampos);

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
    printf("Metadatos guardados con éxito en %s.\n", nombreArchivo);

    //Puesto por Facu para poder resetear el archivo de registros cuando se recrea la metadata
    //Sino quedaria inconsistente el programa dejando registros viejos con formatos diferentes
    //En caso de que tire error se puede borrar
    FILE *archivo2 = fopen("registros.bin","wb");
    fclose(archivo2);
    //----------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------
}

// Función para cargar los metadatos desde un archivo de texto
/* int cargarMetadatos(const char *nombreArchivo, MetadatoCampo **metadatos, int *numCampos) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de metadatos");
        return 0;
    }

    fscanf(archivo, "%d", numCampos);
    *metadatos = (MetadatoCampo *)malloc(sizeof(MetadatoCampo) * (*numCampos));

    for (int i = 0; i < *numCampos; i++) {
        fscanf(archivo, "%s %d", (*metadatos)[i].nombre, &((*metadatos)[i].longitud));
        tamRegistro+=((*metadatos[i]).longitud);
    }

    fclose(archivo);
    return 1;
} */
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

    printf("¿Desea cargar un nuevo formato de registro? (1: Sí / 0: No): ");
    int cargarNuevoFormato;
    scanf("%d", &cargarNuevoFormato);

    if (cargarNuevoFormato) {
        crearMetadatos(nombreMetadatos, &metadatos, &numCampos);
    } else {
        if (!cargarMetadatos(nombreMetadatos, &metadatos, &numCampos)) {
            return 1;
        }
    }
    printf("\ntamaño registro:%d\n",tamRegistro);

    while (1) {
        printf("\nElija una opción:\n");
        printf("1. Agregar registro\n");
        printf("2. Buscar registro\n");
        printf("3. Modificar registro\n");
        printf("4. Eliminar registro\n");
        printf("5. Salir\n");
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
                free(metadatos);
                return 0;
            default:
                printf("Opción no válida.\n");
        }
    }

    return 0;
}


/* DE ACA EN ADELANTE CODIGO DE FACU, TODO LO ANTERIOR FUNCIONA Y ESTA COMPROBADO----------------
   DE ACA EN ADELANTE NO HAY NINGUNA VERIFICACION -----------------------------------------------
------------------------------------------------------
------------------------------------------------------ */




void mostrarArchivo(const char nombreArchivo, MetadatoCampometadatos, int numCampos) {
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
        int j= 0;
        if (!feof(nombreArchivo)) {
        // Si no se leen la cantidad esperada de bytes, se asume que llegamos al final del archivo
        printf("Registro %d:\n", contadorRegistros + 1);
        fclose(archivo);
        return;
        }
        for (int i=0; i < numCampos; i++) {
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