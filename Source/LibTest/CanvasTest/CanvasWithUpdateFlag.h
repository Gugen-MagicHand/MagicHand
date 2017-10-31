/*
// 更新フラグを持つCanvas(Canvas 拡張)
//
// Canvasの機能に加え, これは更新フラグを持ちます.
// この更新フラグは, 点が打たれたときに対応するバッファの位置に基づいた
// フラグを立てます. こうすることで, デバイスの画面更新時間を短縮できます.
//
// 特徴:
//  Canvasの機能を受け継ぎ
//  画面更新フラグの実装
// 
// フラグの使用:
/   更新フラグは, 点が打たれたときに対応するバッファの位置に基づいた
//  フラグを立てます.
// 
//  フラグを下ろすのは, キャンバス内容をどこかデバイスに反映させた時が適切でしょう.
//  フラグが立っているバッファの内容のみデバイスに反映させることで,
//  画面更新時間を短縮できます.
//
// 基底クラスからオーバーロードした関数:
//  SetSize()
//
// 基底クラスからオーバーライドした関数:
//  Dot()
//  
// 更新履歴:
//  2017/10/31:
//   初期版完成
//
*/

#ifndef CANVAS_WITH_UPDATE_FLAG
#define CANVAS_WITH_UPDATE_FLAG

#include "Canvas.h"

class CanvasWithUpdateFlag : public Canvas
{
    typedef Canvas base;

  protected:
    int updatedCount = 0;

    //
    // 画面更新フラグの設定
    // scrBufCount値に基づいて画面更新フラグ配列(updated)を設定します.
    // 必ず, scrBufCountを更新してからこの関数を実行してください.
    //
    void SetUpdated()
    {
        Unallocate(updated);

        updatedCount = (scrBufCount + 7) / 8;

        if (updatedCount > 0)
        {
            updated = new unsigned char[updatedCount];
        }

        for (int i = 0; i < updatedCount; i++)
        {
            updated[i] = 0;
        }
    }

  public:
    unsigned char *updated = 0x00;

    CanvasWithUpdateFlag(int sizeX, int sizeY) : Canvas(sizeX, sizeY)
    {
        // Canvas コンストラクタでバッファが設定された.
        // 次に画面更新フラグを設定する.
        SetUpdated();
    }

    CanvasWithUpdateFlag() : Canvas()
    {
        // Canvas コンストラクタでバッファが設定された.
        // 次に画面更新フラグを設定する.
        SetUpdated();
    }

    ~CanvasWithUpdateFlag()
    {
        Unallocate(updated);
    }

    //
    // キャンバスサイズを設定します.
    //
    // この関数は基底関数SetSizeをオーバーロードしています.
    //
    //
    void SetSize(int sizeX, int sizeY)
    {
        // 基底関数(SetSize)でまずスクリーンバッファを設定する.
        base::SetSize(sizeX, sizeY);

        // バッファの設定後, 画面更新フラグの設定を行う.
        SetUpdated();
    }

    //
    // スクリーンに点を打つ, もしくは消します.
    // メンバ変数colorがtrueのときは点を打ち, falseのときは点を消します.
    //
    // この関数は基底関数Dotをオーバーライドしています.
    //
    // @param x:
    //  点の位置x
    //
    // @param y:
    //  点の位置y
    //
    void Dot(int x, int y)
    {
        if (x < 0 || x >= sizeX || y < 0 || y >= sizeY)
        {
            return;
        }

        int bufIndex = (y >> 3) * sizeX + x;

        //前回の情報保存
        unsigned char temp = scrBuf[bufIndex];

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

        //前回のデータと異なるときフラグを立てる
        if (scrBuf[bufIndex] != temp)
        {
            //memo:
            //  bufIndex >> 3:
            //    bufIndexを8で割ったときの商
            updated[bufIndex >> 3] |= (0x01 << (bufIndex & 0x07));
        }
    }
};
#endif