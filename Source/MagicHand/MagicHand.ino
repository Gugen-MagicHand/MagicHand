
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

//caluculatorの用意
Calculator cal(10, 10);

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

// オペレータ一覧
OperatorDivide operatorDivide;
OperatorLeftBracket operatorLeftBracket;
OperatorMinus operatorMinus;
OperatorMultiply operatorMultiply;
OperatorPlus operatorPlus;
OperatorRightBracket operatorRightBracket;

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

    // 計算機のオペランドスタックに0を代入
    cal.Put(Fraction(0));


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

Operator *LiteralToOperatorPointer(LITERAL lit) {
    if (lit == LITERAL::LITERAL_LEFT_BRACKET) {
        return &operatorLeftBracket;
    }
    else if (lit == LITERAL::LITERAL_RIGHT_BRACKET) {
        return &operatorRightBracket;
    }
    else if (lit == LITERAL::LITERAL_MINUS) {
        return &operatorMinus;
    }
    else if (lit == LITERAL::LITERAL_PLUS) {
        return &operatorPlus;
    }
    else if (lit == LITERAL::LITERAL_MULTIPLY) {
        return &operatorMultiply;
    }
    else if (lit == LITERAL::LITERAL_DIVIDE) {
        return &operatorDivide;
    }

    return 0x00;
}

