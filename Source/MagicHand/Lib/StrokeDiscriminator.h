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
//#include "FeaturePointFilter.h"
#include "Stroke.h"
#include "TinyHOG.h"


class StrokeDiscriminator {
private:

    static const int BASE_FEATURE_MAX_VAL = 255;

    static const int IMAGE_SIZE = 15;

    static const int LABEL_MAP_SIZE = 17;
    static const int CELL_SIZE = 5;
    static const int ORIENTATION = 4;

    static const int FEATURES_COUNT = (IMAGE_SIZE / CELL_SIZE) * (IMAGE_SIZE / CELL_SIZE) * ORIENTATION;

    static const int EACH_BASE_COUNT = 5;

    static const int LABEL_COUNT_LIST_SIZE = 10;

    static const int ENCLOSED_AREA_SIZE_MIN = 2;

    static float compareImage[IMAGE_SIZE * IMAGE_SIZE];
    static float compareFeatures[FEATURES_COUNT];

    // 0, 1, 2, ------ 13, 14, 15, 16
    // l, i, i, ------ i,  i,  i,  l

    static char labelMap[LABEL_MAP_SIZE][LABEL_MAP_SIZE];


    static char labelCountList[LABEL_COUNT_LIST_SIZE];



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

    static char IsContainedNearMinLabeledIncludeMe(int x, int y) {

        char minLabelValue = 0;


        UpdateMinLabelValue(x, y, &minLabelValue);
        if (x != 0) {

            UpdateMinLabelValue(x - 1, y, &minLabelValue);
        }

        if (y != 0) {
            UpdateMinLabelValue(x, y - 1, &minLabelValue);
        }

        if (x != LABEL_MAP_SIZE - 1) {
            UpdateMinLabelValue(x + 1, y, &minLabelValue);
        }

        if (y != LABEL_MAP_SIZE - 1) {
            UpdateMinLabelValue(x, y + 1, &minLabelValue);
        }

        return minLabelValue;
    }

    static void UpdateMinLabelValue(int x, int y, char *minLabelValue) {


        if (labelMap[y][x] != 0 && *minLabelValue == 0) {
            *minLabelValue = labelMap[y][x];
        }

        else if (labelMap[y][x] != 0 && *minLabelValue != 0) {
            if (*minLabelValue > labelMap[y][x]) {
                *minLabelValue = labelMap[y][x];
            }
        }

    }


    static void Labeling(Canvas &img) {
        for (int y = 0; y < LABEL_MAP_SIZE; y++) {
            for (int x = 0; x < LABEL_MAP_SIZE; x++) {
                labelMap[y][x] = 0;
            }
        }

        char curentLabelValue = 1;
        bool anyUpdate = true;


        while (anyUpdate) {
            anyUpdate = false;

            for (int y = 0; y < LABEL_MAP_SIZE; y++) {
                for (int x = 0; x < LABEL_MAP_SIZE; x++) {
                    bool isWhite = true;

                    if (y >= 1 && y < LABEL_MAP_SIZE && x >= 1 && x < LABEL_MAP_SIZE) {
                        isWhite = !img.ReadPixel(x - 1, y - 1);
                    }

                    if (isWhite) {
                        char labelPrev = labelMap[y][x];
                        char minLabelValue = IsContainedNearMinLabeledIncludeMe(x, y);


                        if (minLabelValue > 0) {
                            labelMap[y][x] = minLabelValue;
                        }

                        else {
                            labelMap[y][x] = curentLabelValue++;
                        }


                        if (labelMap[y][x] != labelPrev) {
                            anyUpdate = true;
                        }
                    }

                }
            }
        }
    }

    static int CountEnclosedAreas() {
        int enclosedAreasCount = 0;

        for (int i = 0; i < LABEL_COUNT_LIST_SIZE; i++) {
            labelCountList[i] = 0;
        }

        for (int y = 0; y < LABEL_MAP_SIZE; y++) {
            for (int x = 0; x < LABEL_MAP_SIZE; x++) {

                int label = labelMap[y][x];
                if (label > 0) {
                    if (label <= LABEL_COUNT_LIST_SIZE) {
                        if (labelCountList[label - 1] != 127) {
                            labelCountList[label - 1]++;
                        }
                    }
                }
            }
        }


        for (int i = 0; i < LABEL_COUNT_LIST_SIZE; i++) {
            if (labelCountList[i] >= ENCLOSED_AREA_SIZE_MIN) {
                enclosedAreasCount++;

                //Serial.println(i);
            }
        }

        return enclosedAreasCount - 1;

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
        int enclosedAreasCount = 0;
        unsigned long time = millis();
        Labeling(target);
        enclosedAreasCount = CountEnclosedAreas();
        //Serial.println(enclosedAreasCount);
        Serial.println(millis() - time);

/*
        for (int y = 0; y < LABEL_MAP_SIZE; y++) {
            for (int x = 0; x < LABEL_MAP_SIZE; x++) {
                Serial.print((int)labelMap[y][x]);
            }
            Serial.println("");
        }*/
        time = millis();
        //Serial.println("-");
        //Serial.println(closedAreaCount);


        // HOG特徴を求める
        TinyHOG::HOG(compareImage, compareFeatures, IMAGE_SIZE, IMAGE_SIZE, CELL_SIZE, ORIENTATION);

        Serial.println(millis() - time);

        time = millis();
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
        //time = millis();

        //Serial.println("Sim:");

        float maxSimilarity = 0.0;
        STROKE similarStroke = STROKE::STROKE_SPACE;

        int strokeStartIndex = 2;
        int strokeEndIndex = STROKE_COUNT - 1;

        switch (enclosedAreasCount) {
        default:
            strokeStartIndex = 2;
            strokeEndIndex = STROKE_COUNT - 1;
            break;
        case 0:

            strokeStartIndex = 2;
            strokeEndIndex = 12;
            break;


        case 1:


            strokeStartIndex = 13;
            strokeEndIndex = 15;
            break;


        case 2:

            strokeStartIndex = 16;
            strokeEndIndex = 16;

            break;
        }


        for (int stroke = strokeStartIndex; stroke <= strokeEndIndex; stroke++) {
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

        Serial.println(millis() - time);

        return similarStroke;
    }

};



float StrokeDiscriminator::compareImage[IMAGE_SIZE * IMAGE_SIZE];

float StrokeDiscriminator::compareFeatures[FEATURES_COUNT];

char StrokeDiscriminator::labelMap[LABEL_MAP_SIZE][LABEL_MAP_SIZE];

char StrokeDiscriminator::labelCountList[LABEL_COUNT_LIST_SIZE];


#endif