#include "e_shop.h"

int main() {
    srand(time(NULL));
    Shop eshop;
    initialize_shop(&eshop);

// Here are the servers and clients sockets
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;

//Size of client address structure
    socklen_t client_len = sizeof(client_addr);

//Creating server socket with TCP
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

//Setting up the address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

//Here we bind the server to the address declared
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

//Ignore termination signals from child
    signal(SIGCHLD, SIG_IGN);

//For clients to connect , we set it to accept 5 clients
    listen(server_sock, 5);
    printf("Server is running on port %d...\n", PORT);

//Max 5 clients
    for (int i = 0; i < 5; i++) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);

        //One child for each client
        if (fork() == 0) {
            close(server_sock);
            handle_client(client_sock, &eshop);
            exit(0);
        }
        close(client_sock);
    }

//Waiting for all childs to finish
    for (int i = 0; i < 5; i++) {
        wait(NULL);
    }

//Close our socket
    close(server_sock);
    return 0;
}
