
#ifndef TINY_HOG_H
#define TINY_HOG_H

#define _USE_MATH_DEFINES
#include <math.h>


namespace TinyHOG {


    //
    // セルごとのヒストグラムを作成します
    // 内部関数です.
    //
    void ComputeCellHistogram(float *image, int width, int height, float *cellHist, int cellSize, int orientation) {

        // 各方向の輝度の差分
        float deltaX = 0.0;
        float deltaY = 0.0;

        // 勾配強度
        float magnitude;

        // 勾配方向
        float gradient;

        // 量子化するときの角度
        float angle = 180.0 / (float)orientation;

        int fromIndex, toIndex;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {

                // 横方向の差分
                if (x == 0) {
                    toIndex = y * width + x + 1;
                    fromIndex = y * width + x;
                    deltaX = image[toIndex] - image[fromIndex];
                }
                else if (x == width - 1) {

                    toIndex = y * width + x;
                    fromIndex = y * width + x - 1;
                    deltaX = image[toIndex] - image[fromIndex];
                }
                else {

                    toIndex = y * width + x + 1;
                    fromIndex = y * width + x - 1;
                    deltaX = image[toIndex] - image[fromIndex];
                }


                // 縦方向の差分
                if (y == 0) {
                    toIndex = (y + 1) * width + x;
                    fromIndex = y * width + x;
                    deltaY = image[toIndex] - image[fromIndex];
                }
                else if (y == height - 1) {

                    toIndex = y * width + x;
                    fromIndex = (y - 1) * width + x;
                    deltaY = image[toIndex] - image[fromIndex];
                }
                else {

                    toIndex = (y + 1) * width + x;
                    fromIndex = (y - 1) * width + x;
                    deltaY = image[toIndex] - image[fromIndex];
                }


                // 勾配強度の算出
                magnitude = sqrt((float)deltaX * (float)deltaX + (float)deltaY * (float)deltaY);

                //cout << magnitude << endl;
                // 勾配方向の算出
                gradient = atan2((float)deltaY, (float)deltaX);

                // ラジアンから角度に変換
                gradient = (gradient * 180.0) / M_PI;

                // 符号が負である場合は反転
                if (gradient < 0.0) {
                    gradient += 360.0;
                }

                // 0~360度から, 0~180度に変換
                // Memo:
                //  前は180を含めなかった. だが, 180のときgradientがorientationになる可能性があり,
                //  確保したヒストグラム領域を超える可能性があるので,
                //  180を含めることにした.
                //
                if (gradient >= 180.0) {
                    gradient -= 180.0;
                }

                //cout << gradient << endl;
                gradient = gradient / angle;


                // ヒストグラムに蓄積
                int histIndex = (int)((y / cellSize) * (width / cellSize) * orientation + (x / cellSize) * orientation + (int)gradient);

                cellHist[histIndex] += magnitude;

            }
        }

        //cout << "*";
    }

    //
    // HOG特徴量を計算します
    // 内部関数です.
    //
    void ComputeHOG(float *hogFeatures, int width, int height, int cellSize, int orientation) {

        int fCount = (width / cellSize) * (height / cellSize) * orientation;


        // 特徴量の算出と正規化
            // 特徴量を正規化するときに使用する特徴量の二乗和
        float sumMagnitude = 0.0;

        for (int i = 0; i < fCount; i++) {

            // 正規化のためのヒストグラムの総和の二乗を算出
            sumMagnitude += hogFeatures[i] * hogFeatures[i];
        }

        for (int i = 0; i < fCount; i++) {

            hogFeatures[i] = hogFeatures[i] / sqrt(sumMagnitude + 1.0);
            /*
            if (sumMagnitude == 0.0) {
                hogFeatures[f].featureVal = 0.0;
            }
            else if (sumMagnitude != 0.0) {
                hogFeatures[f].featureVal = cellHist[histIndex] / sumMagnitude;
            }
            */
        }

    }

    //
    // HOG (Histogram of Oriented Gradients)
    //
    // HOG特徴量を計算します. HOG特徴は, 物体の回転大きさには不偏ではありませんが, 明度において不偏です.
    // 
    // TinyHOGは通常のHOGと異なり, ブロックサイズは指定できません.
    // 画像内にある横縦セル数がブロックサイズとなります.
    // 特徴量とヒストグラムのサイズは一致します
    //
    // 求め方:
    //  HOG特徴の求め方は以下のとおりです.
    // 
    //   1. 物体を含む画像の各画素において一回微分に基づく演算によりエッジ点の方向と強度を求めます.
    //   2. 指定された量子化精度で方向を量子化します.
    //   3. 指定されたピクセル幅で画像をセルに分割します. この時, セル幅は画像サイズをちょうど割り切るように設定しなければなりません.
    //   4. 各セルにおいてこれらの特徴をヒストグラムとして記述します.
    //   5. 指定されたセル数の幅を持つブロックを単位に一セルごとずらして上の特徴を正規化します.
    //
    // HOG特徴次元数:
    //  TinyHOG特徴の次元数は以下のとおりです.
    //   dim = (height / cellSize) * (width / cellSize) * orientation
    //
    //  次元数は, CountHOGFeature()で求めることが可能です.
    //
    // @param image:
    //  対象画像. 行メジャーの配列にしてください.
    //  カラーではなく, グレーの画像にしてください.
    //
    // @param hogFeatures
    //  特徴量配列
    //  特徴量を保存するために使用します.
    //  あらかじめHOG特徴の次元数分を用意してください. CountHOGFeature()で次元数を求められます.
    //
    // @param width:
    //  画像横サイズ
    //  
    // @param height:
    //  画像縦サイズ
    //
    // @param cellSize:
    //  セルのサイズ(pixel)
    //  必ずこのサイズは画像サイズをちょうど割るように設定してください.
    //  通常この値は5に設定されます.
    //
    // @param:
    //  角度量子化精度
    //  通常この値は9に設定されます.
    //
    void HOG(float *image, float *hogFeatures, int width, int height, int cellSize, int orientation) {


        // cellごとのヒストグラムのためのメモリを確保
        // 勾配方向ヒストグラムを保存する領域を確保
        int featuresCount = (width / cellSize) * (height / cellSize) * orientation;

        // ヒストグラム初期化, 特徴量初期化
        for (int i = 0; i < featuresCount; i++) {
            hogFeatures[i] = 0.0;
        }

        // 勾配方向と勾配強度から勾配方向ヒストグラムを算出
        ComputeCellHistogram(image, width, height, hogFeatures, cellSize, orientation);

        /*
        for (int i = 0; i < cellHistSize; i++) {
            cout << cellHist[i] << endl;
        }
        */

        // HOG特徴量の算出
        ComputeHOG(hogFeatures, width, height, cellSize, orientation);

        //cout << "*" << endl;
    }

    //
    // HOG特徴の次元数を求めます.
    // 先に次元数を求めておき, その次元数分のfloat配列を用意するためです.
    //
    int CountHOGFeature(int width, int height, int cellSize, int orientation) {

        return (height / cellSize) * (width / cellSize) * orientation;
    }
}


#endif