            #include <stdio.h>
            #include <time.h>
            #include <stdlib.h>
            #include <string.h>

                    struct Account{

                                char name[30];
                                 int IBAN;
                                 int PIN;
                                float balance;
                                  };
                    void sign_up(char *filename_1,struct Account *account);
                    void delete_acc(char *filename_1,char *temp,struct Account *account);
                    int authentication(int IBAN,int PIN,char *filename_1,struct Account *account);
                    void sign_in(struct Account *account,char *filename_1,char *filename_2,char *temp);
                    void transaction(int IBAN_receiver,char *filename_1,struct Account *account,char *filename_2);
                    void ATM(int amount,struct Account *account,int action);
                    void DATA_SAVE(struct Account *account,char *filename_1,char *temp);

                    void main_menu();


                    int main()

                            {
                                srand(time(NULL));
                            main_menu();
                            }

                        void main_menu()
                        {
                         while(1) {

                               struct Account *account;
                               account=malloc(sizeof(struct Account));
                                    if(account==NULL)
                                    {
                                        exit(1);
                                    }
                                int choice_1;
                            printf("Welcome to THMMY bank:\n Press 1 to create account \n Press 2 to sign in \n Press 3 to exit\n");
                            scanf("%d",&choice_1);
                                        if(choice_1==1)
                                        {
                                            sign_up("database.bin",account);
                                            free(account);
                                        }
                                        else if(choice_1==2)
                                        {
                                                int PIN;
                                                int IBAN;
                                            printf("\n Give your 6 digit IBAN: \n");
                                            scanf("%d",&IBAN);
                                            printf("\n Give your PIN: \n");
                                            scanf("%d",&PIN);
                                            if(authentication(IBAN,PIN,"database.bin",account))
                                            {
                                                sign_in(account,"database.bin","transactions.txt","temp.bin");
                                                free(account);

                                            }
                                        else
                                        {
                                             printf("\n Wrong credentials.");
                                             free(account);
                                        }
}
                                    else break;
                            }

                                        return;
                        }



                    int PIN_generator()
                    {
                       return (1000+rand()%9000);



                    }
                    int IBAN_generator()
                    {

                        return(100000+rand()%900000);


                    }





         void sign_up(char *filename_1,struct Account* account)
            {
                    printf("Welcome to account creation. \n Give us your name: \n");
                    getchar();
                    fgets(account->name,25,stdin);
                    (account->name)[strcspn(account->name,"\n")]='\0';
                    printf("Give us your desired balance: \n");
                    scanf("%f",&(account->balance));
                    account->IBAN=IBAN_generator();
                    account->PIN=PIN_generator();
                        printf("\n Your IBAN is %d and your PIN is %d ",account->IBAN,account->PIN);
                        FILE *file=fopen(filename_1,"ab");
                        if(file==NULL)
                        {
                            perror("Unable to open file");
                        }
                    fwrite(account,sizeof(struct Account),1,file);
                            fclose(file);

            }











                int authentication(int IBAN,int PIN,char *filename_1,struct Account *account)
                {
                        FILE *file=fopen(filename_1,"rb");
                        if(file==NULL)
                        {
                            perror("Unable to open file");
                        }


                              while(fread(account,sizeof(struct Account),1,file)==1)
                              {

                                   if(account->PIN==PIN&&account->IBAN==IBAN)
                                    {       fclose(file);
                                            return 1;
                                    }
                              }
                                fclose(file);
                                return 0;
                }

                    void sign_in(struct Account *account,char *filename_1,char *filename_2,char *temp)
                {
                            int choice;
           while(1) {            printf("\nWelcome %s (Your balance is %f $)",account->name,account->balance);
                        printf("\n Press: \n 1 to transfer money \n 2 to withdraw or to deposit \n 3 to delete your account \n 4 to exit\n");
                        scanf("%d",&choice);

                                        if(choice==1)
                                        {
                                             int target_IBAN;
                                            printf("\n Give us your target IBAN: \n");
                                            scanf("%d",&target_IBAN);
                                            transaction(target_IBAN,filename_1,account,filename_2);


                                        }
                                    else if(choice==2)
                                            {
                                                int amount;
                                                int action;
                                            printf("Tell me if you want to withdraw or deposit. \n Press 0 for withdraw \n 1 for deposit \n 2 to go back\n");
                                               scanf("%d",&action);
                                                    if(action==0||action==1)
                                                    {

                                                        printf("Tell me the amount: \n");
                                                        scanf("%d",&amount);
                                                        ATM(amount,account,action);
                                                        DATA_SAVE(account,filename_1,temp);
                                                    }
                                                    else{

                                                            continue;


                                                    }
                                            }
                                    else if(choice==3)
                                    {

                                        delete_acc(filename_1,temp,account);
                                        printf("Account succesfully deleted");
                                        break;
                                    }

                                    else{


                                        break;
                                    }
                                    }
                                        return;
                }
                  void DATA_SAVE(struct Account *account,char* filename_1,char* temp)
                    {
                         struct Account data;


                        FILE *file=fopen(filename_1,"rb");
                        FILE *fp=fopen(temp,"ab");
                                if(file==NULL||fp==NULL){
                                    perror("Error opening file");
                                }


                                        while(fread(&data,sizeof(struct Account),1,file)==1)
                            {
                                if(data.IBAN==account->IBAN)
                                        {
                                            data.balance=account->balance;

                                        }
                            fwrite(&data,sizeof(struct Account),1,fp);
                            }

                            fclose(file);
                            fclose(fp);


              if (remove(filename_1) != 0) {
        perror("Error deleting file");
        return;
    }

    if (rename(temp, filename_1) != 0) {
        perror("Error renaming file");
        return;
    }
             }
