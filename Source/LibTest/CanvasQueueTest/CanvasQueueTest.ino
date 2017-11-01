#include "CanvasQueue.h"



// キャンバスの内容をシリアルモニタに描画
void Draw(Canvas &canvas)
{
    for (int y = 0; y < canvas.SizeY(); y++)
    {
        for (int x = 0; x < canvas.SizeX(); x++)
        {
            if (canvas.ReadPixel(x, y)) {
                Serial.print("*");
            }
            else {
                Serial.print(" ");
            }
        }
        Serial.println("");
    }
}


void setup() {
    Serial.begin(19200);


    // サイズは10に設定
    CanvasQueue canvasQueue(5, 32, 32);

    Canvas *work;

    for (int i = 0; i < 3; i++) {

        if (canvasQueue.GetPushedReadyCanvas(&work)) {
            // 正常に取得できた
            Serial.println("Get.");

            // 数字を描く
            work->color = true;
            work->PutChar('0' + i);

            canvasQueue.Push();

        }
    }
    canvasQueue.Pop(&work);

    for (int i = 3; i < 7; i++) {

        if (canvasQueue.GetPushedReadyCanvas(&work)) {
            // 正常に取得できた
            Serial.println("Get.");

            // 数字を描く
            work->color = true;
            work->PutChar('0' + i);

            canvasQueue.Push();

        }
    }
    Serial.println(canvasQueue.Count());

    while (!canvasQueue.IsEmpty()) {

        if (canvasQueue.Pop(&work)) {
            // 正常に取得できた
            Serial.println("Pop.");

            // シリアルモニタに表示
            Draw(*work);
        }
    }

}

void loop() {


}
