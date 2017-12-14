/*
//
// 更新履歴:
//  2017/11/3:
//   初期版完成
*/


#ifndef STROKE_DISCRIMINATOR
#define STROKE_DISCRIMINATOR

#include "Canvas.h"

#include "StrokeBaseFeatures.h"
#include "FeaturePointFilter.h"
#include "Stroke.h"
#include "TinyHOG.h"


class StrokeDiscriminator {
private:

    static const int BASE_FEATURE_MAX_VAL = 255;

    static const int IMAGE_SIZE = 15;
    static const int CELL_SIZE = 5;
    static const int ORIENTATION = 4;

    static const int FEATURES_COUNT = (IMAGE_SIZE / CELL_SIZE) * (IMAGE_SIZE / CELL_SIZE) * ORIENTATION;

    static const int EACH_BASE_COUNT = 5;


    static float compareImage[IMAGE_SIZE * IMAGE_SIZE];
    static float compareFeatures[FEATURES_COUNT];



    static float CalculateSimilarity(const uint8_t *baseFeatures, float *compareFeatures, int featuresCount) //__attribute__((__optimize__("O2")))
    {

        unsigned int sumOfBaseFeatures = 0;
        unsigned int sumOfCompareFeatures = 0;
        unsigned int dot = 0;

        for (int i = 0; i < featuresCount; i++) {
            unsigned int baseFeature = ((unsigned char)pgm_read_byte_near(&baseFeatures[i]));
            unsigned int compareFeature = compareFeatures[i] * BASE_FEATURE_MAX_VAL;

            sumOfBaseFeatures += baseFeature * baseFeature;
            sumOfCompareFeatures += compareFeature * compareFeature;

            dot += baseFeature * compareFeature;

            //Serial.println(sumOfBaseFeatures);
            //Serial.println(sumOfCompareFeatures);
            //Serial.println(dot);

            //Serial.println(baseFeature);
            //Serial.println((int)pgm_read_byte_near(&baseFeatures[i]));
        }

        // 両方点のとき
        if (sumOfBaseFeatures == 0 && sumOfCompareFeatures == 0) {
            return 1.0;
        }

        // 片方が点で, 他方がベクトルのとき
        if ((sumOfBaseFeatures == 0 || sumOfCompareFeatures == 0)
            && (sumOfBaseFeatures != sumOfCompareFeatures)) {
            return 0.0;
        }

        return dot / (float)sqrt((sumOfBaseFeatures) * (unsigned long)(sumOfCompareFeatures));
    }

    static bool IsMatchingWith(char cell[9], const uint8_t *filter) {
        for (int i = 0; i < 9; i++) {
            unsigned char f = ((unsigned char)pgm_read_byte_near(&filter[i]));

            if (f != 2) {
                if (f != cell[i]) {
                    return false;
                }
            }
        }

        return true;

    }


public:

    static const int STROKE_COUNT = 17;

    static STROKE Discriminate(Canvas &target) {
        // Spaceとdotはパターンが決まっている.
        // Spaceはキャンバス内に何も点が打たれていないとき,
        // Dotは左上のみ点が打たれている場合である.
        //
        // まずこの二つを判別する.

        bool isSpace = true;
        bool isDot = true;

        // baseImageにキャンバスの内容を複製
        for (int y = 0; y < IMAGE_SIZE; y++) {
            for (int x = 0; x < IMAGE_SIZE; x++) {

                if (target.ReadPixel(x, y)) {
                    compareImage[x + y * IMAGE_SIZE] = 1.0;

                    // どこかに点が打たれたので, Spaceフラグを下す.
                    isSpace = false;

                    if (x != 0 || y != 0) {
                        // 左上以外に点が打たれたので, Dotフラグを下す.
                        isDot = false;
                    }
                }
                else {
                    compareImage[x + y * IMAGE_SIZE] = 0.0;
                }
            }
        }

        if (isSpace) {
            return STROKE::STROKE_SPACE;
        }
        else if (isDot) {
            return STROKE::STROKE_DOT;
        }

        // 閉路の数を探す
        int closedAreaCount = 0;

        bool isNoEndPoints = true;

        for (int y = 0; y < IMAGE_SIZE; y++) {
            for (int x = 0; x < IMAGE_SIZE; x++) {
                char cell[9];



                Serial.println("-");
                // cell(filterと比較するための部分)の作成
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {

                        int yi = y + i - 1;
                        int xj = x + j - 1;
                        char val = 0;
                        if (yi >= 0 && xj >= 0 && xj < IMAGE_SIZE && yi < IMAGE_SIZE) {
                            val = target.ReadPixel(xj, yi);
                        }
                        cell[i * 3 + j] = val;
                        Serial.print((int)val);
                        Serial.print(", ");
                    }
                    Serial.println("");
                }

                bool isJudged = false;


                // 端点判断
                if (isNoEndPoints) {
                    for (int c = 0; c < END_POINT_FILTERS_COUNT; c++) {
                        if (IsMatchingWith(cell, endPointFilters[c])) {
                            isNoEndPoints = false;
                            isJudged = true;

                            break;
                        }
                    }
                }


                // 分岐点判断
                if (!isJudged) {
                    for (int c = 0; c < BRANCH_POINT_FILTERS_COUNT; c++) {

                        if (IsMatchingWith(cell, branchPointFilters[c])) {
                            closedAreaCount++;
                            isJudged = true;

                            //Serial.println(c);

                            break;
                        }
                    }
                }


                


            }
        }

        if (isNoEndPoints) {
            closedAreaCount++;
        }

        Serial.println("-");
        Serial.println(closedAreaCount);
        //unsigned long time = millis();

        // HOG特徴を求める
        TinyHOG::HOG(compareImage, compareFeatures, IMAGE_SIZE, IMAGE_SIZE, CELL_SIZE, ORIENTATION);

        /*
        for (int i = 0; i < FEATURES_COUNT / ORIENTATION; i++) {
            for (int j = 0; j < ORIENTATION; j++) {
                Serial.print(compareFeatures[i * ORIENTATION + j], 4);
                Serial.print(", ");
            }
            Serial.println("");
        }
        Serial.println("-");
        */
        //Serial.println(millis() - time);
        //time = millis();

        //Serial.println("Sim:");

        float maxSimilarity = 0.0;
        STROKE similarStroke = STROKE::STROKE_SPACE;

        for (int stroke = 2; stroke < STROKE_COUNT; stroke++) {
            // それぞれのBase開始インデックス(ここから複数のベースがある)を求める.
            int baseStartIndex = (stroke - 2) * EACH_BASE_COUNT;

            float similarity = 0.0;

            for (int i = 0; i < EACH_BASE_COUNT; i++) {
                // 一つの文字に対するBaseそれぞれについて

                similarity += CalculateSimilarity(strokeBaseFeatures[baseStartIndex + i], compareFeatures, FEATURES_COUNT);
                //Serial.println((int)pgm_read_byte_near(&strokeBaseFeatures[baseStartIndex + i]));
            }

            similarity /= EACH_BASE_COUNT;

            if (similarity >= maxSimilarity) {
                maxSimilarity = similarity;

                similarStroke = (STROKE)stroke;
            }


            //Serial.print(stroke);
            //Serial.print(": ");
            //Serial.println(similarity, 4);

        }

        //Serial.println(millis() - time);

        return similarStroke;
    }

};



float StrokeDiscriminator::compareImage[IMAGE_SIZE * IMAGE_SIZE];

float StrokeDiscriminator::compareFeatures[FEATURES_COUNT];

#endif