#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE_SIZE 1024
#define DELIMITER ","

typedef struct account
{
    char name[50];
    int a;
    int amt;
    char pw[12];
    struct account *next;
} node;

node *create(char n[50], int acno, int amount, char password[12])
{
    node *nw;
    nw = (node *)malloc(sizeof(node));
    strcpy(nw->name, n);
    nw->amt = amount;
    nw->a = acno;
    strcpy(nw->pw, password);
    nw->next = NULL;
    return nw;
}

void traves(node *head)
{
    while (head != NULL)
    {
        printf("%s\t", head->name);
        printf("%d\t", head->a);
        printf("%d\t", head->amt);
        printf("%s\t", head->pw);
        printf("\n");
        head = head->next;
    }
}

node *insertfront(node *ptr, char n[50], int acno, int amt, char password[12])
{
    node *nw;
    if (ptr == NULL)
        ptr = create(n, amt, acno, password);
    else
    {
        nw = create(n, amt, acno, password);
        nw->next = ptr;
        ptr = nw;
    }
    return ptr;
}

node *insertlast(node *head, char n[50], int acno, int amt, char password[12])
{
    node *nw;
    nw = create(n, acno, amt, password);
    while (head->next != NULL)
        head = head->next;
    head->next = nw;
}

int checking_password(node *head, int acc, char password[12])
{
    while (head != NULL)
    {
        if (head->a == acc)
        {
            return strcmp(head->pw, password);
        }
        head = head->next;
    }
    printf("Invalid account number\n");
    return -1;
}

int update_balance(node *head, int acc, int withdrawal_amount)
{
    while (head != NULL)
    {
        if (head->a == acc)
        {
            if (head->amt < withdrawal_amount)
            {
                printf("Insufficient balance\n");
                return -1;
            }
            head->amt -= withdrawal_amount;
            return 0;
        }
        head = head->next;
    }
    printf("Invalid account number\n");
    return -1;
}

int deposit(node *head, int acc, int deposit_amount)
{
    while (head != NULL)
    {
        if (head->a == acc)
        {
            head->amt += deposit_amount;
            return 0;
        }
        head = head->next;
    }
    printf("Invalid account number\n");
    return -1;
}

void write_to_csv(node *head)
{
    FILE *file = fopen("dsa1.csv", "w");
    while (head != NULL)
    {
        fprintf(file, "%s,%d,%d,%s\n", head->name, head->a, head->amt, head->pw);
        head = head->next;
    }
    fclose(file);
}

// Function to delete an account by account number
node *delete_account(node *head, int acc)
{
    node *temp = head;
    node *prev = NULL;
    while (temp != NULL && temp->a != acc)
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
    {
        printf("Account not found\n");
        return head;
    }
    if (prev == NULL)
    {
        head = temp->next;
    }
    else
    {
        prev->next = temp->next;
    }
    free(temp);

    printf("Account deleted successfully\n");
    return head;
}



