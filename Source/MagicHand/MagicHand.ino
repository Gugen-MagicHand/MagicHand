//
//ヘッダーファイルのインクルード
#include "Lib/FingerTrackDriver.h"
#include "Lib/FingerTrackSketcher.h"
#include "Lib/CanvasQueue.h"

//これはCanvasをシリアルに出力する関数を使うためのものです。本番はコメントアウトすること。
#include "Lib/CanvasPrint.h"

//FingerTrackDriverの用意
FingerTrackDriver ftDriver;

//FingerTrackSketcherの用意
FingerTrackSketcher ftSketcher;

//CanvasQueueを用意
CanvasQueue canvasQueue(10, 8, 8);

//ArduinOSのタスク宣言

//トラックボールの回転認識のタスク
DeclareTaskLoop(LeftTask);
DeclareTaskLoop(RightTask);
DeclareTaskLoop(UpTask);
DeclareTaskLoop(DownTask);

void setup() {
  //シリアル通信の開始（デバッグ用、本番はコメントアウトする。）
  Serial.begin(19200);

  //FingerTrackDriverのピン設定、初期化
  ftDriver.pinBTN = 2;
  ftDriver.pinLFT = 3;
  ftDriver.pinRHT = 4;
  ftDriver.pinUP = 5;
  ftDriver.pinDWN = 6;

  ftDrivaer.Begin();


  //ArduinOSのタスクループを作る
  //トラックボールの認識タスク
  CreateTaskLoop(LeftTask, LOW_PRIORITY);
  CreateTaskLoop(RightTask, LOW_PRIORITY);
  CreateTaskLoop(UpTask, LOW_PRIORITY);
  CreateTaskLoop(DownTask, LOW_PRIORITY);
}

void loop() {
  // put your main code here, to run repeatedly:

}

//トラックボール左回転を認識するタスク
TaskLoop(LeftTask) {
  ftDriver.ReadLeft();
}

//トラックボール右回転を認識するタスク
TaskLoop(RightTask) {
  ftDriver.ReadRight();
}

//トラックボール上回転を認識するタスク
TaskLoop(UpTask) {
  ftDriver.ReadUp();
}

//トラックボール下回転を認識するタスク
TaskLoop(DownTask) {
  ftDriver.ReadDown();
}
