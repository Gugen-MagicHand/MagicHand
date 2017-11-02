// 更新履歴
//  2017/11/2:
//   完成
//
//  2017/11/3:
//   0わりの問題を修正
//   一部計算の順番変更(桁落ち対策)
//

#ifndef FINGER_TRACK_DRIVER_H
#define FINGER_TRACK_DRIVER_H

#include "TrackBallDriver.h"


class FingerTrackDriver : public TrackBallDriver
{
private:
    const float sensitivity = 32.0;

    // 最大周期
    // 人がトラックボールをゆっくり回せる限界周期
    const int maxRange = 10000;

    // 最小周期
    // 人がトラックボールを速く回せる限界周期
    const int minRange = 500;

    long leftSum = 0;
    long rightSum = 0;
    long upSum = 0;
    long downSum = 0;

public:
    const float range = 1.0 / minRange - 1.0 / maxRange;

    int deltaX = 0;
    int deltaY = 0;



public:

    //セマフォ用に四方向を個別にdeltaX,deltaYに追加
    void AddLeftToDeltaX(void) {
        if (leftSum > 0) {
            deltaX += -sensitivity / leftSum / range;
            leftSum = 0;
        }
    }

    void AddRightToDeltaX(void) {
        if (rightSum > 0) {

            deltaX += sensitivity / rightSum / range;
            rightSum = 0;
        }
    }

    void AddUpToDeltaY(void) {
        if (upSum > 0) {
            deltaY += -sensitivity / upSum / range;
            upSum = 0;
        }
    }

    void AddDownToDeltaY(void) {
        if (downSum > 0) {

            deltaY += sensitivity / downSum / range;
            downSum = 0;
        }
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



    void AddLeftToSum(void) {
        leftSum += left;
    }
    void AddRightToSum(void) {
        rightSum += right;
    }

    void AddUpToSum(void) {
        upSum += up;
    }

    void AddDownToSum(void) {
        downSum += down;
    }

    //
    // 変数 left, right, up, downについて:
    //  これらの変数は, それぞれ一つのタスクがアクセスする変数である.
    //  よって, これら変数に対するアクセス制御は行わない.
    // 
    // pulseIn関数について:
    //  この関数は, パルス幅を返す関数である.
    //  パルスが入力されるのを指定された時間待機し,
    //  その間でパルスが入力されたならば, そのパルスの長さを返します.
    //  その間で入力されなかった場合は, 0を返します.
    //  
    //  パルスが入力されなかったとき, 待機することに注意してください.
    //  この間に, あるリソースに対するアクセス権を持ち続けている際,
    //  他タスクが長時間そのリソースにアクセスできません.



    // --- 上下左右値取得関数 -------------------------------------------------

    //
    // 警告:
    //  この関数実行前であらゆるリソースのアクセス権を開放して下さい.
    //

    //
    // left値をトラックボールデバイスから読み込みます.
    //
    void ReadLeft(void) {

        left = pulseIn(pinLFT, HIGH, timeout);
        if (left >= maxRange || left < minRange) {
            left = 0;
        }
    }

    //
    // right値をトラックボールデバイスから読み込みます.
    //
    void ReadRight(void) {
        right = pulseIn(pinRHT, HIGH, timeout);
        if (right >= maxRange || right < minRange) {
            right = 0;
        }
    }

    //
    // up値をトラックボールデバイスから読み込みます.
    //
    void ReadUp(void) {
        up = pulseIn(pinUP, HIGH, timeout);
        if (up >= maxRange || up < minRange) {
            up = 0;
        }
    }


    //
    // down値をトラックボールデバイスから読み込みます.
    //
    void ReadDown(void) {
        down = pulseIn(pinDWN, HIGH, timeout);
        if (down >= maxRange || down < minRange) {
            down = 0;
        }
    }

    // End 上下左右取得関数 -----------------

};

#endif FINGER_TRACK_DRIVER_H
