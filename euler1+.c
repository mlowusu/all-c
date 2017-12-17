/*This program is an extension on euler1.c: it should allow you to choose two factors and a limit, then print the sum of all the multiples of those factors below the limt.*/

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>


//test characters are valid (will also catch negatives)
bool isvalid(const char array[]){
    for(int n=0; strlen(array)>n; n++){
        if (isdigit(array[n]) == false){
            printf("you must only input numbers and they must be positive\n");
            return false;
        }
    }
return true;
}

// Convert a string into an integer
int convert(const char array[]) {
    return atoi(array);
}

//convert string to long (so it can be used to check for overflow)
long convertl(const char array[]){
    return atol(array);
}

//check if a number is a multiple of p
bool checkp(int n, int p){
    return (n % p ==0) ? true : false;
}

//check if a number is a multiple of q
bool checkq(int n, int q){
    return (n % q == 0) ? true : false;
}

//sum up all multiples of p or q in list of numbers 1 to r
long total(const char factor1[], const char factor2[], const char limit[]){
    if (isvalid(factor1) == false || isvalid(factor2) == false || isvalid(limit)==false) exit(1);
    if(convert(factor1) != convertl(factor1) || convert(factor2) != convertl(factor2) || convert(limit) != convertl(limit)){
        printf("numbers too big\n");
        exit(1);
    }
    int p = convert(factor1);
    int q = convert(factor2);
    int r = convert (limit);
    int listnum[r-1];
    long sum = 0;
    int t=0;
    while (t <= r-1){
        listnum[t] = t+1;
        if(checkp(listnum[t], p) || checkq(listnum[t], q)) sum = sum + listnum[t] ;
        t++;
    }
    return sum;
}

//--------------------------------------------------------------------
//user interface and tests


//check expected results given correct number of arguments within range
void checkexpected() {
    long n = total("5","3", "1000");
    if(n == 234168) printf("check program with expected arguments succesful\n");
}

//check negative errors
void checkneg(){
    long n = total("-3", "4", "5");
    printf("%ld", n);
}

//check overflow
void checkover(){
    printf("check for overflow:\n");
    long n = total("6666666666666666666", "4", "5");
    printf("%ld", n);
    n = total("2", "4", "500000000000000");
    printf("%ld", n);
}

//check expected results given correct number of arguments within range
void checkchar() {
    printf("check using a letter:\n");
    long n = total("a","4","60");
    assert(n == 0);
}

void test(){
    checkexpected();
    checkover();
    checkneg();
    checkchar();
}

//run program or test it (and prompt user for correct number of arguments if appropriate)
int main(int n, char **argnum) {
    if (n == 1) {
        test();
    }
    else if (n == 4){
        printf("sum of all multiples of %s or %s under %s is %ld\n", argnum[1], argnum[2], argnum[3], total(argnum[1], argnum[2], argnum[3]));
    }
    else {
        printf("input in the form ./euler1+ 3 5 1000\n");
        exit(1);
    }
    return 0;
}

