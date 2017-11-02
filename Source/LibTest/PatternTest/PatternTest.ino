#include "Canvas.h"

Canvas canvas(8, 8);


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
}



void loop() {
    for (int i = 0; i < 14; i++) {
        canvas.color = false;
        canvas.Boxf(0, 0, canvas.SizeX(), canvas.SizeY());

        canvas.color = true;
        canvas.Pos(0, 0);

        switch (i) {
        case 0:
            canvas.Celput(pattern0);
            break;
        case 1:
            break;
        case 2:
            canvas.Celput(pattern2);
            break;
        case 3:
            canvas.Celput(pattern3);
            break;
        case 4:
            canvas.Celput(pattern4);
            break;
        case 5:
            canvas.Celput(pattern5);
            break;
        case 6:
            canvas.Celput(pattern6);
            break;
        case 7:
            canvas.Celput(pattern7);
            break;
        case 8:
            canvas.Celput(pattern8);
            break;
        case 9:
            canvas.Celput(pattern9);
            break;
        case 10:
            canvas.Celput(patternVerticalLine);
            break;


        case 11:
            canvas.Celput(patternHorizontalLine);
            break;


        case 12:
            canvas.Celput(patternSlash);
            break;


        case 13:
            canvas.Celput(patternBackSlash);
            break;
        }

        Draw(canvas);
        Serial.println("");
    }

    while (true);
}