#ifndef RESULT_ASSEMBLER_H
#define RESULT_ASSEMBLER_H

#include "Stroke.h"
#include "Fraction.h"
#include "Calculator.h"

#include "Literal.h"



class StrokeAssembler {
public:
    static const int LITERAL_QUEUE_CAPACITY = 5;

    static const int WAITING_ASSEMBLED_STROKES_MAX_COUNT = 3;


    // 組み立て結果文字待ち行列
    Queue<LITERAL> literalQueue;


private:

    //
    int waitingAssembledStrokesCount = 0;


    // 途中経過を保持する用stroke配列
    STROKE waitingAssembledStrokes[WAITING_ASSEMBLED_STROKES_MAX_COUNT] = {};



public:

    StrokeAssembler() : literalQueue(LITERAL_QUEUE_CAPACITY) {

    }

    //
    //
    void Assemble(STROKE strokeToAssemble) {
        //Serial.println("w");
        // --- ストローク1回目 ----------------------------------------------------
        if (waitingAssembledStrokesCount == 0) {
            AssembleOneStroke(strokeToAssemble);

        } // End ストローク一回目 -------------------------------------------------------

          // --- ストローク2回目以上, MaxCountより小さい ----------------------------------------------------------
        else if (waitingAssembledStrokesCount >= 1 && waitingAssembledStrokesCount < WAITING_ASSEMBLED_STROKES_MAX_COUNT) {

            STROKE prevStroke = waitingAssembledStrokes[waitingAssembledStrokesCount - 1];

            //Serial.println(prevStroke);
            //Serial.println(strokeToAssemble);

            if (IsAssociatedWith(prevStroke, strokeToAssemble)) {
                // 直前のストロークと関係がある場合
                waitingAssembledStrokes[waitingAssembledStrokesCount++] = strokeToAssemble;
            }
            else {
                // 直前のストロークと関係がない場合

                // まず, 過去のストロークを組み立てる.
                AssembleFromWaitingAssembledStrokes();

                // 次に, 今のストロークを組み立てる.
                AssembleOneStroke(strokeToAssemble);
            }
        } // End ストローク2回目以上,  MaxCountより小さい--------------------------

        else {

            LiteralQueuePush(LITERAL::LITERAL_UNKNOWN);
            waitingAssembledStrokesCount = 0;
        }
    }






private:
    void LiteralQueuePush(LITERAL lieralToPush) {
        while (!literalQueue.Push(lieralToPush)) {
            // literalQueueにPushできるまで, Popする.
            LITERAL temp;
            literalQueue.Pop(&temp);
        }

        return;
    }

    /*
    switch (stroke) {
    case STROKE::STROKE_SPACE:
    case STROKE::STROKE_DOT:
    case STROKE::STROKE_LEFT_BRACKET:
    case STROKE::STROKE_RIGHT_BRACKET:
    case STROKE::STROKE_0:
    case STROKE::STROKE_2:
    case STROKE::STROKE_3:
    case STROKE::STROKE_PART_OF_4:
    case STROKE::STROKE_PART_OF_5:
    case STROKE::STROKE_6:
    case STROKE::STROKE_PART_OF_7:
    case STROKE::STROKE_8:
    case STROKE::STROKE_9:
    case STROKE::STROKE_VERTICAL_LINE:
    case STROKE::STROKE_HORIZONTAL_LINE:
    case STROKE::STROKE_SLASH:
    case STROKE::STROKE_BACK_SLASH:
    }
    */