//計算、アウトプットのタスク----------------------------------------------------------------
TaskLoop(CaluculateAndOutputTask) {

    

    static CALCULATOR_PHASE phase = CALCULATOR_PHASE::CALCULATOR_PHASE_FOMULA_FIRST_INPUT;

    static Operator *pointerToOperator;

    //Serial.println("C");

    static Calculator::CAL_STATUS calStatus = Calculator::CAL_STATUS::CAL_STATUS_SUCCESS;

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
        display.AssembledLiteralQueuePush(lit);

        switch (phase) {

            // --- 式最初の入力待機段階 -----------------------------------------------------------------------
        case CALCULATOR_PHASE::CALCULATOR_PHASE_FOMULA_FIRST_INPUT:


            if (LiteralIsNumeric(lit)) {
                // 数字が入力されたとき

                // 計算機を初期化する.
                cal.ClearAllStacks();

                // 現在の文字分数に数字を書く
                if (literalFraction.Put(lit)) {
                    display.FormulaLiteralQueuePush(lit);
                }

                // Operand入力段階に移行する
                phase = CALCULATOR_PHASE::CALCULATOR_PHASE_OPERAND_INPUT;

            }
            else if (lit == LITERAL::LITERAL_MINUS ||
                lit == LITERAL::LITERAL_PLUS ||
                lit == LITERAL::LITERAL_DIVIDE ||
                lit == LITERAL::LITERAL_MULTIPLY) {

                // 作業中Operatorに代入する.
                pointerToOperator = LiteralToOperatorPointer(lit);

                // display表示
                display.FormulaLiteralQueuePush(lit);

                // Operator入力段階に移行する.
                phase = CALCULATOR_PHASE::CALCULATOR_PHASE_OPERATOR_INPUT;


            }
            else if (lit == LITERAL::LITERAL_LEFT_BRACKET) {
                // 式先頭に左括弧が来たときは, 計算機に左括弧を代入し, PHASE_CHILD_FOMULA_FIRST_INPUTに移行する.

                // 計算機のスタックを初期化する.
                cal.ClearAllStacks();

                // 左括弧を計算機に代入する
                calStatus = cal.Put(&operatorLeftBracket);

                // display表示
                display.FormulaLiteralQueuePush(lit);

                // PHASE_CHILD_FOMULA_FIRST_INPUTに移行する
                phase = CALCULATOR_PHASE::CALCULATOR_PHASE_CHILD_FOMULA_FIRST_INPUT;

            }

            break; // End 式最初の入力段階 ----------------------------

            // --- 子式最初の入力段階 -----------------------------------------------------------------
        case CALCULATOR_PHASE::CALCULATOR_PHASE_CHILD_FOMULA_FIRST_INPUT:


            if (LiteralIsNumeric(lit)) {
                // 数字が入力されたときは, 文字分数に数字を代入してPHASE_OPERAND_INPUTに移行する.

                // 現在の文字分数に数字を書く
                if (literalFraction.Put(lit)) {


                    display.FormulaLiteralQueuePush(lit);
                }

                // Operand入力段階に移行する
                phase = CALCULATOR_PHASE::CALCULATOR_PHASE_OPERAND_INPUT;

            }
            else if (lit == LITERAL::LITERAL_MINUS ||
                lit == LITERAL::LITERAL_PLUS ||
                lit == LITERAL::LITERAL_DIVIDE ||
                lit == LITERAL::LITERAL_MULTIPLY) {

                calStatus = cal.Put(Fraction(0));

                // 作業中Operatorに代入する.
                pointerToOperator = LiteralToOperatorPointer(lit);

                // display表示
                display.FormulaLiteralQueuePush(lit);

                // Operator入力段階に移行する.
                phase = CALCULATOR_PHASE::CALCULATOR_PHASE_OPERAND_INPUT;


            }
            else if (lit == LITERAL::LITERAL_LEFT_BRACKET) {
                // 式先頭に左括弧が来たときは, 計算機に左括弧を代入し, PHASE_CHILD_FOMULA_FIRST_INPUTに移行する.


                // 左括弧を計算機に代入する
                calStatus = cal.Put(&operatorLeftBracket);

                // display表示
                display.FormulaLiteralQueuePush(lit);

                // PHASE_CHILD_FOMULA_FIRST_INPUTに移行する
                phase = CALCULATOR_PHASE::CALCULATOR_PHASE_CHILD_FOMULA_FIRST_INPUT;

            }




            break; // End 子式最初の入力段階 --------------------------

            // --- Operand入力段階 --------------------------------------------------------------------
        case CALCULATOR_PHASE::CALCULATOR_PHASE_OPERAND_INPUT:

            if (LiteralIsNumeric(lit) || lit == LITERAL::LITERAL_DOT) {
                // 数字, もしくは小数点が来た場合


                // 文字分数に代入
                if (literalFraction.Put(lit)) {



                    display.FormulaLiteralQueuePush(lit);
                }
            }
            else if (lit == LITERAL::LITERAL_DIVIDE ||
                lit == LITERAL::LITERAL_MINUS ||
                lit == LITERAL::LITERAL_PLUS ||
                lit == LITERAL::LITERAL_MULTIPLY) {

                // 四則演算子が来た場合, 現在の文字分数をoperandとして計算機に代入する.
                // その後, PHASE_OPERATOR_INPUTに移行する.

                // 現在のoperandを計算機に代入
                calStatus = cal.Put(literalFraction.ToFraction());
                literalFraction.Clear();


                display.FormulaLiteralQueuePush(lit);

                // 現在のoperatorとして記録
                pointerToOperator = LiteralToOperatorPointer(lit);


                phase = CALCULATOR_PHASE::CALCULATOR_PHASE_OPERATOR_INPUT;
            }

            else if (lit == LITERAL::LITERAL_RIGHT_BRACKET) {
                // 右括弧が来た場合


                // 現在のoperandを計算機に代入
                calStatus = cal.Put(literalFraction.ToFraction());
                literalFraction.Clear();

                calStatus = cal.Put(lit);


                display.FormulaLiteralQueuePush(lit);

                phase = CALCULATOR_PHASE::CALCULATOR_PHASE_OPERATOR_INPUT;

            }
            else if (lit == LITERAL::LITERAL_EQUAL) {


                // 現在のoperandを計算機に代入
                calStatus = cal.Put(literalFraction.ToFraction());
                literalFraction.Clear();

                display.FormulaLiteralQueuePush(lit);

                calStatus = cal.Compute();



                phase = CALCULATOR_PHASE::CALCULATOR_PHASE_FOMULA_FIRST_INPUT;
            }




            break; // End Operand入力段階 --------------------------------




            // --- Operator入力段階 ---------------------------------------------------------------------
        case CALCULATOR_PHASE::CALCULATOR_PHASE_OPERATOR_INPUT:


            if (lit == LITERAL::LITERAL_DIVIDE ||
                lit == LITERAL::LITERAL_MINUS ||
                lit == LITERAL::LITERAL_PLUS ||
                lit == LITERAL::LITERAL_MULTIPLY) {

                // 四則演算子が来た場合, 現在のoperatorとして記録する.

                pointerToOperator = LiteralToOperatorPointer(lit);

                // 前にあるoperatorを消去(一文字分)してから, 新しく描く
                LITERAL temp;
                display.FormulaLiteralQueuePopBack(&temp);
                display.FormulaLiteralQueuePush(lit);
            }

            else if (LiteralIsNumeric(lit)) {
                // 数字が来た場合は, 現在のOperatorを決定演算子として, 計算機に代入し,
                // OperandPhaseに移行する.

                if (pointerToOperator != 0x00) {

                    // operatorを計算機に代入する.
                    calStatus = cal.Put(pointerToOperator);

                    // 数字を文字分数に代入する.
                    if (literalFraction.Put(lit)) {



                        display.FormulaLiteralQueuePush(lit);
                    }

                    // phaseをOperandPhaseに移動する.
                    phase = CALCULATOR_PHASE::CALCULATOR_PHASE_OPERAND_INPUT;
                }
            }

            else if (lit == LITERAL::LITERAL_LEFT_BRACKET) {
                // 左括弧が来た場合は, 現在のoperatorを決定演算子として,
                // 計算機に代入する. その後, PHASE_CHILD_FOMULA_FIRST_INPUTに移行する.

                if (pointerToOperator != 0x00) {

                    // operatorを計算機に代入する.
                    calStatus = cal.Put(pointerToOperator);

                    display.AssembledLiteralQueuePush(lit);

                    // Left bracketを計算機に代入する.
                    calStatus = cal.Put(&operatorLeftBracket);

                    // PHASE_CHILD_FOMULA_FIRST_INPUTに移行
                    phase = CALCULATOR_PHASE::CALCULATOR_PHASE_CHILD_FOMULA_FIRST_INPUT;
                }
            }
            else if (lit == LITERAL::LITERAL_EQUAL) {

                display.FormulaLiteralQueuePush(lit);

                calStatus = cal.Compute();



                phase = CALCULATOR_PHASE::CALCULATOR_PHASE_FOMULA_FIRST_INPUT;
            }
            break;
            // End operator入力段階 ------------------------------
        }





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
    calStatus = cal.TopOfOperandStack(&display.answerFraction);

    // DisplayのCalStatusを更新
    display.calStatus = calStatus;

    // DisplayのcalPhaseを更新
    display.calPhase = phase;


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
