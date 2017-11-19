

#ifndef LITERAL_FRACTION_H
#define LITERAL_FRACTION_H


#include "Fraction.h"
#include "Literal.h"


class LiteralFraction {
public:

    static const int SIGNIFICAND_MAX_COUNT = 10;

private:

    unsigned char significand[SIGNIFICAND_MAX_COUNT] = { 0 };
    int significandCount = 0;

    int negativeExponent = 0;

    bool isPointed = false;


public:

    bool IsPointed() { return isPointed; }
    int NegativeExponent() { return negativeExponent; }
    int SignificandCount() { return significandCount; }
    int Length() { return isPointed ? significandCount + 1 : significandCount; }

    Fraction ToFraction() {

        int numer = 0;
        int base = 1;
        for (int i = 0; i < significandCount; i++) {
            numer += significand[significandCount - i - 1] * base;

            base *= 10;
        }

        int denom = 1;
        for (int i = 0; i < negativeExponent; i++) {
            denom *= 10;
        }

        return Fraction(numer, denom);
    }


    //
    // 文頭に'.'を置くことはできません.
    // 
    //
    // @return: 
    //  trueのとき, 正常に文字が追加された.
    //  falseのときは, 正常に文字が追加されなかった.
    //
    bool Put(LITERAL lit) {
        if (lit >= LITERAL::LITERAL_0 && lit <= LITERAL::LITERAL_9) {

            // 数字の場合

            if (significandCount < SIGNIFICAND_MAX_COUNT) {
                // 仮数部の桁数に入っている場合

                if (significandCount >= 1 && significand[significandCount - 1] == LITERAL::LITERAL_0) {
                    // 直前に0がある場合

                    if (!isPointed) {
                        // 直前に0があり, 小数点が打たれていない場合,
                        // そのあとのいかなる数も受け付けない

                        return false;
                    }
                }

                if (isPointed) {
                    negativeExponent++;
                }

                significand[significandCount++] = lit - LITERAL::LITERAL_0;


                return true;
            }

        }
        else if (lit == LITERAL::LITERAL_DOT) {
            // 点の場合

            if (significandCount == 0) {
                // 文頭の'.'は受け付けない.

                return false;
            }

            if (!isPointed) {
                isPointed = true;

                return true;
            }
        }


        return false;
    }

    //
    // @return: 
    //  trueのとき, 正常に文字が削除できた.
    //  falseのときは, 正常に文字が削除できなかった.
    //
    bool BackSpace() {

        if (isPointed) {

            negativeExponent--;

            if (negativeExponent < 0) {

                negativeExponent = 0;
                isPointed = false;

                return true;
            }
        }

        if (significandCount > 0) {

            significandCount--;

            return true;
        }
        

        return false;
    }

    void Clear() {
        isPointed = false;
        negativeExponent = 0;
        significandCount = 0;
    }




};


#endif