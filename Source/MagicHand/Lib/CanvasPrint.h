//canvasをシリアルプリントする関数です。（デバック用）--------
#ifndef CANVAS_PRINT_H
#define CANVAS_PRINT_H

#include "Canvas.h"

void SerialPrintCanvas(Canvas &canvas){
  int i = 0;
  int j = 0;
  bool dot;

  int sizeX = canvas.SizeX();
  int sizeY = canvas.SizeY();

  Serial.println("--------------------------------------------------------------");
  
  for(i=0; i<sizeY; i++){
    for(j=0; j<sizeX; j++){
      dot = canvas.ReadPixel(j,i);
      if(dot){
        Serial.print("■");
      }else{
        Serial.print("□");
      }
    }
    Serial.println();
  }

  Serial.println("--------------------------------------------------------------");
}

#endif
//---------------------------------------------------------------
