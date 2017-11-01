/*
// サイズ固定の待ち行列
//
// 特徴:
//  std::queueを使用できない状況下の待ち行列の使用
//  メモリを贅沢に使用しないサイズ固定待ち行列
//  例外処理に対応していない環境下での使用
//
//  通常, このようなデータ構造はc++の標準ライブラリstd::queueで使用可能だが,
//  一部の開発環境では使用できない場合がある.
//
//  また, このQueueはサイズ固定の待ち行列で, メモリ資源が少ない場合でも,
//  安全に待ち行列を扱うことができる. 可変長の場合, (実行前にメモリが足りると判断できれば問題ないが)
//  容易にメモリの確保に失敗する場合がある.
//
//  ほとんどの開発環境で例外処理の機能を使用できるが, 一部の開発環境では利用できない場合がある.
//  このQueueはそのことを想定して例外処理の機能を使用しない.
// 
// 更新履歴:
//  2017/5/1:
//   初期版完成
//
//  2017/10/30:
//   Push関数が右辺値参照を引数に取れるようにした.
//
//  2017/11/1:
//   代入演算子部のバグ修正(宣言されていない変数名)
//   継承されることを考慮してprivate変数をprotectedに変更
//
*/

#ifndef QUEUE_H
#define QUEUE_H


template <typename TYPE> class Queue
{
protected:
    int count = 0;
    int capacity = 0;
    TYPE *array = 0x00;

    int startPoint = -1;

    void DeleteQueue() {
        if (array != 0x00) {
            delete[] array;
            array = 0x00;
        }
    }

public:
    int Count() { return this->count; }

    //コンストラクタ, デストラクタ, 
    //コピーコンストラクタ, ムーブコンストラクタ,
    //コピー代入, ムーブ代入


    //
    // コンストラクタ
    //
    // @param capacity:
    //  Queueのサイズ.
    // 
    Queue(int capacity) {
        this->capacity = capacity;
        array = new TYPE[capacity];
    }

    ~Queue() {
        DeleteQueue();
    }

    Queue(const Queue &queue) {
        CopyFrom(queue);

    }

    Queue(Queue&& queue) {
        MoveFrom(queue);


    }

    Queue& operator=(const Queue &rQueue) {
        CopyFrom(rQueue);
        return (*this);
    }

    Queue& operator=(Queue &&rQueue) {
        MoveFrom(rQueue);
        return *this;
    }

    bool Pop(TYPE *item) {

        if (count <= 0)
        {
            return false;
        }

        *item = array[startPoint];

        count--;
        startPoint = (startPoint + 1) % capacity;
        return true;
    }

    bool Push(const TYPE &item) {

        if (count >= capacity)
        {
            return false;
        }

        // 代入を行ってから, countを更新すること.
        // これは, 代入の際に例外が起こることを想定している.
        // もし, 代入処理で例外が発生し処理が中断されたとき, 
        // countを更新しない状況で中断されるべきである.
        array[(startPoint + count) % capacity] = item;

        count++;

        return true;
    }

    bool Push(TYPE &&item) {
        return Push(item);
    }

    bool Peek(TYPE *item) {
        if (count <= 0) {
            return false;
        }

        *item = array[startPoint];

        return true;
    }
    void CopyFrom(const Queue &from) {
        if (this == &from) {
            return;
        }

        DeleteQueue();

        capacity = from.capacity;
        array = new TYPE[capacity];
        count = from.count;
        startPoint = from.startPoint;

        for (int i = 0; i < capacity; i++)
        {
            array[i] = from.array[i];
        }
    }

    void MoveFrom(Queue &queue) {
        if (this == &queue) {
            return;
        }

        DeleteQueue();

        array = queue.array;
        queue.array = 0x00;
        capacity = queue.capacity;
        count = queue.count;
        startPoint = queue.startPoint;
    }
};

#endif // !_INCLUDE_QUEUE_H_
