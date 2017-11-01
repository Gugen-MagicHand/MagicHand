
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

  public:

    SketcherCanvas skCanvas;
    Canvas *toCanvas;

    FingerTrackSketcher() {
      currentX = startPosX;
      currentY = startPosY;
      deltaX = 0;
      deltaY = 0;
      skCanvas.SetSize(skCanvasSizeX, skCanvasSizeY);
    }

    void SetDeltaXY(int deltaX, int deltaY) {
      this->deltaX = deltaX;
      this->deltaY = deltaY;
    }
    

    void Sketch() {
      skCanvas.Line(currentX, currentY, currentX + deltaX, currentY + deltaY);
      currentX += deltaX;
      currentY += deltaY;
      deltaX = 0;
      deltaY = 0;
    }

    /*queueの完成待ち
      void RequestCanvas(){

      }
      */

    //コピー先のキャンバスを設定
    void SetToCanvas(Canvas *toCanvas){
      this->toCanvas = toCanvas;
    }

    void CopyCanvas() {
      //キャンバスのコピー
      toCanvas->Zoom(toCanvas->SizeX(), toCanvas->SizeY(), skCanvas, skCanvas.GetUpperLeftX(), skCanvas.GetUpperLeftY(), skCanvas.GetLowerRightX() - skCanvas.GetUpperLeftX() + 1, skCanvas.GetLowerRightY() - skCanvas.GetUpperLeftY() + 1);

      //キャンバスのクリア
      ClearSketcherCanvas();

      //パラメーターのクリア
      currentX = 0;
      currentY = 0;
      deltaX = 0;
      deltaY = 0;
    }

    void ClearSketcherCanvas(){
      skCanvas.color = false;
      skCanvas.Boxf(0,0,skCanvas.SizeX(), skCanvas.SizeY());
      skCanvas.color = true;
    }

};

#endif
