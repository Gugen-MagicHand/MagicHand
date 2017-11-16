/*
// 分数ライブラリ
//
// NaNについて:
//  一般に分母にゼロが来るものはすべてNaNである.
//
// 例外処理について:
//  各演算子ごとにその演算方法に間違いがないかを確認する.
//  間違いがあった場合は, 例外処理を返すようにする.
//  例外処理を利用できない環境で, 例外処理関係のコードを無効化(コメントアウトなど)しても, NaNとして計算結果を返すようにする.
//
//  ただし, 数学として正しくない計算ではこのライブラリの動作を保証できない.
//
// 更新履歴:
//  2017/10/29:
//   初期版完成
// 
//  2017/11/16:
//   分母, 分子の定義を明確化
//   0で割られた分数はNaNとなるようになった.
//
*/

#ifndef FRACTION_H
#define FRACTION_H

//#include <iostream>
//#include <stdexcept>


class Fraction {
private:

    //
    // 分母, 分子の値について:
    //  分母:
    //   分母は通常0より大きい整数とする.
    //   分母がとりうる値は0以上の整数である.
    //   分母が0のときは, NaNとなる.
    // 
    //  分子:
    //   分子は, あらゆる整数をとることができる.
    //   分子の符号が, 分数の符号となる.
    // 


    // 分母. denominator
    // 必ず正の数.
    int denom = 1;

    // 分子. numerator
    int numer = 0;


    template<typename TYPE>
    TYPE Abs(TYPE a) {
        return a > 0 ? a : -a;
    }

public:
    bool IsZero() {
        // 分母が0ではなく, 分子が0のとき, この分数は0である.
        if ((denom != 0) && (numer == 0)) {
            return true;
        }

        return false;
    }

    bool IsNaN() {
        // 分母が0のときは, NaNである
        if (denom == 0) {
            return true;
        }

        return false;
    }

    static Fraction Inverse(const Fraction &frac) {

        Fraction fracInv = frac;

        if (fracInv.IsZero()) {
            // 0の逆数は定義できない.
            //throw std::range_error("[Error] Fraction::Multiply() >> Divided by zero.");
        }

        // NaNのとき
        if (fracInv.IsNaN()) {
            // NaNのまま返す.
            return fracInv;
        }

        // 負のとき
        if (fracInv.numer < 0) {
            //分母を負にする
            fracInv.denom *= -1;
            fracInv.numer *= -1;
        }
        int temp;

        temp = fracInv.numer;
        fracInv.numer = fracInv.denom;
        fracInv.denom = temp;

        return fracInv;
    }

    int Denom() { return denom; }
    int Numer() { return numer; }

    double ToDouble() { return (double)numer / denom; }

    void SetDenom(int denom) {
        if (denom < 0) {
            denom *= -1;
            this->numer *= -1;
        }
        else if (denom == 0) {
            //throw std::invalid_argument("[Error] Fraction::SetDenominator() >> Denom is zero.");
        }


        this->denom = denom;
    }
    void SetNumer(int numer) { this->numer = numer; }

    void SetFraction(int numer, int denom) {
        if (denom == 0) {
            //throw std::invalid_argument("[Error] Fraction::SetFraction() >> Denom is zero.");
        }
        else if (denom < 0) {
            numer *= -1;
            denom *= -1;
        }

        this->denom = denom;
        this->numer = numer;
    }


    Fraction() {

    }

    Fraction(int numer, int denom) {
        SetFraction(numer, denom);
    }

    Fraction(int a) {
        SetFraction(a, 1);
    }

    ~Fraction() {

    }

    Fraction(const Fraction& frac) {
        SetFraction(frac.numer, frac.denom);
    }


    Fraction(Fraction&& frac) {
        SetFraction(frac.numer, frac.denom);
    }

    Fraction& operator=(const Fraction &frac) {
        SetFraction(frac.numer, frac.denom);

        return *this;
    }

    // 整数から分数の生成.
    Fraction& operator=(int a) {
        SetFraction(a, 1);

        return *this;
    }

    Fraction& operator=(Fraction&& frac) {
        SetFraction(frac.numer, frac.denom);

        return *this;
    }


    Fraction & operator()(int numer, int denom) {
        SetFraction(numer, denom);
        return *this;
    }

