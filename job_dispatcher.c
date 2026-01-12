#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include<mpi.h>

#define WORK_TAG 1
#define FINISH_TAG 2

char permutation[9];

bool isPrime(int n) { //efficient function to check if a number is prime or not
    if (n <= 1) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    if (n % 2 == 0) {
        return false;
    }

    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            return false; // Found a divisor, not prime.
        }
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

void generate_anagram_set(int n, int *anagram_set)
{
    for(int i=0;i<n;i++)
    {
        anagram_set[i]=i;
    }
}

int valid(char *str,int *x,int k)
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

void generate_permutations(int *anagram_set,char *str,char *permutations_set,int *x,int k,int n)
{
    for(int i = 0;i < n;i++)
    {
        x[k]=anagram_set[i];
        if(valid(str,x,k))
        {
            if(solution(k,n))
            {
                for(int i=0;i<n;i++)
                {
                    permutation[i]=str[x[i]];
                }
                permutation[n]='\0';
                strcat(permutations_set,permutation);
                strcat(permutations_set,"\n");
            }
            else
            {
                generate_permutations(anagram_set, str, permutations_set, x, k+1, n);
            }
        }
    }
}

char *generate_anagrams(char *str)
{
    int anagram_set[8] = {0};
    int n = strlen(str);
    generate_anagram_set(8,anagram_set);
    //printf("%s\n",no_duplicates_set);
    char *permutations_set=(char *)malloc((factorial(8) * (8+1) + 1) * sizeof(char));
    int x[10] = {0};
    permutations_set[0]='\0';
    generate_permutations(anagram_set,str,permutations_set,x,0,n);
    return permutations_set;
}

void write_to_client_file(char *char_result,char *dummy,FILE *fp2,int numtasks,int worker)
{
    char log_message[120];
    char message[50];
    char client_id[7];
    char client_file[11];
    char *p = strtok(char_result," ");
    strcpy(client_id,p);
    strcpy(message,p);
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
        for(int i=1;i<=numtasks;i++)
        {
            MPI_Send(dummy,1,MPI_CHAR,i,FINISH_TAG,MPI_COMM_WORLD);
        }
        MPI_Finalize();
    }
    fprintf(cli,"%s\n",p);
    if(fclose(cli)!=0)
    {
        printf("Error closing the client file\n");
        for(int i=1;i<=numtasks;i++)
        {
            MPI_Send(dummy,1,MPI_CHAR,i,FINISH_TAG,MPI_COMM_WORLD);
        }
        MPI_Finalize();
    }
}

