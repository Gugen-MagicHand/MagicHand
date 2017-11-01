/*
// 二値キャンバス
//
// マイコンで2Dグラフィックを扱えるようにします.
// 各画素は0か1かの情報を持つことができます.
// このライブラリは, 基本となる2Dグラフィック関数を用意します.
// このライブラリを派生して機能を拡張することができます.
//
// 特徴:
//  基本となる2Dグラフィック関数
//  拡張性
//
// 拡張方法:
//  拡張はこのクラスを基底クラスとして継承することで実現できます.
//  
// 仮想関数:
//  仮想関数となっているのは以下のとおりです.
//   Dot
//  
//  Dot関数はscrBufに点を打つのに使用されています.
//  すべての描画関数(scrBufに変更を加える関数)は必ずこのDotを使用してます.
//  Dot関数をオバーライドすることで, そのほかすべての描画関数に影響を与えることが可能です.
// 
// 更新履歴:
//  2017/3/29:
//   初期版完成
//
//  2017/10/31:
//   コメントの追加
//   updateフラグ機能分離
//   ScaleTo関数追加
//   Celput関数でキャンバスを引数に取れるようにした
//   ReadPixcel関数追加
//
*/

#ifndef CANVAS_H
#define CANVAS_H

#include "Font.h"
#include "Images.h"

class Canvas
{
  protected:
    static int Abs(int a)
    {
        return a > 0 ? a : -a;
    }
    static void Unallocate(unsigned char *array)
    {
        if (array != 0x00)
        {
            delete[] array;
            array = 0x00;
        }
    }

    int sizeX, sizeY;
    int scrBufCount = 0;

  public:
    // 画面バッファ(スクリーン)
    // このバッファ上に描画関数が描画します.
    // 左上原点
    unsigned char *scrBuf = 0x00;

    //
    // @val:
    //  trueのとき描画関数はスクリーンに点を打ちます.
    //  falseのときは点を消します.
    //
    bool color = true;

    int posX = 0, posY = 0;

    int SizeX() { return sizeX; }
    int SizeY() { return sizeY; }

    //
    // @param sizeX:
    //  キャンバス横サイズ
    //
    // @param sizeY:
    //  キャンバス横サイズ
    //
    Canvas(int sizeX, int sizeY)
    {
        SetSize(sizeX, sizeY);
    }

    Canvas()
    {
        SetSize(0, 0);
    }

    ~Canvas()
    {
        Unallocate(scrBuf);
    }

    //
    // @param sizeX:
    //  設定したいキャンバス横サイズ
    //
    // @param sizeY:
    //  設定したいキャンバス縦サイズ
    //
    void SetSize(int sizeX, int sizeY)
    {
        Unallocate(scrBuf);

        this->sizeX = sizeX;
        this->sizeY = sizeY;

        scrBufCount = sizeX * ((sizeY + 7) / 8);

        if (scrBufCount > 0)
        {
            scrBuf = new unsigned char[scrBufCount];
        }

        // 画面バッファを0で初期化
        for (int i = 0; i < scrBufCount; i++)
        {
            scrBuf[i] = 0x00;
        }
    }

    void Pos(int x, int y)
    {
        posX = x;
        posY = y;
    }

    //
    // スクリーンに点を打つ, もしくは消します.
    // メンバ変数colorがtrueのときは点を打ち, falseのときは点を消します.
    //
    // NOTE:
    //  この関数は, 仮想関数です.
    //  クラスを継承して機能の拡張を行う際, この関数をオーバーライドすることで,
    //  各種機能を追加することが可能です.
    //
    // @param x:
    //  点の位置x
    //
    // @param y:
    //  点の位置y
    //
    virtual void Dot(int x, int y)
    {
        if (x < 0 || x >= sizeX || y < 0 || y >= sizeY)
        {
            return;
        }

        // 点が属するバッファ要素番号を計算
        int bufIndex = (y >> 3) * sizeX + x;

        //点を打つ
        if (color)
        {
            //memo:
            //  y & 0x07:
            //    yを8で割ったときのあまり
            scrBuf[bufIndex] |= 0x01 << (y & 0x07);
        }
        else
        {
            scrBuf[bufIndex] &= ~(0x01 << (y & 0x07));
        }
    }

