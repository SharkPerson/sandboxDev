#include <stdlib.h>
#include <stdio.h>

typedef char* TOKEN_TYPE;

#define ILLEGAL "ILLEGAL"
#define INT "INT"
#define ASSIGN "="
#define PLUS "+"
#define COMMA ","
#define SEMICOLON = ";"
#define LPAREN "("
#define RPAREN ")"
#define LBRACE "{"
#define RBRACE "}"
#define FUNCTION "FUNCTION"
#define LET "LET"

struct Lexer{
    char* input;
    int position;
    int readPosition;
    char ch;
};

void read_char(struct Lexer* l) {
    if (l->readPosition >= l->input[sizeof(l->input)]) {
        l->ch = '\0';
    }
    else {
        l->ch = l->readPosition;
    }
    printf("%c", l->input[l->readPosition]);
    l->position = l->readPosition;
    l->readPosition += 1;
}

struct Lexer* new(char* input) {
    struct Lexer* l = malloc(sizeof(*l)); 
    l->input = input;
    read_char(l);
    return l;
}

int main() {
    char* input = "+";
    new(input);
    return 0;
}