int main(int argc, char **argv)
{
    int numtasks, rank;
    char tmp_commands_string[50]; //a temporary command string
    char commands_string[50];
    char word1[10];
    char word2[20];
    char word3[20];
    char *char_result = (char *)malloc((factorial(8) * (8+1) + 50 + 1) * sizeof(char));
    char dummy[1];
    MPI_Status status;
    //MPI_Request request;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) // the code for the master
    {
        int active_workers=0;
        int worker=-1;
        char log_message[120];

        FILE *fp1 = NULL;
        FILE *fp2 = NULL;
        //char *permutations_set=(char *)malloc((factorial(8) * (8+1) + 1) * sizeof(char));
        if((fp1=fopen("requests_file2.txt","r"))==NULL)
        {
            printf("Error opening the requests file\n");
            for(int i=1;i<=numtasks;i++)
            {
                MPI_Send(dummy,1,MPI_CHAR,i,FINISH_TAG,MPI_COMM_WORLD);
            }
            MPI_Finalize();
        }
        if((fp2=fopen("log_file.txt","w+"))==NULL)
        {
            printf("Error opening the log file\n");
            for(int i=1;i<=numtasks;i++)
            {
                MPI_Send(dummy,1,MPI_CHAR,i,FINISH_TAG,MPI_COMM_WORLD);
            }
            MPI_Finalize();
        }

        for(;active_workers<numtasks-1;)
        {
            fgets(commands_string,50,fp1);

            int valid_string=0;
            while(!valid_string) //check whether the line is a valid command to send to a worker or not
            {
                strcpy(tmp_commands_string,commands_string);
                //printf("%s\n",tmp_commands_string);
                char *p = strtok(tmp_commands_string, " ");
                if(p != NULL)
                {
                    strcpy(word1,p);
                }
                p = strtok(NULL, " ");
                if(p != NULL)
                {
                    strcpy(word2,p);
                }
                p = strtok(NULL," ");
                if(p != NULL)
                {
                    strcpy(word3, p);
                }
                if((word1[0]=='C' && word1[1]=='L' && word1[2]=='I') &&
                 (strcmp(word2,"PRIMES")==0 || strcmp(word2,"PRIMEDIVISORS")==0 || strcmp(word2,"ANAGRAMS")==0))
                {
                    valid_string = 1;
                }
                else if(strcmp(word1,"WAIT") == 0)
                {
                    long time = strtol(word2,NULL,10);
                    usleep(time * 250000);
                    if(fgets(commands_string,50,fp1)!=NULL)
                    {
                        continue;
                    }
                    else
                    {
                        MPI_Send(dummy,1,MPI_CHAR,worker,FINISH_TAG,MPI_COMM_WORLD);
                        active_workers--;
                    }
                }
                else {
                    sprintf(log_message, "Timestamp %.03f: Not a valid request!!\n", MPI_Wtime());
                    fprintf(fp2,"%s",log_message);
                    if(fgets(commands_string,50,fp1)!=NULL)
                    {
                        continue;
                    }
                    else
                    {
                        MPI_Send(dummy,1,MPI_CHAR,worker,FINISH_TAG,MPI_COMM_WORLD);
                        active_workers--;
                    }
                }
            }
            if(commands_string[strlen(commands_string)-1]=='\n')
            {
                commands_string[strlen(commands_string)-1]='\0';
            }
            sprintf(log_message, "Timestamp %.03f: '%s' received from the command file\n", MPI_Wtime(), commands_string);
            fprintf(fp2,"%s",log_message);
            //printf("%s\n",commands_string);
            active_workers++;
            MPI_Send(commands_string,50,MPI_CHAR,active_workers,WORK_TAG,MPI_COMM_WORLD);
            sprintf(log_message, "Timestamp %.03f: '%s' sent to worker %d\n", MPI_Wtime(), commands_string, active_workers);
            fprintf(fp2,"%s",log_message);
        }

        while(active_workers)
        {
            MPI_Recv(char_result, (factorial(8) * (8+1) + 50 + 1), MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            worker=status.MPI_SOURCE;
            write_to_client_file(char_result,dummy,fp2,numtasks,worker);
            if(fgets(commands_string,50,fp1)!=NULL)
            {
                int valid_string=0;
                while(!valid_string) //check whether the line is a valid command to send to a worker or not
                {
                    strcpy(tmp_commands_string,commands_string);
                    //printf("%s\n",tmp_commands_string);
                    char *p = strtok(tmp_commands_string, " ");
                    if(p != NULL)
                    {
                        strcpy(word1,p);
                    }
                    p = strtok(NULL, " ");
                    if(p != NULL)
                    {
                        strcpy(word2,p);
                    }
                    p = strtok(NULL," ");
                    if(p != NULL)
                    {
                        strcpy(word3, p);
                    }
                    if((word1[0]=='C' && word1[1]=='L' && word1[2]=='I') &&
                     (strcmp(word2,"PRIMES")==0 || strcmp(word2,"PRIMEDIVISORS")==0 || strcmp(word2,"ANAGRAMS")==0))
                    {
                        valid_string = 1;
                    }
                    else if(strcmp(word1,"WAIT") == 0)
                    {
                        long time = strtol(word2,NULL,10);
                        usleep(time * 250000);
                        if(fgets(commands_string,50,fp1)!=NULL)
                        {
                            continue;
                        }
                        else
                        {
                            MPI_Send(dummy,1,MPI_CHAR,worker,FINISH_TAG,MPI_COMM_WORLD);
                            active_workers--;
                        }
                    }
                    else {
                        sprintf(log_message, "Timestamp %.03f: Not a valid request!!\n", MPI_Wtime());
                        fprintf(fp2,"%s",log_message);
                        if(fgets(commands_string,50,fp1)!=NULL)
                        {
                            continue;
                        }
                        else
                        {
                            MPI_Send(dummy,1,MPI_CHAR,worker,FINISH_TAG,MPI_COMM_WORLD);
                            active_workers--;
                            break;
                        }
                    }
                }
                //printf("%s\n",commands_string);
                if(active_workers == numtasks-1)
                {
                    if(commands_string[strlen(commands_string)-1]=='\n')
                    {
                        commands_string[strlen(commands_string)-1]='\0';
                    }
                    sprintf(log_message, "Timestamp %.03f: '%s' received from the command file\n", MPI_Wtime(), commands_string);
                    fprintf(fp2,"%s",log_message);
                    MPI_Send(commands_string,50,MPI_CHAR,worker,WORK_TAG,MPI_COMM_WORLD);
                    sprintf(log_message, "Timestamp %.03f: '%s' sent to worker %d\n", MPI_Wtime(), commands_string, worker);
                    fprintf(fp2,"%s",log_message);
                }
            }
            else
            {
                MPI_Send(dummy,1,MPI_CHAR,worker,FINISH_TAG,MPI_COMM_WORLD);
                active_workers--;
            }
        }

        free(char_result);

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
        MPI_Finalize();
    }

    else // the code for the workers
    {
        char client_name[7];
        char instruction[20];
        char input_string[20];
        while(1)
        {
            MPI_Recv(commands_string,50,MPI_CHAR,0,MPI_ANY_TAG,MPI_COMM_WORLD, &status);
            if(status.MPI_TAG == WORK_TAG)
            {
                //printf("%s\n",commands_string);
                char *p=strtok(commands_string," ");
                strcpy(client_name,p);
                p=strtok(NULL," ");
                strcpy(instruction,p);
                p=strtok(NULL," ");
                strcpy(input_string,p);
                //printf("%s %s %s\n",client_name,instruction,input_string);
                if(strcmp(instruction,"ANAGRAMS")==0)
                {
                    char *permutations=generate_anagrams(input_string);
                    strcpy(char_result,client_name);
                    strcat(char_result," ");
                    strcat(char_result,instruction);
                    strcat(char_result," ");
                    strcat(char_result,input_string);
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
                    strcpy(char_result,client_name);
                    strcat(char_result," ");
                    strcat(char_result,instruction);
                    strcat(char_result," ");
                    strcat(char_result,input_string);
                    strcat(char_result," ");
                    strcat(char_result,str_primes);
                }
                else if(strcmp(instruction,"PRIMEDIVISORS")==0)
                {
                    char str_primes[10];
                    long nr=strtol(input_string,NULL,10);
                    int primedivisors=count_prime_divisors(nr);
                    sprintf(str_primes,"%d",primedivisors);
                    strcpy(char_result,client_name);
                    strcat(char_result," ");
                    strcat(char_result,instruction);
                    strcat(char_result," ");
                    strcat(char_result,input_string);
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
        MPI_Finalize();
    }
}