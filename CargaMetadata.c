#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura para el metadato de un campo
typedef struct {
    char nombre[50];
    char tipo[20];
    int longitud;
} MetadatoCampo;

// Función para agregar un registro en un archivo binario
void agregarRegistro(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos) {
    FILE *archivo = fopen(nombreArchivo, "ab");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de registros");
        return;
    }

    for (int i = 0; i < numCampos; i++) {
        char valor[100];
        printf("Ingrese el valor para %s (%s): ", metadatos[i].nombre, metadatos[i].tipo);
        scanf("%s", valor);

        if (strlen(valor) > metadatos[i].longitud) {
            printf("Error: El valor es demasiado largo para %s\n", metadatos[i].nombre);
            fclose(archivo);
            return;
        }

        fwrite(valor, metadatos[i].longitud, 1, archivo);
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
        printf("Ingrese el tipo de dato para %s (Texto/Numero): ", (*metadatos)[i].nombre);
        scanf("%s", (*metadatos)[i].tipo);
        printf("Ingrese la longitud máxima para %s: ", (*metadatos)[i].nombre);
        scanf("%d", &(*metadatos)[i].longitud);
    }

    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        perror("Error al crear el archivo de metadatos");
        return;
    }

    fprintf(archivo, "%d\n", *numCampos);

    for (int i = 0; i < *numCampos; i++) {
        fprintf(archivo, "%s %s %d\n", (*metadatos)[i].nombre, (*metadatos)[i].tipo, (*metadatos)[i].longitud);
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
int cargarMetadatos(const char *nombreArchivo, MetadatoCampo **metadatos, int *numCampos) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de metadatos");
        return 0;
    }

    fscanf(archivo, "%d", numCampos);
    *metadatos = (MetadatoCampo *)malloc(sizeof(MetadatoCampo) * (*numCampos));

    for (int i = 0; i < *numCampos; i++) {
        fscanf(archivo, "%s %s %d", (*metadatos)[i].nombre, (*metadatos)[i].tipo, &((*metadatos)[i].longitud));
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

// Función para buscar un registro por campo
void buscarRegistro(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos) {
    FILE *archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de registros");
        return;
    }

    char valor[100];
    char valorBuscado[100];
    printf("Ingrese el valor a buscar: ");
    scanf("%s", valorBuscado);

    int encontrado = 0;
    while (1) {
        int leidos = 0;
        for (int i = 0; i < numCampos; i++) {
            leidos += fread(valor, 1, metadatos[i].longitud, archivo);
            valor[metadatos[i].longitud] = '\0';  // Asegurar que la cadena esté terminada correctamente
            if (strcmp(valor, valorBuscado) != 0) {
                fseek(archivo, metadatos[i].longitud - leidos, SEEK_CUR);
                break;  // No coincide, avanzar al siguiente registro
            }
        }

        if (leidos == 0) {
            break;  // Fin del archivo
        }

        if (leidos == numCampos) {
            printf("Registro encontrado:\n");
            for (int i = 0; i < numCampos; i++) {
                printf("%s: %s\n", metadatos[i].nombre, valor);
                leidos += fread(valor, 1, metadatos[i].longitud, archivo);
            }
            encontrado = 1;
        }
    }

    if (!encontrado) {
        printf("Registro no encontrado.\n");
    }

    fclose(archivo);
}

// Función para modificar un registro por campo
void modificarRegistro(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos) {
    FILE *archivo = fopen(nombreArchivo, "rb+");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de registros");
        return;
    }

    char valor[100];
    char valorBuscado[100];
    printf("Ingrese el valor a modificar: ");
    scanf("%s", valorBuscado);

    int modificado = 0;
    while (1) {
        long posicion = ftell(archivo);
        int leidos = 0;
        for (int i = 0; i < numCampos; i++) {
            leidos += fread(valor, 1, metadatos[i].longitud, archivo);
            valor[metadatos[i].longitud] = '\0';  // Asegurar que la cadena esté terminada correctamente
            if (strcmp(valor, valorBuscado) != 0) {
                fseek(archivo, metadatos[i].longitud - leidos, SEEK_CUR);
                break;  // No coincide, avanzar al siguiente registro
            }
        }

        if (leidos == 0) {
            break;  // Fin del archivo
        }

        if (leidos == numCampos) {
            printf("Registro encontrado:\n");
            for (int i = 0; i < numCampos; i++) {
                printf("%s: %s\n", metadatos[i].nombre, valor);
            }

            printf("Ingrese el nuevo valor para uno de los campos:\n");
            for (int i = 0; i < numCampos; i++) {
                printf("%s (%s): ", metadatos[i].nombre, metadatos[i].tipo);
                scanf("%s", valor);

                if (strlen(valor) > metadatos[i].longitud) {
                    printf("Error: El valor es demasiado largo para %s\n", metadatos[i].nombre);
                    fclose(archivo);
                    return;
                }

                fseek(archivo, posicion + i * metadatos[i].longitud, SEEK_SET);
                fwrite(valor, metadatos[i].longitud, 1, archivo);
            }

            modificado = 1;
            break;
        }
    }

    if (!modificado) {
        printf("Registro no encontrado.\n");
    } else {
        printf("Registro modificado con éxito.\n");
    }

    fclose(archivo);
}

// Función para eliminar un registro por campo
void eliminarRegistro(const char *nombreArchivo, MetadatoCampo *metadatos, int numCampos) {
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
    char valorBuscado[100];
    printf("Ingrese el valor a eliminar: ");
    scanf("%s", valorBuscado);

    int eliminado = 0;
    while (1) {
        int leidos = 0;
        for (int i = 0; i < numCampos; i++) {
            leidos += fread(valor, 1, metadatos[i].longitud, archivo);
            valor[metadatos[i].longitud] = '\0';  // Asegurar que la cadena esté terminada correctamente
            if (strcmp(valor, valorBuscado) != 0) {
                fseek(archivo, metadatos[i].longitud - leidos, SEEK_CUR);
                break;  // No coincide, avanzar al siguiente registro
            }
        }

        if (leidos == 0) {
            break;  // Fin del archivo
        }

        if (leidos == numCampos) {
            printf("Registro encontrado y eliminado:\n");
            eliminado = 1;
        } else {
            for (int i = 0; i < numCampos; i++) {
                fwrite(valor, metadatos[i].longitud, 1, tempArchivo);
            }
        }
    }

    if (!eliminado) {
        printf("Registro no encontrado.\n");
    }

    fclose(archivo);
    fclose(tempArchivo);

    // Reemplazar el archivo original con el archivo temporal
    remove(nombreArchivo);
    rename("temp.bin", nombreArchivo);
}


