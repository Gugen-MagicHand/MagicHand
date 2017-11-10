/*
//
// 更新履歴:
//  2017/11/3:
//   初期版完成
*/


#ifndef STROKE_DISCRIMINATOR
#define STROKE_DISCRIMINATOR

#include "Canvas.h"

#include "StrokeCellData.h"
#include "Stroke.h"
#include "HOG.h"


class StrokeDiscriminator {
private:

    static const int SCORE_INF = 10000;

    static const int HOG_FEATURE_MAX_VAL = 255;

    static const int IMAGE_SIZE = 15;
    static const int CELL_COUNT = 9;
    static const int HOG_FEATURES_COUNT = 81;

    static const float THRES_STROKE = 0.1;


    static const int CELL_SIZE = 5;
    static const int BLOCK_SIZE = 3;
    static const int ORIENTATION = 9;

    static const float GAMMA = 0.8;


    static double baseImage[IMAGE_SIZE * IMAGE_SIZE];

    static DSP::HOG::HOG_Feature baseFeatures[HOG_FEATURES_COUNT];
    static char baseCellData[CELL_COUNT * 2];

    //

    static int CalculateSimilarity(uint8_t *compareCellData) {
        int similarity = 0;

        for (int i = 0; i < CELL_COUNT; i++) {

            int compareOri = (char)pgm_read_byte_near(&compareCellData[i * 2]);
            int compareOriVar = (char)pgm_read_byte_near(&compareCellData[i * 2 + 1]);

            int baseOri = baseCellData[i * 2];
            int baseOriVar = baseCellData[i * 2 + 1];

            // 片方には方向がなく, 他方が方向があるとき
            if ((baseOri < 0 && compareOri >= 0) || (baseOri >= 0 && compareOri < 0)) {
                similarity--;

                continue;
            }

            // 両方に方向がないとき
            if (baseOri < 0 && compareOri < 0) {
                similarity++;

                continue;
            }


            // 両方に方向があるとき


            // 方向の差を求める.
            // 折り返すことに注意
            int oriDelta = abs(baseOri - compareOri);
            if (oriDelta * 2 >= ORIENTATION) {
                oriDelta = ORIENTATION - oriDelta;
            }

            //
            if (oriDelta <= compareOriVar * GAMMA) {
                similarity++;
            }
            else {
                similarity--;
            }
        }

        return similarity;

        /*

        float score = 0.0;
        for (int i = 0; i < HOG_FEATURES_COUNT; i++) {
            float compareFeature = (unsigned char)pgm_read_byte_near(&compareFeatures[i]) / (float)HOG_FEATURE_MAX_VAL;
            float baseFeature = (baseFeatures[i].featureVal);

            score += abs(baseFeature - compareFeature);
        }

        return score;
        */
        /*
        float sumOfBaseFeatures = 0.0;
        float sumOfCompareFeatures = 0.0;
        float dot = 0.0;

        for (int i = 0; i < HOG_FEATURES_COUNT; i++) {
            float compareFeature = ((unsigned char)pgm_read_byte_near(&compareFeatures[i])) / (float)HOG_FEATURE_MAX_VAL;

            sumOfBaseFeatures += baseFeatures[i].featureVal * baseFeatures[i].featureVal;
            sumOfCompareFeatures += compareFeature * compareFeature;

            dot += baseFeatures[i].featureVal * compareFeature;
        }

        // 両方点のとき
        if (sumOfBaseFeatures == 0.0 && sumOfCompareFeatures == 0.0) {
            return 1.0;
        }

        // 片方が点で, 他方がベクトルのとき
        if ((sumOfBaseFeatures == 0.0 || sumOfCompareFeatures == 0.0)
            && (sumOfBaseFeatures != sumOfCompareFeatures)) {
            return -1.0;
        }



        return dot / sqrt((sumOfBaseFeatures) * (sumOfCompareFeatures));
        */
    }


public:

    static const int STROKE_COUNT = 17;

