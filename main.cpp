#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include "token.h"

const int OPERAND = 0, OPERATOR = 1, SENTINEL = 2, SPACE = 3;

const int LEFT_ASSOCIATIVE = 1;
const int HIGH_PRECEDENCE = 1;
const int LOW_PRECEDENCE = 0;

int getType(char c) {
    if  (c >= '0' && c <= '9') return OPERAND;
    if (c == '*' || c == '+' || c == '/' || c == '-' ) return OPERATOR;
    if (c == '~') return SENTINEL; // sentinel
    if (c == ' ') return SPACE;
    return -1;
}

vector <token> stringToTokens(string s1) {

    string buffer;
    s1 += "~";
    char c;
    int type;
    vector<token> ret;
    bool found = false;
    bool firstNegative = false;
    bool stop = false; //I use this boolean to stop the iteration if there is something wrong in the input string.
    if (s1[0] == '-'){//if first operand is negative
        firstNegative = true;
        s1.erase(0,1);//we delete the negative sign to be added later
    }
    for (int i = 0; i < s1.size() and not stop; ++i) {
        c = s1[i];
        type = getType(c);
        if (type == -1){
            std::cout << "The input contains wrong characters" << endl;
            stop = true;
        }
        else if (type == OPERAND) {
            buffer += c;
            found = true;
        } else if (type == OPERATOR or type == SENTINEL) {
            if (!found) {
                std::cout << "Wrong expression" << endl;
                stop = true;
            }
            if (!buffer.empty()) {
                if (firstNegative) buffer.insert(0, "-");//adding the negative sign we removed before
                firstNegative = false;
                token tk(OPERAND, buffer);
                ret.push_back(tk);
                buffer = "";
                found = false;
            } else {}
            token tk(OPERATOR, s1.substr(i,1));
            ret.push_back(tk);
        }
    }
    if (stop){
        vector<token> fail;
        return fail;
    }

    return ret;
}

queue <token> shuntingYardAlgorithm(vector <token> tokens) {

    queue<token> output;
    stack<token> stck;
    token tk(SPACE, ""); //auxiliar token

    //while there are tokens to be read:
    for (int i = 0; i < tokens.size() - 1; ++i) {
        if (tokens[i].getType() == OPERAND) { //if the token is a number, then:
            output.push(tokens[i]); // push it to the output queue.
        }
        if (tokens[i].getType() == OPERATOR) { //if the token is an operator, then:
            if (!stck.empty())  tk = stck.top();
            //while ((there is a function at the top of the operator stack)  or (there is an operator at the top of the operator stack with greater precedence) or (the operator at the top of the operator stack has equal precedence and is left associative))
            while (!stck.empty() and ((tk.getType() == OPERATOR and tk.getPrecedence() >= tokens[i].getPrecedence()) or
                                      (tk.getType() == OPERATOR and tk.getPrecedence() == tokens[i].getPrecedence() and
                                       tk.getAssociative() == LEFT_ASSOCIATIVE))) {
                output.push(tk);//pop operators from the operator stack into the output queue.
                stck.pop();
                if (!stck.empty()) tk = stck.top();
            }
            stck.push(tokens[i]); //push it onto the operator stack.
        }

    }
    /*
    if there are no more tokens to read:
    while there are still operator tokens on the stack:
    pop the operator from the operator stack onto the output queue.*/
    while (!stck.empty()) {
        output.push(stck.top());
        stck.pop();
    }
    return output;

}
//function used to evaluate an operator token with two double type operands
double evaluate(token tk, double n2, double n1){
    if (tk.getBuffer() == "+") return n2+n1;
    else if (tk.getBuffer() == "-") return n2-n1;
    else if (tk.getBuffer() == "*") return n2*n1;
    else if (tk.getBuffer() == "/") return n2/n1;
    return -1;
}

double solveReversePolish(queue<token> in) {
    stack<double> stck;
    string tmp;
    double n1, n2, result;
    while (!in.empty()) { //for each token in the reversed postfix expression:
        if (in.front().getType() == OPERAND) stck.push(std::stod(in.front().getBuffer()));// if token is an operand:
        else if (in.front().getType() == OPERATOR) {
            n2 = stck.top();
            stck.pop();// operand_2 ← pop from the stack
            n1 = stck.top();
            stck.pop();//operand_1 ← pop from the stack
            result = evaluate(in.front(), n1, n2); // result ← evaluate token with operand_1 and operand_2
            stck.push(result); //push result back onto the stack
        }
        in.pop();
    }
    result = stck.top();
    stck.pop();
    return result;
}


int main() {

    std::cout
            << "Welcome to the Calculator. Enter the arithmetic expresions you would like to evaluate. Write 'exit' to exit"
            << endl;
    string s;
    vector<token> tokens;
    while (getline(std::cin, s)) {
        if (s == "exit") exit(0);
        else {
            tokens = stringToTokens(s); //turns the input into a vector of tokens
            if (!tokens.empty()) {
                queue<token> output = shuntingYardAlgorithm(tokens);//applies the shunting yard algorithm to get our expression in Reverse Polish Notation
                double result = solveReversePolish(output);//computes a Reverse Polish Notation expression.
                std::cout << "The result is: " << result << endl;
            }
        }
    }
}

