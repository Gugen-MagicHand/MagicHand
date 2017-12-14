//  2017/11/2:
//    完成
//  2017/11/3:
//    時間計測して文字の区切りを入れる関数追加
//    点を認識できるようにCopyCanvas関数を改良

#ifndef FINGER_TRACK_SKETCHER_H
#define FINGER_TRACK_SKETCHER_H

//#include "SketcherCanvas.h"
#include "TinyVector2.h"
#include "AccessibleQueue.h"


#define _USE_MATH_DEFINES
#include <math.h>




class FingerTrackSketcher {

private:
    //double currentX;
    //double currentY;

    double deltaX;
    double deltaY;

    //const int skCanvasSizeX = 32;
    //const int skCanvasSizeY = 32;
    //const int startPosX = 15;
    //const int startPosY = 15;

    static const int VECTORS_CAPACITY = 16;

    AccessibleQueue<TinyVector2> vectors;


    double startPointPosX, startPointPosY;
    double endPointPosX, endPointPosY;


public:
    // deltaX, deltaYがともに0になった時間
    unsigned long deltaXYBecomeZeroStartTime;

public:
    //skCanvasは四つ角をとれるようにしたCanvasの拡張。
    //SketcherCanvas skCanvas;


    Canvas testCanvas;

    //toCanvasにはqueueからキャンバスの参照をコピーする。
    Canvas *toCanvas;

    FingerTrackSketcher() : vectors(VECTORS_CAPACITY) {
        //currentX = startPosX;
        //currentY = startPosY;
        deltaX = 0;
        deltaY = 0;
        //skCanvas.SetSize(skCanvasSizeX, skCanvasSizeY);


        //testCanvas.SetSize(16, 16);
    }

    bool IsDeltaXYZero() { return (deltaX == 0 && deltaY == 0); }

    double DeltaX() { return deltaX; }
    double DeltaY() { return deltaY; }

