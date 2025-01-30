#include "e_shop.h"

//Receiving and displaying statistics from server
void receive_shop_statistics(int sock) {
    char buffer[BUFFER_SIZE];
    //Clear buffer before receiving to avoid errors
    memset(buffer, 0, sizeof(buffer));
    //receive data from server
    recv(sock, buffer, sizeof(buffer) - 1, 0);
    //Print them
    printf("%s", buffer);
}