void transaction(int IBAN_receiver, char *filename_1, struct Account *account, char *filename_2) {
    float balance;
    int search = 0;
    char *temp="temp.bin";

    struct Account *account_receiver;
    account_receiver = malloc(sizeof(struct Account));
                if (account_receiver == NULL) {
        perror("Memory allocation error");
        exit(1);
    }

    FILE *file = fopen(filename_1, "rb");
            if (file == NULL) {
        perror("Unable to open file");
        exit(1);
    }

    while (fread(account_receiver, sizeof(struct Account), 1, file) == 1) {
        if (account_receiver->IBAN == IBAN_receiver) {
            search = 1;
            break;
        }
    }

    fclose(file);

    if (search) {
        printf("Give the amount of money you want to transfer: \n");
        scanf("%f", &balance);
        if (account->balance - balance >= 0) {
            account->balance -= balance;
            account_receiver->balance += balance;

            FILE *f = fopen(filename_2, "a");
            if (f == NULL) {
                perror("Unable to open file");
                exit(1);
            }

            fprintf(f, "\n %s transferred %.2f $ to %s", account->name, balance, account_receiver->name);
            fclose(f);

            printf("Transaction successful.\n");
        } else {
            printf("Not enough money.\n");
        }
    } else {
        printf("Your target IBAN isn't found.\n");
    }
    DATA_SAVE(account,filename_1,temp);
    DATA_SAVE(account_receiver,filename_1,temp);

    free(account_receiver);
}
                    void ATM(int amount,struct Account* account,int action)
            {
                    if(action==0)
                    {
                            if(amount>0){
                    if(account->balance-amount<0)
                        {
                                printf("\n Not enough money.Try again");
                        }
                        else{
                            account->balance-=amount;
                            printf("\n You have successfully withdrawn %d $",amount);



                        }



                    }else printf("\nWrong  (negative) amount. Please try again");
                        }
                    else if(action==1)
                    {
                        if(amount>0)
                        {

                            account->balance+=amount;
                            printf("\nYou have succesfully deposited %d $",amount);
                        }
                        else printf("\nWrong (negative) amount.Please try again");



                    }
                    else printf("\nWrong choice. Please try again");
            }
  
                void delete_acc(char* filename_1,char* temp_2,struct Account *account)
                {
                    struct Account data;
                        FILE *file=fopen(filename_1,"rb");
                        FILE *fp=fopen(temp_2,"ab");
                            if(file==NULL||fp==NULL)
                            {
                                perror("Unable to open file");
                            }


                           while(fread(&data,sizeof(struct Account),1,file)==1)
                          {

                                  if(strcmp(data.name,account->name)!=0)
                                    {
                                        fwrite(&data,sizeof(struct Account),1,fp);
                                    }
                        }
                            fclose(file);
                            fclose(fp);
                                   if (remove(filename_1) != 0) {
        perror("Error deleting file");
        return;
    }

    if (rename(temp_2, filename_1) != 0) {
        perror("Error renaming file");
        return;
    }
                }





