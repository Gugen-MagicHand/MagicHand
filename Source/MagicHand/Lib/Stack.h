/*
// サイズ固定のスタック
//
// 特徴:
//  std::stackを使用できない状況下のスタックの使用
//  メモリを贅沢に使用しないサイズ固定スタック
//  例外処理に対応していない環境下での使用
//
//  通常, このようなデータ構造はc++の標準ライブラリstd::stackで使用可能だが,
//  一部の開発環境では使用できない場合がある.
//
//  また, このQueueはサイズ固定のスタックで, メモリ資源が少ない場合でも,
//  安全にスタックを扱うことができる. 可変長の場合, (実行前にメモリが足りると判断できれば問題ないが)
//  容易にメモリの確保に失敗する場合がある.
//
//  ほとんどの開発環境で例外処理の機能を使用できるが, 一部の開発環境では利用できない場合がある.
//  このQueueはそのことを想定して例外処理の機能を使用しない.
//
// 更新履歴:
//  2017/10/27:
//   初期版完成
//
//  2017/10/30:
//   Push関数が右辺値参照を引数に取れるようにした.
//
//  2017/10/31:
//   継承されることを考慮してprivate変数をprotectedに変更
//
//  2017/11/1:
//   IsFull, IsEmpty関数追加
//
*/

#ifndef STACK_H
#define STACK_H

template<typename TYPE>
class Stack {

protected:
    int capacity;
    int count;
    TYPE *array = 0x00;

public:
    int Capacity() { return capacity; }
    int Count() { return count; }

    bool IsEmpty() { return count <= 0; }
    bool IsFull() { return count >= capacity; }

    Stack(int capacity) {
        DeleteStack();

        this->capacity = capacity;
        count = 0;
        array = new TYPE[capacity];
    }

    Stack(const Stack &stack) {
        CopyFrom(stack);
    }

    Stack(Stack &&stack) {
        MoveFrom(stack);
    }

    ~Stack() {
        DeleteStack();
    }

    void DeleteStack() {
        if (array != 0x00) {
            delete[] array;
        }
        array = 0x00;
    }

    void CopyFrom(const Stack &from) {
        if (this == &from) {
            return;
        }

        if (capacity < from.capacity) {
            DeleteStack();
            array = new TYPE[capacity];
            capacity = from.capacity;
        }

        count = from.count;

        for (int i = 0; i < count; i++) {
            array[i] = from.array[i];
        }
    }

    void MoveFrom(Stack &from) {
        if (this == &from) {
            return;
        }

        DeleteStack();
        array = from.array;
        from.array = 0x00;

        capacity = from.capacity;
        count = from.count;
    }

    Stack &operator=(const Stack &stack) {
        CopyFrom(stack);
        return *this;
    }

    Stack &operator=(Stack &&stack) {
        MoveFrom(stack);

        return *this;
    }

    bool Pop(TYPE *item) {
        if (count <= 0) {
            return false;
        }

        // 先に代入してから, countをデクリメントすること.
        // これは, 代入に失敗し例外で抜けたときcountを更新しない状況で
        // 抜け出す必要があるからである.
        *item = array[count - 1];

        count--;

        return true;
    }

    bool Push(TYPE &item) {
        if (count >= capacity) {
            return false;
        }

        // 先に代入をしてから, countをインクリメントすること.
        // これは, 代入に失敗し例外で抜けたときcountを更新しない状況で
        // 抜け出す必要があるからである.
        array[count] = item;

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

        *item = array[count - 1];

        return true;

    }

    void Clear() {
        count = 0;
    }

};

#endif