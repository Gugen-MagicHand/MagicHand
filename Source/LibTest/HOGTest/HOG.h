
#ifndef HOG_H
#define HOG_H

#define _USE_MATH_DEFINES
#include <math.h>

namespace DSP {


    namespace HOG {

        struct HOG_Feature {
            // ブロック領域の左上x座標(0~width)
            int blockPosX;

            // ブロック領域左上のy座標(0~height)
            int blockPosY;

            // セル領域のx座標(0~blockSize)
            int cellPosX;

            // セル領域のy座標(0~blockSize)
            int cellPosY;

            // 勾配の方向
            int orientation;

            // 特徴量
            double featureVal;
        };


        //
        // セルごとのヒストグラムを作成します
        // 内部関数です.
        //
        void ComputeCellHistogram(double *image, int width, int height, double *cellHist, int cellSize, int orientation) {

            // 各方向の輝度の差分
            double deltaX = 0.0;
            double deltaY = 0.0;

            // 勾配強度
            double magnitude;

            // 勾配方向
            double gradient;

            // 量子化するときの角度
            double angle = 180.0 / (double)orientation;

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
                    magnitude = sqrt((double)deltaX * (double)deltaX + (double)deltaY * (double)deltaY);

                    //cout << magnitude << endl;
                    // 勾配方向の算出
                    gradient = atan2((double)deltaY, (double)deltaX);

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
        void ComputeHOG(HOG_Feature *hogFeatures, double *cellHist, int width, int height, int cellSize, int blockSize, int orientation) {


            int fCount = 0;

            // ブロックの移動
            for (int y = 0; y < ((height / cellSize) - blockSize + 1); y++) {
                for (int x = 0; x < ((width / cellSize) - blockSize + 1); x++) {
                    // セル内の移動
                    for (int j = 0; j < blockSize; j++) {
                        for (int i = 0; i < blockSize; i++) {

                            // 勾配方向
                            for (int k = 0; k < orientation; k++) {

                                // 特徴量のパラメータを代入
                                hogFeatures[fCount].blockPosX = x;
                                hogFeatures[fCount].blockPosY = y;

                                hogFeatures[fCount].cellPosX = i;
                                hogFeatures[fCount].cellPosY = j;

                                hogFeatures[fCount].orientation = k;

                                // 特徴量のカウント
                                fCount++;
                            }
                        }
                    }
                }
            }



            // 特徴量の算出と正規化
            for (int f = 0; f < fCount; f++) {

                // 特徴量を正規化するときに使用する特徴量の二乗和
                double sumMagnitude = 0.0;

                // セル内の移動
                for (int j = 0; j < blockSize; j++) {
                    for (int i = 0; i < blockSize; i++) {

                        // 勾配方向
                        for (int k = 0; k < orientation; k++) {
                            // 正規化のためのヒストグラムの総和の二乗を算出
                            int histIndex = (int)((hogFeatures[f].blockPosY + j) * (width / cellSize) * orientation + (hogFeatures[f].blockPosX + i) * orientation + k);

                            sumMagnitude += cellHist[histIndex] * cellHist[histIndex];
                        }
                    }
                }

                // 特徴量の正規化
                int histIndex = (int)((hogFeatures[f].blockPosY + hogFeatures[f].cellPosY) * (width / cellSize) * orientation + (hogFeatures[f].blockPosX + hogFeatures[f].cellPosX) * orientation + hogFeatures[f].orientation);
                
                hogFeatures[f].featureVal = cellHist[histIndex] / sqrt(sumMagnitude + 1.0);
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
        //  HOG特徴の次元数は以下のとおりです.
        //   dim = ((height / cellSize) - blockSize + 1) * ((width / cellSize) - blockSize + 1) * blockSize^2 * orientation
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
        // @param blockSize:
        //  ブロックサイズ(cell)
        //  必ず画像縦横に占めるセル数以下に設定してください.
        //  通常この値は3に設定されます.
        //
        // @param:
        //  角度量子化精度
        //  通常この値は9に設定されます.
        //
        void HOG(double *image, HOG_Feature *hogFeatures, int width, int height, int cellSize, int blockSize, int orientation) {

            // cellごとのヒストグラムのためのメモリを確保
            // 勾配方向ヒストグラムを保存する領域を確保
            int cellHistSize = (width / cellSize) * (height / cellSize) * orientation;
            double *cellHist = new double[cellHistSize];

            // ヒストグラム初期化
            for (int i = 0; i < cellHistSize; i++) {
                cellHist[i] = 0.0;
            }

            // 勾配方向と勾配強度から勾配方向ヒストグラムを算出
            ComputeCellHistogram(image, width, height, cellHist, cellSize, orientation);

            /*
            for (int i = 0; i < cellHistSize; i++) {
                cout << cellHist[i] << endl;
            }
            */

            // HOG特徴量の算出
            ComputeHOG(hogFeatures, cellHist, width, height, cellSize, blockSize, orientation);


            // ヒストグラム解放
            delete[] cellHist;

            //cout << "*" << endl;
        }

        //
        // HOG特徴の次元数を求めます.
        // 先に次元数を求めておき, その次元数分のHOG_Featureを用意するためです.
        //
        int CountHOGFeature(int width, int height, int cellSize, int blockSize, int orientation) {
            int fCount = 0;

            // ブロックの移動
            for (int y = 0; y < ((height / cellSize) - blockSize + 1); y++) {
                for (int x = 0; x < ((width / cellSize) - blockSize + 1); x++) {

                    // セル内の移動
                    for (int j = 0; j < blockSize; j++) {
                        for (int i = 0; i < blockSize; i++) {

                            // 勾配方向数
                            for (int k = 0; k < orientation; k++) {
                                // 特徴量のカウント
                                fCount++;
                            }
                        }
                    }
                }
            }


            return fCount;
        }
    }
}


#endif