/*
//
// 更新履歴:
//  2017/11/3:
//   初期版完成
*/


#ifndef STROKE_DISCRIMINATOR
#define STROKE_DISCRIMINATOR

#include "Canvas.h"

#include "Pattern.h"
#include "Stroke.h"


class StrokeDiscriminator {
private:

    static const int SCORE_INF = 10000;

    //
    // 指定されたキャンバスがベース指定されたキャンバスにどれだけ似ているかを,
    // スコア値によって表現します.
    //
    static int CalculateCorrelationScore(Canvas &base, Canvas &compare) {
        if (base.SizeX() != compare.SizeX() || base.SizeY() != compare.SizeY()) {
            return -1;
        }

        int sizeX = base.SizeX();
        int sizeY = base.SizeY();

        int score = 0;

        for (int x = 0; x < sizeX; x++) {
            for (int y = 0; y < sizeY; y++) {

                int baseDotCount = 0;
                int compareDotCount = 0;

                // ---- スキャン窓 -------------
                for (int i = 0; i <= 1; i++) {
                    for (int j = 0; j <= 1; j++) {

                        // 範囲チェック
                        if ((x + i < 0) || (x + i >= sizeX)
                            || (y + j < 0) || (y + j >= sizeY)) {
                            continue;
                        }

                        if (compare.ReadPixel(x + i, y + j)) {
                            compareDotCount++;
                        }

                        if (base.ReadPixel(x + i, y + j)) {
                            baseDotCount++;
                        }
                    }

                }
                // End スキャン窓 -------

                score += (compareDotCount - baseDotCount) * (compareDotCount - baseDotCount);


                /*
                if (base.ReadPixel(x, y) != compare.ReadPixel(x, y)) {
                    score += 1;
                }
                */

                /*

                bool isDetected = false;

                // ---- 基準点と周り8近傍を調べる -------------
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {

                        // 範囲チェック
                        if ((i == 0 && j == 0) || (x + i < 0) || (x + i >= sizeX)
                            || (y + j < 0) || (y + j >= sizeY)) {
                            continue;
                        }

                        // 異なる場合
                        if (base.ReadPixel(x + i, y + j) != canvasB.ReadPixel(x + i, y + j)) {
                            
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
                */

            }
        }


        return score;
    }


public:

    static const int STROKE_PATTERNS_COUNT = 17;

    static STROKE Discriminate(Canvas &target) {

        int minScore = SCORE_INF;
        STROKE similarStroke = STROKE::STROKE_SPACE;

        Canvas pattern(8, 8);
        



        for (int i = 0; i < STROKE_PATTERNS_COUNT; i++) {
            // 白紙に戻す
            pattern.color = false;
            pattern.Boxf(0, 0, 7, 7);

            // パターン画像を描画する
            pattern.color = true;
            pattern.Pos(0, 0);
            pattern.Celput(strokePatterns[i]);

            // スコアを求める
            int score = CalculateCorrelationScore(pattern, target);
            Serial.println(score);
            if (score >= 0 && score <= minScore) {
                minScore = score;

                similarStroke = (STROKE)i;
            }
            
        }

        return similarStroke;
    }

};



#endif