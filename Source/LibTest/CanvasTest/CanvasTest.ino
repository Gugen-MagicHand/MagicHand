

#include "Canvas.h"



// 数字が描画されたキャンバス(8x8)
Canvas numberImage(5, 7);

// 描画したい数字文字
char num = '7';

// 拡大されたキャンバス(サイズ任意 ただしメモリ容量に気を付けること)
Canvas scaledImage(8, 8);

// キャンバスの内容をシリアルモニタに描画
void Draw(Canvas &canvas)
{
    for (int y = 0; y < canvas.SizeY(); y++)
    {
        for (int x = 0; x < canvas.SizeX(); x++)
        {
            if (canvas.ReadPixel(x, y)) {
                Serial.print("■");
            }
            else {
                Serial.print("□");
            }
        }
        Serial.println("");
    }
}

void setup()
{
    // シリアル通信の開始
    Serial.begin(19200);

    for (int i = 0; i < 10; i++) {
        numberImage.color = false;
        numberImage.Boxf(0, 0, numberImage.SizeX(), numberImage.SizeY());

        scaledImage.color = false;
        scaledImage.Boxf(0, 0, scaledImage.SizeX(), scaledImage.SizeY());


        numberImage.color = true;
        numberImage.Pos(0, 0);
        numberImage.PutChar('(' + i);


        // コピー元すべての領域を自身すべての領域に変倍コピー
        scaledImage.Pos(0, 0);
        scaledImage.Zoom(6, scaledImage.SizeY(),
            numberImage, 0, 0, numberImage.SizeX(), numberImage.SizeY());

        Draw(scaledImage);
        Serial.println("");
    }


    //// 数字を描画
    //numberImage.Pos(0, 0);
    //numberImage.color = true;
    //numberImage.PutChar(num);



    //// --- スケール変換 -----------------
    //// カレントポジションを(0, 0)にする.
    //scaledImage.Pos(0, 0);

    //// コピー元すべての領域を自身すべての領域に変倍コピー
    //scaledImage.Zoom(scaledImage.SizeX(), scaledImage.SizeY(),
    //    numberImage, 0, 0, numberImage.SizeX(), numberImage.SizeY());

    //// End スケール変換 ------

    /*

    // --- コピー -----------------------
    // カレントポジションを(0, 0)にする.
    scaledImage.Pos(10, 0);

    // コピー元のすべてぼ領域を自身のカレントポジションへコピー
    scaledImage.Copy(numberImage, 0, 0, numberImage.SizeX(), numberImage.SizeY());
    // End コピー -------------

    */
    //InitMainLoopStackSize(100);

}

void loop()
{
    //    // スケール変換前の画像を描画
    //    Draw(numberImage);
    //
    //    // スケール変換後の画像を描画
    //    Draw(scaledImage);
    //
    while (true);
}
