#include <iostream>
#include <cmath>
#include <map>
#include <stack>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cctype>

const int N_OPTR = 9; 
const char operators[] = {'+', '-', '*', '/', '^', '!', '(', ')', '#'}; 
std::map<char, int> opIndex;

// 初始化操作符索引
void initializeOpIndex() {
    opIndex.insert(std::make_pair('+', 0));
    opIndex.insert(std::make_pair('-', 1));
    opIndex.insert(std::make_pair('*', 2));
    opIndex.insert(std::make_pair('/', 3));
    opIndex.insert(std::make_pair('^', 4));
    opIndex.insert(std::make_pair('!', 5));
    opIndex.insert(std::make_pair('(', 6));
    opIndex.insert(std::make_pair(')', 7));
    opIndex.insert(std::make_pair('#', 8));
}

// 书本4.6优先级表
char priorityTable[N_OPTR][N_OPTR] = {
    // +   -   *   /   ^   !   (   )   #
    {'>', '>', '<', '<', '<', '<', '<', '>', '>'}, // +
    {'>', '>', '<', '<', '<', '<', '<', '>', '>'}, // -
    {'>', '>', '>', '>', '<', '<', '<', '>', '>'}, // *
    {'>', '>', '>', '>', '<', '<', '<', '>', '>'}, // /
    {'>', '>', '>', '>', '>', '<', '<', '>', '>'}, // ^
    {'>', '>', '>', '>', '>', '>', ' ', '>', '>'}, // !
    {'<', '<', '<', '<', '<', '<', '<', '=', ' '}, // (
    {'>', '>', '>', '>', '>', '>', ' ', '>', '>'}, // )
    {'<', '<', '<', '<', '<', '<', '<', ' ', '='}  // #
};

// 自定义阶乘函数
double factorial(int n) {
    if (n <= 1) return 1;
    double result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

double stringToDouble(const std::string& s) {
    std::stringstream ss(s);
    double result;
    ss >> result;
    return result;
}

// 获取优先级关系
char precedence(char op1, char op2) {
    return priorityTable[opIndex[op1]][opIndex[op2]];
}

// 基本运算函数
double operate(char op, double a, double b = 0) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return std::pow(a, b);
        case '!': return factorial(static_cast<int>(a)); 
        default: return 0;
    }
}

// 扩展运算
double advancedOperate(const std::string& func, double value) {
    if (func == "sin") return std::sin(value);
    if (func == "cos") return std::cos(value);
    if (func == "log") return std::log(value);
    throw std::invalid_argument("Unsupported function");
}

// 计算器
double evaluate(const std::string& expression) {
    std::stack<char> opStack;
    std::stack<double> valStack;
    opStack.push('#');  

    size_t i = 0;
    while (i < expression.length()) {
        char ch = expression[i];

        if (isdigit(ch)) { 
            double value = 0;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                value = value * 10 + (expression[i] - '0');
                i++;
            }
            valStack.push(value);
            continue;
        } else if (opIndex.find(ch) != opIndex.end()) { 
            while (!opStack.empty() && precedence(opStack.top(), ch) == '>') {
                char op = opStack.top();
                opStack.pop();
                if (op == '!') {
                    double a = valStack.top();
                    valStack.pop();
                    valStack.push(operate(op, a));
                } else {
                    double b = valStack.top();
                    valStack.pop();
                    double a = valStack.top();
                    valStack.pop();
                    valStack.push(operate(op, a, b));
                }
            }
            if (ch != ')') {
                opStack.push(ch);
            } else {
                opStack.pop();
            }
        } else if (isalpha(ch)) {  
            std::string funcName;
            while (i < expression.length() && isalpha(expression[i])) {
                funcName += expression[i];
                i++;
            }
            i++; 
            std::string funcArg;
            while (i < expression.length() && expression[i] != ')') {
                funcArg += expression[i];
                i++;
            }
            i++; 
            double argValue = stringToDouble(funcArg);
            valStack.push(advancedOperate(funcName, argValue));
            continue;
        }
        i++;
    }

    while (opStack.top() != '#') {
        char op = opStack.top();
        opStack.pop();
        if (op == '!') {
            double a = valStack.top();
            valStack.pop();
            valStack.push(operate(op, a));
        } else {
            double b = valStack.top();
            valStack.pop();
            double a = valStack.top();
            valStack.pop();
            valStack.push(operate(op, a, b));
        }
    }

    return valStack.top();
}

int main() {
    initializeOpIndex(); 

    // 案例测试
    try {
        std::cout << "Test 1 (3+5*2): " << evaluate("3+5*2") << std::endl;  // 输出 13
        std::cout << "Test 2 (10+(2*3)^2): " << evaluate("10+(2*3)^2") << std::endl;  // 输出 46
        std::cout << "Test 3 (sin(0.5) + cos(0.5)): " << evaluate("sin(0.5)+cos(0.5)") << std::endl;  // sin(0.5) + cos(0.5)
        std::cout << "Test 4 (log(10) + 3^2): " << evaluate("log(10)+3^2") << std::endl;  // log(10) + 9
        std::cout << "Test 5 (5!): " << evaluate("5!") << std::endl;  // 输出 120
        std::cout << "Test 6 ((3+4)*5): " << evaluate("(3+4)*5") << std::endl;  // 输出 35
        std::cout << "Test 7 (10-3*4+2): " << evaluate("10-3*4+2") << std::endl;  // 输出 0
        std::cout << "Test 8 ((2+3)*(5+6)): " << evaluate("(2+3)*(5+6)") << std::endl;  // 输出 55
        std::cout << "Test 9 (7^2 - 3*2 + 4/2): " << evaluate("7^2 - 3*2 + 4/2") << std::endl;  // 输出 45
        std::cout << "Test 10 (8/(4-2)+sin(1)): " << evaluate("8/(4-2)+sin(1)") << std::endl;  // 8/2 + sin(1)
        std::cout << "Test 11 (cos(0) + 10/2): " << evaluate("cos(0) + 10/2") << std::endl;  // cos(0) + 5 = 6
        std::cout << "Test 12 (3 + 4! + log(1)): " << evaluate("3+4!+log(1)") << std::endl;  // 输出 27
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

