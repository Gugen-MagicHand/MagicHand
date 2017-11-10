#include "TinyHOG.h"

#include "TestData.h"

#define IMAGE_SIZE 15
#define ORIENTAION 4
#define CELL_SIZE 5
#define FEATURES_COUNT (IMAGE_SIZE / CELL_SIZE) * (IMAGE_SIZE / CELL_SIZE) * ORIENTAION

float image[IMAGE_SIZE * IMAGE_SIZE];
float features[FEATURES_COUNT];

void setup() {
    Serial.begin(19200);

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

    TinyHOG::HOG(image, features, IMAGE_SIZE, IMAGE_SIZE, CELL_SIZE, ORIENTAION);


    Serial.println("Result:");
    // HOG特徴量表示
    for (int i = 0; i < FEATURES_COUNT / ORIENTAION; i++) {
        for (int j = 0; j < ORIENTAION; j++) {
            Serial.print(features[i * ORIENTAION + j]);
            Serial.print(", ");
        }
        Serial.println("");
    }
}

void loop() {
  // put your main code here, to run repeatedly:

}
