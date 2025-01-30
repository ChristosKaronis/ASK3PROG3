#include "e_shop.h"

int main() {

//Random number generator
    srand(time(NULL));

//Our socket for communication
    int sock = socket(AF_INET, SOCK_STREAM, 0);

//Struct to store server addr details
    struct sockaddr_in server_addr;

//Server addr properties
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

//Trying to connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

//Give base items names in order to match names from shop
    const char *base_names[5] = {"Laptop", "Mouse", "Keyboard", "Monitor", "Headphones"};

//Creating 10 random orders
    for (int i = 0; i < 10; i++) {
        Order order;
        int random_item_index = rand() % 20;
        
        //Generating a item name to match the servers inventory 
        snprintf(order.item_name, sizeof(order.item_name), "%s_%d", 
                 base_names[random_item_index % 5], (random_item_index / 5) + 1);

        //Quantity is always at least 1 
        order.quantity = (rand() % 3) + 1;  // Orders between 1-3 items

        //In order to check the order being placed
        printf("Placing Order: %s x%d\n", order.item_name, order.quantity);

        //Send the order to the server
        send(sock, &order, sizeof(Order), 0);

        //Receive response from server
        char response[BUFFER_SIZE];
        recv(sock, response, sizeof(response), 0);
        printf("%s", response);

        //A delay between orders
        sleep(1);  
    }

    //Receive final shop statistics from server
    receive_shop_statistics(sock);

    //Close socket
    close(sock);
    return 0;
}

