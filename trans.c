// prototype
void searchRecord(FILE *fPtr);

// Add in menu
printf("6 - search an account\n");

// Add in switch case
case 6:
    searchRecord(cfPtr);
    break;

// Function definition
void searchRecord(FILE *fPtr)
{
    unsigned int accountNum;
    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number to search (1 - 100): ");
    scanf("%u", &accountNum);

    // Move pointer to correct record
    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    // Read record
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\nAccount Details:\n");
        printf("Account Number : %u\n", client.acctNum);
        printf("Last Name      : %s\n", client.lastName);
        printf("First Name     : %s\n", client.firstName);
        printf("Balance        : %.2lf\n", client.balance);
    }
}