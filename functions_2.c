#include "e_shop.h"

//Receiving and displaying statistics from server
void receive_shop_statistics(int sock) {
    char buffer[BUFFER_SIZE];
    //Clear buffer before receiving to avoid errors
    memset(buffer, 0, sizeof(buffer));

// Continuously receive data from server until all statistics are received 
//since it didnt print everything at start
    int bytes_received;
    while ((bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0'; 
        printf("%s", buffer); 
        //Clear buffer for next data
        memset(buffer, 0, sizeof(buffer)); 
    }

    if (bytes_received < 0) {
        perror("Error receiving shop statistics");
    }
}