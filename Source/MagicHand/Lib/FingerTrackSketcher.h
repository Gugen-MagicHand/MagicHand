//  2017/11/2:
//    完成
//  2017/11/3:
//    時間計測して文字の区切りを入れる関数追加
//    点を認識できるようにCopyCanvas関数を改良

#ifndef FINGER_TRACK_SKETCHER_H
#define FINGER_TRACK_SKETCHER_H

#include "SketcherCanvas.h"




class FingerTrackSketcher {

private:
    double currentX;
    double currentY;

    double deltaX;
    double deltaY;

    const int skCanvasSizeX = 32;
    const int skCanvasSizeY = 32;
    const int startPosX = 15;
    const int startPosY = 15;

public:
    // deltaX, deltaYがともに0になった時間
    unsigned long deltaXYBecomeZeroStartTime;

public:
    //skCanvasは四つ角をとれるようにしたCanvasの拡張。
    SketcherCanvas skCanvas;

    //toCanvasにはqueueからキャンバスの参照をコピーする。
    Canvas *toCanvas;

    FingerTrackSketcher() {
        currentX = startPosX;
        currentY = startPosY;
        deltaX = 0;
        deltaY = 0;
        skCanvas.SetSize(skCanvasSizeX, skCanvasSizeY);
    }

    bool IsDeltaXYZero() { return (deltaX == 0 && deltaY == 0); }

    int DeltaX() { return deltaX; }
    int DeltaY() { return deltaY; }

    void SetDeltaXY(int deltaX, int deltaY) {
        this->deltaX += deltaX;
        this->deltaY += deltaY;


        //Serial.println(deltaX);
        //Serial.println(deltaY);

        if (!(deltaX == 0 && deltaY == 0)) {
            // deltaX, deltaYがともに0にならなかった時間を記録
            // ともに0になったときは更新されない
            deltaXYBecomeZeroStartTime = millis();
        }
    }


    void Sketch() {
        // これでは, 合わされば大きくなる微小な速度の描画が反映されない.
        // 
        /*
        if ((abs(deltaX) < 3) && (abs(deltaY) < 3)) {
            // deltaX, deltaYがともに3より小さい場合は,
            // 描画を行わない.
        }
        else*/


        if (deltaX != 0.0 || deltaY != 0.0)
        {
            // deltaX, deltaYが0ではないときに, 描画を始める.

            skCanvas.Line(currentX, currentY, currentX + deltaX, currentY + deltaY);
            currentX += deltaX;
            currentY += deltaY;
        }

        deltaX = 0;
        deltaY = 0;
    }

    //
    // @return:
    //  deltaX, deltaYがともに0である時間を取得する
    //
    unsigned long DeltaXYStayZeroTime() {
        return millis() - deltaXYBecomeZeroStartTime;
    }



    //コピー先のキャンバスを設定
    void SetToCanvas(Canvas *toCanvas) {
        this->toCanvas = toCanvas;
    }


    void CopyCanvas() {

        //SerialPrintCanvas(skCanvas);

        ClearToCanvas();

        if (skCanvas.SeekCorner()) {

            // 正常にコーナを取得できた時

            // 左上座標, 右下座標を取得し, コピー元のサイズを計算する.
            int fromSizeX = skCanvas.GetLowerRightX() - skCanvas.GetUpperLeftX() + 1;
            int fromSizeY = skCanvas.GetLowerRightY() - skCanvas.GetUpperLeftY() + 1;



            if ((fromSizeX <= 3) && (fromSizeY <= 3)) {
                toCanvas->Dot(0, 0);
            }


            else {
                int toSizeX;
                int toSizeY;

                if (fromSizeX > fromSizeY) {
                    toSizeX = toCanvas->SizeX();
                    toSizeY = fromSizeY * (double)toSizeX / fromSizeX;

                    if (toSizeY <= 0) {
                        toSizeY = 1;
                    }
                }
                else {
                    toSizeY = toCanvas->SizeY();
                    toSizeX = fromSizeX * (double)toSizeY / fromSizeY;

                    if (toSizeX <= 0) {
                        toSizeX = 1;
                    }
                }

                //キャンバスのコピー
                toCanvas->Pos(0, 0);
                /*
                Serial.println(skCanvas.GetUpperLeftX());
                Serial.println(skCanvas.GetUpperLeftY());
                Serial.println(fromSizeX);
                Serial.println(fromSizeY);
                Serial.println(toSizeX);
                Serial.println(toSizeY);
                */
                toCanvas->Zoom(toSizeX, toSizeY, skCanvas, skCanvas.GetUpperLeftX(), skCanvas.GetUpperLeftY(), fromSizeX, fromSizeY);
            }

        }




        //SerialPrintCanvas(*toCanvas);

        //キャンバスのクリア
        ClearSketcherCanvas();

        //パラメータの初期化
        currentX = startPosX;
        currentY = startPosY;
        deltaX = 0.0;
        deltaY = 0.0;
    }

    void ClearSketcherCanvas() {
        skCanvas.color = false;
        skCanvas.Boxf(0, 0, skCanvas.SizeX(), skCanvas.SizeY());
        skCanvas.color = true;
    }

    void ClearToCanvas() {
        toCanvas->color = false;
        toCanvas->Boxf(0, 0, skCanvas.SizeX(), skCanvas.SizeY());
        toCanvas->color = true;
    }

};

#endif
