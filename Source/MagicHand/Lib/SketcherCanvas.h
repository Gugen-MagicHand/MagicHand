// 2017/11/2:
//    完成
#ifndef SKETCHER_CANVAS_H
#define SKETCHER_CANVAS_H

#include "Canvas.h"

class SketcherCanvas : public Canvas
{
private:
    int upperLeftX;
    int upperLeftY;
    int lowerRightX;
    int lowerRightY;

public:
    SketcherCanvas(int sizeX, int sizeY) : Canvas(sizeX, sizeY) {
    }

    SketcherCanvas() : Canvas() {
    }

    int GetUpperLeftX() {
        return upperLeftX;
    }

    int GetUpperLeftY() {
        return upperLeftY;
    }

    int GetLowerRightX() {
        return lowerRightX;
    }

    int GetLowerRightY() {
        return lowerRightY;
    }

    void SeekCorner() {
        int x;
        int y;
        upperLeftX = sizeX;
        upperLeftY = sizeY;
        lowerRightX = 0;
        lowerRightY = 0;

        for (y = 0; y < sizeY; y++) {
            for (x = 0; x < sizeX; x++) {
                if (ReadPixel(x, y)) {

                    //描画されているx座標中でもっとも小さいものを抽出
                    if (x < upperLeftX) {
                        upperLeftX = x;
                    }
                    //描画されているx座標中でもっとも大きいものを抽出
                    if (x > lowerRightX) {
                        lowerRightX = x;
                    }
                    //描画されているy座標中でもっとも小さいものを抽出
                    if (y < upperLeftY) {
                        upperLeftY = y;
                    }
                    //描画されているy座標中でもっとも大きいものを抽出
                    if (y > lowerRightY) {
                        lowerRightY = y;
                    }

                }
            }
        }
    }

};


#endif


