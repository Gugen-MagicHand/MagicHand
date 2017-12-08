#ifndef CALCULATOR_DISPLAY_H
#define CALCULAtOR_DISPLAY_H


#include <TFT.h>
//#include "LiteralQueue.h"
#include "CharQueue.h"
#include "Stroke.h"

#include "Fraction.h"
#include "Calculator.h"
#include "CalculateController.h"
//#include "CalculatorPhase.h"

class CalculatorDisplay : public TFT
{

public:

    int canvasQueueCount;
    bool isFraction;

    Fraction answerFraction;

    Calculator::CAL_STATUS calStatus;

    CalculateController::CALCULATE_PHASE calPhase;

    static const int DISPLAY_WIDTH = 160;
    static const int DISPLAY_HEIGHT = 128;

    static const int FORMULA_FIELD_WIDTH = 100;
    static const int FORMULA_FIELD_HEIGHT = 42;

    static const int ANSWER_FIELD_WIDTH = FORMULA_FIELD_WIDTH;
    static const int ANSWER_FIELD_HEIGHT = DISPLAY_HEIGHT - FORMULA_FIELD_HEIGHT - 1;

    static const int ASSEMBLER_FILED_POS_X = FORMULA_FIELD_WIDTH + 1;

    static const int ASSEMBLER_FIELD_WIDTH = DISPLAY_WIDTH - FORMULA_FIELD_WIDTH - 1;
    static const int ASSEMBLER_FIELD_HEIGHT = FORMULA_FIELD_HEIGHT;

    static const int FORMULA_CHAR_QUEUE_CAPACITY = 16;
    static const int ASSEMBLED_CHAR_QUEUE_CAPACITY = 3;

    static const int CONSOLE_FIELD_POS_X = ASSEMBLER_FILED_POS_X;



private:
    int canvasQueueCountPrev;
    bool isFractionPrev;

    Fraction answerFractionPrev;

    Calculator::CAL_STATUS calStatusPrev;

    CalculateController::CALCULATE_PHASE calPhasePrev;

    CharQueue assembledCharQueue;

    CharQueue formulaCharQueue;

    bool isFormulaUpdated;

    bool isAssembledQueueUpdated;


public:
    CalculatorDisplay(uint8_t CS, uint8_t RS, uint8_t RST) : TFT(CS, RS, RST), assembledCharQueue(ASSEMBLED_CHAR_QUEUE_CAPACITY), formulaCharQueue(FORMULA_CHAR_QUEUE_CAPACITY)
    {};


    void Begin() {

        begin();


        setRotation(3);
        background(0, 0, 0);
        stroke(255, 255, 255);

        SetFrame();

        canvasQueueCountPrev = 0;
        canvasQueueCount = 0;


        isFractionPrev = true;


        DrawDisplayMode();

        isFormulaUpdated = false;
        isAssembledQueueUpdated = false;

        calStatusPrev = Calculator::CAL_STATUS::CAL_STATUS_SOMETHING_ERROR;
        calPhasePrev = CalculateController::CALCULATE_PHASE::CALCULATE_PHASE_OPERAND_INPUT;

        answerFractionPrev.SetFraction(0, 0);

        setTextWrap(true);
    }

    void Update() {
        DrawAnswer();
        DrawConsole();
        DrawCanvasQueueCount();
        DrawDisplayMode();
        DrawFormula();
        DrawAssembledLiterals();
    }



    void FormulaCharQueuePush(char charToPush) {
        while (!formulaCharQueue.Push(charToPush))
        {
            char ch;
            formulaCharQueue.Pop(&ch);
        }

        isFormulaUpdated = true;
    }

    bool FormulaCharQueuePopBack(char *charToPop) {
        isFormulaUpdated = true;

        return formulaCharQueue.PopBack(charToPop);
    }

    void FormulaCharQueueClear() {
        formulaCharQueue.Clear();
    }


    void AssembledCharQueuePush(char charToPush) {
        while (!assembledCharQueue.Push(charToPush))
        {
            char ch;
            assembledCharQueue.Pop(&ch);
        }

        isAssembledQueueUpdated = true;
    }

    bool AssembledCharQueuePopBack(char *charToPop) {
        return assembledCharQueue.PopBack(charToPop);
        isAssembledQueueUpdated = true;
    }

private:

