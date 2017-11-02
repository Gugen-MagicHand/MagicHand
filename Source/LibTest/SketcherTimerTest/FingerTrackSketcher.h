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
    int currentX;
    int currentY;
    int deltaX;
    int deltaY;

    const int skCanvasSizeX = 32;
    const int skCanvasSizeY = 32;
    const int startPosX = 15;
    const int startPosY = 15;

    bool pushFlag;
    bool strokeEndFlag;
    bool tokenEndFlag;

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

      pushFlag = false;
      strokeEndFlag = false;
      tokenEndFlag = false;
    }

    void SetDeltaXY(int deltaX, int deltaY) {
      this->deltaX = deltaX;
      this->deltaY = deltaY;
    }


    void Sketch() {
      if ((deltaX < 3) && (deltaY < 3)) {

      } else {
        skCanvas.Line(currentX, currentY, currentX + deltaX, currentY + deltaY);
        currentX += deltaX;
        currentY += deltaY;
      }
      deltaX = 0;
      deltaY = 0;
    }



    //コピー先のキャンバスを設定
    void SetToCanvas(Canvas *toCanvas) {
      this->toCanvas = toCanvas;
    }


    void CopyCanvas() {
      int fromSizeX = skCanvas.GetLowerRightX() - skCanvas.GetUpperLeftX() + 1;
      int fromSizeY = skCanvas.GetLowerRightY() - skCanvas.GetUpperLeftY() + 1;

      ClearToCanvas();

      if ((fromSizeX < 3) && (fromSizeY < 3)) {
        toCanvas->Dot(0,0);
      } else {
        int toSizeX;
        int toSizeY;

        if (fromSizeX > fromSizeY) {
          toSizeX = toCanvas->SizeX();
          toSizeY = fromSizeY * toSizeX / fromSizeX;
        } else {
          toSizeY = toCanvas->SizeY();
          toSizeX = fromSizeX * toSizeY / fromSizeY;
        }

        //キャンバスのコピー
        toCanvas->Pos(0, 0);
        toCanvas->Zoom(toSizeX, toSizeY, skCanvas, skCanvas.GetUpperLeftX(), skCanvas.GetUpperLeftY(), fromSizeX, fromSizeY);
      }

      //キャンバスのクリア
      ClearSketcherCanvas();

      //パラメーターのクリア
      currentX = 0;
      currentY = 0;
      deltaX = 0;
      deltaY = 0;
    }

    void ClearSketcherCanvas() {
      skCanvas.color = false;
      skCanvas.Boxf(0, 0, skCanvas.SizeX(), skCanvas.SizeY());
      skCanvas.color = true;
    }

    void ClearToCanvas(){
      toCanvas=>color = false;
      toCanvas->Boxf(0, 0, skCanvas.SizeX(), skCanvas.SizeY());
      toCanvas->color = true;
    }

};

#endif