    //
    // 指定された位置のピクセル値(点があるか無いか)を読みます.
    // 
    // @param x:
    //  読みたいピクセル位置x
    //
    // @param y:
    //  読みたいピクセル位置y
    //
    // @return: trueのときは点が打たれています. falseのときは点が打たれていません.
    //
    bool ReadPixcel(int x, int y)
    {
        if (x < 0 || x >= sizeX || y < 0 || y >= sizeY)
        {
            return false;
        }

        int bufIndex = (y >> 3) * sizeX + x;

        return scrBuf[bufIndex] & (0x01 << (y & 0x07));
    }

    // ###########################################################################
    // Dot関数より高次の描画関数
    // ###########################################################################

    //
    // スクリーン上に線を引きます.
    // 始点から終点に向かって線が引かれます.
    //
    // @param x0:
    //  始点x
    //
    // @param y0:
    //  始点y
    //
    // @param x1:
    //  終点x
    //
    // @param y1:
    //  終点y
    //
    void Line(int x0, int y0, int x1, int y1)
    {
        int steep, t;
        int deltaX, deltaY;
        int x, y;
        int yStep, error;

        // 差分の大きいほうを求める
        steep = (Abs(y1 - y0) > Abs(x1 - x0));
        //x,yの入れ替え
        if (steep)
        {
            t = x0;
            x0 = y0;
            y0 = t;
            t = x1;
            x1 = y1;
            y1 = t;
        }
        if (x0 > x1)
        {
            t = x0;
            x0 = x1;
            x1 = t;
            t = y0;
            y0 = y1;
            y1 = t;
        }
        deltaX = x1 - x0; // 傾き計算
        deltaY = Abs(y1 - y0);
        error = 0;
        y = y0;
        // 傾きでステップの正負を切り替え
        if (y0 < y1)
            yStep = 1;
        else
            yStep = -1;
        // 直線を点で描画
        for (x = x0; x <= x1; x++)
        {
            if (steep)
            {
                Dot(y, x);
            }
            else
            {
                Dot(x, y);
            }

            error += deltaY;

            if ((error << 1) >= deltaX)
            {
                y += yStep;
                error -= deltaX;
            }
        }
    }

    //
    // スクリーン上に垂直線を引きます.
    // 始点からy軸正の方向に指定された長さ分の垂直線を引きます.
    // lengthが負数のときはy軸負の方向に線が引かれることに注意してください.
    //
    // @param x:
    //  始点x
    //
    // @param y:
    //  始点y
    //
    // @param length:
    //  線の長さ
    //
    void VerticalLine(int x, int y, int length)
    {
        int dir = (length > 0) ? 1 : -1;
        length = Abs(length);
        for (int i = 0; i < length; i++)
        {
            Dot(x, y);
            y += dir;
        }
    }

    //
    // 塗りつぶし矩形をスクリーン上に描画します.
    // 左上座標から右下座標に向けて矩形が描画されます.
    // この矩形内は塗りつぶしされます.
    //
    // @param x0:
    //  矩形左上点x
    //
    // @param y0:
    //  矩形左上点y
    //
    // @param x1:
    //  矩形右下点x
    //
    // @param y1:
    //  矩形右下点y
    //
    void Boxf(int x0, int y0, int x1, int y1)
    {
        int t;
        if (x1 < x0) //xの入れかえ
        {
            t = x0;
            x0 = x1;
            x1 = x0;
        }
        for (; x0 <= x1; x0++)
        {
            //直線を描く
            VerticalLine(x0, y0, y1 - y0 + 1);
        }
    }

    //
    // ワイヤー矩形をスクリーン上に描画します.
    // 左上座標から右下座標に向けて矩形が描画されます.
    // この矩形内は塗りつぶしされません
    //
    // @param x0:
    //  矩形左上点x
    //
    // @param y0:
    //  矩形左上点y
    //
    // @param x1:
    //  矩形右下点x
    //
    // @param y1:
    //  矩形右下点y
    //
    void Boxw(int x0, int y0, int x1, int y1)
    {
        Line(x0, y0, x1, y0);
        Line(x1, y0, x1, y1);
        Line(x1, y1, x0, y1);
        Line(x0, y1, x0, y0);
    }

