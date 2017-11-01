#ifndef FINGER_TRACK_DRIVER_H
#define FINGER_TRACK_DRIVER_H

#include "TrackBallDriver.h"


class FingerTrackDriver : public TrackBallDriver
{
  private:
    const byte sensitivity = 32;
    const int maxRange = 20000;
    const int minRange = 10;
  public:
    const float range = 1.0 / minRange - 1.0 / maxRange;

    float deltaX;
    float deltaY;



  public:

    long GetDeltaX(void) {
      return deltaX;
    }

    long GetDeltaY(void) {
      return deltaY;
    }

    void ResetDeltaXY(void) {
      deltaX = 0;
      deltaY = 0;
    }

    void ResetDeltaX(void) {
      deltaX = 0;
    }

    void ResetDeltaY(void) {
      deltaY = 0;
    }

    void ReadLeft(void) {
      left = pulseIn(pinLFT, HIGH, timeout);
      if (left >= maxRange || left < minRange) {
        left = 0;
      } else {
        deltaX -= 1.0 / left / range * sensitivity;
      }
    }

    void ReadRight(void) {
      right = pulseIn(pinRHT, HIGH, timeout);
      if (right >= maxRange || right < minRange) {
        right = 0;
      }else{
        deltaX += 1.0 / right / range * sensitivity;
      }
    }

    void ReadUp(void) {
      up = pulseIn(pinUP, HIGH, timeout);
      if (up >= maxRange || up < minRange) {
        up = 0;
      }else{
        deltaY -= 1.0 / up / range * sensitivity;
      }
    }

    void ReadDown(void) {
      down = pulseIn(pinDWN, HIGH, timeout);
      if (down >= maxRange || down < minRange) {
        down = 0;
      }else{
        deltaY += 1.0 / down / range * sensitivity;
      }
    }

};


#endif FINGER_TRACK_DRIVER_H
