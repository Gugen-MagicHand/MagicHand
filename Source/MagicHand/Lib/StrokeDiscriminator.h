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

    static const int PATTERN_PIXEL_MAX_VAL = 255;

    static const int PATTERN_SIZE = 16;


    //

    static float CalculateCorrelationScore(Canvas &compare, uint8_t *pattern) {

        if (compare.SizeX() != PATTERN_SIZE || compare.SizeY() != PATTERN_SIZE) {
            return -1.0;
        }


        float score = 0.0;

        for (int y = 0; y < PATTERN_SIZE; y++) {
            for (int x = 0; x < PATTERN_SIZE; x++) {

                float patternPixelVal = ((unsigned char)pgm_read_byte_near(&pattern[x + y * PATTERN_SIZE])) / (float)PATTERN_PIXEL_MAX_VAL;

                float comparePixelVal = 0.0;

                if (compare.ReadPixel(x, y)) {
                    comparePixelVal = 1.0;
                }
                else {
                    comparePixelVal = 0.0;
                }

                score += (comparePixelVal - patternPixelVal) * (comparePixelVal - patternPixelVal);
            }
        }

        return score;
    }


public:

    static const int STROKE_PATTERNS_COUNT = 17;

    static STROKE Discriminate(Canvas &target) {

        float minScore = SCORE_INF;
        STROKE similarStroke = STROKE::STROKE_SPACE;

        for (int i = 0; i < STROKE_PATTERNS_COUNT; i++) {

            // スコアを求める
            float score = CalculateCorrelationScore(target, (uint8_t *)&strokePatterns[i]);

            //Serial.println(score);
            if (score >= 0.0 && score <= minScore) {
                minScore = score;

                similarStroke = (STROKE)i;
            }

        }

        return similarStroke;
    }

};



#endif