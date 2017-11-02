#include "CanvasQueue.h"
#include "FingerTrackSketcher.h"
#include "CanvasPrint.h"


CanvasQueue canvasQueue(10, 8, 8);
FingerTrackSketcher ftSketcher;

int i;

Canvas *popedCanvas;

bool popResult;

void setup() {
  Serial.begin(19200);

  for (i = 0; i < 10; i++) {
    ftSketcher.skCanvas.Pos(10, 10);
    ftSketcher.skCanvas.PutChar(0x41 + i);
    ftSketcher.skCanvas.SeekCorner();

    SerialPrintCanvas(ftSketcher.skCanvas);

    ftSketcher.pushFlag = canvasQueue.GetPushedReadyCanvas(&ftSketcher.toCanvas);
    if (ftSketcher.pushFlag) {
      ftSketcher.CopyCanvas();
      SerialPrintCanvas(*(ftSketcher.toCanvas));
      canvasQueue.Push();
    }
  }

  Serial.println("End Push. Pop starts now.-----------------------------------------------------------");

  popResult = false;
  for (i = 0; i < 10; i++) {
    popResult = canvasQueue.Pop(&popedCanvas);
    if (popResult) {
      SerialPrintCanvas(*popedCanvas);
    }
  }


}

void loop() {
  ftSketcher.SetDeltaXY(10,10);
  delay(1000);
  Serial.println(ftSketcher.GetTimeFromDeltaXYGetTime());

}
