
// Main header
#include "Lib/MagicHand.h"

//FingerTrackDriverの用意
FingerTrackDriver ftDriver;

//FingerTrackSketcherの用意
FingerTrackSketcher ftSketcher;

//CanvasQueueを用意
CanvasQueue canvasQueue(10, 8, 8);

// --- ArduinOSのタスク宣言 --------------------------------------------------

//トラックボールの回転認識のタスク
DeclareTaskLoop(TrackBallLeftRotationTask);
DeclareTaskLoop(TrackBallRightRotationTask);
DeclareTaskLoop(TrackBallUpRotationTask);
DeclareTaskLoop(TrackBallDownRotationTask);



// End 


// --- セマフォ宣言 -------------------------------------------------------

SemaphoreHandle trackBallLeftRotationSem;
SemaphoreHandle trackBallRightRotationSem;
SemaphoreHandle trackBallUpRotationSem;
SemaphoreHandle trackBallDownRotationSem;

// End 


void setup() {
    //シリアル通信の開始（デバッグ用、本番はコメントアウトする。）
    Serial.begin(19200);

    //FingerTrackDriverのピン設定、初期化
    ftDriver.Begin(2, 3, 4, 5, 6);



    // --- Task 作成 --------------------------------------------------------------

    //トラックボールの認識タスク
    CreateTaskLoop(TrackBallLeftRotationTask, LOW_PRIORITY);
    CreateTaskLoop(TrackBallRightRotationTask, LOW_PRIORITY);
    CreateTaskLoop(TrackBallUpRotationTask, LOW_PRIORITY);
    CreateTaskLoop(TrackBallDownRotationTask, LOW_PRIORITY);


    // --- セマフォ作成 -----------------------------------------------------------


    CreateBinarySemaphore(trackBallLeftRotationSem);
    CreateBinarySemaphore(trackBallRightRotationSem);
    CreateBinarySemaphore(trackBallUpRotationSem);
    CreateBinarySemaphore(trackBallDownRotationSem);

    InitMainLoopStackSize(100);



}

void loop() {


    if (Acquire(trackBallLeftRotationSem, 1000)) {
        ftDriver.AddLeftToDeltaX();
        Release(trackBallLeftRotationSem);
    }

    if (Acquire(trackBallRightRotationSem, 1000)) {
        ftDriver.AddRightToDeltaX();
        Release(trackBallRightRotationSem);
    }

    if (Acquire(trackBallUpRotationSem, 1000)) {
        ftDriver.AddUpToDeltaY();
        Release(trackBallUpRotationSem);
    }

    if (Acquire(trackBallDownRotationSem, 1000)) {
        ftDriver.AddDownToDeltaY();
        Release(trackBallDownRotationSem);
    }

    Serial.print("deltaX:");
    Serial.print(ftDriver.GetDeltaX());
    Serial.print("  deltaY:");
    Serial.println(ftDriver.GetDeltaY());
    ftDriver.ResetDeltaXY();
}







// --- LeftTask-------------------------------------------------------------

TaskLoop(TrackBallLeftRotationTask) {
    ftDriver.ReadLeft();

    if (Acquire(trackBallLeftRotationSem, 1000)) {
        ftDriver.AddLeftToSum();
        Release(trackBallLeftRotationSem);

    }
}


// --- rightTask--------------------------------------------------------------

TaskLoop(TrackBallRightRotationTask) {
    ftDriver.ReadRight();

    if (Acquire(trackBallRightRotationSem, 1000)) {
        ftDriver.AddRightToSum();
        Release(trackBallRightRotationSem);

    }
}


// --- UpTask--------------------------------------------------------------------

TaskLoop(TrackBallUpRotationTask) {
    ftDriver.ReadUp();

    if (Acquire(trackBallUpRotationSem, 1000)) {
        ftDriver.AddUpToSum();
        Release(trackBallUpRotationSem);

    }
}

// --- DownTask---------------------------------------------------------------------

TaskLoop(TrackBallDownRotationTask) {
    ftDriver.ReadDown();

    if (Acquire(trackBallDownRotationSem, 1000)) {
        ftDriver.AddDownToSum();
        Release(trackBallDownRotationSem);

    }

}
