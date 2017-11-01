#include "FingerTrackSketcher.h"
#include "CanvasPrint.h"

FingerTrackSketcher ftSketcher;

Canvas toCanvas(8,8);
Canvas fromCanvas(8,8);

int deltaX;
int deltaY;

void setup() {
  ftSketcher.sketchCanvas.SetSize(8,8);
  deltaX = 5;
  deltaY = 1;
  Serial.begin(9600);
}

void loop() {
  //スケッチャーのキャンバスをコピー先にコピーする
  ftSketcher.SetDeltaXY(deltaX, deltaY);
  ftSketcher.Sketch();
  
  ftSketcher.CopyCanvas(&toCanvas);

  SerialPrintCanvas(toCanvas);

  delay(1000);

  ftSketcher.ClearCanvas();

  
  //コピー先からスケッチャーのキャンバスにコピーする
  
  

  

  
}

