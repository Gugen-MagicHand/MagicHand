#include "TinyHOG.h"

#include "TestData.h"

#define IMAGE_SIZE 15
#define ORIENTAION 4
#define CELL_SIZE 5
#define FEATURES_COUNT (IMAGE_SIZE / CELL_SIZE) * (IMAGE_SIZE / CELL_SIZE) * ORIENTAION

float image[IMAGE_SIZE * IMAGE_SIZE];
float features[FEATURES_COUNT];


void loop()  __attribute__((__optimize__("O2")));

float CalculateSimilarity(const uint8_t *baseFeatures, float *compareFeatures, int featuresCount) {

    float sumOfBaseFeatures = 0.0;
    float sumOfCompareFeatures = 0.0;
    float dot = 0.0;

    for (int i = 0; i < featuresCount; i++) {
        float baseFeature = ((unsigned char)pgm_read_byte_near(&baseFeatures[i])) / 255.0;
        float compareFeature = compareFeatures[i];

        sumOfBaseFeatures += baseFeature * baseFeature;
        sumOfCompareFeatures += compareFeature * compareFeature;

        dot += baseFeature * compareFeature;

        //Serial.println(baseFeature);
    }

    // 両方点のとき
    if (sumOfBaseFeatures == 0.0 && sumOfCompareFeatures == 0.0) {
        return 1.0;
    }

    // 片方が点で, 他方がベクトルのとき
    if ((sumOfBaseFeatures == 0.0 || sumOfCompareFeatures == 0.0)
        && (sumOfBaseFeatures != sumOfCompareFeatures)) {
        return 0.0;
    }

    return dot / sqrt((sumOfBaseFeatures) * (sumOfCompareFeatures));
}


void setup() {
    Serial.begin(19200);

    // テストデータ読み込み
    for (int y = 0; y < IMAGE_SIZE; y++) {
        for (int x = 0; x < IMAGE_SIZE; x++) {
            if (pgm_read_byte_near(&test[y * IMAGE_SIZE + x])) {
                image[y * IMAGE_SIZE + x] = 1.0;
            }
            else {
                image[y * IMAGE_SIZE + x] = 0.0;
            }
        }
    }

    // テストデータ表示
    for (int y = 0; y < IMAGE_SIZE; y++) {
        for (int x = 0; x < IMAGE_SIZE; x++) {
            if (image[x + y * IMAGE_SIZE] >= 1.0) {
                Serial.print("*");
            }
            else {
                Serial.print(" ");
            }

        }
        Serial.println("");
    }

    //InitMainLoopStackSize(100);
}

void loop() {

    TinyHOG::HOG(image, features, IMAGE_SIZE, IMAGE_SIZE, CELL_SIZE, ORIENTAION);


    Serial.println("Result:");
    // HOG特徴量表示
    for (int i = 0; i < FEATURES_COUNT / ORIENTAION; i++) {
        for (int j = 0; j < ORIENTAION; j++) {
            Serial.print(features[i * ORIENTAION + j], 4);
            Serial.print(", ");
        }
        Serial.println("");
    }

    float similarity = CalculateSimilarity(baseTest, features, FEATURES_COUNT);

    Serial.println(similarity, 4);
    while (true);
}
