/*takes an input of a date and outputs day of the week. note: it will extrapolate the standard gregorian calender back ignoring the fact that the calender has changed over time.*/

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>

void errmsg(int x[4]);

enum Day { Monday = 0, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };
enum month { Jan = 1, Feb, Mar, Apr, May, June, July, Aug, Sept, Oct, Nov, Dec };


// Convert string to integer
int convert(const char array[]){
  return atoi(array);
}

//check if given year is leap(lea years are not actually every 4 years necasarily)
bool isleap(int c){
  if((c%4 == 0) && ((c%100 != 0) || (c%400==0)))return true;
  return  false;
}

//check inputs are valid dates and mark which validity tests fail
bool isvalid(const char days[], const char months[], const char years[]){
  int a = convert(days);
  int b = convert(months);
  int c = convert(years);
  int errors[4] = {false, false, false, false};
  if(strlen(days) !=2 || strlen(months) !=2 || strlen(years) !=4) errors[0] = true;
  else if(isdigit(days[0]) == false || isdigit(days[1]) == false || isdigit(months[0]) == false || isdigit(months[1]) == false ||
         isdigit(years[0]) == false || isdigit(years[1]) == false || isdigit(years[2]) == false || isdigit(years[3]) == false) errors[1] = true;
  else if(a==0 || b==0 || c==0) errors[2] = true;
  if((b > Dec) || (a > 31) || ((a>30) && ((b==Apr) || (b==June) || (b==Sept) || (b==Nov))) || ((isleap(c) == true) && (a>29) && (b==Feb)) ||
    ((isleap(c) == false) && (a>28) && (b==Feb)))errors[3] = true;
  errmsg(errors);
  if(errors[0] == true || errors[1] == true || errors[2] == true || errors[3] == true)return false;
  return true;
}

    
//calculate day
int day(const char days[], const char months[], const char years[]){
  int a = convert(days);
  int b = convert(months);
  int c = convert(years);
  int remainder;
  switch(b){//adjust remainder for months that year by setting to cumulative days %7 up to previous month
    case Jan:
    case Oct: remainder = 0;break;
    case Feb:
    case Mar:
    case Nov: remainder = 3;break;
    case Apr:
    case July: remainder = 6;break;
    case May: remainder = 1;break;
    case June: remainder = 4;break;
    case Aug: remainder = 2;break;
    case Sept:
    case Dec: remainder = 5;break;
  }
  if((isleap(c) == true) && (b>Feb))remainder++;//add a day if gone past leap day in current day

  remainder = remainder + ((c-1)%7);//adjust for years (assuming common) then for leap years
  for(int i = 1; i< c; i++){
    if(isleap(i) == true) remainder++;//leap years ( not actually every 4 years)
  }
  remainder = (remainder + (a-1))%7;
  return remainder;
}


//----------------------------- testing and user interface


//test day is correct for valid input
void daynumber(){
  assert(day("31","10","2017") == Tuesday);
  assert(day("01","08","1999") == Sunday);
  assert(day("01","01","0001") == Monday);
}


//test isvalid functiom 
void validity(){
  assert(isvalid("01","1","2000") == false);
  assert(isvalid("ab","12","3456") == false);
  assert(isvalid("12","01","0000") == false);
  assert(isvalid("32","01","1000") == false);
  assert(isvalid("29","02","2017") == false);
}

//test functions
void test(){
  printf("test errors:\n");
  daynumber();
  validity();
  printf("all tests pass\n");
}


//print helpful error messages in terminal
void errmsg(int x[4]){
  if(x[0] == true) printf("error1: 1 or more arguments wrong length, date should look like this:\n\"./date DD MM YYYY\"\n");
  if(x[1] == true) printf("error2: not all characters are digits 0-9\n");
  if(x[2] == true) printf("error3: 0th day, month or year does not exist\n");
  if(x[3] == true) printf("error4: date does not exist(too many days in month or months in year)\n");
}

//print day in terminal
void print(int day) {
  switch (day){
    case Monday: printf("Monday"); break;
    case Tuesday: printf("Tuesday"); break;
    case Wednesday: printf("Wednesday"); break;
    case Thursday: printf("Thursday"); break;
    case Friday: printf("Friday"); break;
    case Saturday: printf("Saturday"); break;
    case Sunday: printf("Sunday"); break;
  }
  printf("\n");
}

//run program with no args for testing or with 3 to run normally
int main(int n, char **arg){
  if(n == 1){
    test();
    printf("unless testing enter a date in the form \"./date DD MM YYYY\" \n");
  }else if(n != 4){
    printf("Incorrect number of arguments. Enter a date in the form \"./date DD MM YYYY\" \n");
  }else if(isvalid(arg[1], arg[2], arg[3]) == true){ 
    int result = day(arg[1], arg[2], arg[3]);
    print(result);
  }
  return 0;
}

