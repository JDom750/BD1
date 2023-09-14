#include <stdio.h>
#include <string.h>

#define MAX_CLIENTES 100
#define MAX_CAMPOS 10
#define MAX_CARACTERES 50

// Estructura para representar un cliente
struct Cliente {
    char campos[MAX_CAMPOS][MAX_CARACTERES];
    char valores[MAX_CAMPOS][MAX_CARACTERES];
};

// Función para agregar un registro a la lista
void agregarRegistro(struct Cliente lista[], int *numClientes, int *numCampos) {
    if (*numClientes < MAX_CLIENTES) {
        for (int i = 0; i < *numCampos; i++) {
            printf("Ingrese %s: ", lista[0].campos[i]);
            scanf("%s", lista[*numClientes].valores[i]);
        }
        (*numClientes)++;
        printf("Registro agregado con éxito.\n");
    } else {
        printf("La lista de clientes está llena.\n");
    }
}

// Función principal del programa
int main() {
    struct Cliente listaClientes[MAX_CLIENTES];
    int numClientes = 0;
    int numCampos = 0;

    // Permitir al usuario definir la estructura de clientes
    printf("Definición de la estructura de clientes (máximo %d campos, 'fin' para terminar):\n", MAX_CAMPOS);
    while (numCampos < MAX_CAMPOS) {
        printf("Nombre del campo (o 'fin' para terminar): ");
        scanf("%s", listaClientes[0].campos[numCampos]);
        if (strcmp(listaClientes[0].campos[numCampos], "fin") == 0) {
            break;
        }
        numCampos++;
    }

    while (1) {
        printf("\nOpciones:\n");
        printf("1. Agregar registro\n");
        printf("2. Mostrar lista de clientes\n");
        printf("3. Salir\n");

        int opcion;
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                agregarRegistro(listaClientes, &numClientes, &numCampos);
                break;
            case 2:
                printf("Lista de clientes:\n");
                for (int i = 0; i < numClientes; i++) {
                    printf("Registro %d:\n", i + 1);
                    for (int j = 0; j < numCampos; j++) {
                        printf("%s: %s\n", listaClientes[0].campos[j], listaClientes[i].valores[j]);
                    }
                }
                break;
            case 3:
                return 0; // Salir del programa
            default:
                printf("Opción no válida. Intente nuevamente.\n");
        }
    }

    return 0;
}
