
// Main header
#include "Lib/MagicHand.h"

static const int PIN_TRACK_BALL_BUTTON = 13;
static const int PIN_TRACK_BALL_LEFT = 12;
static const int PIN_TRACK_BALL_RIGHT = 11;
static const int PIN_TRACK_BALL_UP = 10;
static const int PIN_TRACK_BALL_DOWN = 8;
static const int PIN_TRACK_BALL_WHITE = 7;
static const int PIN_TRACK_BALL_GREEN = 6;
static const int PIN_TRACK_BALL_RED = 5;
static const int PIN_TRACK_BALL_BLUE = 9;


static const int PIN_TFT_RS = 40;
static const int PIN_TFT_RST = 38;
static const int PIN_TFT_CS = 36;


static const unsigned long STROKE_INTERVAL_TIME = 200;
static const unsigned long WORD_INTERVAL_TIME = 500;

static const unsigned long SKETCH_INTERVAL_TIME = 10;




// FingerTrackDriverの用意
FingerTrackDriver ftDriver;

// FingerTrackSketcherの用意
FingerTrackSketcher ftSketcher;

// StrokeAssemblerの用意
StrokeAssembler strokeAssembler;

// LiteralFractionの用意
LiteralFraction literalFraction;

// CanvasQueueを用意
CanvasQueue canvasQueue(6, 15, 15);

//Discriminator用canvas
//Canvas targetCanvas(8, 8);

//Canvas testCanvas(16, 16);

//StrokeQueueの用意
Queue<STROKE> strokeQueue(10);



CalculateController calculateController(10, 10);


// CalculatorDisplayの用意
CalculatorDisplay display(PIN_TFT_CS, PIN_TFT_RS, PIN_TFT_RST);

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
DeclareTaskLoop(CaluculateAndOutputTask);

// End

// --- セマフォ宣言 -------------------------------------------------------

//トラックボールのセマフォ
SemaphoreHandle trackBallLeftRotationSem;
SemaphoreHandle trackBallRightRotationSem;
SemaphoreHandle trackBallUpRotationSem;
SemaphoreHandle trackBallDownRotationSem;

//キャンバスキューのセマフォ
SemaphoreHandle canvasQueueSem;

//ストロークキューのセマフォ
SemaphoreHandle strokeQueueSem;

// End セマフォ宣言 ------------------


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
                Serial.print("_");
            }
        }
        Serial.println("");
    }
}

*/

void OnPopBackLiteralFromFormula() {
    char ch;
    display.FormulaCharQueuePopBack(&ch);
}

void OnPushLiteralIntoFormula(LITERAL lit) {
    display.FormulaCharQueuePush(LiteralToChar(lit));
}

//セットアップ関数---------------------------------------------------------------------

void setup()
{
    //シリアル通信の開始（デバッグ用、本番はコメントアウトする。）
    Serial.begin(19200);

    //FingerTrackDriverのピン設定、初期化
    ftDriver.Begin(PIN_TRACK_BALL_BUTTON, PIN_TRACK_BALL_LEFT, PIN_TRACK_BALL_RIGHT, PIN_TRACK_BALL_UP, PIN_TRACK_BALL_DOWN,
        PIN_TRACK_BALL_WHITE, PIN_TRACK_BALL_GREEN, PIN_TRACK_BALL_RED, PIN_TRACK_BALL_BLUE);

    // ディスプレイの開始
    display.Begin();


    calculateController.onPopBackLiteralFromFormula = OnPopBackLiteralFromFormula;
    calculateController.onPushLiteralIntoFormula = OnPushLiteralIntoFormula;


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
    CreateTaskLoopWithStackSize(CaluculateAndOutputTask, LOW_PRIORITY, 200);

    // --- セマフォ作成 -----------------------------------------------------------

    //トラックボールのセマフォ
    CreateBinarySemaphore(trackBallLeftRotationSem);
    CreateBinarySemaphore(trackBallRightRotationSem);
    CreateBinarySemaphore(trackBallUpRotationSem);
    CreateBinarySemaphore(trackBallDownRotationSem);

    //キャンバスキューのセマフォ
    CreateBinarySemaphore(canvasQueueSem);

    //ストロークキューのセマフォ
    CreateBinarySemaphore(strokeQueueSem);

    //InitMainLoopStackSize(200);


}


// ループ関数--------------------------------------------------------------------------------
void loop()
{
    /*
    LITERAL lit;

    LiteralQueue literalQueue(5);

    literalQueue.Push(LITERAL_0);
    literalQueue.Push(LITERAL_1);

    literalQueue.Pop(&lit);
    literalQueue.PopBack(&lit);
    literalQueue.Push(LITERAL_3);

    //Serial.println(literalQueue.Count());

    for (int i = 0; i < literalQueue.Count(); i++) {
        Serial.println(literalQueue[i]);
    }

    while (true);
    */
    /*
    literalFraction.Put(LITERAL::LITERAL_1);
    literalFraction.Put(LITERAL::LITERAL_DOT);
    literalFraction.Put(LITERAL::LITERAL_2);
    literalFraction.Put(LITERAL::LITERAL_8);
    // literalFraction.BackSpace();
    // literalFraction.BackSpace();
    // literalFraction.BackSpace();
    //Serial.println(literalFraction.SignificandCount());
    //literalFraction.Put(LITERAL::LITERAL_2);
    Serial.println(literalFraction.ToFraction().ToString());

    while (true);
    */
}