int main()
{
    FILE *file = fopen("dsa1.csv", "r");
    FILE *fp = fopen("dsa1.csv", "a+");
    node *head = NULL;
    int m, k, acc, flag = 0;
    int acc1 = 0, amt = 0;
    char n1[100];
    char *pos;
    char pw1[12];
    char city[30];
    for (int j = 1; j <= 6; j++)
    {
        if (file == NULL)
        {
            printf("Failed to open file.\n");
            return -1;
        }
        int target_row = j;
        char line[MAX_LINE_SIZE];
        char *fields[4];
        if (fgets(line, sizeof(line), file) == NULL)
        {
            printf("Failed to read row %d.\n", target_row);
            return -1;
        }
        int field_index = 0;
        char *field = strtok(line, DELIMITER);
        while (field != NULL && field_index < 10)
        {
            fields[field_index++] = field;
            field = strtok(NULL, DELIMITER);
        }
        if ((pos = strchr(fields[3], '\n')) != NULL)
            *pos = '\0';
        if (j == 1)
            head = insertfront(head, fields[0], atoi(fields[1]), atoi(fields[2]), fields[3]);
        else
            insertlast(head, fields[0], atoi(fields[1]), atoi(fields[2]), fields[3]);
    }
    traves(head);
    node *ptr = head;

    printf("Enter user type:\n1. Admin\n2. User\n");
    int userType;
    scanf("%d", &userType);

    switch (userType)
    {
    case 1: // Admin
    {
        printf("Enter admin password: ");
        char adminPassword[12];
        scanf("%s", adminPassword);

        // Assuming admin password is "admin123"
        if (strcmp(adminPassword, "admin123") == 0)
        {
            // Admin functionalities
            printf("Admin Login Successful!\n");

            // Admin menu
            printf("Admin Menu:\n1. Create Account\n2. Delete Account\n");
            int adminChoice;
            scanf("%d", &adminChoice);

            switch (adminChoice)
            {
            case 1:
                printf("Enter name:\t");
                scanf("%s", &n1);
                while (ptr->next != NULL)
                {
                    ptr = ptr->next;
                }
                acc1 = ptr->a + 1;
                printf("Enter the initial amount");
                scanf("%d", &amt);
                printf("Enter password consisting of 12 or less than 12 characters");
                scanf("%s", pw1);
                printf("Account number generated %d\n", acc1);
                fprintf(fp, "%s,%d,%d %s\n", n1, acc1, amt, pw1);
                insertlast(ptr, n1, acc1, amt, pw1);
                traves(head);
                break;

            case 2:
                // Delete Account
                {
                    printf("Enter account number to delete:\n");
                    scanf("%d", &acc);
                    head = delete_account(head, acc);
                    traves(head);
                    break;
                }
                break;

            default:
                printf("Invalid choice\n");
                break;
            }
        }
        else
        {
            printf("Invalid admin password\n");
        }
        break;
    }

    case 2: // User
       
        printf("User Menu:\n1. Check Account Details\n2. Withdraw\n3. Deposit\n4. Expense Manager\n5. Office Locator\n");
        printf("Enter your choice: ");
        scanf("%d", &m);

        
        
            switch (m)
            {
            case 1:
            {
                printf("Enter account number\n");
                scanf("%d", &acc);
                printf("Enter password: ");
                scanf("%s", pw1);
                while (head != NULL)
                {
                    if (head->a == acc)
                    {
                        if (checking_password(head, acc, pw1) == 0)
                        {
                            printf("Name of account holder: %s", head->name);
                            printf("\nAccount Number: %d", head->a);
                            printf("\nBalance: %d", head->amt);
                            flag = 1;
                            break;
                        }
                        else
                        {
                            printf("Incorrect password");
                            flag = 1;
                        }
                    }
                    head = head->next;
                }
                if (flag != 1)
                    printf("Account not found");
                break;
            }

            case 2:
                printf("Enter account number\n");
                printf("Enter account number\n");
                scanf("%d", &acc);
                printf("Enter password\n");
                char password[12];
                scanf("%s", password);
                if (checking_password(head, acc, password) == 0)
                {
                    printf("Enter withdrawal amount\n");
                    int withdrawal_amount;
                    scanf("%d", &withdrawal_amount);
                    if (update_balance(head, acc, withdrawal_amount) == 0)
                    {
                        write_to_csv(head);
                        printf("Balance remaining: %d\n", head->amt);
                    }
                }
                else
                {
                    printf("Incorrect password\n");
                }
                traves(head);
                break;

            case 3:
                printf("Enter account number\n");
                scanf("%d", &acc);
                printf("Enter password\n");
                scanf("%s", pw1);
                if (checking_password(head, acc, pw1) == 0)
                {
                    printf("Enter deposit amount\n");
                    int deposit_amount;
                    scanf("%d", &deposit_amount);
                    if (deposit(head, acc, deposit_amount) == 0)
                    {
                        write_to_csv(head);
                    }
                }
                else
                {
                    printf("Incorrect password\n");
                }
                traves(head);
                break;

            case 4:
                printf("Enter account number\n");
                scanf("%d", &acc);
                printf("Enter password");
                scanf("%s", pw1);
                while (head != NULL)
                {
                    if (head->a == acc)
                    {
                        if (checking_password(head, acc, pw1) == 0)
                        {
                            printf("Welcome %s", head->name);
                            // printf("\nBalance: %d",head->amt);
                            printf("\nWe're introducing the 50-30-20 budget rule for effective expense management. It suggests dividing your income into 50% for necessities and taxes, 30% for discretionary spending, and 20% for savings. This simple rule can help ensure financial stability and stress-free money management.\n\n");
                            // printf("\nBalance: %d",1000*head->amt);
                            printf("Based on your account balance, your expenses should be allocated as follows: \nRs.%.2f for NECESSITIES,\nRs.%.2f for DISCRETIONARY SPENDING,\nRs.%.2f for SAVINGS AND INVESTMENTS.", 0.5 * head->amt, 0.3 * head->amt, 0.2 * head->amt);
                            break;
                        }
                        else
                        {
                            printf("Incorrect password");
                        }
                    }
                    head = head->next;
                }
                break;

            case 5:
                printf("Currently, our operations are limited to KARNATAKA state. We are planning to expand our offices further in the future, but for now, this is our operational boundary.\n");
                printf("We'll help you locate our branches in your city\n");
                printf("Please select the city you live in by choosing the corresponding number.");
                printf("1.Bangalore\n2.Mandya\n3.Bidar\n4.Mysore\n5.Mangalore\n6.Hubli\n7.Shimoga\n8.Madikeri\n9.Hampi");
                scanf("%d", &k);

                switch (k)

                {
                case 1:
                    printf("MG Road, Phno. 26675\n");
                    printf("Jayanagar, Phno. 26690\n");
                    printf("Hebbal, Phno. 26671\n");
                    break;

                case 2:
                    printf("Balenhalli, Phno. 26675\n");
                    printf("Alakere, Phno. 26698\n");
                    printf("Ankuppe, Phno. 26612\n");
                    break;

                case 3:
                    printf("Atwar, Phno. 26672\n");
                    printf("Basanthpur, Phno. 266981\n");
                    printf("Nagora, Phno. 26611\n");
                    break;

                case 4:
                    printf("Sindhuvalli, Phno. 26672\n");
                    printf("Anagalli, Phno. 266981\n");
                    printf("Rammanhalli, Phno. 26611\n");
                    break;

                case 5:
                    printf("Ammunje, Phno. 28981\n");
                    printf("Ullal, Phno. 23481\n");
                    printf("Bajpe, Phno. 28451\n");

                case 6:
                    printf("Devargudihal, Phno. 28567\n");
                    printf("Gabbur, Phno. 28982\n");
                    printf("Gangiwal, Phno. 25657\n");

                case 7:
                    printf("Hosanagara, Phno. 28897\n");
                    printf("Shikaripura, Phno. 26782\n");
                    printf("Bhadravathi, Phno. 28007\n");

                case 8:
                    printf("Aruvathoklu, Phno. 21237\n");
                    printf("Badaga, Phno. 20092\n");
                    printf("Bengoor, Phno. 21757\n");

                case 9:
                    printf("Aruvathoklu, Phno. 21237\n");
                    printf("Badaga, Phno. 20092\n");
                    printf("Bengoor, Phno. 21757\n");
                    break;
                }
            case 6: // Delete Account
                printf("You do not have permission to delete accounts.\n");
                break;

            default:
                printf("Invalid choice\n");
                break;
            }
        

        fclose(fp);
        fclose(file);
        return 0;
    }
}
