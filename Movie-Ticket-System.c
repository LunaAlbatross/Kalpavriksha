#include <stdio.h>
#include <string.h>

// Function to calculate the final bill after applying discount
float calculateFinalBill(float subTotal, float discount) {
    return subTotal - (subTotal * discount);
}

int main(){
    const int STANDARD_PRICE = 150;
    const int PREMIUM_PRICE = 250;
    const int RECLINER_PRICE = 400;

    char customerName[50];
    int numberOfTickets, ticketCategory;
    char Category[20];
    int ticketPrice;
    float subTotal = 0.0;
    float discount = 0.0;
    float finalBill;

    scanf("%49s", customerName);
    scanf("%d", &numberOfTickets);
    if (numberOfTickets < 1 || numberOfTickets > 20) {
        printf("Invalid ticket count.\n");
        return 1;
    }
    scanf("%d", &ticketCategory);
    switch(ticketCategory){
        case 1:
            ticketPrice = STANDARD_PRICE;
            strcpy(Category, "Standard");
            break;
        case 2:
            ticketPrice = PREMIUM_PRICE;
            strcpy(Category, "Premium");
            break;
        case 3:
            ticketPrice = RECLINER_PRICE;
            strcpy(Category, "Recliner");
            break;
        default:
            printf("Invalid ticket category.\n");
            return 1;
    }
    if (numberOfTickets >=5) {
        discount = 0.10; // 10% discount for 5 or more tickets
    }

    subTotal = numberOfTickets * ticketPrice;
    finalBill = calculateFinalBill(subTotal, discount);

    printf("Customer: %s\n", customerName);
    printf("Ticket Category: %s\n", Category);
    printf("Tickets: %d\n", numberOfTickets);
    printf("Subtotal: %.2f\n", subTotal);
    printf("Discount: %.2f\n", subTotal * discount);
    printf("Final Bill: %.2f\n", finalBill);

    return 0;
} 