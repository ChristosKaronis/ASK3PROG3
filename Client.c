#include "e_shop.h"

int main() {
    srand(time(NULL));

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    const char *base_names[5] = {"Laptop", "Mouse", "Keyboard", "Monitor", "Headphones"};

    for (int i = 0; i < 10; i++) {
        Order order;
        int random_item_index = rand() % 20;
        
        // Generate a valid item name that matches the server's inventory format
        snprintf(order.item_name, sizeof(order.item_name), "%s_%d", 
                 base_names[random_item_index % 5], (random_item_index / 5) + 1);

        // Ensure the order quantity is always at least 1
        order.quantity = (rand() % 3) + 1;  // Orders between 1-3 items

        // Debugging output to check the order being placed
        printf("Placing Order: %s x%d\n", order.item_name, order.quantity);

        // Send order to server
        send(sock, &order, sizeof(Order), 0);

        // Receive response from server
        char response[BUFFER_SIZE];
        recv(sock, response, sizeof(response), 0);
        printf("%s", response);

        sleep(1);  // Simulating delay between orders
    }

    // Receive final shop statistics from server
    receive_shop_statistics(sock);

    close(sock);
    return 0;
}

