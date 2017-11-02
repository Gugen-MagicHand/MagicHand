//更新履歴
//  2017/11/2:
//    完成

#ifndef FINGER_TRACK_DRIVER_H
#define FINGER_TRACK_DRIVER_H

#include "TrackBallDriver.h"


class FingerTrackDriver : public TrackBallDriver
{
  private:
    const byte sensitivity = 32;
    const int maxRange = 20000;
    const int minRange = 10;

    long leftSum;
    long rightSum;
    long upSum;
    long downSum;

  public:
    const float range = 1.0 / minRange - 1.0 / maxRange;

    int deltaX;
    int deltaY;



  public:

    //セマフォ用に四方向を個別にdeltaX,deltaYに追加
    void AddLeftToDeltaX(void) {
      deltaX = (int)-1 / leftSum / range * sensitivity;
      leftSum = 0;
    }

    void AddRightToDeltaX(void) {
      deltaX = (int)1 / rightSum / range * sensitivity;
      rightSum = 0;
    }

    void AddUpToDeltaY(void) {
      deltaY = (int)-1 / upSum / range * sensitivity;
      upSum = 0;
    }

    void AddDownToDeltaY(void) {
      deltaY = (int)1 / downSum / range * sensitivity;
      downSum = 0;
    }

    int GetDeltaX(void) {
      return deltaX;
    }

    int GetDeltaY(void) {
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
      }
    }

    void AddLeftToSum(void) {
      leftSum += left;
    }

    void ReadRight(void) {
      right = pulseIn(pinRHT, HIGH, timeout);
      if (right >= maxRange || right < minRange) {
        right = 0;
      }
    }

    void AddRightToSum(void) {
      rightSum += right;
    }

    void ReadUp(void) {
      up = pulseIn(pinUP, HIGH, timeout);
      if (up >= maxRange || up < minRange) {
        up = 0;
      }
    }

    void AddUpToSum(void) {
      upSum += up;
    }

    void ReadDown(void) {
      down = pulseIn(pinDWN, HIGH, timeout);
      if (down >= maxRange || down < minRange) {
        down = 0;
      }
    }

    void AddDownToSum(void) {
      downSum += down;
    }
};

#endif FINGER_TRACK_DRIVER_H
