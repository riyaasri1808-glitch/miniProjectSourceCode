#include <stdio.h>
#include <stdlib.h>

// clientData structure definition
struct clientData
{
    unsigned int acctNum; // account number
    char lastName[15];    // account last name
    char firstName[10];   // account first name
    double balance;       // account balance
};

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void searchRecord(FILE *fPtr); // New feature

int main(int argc, char *argv[])
{
    FILE *cfPtr;         // credit.dat file pointer
    unsigned int choice; // user's choice

    // open file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("%s: File could not be opened.\n", argv[0]);
        exit(-1);
    }

    // menu loop
    while ((choice = enterChoice()) != 6)
    {
        switch (choice)
        {
        // create text file
        case 1:
            textFile(cfPtr);
            break;

        // update record
        case 2:
            updateRecord(cfPtr);
            break;

        // create record
        case 3:
            newRecord(cfPtr);
            break;

        // delete record
        case 4:
            deleteRecord(cfPtr);
            break;

        // search account
        case 5:
            searchRecord(cfPtr);
            break;

        default:
            puts("Incorrect choice");
            break;
        }
    }

    fclose(cfPtr);
    return 0;
}

// create formatted text file
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    int result;

    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
    }
    else
    {
        rewind(readPtr);

        fprintf(writePtr,
                "%-6s%-16s%-11s%10s\n",
                "Acct",
                "Last Name",
                "First Name",
                "Balance");

        while (!feof(readPtr))
        {
            result = fread(&client,
                            sizeof(struct clientData),
                            1,
                            readPtr);

            if (result != 0 && client.acctNum != 0)
            {
                fprintf(writePtr,
                        "%-6d%-16s%-11s%10.2f\n",
                        client.acctNum,
                        client.lastName,
                        client.firstName,
                        client.balance);
            }
        }

        fclose(writePtr);
        printf("accounts.txt created successfully.\n");
    }
}

// update balance
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account to update (1 - 100): ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account #%u has no information.\n",
               account);
    }
    else
    {
        printf("\n%-6u%-16s%-11s%10.2f\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);

        printf("Enter charge (+) or payment (-): ");
        scanf("%lf", &transaction);

        client.balance += transaction;

        printf("Updated Balance: %.2f\n",
               client.balance);

        fseek(fPtr,
              -sizeof(struct clientData),
              SEEK_CUR);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);
    }
}

// delete record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete (1 - 100): ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) *
              sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account %u does not exist.\n",
               accountNum);
    }
    else
    {
        fseek(fPtr,
              (accountNum - 1) *
                  sizeof(struct clientData),
              SEEK_SET);

        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account deleted successfully.\n");
    }
}

// create record
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number (1 - 100): ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) *
              sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum != 0)
    {
        printf("Account #%u already exists.\n",
               client.acctNum);
    }
    else
    {
        printf("Enter lastname firstname balance:\n");

        scanf("%14s%9s%lf",
              client.lastName,
              client.firstName,
              &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (client.acctNum - 1) *
                  sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account added successfully.\n");
    }
}

// NEW FEATURE - Search Account
void searchRecord(FILE *fPtr)
{
    unsigned int accountNum;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number to search (1 - 100): ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) *
              sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\nAccount Details\n");
        printf("Account Number : %u\n",
               client.acctNum);

        printf("Last Name      : %s\n",
               client.lastName);

        printf("First Name     : %s\n",
               client.firstName);

        printf("Balance        : %.2lf\n",
               client.balance);
    }
}

// menu
unsigned int enterChoice(void)
{
    unsigned int menuChoice;

    printf("\nEnter your choice\n"
           "1 - store accounts in text file\n"
           "2 - update an account\n"
           "3 - add a new account\n"
           "4 - delete an account\n"
           "5 - search account\n"
           "6 - end program\n? ");

    scanf("%u", &menuChoice);

    return menuChoice;
}