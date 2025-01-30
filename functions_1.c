#include "e_shop.h"

//We initialise the shop with 20 items
void initialize_shop(Shop *shop) {
    //Our items names
    const char *base_names[5] = {"Laptop", "Mouse", "Keyboard", "Monitor", "Headphones"};
    //to make 5 of each
    int name_counter[5] = {0, 0, 0, 0, 0};  

//We put our items to the shop with their name and starting values
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

//When we have to process the info given by orders
int process_order(Shop *shop, Order *order) {

//If quantity is problematic
    if (order->quantity <= 0) {
    printf("Invalid order: %s x%d. Quantity must be greater than 0.\n",
           order->item_name, order->quantity);
    return -1;
}

//Trying to match with one of the shop items
    for (int i = 0; i < 20; i++) {
        printf("Checking item: %s (Stock: %d) against order: %s requested quantity: %d\n",
               shop->items[i].description, shop->items[i].quantity,
               order->item_name, order->quantity);

        //If we find the matching one we give it below
        if (strcmp(shop->items[i].description, order->item_name) == 0) {
            shop->items[i].total_orders++;

            //Check if we have enough of the item to complete the order
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

//If item doeesnt match we decline the order
    shop->declined_orders++;
    printf("Order declined: Item not found.\n");
    return -1;
}


void handle_client(int client_sock, Shop *shop) {
    Order order;

//We have 10 orders for each client 
    for (int i = 0; i < 10; i++) {  

        //Receives order from client
        int bytes_received = recv(client_sock, &order, sizeof(Order), 0);
        if (bytes_received <= 0) {
            printf("Client disconnected or error occurred.\n");
            close(client_sock);
            return;
        }

        //We get the total price here
        int price = process_order(shop, &order);
        char response[BUFFER_SIZE];

        //If everything went smoothly we get a message with the price
        if (price > 0) {
            snprintf(response, BUFFER_SIZE, "Order Successful: %s quantity: %d. Total: $%.2f\n",
                     order.item_name, order.quantity, (float)price);
        } else {
            //order declined
            snprintf(response, BUFFER_SIZE, "Order Declined: %s quantity: %d. Not enough stock.\n",
                     order.item_name, order.quantity);
        }

        //Response to the client
        send(client_sock, response, strlen(response), 0);
        //A little waiting time
        sleep(1);  
    }

//Send shop statistics to client after all orders are finished
    send_shop_statistics(client_sock, shop);

//Close the client socket 
    close(client_sock);
}

//Send shop statistics to client
void send_shop_statistics(int client_sock, Shop *shop) {
    char buffer[BUFFER_SIZE];  

    //General stats from shop
    snprintf(buffer, sizeof(buffer),
             "\nShop Statistics:\nTotal Earnings: $%.2f\nSuccessful Orders: %d\nDeclined Orders: %d\n\n",
             shop->total_earnings, shop->successful_orders, shop->declined_orders);
    send(client_sock, buffer, strlen(buffer), 0);

    //Stats for each item seperetaly
    for (int i = 0; i < 20; i++) {
        //Clear buffer for each item
        memset(buffer, 0, sizeof(buffer));  
        snprintf(buffer, sizeof(buffer),
                 "%s: Total Orders = %d, Price = $%.2f, Sold = %d, Unsuccessful Orders = %d\n",
                 shop->items[i].description, shop->items[i].total_orders, shop->items[i].price,
                 shop->items[i].quantity_sold, shop->items[i].unsuccessful_orders);

        //Send item data to the client
        send(client_sock, buffer, strlen(buffer), 0);
    }
}

