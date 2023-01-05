/*
Code adopted from: focs_11_27.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#define FAILED NULL
#define nullChar 'X' // X is symbol for null

const int N = 8;
bool sym = false;
bool paren = false;
bool ch = false;


typedef struct NODE* TREE;
struct NODE {
    char label;
    bool isTerminal;
    TREE leftmostChild;
    TREE rightSibling;
};

TREE makeNode0(char x, bool terminal);
TREE makeNode1(char x, TREE t);
TREE makeNode2(char x, TREE t1, TREE t2);
TREE makeNode3(char x, TREE t1, TREE t2, TREE t3);
TREE makeNode4(char x, TREE t1, TREE t2, TREE t3, TREE t4); 
TREE E();
TREE e();
TREE T();
TREE t();
TREE F();
TREE S();
TREE s();
TREE B();
TREE parseTree; /* holds the result of the parse */
char* nextTerminal; /* current position in input string */
bool lookahead (char c);
bool match (char c);
void printTree (TREE t, int lvl);
void printYield (TREE t);
void freeTree(TREE t);

bool lookahead (char c) {
    return *nextTerminal == c;
}

bool match(char c) {
    if (lookahead(c)) {
	nextTerminal += 1;
	return true;
    } else {
	return false;
    }
}

// make node of 0 children (ie leaf)
TREE makeNode0(char x, bool terminal)
{
    TREE root;
    root = (TREE) malloc(sizeof(struct NODE));
    root->label = x;
    root->leftmostChild = NULL;
    root->rightSibling = NULL;
    root->isTerminal = terminal;
    return root;
}

// make tree with one child (leftmost is input tree)
TREE makeNode1(char x, TREE t)
{
    TREE root;
    root = makeNode0(x, false);
    root->leftmostChild = t;
    return root;
}

// make tree with 2 children (ie one left child and that child having 1 right sibling)
TREE makeNode2(char x, TREE t1, TREE t2) {

    TREE root;
    root = makeNode1(x, t1);
    root->leftmostChild = t1;
    root->leftmostChild->rightSibling = t2;
    return root; 
}

// make tree with 2 children (ie one left child and that child having 1 right sibling)
TREE makeNode3(char x, TREE t1, TREE t2, TREE t3) {

    TREE root;
    root = makeNode1(x, t1);
    root->leftmostChild = t1;
    root->leftmostChild->rightSibling = t2;
    root->leftmostChild->rightSibling->rightSibling = t3;
    return root; 
}

// make tree with 4 children (right siblings are input trees)
TREE makeNode4(char x, TREE t1, TREE t2, TREE t3, TREE t4)
{
    TREE root;
    root = makeNode1(x, t1);
    t1->rightSibling = t2;
    t2->rightSibling = t3;
    t3->rightSibling = t4;
    return root;
}

TREE E() {
// branch into two trees and call T() and ET() on each.
// ensure none are null (if so return null)
// add as left child/right sibling to origin tree
TREE r1 = T();
if (r1 == NULL)  {
    return FAILED;
}

TREE r2 = e();
if (r2 == NULL) {
    freeTree(r1);
    return FAILED;
}

TREE r3 = makeNode2('E', r1, r2);
return r3;

}

TREE e() {// Synonymous to ET
// lookahead to find epsilon (denotated 3). if so, return one branched node called ET---3 using left sibling
// else we must match a "|" (use match('|') and E(), so make node ET with left sibling of "|" and right sibling of E() similar proc as above
if (lookahead('|')) {

    match('|');
    TREE r1 = E();
    if (r1 == NULL) {
        return FAILED;
    }
    TREE r2 = makeNode2('e', makeNode0('|', true), r1);
    return r2;
}

else {
    TREE r1 = makeNode1('e',makeNode0(nullChar, false));
    return r1;
}
}

TREE T() {   
// must match F() and t() as left child and right sibling respectively

TREE r1 = F();
if (r1 == NULL) {
    return FAILED;
}

TREE r2 = t();
if (r2 == NULL) {
    freeTree(r1);
    return FAILED;
}

TREE r3 = makeNode2('T', r1, r2);
return r3;
}

TREE t() {// Synonymous to TT

// like e, look for epsilon and return respective tree.
// else we must match an "&" and T(), again refer to e.

if (lookahead('&')) {
    match('&');
    TREE r1 = T();
    if (r1 == NULL) {
        return FAILED;
    }

    TREE r2 = makeNode2('t', makeNode0('&', true), r1);
    return r2;
}

else {
    TREE r1 = makeNode0(nullChar, false);
    return makeNode1('t', r1);
}
}

