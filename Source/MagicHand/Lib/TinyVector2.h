#ifndef TINY_VECTOR2_H
#define TINY_VECTOR2_H

#include <math.h>


class TinyVector2 {

public:

    char x = 0;
    char y = 0;

    TinyVector2() {

    }

    TinyVector2(char x, char y) {
        this->x = x;
        this->y = y;
    }


    TinyVector2& operator =(const TinyVector2 &vec) {

        x = vec.x;
        y = vec.y;

        return (*this);
    }
    /*
    int SqrMagnitude() {
        return ((int)x) * x + ((int)y) * y;
    }

    int Magnitude() {
        return sqrt(((int)x) * x + ((int)y) * y);
    }
*/

};

#endif