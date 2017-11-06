
// Main header
#include "Lib/MagicHand.h"

static const unsigned long STROKE_INTERVAL_TIME = 200;
static const unsigned long LITERAL_INTERVAL_TIME = 500;

static const unsigned long SKETCH_INTERVAL_TIME = 10;

//FingerTrackDriverの用意
FingerTrackDriver ftDriver;

//FingerTrackSketcherの用意
FingerTrackSketcher ftSketcher;

//CanvasQueueを用意
CanvasQueue canvasQueue(10, 16, 16);

//Discriminator用canvas
//Canvas targetCanvas(8, 8);

Canvas testCanvas(16, 16);

// --- タスク宣言 --------------------------------------------------

//トラックボールの回転認識のタスク
DeclareTaskLoop(TrackBallLeftRotationTask);
DeclareTaskLoop(TrackBallRightRotationTask);
DeclareTaskLoop(TrackBallUpRotationTask);
DeclareTaskLoop(TrackBallDownRotationTask);

//スケッチャー関連のタスク
DeclareTaskLoop(SketchCanvasFromTrackBallTask);

//パターン認識のタスク
DeclareTaskLoop(DiscriminatorTask);

//計算、アウトプットのタスク
//DeclareTaskLoop(CaluculateAndOutputTask);

// End

// --- セマフォ宣言 -------------------------------------------------------

//トラックボールのセマフォ
SemaphoreHandle trackBallLeftRotationSem;
SemaphoreHandle trackBallRightRotationSem;
SemaphoreHandle trackBallUpRotationSem;
SemaphoreHandle trackBallDownRotationSem;

//キャンバスキューのセマフォ
SemaphoreHandle canvasQueueSem;

// End

/*
// キャンバスの内容をシリアルモニタに描画
void Draw(Canvas &canvas)
{
    for (int y = 0; y < canvas.SizeY(); y++)
    {
        for (int x = 0; x < canvas.SizeX(); x++)
        {
            if (canvas.ReadPixel(x, y)) {
                Serial.print("@");
            }
            else {
                Serial.print(" ");
            }
        }
        Serial.println("");
    }
}
*/
void setup()
{
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

    //スケッチャー関連のタスク
    CreateTaskLoopWithStackSize(SketchCanvasFromTrackBallTask, LOW_PRIORITY, 200);

    //パターン認識のタスク
    CreateTaskLoopWithStackSize(DiscriminatorTask, LOW_PRIORITY, 200);

    //計算、アウトプットのタスク
    //CreateTaskLoop(CaluculateAndOutputTask);

    // --- セマフォ作成 -----------------------------------------------------------

    //トラックボールのセマフォ
    CreateBinarySemaphore(trackBallLeftRotationSem);
    CreateBinarySemaphore(trackBallRightRotationSem);
    CreateBinarySemaphore(trackBallUpRotationSem);
    CreateBinarySemaphore(trackBallDownRotationSem);

    //キャンバスキューのセマフォ
    CreateBinarySemaphore(canvasQueueSem);

    InitMainLoopStackSize(200);
}

void loop()
{
}

//パターン認識のタスク----------------------------------------------------------------------
TaskLoop(DiscriminatorTask)
{
    Canvas *work;
    STROKE stroke;

    enum STATE {
        PEEKING,
        DISCRIMINATING,
        POPING
    };

    static STATE state = STATE::PEEKING;

    switch (state) {
    case STATE::PEEKING:

        if (Acquire(canvasQueueSem, 1000))
        {
            if (canvasQueue.Peek(&work))
            {
                state = STATE::DISCRIMINATING;

            }
            Release(canvasQueueSem);
            Yield();
        }

        break;

    case STATE::DISCRIMINATING:
        SerialPrintCanvas(*work);
        //Draw(*work);
        Serial.println("");
        stroke = StrokeDiscriminator::Discriminate(*work);

        Serial.println(stroke);
        state = STATE::POPING;

        break;

    case STATE::POPING:

        if (Acquire(canvasQueueSem, 1000))
        {
            canvasQueue.Pop(&work);

            state = STATE::PEEKING;

            Release(canvasQueueSem);
        }
        break;
    }

}

