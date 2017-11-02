#include "StrokeDiscriminator.h"


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

void setup() {

    Serial.begin(19200);

    Canvas pattern(8, 8);

    for (int i = 0; i < StrokeDiscriminator::STROKE_PATTERNS_COUNT; i++) {
        // 白紙に戻す
        pattern.color = false;
        pattern.Boxf(0, 0, 7, 7);

        // パターン画像を描画する
        pattern.color = true;
        pattern.Pos(0, 0);
        pattern.Celput(strokePatterns[i]);


        Draw(pattern);

        STROKE guessed = StrokeDiscriminator::Discriminate(pattern);
        Serial.println((int)guessed);

        Serial.println("");
    }
}

void loop() {

}
