#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

enum coordinate { X, Y};
typedef enum coordinate coordinate;

enum op { DX, DY, DT, PEN, CLEAR, KEY, COL };
typedef enum op op;

struct state {
    int prev[2];
    int current[2];
    bool pendown;
};
typedef struct state state;

int opcode(unsigned char byte);
void extended(int code, int erand, display *display, state *state, FILE *f);


//extract operand from given byte
int operand(unsigned char byte){
    int x = (byte & 31) - (byte & 32);
    if(opcode(byte) == DT) x = (byte & 63);
    return x;
}

//extract opcode from given byte
int opcode(unsigned char byte){
    int x = (byte >> 6);
    return x;
}

//move pen to by changing x coordinate(never draws)
void moveX(state *state, int x){
    state->prev[X] = state->current[X];
    state->current[X] = (state->current[X]) + x;
    if (!(state->pendown))state->prev[X] = state->current[X];
}

//move pen by changng y coordinate(draws if pen down)
void moveY(state *state, int y){
    state->prev[Y] = state->current[Y];
    state->current[Y] = (state->current[Y]) + y;
}

//read in extra bytes for new operand
int newerand(int l, FILE *f, int code){
    int erand = 0;
    int i = 0;
    int x;
    for(;i<l;i++){
        x = fgetc(f);
        erand = (erand <<8) | x;
    }
    int y = 8*(4-l);
    erand = (erand<<y)>>y;
    if (code == DT && erand < 0) erand = erand | 2*(1 << (8*l));
    return erand;
}

//IO/display.h calls
//---------------------------------------------------------------------------


//carry out a single instruction
void show(int code, int erand, display *display, state *state, FILE *f){
    switch(code) {
        case DX: moveX(state,erand);break;
        case DY: moveY(state, erand);
            if(state->pendown == true){
                line(display,state->prev[X],state->prev[Y],state->current[X],state->current[Y]);
                state->prev[X] = state->current[X];
            }
            break;
        case DT: pause(display, 10*erand);break;
        case PEN: extended(code, erand, display, state, f);break;
        case CLEAR: clear(display);break;
        case KEY: key(display);break;
        case COL: colour(display,erand);break;
    }
}

//carry out an extended instruction
void extended(int code, int erand, display *display, state *state, FILE *f){
    int l = ((erand & 48)>>4);
    if(l ==3) l++;
    int newcode = (erand & 15);
    if(newcode == PEN) state->pendown = !(state->pendown);
    else show(newcode, newerand(l,f, newcode), display, state, f);
}

//graph all instructions in a given file
void graph(FILE *f, char *title){
    int code;
    int erand;
    unsigned char byte;
    state *state = malloc(sizeof(state));
    state->prev[X]=0; state->prev[Y]=0; state->current[X]=0; state->current[Y]=0;
    state->pendown = false;
    display *display = newDisplay(title, 200, 200);
    while (! feof(f)) {
        byte = fgetc(f);
        code = opcode(byte);
        erand = operand(byte);
        show(code,erand,display,state, f);
    }
    free(state);
    end(display);
}


//tests
//---------------------------------------------------------------------------------

//test operand function
void testoperand(){
    assert(operand(25) == 25);
    assert(operand(0) == 0);
    assert(operand(141) == 13);
}

//test opcode function
void testopcode(){
    assert(opcode(25) == 0);
    assert(opcode(141) == 2);
    assert(opcode(127) == 1);
}

//test moveX and moveY
void testMove(){
    state *state = malloc(sizeof(state));
    assert(state->prev[0] == 0);
    assert(state->current[1] ==0);
    moveX(state, 2);
    moveY(state, 5);
    assert(state->prev[0] == 0);
    assert(state->current[0] == 2);
    assert(state->prev[1] == 0);
    assert(state->current[1] == 5);
    free(state);
}

void test(){
    testoperand();
    testopcode();
    testMove();
    printf("Tests pass\n");
}

int main(int n, char *args[n]) {
    assert(n==2);
        if(strcmp(args[1],"t") == 0) test();
        else{
            FILE *f = fopen(args[1], "rb");
            graph(f, args[1]);
            fclose(f);
        }
    return 0;
}    
