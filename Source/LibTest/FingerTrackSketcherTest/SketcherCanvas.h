
#ifndef SKETCHER_CANVAS_H
#define SLETCHER_CANVAS_H

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

    SketcherCanvas() : Canvas(){
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

    void SeekUpperLeftCorner() {
      int x;
      int y;
      upperLeftX = 0;
      upperLeftY = 0;

      for (y = 0; y < sizeY; y++) {
        for (x = 0; x < sizeX; x++) {
          if (ReadPixel(x, y)) {
            upperLeftX = x;
            upperLeftY = y;
            return;
          }
        }
      }
    }

    void SeekLowerRightCorner() {
      int x;
      int y;
      lowerRightX = 0;
      lowerRightY = 0;

      for (y = sizeY; y > 0; y--) {
        for (x = sizeX; x > 0; x--) {
          if (ReadPixel(x, y)) {
            lowerRightX = x;
            lowerRightY = y;
            return;
          }
        }
      }
    }

};


#endif