    void SetDeltaXY(double deltaX, double deltaY) {
        this->deltaX += deltaX;
        this->deltaY += deltaY;


        //Serial.println(deltaX);
        //Serial.println(deltaY);

        if (!(deltaX == 0.0 && deltaY == 0.0)) {
            // deltaX, deltaYがともに0にならなかった時間を記録
            // ともに0になったときは更新されない
            deltaXYBecomeZeroStartTime = millis();

            //Serial.println("+");
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


        //if (deltaX != 0.0 || deltaY != 0.0)
        if (fabs(deltaX) + fabs(deltaY) > 4.0)
        {

            //Serial.println(deltaX);
            //Serial.println(deltaY);
            // deltaX, deltaYが0ではないときに, 描画を始める.

            //skCanvas.Line(currentX, currentY, currentX + deltaX, currentY + deltaY);
            if (startPointPosX != endPointPosX || startPointPosY != endPointPosY) {
                // はじめのベクトルではない



                double gradFrom = atan2(endPointPosY - startPointPosY, endPointPosX - startPointPosX);
                double gradTo = atan2(deltaY, deltaX);

                double angle = fabs(gradTo - gradFrom);

                if (angle >= M_PI * 2.0) {
                    angle -= 2.0 * M_PI;
                }


                if (angle > 0.5) {
                    //Serial.println(angle);
                    vectors.Push(TinyVector2(endPointPosX - startPointPosX, endPointPosY - startPointPosY));

                    startPointPosX = endPointPosX;
                    startPointPosY = endPointPosY;


                    endPointPosX = startPointPosX;
                    endPointPosY = startPointPosY;


                }
            }


            endPointPosX += deltaX;
            endPointPosY += deltaY;


            deltaX = 0.0;
            deltaY = 0.0;
        }

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

        if (endPointPosX - startPointPosX != 0.0 || endPointPosY - startPointPosY != 0.0) {
            vectors.Push(TinyVector2(endPointPosX - startPointPosX, endPointPosY - startPointPosY));

        }

        if (deltaX != 0.0 || deltaY != 0.0) {

            vectors.Push(TinyVector2(deltaX, deltaY));
        }

        ClearToCanvas();


        int minPosX, minPosY;
        int maxPosX, maxPosY;

        if (GetBounds(minPosX, minPosY, maxPosX, maxPosY)) {
            int fromSizeX = maxPosX - minPosX + 1;
            int fromSizeY = maxPosY - minPosY + 1;

            //Serial.println(minPosX);
            //Serial.println(minPosY);
            //Serial.println(maxPosX);
            //Serial.println(maxPosY);

            if ((fromSizeX <= 3) && (fromSizeY <= 3) && (fromSizeX >= 1) && (fromSizeY >= 1)) {

                toCanvas->color = true;
                toCanvas->Dot(0, 0);
            }

            else {
                double zoomRate = fromSizeX > fromSizeY ? toCanvas->SizeX() / (double)fromSizeX : toCanvas->SizeY() / (double)fromSizeY;

                //Serial.println(fromSizeX);
                //Serial.println(fromSizeY);
                //Serial.println(zoomRate);
                int posX = -minPosX;
                int posY = -minPosY;
                toCanvas->color = true;
                for (int i = 0; i < vectors.Count(); i++) {
                    toCanvas->Line(posX * zoomRate, posY * zoomRate, (posX + vectors[i].x) * zoomRate, (posY + vectors[i].y) * zoomRate);

                    posX += vectors[i].x;
                    posY += vectors[i].y;
                }
            }
        }

        //
        //if (skCanvas.SeekCorner()) {

        //    // 正常にコーナを取得できた時

        //    // 左上座標, 右下座標を取得し, コピー元のサイズを計算する.
        //    int fromSizeX = skCanvas.GetLowerRightX() - skCanvas.GetUpperLeftX() + 1;
        //    int fromSizeY = skCanvas.GetLowerRightY() - skCanvas.GetUpperLeftY() + 1;



        //    if ((fromSizeX <= 3) && (fromSizeY <= 3) && (fromSizeX >= 1) && (fromSizeY >= 1)) {
        //        toCanvas->Dot(0, 0);
        //    }


        //    else {
        //        int toSizeX;
        //        int toSizeY;

        //        if (fromSizeX > fromSizeY) {
        //            toSizeX = toCanvas->SizeX();
        //            toSizeY = fromSizeY * (double)toSizeX / fromSizeX;

        //            if (toSizeY <= 0) {
        //                toSizeY = 1;
        //            }
        //        }
        //        else {
        //            toSizeY = toCanvas->SizeY();
        //            toSizeX = fromSizeX * (double)toSizeY / fromSizeY;

        //            if (toSizeX <= 0) {
        //                toSizeX = 1;
        //            }
        //        }

        //        //キャンバスのコピー
        //        toCanvas->Pos(0, 0);
        //        /*
        //        Serial.println(skCanvas.GetUpperLeftX());
        //        Serial.println(skCanvas.GetUpperLeftY());
        //        Serial.println(fromSizeX);
        //        Serial.println(fromSizeY);
        //        Serial.println(toSizeX);
        //        Serial.println(toSizeY);
        //        */
        //        toCanvas->Zoom(toSizeX, toSizeY, skCanvas, skCanvas.GetUpperLeftX(), skCanvas.GetUpperLeftY(), fromSizeX, fromSizeY);
        //    }

        //}
/*
        
        int fromPosX = 8;
        int fromPosY = 0;
        testCanvas.color = true;

        Serial.println(vectors.Count());
        for (int i = 0; i < vectors.Count(); i++) {
            Serial.print((int)vectors[i].x);
            Serial.print(", ");
            Serial.println((int)vectors[i].y);
            testCanvas.Line(fromPosX, fromPosY, fromPosX + vectors[i].x, fromPosY + vectors[i].y);

            fromPosX += vectors[i].x;
            fromPosY += vectors[i].y;
        }

        SerialPrintCanvas(testCanvas);

        testCanvas.color = false;
        testCanvas.Boxf(0, 0, testCanvas.SizeX(), testCanvas.SizeY());
*/
        //SerialPrintCanvas(*toCanvas);

        //キャンバスのクリア
        //ClearSketcherCanvas();

        vectors.Clear();

        //パラメータの初期化
        //currentX = startPosX;
        //currentY = startPosY;
        startPointPosX = 0.0;
        startPointPosY = 0.0;

        endPointPosX = 0.0;
        endPointPosY = 0.0;

        deltaX = 0.0;
        deltaY = 0.0;
    }
    /*
        void ClearSketcherCanvas() {
            skCanvas.color = false;
            skCanvas.Boxf(0, 0, skCanvas.SizeX(), skCanvas.SizeY());
            skCanvas.color = true;
        }
    */
    void ClearToCanvas() {
        toCanvas->color = false;
        toCanvas->Boxf(0, 0, toCanvas->SizeX(), toCanvas->SizeY());
        //toCanvas->color = true;
    }


    // Bounding box
    bool GetBounds(int &minPosX, int &minPosY, int &maxPosX, int &maxPosY) {

        if (vectors.Count() <= 0) {
            return false;
        }


        int posX = 0;
        int posY = 0;

        minPosX = 0;
        minPosY = 0;

        maxPosX = 0;
        maxPosY = 0;



        for (int i = 0; i < vectors.Count(); i++) {

            posX += vectors[i].x;
            posY += vectors[i].y;


            if (posX < minPosX) {
                minPosX = posX;
            }

            if (posY < minPosY) {
                minPosY = posY;
            }

            if (posX > maxPosX) {
                maxPosX = posX;
            }

            if (posY > maxPosY) {
                maxPosY = posY;
            }


        }

        return true;
    }




};

#endif
