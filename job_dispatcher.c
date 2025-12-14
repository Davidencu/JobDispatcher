#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdbool.h>
#include<mpi.h>

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
        if (isPrime(n))
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

void anagrams(char *str)
{
    char arr[80] = {0}; //array of frequencies
    char anagram[100] = {0}; //maximum of 100 characters
    int anagram_length=0;
    for(int i=0;i<strlen(str);i++)
    {
        arr[str[i]-'A']++;
    }
    for(int i=0;i<80;i++)
    {
        if(arr[i]!=0)
        {
            anagram[anagram_length++]=i+'A';
        }
    }
    //backtracking
    
}

int main(int argc, char **argv)
{
    return 0;
}