#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include<mpi.h>

#define WORK_TAG 1
#define FINISH_TAG 2

bool isPrime(long n)
{
    if (n <= 1)
        return false;
    for (long i = 2; i < n; i++)
        {
            if (n % i == 0) return false;
        }
    return true;
}

int count_primes(long n)
{
    int tmp=0;
    for(long i=2;i<n;i++)
    {
        if (isPrime(i))
        {
            tmp++;
        }
    }
    return tmp;
}

int count_prime_divisors(long n)
{
    int count = 0;
    for (long i = 1; i <= n; i++) {
        if (n % i == 0) {
            if (isPrime(i))
                count++;
        }
    }
    return count;
}

void generate_anagram_set(char *str, char *anagram_set)
{
    char arr[256] = {0}; //array of frequencies
    int anagram_length=0;
    for(int i=0;i<strlen(str);i++)
    {
        arr[str[i]-'A']++;
    }
    for(int i=0;i<80;i++)
    {
        if(arr[i]!=0)
        {
            anagram_set[anagram_length++]=i+'A';
        }
    }
    anagram_set[anagram_length]='\0';
}

int valid(char *x,int k)
{
    for(int i=0;i<k;i++)
    {
        if(x[i]==x[k])
            return 0;
    }
    return 1;
}

int solution(int k,int n)
{
    if(k == n-1)
    {
        return 1;
    }
    return 0;
}

int factorial(int n)
{
    int tmp = 1;
    for(int i = 2;i <= n;i++)
    {
        tmp *= i;
    }
    return tmp;
}

void generate_permutations(char *no_duplicates_set,char *permutations_set,char *x,int k,int n)
{
    for(int i = 0;i < n;i++)
    {
        x[k]=no_duplicates_set[i];
        if(valid(x,k))
        {
            if(solution(k,n))
            {
                x[k+1]='\n';
                x[k+2]='\0';
                strcat(permutations_set,x);
            }
            else
            {
                generate_permutations(no_duplicates_set, permutations_set, x, k+1, n);
            }
        }
    }
}

char *generate_anagrams(char *str)
{
    char no_duplicates_set[10] = {0};
    generate_anagram_set(str,no_duplicates_set);
    //printf("%s\n",no_duplicates_set);
    int n = strlen(no_duplicates_set);
    char *permutations_set=(char *)malloc((factorial(8) * (8+1) + 1) * sizeof(char));
    char x[10] = {0};
    permutations_set[0]='\0';
    generate_permutations(no_duplicates_set,permutations_set,x,0,n);
    return permutations_set;
}