    void AssembleOneStroke(STROKE oneStrokeToAssemble) {
        switch (oneStrokeToAssemble) {
            // --- 一つの文字に対応しているOneStroke ----------------------
        case STROKE::STROKE_SPACE:
            break;

        case STROKE::STROKE_LEFT_BRACKET:
            LiteralQueuePush(LITERAL::LITERAL_LEFT_BRACKET);
            break;

        case STROKE::STROKE_RIGHT_BRACKET:
            LiteralQueuePush(LITERAL::LITERAL_RIGHT_BRACKET);
            break;

        case STROKE::STROKE_0:
            LiteralQueuePush(LITERAL::LITERAL_0);
            break;

        case STROKE::STROKE_2:
            LiteralQueuePush(LITERAL::LITERAL_2);
            break;

        case STROKE::STROKE_3:
            LiteralQueuePush(LITERAL::LITERAL_3);
            break;

        case STROKE::STROKE_6:
            LiteralQueuePush(LITERAL::LITERAL_6);
            break;

        case STROKE::STROKE_8:
            LiteralQueuePush(LITERAL::LITERAL_8);
            break;

        case STROKE::STROKE_9:
            LiteralQueuePush(LITERAL::LITERAL_9);
            break;

        case STROKE::STROKE_DOT:
            LiteralQueuePush(LITERAL::LITERAL_DOT);
            break;


            // End 一つの文字に対応しているOneStroke ---------------

            // --- ストローク一回では判断できない文字 ------------------
        case STROKE::STROKE_VERTICAL_LINE:
        case STROKE::STROKE_HORIZONTAL_LINE:
        case STROKE::STROKE_SLASH:
        case STROKE::STROKE_BACK_SLASH:
        case STROKE::STROKE_PART_OF_4:
        case STROKE::STROKE_PART_OF_5:
        case STROKE::STROKE_PART_OF_7:
            waitingAssembledStrokes[waitingAssembledStrokesCount++] = oneStrokeToAssemble;
            break;
            // End ストローク一回では判断できない文字 ----------------

        default:
            LiteralQueuePush(LITERAL::LITERAL_UNKNOWN);
            break;
        }
    }

    void AssembleFromWaitingAssembledStrokes() {
        if (waitingAssembledStrokesCount == 1) {
            STROKE stroke = waitingAssembledStrokes[0];

            switch (stroke) {
            case STROKE::STROKE_DOT:
                LiteralQueuePush(LITERAL::LITERAL_DOT);
                break;
            case STROKE::STROKE_LEFT_BRACKET:
                LiteralQueuePush(LITERAL::LITERAL_LEFT_BRACKET);
                break;
            case STROKE::STROKE_RIGHT_BRACKET:
                LiteralQueuePush(LITERAL::LITERAL_RIGHT_BRACKET);
                break;
            case STROKE::STROKE_0:
                LiteralQueuePush(LITERAL::LITERAL_0);
                break;
            case STROKE::STROKE_VERTICAL_LINE:
                LiteralQueuePush(LITERAL::LITERAL_1);
                break;
            case STROKE::STROKE_2:
                LiteralQueuePush(LITERAL::LITERAL_2);
                break;
            case STROKE::STROKE_3:
                LiteralQueuePush(LITERAL::LITERAL_3);
                break;
            case STROKE::STROKE_6:
                LiteralQueuePush(LITERAL::LITERAL_6);
                break;
            case STROKE::STROKE_8:
                LiteralQueuePush(LITERAL::LITERAL_8);
                break;
            case STROKE::STROKE_9:
                LiteralQueuePush(LITERAL::LITERAL_9);
                break;
            case STROKE::STROKE_HORIZONTAL_LINE:
                LiteralQueuePush(LITERAL::LITERAL_MINUS);
                break;
            case STROKE::STROKE_SLASH:
                LiteralQueuePush(LITERAL::LITERAL_DIVIDE);
                break;
            default:
                LiteralQueuePush(LITERAL::LITERAL_UNKNOWN);
                break;
            }


        }
        else if (waitingAssembledStrokesCount == 2) {
            STROKE prevStroke = waitingAssembledStrokes[0];
            STROKE stroke = waitingAssembledStrokes[1];
            /*
            Serial.println("T");
            Serial.println(prevStroke);
            Serial.println(stroke);
            */
            if (prevStroke == STROKE::STROKE_PART_OF_4 && stroke == STROKE::STROKE_VERTICAL_LINE) {
                LiteralQueuePush(LITERAL::LITERAL_4);
            }
            else if (prevStroke == STROKE::STROKE_PART_OF_5 && stroke == STROKE::STROKE_HORIZONTAL_LINE) {
                LiteralQueuePush(LITERAL::LITERAL_5);
            }
            else if (prevStroke == STROKE::STROKE_HORIZONTAL_LINE && stroke == STROKE::STROKE_PART_OF_5) {
                LiteralQueuePush(LITERAL::LITERAL_5);
            }
            else if (prevStroke == STROKE::STROKE_VERTICAL_LINE && stroke == STROKE::STROKE_PART_OF_7) {
                LiteralQueuePush(LITERAL::LITERAL_7);
            }
            else if (prevStroke == STROKE::STROKE_SLASH && stroke == STROKE::STROKE_BACK_SLASH) {
                LiteralQueuePush(LITERAL::LITERAL_MULTIPLY);
            }
            else if (prevStroke == STROKE::STROKE_BACK_SLASH && stroke == STROKE::STROKE_SLASH) {
                LiteralQueuePush(LITERAL::LITERAL_MULTIPLY);
            }
            else if (prevStroke == STROKE::STROKE_HORIZONTAL_LINE && stroke == STROKE::STROKE_VERTICAL_LINE) {
                LiteralQueuePush(LITERAL::LITERAL_PLUS);
            }
            else if (prevStroke == STROKE::STROKE_VERTICAL_LINE && stroke == STROKE::STROKE_HORIZONTAL_LINE) {
                LiteralQueuePush(LITERAL::LITERAL_PLUS);
            }
            else if (prevStroke == STROKE::STROKE_HORIZONTAL_LINE && stroke == STROKE::STROKE_HORIZONTAL_LINE) {
                LiteralQueuePush(LITERAL::LITERAL_EQUAL);
            }
            else {
                LiteralQueuePush(LITERAL::LITERAL_UNKNOWN);
            }
        }
        else if (waitingAssembledStrokesCount == 3) {
            STROKE prevPrevStroke = waitingAssembledStrokes[0];
            STROKE prevStroke = waitingAssembledStrokes[1];
            STROKE stroke = waitingAssembledStrokes[2];

            if (prevPrevStroke == STROKE::STROKE_HORIZONTAL_LINE && prevStroke == STROKE::STROKE_DOT && stroke == STROKE::STROKE_DOT) {
                LiteralQueuePush(LITERAL::LITERAL_DIVIDE);
            }
            else if (prevPrevStroke == STROKE::STROKE_DOT && prevStroke == STROKE::STROKE_DOT && stroke == STROKE::STROKE_HORIZONTAL_LINE) {
                LiteralQueuePush(LITERAL::LITERAL_DIVIDE);
            }
            else {
                LiteralQueuePush(LITERAL::LITERAL_UNKNOWN);
            }
        }

        waitingAssembledStrokesCount = 0;
    }

