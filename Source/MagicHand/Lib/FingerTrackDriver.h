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
    const float sensitivity = 4.0;

    const int X_AXIS_DIR = -1;
    const int Y_AXIS_DIR = -1;


    // 最大周期
    // 人がトラックボールをゆっくり回せる限界周期
    const int maxRange = 30000;

    // 最小周期
    // 人がトラックボールを速く回せる限界周期
    const int minRange = 900;

    double deltaLeft = 0.0;
    double deltaRight = 0.0;
    double deltaUp = 0.0;
    double deltaDown = 0.0;

public:
    const float range = 1.0 / minRange - 1.0 / maxRange;

    double deltaX = 0.0;
    double deltaY = 0.0;



public:

    //セマフォ用に四方向を個別にdeltaX,deltaYに追加
    void AddDeltaLeftToDeltaX(void) {

        deltaX -= X_AXIS_DIR * deltaLeft;

        deltaLeft = 0.0;
    }

    void AddDeltaRightToDeltaX(void) {

        deltaX += X_AXIS_DIR * deltaRight;

        deltaRight = 0.0;
    }

    void AddDeltaUpToDeltaY(void) {
        deltaY += Y_AXIS_DIR * deltaUp;

        deltaUp = 0.0;
    }

    void AddDeltaDownToDeltaY(void) {
        deltaY -= Y_AXIS_DIR * deltaDown;

        deltaDown = 0.0;
    }

    double GetDeltaX(void) {
        return deltaX;
    }

    double GetDeltaY(void) {
        return deltaY;
    }

    void ResetDeltaXY(void) {
        deltaX = 0.0;
        deltaY = 0, 0;
    }

    void ResetDeltaX(void) {
        deltaX = 0, 0;
    }

    void ResetDeltaY(void) {
        deltaY = 0, 0;
    }



    void AddLeftToDelta(void) {

        //Serial.println(left);

        if (left > 0) {
            deltaLeft += sensitivity / left / range;
        }
    }
    void AddRightToDelta(void) {
        if (right > 0) {
            deltaRight += sensitivity / right / range;
        }
    }

    void AddUpToDelta(void) {
        if (up > 0) {
            deltaUp += sensitivity / up / range;
        }
    }

    void AddDownToDelta(void) {
        if (down > 0) {
            deltaDown += sensitivity / down / range;
        }
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
        //Serial.println(left);
        if (left >= maxRange || left < minRange) {
            //Serial.println("+");
            left = 0;
        }
        //Serial.println("+");
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