int main(int argc, char **argv)
{
    int numtasks, rank;
    char amount_of_time[5];
    char commands_string[50];
    char command_type[6];
    char command[20];
    char string[20];
    char *char_result = (char *)malloc((factorial(8) * (8+1) + 50 + 1) * sizeof(char));
    char dummy[1];
    MPI_Status status;
    //MPI_Request request;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) // the code for the master
    {
        int all_tasks_done=0;
        int worker=-1;

        FILE *fp1 = NULL;
        FILE *fp2 = NULL;
        //char *permutations_set=(char *)malloc((factorial(8) * (8+1) + 1) * sizeof(char));
        if((fp1=fopen("requests_file.txt","r+"))==NULL)
        {
            printf("Error opening the requests file\n");
            MPI_Finalize();
        }
        if((fp2=fopen("log_file.txt","w+"))==NULL)
        {
            printf("Error opening the log file\n");
            MPI_Finalize();
        }

        for(int i=1;i<numtasks;i++)
        {
            fscanf(fp1,"%s",command_type);
            if(strcmp(command_type, "WAIT")==0)
            {
                fscanf(fp1," %s\n",amount_of_time);
                commands_string[0]='\0';
                strcat(commands_string,command_type);
                strcat(commands_string," ");
                strcat(commands_string,amount_of_time);
                long time = strtol(amount_of_time,NULL,10);
                //printf("%s\n",commands_string);
                usleep(time * 250000);
            }
            else
            {
                fscanf(fp1,"%s %s\n",command,string);
                commands_string[0]='\0';
                strcat(commands_string,command_type);
                strcat(commands_string," ");
                strcat(commands_string,command);
                strcat(commands_string," ");
                strcat(commands_string,string);
                //printf("%s\n",commands_string);
                MPI_Send(commands_string,100,MPI_CHAR,i,WORK_TAG,MPI_COMM_WORLD);
            }
        }

        while(!all_tasks_done)
        {
            MPI_Recv(char_result, (factorial(8) * (8+1) + 50 + 1), MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            worker=status.MPI_SOURCE;
            char message[50];
            char log_message[120];
            char client_id[7];
            char client_file[11];
            int end_tasks=0;
            message[0]='\0';
            char *p = strtok(char_result," ");
            strcpy(client_id,p);
            strcat(message,p);
            strcat(message," ");
            p = strtok(NULL," ");
            strcat(message,p);
            strcat(message," ");
            p = strtok(NULL," ");
            strcat(message,p);
            strcat(message," ");
            sprintf(log_message, "Timestamp %.03f: '%s' received from worker %d\n", MPI_Wtime(), message, worker);
            fprintf(fp2,"%s",log_message);
            p = strtok(NULL," ");
            sprintf(client_file,"%s.txt",client_id);
            FILE *cli=NULL;
            if((cli=fopen(client_file,"a+"))==NULL)
            {
                printf("Error opening the client file\n");
                MPI_Finalize();
            }
            fprintf(cli,"%s\n",p);
            if(fclose(cli)!=0)
            {
                printf("Error closing the client file\n");
                MPI_Finalize();
            }
            if(fscanf(fp1, "%s", command_type)==1)
            {
                if(strcmp(command_type, "WAIT")==0)
                {
                    fscanf(fp1," %s\n",amount_of_time);
                    commands_string[0]='\0';
                    strcat(commands_string,command_type);
                    strcat(commands_string," ");
                    strcat(commands_string,amount_of_time);
                    long time = strtol(amount_of_time,NULL,10);
                    //printf("%s\n",commands_string);
                    usleep(time * 250000);
                }
                else
                {
                    fscanf(fp1,"%s %s\n",command,string);
                    commands_string[0]='\0';
                    strcat(commands_string,command_type);
                    strcat(commands_string," ");
                    strcat(commands_string,command);
                    strcat(commands_string," ");
                    strcat(commands_string,string);
                    //printf("%s\n",commands_string);
                    sprintf(log_message, "Timestamp %.03f: '%s' received from the command file\n", MPI_Wtime(), message);
                    MPI_Send(commands_string,100,MPI_CHAR,worker,WORK_TAG,MPI_COMM_WORLD);
                    sprintf(log_message, "Timestamp %.03f: '%s' sent to worker %d\n", MPI_Wtime(), message, worker);
                }
            }
            else
            {
                MPI_Send(dummy,1,MPI_CHAR,worker,FINISH_TAG,MPI_COMM_WORLD);
                end_tasks++;
                if(end_tasks == numtasks - 1)
                {
                    all_tasks_done = 1;
                }
            }
        }

        if(fclose(fp1)!=0)
        {
            printf("Error closing the requests file\n");
            MPI_Finalize();
        }
        if(fclose(fp2)!=0)
        {
            printf("Error closing the log file\n");
            MPI_Finalize();
        }
    }

    else // the code for the workers
    {
        char instruction[20];
        char input_string[20];
        while(1)
        {
            MPI_Recv(commands_string,100,MPI_CHAR,0,MPI_ANY_TAG,MPI_COMM_WORLD, &status);
            printf("%s\n",commands_string);
            if(status.MPI_TAG == WORK_TAG)
            {
                //printf("%s\n",commands_string);
                char *p=strtok(commands_string," ");
                p=strtok(NULL," ");
                strcpy(instruction,p);
                p=strtok(NULL," ");
                strcpy(input_string,p);
                if(strcmp(instruction,"ANAGRAMS")==0)
                {
                    char *permutations=generate_anagrams(input_string);
                    strcpy(char_result,commands_string);
                    strcat(char_result," ");
                    strcat(char_result,permutations);
                    free(permutations);
                }
                else if(strcmp(instruction,"PRIMES")==0)
                {
                    char str_primes[10];
                    long nr=strtol(input_string,NULL,10);
                    int primes=count_primes(nr);
                    sprintf(str_primes,"%d",primes);
                    strcpy(char_result,commands_string);
                    strcat(char_result," ");
                    strcat(char_result,str_primes);
                }
                else if(strcmp(instruction,"PRIMEDIVISORS")==0)
                {
                    char str_primes[10];
                    long nr=strtol(input_string,NULL,10);
                    int primedivisors=count_prime_divisors(nr);
                    sprintf(str_primes,"%d",primedivisors);
                    strcpy(char_result,commands_string);
                    strcat(char_result," ");
                    strcat(char_result,str_primes);
                }
                MPI_Send(char_result, (factorial(8) * (8+1) + 50 + 1), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            }
            else if(status.MPI_TAG == FINISH_TAG)
            {
                free(char_result);
                break;
            }
        }
    }

    MPI_Finalize();
}