    static STROKE Discriminate(Canvas &target) {
        // baseImageにキャンバスの内容を複製
        for (int y = 0; y < IMAGE_SIZE; y++) {
            for (int x = 0; x < IMAGE_SIZE; x++) {
                if (target.ReadPixel(x, y)) {
                    baseImage[x + y * IMAGE_SIZE] = 1.0;
                }
                else {
                    baseImage[x + y * IMAGE_SIZE] = 0.0;
                }
            }
        }


        // HOG特徴を求める
        DSP::HOG::HOG(baseImage, baseFeatures, IMAGE_SIZE, IMAGE_SIZE, CELL_SIZE, BLOCK_SIZE, ORIENTATION);

        
        for (int i = 0; i < HOG_FEATURES_COUNT; i++) {
            Serial.println(int(baseFeatures[i].featureVal * 255));
        }
        Serial.println("-");
        
        // CellDataを求める.

        for (int cell = 0; cell < CELL_COUNT; cell++) {
            // 各セルごとに平均と分散を求める


            float mean = 0.0;
            float sumFeature = 0.0;
            float variance = 0.0;

            // 方向の重み付き平均を求める
            for (int i = 0; i < ORIENTATION; i++) {
                mean += baseFeatures[cell * ORIENTATION + i].featureVal * i;
                sumFeature += baseFeatures[cell * ORIENTATION + i].featureVal;
            }

            //Serial.println("*");
            //Serial.println(sumFeature);

            // 特徴量がある閾値以下の場合はそこにストロークがないとする
            if (sumFeature < THRES_STROKE) {
                baseCellData[cell * 2] = -1;
                baseCellData[cell * 2 + 1] = 0;

                //Serial.println("*");
                continue;
            }

            mean /= sumFeature;


            // 分散を求める
            for (int i = 0; i < ORIENTATION; i++) {

                // 方向の差, 折り返すことに注意
                double oriDelta = abs(i - mean);
                if (oriDelta * 2 >= ORIENTATION) {
                    oriDelta = ORIENTATION - oriDelta;
                }

                variance += oriDelta * oriDelta * baseFeatures[cell * ORIENTATION + i].featureVal;
            }
            variance /= sumFeature;
            variance = sqrt(variance);


            baseCellData[cell * 2] = (char)(mean + 0.5);
            baseCellData[cell * 2 + 1] = (char)(variance + 0.5);


        }


        for (int i = 0; i < CELL_COUNT; i++) {
            Serial.print((int)baseCellData[i * 2]);
            Serial.print(", ");
            Serial.println((int)baseCellData[i * 2 + 1]);
        }
        Serial.println("-");


        int maxSimilarity = -SCORE_INF;
        STROKE similarStroke = STROKE::STROKE_SPACE;

        for (int i = 0; i < STROKE_COUNT; i++) {
            // 各ストロークごとにベースと比較する

            // 類似度を求める
            int similarity = CalculateSimilarity((uint8_t *)&strokeHOGFeatures[i]);

            Serial.println(similarity);

            if (similarity >= maxSimilarity) {
                maxSimilarity = similarity;

                similarStroke = (STROKE)i;

            }
        }

        return similarStroke;

        /*
        int minScore = 0;
        STROKE similarStroke = STROKE::STROKE_SPACE;

        for (int i = 0; i < STROKE_COUNT; i++) {

            // スコアを求める
            int score = CalculateSimilarity((uint8_t *)&strokeHOGFeatures[i]);

            Serial.println(score);
            if (score >= 0 && score <= minScore) {
                minScore = score;

                similarStroke = (STROKE)i;
            }

        }

        return similarStroke;
        */
    }

};


char StrokeDiscriminator::baseCellData[CELL_COUNT * 2];
double StrokeDiscriminator::baseImage[IMAGE_SIZE * IMAGE_SIZE];

DSP::HOG::HOG_Feature StrokeDiscriminator::baseFeatures[HOG_FEATURES_COUNT];

#endif