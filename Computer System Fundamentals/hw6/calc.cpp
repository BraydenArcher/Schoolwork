#include "calc.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

struct Calc {

};

class CalcImpl : public Calc {
    std::map<std::string, int> variables{};
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
public:
    CalcImpl() = default;
    int evalExpr(const char *expr, int *result);
    static std::vector<std::string> tokenize(const std::string& expr);
    bool variableExists (std::string variable);
    bool evaluateThreeExpression(std::string num1, std::string op, std::string num2, int *result);
    bool fetchValueOfExpression(std::string expression, int *result);
    bool valueIsNumber(std::string num);
    bool validVariable(std::string num);
};

std::vector<std::string> CalcImpl::tokenize(const std::string& expr) {
    std::vector<std::string> vec;
    std::stringstream s(expr);
    std::string tok;
    while (s >> tok) {
        vec.push_back(tok);
    }

    return vec;
}

bool CalcImpl::valueIsNumber(std::string num) { //Checks if a value is a number
    char *endPtr = NULL;
    errno = 0;
    strtol(num.c_str(), &endPtr, 10);
    if(errno == 0 && num.c_str() && !*endPtr) {
        return true;
    }

    return false;
}

bool CalcImpl::validVariable(std::string num) { //Checks if a variable is valid (does not have numbers)
    for (char i : num) {
        if(i < 65) { //If numeric is found return false
            return false;
        }
    }
    return true;
}

bool CalcImpl::variableExists (std::string variable) { //Checks if the given variable has been defined
    pthread_mutex_lock(&mutex);
    if(variables.find(variable) == variables.end()) {
        pthread_mutex_unlock(&mutex);
        return false;
    }
    pthread_mutex_unlock(&mutex);
    return true;
}


bool CalcImpl::evaluateThreeExpression(std::string num1, std::string op, std::string num2, int *result) { //Evaluates a string expression separated
    int firstNum = 0;
    if(!fetchValueOfExpression(num1, &firstNum)) {
        return false;
    }
    int secondNum = 0;
    if(!fetchValueOfExpression(num2, &secondNum)) {
        return false;
    }
    if(op == "+") { //Actually do the expression
        *result = firstNum + secondNum;
        return true;
    } else if (op == "-"){
        *result = firstNum - secondNum;
        return true;
    } else if (op == "*"){
        *result = firstNum * secondNum;
        return true;
    } else if (op == "/"){
        if(secondNum == 0) {
            return false;
        }
        *result = firstNum / secondNum;
        return true;
    }
    return false; //If op isn't found its false
}


bool CalcImpl::fetchValueOfExpression(std::string expression, int *result) { //Get the value of the string and store in result, if value not valid then return false
    if(validVariable(expression)) {
        if(variableExists(expression)) {
            pthread_mutex_lock(&mutex);
            *result = variables[expression];
            pthread_mutex_unlock(&mutex);
            return true;
        }
        return false;
    }
    if(valueIsNumber(expression)) {
        *result = strtol(expression.c_str(), NULL, 10);
        return true;
    }
    return false;
}


int CalcImpl::evalExpr(const char *expr, int *result) { //Evaluates the expression
    std::vector<std::string> expression = tokenize(expr);
    //Number first
    if(expression.size() == 1) { //If its just a request
        if(!valueIsNumber(expression[0]) && !variableExists(expression[0])) { //If its not a number and doesnt exist error
            return 0;
        }
        if(fetchValueOfExpression(expression[0], result)) { //If its value is valid, return value and 1
            return 1;
        }
    }
    if(expression.size() == 3 && expression[1] == "=" && !valueIsNumber(expression[0])) { //If its a 'simple' assignment
        if(!validVariable(expression[0])) { //If its not valid error
            return 0;
        }
        if(fetchValueOfExpression(expression[2], result)) { //Get value and return, if no value, false
            pthread_mutex_lock(&mutex);
            variables[expression[0]] = *result;
            pthread_mutex_unlock(&mutex);
            return 1;
        }
    }
    if(expression.size() == 3) { //If its a simple expression
        if(evaluateThreeExpression(expression[0], expression[1], expression[2], result)) { //Evaluate
            return 1;
        }
    }
    if(expression.size() == 5 && validVariable(expression[0])) { //If its a complex assignment
        if(evaluateThreeExpression(expression[2], expression[3], expression[4], result)) { //Evaluate
            pthread_mutex_lock(&mutex);
            variables[expression[0]] = *result;
            pthread_mutex_unlock(&mutex);
            return 1;
        }
    }
    return 0;
}

extern "C" struct Calc *calc_create(void) {
    return new CalcImpl();
}

extern "C" void calc_destroy(struct Calc *calc) {
    delete calc;
}


extern "C" int calc_eval(struct Calc *calc, const char *expr, int *result) {
    CalcImpl *obj = static_cast<CalcImpl *>(calc);
    return obj->evalExpr(expr, result);
}