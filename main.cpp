#include <iostream>
#include <vector>
#include <algorithm> // any_of
#include <sstream> // stringstream

using std::cout;
using std::endl;
using std::string;
using std::vector;

enum class OpType { OPERATOR, OPERAND };
vector<string> ranges = { "+-*/", "0123456789." };

void removeSymbolFromString(string &readjust, char const symbol, int leave = 0) {
    int count = 0;

    if (readjust[readjust.length() - 1] == symbol) {
        readjust.erase(std::prev(readjust.end()));
    }

    auto it = std::remove_if(
            readjust.begin(),
            readjust.end(),
            [&count, symbol, leave](char &c) {
                if (c == symbol) ++count;
                return (count > leave && c == symbol);
            });

    readjust.erase(it, readjust.end());
}

// Удаляем из переданной строки точки, кроме первой
void removeDots(string &readjust) {
    const char DOT = 46;
    removeSymbolFromString(readjust, DOT, 1);
}

// Получаем true если элемент `item` хоть раз встречается в диапазоне `range`
bool isIncludes(const string &range, const char &item) {
    return std::any_of(range.begin(),
                       range.end(),
                       [&item](const char &c) { return c == item; });
}

// True если это число или точка
bool isIncludesDotOrDigits(const char &letter) {
    return isIncludes(ranges[static_cast<int>(OpType::OPERAND)], letter);
}

// True если это один из операторов
bool isIncludesOperators(const char &letter) {
    return isIncludes(ranges[static_cast<int>(OpType::OPERATOR)], letter);
}

bool processedOperand(string &text, OpType opType) {
    bool validState = true;
    string processedText;

    for (int i = 0; i < text.length(); ++i) {
        char letter = text[i];

        if (opType == OpType::OPERAND) {
            if (i == 0 && letter == '-') processedText += letter;
            if (isIncludesDotOrDigits(letter)) processedText += letter;
        }

        if (opType == OpType::OPERATOR) {
            if (isIncludesOperators(letter)) processedText += letter;
        }
    }

    if (opType == OpType::OPERAND){
        removeDots(processedText);
    }

    if (processedText.empty()) {
        cout << "Operand is empty. Try again" << endl;
        validState = false;
    } else {
        text = processedText;
    }

    return validState;
}

std::string getJoinRange(const std::string &range) {
    char const TERMINATOR = '.';
    char const JOIN = ',';

    std::string joinRange;
    for (int i = 0; i < range.size(); ++i) {
        joinRange += range[i];
        joinRange += (i != range.size() - 1) ? JOIN : TERMINATOR;
    }

    return joinRange;
}

double stringToDouble(string const &text) {
    double result;
    try {
        result = std::stod(text);
    } catch (std::invalid_argument &err) {
        std::cerr << "\nError: " << err.what() << endl;
    }
    return result;
}

double getResult(vector<string> const &parts) {
    double result = stringToDouble(parts[0]);
    double right = stringToDouble(parts[2]);
    char operatorType = parts[1][0];

    if (operatorType == 42) {
        result *= right;
    } else if (operatorType == 43) {
        result += right;
    } else if (operatorType == 45) {
        result -= right;
    } else if (operatorType == 47) {
        result /= right;
    }

    return result;
}

void printResult(vector<string> const &parts) {
    cout << "After process:  ";
    for (const string &item: parts) cout << item << " ";
    cout << "= ";
    printf("%.3f\n", getResult(parts));
    cout << "---------------------" << endl;
}

void setUserInput(vector<string> &parts, string const &info) {
    cout << "Enter an expression. " << info << ": ";

    string buffer;
    std::getline(std::cin, buffer);
    std::stringstream userStream(buffer);

    for (int i = 0; i < 3; ++i) {
        string currentTextFromStream;
        userStream >> currentTextFromStream;
        bool validState = false;
        while (!validState) {
            OpType opType = (i == 0 || i == 2) ? OpType::OPERAND : OpType::OPERATOR;
            validState = processedOperand(currentTextFromStream, opType);
            if (validState) {
                parts.push_back(currentTextFromStream);
            } else {
                printf("String does not contain %s or empty. Try again: ",
                       getJoinRange(ranges[static_cast<int>(opType)]).c_str());
                string temp;
                std::getline(std::cin, temp);
                std::stringstream userCurrentStream(temp);
                userCurrentStream >> currentTextFromStream;
            }
        }
    }
}

int main() {
    const string INFO = "Required format (separated by spaces): digits operator digits";
    vector<string> parts;

    setUserInput(parts, INFO);

    if (parts.size() == 3) {
        printResult(parts);
    } else {
        cout << "Invalid input format. " << INFO << ".Try again!" << endl;
    }
}