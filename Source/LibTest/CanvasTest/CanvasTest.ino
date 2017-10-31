

#include "Canvas.h"



// 数字が描画されたキャンバス(8x8)
Canvas numberImage(8, 8);

// 描画したい数字文字
char num = '7';

// 拡大されたキャンバス(サイズ任意 ただしメモリ容量に気を付けること)
Canvas scaledImage(32, 32);

// キャンバスの内容をシリアルモニタに描画
void Draw(Canvas &canvas)
{
    for (int y = 0; y < canvas.SizeY(); y++)
    {
        for (int x = 0; x < canvas.SizeX(); x++)
        {
            if(canvas.ReadPixcel(x, y)){
                Serial.print("*");
            }
            else{
                Serial.print(" ");
            }
        }
        Serial.println("");
    }
}

void setup()
{
    // シリアル通信の開始
    Serial.begin(19200);

    // 数字を描画
    numberImage.Pos(0,0);
    numberImage.color = true;
    numberImage.PutChar(num);

    

    // スケール変換
    numberImage.ScaleTo(scaledImage);

    //InitMainLoopStackSize(100);

}

void loop()
{
    // スケール変換前の画像を描画
    Draw(numberImage);

    // スケール変換後の画像を描画
    Draw(scaledImage);

    while (true);
}