    bool IsAssociatedWith(STROKE leftStroke, STROKE rightStroke) {
        switch (leftStroke) {
        case STROKE::STROKE_SPACE:
            return false;

        case STROKE::STROKE_DOT:
            switch (rightStroke) {
            case STROKE::STROKE_DOT:
            case STROKE::STROKE_HORIZONTAL_LINE:
                return true;

            default:
                return false;

            }
            break;

        case STROKE::STROKE_LEFT_BRACKET:
            return false;

        case STROKE::STROKE_RIGHT_BRACKET:
            return false;

        case STROKE::STROKE_0:
            return false;

        case STROKE::STROKE_2:
            return false;

        case STROKE::STROKE_3:
            return false;

        case STROKE::STROKE_PART_OF_4:
            switch (rightStroke) {
            case STROKE::STROKE_VERTICAL_LINE:
                return true;

            default:
                return false;
            }
            break;

        case STROKE::STROKE_PART_OF_5:
            switch (rightStroke) {
            case STROKE::STROKE_HORIZONTAL_LINE:
                return true;

            default:
                return false;
            }
            break;

        case STROKE::STROKE_6:
            return false;

        case STROKE::STROKE_PART_OF_7:
            return false;

        case STROKE::STROKE_8:
            return false;

        case STROKE::STROKE_9:
            return false;

        case STROKE::STROKE_VERTICAL_LINE:
            switch (rightStroke) {
            case STROKE::STROKE_HORIZONTAL_LINE:
            case STROKE::STROKE_PART_OF_7:
                return true;

            default:
                return false;
            }
            break;

        case STROKE::STROKE_HORIZONTAL_LINE:
            switch (rightStroke) {
            case STROKE::STROKE_PART_OF_5:
            case STROKE::STROKE_HORIZONTAL_LINE:
            case STROKE::STROKE_VERTICAL_LINE:
            case STROKE::STROKE_DOT:
                return true;

            default:
                return false;
            }
            break;

        case STROKE::STROKE_SLASH:
            switch (rightStroke) {
            case STROKE::STROKE_BACK_SLASH:
                return true;

            default:
                return false;
            }
            break;

        case STROKE::STROKE_BACK_SLASH:
            switch (rightStroke) {
            case STROKE::STROKE_SLASH:
                return true;

            default:
                return false;
            }
            break;
        }

        return false;
    }

};

#endif
