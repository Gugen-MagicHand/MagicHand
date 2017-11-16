#ifndef FRACTION_H
#define FRACTION_H

//#include <iostream>
//#include <stdexcept>


class Fraction
{
private:
	// 分母. denominator
	// 必ず正の数.
	int denom;

	// 分子. numerator
	int numer;


	template<typename TYPE>
	TYPE Abs(TYPE a) {
		return a > 0 ? a : -a;
	}

public:

	static Fraction Inverse(const Fraction &frac) {

		Fraction fracInv = frac;

		// 0のとき
		if (fracInv.numer == 0) {
			// 0のまま
			return fracInv;
		}

		// 負のとき
		else if (fracInv.numer < 0) {
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
			return;
		}


		this->denom = denom;
	}
	void SetNumer(int numer) { 
		this->numer = numer; 
		this->denom = 1;
	}

	void SetFraction(int numer, int denom) {
		if (denom == 0) {
			//throw std::invalid_argument("[Error] Fraction::SetDenominator() >> Denom is zero.");
			return;
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

	String ToString() {
		String output;
		if (denom == 1) {
			output = String(numer);

		}
		else {
			output = String(numer) + "/" + String(denom);
		}
		return output;
	}


	/*
		friend std::ostream & operator<<(std::ostream &output, Fraction &frac) {
			if (frac.numer == 0) {
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
		// 分子がゼロの場合は約分しない
		if (numer == 0) {

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

		fracC.denom = this->denom;
		fracC.numer = this->numer * b;

		fracC.Reduce();
	}



};


#endif
