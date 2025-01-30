#include "e_shop.h"

void initialize_shop(Shop *shop) {
    const char *base_names[5] = {"Laptop", "Mouse", "Keyboard", "Monitor", "Headphones"};
    int name_counter[5] = {0, 0, 0, 0, 0};  

    for (int i = 0; i < 20; i++) {
        int name_index = i % 5;
        name_counter[name_index]++;
        sprintf(shop->items[i].description, "%s_%d", base_names[name_index], name_counter[name_index]);
        shop->items[i].quantity = 10;
        shop->items[i].price = ((rand() % 100) + 1) * 0.5;
        shop->items[i].total_orders = 0;
        shop->items[i].quantity_sold = 0;
        shop->items[i].unsuccessful_orders = 0;
    }
    
    shop->total_earnings = 0.0;
    shop->successful_orders = 0;
    shop->declined_orders = 0;
}

int process_order(Shop *shop, Order *order) {

    if (order->quantity <= 0) {
    printf("Invalid order: %s x%d. Quantity must be greater than 0.\n",
           order->item_name, order->quantity);
    return -1;
}

    for (int i = 0; i < 20; i++) {
        printf("Checking item: %s (Stock: %d) against order: %s requested quantity: %d\n",
               shop->items[i].description, shop->items[i].quantity,
               order->item_name, order->quantity);

        if (strcmp(shop->items[i].description, order->item_name) == 0) {
            shop->items[i].total_orders++;
            if (shop->items[i].quantity >= order->quantity) {
                shop->items[i].quantity -= order->quantity;
                shop->items[i].quantity_sold += order->quantity;
                shop->total_earnings += shop->items[i].price * order->quantity;
                shop->successful_orders++;
                printf("Order successful!\n");
                return shop->items[i].price * order->quantity;
            } else {
                shop->items[i].unsuccessful_orders++;
                shop->declined_orders++;
                printf("Order declined: Not enough stock.\n");
                return -1;
            }
        }
    }
    shop->declined_orders++;
    printf("Order declined: Item not found.\n");
    return -1;
}


void handle_client(int client_sock, Shop *shop) {
    Order order;
    
    for (int i = 0; i < 10; i++) {  // Each client makes 10 orders
        int bytes_received = recv(client_sock, &order, sizeof(Order), 0);
        if (bytes_received <= 0) {
            printf("Client disconnected or error occurred.\n");
            close(client_sock);
            return;
        }

        int price = process_order(shop, &order);
        char response[BUFFER_SIZE];

        if (price > 0) {
            snprintf(response, BUFFER_SIZE, "Order Successful: %s quantity: %d. Total: $%.2f\n",
                     order.item_name, order.quantity, (float)price);
        } else {
            snprintf(response, BUFFER_SIZE, "Order Declined: %s quantity: %d. Not enough stock.\n",
                     order.item_name, order.quantity);
        }

        send(client_sock, response, strlen(response), 0);
        sleep(1);  // Simulating processing time
    }

    // Send shop statistics to client
    send_shop_statistics(client_sock, shop);

    // Close the client socket after communication is complete
    close(client_sock);
}

void send_shop_statistics(int client_sock, Shop *shop) {
    char buffer[BUFFER_SIZE];  // Increased buffer size

    // Send shop summary
    snprintf(buffer, sizeof(buffer),
             "\nShop Statistics:\nTotal Earnings: $%.2f\nSuccessful Orders: %d\nDeclined Orders: %d\n\n",
             shop->total_earnings, shop->successful_orders, shop->declined_orders);
    send(client_sock, buffer, strlen(buffer), 0);

    // Send each item separately to avoid truncation
    for (int i = 0; i < 20; i++) {
        memset(buffer, 0, sizeof(buffer));  // Clear buffer for each item
        snprintf(buffer, sizeof(buffer),
                 "%s: Total Orders = %d, Sold = %d, Unsuccessful Orders = %d\n",
                 shop->items[i].description, shop->items[i].total_orders,
                 shop->items[i].quantity_sold, shop->items[i].unsuccessful_orders);
        send(client_sock, buffer, strlen(buffer), 0);
    }
}

