void displayAll(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    rewind(fPtr);

    printf("\n%-6s %-16s %-11s %10s\n",
           "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-6u %-16s %-11s %10.2lf\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}