//------------------------------------------------------------------------------------------

/*
String CalculatorStatusToString(Calculator::CAL_STATUS status) {
    switch (status) {
    case Calculator::CAL_STATUS::CAL_STATUS_MISMATCHED_PARENTHESES:
        return String(F("MIS_PAREN"));

    case Calculator::CAL_STATUS::CAL_STATUS_OPERAND_IS_NOT_ENOUGH:
        return String(F("OPRND_NOT_ENOUGH"));

    case Calculator::CAL_STATUS::CAL_STATUS_SOMETHING_ERROR:
        return String(F("SOME_ERR"));

    case Calculator::CAL_STATUS::CAL_STATUS_STACK_IS_EMPTY:
        return String(F("STACK_EMP"));

    case Calculator::CAL_STATUS::CAL_STATUS_STACK_IS_FULL:
        return String(F("STACK_FUL"));

    case Calculator::CAL_STATUS::CAL_STATUS_SUCCESS:
        return String(F("SUCCESS"));
    }

    return String(F("?"));
}
*/



//計算、アウトプットのタスク----------------------------------------------------------------


TaskLoop(CaluculateAndOutputTask) {

    

    //Serial.println("C");

    STROKE stroke;

    //static Fraction resultFrac(0);
    //static Operator* resultOp;

    static bool canAssemble = false;

    if (Acquire(strokeQueueSem, 1000)) {
        if (strokeQueue.Pop(&stroke)) {

            //Serial.println("Pop");
            canAssemble = true;

        }
        Release(strokeQueueSem);
        Yield();
    }

    if (canAssemble) {
        //Serial.println("s");
        strokeAssembler.Assemble(stroke);
        canAssemble = false;
    }

    LITERAL lit;
    //Serial.println(strokeAssembler.literalQueue.Count());
    while (strokeAssembler.literalQueue.Pop(&lit)) {
        // Literalごとの処理を行う

        // まず, 組み立て結果文字をDisplayに表示する.
        display.AssembledCharQueuePush(LiteralToChar(lit));

        
        // DisplayのCalStatusを更新
        display.calStatus = calculateController.Put(lit);




        // Serial.println(LiteralToString(lit));
        //Serial.println(phase);
        // displayのfomulaLiteralQueueに代入
        //display.FormulaLiteralQueuePush(lit);

    }


    // CanvasQueueのCountを取得
    int canvasQueueCount;
    if (Acquire(canvasQueueSem, 1000)) {

        canvasQueueCount = canvasQueue.Count();

        Release(canvasQueueSem);
    }

    display.canvasQueueCount = canvasQueueCount;

    // 計算機のオペランドスタックの先頭をdisplayのanswer欄に表示する.
    calculateController.calculator.TopOfOperandStack(&display.answerFraction);


    // DisplayのcalPhaseを更新
    display.calPhase = calculateController.Phase();


    // Displayの更新
    display.Update();

    if (canvasQueueCount > 0) {
        ftDriver.LEDColor(true, true, false, false);
    }
    else {
        ftDriver.LEDColor(false, true, true, false);
    }
}


//パターン認識のタスク----------------------------------------------------------------------
TaskLoop(DiscriminatorTask)
{

    //Serial.println("D");
    //Serial.println("Test");
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

                //Serial.println("Peek");
            }
            Release(canvasQueueSem);
            Yield();
        }


        break;

    case STATE::DISCRIMINATING:
        //SerialPrintCanvas(*work);
        //Draw(*work);
        //Serial.println("-");
        stroke = StrokeDiscriminator::Discriminate(*work);

        //Serial.println("Dis");
        if (Acquire(strokeQueueSem, 1000))
        {
            //Serial.println("Push");
            strokeQueue.Push(stroke);

            Release(strokeQueueSem);
        }
        //Serial.println(stroke);
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

    //Serial.println("S");
    //Serial.println(ftSketcher.DeltaXYStayZeroTime());
    static bool isAlreadyStrokePushed = true;
    static bool isAlreadyWordPushed = true;
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
        // ストローク, Wordの区切りとした
        // pushフラグを下す.
        isAlreadyStrokePushed = false;
        isAlreadyWordPushed = false;
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
    else if (!isAlreadyWordPushed && (ftSketcher.DeltaXYStayZeroTime() > WORD_INTERVAL_TIME))
    {
        ftSketcher.CopyCanvas();

        if (Acquire(canvasQueueSem, 1000))
        {
            canvasQueue.Push();
            Release(canvasQueueSem);
        }

        // wordプッシュ済みなのでフラグを上げる.
        isAlreadyWordPushed = true;
        //Serial.println("LiteralP");
    }
}

// --- LeftTask-------------------------------------------------------------

TaskLoop(TrackBallLeftRotationTask)
{

    //Serial.println("L");
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

    //Serial.println("R");
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

    //Serial.println("U");
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

    //Serial.println("Do");
    ftDriver.ReadDown();

    if (Acquire(trackBallDownRotationSem, 1000))
    {
        ftDriver.AddDownToSum();
        Release(trackBallDownRotationSem);
    }
}
