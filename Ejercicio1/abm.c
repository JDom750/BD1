#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cliente{
    int posicion;
    char apellido[20];
    char nombre[20];
    int id;
} cliente;

#define MAX_CLIENTES 100

void altaCliente() {
    FILE* fh_co;
    int cant, n_cliente;
    printf("Ingrese cuántos clientes desea cargar al sistema: ");
    scanf("%d", &cant);

    fh_co = fopen("clientes.txt", "w");
    if (fh_co == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    for (int j = 0; j < cant; j++) {
        cliente c;

        printf("\nIngrese el nombre del cliente: ");
        scanf("%s", c.nombre);

        printf("\nIngrese el apellido del cliente: ");
        scanf("%s", c.apellido);

        printf("Ingrese el número de cliente (nro entero de 4 dígitos mayor que 0): ");
        scanf("%d", &c.id);

        c.posicion = j + 1; // Start positions from 1
        fwrite(&c, sizeof(cliente), 1, fh_co);
    }
    fclose(fh_co);
}

void buscarCliente(){
    FILE *fh_co;
    int posicion_buscada;

    printf("Ingrese la posicion del cliente que desea buscar: ");
    scanf("%d", &posicion_buscada);

    fh_co = fopen("clientes.txt", "r");
    if (fh_co == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    cliente c;
    //fread devuelve la cantidad de elementos leidos con exito. en este caso si devuleve 1 es porque leyo ese regsitro correctamente. tambien la funcion mueve al cursor del archivo (puntero de archivo)
    while (fread(&c, sizeof(cliente), 1, fh_co) == 1) {
        if (c.posicion == posicion_buscada) {
            printf("Cliente encontrado:\n");
            printf("Posicion: %d\n", c.posicion);
            printf("Nombre: %s\n", c.nombre);
            printf("Apellido: %s\n", c.apellido);
            printf("ID: %d\n", c.id);
            break;  // Termina el bucle una vez que se encuentra el cliente
        }
    }

    if (feof(fh_co)) {
        printf("Cliente no encontrado.\n");
    }

    fclose(fh_co);

}

void modificarCliente() {
    FILE* fh_co;
    int posicion_buscada;
    int opc, n;
    char cad[20];

    printf("Ingrese la posición del cliente que desea modificar: ");
    scanf("%d", &posicion_buscada);

    fh_co = fopen("clientes.txt", "r+");
    if (fh_co == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    cliente c;
    int encontrado = 0;

    while (fread(&c, sizeof(cliente), 1, fh_co) == 1) {
        if (c.posicion == posicion_buscada) {
            printf("Cliente encontrado:\n");
            printf("Posición: %d\n", c.posicion);
            printf("Nombre: %s\n", c.nombre);
            printf("Apellido: %s\n", c.apellido);
            printf("ID: %d\n", c.id);

            printf("\nIngrese el aspecto que desea modificar (1-Nombre, 2-Apellido, 3-ID): ");
            scanf("%d", &opc);

            switch (opc) {
                case 1:
                    printf("\nIngrese el nuevo nombre: ");
                    scanf("%s", cad);
                    strcpy(c.nombre, cad);
                    break;
                case 2:
                    printf("\nIngrese el nuevo apellido: ");
                    scanf("%s", cad);
                    strcpy(c.apellido, cad);
                    break;
                case 3:
                    printf("\nIngrese el nuevo ID: ");
                    scanf("%d", &n);
                    c.id = n;
                    break;
            }

            fseek(fh_co, -sizeof(cliente), SEEK_CUR);
            fwrite(&c, sizeof(cliente), 1, fh_co);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Cliente no encontrado.\n");
    }

    fclose(fh_co);
}

 void bajaCliente() {
    FILE* fh_co;
    int posicion_buscada;

    printf("Ingrese la posición del cliente que desea dar de baja: ");
    scanf("%d", &posicion_buscada);

    fh_co = fopen("clientes.txt", "r+");
    if (fh_co == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    cliente c;
    int encontrado = 0;

    while (fread(&c, sizeof(cliente), 1, fh_co) == 1) {
        if (c.posicion == posicion_buscada) {
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Cliente no encontrado.\n");
        fclose(fh_co);
        return;
    }

    // Create a temporary file to write all clients except the one to delete
    FILE* temp_fh = fopen("temp.txt", "w");
    if (temp_fh == NULL) {
        printf("No se pudo abrir el archivo temporal.\n");
        fclose(fh_co);
        return;
    }

    rewind(fh_co); // Move to the beginning of the file

    while (fread(&c, sizeof(cliente), 1, fh_co) == 1) {
        if (c.posicion != posicion_buscada) {
            if (c.posicion > posicion_buscada){
                c.posicion = c.posicion-1;
            }
            fwrite(&c, sizeof(cliente), 1, temp_fh);
        }
    }

    fclose(fh_co);
    fclose(temp_fh);

    // Remove the old file and rename the temporary file
    remove("clientes.txt");
    rename("temp.txt", "clientes.txt");

    printf("Cliente eliminado exitosamente.\n");
}

void mostrarClientes() {
    FILE* fh_co;

    fh_co = fopen("clientes.txt", "r");
    if (fh_co == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    cliente c;

    printf("Lista de Clientes:\n");
    printf("Posicion\tNombre\tApellido\tID\n");

    while (fread(&c, sizeof(cliente), 1, fh_co) == 1) {
        printf("%d\t\t%s\t%s\t%d\n", c.posicion, c.nombre, c.apellido, c.id);
    }

    fclose(fh_co);
}


int main(){
    int opcion;
    do {
        printf("\n");
        printf("1.Alta de Clientes (borrara el actual si ya existe uno creado)\n");
        printf("2.Buscar cliente por posicion\n");
        printf("3.Modificar Datos de un cliente\n");
        printf("4. Dar baja de cliente\n");
        printf("5.Mostrar Clientes\n");
        printf("0. Salir");
        printf("\nSeleccione una opcion: ");
        scanf("%d",&opcion); // FALTA VALIDAR

        switch (opcion)
        {
        case 1:
            altaCliente();
            break;
        case 2:
            buscarCliente();
            break;
        case 3:
            modificarCliente();
            break;
        case 4:
            bajaCliente();
            break;
        case 0:
            printf("\nSaliendo...\n");
            break;
        case 5:
            mostrarClientes();
            break;

        default:

            break;
        }

    } while (opcion !=0);

    return 0;

}