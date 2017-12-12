

#ifndef ACCESSIBLE_QUEUE_H
#define ACCESSIBLE_QUEUE_H

#include "Queue.h"


template<typename TYPE>
class AccessibleQueue : public Queue<TYPE> {
public:

    AccessibleQueue(int capacity) : Queue<TYPE>(capacity) {

    }

    bool PopBack(TYPE *itemToPop) {

        if (this->count <= 0)
        {
            return false;
        }

        *itemToPop = this->array[(this->startPoint + this->count - 1) % this->capacity];

        this->count--;
        return true;
    }

    TYPE& operator[] (int index)
    {
        /*
        if (index < 0 || index >= count)
        {
        throw std::out_of_range("[List.oprator[]] IndexOutOfRange");
        }
        */
        return this->array[(this->startPoint + index) % this->capacity];
    }
};


#endif