#include <iostream>
#include <vector>
#include <algorithm> // any_of
#include <sstream> // stringstream

using std::cout;
using std::endl;
using std::string;
using std::vector;

void removeSymbolsFromString(string &readjust, char const SYMBOL) {
    int count = 0;

    if (readjust[readjust.length() - 1] == SYMBOL) {
        readjust.erase(std::prev(readjust.end()));
    }

    auto it =
            std::remove_if(readjust.begin(), readjust.end(), [&count, SYMBOL](char &c) {
                if (c == SYMBOL) ++count;
                return (count > 1 && c == SYMBOL);
            });

    readjust.erase(it, readjust.end());
}

// Удаляем из переданной строки точки, кроме первой
void removeDots(string &readjust) {
    const char DOT = 46;
    removeSymbolsFromString(readjust, DOT);
}

// Получаем true если элемент `item` хоть раз встречается в диапазоне `range`
bool isIncludes(const string &range, const char &item) {
    return std::any_of(range.begin(),
                       range.end(),
                       [&item](const char &c) { return c == item; });
}

// Получаем char-символ в обозначенном диапазоне `range`
template<typename T>
T getUserChar(string const &range) {
    T input;

    while (true) {
        std::cin >> input;
        // Если введено значение из диапазона - сбрасываем
        if (isIncludes(range, input)) break;
        printf("Error. (%c) not in range (%s). Try again: ", input, range.c_str());
        // Сбрасываем коматозное состояние cin
        std::cin.clear();
        // Очищаем поток ввода
        fflush(stdin);
    }

    return input;
}

// True если это число или точка
bool isIncludesDotOrDigits(const char &letter) {
    const std::string operators{"0123456789."};
    return isIncludes(operators, letter);
}

void validateOperand(string &text, vector<string> &parts) {
    string validText;

    for (int i = 0; i < text.length(); ++i) {
        char letter = text[i];

        if (i == 0 && letter == '-') validText += letter;

        if (isIncludesDotOrDigits(letter)) {
            validText += letter;
        }
    }

    removeDots(validText);
    parts.push_back(validText);
}

void validateOperator(string &text, vector<string> &parts) {
    const std::string range{"+-*/"};
    string validText;

    if (isIncludes(range, text[0])) {
        validText += text[0];
    } else {
        printf("Enter one of the operators: (%s): ", range.c_str());
        char a = getUserChar<char>(range);
        validText += a;
    }

    parts.push_back(validText);
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
    char op;

    for (int i = 1; i < parts.size(); ++i) {
        if (i % 2 == 1) {
            op = parts[i][0];
        } else {
            double current = stringToDouble(parts[i]);
            if (op == 42) {
                result *= current;
            } else if (op == 43) {
                result += current;
            } else if (op == 45) {
                result -= current;
            } else if (op == 47) {
                result /= current;
            }
        }
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
    string buffer;

    cout << "Enter an expression. " << info << ": ";

    std::getline(std::cin, buffer);
    std::stringstream userStream(buffer);

    for (int i = 0; i < 3; ++i) {
        string currentTextFromStream;
        userStream >> currentTextFromStream;
        if (i == 0 || i == 2) {
            validateOperand(currentTextFromStream, parts);
        } else if (i == 1) {
            validateOperator(currentTextFromStream, parts);
        }
    }
}

int main() {
    const string INFO = "Required format (separated by spaces): digits operator digits";
    vector<string> parts;

    setUserInput(parts, INFO);

    if (parts.size() == 3) {
        cout << parts[2] << endl;
        printResult(parts);
    } else {
        cout << "Invalid input format. " << INFO << ".Try again!" << endl;
    }
}