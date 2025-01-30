#include "e_shop.h"

void receive_shop_statistics(int sock) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer) - 1, 0);
    printf("%s", buffer);
}

int main() {
    srand(time(NULL));

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    for (int i = 0; i < 10; i++) {
        Order order;
        int random_item_index = rand() % 20;
        snprintf(order.item_name, sizeof(order.item_name), "Item_%d", random_item_index);
        order.quantity = 1;
        send(sock, &order, sizeof(Order), 0);
        char response[BUFFER_SIZE];
        recv(sock, response, sizeof(response), 0);
        printf("%s", response);
        sleep(1);
    }

    receive_shop_statistics(sock);
    close(sock);
    return 0;
}
