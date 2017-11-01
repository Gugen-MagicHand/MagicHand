#ifndef FINGER_TRACK_SKETCHER_H
#define FINGER_TRACK_SKETCHER_H

#include "Canvas.h"

class FingerTrackSketcher {

  public:
    int currentX;
    int currentY;
    int deltaX;
    int deltaY;

  public:

    Canvas *sketchCanvas;

    FingerTrackSketcher() {
      currentX = 0;
      currentY = 0;
      deltaX = 0;
      deltaY = 0;
    }

    void SetDeltaXY(int deltaX, int deltaY) {
      this->deltaX = deltaX;
      this->deltaY = deltaY;
    }

    void ChangeSketchCanvas(Canvas* canvas){
      sketchCanvas = canvas;
      ClearCanvas();
    }
    

    void Sketch() {
      sketchCanvas->Line(currentX, currentY, currentX + deltaX, currentY + deltaY);
      currentX += deltaX;
      currentY += deltaY;
      deltaX = 0;
      deltaY = 0;
    }

    /*queueの完成待ち
      void RequestCanvas(){

      }
    */


    void CopyCanvas(Canvas *toCanvas) {
      //キャンバスのコピー
      sketchCanvas->ScaleTo(*toCanvas);

      //パラメーターのクリア
      currentX = 0;
      currentY = 0;
      deltaX = 0;
      deltaY = 0;
    }

    void ClearCanvas(){
      sketchCanvas->color = false;
      sketchCanvas->Boxf(0,0,sketchCanvas->SizeX(), sketchCanvas->SizeY());
    }

};

#endif FINGER_TRACK_SKETCHER_H
