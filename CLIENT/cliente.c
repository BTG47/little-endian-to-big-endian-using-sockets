
#include "cliente.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <unistd.h>

void send_integer(int sock, uint32_t sentbig_endian_to_send);
void send_char(int sock, char char_to_send);
uint32_t transform_integer_to_big_endian(int original_int);


// =========================================================
// Declaracion de variables con el orden de la actividad
// =========================================================
char c1 = 'A';
char c2 = 'B';
int i = 16;
char c3 = 'C';
char c4 = 'D';

int main(int argc, char *argv[])
{
    // communicating with server
    int sock = connection();
    
    // Enviar los dos char
    send_char(sock, c1);
    send_char(sock, c2);

    // Transformar el int a big endian
    uint32_t big_endian = transform_integer_to_big_endian(i);

    // Enviar el int por sockets
    send_integer(sock, big_endian);
    // Enviar los otros dos char

    send_char(sock, c3);
    send_char(sock, c4);
    
    
    close(sock);
    return 0;
}

// Creación de una función que transforma un entero a un UINT32 con formato big-endian
uint32_t transform_integer_to_big_endian(int original_int){

    // Se combierte a un entero de 32 bits sin signo
    uint32_t valor_32bits = (uint32_t)original_int;

    //Punto de espía: representación original en memoria.
    unsigned char *memory = (unsigned char *)&valor_32bits;

    printf(
        "Original bytes: %02X %02X %02X %02X\n",
        memory[0],
        memory[1],
        memory[2],
        memory[3]
    );
    // Se van a separar todos los bytes por separado y se recorreran inversamente. 
    // Esto con el fin de pasar de little endian a big endian.

    // En una máquina Little Endian, el valor 0x00000010
    // se almacena en memoria como:

    //     10 00 00 00
  
    // Para enviarlo en Big Endian se intercambian manualmente las posiciones de sus cuatro bytes.

    // Para ejemplificarlo, si hay un little endian tal que: DD CC BB AA. 
    // Queremos hacer que llegue a ser AA BB CC DD.

    // El primer paso es seleccionar el primer byte del big_endian (AA) con una máscara, 
    // posterior, se mueve hasta el inicio
    uint32_t byte_0 = (valor_32bits & 0x000000FF) << 24;
    // El segundo byte es BB
    uint32_t byte_1 = (valor_32bits & 0x0000FF00) << 8;
    // El tercer byte es CC, ahora se mueve en sentido contrario
    uint32_t byte_2 = (valor_32bits & 0x00FF0000) >> 8;
    uint32_t byte_3 = (valor_32bits & 0xFF000000) >> 24;

    // Finalmente, se vuelven a ensamblar utilizando OR bit a bit    
    return (byte_0 | byte_1 | byte_2 | byte_3);
} 

void send_char(int sock, char char_to_send) {
    // Se usa la forma canonica para enviar datos de sockets. Primero se ingresa la aceptacion de sockets,
    // posterior la referencia al dato a enviar, el tamaño del dato y la flag

    if (send(sock, &char_to_send, sizeof(char_to_send), 0) < 0){
        puts("Send char failed");
        exit(1);
    }

    printf("Char %c sent \n", char_to_send);
}

void send_integer(int sock, uint32_t sentbig_endian_to_send)
{    
    // Enviar los datos siguiendo la forma 
    if(send(sock, (const char*) &sentbig_endian_to_send, sizeof(sentbig_endian_to_send), 0) < 0) {
        puts("Send integer failed");
        exit(1);
    }
    
    // Punto de espía: bytes que realmente serán transmitidos.
    unsigned char *memory = (unsigned char *)&sentbig_endian_to_send;

    printf(
        "Bytes sent: %02X %02X %02X %02X\n",
        memory[0],
        memory[1],
        memory[2],
        memory[3]
    );
}
