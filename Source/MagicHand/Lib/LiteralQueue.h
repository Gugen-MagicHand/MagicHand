

#ifndef LITERAL_QUEUE_H
#define LITERAL_QUEUE_H

#include "Queue.h"
#include "Literal.h"

//#include <iostream>

class LiteralQueue : public Queue<LITERAL> {
public:
    LiteralQueue(int capacity) : Queue(capacity) {

    }

    bool PopBack(LITERAL *literalToPop) {

        if (count <= 0)
        {
            return false;
        }

        *literalToPop = array[(startPoint + count - 1) % capacity];

        count--;
        return true;
    }

    LITERAL& operator[] (int index)
    {
        /*
        if (index < 0 || index >= count)
        {
        throw std::out_of_range("[List.oprator[]] IndexOutOfRange");
        }
        */
        return array[(startPoint + index) % capacity];
    }
};


#endif