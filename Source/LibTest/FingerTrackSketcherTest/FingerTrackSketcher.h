
#ifndef FINGER_TRACK_SKETCHER_H
#define FINGER_TRACK_SKETCHER_H

#include "Canvas.h"

class FingerTrackSketcher {

  private:
    int currentX;
    int currentY;
    int deltaX;
    int deltaY;

    const int canvasSizeX = 32;
    const int canvasSizeY = 32;

  public:

    Canvas *canvas;

    FingerTrackSketcher() {
      currentX = 0;
      currentY = 0;
      deltaX = 0;
      deltaY = 0;
      canvas->SetSize(canvasSizeX, canvasSizeY);
    }

    void SetDeltaXY(int deltaX, int deltaY) {
      this->deltaX = deltaX;
      this->deltaY = deltaY;
    }
    

    void Sketch() {
      canvas->Line(currentX, currentY, currentX + deltaX, currentY + deltaY);
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
      canvas->Zoom(*toCanvas);

      //キャンバスのクリア
      ClearCanvas();

      //パラメーターのクリア
      currentX = 0;
      currentY = 0;
      deltaX = 0;
      deltaY = 0;
    }

    void ClearCanvas(){
      canvas->color = false;
      canvas->Boxf(0,0,sketchCanvas->SizeX(), sketchCanvas->SizeY());
      canvas->color = true;
    }

};

#endif FINGER_TRACK_SKETCHER_H
