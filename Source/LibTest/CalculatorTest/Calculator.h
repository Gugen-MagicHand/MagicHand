#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "Stack.h"
#include "Fraction.h"

#include <string.h>

class Operator
{
public:
    enum ASSOCIATIVITY {
        RIGHT,
        LEFT
    };

    enum BRACKET {
        RIGHT_BRACKET,
        LEFT_BRACKET
    };

    int precedence;
    ASSOCIATIVITY associativity;
    char *token = 0x00;
    bool isBracket = false;
    BRACKET bracket = BRACKET::RIGHT_BRACKET;

    void DeleteOperator() {
        if (token != 0x00) {
            delete[] token;
        }

    }

    Operator(char *token, int precedence, ASSOCIATIVITY associativity) {
        // tokenのコピー
        int len = strlen(token);
        this->token = new char[len + 1];

        for (int i = 0; i < len + 1; i++) {
            this->token[i] = token[i];
        }

        this->precedence = precedence;
        this->associativity = associativity;
        this->isBracket = false;
    }

    Operator(char *token, BRACKET bracket) {
        // tokenのコピー
        int len = strlen(token);
        this->token = new char[len + 1];

        for (int i = 0; i < len + 1; i++) {
            this->token[i] = token[i];
        }

        this->bracket = bracket;
        this->isBracket = true;
    }

    ~Operator() {
        DeleteOperator();
    }

    virtual Fraction Operate(const Fraction &fracA, const Fraction &fracB) {
        return 0x00;
    }

};

class OperatorPlus : public Operator {
public:
    OperatorPlus() : Operator("+", 2, ASSOCIATIVITY::LEFT) {

    }

    Fraction Operate(const Fraction &fracA, const Fraction &fracB) {
        return fracA + fracB;
    }
};

class OperatorMinus : public Operator {
public:
    OperatorMinus() : Operator("-", 2, ASSOCIATIVITY::LEFT) {

    }

    Fraction Operate(const Fraction &fracA, const Fraction &fracB) {
        return fracA - fracB;
    }
};


class OperatorMultiply : public Operator {
public:
    OperatorMultiply() : Operator("*", 3, ASSOCIATIVITY::LEFT) {

    }

    Fraction Operate(const Fraction &fracA, const Fraction &fracB) {
        return fracA * fracB;
    }
};


class OperatorDivide : public Operator {
public:
    OperatorDivide() : Operator("/", 3, ASSOCIATIVITY::LEFT) {

    }

    Fraction Operate(const Fraction &fracA, const Fraction &fracB) {
        return fracA / fracB;
    }
};

class OperatorLeftBracket : public Operator {
public:
    OperatorLeftBracket() : Operator("(", BRACKET::LEFT_BRACKET) {

    }
};

class OperatorRightBracket : public Operator {
public:
    OperatorRightBracket() : Operator(")", BRACKET::RIGHT_BRACKET) {

    }
};


class Calculator {
private:

    Stack<Fraction> operandStack;
    Stack<Operator *> operatorPointerStack;

public:

    enum CAL_STATUS {
        CAL_STATUS_SUCCESS,
        CAL_STATUS_STACK_IS_FULL,
        CAL_STATUS_MISMATCHED_PARENTHESES,
        CAL_STATUS_OPERAND_IS_NOT_ENOUGH,
        CAL_STATUS_SOMETHING_ERROR,
        CAL_STATUS_STACK_IS_EMPTY
    };
    Calculator(int operandStackSize, int operatorPointerStackSize) : operandStack(operandStackSize), operatorPointerStack(operatorPointerStackSize) {

    }

    ~Calculator() {

    }

