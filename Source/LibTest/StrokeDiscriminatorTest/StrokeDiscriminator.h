
#ifndef STROKE_DISCRIMINATOR
#define STROKE_DISCRIMINATOR

#include "Canvas.h"

#include "Pattern.h"
#include "Stroke.h"


class StrokeDiscriminator {
private:

    static const int strokeMaxCount = 17;
    

    //
    // スコアが低い方がよく似ている
    //
    static int CalculateCorrelationScore(Canvas &canvasA, Canvas &canvasB) {
        if (canvasA.SizeX() != canavsB.SizeX() || canvasA.SizeY() != canvasB.SizeY()) {
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

    static STROKE Discriminate(Canvas &target) {
        for (int i = 0; i < strokeMaxCount; i++) {
            switch (i) {
            case 0:

                break;


            case 1:

                break;


            case 2:

                break;
                {
            case 0:

                break;


            case 1:

                break;


            case 2:

                break;
                {
            case 0:

                break;


            case 1:

                break;


            case 2:

                break;
                {
            case 0:

                break;


            case 1:

                break;


            case 2:

                break;
                {
            case 0:

                break;


            case 1:

                break;


            case 2:

                break;
                {
            case 0:

                break;


            case 1:

                break;


            case 2:

                break;
            }
        }
    }

};



#endif