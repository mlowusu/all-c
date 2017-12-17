/* Calculate a grade from a mark. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

// Integer constants representing grades.
enum { G1, G21, G22, G3, Fail, Invalid };

// Check whether a character is a digit.
bool digit(char c) {
if(c<'0' || c>'9') return false; //check ascii so char is number
    return true;
}


// Convert a mark string into an integer.
int convert(int n, const char mark[]) {
    return atoi(mark);
}

//check that it is in range
bool range(int n, const char mark[]){
if(convert(n,mark)>100) return false;
    return true;
}

// Check whether a mark string is properly formatted. i++ means i = i+1
bool valid(int n, const char mark[]) {
if(mark[0]=='0' && n>1) return false;
if(range(n,mark)==false) return false;
for(int i=0;i<n;i++){
    if(digit(mark[i])==false) return false;
}
    return true;
}

// Convert a mark string into a grade.
int grade(int n, const char mark[]) {
    if(valid(n,mark)==false) return Invalid;
    else{
        if(convert(n,mark)>=70) return G1;
        else if(convert(n,mark)>=60) return G21;
        else if(convert(n,mark)>=50) return G22;
        else if(convert(n,mark)>=40) return G3;
        else return Fail;
    }
}
