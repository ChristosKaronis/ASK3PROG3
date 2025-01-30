#ifndef E_SHOP_H
#define E_SHOP_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>

#define PORT 8080
#define BUFFER_SIZE 100


typedef struct {
    char description[50];
    int quantity;
    float price;
    int total_orders;
    int quantity_sold;
    int unsuccessful_orders;
} ShopItem;


typedef struct {
    ShopItem items[20];
    float total_earnings;
    int successful_orders;
    int declined_orders;
} Shop;


typedef struct {
    char item_name[50];
    int quantity;
} Order;


void initialize_shop(Shop *shop);
int process_order(Shop *shop, Order *order);
void handle_client(int client_sock, Shop *shop);
void receive_shop_statistics(int sock);
void send_shop_statistics(int client_sock, Shop *shop);

#endif 
