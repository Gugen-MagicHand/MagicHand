

#ifndef LITERAL_FRACTION_H
#define LITERAL_FRACTION_H


#include "Fraction.h"
#include "Literal.h"

//#include <iostream>

class LiteralFraction {
public:

    static const int SIGNIFICAND_MAX_COUNT = 10;

private:

    unsigned char significand[SIGNIFICAND_MAX_COUNT] = { 0 };
    int significandCount = 0;

    int negativeExponent = 0;


public:


    Fraction ToFraction() {

    }

    void Put(LITERAL lit) {

    }


    void BackSpace() {

    }




};


#endif