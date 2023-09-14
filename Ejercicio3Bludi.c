#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char nombre_campo[50];
    int longitud;
}Campo;

void guardar_metadato(){ //Cargo la metadata al primer archivo binario
    printf("Ingrese la cantidad de campos:");
    int cant_campos;
    scanf("%d", &cant_campos);
    Campo *metadata = (Campo *)malloc(cant_campos * sizeof(Campo));
    int i;
    for (i = 0; i < cant_campos; i++){
        printf("Ingrese el nombre del campo %d: ", i + 1);
        scanf("%s", &metadata[i].nombre_campo);
        printf("Ingrese la longitud del campo %d:", i + 1);
        scanf("%d", &metadata[i].longitud);
    }
    printf("Carga de metadata terminada.\n");
    FILE *archivo = fopen("archivometadatos.dat", "wb");
    for (i = 0; i < cant_campos; i++){
        fwrite(&metadata[i], sizeof(Campo), 1, archivo);
    }
    fflush(archivo);
    fclose(archivo);
}

void verificar_metadato() { //Funcion que uso para verificar si la metadata se cargo BIEN al archivo binario
    FILE *archivo = fopen("archivometadatos.dat", "rb");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    Campo *metadata;
    int cant_campos = 5;

    // Leer la cantidad de campos desde el archivo
    //fread(&cant_campos, sizeof(int), 2, archivo);

    // Asignar memoria para los metadatos
    metadata = (Campo *)malloc(cant_campos * sizeof(Campo));

    if (metadata == NULL) {
        printf("Error al asignar memoria.\n");
        fclose(archivo);
        return;
    }

    // Leer los datos del archivo
    fread(metadata, sizeof(Campo), cant_campos, archivo);

    // Mostrar los datos en pantalla
    int i;
    for (i = 0; i < cant_campos; i++) {
        printf("Campo %d: Nombre: %s, Longitud: %d\n", i + 1, metadata[i].nombre_campo, metadata[i].longitud);
    }

    fclose(archivo);
    free(metadata); // Liberar la memoria asignada
}

void cargar_dato(Campo *metadata, int cant_campos){ //Funcion para cargar los datos al archivo principal
    FILE *archivo = fopen("datos.dat", "ab");
    if (archivo == NULL){
        FILE *archivo = fopen("datos.dat", "ab+");
    }
    int i;
    for (i = 0; i < cant_campos; i++) {
        printf("Ingrese el valor para %s (%d caracteres máximo): ", metadata[i].nombre_campo, metadata[i].longitud);
        char valor[metadata[i].longitud + 1]; // +1 para el carácter nulo
        scanf("%s", valor);

        // Escribir el valor en el archivo
        fwrite(valor, sizeof(char), metadata[i].longitud, archivo);
    }
}
void leer_metadata(Campo **metadata, int *cant_campos) {
    FILE *archivo = fopen("archivometadatos.dat", "rb");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo de metadatos.\n");
        return;
    }

    // Inicializar el contador de campos
    *cant_campos = 0;

    Campo *temp_metadata = NULL;

    // Bucle para leer campos hasta el final del archivo
    while (1) {
        temp_metadata = (Campo *)realloc(temp_metadata, (*cant_campos + 1) * sizeof(Campo));
        if (temp_metadata == NULL) {
            printf("Error al asignar memoria.\n");
            fclose(archivo);
            return;
        }

        size_t num_leidos = fread(&temp_metadata[*cant_campos], sizeof(Campo), 1, archivo);
        
        // Si no se pudo leer un registro completo, terminar el bucle
        if (num_leidos != 1) {
            break;
        }

        (*cant_campos)++;
    }

    fclose(archivo);

    // Asignar el puntero metadata
    *metadata = temp_metadata;
}
int main(){
 guardar_metadato();
 //verificar_metadato();

 Campo *metadata;
 int cant_campos;

 leer_metadata(&metadata, &cant_campos);
 cargar_dato(metadata, cant_campos);

return 0;

}