    CAL_STATUS Put(Operator *pointerToOperator) {

        // 括弧の場合
        if (pointerToOperator->isBracket) {
            // 左括弧の場合
            if (pointerToOperator->bracket == Operator::BRACKET::LEFT_BRACKET) {
                // オペレータスタックにプッシュ
                if (operatorPointerStack.Push(pointerToOperator)) {
                    return CAL_STATUS::CAL_STATUS_SUCCESS;
                }
                else {
                    return CAL_STATUS::CAL_STATUS_STACK_IS_FULL;
                }
            }

            // 右括弧の場合
            else {
                Operator *op;

                // 左括弧が出るまで繰り返す
                while (operatorPointerStack.Pop(&op)) {

                    // 左括弧をポップした場合
                    if (op->isBracket && (op->bracket == Operator::BRACKET::LEFT_BRACKET)) {
                        return CAL_STATUS::CAL_STATUS_SUCCESS;
                    }

                    else if (!op->isBracket) {
                        Fraction fracA, fracB;

                        if (operandStack.Pop(&fracB) && operandStack.Pop(&fracA)) {
                            Fraction fracC = op->Operate(fracA, fracB);
                            if (operandStack.Push(fracC)) {
                                // 何もしない. 
                                // 次のループへ進む
                            }
                            else {
                                return CAL_STATUS::CAL_STATUS_STACK_IS_FULL;
                            }
                        }

                        else {
                            return CAL_STATUS::CAL_STATUS_OPERAND_IS_NOT_ENOUGH;
                        }
                    }

                }

                // 左括弧が出るはずだが, 出ずにすべての演算子がスタックから取り出された.
                // これは, 右括弧と左括弧があっていないからである.
                return CAL_STATUS::CAL_STATUS_MISMATCHED_PARENTHESES;


            } // End 右括弧の場合

        } // End 括弧の場合

        // 括弧以外の演算子の場合
        else {
            while (true) {
                Operator *op;
                // オペレータスタックにオペレータop2があり, op1が左結合性で,
                // かつ優先順位がop2と等しいか低い場合
                if (operatorPointerStack.Peek(&op)
                    && (pointerToOperator->associativity == Operator::ASSOCIATIVITY::LEFT)
                    && (pointerToOperator->precedence <= op->precedence)) {
                    // オペレータを取り出す
                    operatorPointerStack.Pop(&op);

                    Fraction fracA, fracB;

                    // オペランドスタックから値を二つ取り出し演算する.
                    if (operandStack.Pop(&fracB) && operandStack.Pop(&fracA)) {
                        Fraction fracC = op->Operate(fracA, fracB);
                        if (operandStack.Push(fracC)) {
                            
                        }
                        else {
                            return CAL_STATUS::CAL_STATUS_STACK_IS_FULL;
                        }
                    }

                    // あるはずの二つのオペランドがない場合
                    else {
                        return CAL_STATUS::CAL_STATUS_OPERAND_IS_NOT_ENOUGH;
                    }
                }

                else {
                    break;
                }
            }

            if (operatorPointerStack.Push(pointerToOperator)) {
                return CAL_STATUS::CAL_STATUS_SUCCESS;
            }
            else {
                return CAL_STATUS::CAL_STATUS_STACK_IS_FULL;
            }
        } // End 括弧以外の演算子の場合

        return CAL_STATUS::CAL_STATUS_SOMETHING_ERROR;
    }

    CAL_STATUS Put(Fraction &frac) {
        if (operandStack.Push(frac)) {
            return CAL_STATUS::CAL_STATUS_SUCCESS;
        }

        return CAL_STATUS::CAL_STATUS_STACK_IS_FULL;

    }

    CAL_STATUS Put(Fraction &&frac) {
        return Put(frac);
    }

    CAL_STATUS Compute() {

        Operator *op;
        while (operatorPointerStack.Pop(&op)) {
            Fraction fracA, fracB;

            if (operandStack.Pop(&fracB) && operandStack.Pop(&fracA)) {
                Fraction fracC = op->Operate(fracA, fracB);
                
                // このループ内では, オペランドのスタックは減る方向になるので,
                // スタックがフルであるか調べる必要はない.
                operandStack.Push(fracC);
            }
            else {
                return CAL_STATUS::CAL_STATUS_OPERAND_IS_NOT_ENOUGH;
            }

        }

        return CAL_STATUS::CAL_STATUS_STACK_IS_FULL;
    }


    CAL_STATUS TopOfOperandStack(Fraction *frac) {
        if (operandStack.Peek(frac)) {
            return CAL_STATUS::CAL_STATUS_SUCCESS;
        }

        return CAL_STATUS::CAL_STATUS_STACK_IS_EMPTY;
    }

    void ClearOperandStack() {
        operandStack.Clear();
    }

};


#endif