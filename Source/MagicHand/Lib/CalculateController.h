#ifndef CALCULATE_CONTROLLER_H
#define CALCULATE_CONTROLLER_H


#include "Calculator.h"
#include "Literal.h"
#include "LiteralFraction.h"


class CalculateController {




public:

    enum CALCULATE_PHASE {
        CALCULATE_PHASE_FOMULA_FIRST_INPUT,
        CALCULATE_PHASE_CHILD_FOMULA_FIRST_INPUT,
        CALCULATE_PHASE_OPERATOR_INPUT,
        CALCULATE_PHASE_OPERAND_INPUT
    };


    // オペレータ一覧
    OperatorDivide operatorDivide;
    OperatorLeftBracket operatorLeftBracket;
    OperatorMinus operatorMinus;
    OperatorMultiply operatorMultiply;
    OperatorPlus operatorPlus;
    OperatorRightBracket operatorRightBracket;



    Calculator calculator;

    Operator *pointerToOperator;


    void(*onPushLiteralIntoFormula)(LITERAL lit);
    void(*onPopBackLiteralFromFormula)();


private:
    CALCULATE_PHASE phase = CALCULATE_PHASE::CALCULATE_PHASE_FOMULA_FIRST_INPUT;
    LiteralFraction literalFraction;


public:

    CALCULATE_PHASE Phase() { return phase; }

    CalculateController(int operandStackSize, int operatorPointerStackSize) : calculator(operandStackSize, operatorPointerStackSize) {

    }


    Operator *LiteralToOperatorPointer(LITERAL lit) {
        if (lit == LITERAL::LITERAL_LEFT_BRACKET) {
            return &operatorLeftBracket;
        }
        else if (lit == LITERAL::LITERAL_RIGHT_BRACKET) {
            return &operatorRightBracket;
        }
        else if (lit == LITERAL::LITERAL_MINUS) {
            return &operatorMinus;
        }
        else if (lit == LITERAL::LITERAL_PLUS) {
            return &operatorPlus;
        }
        else if (lit == LITERAL::LITERAL_MULTIPLY) {
            return &operatorMultiply;
        }
        else if (lit == LITERAL::LITERAL_DIVIDE) {
            return &operatorDivide;
        }

        return 0x00;
    }

