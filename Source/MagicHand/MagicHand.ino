
// Main header
#include "Lib/MagicHand.h"



static const unsigned long STROKE_INTERVAL_TIME = 100;
static const unsigned long LITERAL_INTERVAL_TIME = 500;









//FingerTrackDriverの用意
FingerTrackDriver ftDriver;

//FingerTrackSketcherの用意
FingerTrackSketcher ftSketcher;

//CanvasQueueを用意
CanvasQueue canvasQueue(10, 8, 8);

// --- タスク宣言 --------------------------------------------------

//トラックボールの回転認識のタスク
DeclareTaskLoop(TrackBallLeftRotationTask);
DeclareTaskLoop(TrackBallRightRotationTask);
DeclareTaskLoop(TrackBallUpRotationTask);
DeclareTaskLoop(TrackBallDownRotationTask);

DeclareTaskLoop(SketchCanvasFromTrackBallTask);



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

    CreateTaskLoopWithStackSize(SketchCanvasFromTrackBallTask, LOW_PRIORITY, 200);


    // --- セマフォ作成 -----------------------------------------------------------


    CreateBinarySemaphore(trackBallLeftRotationSem);
    CreateBinarySemaphore(trackBallRightRotationSem);
    CreateBinarySemaphore(trackBallUpRotationSem);
    CreateBinarySemaphore(trackBallDownRotationSem);

    InitMainLoopStackSize(200);



}

void loop() {

}


TaskLoop(SketchCanvasFromTrackBallTask) {

    //Serial.println(ftSketcher.DeltaXYStayZeroTime());
    static bool isAlreadyStrokePushed = true;
    static bool isAlreadyLiteralPushed = true;

    // CanvasQueue から作業領域キャンバスを確保できるまで待機
    // 前回ループでプッシュされない限り, 同じキャンバスを返す.
    while (true) {
        Canvas *work;
        if (canvasQueue.GetPushedReadyCanvas(&work)) {
            ftSketcher.SetToCanvas(work);
            break;
        }
    }


    // --- トラックボールからdeltaX, deltaYを取得 --------------------
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

    ftSketcher.SetDeltaXY(ftDriver.GetDeltaX(), ftDriver.GetDeltaY());

    ftDriver.ResetDeltaXY();
    // End トラックボールからdeltaX, deltaYを取得　------


    if (!ftSketcher.IsDeltaXYZero()) {
        // deltaX, deltaYがともに0ではないときは,
        // 何らかの入力が行われたことなので,
        // ストローク, リテラルの区切りとした
        // pushフラグを下す.
        isAlreadyStrokePushed = false;
        isAlreadyLiteralPushed = false;
    }


    // スケッチに描画
    ftSketcher.Sketch();


    if (!isAlreadyStrokePushed
        && (ftSketcher.DeltaXYStayZeroTime() > STROKE_INTERVAL_TIME)) {
        ftSketcher.CopyCanvas();
        canvasQueue.Push();

        // strokeプッシュ済みなのでフラグを上げる.
        isAlreadyStrokePushed = true;
        Serial.println("StrokeP");
    }
    else if (!isAlreadyLiteralPushed
        && (ftSketcher.DeltaXYStayZeroTime() > LITERAL_INTERVAL_TIME)) {
        ftSketcher.CopyCanvas();
        canvasQueue.Push();

        // literalプッシュ済みなのでフラグを上げる.
        isAlreadyLiteralPushed = true;
        Serial.println("LiteralP");
    }




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
