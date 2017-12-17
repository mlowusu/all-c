/* Play blackjack/21 with basic casino rules. Uses a 1 deck and does not allow rule variations like splitting or doubling down. I used oxo.c as a starting point for structuring a game.*/

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

//---------------------------------------------------------------------
// types and constants

// Constants for stage in game
enum stage {Betting, Player, Dealer };
typedef enum stage stage;

// Constants to represent validity checking.
enum validity { Unchecked, OK, HitStand, TooHigh, Format };
typedef enum validity validity;

// game objects with players and dealers hand along with players stack and whose turn to act next
struct game {
    int playerhand [9];
    int dealerhand[9];
    int stack;
    stage stage;
    int bet;
};
typedef struct game game;

//---------------------------------------------------------------------
// logic of the game

// Initialize a game
void newGame(game *g, int chips) {
    for(int i=0;i<9;i++){
        g->playerhand[i] = 0;
        g->dealerhand[i] = 0;
    }
    g->stack = chips;
    g->stage = Betting;
    g->bet = 0;
}


// Check whether string typed in by the user is valid return a validity code.
validity valid(game *g, char *text) {
    if(g->stage == Betting){
        for(int i = 0; text[i] != '\0'; i++){
            if(isdigit(text[i]) == 0)return Format;
        }
        if(atoi(text) > g->stack)return TooHigh;
    }
    if(g->stage == Player){
        if(strlen(text)!=1 || !(text[0] == 's' || text[0] == 'h'))return HitStand;
    } 
    return OK;
}

//generate card from remaining pack
int nextcard(game *g){
    int card;
    bool used = true;
    while(used == true){
        card = (rand() % 52)+1;
        for(int i=0; i<9; i++){
            used = false;
            if(g->playerhand[i] == card || g->dealerhand[i] == card) used = true;
        }
    }
    return card;
}
//count number of cards already in a hand
int cards(int array[9]){
    int cards = 0;
    for(int i = 0; array[i] != 0; i++){
        if(array[i] != 0) cards++;
    }
    return cards;
}

//deal out to player and dealer initial two cards 
void deal(game *g){
    for(int i=0; i<2; i++){
        g->playerhand[i] = nextcard(g);
        g->dealerhand[i] = nextcard(g);
    }
}

//allocate next card to player or dealer
void hit(game *g){
    int card = nextcard(g);
    if(g->stage == Player){
        g->playerhand[cards(g->playerhand)] = card;
    }
    else{
        g->dealerhand[cards(g->dealerhand)] = card;
    }
}

//value of card
int value(int card, bool hard){
    if(card > 36) return 10;
    if(card > 32) return 9;
    if(card > 28) return 8;
    if(card > 24) return 7;  
    if(card > 20) return 6;
    if(card > 16) return 5;
    if(card > 12) return 4;
    if(card > 8) return 3;
    if(card > 4) return 2;
    if(card == 0) return 0;
    if(hard == true)return 11;
    return 1;
}


int countsoft(int hand[9]){
    int count = 0;
    for(int i = 0; i<9; i++){
        count = count + value(hand[i], false);
    }
    return count;
}

int counthard(int hand[9]){
    int count = 0;
    for(int i = 0; i<9; i++){
        count = count + value(hand[i], true);
    }
    return count;
}

//calculate winnings from bet (if any)
int winnings(game *g){
    int cash = 0;
    int player = countsoft(g->playerhand);
    int dealer = countsoft(g->dealerhand);
    if(counthard(g->playerhand) <22) player = counthard(g->playerhand);
    if(counthard(g->dealerhand) <22) dealer = counthard(g->dealerhand);
    if(player > 21) cash = 0;
    else if(player > dealer || dealer >21){
        if(player == 21 && g->playerhand[2] == 0)cash = 5*(g->bet)/2;//blackjack pays 3:2
        else cash = 2*(g->bet);
    }
    else if(player == dealer){
        if((player == 21 && cards(g->playerhand) == 2) && cards(g->dealerhand) !=2)cash=5*(g->bet)/2;
        else if(dealer == 21 && cards(g->dealerhand) == 2) cash = 0;
        else cash = g->bet;
    }
    return cash;
}
//---------------------------------------------------------------------
// I/O


// Print a validity error message.
void printInvalid(validity v, game *g) {
    if (v == Format) printf("\nenter a numerical bet");
    else if (v == HitStand) printf("Type \"h\" to hit or \"s\" to stand");
    else if (v == TooHigh) printf("You cant bet more than %d", g->stack);
    printf("\n");
}

// Display table
void display(game *g) {
    bool end = false;
    if(g->stage == Player){
        printf("Dealers face up card: %d\n", value(g->dealerhand[0], false));
    }
    else{
        printf("Dealer: ");
        for(int i = 0; i<9 && end == false; i++){
            if(g->dealerhand[i] == 0)end = true;
            if(end == false)printf("%d ", value(g->dealerhand[i], false));
        }
        printf("\n");
    }
    end = false;
    printf("Players cards: ");
    for(int i = 0; i<9 && end == false; i++){
        if(g->playerhand[i] == 0)end = true;
        if(end == false)printf("%d ", value(g->playerhand[i], false));
        }
    printf("\nstack: £%d\ncurrent bet: £%d\n\n", g->stack, g->bet);
    
}

