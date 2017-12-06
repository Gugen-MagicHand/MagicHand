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


    // �I�y���[�^�ꗗ
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

            // --- ���ŏ��̓��͑ҋ@�i�K -----------------------------------------------------------------------
        case CALCULATE_PHASE_FOMULA_FIRST_INPUT:


            if (LiteralIsNumeric(lit)) {
                // ���������͂��ꂽ�Ƃ�

                // �v�Z�@������������.
                calculator.ClearAllStacks();

                // ���݂̕��������ɐ���������
                if (literalFraction.Put(lit)) {
                    onPushLiteralIntoFormula(lit);
                }

                // Operand���͒i�K�Ɉڍs����
                phase = CALCULATE_PHASE_OPERAND_INPUT;

            }
            else if (lit == LITERAL::LITERAL_MINUS ||
                lit == LITERAL::LITERAL_PLUS ||
                lit == LITERAL::LITERAL_DIVIDE ||
                lit == LITERAL::LITERAL_MULTIPLY) {

                // ��ƒ�Operator�ɑ������.
                pointerToOperator = LiteralToOperatorPointer(lit);

                // display�\��
                onPushLiteralIntoFormula(lit);

                // Operator���͒i�K�Ɉڍs����.
                phase = CALCULATE_PHASE_OPERATOR_INPUT;


            }
            else if (lit == LITERAL::LITERAL_LEFT_BRACKET) {
                // ���擪�ɍ����ʂ������Ƃ���, �v�Z�@�ɍ����ʂ�����, PHASE_CHILD_FOMULA_FIRST_INPUT�Ɉڍs����.

                // �v�Z�@�̃X�^�b�N������������.
                calculator.ClearAllStacks();

                // �����ʂ��v�Z�@�ɑ������
                calStatus = calculator.Put(&operatorLeftBracket);

                // display�\��
                onPushLiteralIntoFormula(lit);

                // PHASE_CHILD_FOMULA_FIRST_INPUT�Ɉڍs����
                phase = CALCULATE_PHASE_CHILD_FOMULA_FIRST_INPUT;

            }

            break; // End ���ŏ��̓��͒i�K ----------------------------

                   // --- �q���ŏ��̓��͒i�K -----------------------------------------------------------------
        case CALCULATE_PHASE_CHILD_FOMULA_FIRST_INPUT:


            if (LiteralIsNumeric(lit)) {
                // ���������͂��ꂽ�Ƃ���, ���������ɐ�����������PHASE_OPERAND_INPUT�Ɉڍs����.

                // ���݂̕��������ɐ���������
                if (literalFraction.Put(lit)) {

                    onPushLiteralIntoFormula(lit);

                }

                // Operand���͒i�K�Ɉڍs����
                phase = CALCULATE_PHASE_OPERAND_INPUT;

            }
            else if (lit == LITERAL::LITERAL_MINUS ||
                lit == LITERAL::LITERAL_PLUS ||
                lit == LITERAL::LITERAL_DIVIDE ||
                lit == LITERAL::LITERAL_MULTIPLY) {

                calStatus = calculator.Put(Fraction(0));

                // ��ƒ�Operator�ɑ������.
                pointerToOperator = LiteralToOperatorPointer(lit);

                // display�\��
                onPushLiteralIntoFormula(lit);

                // Operator���͒i�K�Ɉڍs����.
                phase = CALCULATE_PHASE_OPERATOR_INPUT;


            }
            else if (lit == LITERAL::LITERAL_LEFT_BRACKET) {
                // ���擪�ɍ����ʂ������Ƃ���, �v�Z�@�ɍ����ʂ�����, PHASE_CHILD_FOMULA_FIRST_INPUT�Ɉڍs����.


                // �����ʂ��v�Z�@�ɑ������
                calStatus = calculator.Put(&operatorLeftBracket);

                // display�\��
                onPushLiteralIntoFormula(lit);

                // PHASE_CHILD_FOMULA_FIRST_INPUT�Ɉڍs����
                phase = CALCULATE_PHASE_CHILD_FOMULA_FIRST_INPUT;

            }




            break; // End �q���ŏ��̓��͒i�K --------------------------

                   // --- Operand���͒i�K --------------------------------------------------------------------
        case CALCULATE_PHASE_OPERAND_INPUT:

            if (LiteralIsNumeric(lit) || lit == LITERAL::LITERAL_DOT) {
                // ����, �������͏����_�������ꍇ


                // ���������ɑ��
                if (literalFraction.Put(lit)) {


                    onPushLiteralIntoFormula(lit);
                }
            }
            else if (lit == LITERAL::LITERAL_DIVIDE ||
                lit == LITERAL::LITERAL_MINUS ||
                lit == LITERAL::LITERAL_PLUS ||
                lit == LITERAL::LITERAL_MULTIPLY) {

                // �l�����Z�q�������ꍇ, ���݂̕���������operand�Ƃ��Čv�Z�@�ɑ������.
                // ���̌�, PHASE_OPERATOR_INPUT�Ɉڍs����.

                // ���݂�operand���v�Z�@�ɑ��
                calStatus = calculator.Put(literalFraction.ToFraction());
                literalFraction.Clear();

                onPushLiteralIntoFormula(lit);

                // ���݂�operator�Ƃ��ċL�^
                pointerToOperator = LiteralToOperatorPointer(lit);


                phase = CALCULATE_PHASE_OPERATOR_INPUT;
            }

            else if (lit == LITERAL::LITERAL_RIGHT_BRACKET) {
                // �E���ʂ������ꍇ


                // ���݂�operand���v�Z�@�ɑ��
                calStatus = calculator.Put(literalFraction.ToFraction());
                literalFraction.Clear();

                calStatus = calculator.Put(&operatorRightBracket);


                onPushLiteralIntoFormula(lit);

                phase = CALCULATE_PHASE_OPERATOR_INPUT;

            }
            else if (lit == LITERAL::LITERAL_EQUAL) {


                // ���݂�operand���v�Z�@�ɑ��
                calStatus = calculator.Put(literalFraction.ToFraction());
                literalFraction.Clear();

                onPushLiteralIntoFormula(lit);

                calStatus = calculator.Compute();



                phase = CALCULATE_PHASE_FOMULA_FIRST_INPUT;
            }




            break; // End Operand���͒i�K --------------------------------




                   // --- Operator���͒i�K ---------------------------------------------------------------------
        case CALCULATE_PHASE_OPERATOR_INPUT:


            if (lit == LITERAL::LITERAL_DIVIDE ||
                lit == LITERAL::LITERAL_MINUS ||
                lit == LITERAL::LITERAL_PLUS ||
                lit == LITERAL::LITERAL_MULTIPLY) {

                // �l�����Z�q�������ꍇ, ���݂�operator�Ƃ��ċL�^����.

                pointerToOperator = LiteralToOperatorPointer(lit);

                // �O�ɂ���operator������(�ꕶ����)���Ă���, �V�����`��

                onPopBackLiteralFromFormula();
                onPushLiteralIntoFormula(lit);

            }

            else if (LiteralIsNumeric(lit)) {
                // �����������ꍇ��, ���݂�Operator�����艉�Z�q�Ƃ���, �v�Z�@�ɑ����,
                // OperandPhase�Ɉڍs����.

                if (pointerToOperator != 0x00) {

                    // operator���v�Z�@�ɑ������.
                    calStatus = calculator.Put(pointerToOperator);

                    // �����𕶎������ɑ������.
                    if (literalFraction.Put(lit)) {


                        onPushLiteralIntoFormula(lit);

                    }

                    // phase��OperandPhase�Ɉړ�����.
                    phase = CALCULATE_PHASE_OPERAND_INPUT;
                }
            }

            else if (lit == LITERAL::LITERAL_LEFT_BRACKET) {
                // �����ʂ������ꍇ��, ���݂�operator�����艉�Z�q�Ƃ���,
                // �v�Z�@�ɑ������. ���̌�, PHASE_CHILD_FOMULA_FIRST_INPUT�Ɉڍs����.

                if (pointerToOperator != 0x00) {

                    // operator���v�Z�@�ɑ������.
                    calStatus = calculator.Put(pointerToOperator);

                    onPushLiteralIntoFormula(lit);

                    // Left bracket���v�Z�@�ɑ������.
                    calStatus = calculator.Put(&operatorLeftBracket);

                    // PHASE_CHILD_FOMULA_FIRST_INPUT�Ɉڍs
                    phase = CALCULATE_PHASE_CHILD_FOMULA_FIRST_INPUT;
                }
            }
            else if (lit == LITERAL::LITERAL_EQUAL) {

                onPushLiteralIntoFormula(lit);

                calStatus = calculator.Compute();



                phase = CALCULATE_PHASE_FOMULA_FIRST_INPUT;
            }
            break;
            // End operator���͒i�K ------------------------------
        }

        return calStatus;
    }


};


#endif