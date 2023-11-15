#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <stack>
#include <map>

struct Node {
    char data;
    Node* left;
    Node* right;

    Node(char val) : data(val), left(nullptr), right(nullptr) {}
};

bool isOperator(char c) {
    return c == '&' || c == '|' || c == '^';
}

bool isOperand(char c) {
    return isalpha(c);
}

int getPrecedence(char op) {
    if (op == '&') return 2;
    else if (op == '|') return 1;
    else if (op == '^') return 3;
    else return 0;
}

std::string minimizeExpression(const std::string& expression) {
    std::stack<char> operators;
    std::stack<std::string> operands;

    for (char c : expression) {
        if (isOperand(c)) {
            std::string op(1, c);
            operands.push(op);
        }
        else if (isOperator(c)) {
            while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(c)) {
                std::string op2 = operands.top();
                operands.pop();
                std::string op1 = operands.top();
                operands.pop();
                std::string newOperand = "(" + op1 + c + op2 + ")";
                operands.push(newOperand);
            }
            operators.push(c);
        }
    }

    while (!operators.empty()) {
        std::string op2 = operands.top();
        operands.pop();
        std::string op1 = operands.top();
        operands.pop();
        std::string newOperand = "(" + op1 + operators.top() + op2 + ")";
        operands.push(newOperand);
        operators.pop();
    }

    return operands.top();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Boolean Expression Minimizer");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Font file not found." << std::endl;
        return -1;
    }

    sf::Text instructionText;
    instructionText.setFont(font);
    instructionText.setCharacterSize(18);
    instructionText.setPosition(10, 10);
    instructionText.setFillColor(sf::Color::Black);
    instructionText.setString("Enter the boolean expression: ");

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setPosition(10, 40);
    inputText.setFillColor(sf::Color::Black);

    sf::Text resultText;
    resultText.setFont(font);
    resultText.setCharacterSize(18);
    resultText.setPosition(10, 70);
    resultText.setFillColor(sf::Color::Black);

    std::string expression;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == '\b' && !expression.empty()) {
                        expression.pop_back();
                    }
                    else if (event.text.unicode != '\b') {
                        expression += static_cast<char>(event.text.unicode);
                    }
                }
                inputText.setString(expression);
                resultText.setString("Minimized: " + minimizeExpression(expression));
            }
        }

        window.clear(sf::Color::White);
        window.draw(instructionText);
        window.draw(inputText);
        window.draw(resultText);
        window.display();
    }

    return 0;
}
