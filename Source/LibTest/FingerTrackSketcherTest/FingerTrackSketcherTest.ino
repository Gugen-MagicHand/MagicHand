
#include "FingerTrackSketcher.h"
#include "CanvasPrint.h"

FingerTrackSketcher ftSketcher;

Canvas toCanvas(8,8);
Canvas fromCanvas(8,8);

int deltaX;
int deltaY;

void setup() {
  deltaX = -10;
  deltaY = 10;
  Serial.begin(9600);


  //deltaX,deltaY分だけ描画
  ftSketcher.SetDeltaXY(deltaX, deltaY);
  ftSketcher.Sketch();
  SerialPrintCanvas(ftSketcher.skCanvas);
  
  
  //スケッチャーのキャンバスをコピー先にコピーする


  
  //コピー先からスケッチャーのキャンバスにコピーする
    
}

void loop() {
  //書かれているものの角をとる
  ftSketcher.skCanvas.SeekCorner();
  Serial.print("UpperLeftX:");
  Serial.println(ftSketcher.skCanvas.GetUpperLeftX());
  Serial.print("UpperLeftY:");
  Serial.println(ftSketcher.skCanvas.GetUpperLeftY());
  Serial.print("LowerRightX: ");
  Serial.println(ftSketcher.skCanvas.GetLowerRightX());
  Serial.print("LowerRightY: ");
  Serial.println(ftSketcher.skCanvas.GetLowerRightY());

  while(1){
  }
}



