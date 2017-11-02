
#ifndef STROKE_DISCRIMINATOR
#define STROKE_DISCRIMINATOR

#include "Canvas.h"

#include "Pattern.h"
#include "Stroke.h"


class StrokeDiscriminator {
private:

    

    //
    // 二つのキャンバスの類似度をスコア値によって表現します.
    // スコアが低い方がよく似ています.
    //
    static int CalculateCorrelationScore(Canvas &canvasA, Canvas &canvasB) {
        if (canvasA.SizeX() != canvasB.SizeX() || canvasA.SizeY() != canvasB.SizeY()) {
            return -1;
        }

        int sizeX = canvasA.SizeX();
        int sizeY = canvasA.SizeY();

        int score = 0;

        for (int x = 0; x < sizeX; x++) {
            for (int y = 0; y < sizeY; y++) {
                bool isDetected = false;

                // ---- 自分と周り8近傍を調べる -------------
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {

                        // 範囲チェック
                        if ((i == 0 && j == 0) || (x + i < 0) || (x + i >= sizeX)
                            || (y + j < 0) || (y + j >= sizeY)) {
                            continue;
                        }

                        if (canvasA.ReadPixel(x + i, y + j) == canvasB.ReadPixel(x + i, y + j)) {
                            if (i == 0 && j == 0) {
                                score += 2;
                            }
                            else {
                                score += 1;
                            }

                            isDetected = true;
                        }

                        if (isDetected) {
                            break;
                        }
                    
                    
                    }

                    if (isDetected) {
                        break;
                    }
                }

                // End 自分と周り8近傍を調べる -------


            }
        }


        return score;
    }


public:

    static const int STROKE_PATTERNS_COUNT = 17;

    static STROKE Discriminate(Canvas &target) {

        int minScore = 0;
        STROKE res = STROKE::STROKE_SPACE;

        Canvas pattern(8, 8);
        



        for (int i = 0; i < STROKE_PATTERNS_COUNT; i++) {
            // 白紙に戻す
            pattern.color = false;
            pattern.Boxf(0, 0, 7, 7);

            // パターン画像を描画する
            pattern.color = true;
            pattern.Pos(0, 0);
            pattern.Celput(strokePatterns[i]);


            
        }
    }

};



#endif