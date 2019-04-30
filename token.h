//
// Created by sergi on 4/2/2019.
//
#include <iostream>
using namespace std;
#ifndef CALCULADORA_TOKEN_H
#define CALCULADORA_TOKEN_H


class token {
private:
    int type;
    string buffer;
    int precedence;
    int associative;
public:
    token();
    token(int type, string buffer);
    int getType();
    int getAssociative();
    int getPrecedence();
    string getBuffer();
};


#endif //CALCULADORA_TOKEN_H
