//
// Created by sergi on 4/2/2019.
//

#include "token.h"
const int OPERAND = 0, OPERATOR = 1, SENTINEL = 2;
const int LEFT_ASSOCIATIVE = 1;
const int HIGH_PRECEDENCE = 1;
const int LOW_PRECEDENCE = 0;
using namespace std;

token::token(int type, string buffer) {
    this->type = type;
    this->buffer = buffer;
    this->associative = -1;
    this->precedence = -1;
    char c;
    if (type == OPERATOR){
        c = buffer[0];
        if (c == '*' or c == '/') this->precedence = HIGH_PRECEDENCE; /// / and * are higher precedence than + and -. plus / and - are left associative
        else this->precedence = LOW_PRECEDENCE;
        if (c == '-' or c == '/') this->associative = LEFT_ASSOCIATIVE;
        else this->associative = 0;
    }
}

int token::getType() {
    return this->type;
}
string token::getBuffer() {
    return this->buffer;
}
int token::getAssociative() {
    return this->associative;
}
int token::getPrecedence() {
    return this->precedence;
}