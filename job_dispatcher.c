#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdbool.h>
// #include<mpi.h>

bool isPrime(int n)
{
    if (n <= 1)
        return false;
    for (int i = 2; i < n; i++)
        {
            if (n % i == 0) return false;
        }
    return true;
}

int count_primes(int n)
{
    int tmp=0;
    for(int i=2;i<n;i++)
    {
        if (isPrime(i))
        {
            tmp++;
        }
    }
    return tmp;
}

int count_prime_divisors(int n)
{
    int count = 0;
    for (int i = 1; i <= n; i++) {
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
    char *permutations_set=(char *)malloc((factorial(n) * (n+1) * sizeof(char)) + 1);
    char x[10] = {0};
    permutations_set[0]='\0';
    generate_permutations(no_duplicates_set,permutations_set,x,0,n);
    return permutations_set;
}

int main(int argc, char **argv)
{
    //char *permutations_set = generate_anagrams("sunshine");
    //printf("%s\n",permutations_set);
    //free(permutations_set);
    int primes = count_primes(9);
    printf("%d\n",primes);
    return 0;
}