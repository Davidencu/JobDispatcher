#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include<time.h>

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

int main(void)
{
    FILE *fp1 = NULL;
    FILE *fp2 = NULL;
    struct timespec start;
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC,&start);
    char log_message[120];
    char commands_string[50];
    char word1[10];
    char word2[20];
    char word3[20];
    char tmp_commands_string[50];
    char *char_result = (char *)malloc((factorial(8) * (8+1) + 50 + 1) * sizeof(char));
    //char *permutations_set=(char *)malloc((factorial(8) * (8+1) + 1) * sizeof(char));
    if((fp1=fopen("requests_file2.txt","r"))==NULL)
    {
        printf("Error opening the requests file\n");
        exit(-1);
    }
    if((fp2=fopen("log_file.txt","w+"))==NULL)
    {
        printf("Error opening the log file\n");
        exit(-1);
    }
    while(fgets(commands_string,50,fp1)!=NULL)
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
            if(word3[strlen(word3)-1]=='\n')
            {
                word3[strlen(word3)-1]='\0';
            }
        }
    //printf("%s\n",commands_string);
        if(commands_string[strlen(commands_string)-1]=='\n')
        {
            commands_string[strlen(commands_string)-1]='\0';
        }
        if(strcmp(word1, "WAIT")==0)
        {
            long time = strtol(word2,NULL,10);
            usleep(time * 250000);
        }
        else
        {
            clock_gettime(CLOCK_MONOTONIC, &end);
            double time=((double)end.tv_sec + (double)end.tv_nsec / 1000000000.0) - ((double)start.tv_sec + (double)start.tv_nsec / 1000000000.0);
            sprintf(log_message, "Timestamp %.03f: '%s' received from the command file\n", time, commands_string);
            fprintf(fp2,"%s",log_message);
            //printf("%s %s %s\n",client_name,instruction,input_string);
            if(strcmp(word2,"ANAGRAMS")==0)
            {
                char *permutations=generate_anagrams(word3);
                strcpy(char_result,word1);
                strcat(char_result," ");
                strcat(char_result,word2);
                strcat(char_result," ");
                strcat(char_result,word3);
                strcat(char_result," ");
                strcat(char_result,permutations);
                free(permutations);
            }
            else if(strcmp(word2,"PRIMES")==0)
            {
                char str_primes[10];
                long nr=strtol(word3,NULL,10);
                int primes=count_primes(nr);
                sprintf(str_primes,"%d",primes);
                strcpy(char_result,word1);
                strcat(char_result," ");
                strcat(char_result,word2);
                strcat(char_result," ");
                strcat(char_result,word3);
                strcat(char_result," ");
                strcat(char_result,str_primes);
            }
            else if(strcmp(word2,"PRIMEDIVISORS")==0)
            {
                char str_primes[10];
                long nr=strtol(word3,NULL,10);
                int primedivisors=count_prime_divisors(nr);
                sprintf(str_primes,"%d",primedivisors);
                strcpy(char_result,word1);
                strcat(char_result," ");
                strcat(char_result,word2);
                strcat(char_result," ");
                strcat(char_result,word3);
                strcat(char_result," ");
                strcat(char_result,str_primes);
            }
            else {
                clock_gettime(CLOCK_MONOTONIC, &end);
                time=((double)end.tv_sec + (double)end.tv_nsec / 1000000000.0) - ((double)start.tv_sec + (double)start.tv_nsec / 1000000000.0);
                sprintf(log_message, "Timestamp %.03f: not a valid request!!\n", time);
                fprintf(fp2,"%s",log_message);
                continue;
            }
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
            clock_gettime(CLOCK_MONOTONIC, &end);
            time=((double)end.tv_sec + (double)end.tv_nsec / 1000000000.0) - ((double)start.tv_sec + (double)start.tv_nsec / 1000000000.0);
            sprintf(log_message, "Timestamp %.03f: '%s' finished\n", time, message);
            fprintf(fp2,"%s",log_message);
            p = strtok(NULL," ");
            sprintf(client_file,"%s.txt",client_id);
            FILE *cli=NULL;
            if((cli=fopen(client_file,"a+"))==NULL)
            {
                printf("Error opening the client file\n");
                exit(-1);
            }
            fprintf(cli,"%s\n",p);
            if(fclose(cli)!=0)
            {
                printf("Error closing the client file\n");
                exit(-1);
            }
        }
    }
    free(char_result);
    if(fclose(fp1)!=0)
    {
        printf("Error closing the requests file\n");
        exit(-1);
    }
    if(fclose(fp2)!=0)
    {
        printf("Error closing the log file\n");
        exit(-1);
    }
    return 0;
}