TREE F() {
// must have either an ~ then F(), parenthesized expression with E(), or S()

if (lookahead('~')) {
    match('~');
    TREE r1 = F();
    if (r1==NULL) {
        return FAILED;
    } 

    TREE r2 = makeNode2('F', makeNode0('~', true), r1);
    return r2;
}

else if (lookahead('(')) {
    match('(');
    TREE r1 = E();
    if (r1==NULL) {
        return FAILED;
    }
    if(!lookahead(')')) {
        return FAILED;
    }
    match(')');
    return makeNode3('F', makeNode0('(', true), r1, makeNode0(')', true));
}

else {

    TREE r1 = S();
    if(r1==NULL) {
        return FAILED;
    }

    TREE r2 = makeNode1('F', r1);
    return r2;
}
}

TREE S() {
// must match B() and s()

TREE r1 = B();
if (r1 == NULL) {
    return FAILED;
}

TREE r2 = s();
if (r2 == NULL) {
    freeTree(r1);
    return FAILED;  
}

TREE r3 = makeNode2('S', r1, r2);
return r3;
}

TREE s() {// synonymous to ST
//check for epsilon
// else must match an S()

TREE r1 = S();

if (r1 != NULL) {

    TREE r2 = makeNode1('s', r1);
    return r2;
}

else {

    TREE r1 = makeNode1('s', makeNode0(nullChar, false));
    return r1;
}
}

TREE B() {
// must match a 0 or 1 (makenode1 on 0 and 1 with B as tree input)

if (lookahead('0')) {
    match('0');
    TREE r1 = makeNode1('B', makeNode0('0', true));
    return r1;
}

else if (lookahead('1')) {
    match('1');
    TREE r1 = makeNode1('B', makeNode0('1', true));
    return r1;
}

else {
    return FAILED;
}
}

void printTree (TREE t, int lvl) {

    printf("\n");
    for (int i = 0; i <= lvl; i++) {
        printf(" ");
    }

    if (t != FAILED) {

        printf("%c",t->label);
        // go down tree
        if (t->leftmostChild != NULL) {
        printTree(t->leftmostChild, lvl+1);
        }
        // print siblings
        if (t->rightSibling != NULL) {
        printTree(t->rightSibling, lvl);
        }
    }
}

void printYield(TREE t) {

if (t->isTerminal) printf("%c", t->label);

if(t->leftmostChild != NULL) {
    printYield(t->leftmostChild);
}

if(t->rightSibling != NULL) {
    printYield(t->rightSibling);
}
}

void freeTree(TREE t) {
if (t != NULL) {
    if(t->leftmostChild != NULL) {
    freeTree(t->leftmostChild);
    }
    if(t->rightSibling != NULL) {
        freeTree(t->rightSibling);
    }

    free(t);
    t = NULL;
}
}

// second part
char stack[256];
int count = 0;

void push(char* c) {
    if (count == 256) {
        fprintf(stderr, "There's no space in the stack\n");
        return;
    }
    stack[count] = *c;
    count++;
}

char pop() {
    if (count == 0) {
        fprintf(stderr, "Nothing in stack\n");
        return 'z';
    }
    char top = stack[count-1];
    count--;
    return top;
}

void printStack() {
    
    printf("Stack: ");
    for (int i = count - 1; i >= 0; i--) {
        printf("%c ", stack[i]);
    }
    printf("\n");
}




