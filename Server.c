#include "e_shop.h"

int main() {
    srand(time(NULL));
    Shop eshop;
    initialize_shop(&eshop);

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    listen(server_sock, 5);
    printf("Server is running on port %d...\n", PORT);

    for (int i = 0; i < 5; i++) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (fork() == 0) {
            close(server_sock);
            handle_client(client_sock, &eshop);
            exit(0);
        }
        close(client_sock);
    }

    for (int i = 0; i < 5; i++) {
        wait(NULL);
    }

    close(server_sock);
    return 0;
}