    //
    // 画像を描画します
    // 画像データは同階層にあるImage.h内で定義してください.
    //
    // @param imageName:
    //  画像データ変数名
    //
    template <typename T, int COL>
    void Celput(const T (&imageName)[COL])
    {

        //画像の縦の長さを調べる
        byte row = sizeof(T);

        for (int i = 0; i < COL; i++)
        {
            //縦の長さに応じて処理を分ける
            if (row == 1)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (0x01 & (pgm_read_byte_near(&imageName[i]) >> j))
                    {
                        Dot(posX, posY + j);
                    }
                }
            }
            else if (row == 2)
            {
                for (int j = 0; j < 16; j++)
                {

                    if (0x01 & (pgm_read_word_near(&imageName[i]) >> j))
                    {
                        Dot(posX, posY + j);
                    }
                }
            }
            else if (row == 4)
            {
                for (int j = 0; j < 32; j++)
                {
                    if (0x01 & (pgm_read_dword_near(&imageName[i]) >> j))
                    {
                        Dot(posX, posY + j);
                    }
                }
            }

            //次の列へ移動
            posX++;
        }
    }

    //
    // 引数で渡されたキャンバスを自身に転写します.
    //
    // @param image:
    //  転写元のキャンバス
    //
    void Celput(Canvas &image)
    {
        for (int x = 0; x < image.sizeX; x++)
        {

            for (int y = 0; y < image.sizeY; y++)
            {
                if (image.ReadPixcel(x, y))
                {
                    Dot(posX, posY + y);
                }
            }

            // 次の列へ移動
            posX++;
        }
    }

    //
    // 文字を描画します
    // ASC2コードのみの対応となっております.
    //
    // @param ch:
    //  文字コード
    //
    void PutChar(char ch)
    {
        if (ch < 0x20 || ch > 0x7f)
        {
            Celput(font[0x2e - 0x20]);
        }
        else
        {
            Celput(font[ch - 0x20]);
        }

        //文字間隔を1ドット分あける
        posX++;
    }

    // 文字列を描画します
    // 
    // 警告:
    //  文字列の最後にヌル文字を置いてください
    //
    // @param str:
    //  文字列先頭文字アドレス
    //
    void Mes(char *str)
    {
        while (*str)
        {
            PutChar(*str++);
        }
    }

    // End Dot関数より高次の描画関数 ###################################

    //
    // 自身の内容をスケール変換して指定されたキャンバスへ描きこみます.
    // 自身のキャンバスサイズから相手のキャンバスサイズへスケール変換されます.
    //
    // このスケール変換は線型補間が用いられています.
    //
    // 注意:
    //  相手キャンバスのメンバ変数colorがこの関数実行前と実行後で変化している可能性があります.
    //
    // @param dst:
    //  スケール変換先のキャンバス
    //
    void ScaleTo(Canvas &dst)
    {
        double zx = dst.sizeX / (double)sizeX;
        double zy = dst.sizeY / (double)sizeY;

        for (int y = 0; y < dst.sizeY; y++)
        {
            for (int x = 0; x < dst.sizeX; x++)
            {
                // 拡大した画像の座標(x, y)に対応する元の画像の座標値(fromX, fromY)
                double fromX = (double)x / zx;
                double fromY = (double)y / zy;

                // 左下の点を求める
                int m = (int)fromX;
                int n = (int)fromY;

                // 左下の点からの差を求める
                double p = fromX - m;
                double q = fromY - n;
                if (q == 1.0)
                {
                    q = 0.0;
                    n = n + 1;
                }
                if (p == 1.0)
                {
                    p = 0.0;
                    m = m + 1;
                }

                // 右上の座標
                int m1 = m + 1;
                int n1 = n + 1;
                if (m1 >= sizeX)
                {
                    m1 = sizeX - 1;
                }
                if (n1 >= sizeY)
                {
                    n1 = sizeY - 1;
                }

                // 線型補間
                double d = (1.0 - q) * ((1.0 - p) * ReadPixcel(m, n) + p * ReadPixcel(m1, n)) + q * ((1.0 - p) * ReadPixcel(m, n1) + p * ReadPixcel(m1, n1));

                // 四捨五入
                if (d >= 0.5)
                {
                    dst.color = true;
                }
                else
                {
                    dst.color = false;
                }
                dst.Dot(x, y);
            }
        }
    }
};

#endif
