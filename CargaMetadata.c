#include <stdio.h>
#include <string.h>

// Estructura para representar un cliente
struct Cliente {
    char nombre[50];
    char apellido[50];
    char email[100];
};

// Función para agregar un registro a la lista
void agregarRegistro(struct Cliente lista[], int *numClientes) {
    if (*numClientes < 100) {
        struct Cliente nuevoCliente;
        printf("Ingrese el nombre: ");
        scanf("%s", nuevoCliente.nombre);
        printf("Ingrese el apellido: ");
        scanf("%s", nuevoCliente.apellido);
        printf("Ingrese el email: ");
        scanf("%s", nuevoCliente.email);
        
        lista[*numClientes] = nuevoCliente;
        (*numClientes)++;
        printf("Registro agregado con éxito.\n");
    } else {
        printf("La lista de clientes está llena.\n");
    }
}

// Función para eliminar un registro por posición
void eliminarRegistro(struct Cliente lista[], int *numClientes, int posicion) {
    if (posicion >= 0 && posicion < *numClientes) {
        for (int i = posicion; i < *numClientes - 1; i++) {
            lista[i] = lista[i + 1];
        }
        (*numClientes)--;
        printf("Registro eliminado con éxito.\n");
    } else {
        printf("La posición de registro no es válida.\n");
    }
}

// Función para actualizar un registro por posición
void actualizarRegistro(struct Cliente lista[], int numClientes, int posicion) {
    if (posicion >= 0 && posicion < numClientes) {
        struct Cliente *cliente = &lista[posicion];
        printf("Ingrese el nuevo nombre: ");
        scanf("%s", cliente->nombre);
        printf("Ingrese el nuevo apellido: ");
        scanf("%s", cliente->apellido);
        printf("Ingrese el nuevo email: ");
        scanf("%s", cliente->email);
        printf("Registro actualizado con éxito.\n");
    } else {
        printf("La posición de registro no es válida.\n");
    }
}

// Función principal del programa
int main() {
    struct Cliente listaClientes[100];
    int numClientes = 0;
    
    while (1) {
        printf("\nOpciones:\n");
        printf("1. Agregar registro\n");
        printf("2. Eliminar registro por posición\n");
        printf("3. Actualizar registro por posición\n");
        printf("4. Mostrar lista de clientes\n");
        printf("5. Salir\n");
        
        int opcion;
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        
        switch (opcion) {
            case 1:
                agregarRegistro(listaClientes, &numClientes);
                break;
            case 2:
                if (numClientes > 0) {
                    int posicion;
                    printf("Ingrese la posición del registro a eliminar: ");
                    scanf("%d", &posicion);
                    eliminarRegistro(listaClientes, &numClientes, posicion);
                } else {
                    printf("La lista de clientes está vacía.\n");
                }
                break;
            case 3:
                if (numClientes > 0) {
                    int posicion;
                    printf("Ingrese la posición del registro a actualizar: ");
                    scanf("%d", &posicion);
                    actualizarRegistro(listaClientes, numClientes, posicion);
                } else {
                    printf("La lista de clientes está vacía.\n");
                }
                break;
            case 4:
                printf("Lista de clientes:\n");
                for (int i = 0; i < numClientes; i++) {
                    printf("Registro %d: %s %s %s\n", i + 1, listaClientes[i].nombre, listaClientes[i].apellido, listaClientes[i].email);
                }
                break;
            case 5:
                return 0; // Salir del programa
            default:
                printf("Opción no válida. Intente nuevamente.\n");
        }
    }
    
    return 0;
}