    void SetFrame() {
        drawFastHLine(0, FORMULA_FIELD_HEIGHT, DISPLAY_WIDTH, ST7735_WHITE);
        drawFastVLine(FORMULA_FIELD_WIDTH, 0, DISPLAY_WIDTH, ST7735_WHITE);
        drawFastHLine(101, 84, 60, ST7735_WHITE);

        text(String(F("Formula")).c_str(), 1, 1);
        text(String(F("Answer")).c_str(), 1, 43);
        for (int i = 0; i < 5; i++) {
            drawCircle(106 + 12 * i, 4, 4, ST7735_WHITE);
            drawCircle(106 + 12 * i, 14, 4, ST7735_WHITE);
        }
        text(String(F("Mode")).c_str(), 102, 44);
        text(String(F("Calculator")).c_str(), 102, 86);
        text(String(F("CalPhase")).c_str(), 102, 105);
    }


    void DrawCanvasQueueCount() {

        if (canvasQueueCount < 0) {
            canvasQueueCount = 0;
        }

        if (canvasQueueCount > 10) {
            canvasQueueCount = 10;
        }


        while (canvasQueueCountPrev != canvasQueueCount) {

            //表示している方が多い時（表示を減らす）
            if (canvasQueueCountPrev > canvasQueueCount) {
                if (canvasQueueCountPrev <= 5) {
                    fillCircle(106 + 12 * (canvasQueueCountPrev - 1), 4, 4, ST7735_BLACK);
                    drawCircle(106 + 12 * (canvasQueueCountPrev - 1), 4, 4, ST7735_WHITE);
                }
                else {
                    fillCircle(106 + 12 * (canvasQueueCountPrev - 6), 14, 4, ST7735_BLACK);
                    drawCircle(106 + 12 * (canvasQueueCountPrev - 6), 14, 4, ST7735_WHITE);
                }

                canvasQueueCountPrev--;
            }
            //表示している方が少ない時（表示を増やす）
            else {
                if (canvasQueueCountPrev < 5) {
                    fillCircle(106 + 12 * canvasQueueCountPrev, 4, 4, ST7735_WHITE);
                }
                else {
                    fillCircle(106 + 12 * (canvasQueueCountPrev - 5), 14, 4, ST7735_WHITE);
                }
                canvasQueueCountPrev++;
            }

        }
    }


    void DrawFormula() {
        if (isFormulaUpdated) {

            char str[2];
            str[1] = 0;



            fillRect(1, 16, 99, 8, ST7735_BLACK);
            for (int i = 0; i < formulaCharQueue.Count(); i++) {
                str[0] = formulaCharQueue[i];
                text(str, 1 + i * 6, 16);
            }


            isFormulaUpdated = false;
        }
    }

    void DrawAssembledLiterals() {
        if (isAssembledQueueUpdated) {


            setTextSize(2);

            char str[2];
            str[1] = 0;

            fillRect(ASSEMBLER_FILED_POS_X, 21, ASSEMBLER_FIELD_WIDTH, 16, ST7735_BLACK);


            for (int i = 0; i < assembledCharQueue.Count(); i++) {
                str[0] = assembledCharQueue[i];
                text(str, 103 + i * 20, 21);
            }


            isAssembledQueueUpdated = false;


            setTextSize(1);
        }
    }

    void DrawDisplayMode() {
        if (isFractionPrev != isFraction) {

            fillRect(104, 54, ASSEMBLER_FIELD_WIDTH, 8, ST7735_BLACK);


            if (isFraction) {
                text(String(F("Fraction")).c_str(), 104, 54);
            }
            else {
                text(String(F("Decimal")).c_str(), 104, 54);
            }

            isFractionPrev = isFraction;
        }
    }