// Ask  player to bet repeatedly until they enter valid bet
void askBet(game *g, char text[100]){
    int i;
    while (i != OK){
        printf("place a bet\n");
        fgets(text, 100, stdin);
        text[strcspn(text, "\r\n")] = '\0';//this is needed bc fgets leaves newline in input
        i = valid(g, text);
        printInvalid(i, g);
    }
    g->bet = atoi(text);
    g->stage = Player;
    g->stack = (g->stack - atoi(text));
}

//continuously ask hit or stand and carry out action until standing
bool askHit(game *g, char text[100], bool isstand){
    int i = Unchecked;
        while(i != OK){
            printf("hit or stand?\n");
            fgets(text, 100, stdin);
            text[strcspn(text, "\r\n")] = '\0';//this is needed bc fgets leaves newline in input
            i = valid(g, text);
            printInvalid(i, g);
        }
    if(text[0] == 'h'){
        hit(g);
        isstand = false;
        display(g);
    }
    else{
        isstand = true;
        g->stage = Dealer;
    }
    return isstand;
}

void dealerPlay(game *g){
    while(counthard(g->dealerhand) < 17){
        hit(g);
    }
    display(g);
}

bool bustP(game *g){
    if(countsoft(g->playerhand) > 21){
        printf("your bust\n\n");
        g->stage = Dealer;
        return true;
    }
    return false;
}

// Play against dealer (computer)
void play() {
    game *g = malloc(sizeof(game));
    newGame(g, 500);
    char text[100];
    bool isstand = false;
    while (g->stack != 0){
        printf("stack: £%d\n", g->stack);
        askBet(g, text);
        deal(g);
        display(g);
        while(askHit(g, text, isstand) == false && bustP(g) == false){}
        dealerPlay(g);
        printf("you won £%d\n\n", winnings(g));
        newGame(g, g->stack + winnings(g));
    }
    display(g);
    printf("you have run out of chips\n");
}

//---------------------------------------------------------------------
// Testing and running


// Test initialization of the game
void testNew(game *g) {
    *g = (game) {{2,2,2,2,2,2,2,2,2}, {2,2,2,2,2,2,2,2,2}, 0, 5, 8};
    newGame(g, 500);
    assert(g->playerhand[0] == 0);
    assert(g->stack == 500);
    assert(g->stage == Betting);
    assert(g->bet == 0);
}

void testValid(game *g) {
    char text[100] = {'4','1','0','\0'};
    assert(valid(g, text)  == OK);
    text[0] = '5';
    assert(valid(g, text) == TooHigh);
    g->stage = Player;
    char text2[100] = {'h','\0'};
    assert(valid(g, text2) == true);
    text2[0] = 't';
    assert(valid(g, text2) == HitStand);
    text2[0] = 's';
    text2[1] = 'h';
    text2[2] = '\0';
    assert(valid(g, text2) == HitStand);
}

void testHit(game *g){
    *g = (game) {{2,5,0,0,0,0,0,0,0}, {2,2,2,2,2,2,2,2,2}, 500, Player, 50};
    hit(g);
    assert(g->playerhand[2] != 0);
    assert(g->playerhand[3] == 0); 
}

void testDeal(game *g){
    newGame(g, 500);
    deal(g);
    assert(g->dealerhand[1] != 0);
    assert(g->playerhand[2] == 0);
}

void testCount(game *g){
    int array[9] = {2,50};
    int array2[9] = {0};
    assert(counthard(array) == 21);
    assert(counthard(array2) == 0);
    assert(countsoft(array) == 11);
}

void testWinnings(game *g){
    *g = (game) {{2,30,0,0,0,0,0,0,0}, {3,50,0,0,0,0,0,0,0}, 500, Player, 50};
    assert(winnings(g) == 0);
    *g = (game) {{52,1,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0}, 500, Player, 20};
    assert(winnings(g) == 50);
    *g = (game) {{2,5,37,11,0,0,0,0,0}, {10,16,0,0,0,0,0,0,0}, 500, Player, 50};
    assert(winnings(g) == 100);
}

// Unit testing.
void test() {
    game *g = malloc(sizeof(game));
    testNew(g);
    testValid(g);
    testHit(g);
    testDeal(g);
    testCount(g);
    testWinnings(g);
    printf("All tests passed.\n");
}


// Run the program with no arguments to play or with anything else to test
int main(int n, char *args[n]) {
    srand(time(NULL));
    if (n == 1) play();
    else{
        fprintf(stderr, "Use: \"./blackjack\" to play or \"./blackjack t\" to test\n");
        test();
    }
    return 0;
}