    Calculator::CAL_STATUS Put(LITERAL lit) {
        Calculator::CAL_STATUS calStatus = Calculator::CAL_STATUS::CAL_STATUS_SUCCESS;

        switch (phase) {

            // --- 式最初の入力待機段階 -----------------------------------------------------------------------
        case CALCULATE_PHASE_FOMULA_FIRST_INPUT:


            if (LiteralIsNumeric(lit)) {
                // 数字が入力されたとき

                // 計算機を初期化する.
                calculator.ClearAllStacks();

                // 現在の文字分数に数字を書く
                if (literalFraction.Put(lit)) {
                    onPushLiteralIntoFormula(lit);
                }

                // Operand入力段階に移行する
                phase = CALCULATE_PHASE_OPERAND_INPUT;

            }
            else if (lit == LITERAL::LITERAL_MINUS ||
                lit == LITERAL::LITERAL_PLUS ||
                lit == LITERAL::LITERAL_DIVIDE ||
                lit == LITERAL::LITERAL_MULTIPLY) {

                // 作業中Operatorに代入する.
                pointerToOperator = LiteralToOperatorPointer(lit);

                // display表示
                onPushLiteralIntoFormula(lit);

                // Operator入力段階に移行する.
                phase = CALCULATE_PHASE_OPERATOR_INPUT;


            }
            else if (lit == LITERAL::LITERAL_LEFT_BRACKET) {
                // 式先頭に左括弧が来たときは, 計算機に左括弧を代入し, PHASE_CHILD_FOMULA_FIRST_INPUTに移行する.

                // 計算機のスタックを初期化する.
                calculator.ClearAllStacks();

                // 左括弧を計算機に代入する
                calStatus = calculator.Put(&operatorLeftBracket);

                // display表示
                onPushLiteralIntoFormula(lit);

                // PHASE_CHILD_FOMULA_FIRST_INPUTに移行する
                phase = CALCULATE_PHASE_CHILD_FOMULA_FIRST_INPUT;

            }

            break; // End 式最初の入力段階 ----------------------------

                   // --- 子式最初の入力段階 -----------------------------------------------------------------
        case CALCULATE_PHASE_CHILD_FOMULA_FIRST_INPUT:


            if (LiteralIsNumeric(lit)) {
                // 数字が入力されたときは, 文字分数に数字を代入してPHASE_OPERAND_INPUTに移行する.

                // 現在の文字分数に数字を書く
                if (literalFraction.Put(lit)) {

                    onPushLiteralIntoFormula(lit);

                }

                // Operand入力段階に移行する
                phase = CALCULATE_PHASE_OPERAND_INPUT;

            }
            else if (lit == LITERAL::LITERAL_MINUS ||
                lit == LITERAL::LITERAL_PLUS ||
                lit == LITERAL::LITERAL_DIVIDE ||
                lit == LITERAL::LITERAL_MULTIPLY) {

                calStatus = calculator.Put(Fraction(0));

                // 作業中Operatorに代入する.
                pointerToOperator = LiteralToOperatorPointer(lit);

                // display表示
                onPushLiteralIntoFormula(lit);

                // Operator入力段階に移行する.
                phase = CALCULATE_PHASE_OPERATOR_INPUT;


            }
            else if (lit == LITERAL::LITERAL_LEFT_BRACKET) {
                // 式先頭に左括弧が来たときは, 計算機に左括弧を代入し, PHASE_CHILD_FOMULA_FIRST_INPUTに移行する.


                // 左括弧を計算機に代入する
                calStatus = calculator.Put(&operatorLeftBracket);

                // display表示
                onPushLiteralIntoFormula(lit);

                // PHASE_CHILD_FOMULA_FIRST_INPUTに移行する
                phase = CALCULATE_PHASE_CHILD_FOMULA_FIRST_INPUT;

            }




            break; // End 子式最初の入力段階 --------------------------

                   // --- Operand入力段階 --------------------------------------------------------------------
        case CALCULATE_PHASE_OPERAND_INPUT:

            if (LiteralIsNumeric(lit) || lit == LITERAL::LITERAL_DOT) {
                // 数字, もしくは小数点が来た場合


                // 文字分数に代入
                if (literalFraction.Put(lit)) {


                    onPushLiteralIntoFormula(lit);
                }
            }
            else if (lit == LITERAL::LITERAL_DIVIDE ||
                lit == LITERAL::LITERAL_MINUS ||
                lit == LITERAL::LITERAL_PLUS ||
                lit == LITERAL::LITERAL_MULTIPLY) {

                // 四則演算子が来た場合, 現在の文字分数をoperandとして計算機に代入する.
                // その後, PHASE_OPERATOR_INPUTに移行する.

                // 現在のoperandを計算機に代入
                calStatus = calculator.Put(literalFraction.ToFraction());
                literalFraction.Clear();

                onPushLiteralIntoFormula(lit);

                // 現在のoperatorとして記録
                pointerToOperator = LiteralToOperatorPointer(lit);


                phase = CALCULATE_PHASE_OPERATOR_INPUT;
            }

            else if (lit == LITERAL::LITERAL_RIGHT_BRACKET) {
                // 右括弧が来た場合


                // 現在のoperandを計算機に代入
                calStatus = calculator.Put(literalFraction.ToFraction());
                literalFraction.Clear();

                calStatus = calculator.Put(&operatorRightBracket);


                onPushLiteralIntoFormula(lit);

                phase = CALCULATE_PHASE_OPERATOR_INPUT;

            }
            else if (lit == LITERAL::LITERAL_EQUAL) {


                // 現在のoperandを計算機に代入
                calStatus = calculator.Put(literalFraction.ToFraction());
                literalFraction.Clear();

                onPushLiteralIntoFormula(lit);

                calStatus = calculator.Compute();



                phase = CALCULATE_PHASE_FOMULA_FIRST_INPUT;
            }




            break; // End Operand入力段階 --------------------------------




                   // --- Operator入力段階 ---------------------------------------------------------------------
        case CALCULATE_PHASE_OPERATOR_INPUT:


            if (lit == LITERAL::LITERAL_DIVIDE ||
                lit == LITERAL::LITERAL_MINUS ||
                lit == LITERAL::LITERAL_PLUS ||
                lit == LITERAL::LITERAL_MULTIPLY) {

                // 四則演算子が来た場合, 現在のoperatorとして記録する.

                pointerToOperator = LiteralToOperatorPointer(lit);

                // 前にあるoperatorを消去(一文字分)してから, 新しく描く

                onPopBackLiteralFromFormula();
                onPushLiteralIntoFormula(lit);

            }

            else if (LiteralIsNumeric(lit)) {
                // 数字が来た場合は, 現在のOperatorを決定演算子として, 計算機に代入し,
                // OperandPhaseに移行する.

                if (pointerToOperator != 0x00) {

                    // operatorを計算機に代入する.
                    calStatus = calculator.Put(pointerToOperator);

                    // 数字を文字分数に代入する.
                    if (literalFraction.Put(lit)) {


                        onPushLiteralIntoFormula(lit);

                    }

                    // phaseをOperandPhaseに移動する.
                    phase = CALCULATE_PHASE_OPERAND_INPUT;
                }
            }

            else if (lit == LITERAL::LITERAL_LEFT_BRACKET) {
                // 左括弧が来た場合は, 現在のoperatorを決定演算子として,
                // 計算機に代入する. その後, PHASE_CHILD_FOMULA_FIRST_INPUTに移行する.

                if (pointerToOperator != 0x00) {

                    // operatorを計算機に代入する.
                    calStatus = calculator.Put(pointerToOperator);

                    onPushLiteralIntoFormula(lit);

                    // Left bracketを計算機に代入する.
                    calStatus = calculator.Put(&operatorLeftBracket);

                    // PHASE_CHILD_FOMULA_FIRST_INPUTに移行
                    phase = CALCULATE_PHASE_CHILD_FOMULA_FIRST_INPUT;
                }
            }
            else if (lit == LITERAL::LITERAL_EQUAL) {

                onPushLiteralIntoFormula(lit);

                calStatus = calculator.Compute();



                phase = CALCULATE_PHASE_FOMULA_FIRST_INPUT;
            }
            break;
            // End operator入力段階 ------------------------------
        }

        return calStatus;
    }


};


#endif