    void DrawAnswer() {

        if ((isFraction != isFractionPrev) 
            || (answerFraction.Denom() != answerFractionPrev.Denom()) 
            || answerFraction.Numer() != answerFractionPrev.Numer()) {

            //answer画面の初期化
            fillRect(0, 50, 99, 79, ST7735_BLACK);

            String bottomStr = "";
            String topStr = "";



            if (answerFraction.Denom() == 0 && answerFraction.Numer() == 0) {
                // 分母と分子がともに0の場合はNaN

                topStr = "NaN";
            }

            else if (answerFraction.Denom() == 0 && answerFraction.Numer() > 0) {
                // 分母が0で分子が正の整数(0を含まない)の場合は+Inf
                topStr = "INF";
            }

            else if (answerFraction.Denom() == 0 && answerFraction.Numer() < 0) {
                // 分母が0で分子が負の整数(0を含まない)の場合は-Inf
                topStr = "-INF";
            }

            else if (answerFraction.Numer() == 0) {
                topStr = "0";
            }

            else if (answerFraction.Denom() == 1) {
                topStr = String(answerFraction.Numer());
            }

            else {

                if (isFraction) {
                    topStr = String(answerFraction.Numer());
                    bottomStr = String(answerFraction.Denom());
                }
                else {
                    topStr = String(answerFraction.ToDouble());
                }
            }




            // 分母が1の時
            if (bottomStr == "") {

                if (topStr.length() < 6) {

                    setTextSize(3);
                }
                else {

                    setTextSize(2);
                }

                text(topStr.c_str(), 10, 55);
            }


            else {
                setTextSize(2);

                text(topStr.c_str(), 10, 70);
                text(bottomStr.c_str(), 10, 92);

                int maxLength = max(topStr.length(), bottomStr.length());
                drawFastHLine(10, 90, 12 * maxLength, ST7735_WHITE);
            }

            answerFractionPrev = answerFraction;

            setTextSize(1);

        }

    }

    void DrawConsole() {

        if (calStatus != calStatusPrev) {


            //画面の初期化
            fillRect(102, 94, 59, 9, ST7735_BLACK);

            String calStatusStr;

            switch (calStatus) {
            case Calculator::CAL_STATUS::CAL_STATUS_MISMATCHED_PARENTHESES:
                calStatusStr = String(F("MIS_PAREN"));
                stroke(0, 0, 255);
                break;

            case Calculator::CAL_STATUS::CAL_STATUS_OPERAND_IS_NOT_ENOUGH:
                calStatusStr = String(F("OPRND_NE"));
                stroke(0, 0, 255);

                break;

            case Calculator::CAL_STATUS::CAL_STATUS_SOMETHING_ERROR:
                calStatusStr = String(F("SOME_ERR"));
                stroke(0, 0, 255);

                break;

            case Calculator::CAL_STATUS::CAL_STATUS_STACK_IS_EMPTY:
                calStatusStr = String(F("STACK_EMP"));
                stroke(0, 0, 255);

                break;

            case Calculator::CAL_STATUS::CAL_STATUS_STACK_IS_FULL:
                calStatusStr = String(F("STACK_FUL"));
                stroke(0, 0, 255);

                break;

            case Calculator::CAL_STATUS::CAL_STATUS_SUCCESS:
                calStatusStr = String(F("SUCCESS"));
                stroke(0, 255, 0);

                break;

            default:

                calStatusStr = String(F("?"));
                break;
            }

            text(calStatusStr.c_str(), 102, 96);

            stroke(255, 255, 255);

            calStatusPrev = calStatus;
        }

        if (calPhase != calPhasePrev) {
            fillRect(102, 113, 59, 9, ST7735_BLACK);

            String phaseStr;

            switch (calPhase) {
            case CalculateController::CALCULATE_PHASE::CALCULATE_PHASE_CHILD_FOMULA_FIRST_INPUT:
                phaseStr = String(F("CFIR_IN"));
                stroke(0, 255, 0);
                break;

            case CalculateController::CALCULATE_PHASE::CALCULATE_PHASE_FOMULA_FIRST_INPUT:
                phaseStr = String(F("FIR_IN"));
                stroke(0, 255, 0);
                break;

            case CalculateController::CALCULATE_PHASE::CALCULATE_PHASE_OPERAND_INPUT:
                phaseStr = String(F("OPRND_IN"));
                stroke(0, 255, 0);
                break;

            case CalculateController::CALCULATE_PHASE::CALCULATE_PHASE_OPERATOR_INPUT:
                phaseStr = String(F("OPRTOR_IN"));
                stroke(0, 255, 0);
                break;

            default:
                phaseStr = String(F("?"));
                break;
            }


            text(phaseStr.c_str(), 102, 113);

            stroke(255, 255, 255);

            calPhasePrev = calPhase;


        }
    }


};






#endif // !CALCULATOR_DISPLAY_H
