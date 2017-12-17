/*The program should find the sum of all the multiples of 3 and 5 that are less than 1000. The idea for this project came from ProjectEuler.net*/ 

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>

//check if a number is a multiple of 3
bool check3(int n){
return (n % 3 ==0) ? true : false;
}

//check if a number is a multiple of 5
bool check5(int n){
return (n % 5 == 0) ? true : false;
}

//sum up all multiples of 3 or 5 in list of numbers 1 - 1000
int total(){
int listnum[999];
int sum = 0;
int t=0;
while (t <= 999){
listnum[t] = t+1;
if(check3(listnum[t]) || check5(listnum[t])) sum = sum + listnum[t] ;
t++;//note: t=t+1 so that loops are counted and program can end loop after 1000 repetitions
}
return sum;
}

//print total
int main(){
printf("sum of all multiples of 3 or 5 under 1000 is %d\n", total());//note: calls total function which returns sum(an int)
return 0;
}
 