//スケッチ関連のタスク----------------------------------------------------------------------
TaskLoop(SketchCanvasFromTrackBallTask)
{

    //Serial.println(ftSketcher.DeltaXYStayZeroTime());
    static bool isAlreadyStrokePushed = true;
    static bool isAlreadyLiteralPushed = true;
    static unsigned long lastSketchTime = millis();

    // CanvasQueue から作業領域キャンバスを確保できるまで待機
    // 前回ループでプッシュされない限り, 同じキャンバスを返す.
    while (true)
    {
        Canvas *work;

        if (canvasQueue.GetPushedReadyCanvas(&work))
        {
            ftSketcher.SetToCanvas(work);
            break;
        }
    }

    // --- トラックボールからdeltaX, deltaYを取得 --------------------
    if (Acquire(trackBallLeftRotationSem, 1000))
    {
        ftDriver.AddLeftSumToDeltaX();
        Release(trackBallLeftRotationSem);
    }

    if (Acquire(trackBallRightRotationSem, 1000))
    {
        ftDriver.AddRightSumToDeltaX();
        Release(trackBallRightRotationSem);
    }

    if (Acquire(trackBallUpRotationSem, 1000))
    {
        ftDriver.AddUpSumToDeltaY();
        Release(trackBallUpRotationSem);
    }

    if (Acquire(trackBallDownRotationSem, 1000))
    {
        ftDriver.AddDownSumToDeltaY();
        Release(trackBallDownRotationSem);
    }
    ftSketcher.SetDeltaXY(ftDriver.GetDeltaX(), ftDriver.GetDeltaY());

    ftDriver.ResetDeltaXY();
    // End トラックボールからdeltaX, deltaYを取得　------

    if (!ftSketcher.IsDeltaXYZero())
    {
        // deltaX, deltaYがともに0ではないときは,
        // 何らかの入力が行われたことなので,
        // ストローク, リテラルの区切りとした
        // pushフラグを下す.
        isAlreadyStrokePushed = false;
        isAlreadyLiteralPushed = false;
    }

    // スケッチに描画
    if (millis() - lastSketchTime > SKETCH_INTERVAL_TIME)
    {
        ftSketcher.Sketch();
        lastSketchTime = millis();
    }

    //Serial.println(ftSketcher.DeltaXYStayZeroTime());

    if (!isAlreadyStrokePushed && (ftSketcher.DeltaXYStayZeroTime() > STROKE_INTERVAL_TIME))
    {

        ftSketcher.CopyCanvas();

        if (Acquire(canvasQueueSem, 1000))
        {
            canvasQueue.Push();
            Release(canvasQueueSem);
        }

        // strokeプッシュ済みなのでフラグを上げる.
        isAlreadyStrokePushed = true;
        //Serial.println("StrokeP");
    }
    else if (!isAlreadyLiteralPushed && (ftSketcher.DeltaXYStayZeroTime() > LITERAL_INTERVAL_TIME))
    {
        ftSketcher.CopyCanvas();

        if (Acquire(canvasQueueSem, 1000))
        {
            canvasQueue.Push();
            Release(canvasQueueSem);
        }

        // literalプッシュ済みなのでフラグを上げる.
        isAlreadyLiteralPushed = true;
        //Serial.println("LiteralP");
    }
}

// --- LeftTask-------------------------------------------------------------

TaskLoop(TrackBallLeftRotationTask)
{
    ftDriver.ReadLeft();
    if (Acquire(trackBallLeftRotationSem, 1000))
    {
        ftDriver.AddLeftToSum();
        Release(trackBallLeftRotationSem);
    }
}

// --- rightTask--------------------------------------------------------------

TaskLoop(TrackBallRightRotationTask)
{
    ftDriver.ReadRight();

    if (Acquire(trackBallRightRotationSem, 1000))
    {
        ftDriver.AddRightToSum();
        Release(trackBallRightRotationSem);
    }
}

// --- UpTask--------------------------------------------------------------------

TaskLoop(TrackBallUpRotationTask)
{
    ftDriver.ReadUp();

    if (Acquire(trackBallUpRotationSem, 1000))
    {
        ftDriver.AddUpToSum();
        Release(trackBallUpRotationSem);
    }
}

// --- DownTask---------------------------------------------------------------------

TaskLoop(TrackBallDownRotationTask)
{
    ftDriver.ReadDown();

    if (Acquire(trackBallDownRotationSem, 1000))
    {
        ftDriver.AddDownToSum();
        Release(trackBallDownRotationSem);
    }
}
