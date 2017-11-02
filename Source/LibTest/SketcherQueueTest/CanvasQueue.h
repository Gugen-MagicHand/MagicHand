/*
// キャンバス待ち行列
//
// キャンバスの待ち行列です.
// 継承元の待ち行列では, 待ち行列内部のアイテムと登録されるアイテムの実体が,
// 異なりますが, この待ち行列は同じであることに注意してください.
// 
// Example usage:
//

// 容量を10に設定, キャンバスは32x32に設定
CanvasQueue queue(10, 32, 32);

// 作業用キャンバス
Canvas *work;

// プッシュ待機キャンバスを取得
if(queue.GetPushedReadyCanvas(&work)){
    // 取得できた
    
    // 1を描く
    work->color = true;
    work->PutChar('1');

    // 待ち行列にPush
    queue.Push();
}

*/



#ifndef CANVAS_QUEUE_H
#define CANVAS_QUEUE_H

#include "Queue.h"
#include "Canvas.h"

class CanvasQueue : public Queue<Canvas>
{
  public:

      CanvasQueue(int capacity, int canvasSizeX, int canvasSizeY) : Queue<Canvas>(capacity) {
          for (int i = 0; i < capacity; i++) {
              array[i].SetSize(canvasSizeX, canvasSizeY);
          }
      }


      //
      // 次にプッシュされるキャンバスの参照を受け取ります.
      // プッシュ待ち状態のキャンバスに対して操作を行ってから,
      // 待ち行列へプッシュするとよいでしょう.
      //
      // @param pushedReadyCanvas:
      //  プッシュ待ちのキャンバスに対するポインタが代入されます.
      //
      bool GetPushedReadyCanvas(Canvas **pushedReadyCanvas) {
          if (count >= capacity) {
              return false;
          }

          *pushedReadyCanvas = &array[(startPoint + count) % capacity];
          //Serial.println((startPoint + count) % capacity);
          return true;
      }

      bool Pop(Canvas **canvasToPop) {

          if (count <= 0)
          {
              return false;
          }

          *canvasToPop = &array[startPoint];

          count--;
          startPoint = (startPoint + 1) % capacity;
          return true;
      }


      bool Push() {

          if (count >= capacity)
          {
              return false;
          }

          count++;

          return true;
      }


      bool Peek(Canvas **canvasToPeek) {
          if (count <= 0) {
              return false;
          }

          *canvasToPeek = &array[startPoint];

          return true;
      }
};

#endif
