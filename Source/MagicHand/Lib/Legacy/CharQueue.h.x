

#ifndef CHAR_QUEUE_H
#define CHAR_QUEUE_H

#include "Queue.h"
#include "Literal.h"

//#include <iostream>

class CharQueue : public Queue<char> {
public:
    CharQueue(int capacity) : Queue(capacity) {

    }

    bool PopBack(char *literalToPop) {

        if (count <= 0)
        {
            return false;
        }

        *literalToPop = array[(startPoint + count - 1) % capacity];

        count--;
        return true;
    }

    char& operator[] (int index)
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