    /*
    friend std::ostream & operator<<(std::ostream &output, Fraction &frac) {
        if (frac.IsNaN()) {
            output << "NaN";
        }

        else if (frac.IsZero()) {
            output << 0;
        }

        else if (frac.denom == 1) {
            output << frac.numer;
        }

        else {
            output << frac.numer << "/" << frac.denom;
        }

        return output;
    }
    */
    String ToString() {
        String output;
        if (IsNaN()) {
            output = "NaN";
        }

        else if (IsZero()) {
            output = "0";
        }

        else if (denom == 1) {
            output = String(numer);
        }

        else {
            output = String(numer) + "/" + String(denom);
        }

        return output;
    }

    //
    // --- 足し算 ----------------------------------------------------------
    //

    // 分数 + 分数
    friend Fraction operator+(const Fraction &fracA, const Fraction &fracB) {
        Fraction fracC = fracA;
        fracC.Plus(fracB, fracC);

        return fracC;
    }

    // 整数 + 分数
    friend Fraction operator+(int a, const Fraction &fracB) {
        Fraction fracC = fracB;
        fracC.Plus(a, fracC);

        return fracC;
    }

    // 分数 + 整数
    friend Fraction operator+(const Fraction &fracA, int b) {
        Fraction fracC = fracA;
        fracC.Plus(b, fracC);

        return fracC;
    }

    //
    // End 足し算 -----------------------------------------------


    //
    // --- 引き算 -----------------------------------------------------------------
    //

    // 分数 - 分数
    friend Fraction operator-(const Fraction &fracA, const Fraction &fracB) {
        Fraction fracC = fracA;
        fracC.Plus(-1 * fracB, fracC);

        return fracC;
    }

    // 分数 - 整数
    friend Fraction operator-(const Fraction &fracA, int b) {
        Fraction fracC = fracA;
        fracC.Plus(-b, fracC);

        return fracC;
    }

    // 整数 - 分数
    friend Fraction operator-(int a, const Fraction &fracB) {
        Fraction fracC(a, 1);
        fracC.Plus(-1 * fracB, fracC);

        return fracC;
    }

    // End 引き算 -------------------------------------


    //
    // --- 掛け算 ----------------------------------------------------------
    //

    // 分数 x 分数
    friend Fraction operator*(const Fraction &fracA, const Fraction &fracB) {
        Fraction fracC = fracA;
        fracC.Multiply(fracB, fracC);

        return fracC;
    }

    // 整数 x 分数
    friend Fraction operator*(int a, const Fraction &fracB) {
        Fraction fracC = fracB;
        fracC.Multiply(a, fracC);

        return fracC;
    }

    // 分数 x 整数
    friend Fraction operator*(const Fraction &fracA, int b) {
        Fraction fracC = fracA;
        fracC.Multiply(b, fracC);

        return fracC;
    }

    // End 掛け算 ----------------------------------------

    //
    // --- 割り算 --------------------------------------------------
    //

    // 分数 / 分数
    friend Fraction operator/(const Fraction &fracA, const Fraction &fracB) {
        Fraction fracC = fracA;
        fracC.Multiply(Fraction::Inverse(fracB), fracC);

        return fracC;
    }

    // 分数 / 整数
    friend Fraction operator/(const Fraction &fracA, int b) {
        Fraction fracC = fracA;
        Fraction fracB(1, b);
        fracC.Multiply(fracB, fracC);

        return fracC;
    }

    // 整数 / 分数
    friend Fraction operator/(int a, const Fraction &fracB) {
        Fraction fracC(a, 1);
        fracC.Multiply(Fraction::Inverse(fracB), fracC);

        return fracC;
    }

    //
    // End 割り算 --------------------------------------

    Fraction &operator+=(const Fraction &frac) {
        Plus(frac, *this);
        return *this;
    }

    Fraction &operator+=(int a) {
        Plus(a, *this);
        return *this;
    }

    Fraction &operator-=(const Fraction &frac) {
        Plus(-1 * frac, *this);
        return *this;
    }

    Fraction &operator-=(int a) {
        Plus(-a, *this);
        return *this;
    }

    Fraction &operator*=(const Fraction &frac) {
        Multiply(frac, *this);
        return *this;
    }

