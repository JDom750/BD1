#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabla_hash.h" // Asegúrate de incluir la interfaz de la tabla hash aquí

#define MAX_CLIENTES 700

typedef struct cliente {
    int id; // Cambiamos la clave a un campo único, que es el código de cliente
    char apellido[20];
    char nombre[20];
} cliente;

int calcularHash(int clave) {
    // Aquí puedes implementar tu función de hash, por ejemplo, una simple módulo
    return clave % MAX_CLIENTES;
}

int main() {
    TablaHash tabla = th_crear(MAX_CLIENTES, calcularHash);

    int opcion;
    int codigo_cliente;

    do {
        printf("\n");
        printf("1. Alta de Cliente\n");
        printf("2. Buscar Cliente por Código\n");
        printf("3. Modificar Cliente por Código\n");
        printf("4. Dar de Baja Cliente por Código\n");
        printf("5. Mostrar Clientes\n");
        printf("0. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: {
                cliente nuevoCliente;
                printf("Ingrese el código del cliente (único, mayor que 0): ");
                scanf("%d", &nuevoCliente.id);
                printf("Ingrese el nombre del cliente: ");
                scanf("%s", nuevoCliente.nombre);
                printf("Ingrese el apellido del cliente: ");
                scanf("%s", nuevoCliente.apellido);
/*                 // Create a new cliente structure and copy data from nuevoCliente
                cliente *nuevoClientePtr = (cliente *)malloc(sizeof(cliente));
                nuevoClientePtr->id = nuevoCliente.id;
                strcpy(nuevoClientePtr->nombre, nuevoCliente.nombre);
                strcpy(nuevoClientePtr->apellido, nuevoCliente.apellido); */

                // Insert the cliente in the hash table using its código as clave
                TipoElemento x = te_crear_con_valor(nuevoCliente.id, &nuevoCliente);
                th_insertar(tabla, x);
                
/*                 // Insertamos el cliente en la tabla hash utilizando su código como clave
                TipoElemento x = te_crear_con_valor(nuevoCliente.id,(void*)nuevoCliente);
                th_insertar(tabla, x); */
                printf("Cliente agregado con éxito.\n");
                break;
            }
            case 2: {
                printf("Ingrese el código del cliente que desea buscar: ");
                scanf("%d", &codigo_cliente);

                // Recuperamos el cliente por su código
                cliente *clienteEncontrado = (cliente *)th_recuperar(tabla, codigo_cliente);
                if (clienteEncontrado != NULL) {
                    printf("Cliente encontrado:\n");
                    printf("Código: %d\n", clienteEncontrado->id);
                    printf("Nombre: %s\n", clienteEncontrado->nombre);
                    printf("Apellido: %s\n", clienteEncontrado->apellido);
                } else {
                    printf("Cliente no encontrado.\n");
                }
                break;
            }
            case 3: {
                printf("Ingrese el código del cliente que desea modificar: ");
                scanf("%d", &codigo_cliente);

                cliente *clienteAModificar = (cliente *)th_recuperar(tabla, codigo_cliente);
                if (clienteAModificar != NULL) {
                    printf("Cliente encontrado:\n");
                    printf("Código: %d\n", clienteAModificar->id);
                    printf("Nombre actual: %s\n", clienteAModificar->nombre);
                    printf("Apellido actual: %s\n", clienteAModificar->apellido);

                    printf("Ingrese el nuevo nombre: ");
                    scanf("%s", clienteAModificar->nombre);

                    printf("Ingrese el nuevo apellido: ");
                    scanf("%s", clienteAModificar->apellido);
                    
                    printf("Cliente modificado con éxito.\n");
                } else {
                    printf("Cliente no encontrado.\n");
                }
                break;
            }
            case 4: {
                printf("Ingrese el código del cliente que desea dar de baja: ");
                scanf("%d", &codigo_cliente);

                // Eliminamos el cliente por su código
                th_eliminar(tabla, codigo_cliente);
                printf("Cliente eliminado con éxito.\n");
                break;
            }
            case 5: {
                // Mostramos todos los clientes en la tabla hash
                th_mostrar(tabla);
                break;
            }
            case 0: {
                // Liberamos la memoria de la tabla hash antes de salir
                printf("\nSaliendo...\n");
                break;
            }
            default: {
                printf("Opción no válida. Por favor, elija una opción válida.\n");
                break;
            }
        }
    } while (opcion != 0);

    return 0;
}