bool TableParsing(char expression[], int N, int table[][N]) {

    int length = 0;
    int opc = 0;
    int cpc = 0;
    while (expression[length] != '\0') {
        length++;
    } 
    
    for (int i = 0; i < length; i++) {
        if (i > 0) {
            if ((expression[i] == '|' && expression[i-1] == '|') || (expression[i] == '&' && expression[i-1] == '&')) {
                sym = true;
                return false;
            }
        }
        if (expression[i] == '(') {
            opc++;
        }
        if (expression[i] == ')') {
            cpc++;
        }
        if (expression[i] != '0' && expression[i] != '1' && expression[i] != '&' && expression[i] != '~' && expression[i] != '|' && expression[i] != '(' && expression[i] != ')') {
            ch = true;
            return false;
        }
    }

    if (opc != cpc) {
        paren = true; 
        return false;

    }
   
   
    char grammar[14][4] = {{"Te"}, {"|E"}, {"$"}, {"Ft"}, {"&T"}, {"$"}, 
    {"~F"}, {"(E)"}, {"S"}, {"Bs"}, {"S"}, {"$"}, {"0"}, {"1"}};
    int L[14] = {2,2,1,2,2,1,2,3,1,2,1,1,1,1};
    char x[8] = {'|','&','~','(',')','0','1','$'};
    char y[8] = {'E','e','T','t','F','S','s','B'};

    char temp = 'E';
    push(&temp);
    printStack();

    int c1 = 8;
    int c2 = 8;
    bool shift = false;
 
    for (int i = 0; i < length; i++) {

    shift = false;

    while (shift == false) {
    for (int j = 0; j < 8; j++) {
        if (x[j] == expression[i]) {
            c2 = j;
            break;
        }
        else if (j == 7 && x[j] != expression[i]) {
            return false;
        }
         
    }
    char symbol = pop();
    for (int j = 0; j < 8; j++) {
        if (symbol == x[j]) {
            if (symbol == expression[i]) {
                shift = true;
                break;
            }
            else {
                return false;
            }

        }
    }
    if (shift == true) {
        break;
    }
    for (int j = 0; j < 8; j++) {
       
       if (y[j] == symbol) {
            c1 = j;
            break;
        }
        else if (j == 7 && y[j] != symbol) {
            return false;
        }  
    }
    bool a = true;
    printf("(%c,%c) = %d\n", y[c1], x[c2], table[c1][c2]);
    
   
    if (table[c1][c2] == 0 && x[c2] == ')') {
        
        while (count > 0) {
            if (pop() == ')') {
                a = false;
                break;
            }
            else if (count == 1 && pop() != ')') {
                return false;
            }
        }
    }
    if (!a) {
        break;
    }
   
    for (int j = L[table[c1][c2] - 1]; j >= 1; j--) {
        
        push(&grammar[table[c1][c2] - 1][j - 1]);
       
        
    }
    printf("\n");
    printStack();
    
     
    }
       
}
    // handles empty symbols
    printf("\n-------------------------TAIL CATEGORIES----------------------\n\n");
    for (int i = count - 1; i >= 0; i--) {
        
        printStack();
        char symbol = pop();
        for (int j = 0; j < 7; j++) {
            if (y[j] == symbol) {
                if (table[j][7] != 0) {
                    printf("(%c,%c) = %d\n", symbol, x[7], table[j][7]);
                    continue;
                }
                else {
                    return false;
                }
            }
        }
    }

    
    return true;

}

int* parseTable() {

    static int table[64] = {0,0,1,1,0,1,1,0,2,0,0,0,0,0,0,3,0,0,4,4,0,4,4,0,0,5,0,0,0,0,0,6,0,0,7,8,0,9,9,0,0,0,0,0,0,10,10,0,2,5,0,0,0,11,11,12,0,0,0,0,0,13,14,0};
    return table;
}

int main()
{
    bool quit = false;
    nextTerminal = (char*)calloc(100, sizeof(char));
    char* origin = nextTerminal;
    while (!quit) {
        printf("\nEnter a bitstring statement for recursive descent parsing (\"quit\" to quit): ");
        fgets(nextTerminal, 100, stdin);
        printf("\nYou entered: %s", nextTerminal);

        if(strstr(nextTerminal, "quit")) {
            quit = true;
            continue;
        }

        else {
            parseTree = E();
                if (parseTree == FAILED) {
                printf("FAILED");
                nextTerminal = origin;
            }

            else {
            printf("\nSuccess! Here's the parse tree:\n");
            printTree(parseTree, 0);
            printf("\nTree Yield: ");
            printYield(parseTree);
            freeTree(parseTree);
            nextTerminal = origin;
            }
        }
    }
    free(nextTerminal);
    nextTerminal = NULL;
    printf("\nQuitting RDP program...");

// second part
int* t = parseTable();
int count = 0;
int table[N][N];
bool parse = true;

    for (int i = 0; i < N; i++) {
        printf("\n");
        for (int j = 0; j < N; j++) {
            table[i][j] = t[count];
            count++;
        }
    }

    char bitInput[32];

    while (parse) {

    printf("\nPlease enter your bitstrings for table-driven parsing (\"quit\" to quit): ");
    scanf("%s", bitInput);

    if (bitInput[0] == 'q' && bitInput[1] == 'u' && bitInput[2] == 'i' && bitInput[3] == 't') {
        parse = false;
        printf("\nThanks for using\n");
        break;
    }

    bool x = TableParsing(bitInput, N, table);
     if (x)
            {
                printf("\ninput is accepted\n");
            }
            else
            {
                if (ch == true) {
                    printf("\nInvalid grammar");
                    ch = false;
                }
                if (sym == true) {
                    printf("\nExcess operators");
                    sym = false;
                }
                if (paren == true) {
                    printf("\nExcess parentheses");
                    paren = false;
                }
                

                printf("\ninput is rejected\n");
            }

    printf("\n");
    }
   
    return 0;
    
}