    Fraction &operator*=(int a) {
        Multiply(a, *this);
        return *this;
    }

    Fraction &operator/=(const Fraction &frac) {
        Multiply(Fraction::Inverse(frac), *this);
        return *this;
    }

    Fraction &operator/=(int a) {
        Multiply(Fraction(1, a), *this);
        return *this;
    }






    //
    // 二つの数の最大公約数(Greatest Common Divisor)を求めます.
    // ユークリッドの互除法を使用します.
    //
    // いづれの数も正の数である必要があります.
    //
    // @param a:
    //  最大公約数を求めるための数の一つ.
    //  0より大きい整数
    //
    // @param b:
    //  最大公約数を求めるための数の一つ.
    //  0より大きい整数
    //
    int CalculateGCD(int a, int b) {

        // aが大きくなるようにする
        if (a < b) {
            int temp = a;
            a = b;
            b = temp;
        }


        // ユークリッドの互除法
        for (;;) {
            // a と b の余りを求める.
            int c = a % b;

            // 割り切れた場合
            if (c == 0) {
                break;
            }

            // 次に割られる数をbにする 
            a = b;

            // 次に割る数をcにする
            b = c;
        }

        return b;
    }

    // 
    // 二つの数の最小公倍数(Least Common Multiple)を求めます
    //
    // いづれの数も正の数である必要があります.
    //
    int CalculateLCM(int a, int b) {
        int gcd = CalculateGCD(a, b);
        return a * b / gcd;
    }


    void Reduce() {
        // 分子または分母がゼロの場合は約分しない
        if (numer == 0 || denom == 0) {
            
            return;
        }

        // 分子が負の数の場合があるので, 絶対値をとる.
        int gcd = CalculateGCD(Abs(numer), denom);

        numer /= gcd;
        denom /= gcd;
    }





    //
    // 分数の足し算
    // 足し算後, 約分されます.
    //
    // 計算式は以下のとおりです.
    //  fracC = fracA(this) + fracB
    //
    void Plus(const Fraction &fracB, Fraction &fracC) {
        // いづれか片方の分母が0のとき, 結果はNaNである
        if (this->denom == 0 || fracB.denom == 0) {


            fracC.denom = 0;
            fracC.numer = 1;


            //throw std::invalid_argument("[Error] Fraction::Plus() >> Denom is zero.");

            return;
        }


        int lcm = CalculateLCM(this->denom, fracB.denom);

        int fracANumer = this->numer * (lcm / this->denom);
        int fracBNumer = fracB.numer * (lcm / fracB.denom);

        fracC.denom = lcm;
        fracC.numer = fracANumer + fracBNumer;

        fracC.Reduce();
    }

    //
    // 分数と整数の足し算
    // 計算後, 約分されます.
    //
    // 計算式は以下のとおりです.
    //  fracC = fracA(this) + b
    //
    void Plus(int b, Fraction &fracC) {
        if (this->denom == 0) {
            //throw std::invalid_argument("[Error] Fraction::Plus() >> Denom is zero.");
        }

        fracC.denom = this->denom;
        fracC.numer = this->numer + b * this->denom;

        fracC.Reduce();
    }

    //
    // 分数の掛け算
    // 計算後, 約分されます.
    // 
    // 計算式は以下のとおりです.
    //  fracC = fracA(this) * fracB
    // 
    void Multiply(const Fraction &fracB, Fraction &fracC) {
        if (this->denom == 0 || fracB.denom == 0) {
            //throw std::range_error("[Error] Fraction::Multiply() >> Divided by zero.");
        }

        fracC.denom = this->denom * fracB.denom;
        fracC.numer = this->numer * fracB.numer;

        fracC.Reduce();
    }

    //
    // 分数と整数の掛け算
    // 計算後, 約分されます.
    // 
    // 計算式は以下のとおりです.
    //  fracC = fracA(this) * b
    // 
    void Multiply(int b, Fraction &fracC) {
        if (this->denom == 0) {
            //throw std::range_error("[Error] Fraction::Multiply() >> Divided by zero.");
        }

        fracC.denom = this->denom;
        fracC.numer = this->numer * b;

        fracC.Reduce();
    }



};


#endif