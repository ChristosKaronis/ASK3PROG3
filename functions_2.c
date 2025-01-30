#include "e_shop.h"

void receive_shop_statistics(int sock) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer) - 1, 0);
    printf("%s", buffer);
}