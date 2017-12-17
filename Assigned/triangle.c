/* Classify a triangle according to the integer lengths of its sides. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

// Integer constants representing types of triangle.
enum { Equilateral, Isosceles, Right, Scalene, Flat, Impossible, Illegal };


// Convert a string into an integer(long)
long convert(const char array[]) {
  return atoi(array);
}


//check validity of characters in each length
bool isvalid(const char array[]){
for(int n=0; strlen(array)>n; n++){
  if (isdigit(array[n]) == false) return false;
  }
return true;
}

// Classify a triangle, given side lengths as strings:
int triangle(const char sa[], const char sb[], const char sc[]) {
long a = convert(sa);
long b = convert(sb);
long c = convert(sc);
if(a<=0 || b<=0 || c<=0 || isvalid(sa)==false || isvalid(sb)==false || isvalid(sc)==false || sa[0]=='0'|| sb[0]=='0'|| sc[0]=='0') return Illegal;
if(a + b < c || b + c < a || a + c < b) return Impossible;
if(a + b == c || b + c == a || a + c == b) return Flat;
if(a*a + b*b == c*c || a*a + c*c == b*b || b*b + c*c == a*a) return Right;
if(a == b && b == c) return Equilateral;
if(a == b || b == c || a == c) return Isosceles;
return Scalene;
}

// -----------------------------------------------------------------------------
// User interface and testing.

void print(int type) {
    switch (type) {
        case Equilateral: printf("Equilateral"); break;
        case Isosceles: printf("Isosceles"); break;
        case Right: printf("Right"); break;
        case Scalene: printf("Scalene"); break;
        case Flat: printf("Flat"); break;
        case Impossible: printf("Impossible"); break;
        case Illegal: printf("Illegal"); break;
    }
    printf("\n");
}

// Check that you haven't changed the triangle type constants.  (If you do, it
// spoils automatic marking, when your program is linked with a test program.)
void checkConstants() {
    assert(Equilateral==0 && Isosceles==1 && Right==2 && Scalene==3);
    assert(Flat==4 && Impossible==5 && Illegal==6);
}

// Tests 1 to 2: check equilateral
void testEquilateral() {
    assert(triangle("8", "8", "8") == Equilateral);
    assert(triangle("1073", "1073", "1073") == Equilateral);
}

// Tests 3 to 5: check isosceles
void testIsosceles() {
    assert(triangle("25", "25", "27") == Isosceles);
    assert(triangle("25", "27", "25") == Isosceles);
    assert(triangle("27", "25", "25") == Isosceles);
}

// Tests 6 to 14: check right angled
void testRight() {
    assert(triangle("3", "4", "5") == Right);
    assert(triangle("3", "5", "4") == Right);
    assert(triangle("5", "3", "4") == Right);
    assert(triangle("5", "12", "13") == Right);
    assert(triangle("5", "13", "12") == Right);
    assert(triangle("12", "5", "13") == Right);
    assert(triangle("12", "13", "5") == Right);
    assert(triangle("13", "5", "12") == Right);
    assert(triangle("13", "12", "5") == Right);
}

// Tests 15 to 20: check scalene
void testScalene() {
    assert(triangle("12", "14", "15") == Scalene);
    assert(triangle("12", "15", "14") == Scalene);
    assert(triangle("14", "12", "15") == Scalene);
    assert(triangle("14", "15", "12") == Scalene);
    assert(triangle("15", "12", "14") == Scalene);
    assert(triangle("15", "14", "12") == Scalene);
}

// Tests 21 to 25: check flat
void testFlat() {
    assert(triangle("7", "9", "16") == Flat);
    assert(triangle("7", "16", "9") == Flat);
    assert(triangle("9", "16", "7") == Flat);
    assert(triangle("16", "7", "9") == Flat);
    assert(triangle("16", "9", "7") == Flat);
}

// Tests 26 to 31: check impossible
void testImpossible() {
    assert(triangle("2", "3", "13") == Impossible);
    assert(triangle("2", "13", "3") == Impossible);
    assert(triangle("3", "2", "13") == Impossible);
    assert(triangle("3", "13", "2") == Impossible);
    assert(triangle("13", "2", "3") == Impossible);
    assert(triangle("13", "3", "2") == Impossible);
}

// Tests 32 to 42: check illegal
void testIllegal() {
    assert(triangle("0", "0", "0") == Illegal);
    assert(triangle("0", "10", "12") == Illegal);
    assert(triangle("10", "12", "0") == Illegal);
    assert(triangle("-1", "-1", "-1") == Illegal);
    assert(triangle("-1", "10", "12") == Illegal);
    assert(triangle("10", "-1", "12") == Illegal);
    assert(triangle("10", "12", "-1") == Illegal);
    assert(triangle("x", "y", "z") == Illegal);
    assert(triangle("3", "4y", "5") == Illegal);
    assert(triangle("10", "12", "13.4") == Illegal);
    assert(triangle("03", "4", "5") == Illegal);
}

// Tests 43 to 47: check the upper limits on lengths
void testLimits() {
    assert(triangle("2147483647","2147483647","2147483647") == Equilateral);
    assert(triangle("2147483648","2147483647","2147483647") == Illegal);
    assert(triangle("2147483647","2147483648","2147483647") == Illegal);
    assert(triangle("2147483647","2147483647","2147483648") == Illegal);
    assert(triangle("2147483649","2147483649","2147483649") == Illegal);
}

// Tests 48 to 50: check for correct handling of overflow
void testOverflow() {
    assert(triangle("1100000000","1705032704","1805032704") == Scalene);
    assert(triangle("2000000001","2000000002","2000000003") == Scalene);
    assert(triangle("150000002","666666671","683333338") == Scalene);
}

// Run tests on the triangle function.
void test() {
    checkConstants();
    testEquilateral();
    testIsosceles();
    testRight();
    testScalene();
    testFlat();
    testImpossible();
    testIllegal();
    testLimits();
    testOverflow();
    printf("All tests passed\n");
}

// Run the program or, if there are no arguments, test it.
int main(int n, char **argv) {
    if (n == 1) {
        test();
    }
    else if (n == 4) {
        int result = triangle(argv[1], argv[2], argv[3]);
        print(result);
    }
    else {
        fprintf(stderr, "Use e.g.: ./triangle 3 4 5\n");
        exit(1);
    }
    return 0;
}

