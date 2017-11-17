#ifndef RESULT_ASSEMBLER_H
#define RESULT_ASSEMBLER_H

#include "Stroke.h"
#include "Fraction.h"
#include "Calculator.h"

class StrokeAssembler {

public:
	enum STATUS {
		SUCCESS,
		ASSEMBLING,
		OVERFLOW,
		FAILED
	};

	enum RESULT {
		RESULT_LEFT_BRACKET,
		RESULT_RIGHT_BRACKET,
		RESULT_0,
		RESULT_1,
		RESULT_2,
		RESULT_3,
		RESULT_4,
		RESULT_5,
		RESULT_6,
		RESULT_7,
		RESULT_8,
		RESULT_9,
		RESULT_PLUS,
		RESULT_MINUS,
		RESULT_MULTIPLY,
		RESULT_DEVIDE,
		RESULT_EQUAL,
		RESULT_UNKNOWN
	};

	enum FORMUlA_STATUS {
		FORMULA_TOP,
		FORMULA_STARTED,
		FORMULA_END
	};

public:
	//結果の場合分け用
	static RESULT result;

	//組み立ての状態
	static STATUS status;

	//スペースまでのストロークの数
	static int count;

	//結果のタイプ識別用
	static bool resultIsOperator;

	//結果出力用オペレータとオペランド
	static Operator *ResultOp;
	static Fraction ResultFrac;

	//結果の数値
	static int resultNum;

	//途中経過を保持する用sttoke配列
	static STROKE strokes[3];

	//式の最初を判別するフラグ（マイナスの値用）
	static FORMUlA_STATUS formulaStatus;

	//桁数を数える
	static byte digit;

	//マックス桁数
	static const byte maxDigit = 10;

	//数字を一時保存するための配列
	static byte numbers[maxDigit];

	//結果のオペレータ一覧
	static OperatorDivide operatorDivide;
	static OperatorLeftBracket operatorLeftBracket;
	static OperatorMinus operatorMinus;
	static OperatorMultiply operatorMultiply;
	static OperatorPlus operatorPlus;
	static OperatorRightBracket operatorRightBracket;


public:

	static void Assemble(STROKE stroke) {


		//最初に文字の組み立て
		AssembleMain(stroke);


		//一文字組み立てが成功したとき
		if (status == SUCCESS) {


			//数式の状態の入れ替え
			if (formulaStatus == FORMULA_TOP) {
				formulaStatus = FORMULA_STARTED;
			}
			else if (formulaStatus == FORMULA_END) {
				formulaStatus = FORMULA_TOP;
			}

			//結果のタイプをセット
			SetResultType();

			//Serial.print(resultIsOperator);

			//結果がオペランドの時
			if (!resultIsOperator) {

				//桁数がマックス以下かどうか判別
				if (digit <= maxDigit) {

					switch (result)
					{
					case StrokeAssembler::RESULT_0:
						numbers[digit] = 0;
						break;
					case StrokeAssembler::RESULT_1:
						numbers[digit] = 1;
						break;
					case StrokeAssembler::RESULT_2:
						numbers[digit] = 2;
						break;
					case StrokeAssembler::RESULT_3:
						numbers[digit] = 3;
						break;
					case StrokeAssembler::RESULT_4:
						numbers[digit] = 4;
						break;
					case StrokeAssembler::RESULT_5:
						numbers[digit] = 5;
						break;
					case StrokeAssembler::RESULT_6:
						numbers[digit] = 6;
						break;
					case StrokeAssembler::RESULT_7:
						numbers[digit] = 7;
						break;
					case StrokeAssembler::RESULT_8:
						numbers[digit] = 8;
						break;
					case StrokeAssembler::RESULT_9:
						numbers[digit] = 9;
						break;
					}

					digit++;
				}
				else {
					status = OVERFLOW;
				}
			}

			//結果がオペレーターの時
			else {

				//結果をオペレーターポインタに代入
				switch (result) {
				case RESULT_LEFT_BRACKET:
					ResultOp = &operatorLeftBracket;
					break;
				case RESULT_RIGHT_BRACKET:
					ResultOp = &operatorRightBracket;
					break;
				case RESULT_PLUS:
					ResultOp = &operatorPlus;
					break;
				case RESULT_MINUS:
					ResultOp = &operatorMinus;
					break;
				case RESULT_MULTIPLY:
					ResultOp = &operatorMultiply;
					break;
				case RESULT_DEVIDE:
					ResultOp = &operatorDivide;
					break;

					//イコールの時は結果の代入はしない
				case RESULT_EQUAL:
					formulaStatus = FORMULA_END;
					break;
				}

				//結果の数値を計算
				for (int i = 0; i < digit; i++) {
					resultNum = resultNum + numbers[digit - 1 - i] * pow(10, i);

					//Serial.println(resultNum);

					//結果の数値をfractionに代入
					ResultFrac.SetNumer(resultNum);

				}

				//桁数、数値を初期化
				digit = 0;
				resultNum = 0;
			}

			//初期化
			count = 0;
			//result = RESULT_UNKNOWN;


		}

		//Serial.println(resultIsOperator);
		//Serial.println();
	}





	//結果がオペランドかオペレーターかを判別
	static bool GetResultIsOperator() {
		return resultIsOperator;
	}

	static STATUS GetStatus() {
		return status;
	}

	//オペレーターの結果を返す
	static Operator* GetResultOperator() {
		return ResultOp;
	}

	//結果のオペランドを返す
	static Fraction& GetResultOperand() {
		return ResultFrac;
	}

	//数式の状態を返す
	static FORMUlA_STATUS GetFormulaStatus() {
		return formulaStatus;
	}

	//数式の状態をリセット
	static void ResetFormulaStatus() {
		formulaStatus = FORMULA_TOP;
	}


private:

	//スペースがまでのストロークから文字を一文字判別
	static void AssembleMain(STROKE stroke) {

		//スペースが来たときはリテラルの終了
		if (stroke == STROKE_SPACE) {
			//ストロークの初期化

			//結果が不明の時はFAILEDを返す。
			if (result == RESULT_UNKNOWN) {
				status = FAILED;
			}
			//それ以外の時は成功
			else {
				status = SUCCESS;
			}
		}
		//ストローク1回目
		else {

			if (count == 0) {

				switch (stroke) {

				case STROKE_DOT:
					result = RESULT_UNKNOWN;
					break;

				case STROKE_LEFT_BRACKET:
					result = RESULT_LEFT_BRACKET;
					break;

				case STROKE_RIGHT_BRACKET:
					result = RESULT_RIGHT_BRACKET;
					break;

				case STROKE_0:
					result = RESULT_0;
					break;

				case STROKE_2:
					result = RESULT_2;
					break;

				case STROKE_3:
					result = RESULT_3;
					break;

				case STROKE_6:
					result = RESULT_6;
					break;

				case STROKE_8:
					result = RESULT_8;
					break;

				case STROKE_9:
					result = RESULT_9;
					break;

				case STROKE_VERTICAL_LINE:
					result = RESULT_1;
					break;

				case STROKE_HORIZONTAL_LINE:
					result = RESULT_MINUS;
					break;

				case STROKE_SLASH:
					result = RESULT_DEVIDE;
					break;

				case STROKE_BACK_SLASH:
					result = RESULT_MULTIPLY;
					break;

				case STROKE_PART_OF_4:
				case STROKE_PART_OF_5:
				case STROKE_PART_OF_7:
					result = RESULT_UNKNOWN;
					break;
				}
			}
			//ストローク２回目
			else if (count == 1) {
				switch (stroke) {
					//ストローク２回目に左括弧、右括弧、0、２、３、６、８、９が来たときは、エラー。
				case STROKE_LEFT_BRACKET:
				case STROKE_RIGHT_BRACKET:
				case STROKE_0:
				case STROKE_2:
				case STROKE_3:
				case STROKE_6:
				case STROKE_8:
				case STROKE_9:
					result = RESULT_UNKNOWN;
					break;

					//２回目のストロークがドットの時
				case STROKE_DOT:
					//一つ前のストロークがドットか横線の時は、÷
					if ((strokes[count - 1] == STROKE::STROKE_HORIZONTAL_LINE) || (strokes[count - 1] == STROKE_DOT)) {
						strokes[count] = stroke;
						result = RESULT_DEVIDE;
					}
					//その他はエラー
					else {
						result = RESULT_UNKNOWN;
					}
					break;

					//２回目のストロークが４の一部の時
				case STROKE_PART_OF_4:
					//一つ前のストロークが縦線の時、４
					if (strokes[count - 1] == STROKE_VERTICAL_LINE) {
						result = RESULT_4;
					}
					//その他はエラー
					else {
						result = RESULT_UNKNOWN;
					}
					break;

					//２回目のストロークが５の一部の時
				case STROKE_PART_OF_5:
					//一つ前のストロークが横線の時、５
					if (strokes[count - 1] == STROKE_HORIZONTAL_LINE) {
						result = RESULT_5;
					}
					//その他はエラー
					else {
						result = RESULT_UNKNOWN;
					}
					break;

					//２回目のストロークが７の一部の時
				case STROKE_PART_OF_7:
					//一つ前のストロークが縦線の時、７
					if (strokes[count - 1] == STROKE_VERTICAL_LINE) {
						result = RESULT_7;
					}
					//その他の時、不明
					else {
						result = RESULT_UNKNOWN;
					}
					break;

					//２回目のストロークが縦線の時
				case STROKE_VERTICAL_LINE:
					switch (strokes[count - 1]) {
						//一つ前のストロークが４の一部の時、４
					case STROKE_PART_OF_4:
						result = RESULT_4;
						break;

						//一つ前のストロークが７の時、７
					case STROKE_PART_OF_7:
						result = RESULT_7;
						break;

						//一つ前のストロークが横線の時、＋
					case STROKE_HORIZONTAL_LINE:
						result = RESULT_PLUS;
						break;

						//その他の時はエラー
					default:
						result = RESULT_UNKNOWN;
						break;
					}

					break;

					//二回目のストロークが横線の時
				case STROKE_HORIZONTAL_LINE:
					switch (strokes[count - 1]) {
						//一つ前のストロークが５の一部の時、５
					case STROKE_PART_OF_4:
						result = RESULT_4;
						break;

						//一つ前のストロークが縦線の時、＋
					case STROKE_VERTICAL_LINE:
						result = RESULT_PLUS;
						break;

						//一つ前のストロークがドットの時、÷
					case STROKE_DOT:
						result = RESULT_DEVIDE;
						break;

						//一つ前のストロークが横線の時、＝
					case STROKE_HORIZONTAL_LINE:
						result = RESULT_EQUAL;
						break;
						//その他の時はエラー
					default:
						result = RESULT_UNKNOWN;
						break;
					}
					break;
				}
			}
			//ストローク３回目
			else if (count == 2) {
				switch (stroke) {
					//３回目のストロークがドットの時
				case STROKE_DOT:
					//一回目と二回目のストロークがドットと横線である時、÷
					if ((strokes[count - 1] == STROKE_DOT && strokes[count - 2] == STROKE_VERTICAL_LINE) || (strokes[count - 1] == STROKE_VERTICAL_LINE && strokes[count - 2] == STROKE_DOT)) {
						result = RESULT_DEVIDE;
					}
					break;


					//３回目のストロークが横線の時
				case STROKE_HORIZONTAL_LINE:
					//一回目と二回目のストロークがどちらもドットである時、÷
					result = RESULT_DEVIDE;
					break;

					//その他の時、不明
				default:
					result = RESULT_UNKNOWN;
					break;
				}
			}
			//ストロークが四回以上あるとき
			else {
				result = RESULT_UNKNOWN;
			}

			//ストロークが三回以下の時のみ結果を保持
			if (count < 3) {
				strokes[count] = stroke;
			}
			status = ASSEMBLING;
			count++;
		}
	}


	//結果がオペランドかオペレーター化をセットする
	static void SetResultType(void) {
		switch (result) {
		case RESULT_LEFT_BRACKET:
		case RESULT_RIGHT_BRACKET:
		case RESULT_PLUS:
		case RESULT_MINUS:
		case RESULT_MULTIPLY:
		case RESULT_DEVIDE:
		case RESULT_EQUAL:
			resultIsOperator = true;
			break;
		case RESULT_0:
		case RESULT_1:
		case RESULT_2:
		case RESULT_3:
		case RESULT_4:
		case RESULT_5:
		case RESULT_6:
		case RESULT_7:
		case RESULT_8:
		case RESULT_9:
			resultIsOperator = false;
			break;
		}
	}
};



//変数の初期化

//結果の場合分け用
StrokeAssembler::RESULT StrokeAssembler::result = RESULT_UNKNOWN;

//組み立ての状態
StrokeAssembler::STATUS StrokeAssembler::status = FAILED;

//スペースまでのストロークの数
int StrokeAssembler::count = 0;

//結果のタイプ識別用
bool StrokeAssembler::resultIsOperator = true;

//結果出力用オペレータとオペランド
Operator* StrokeAssembler::ResultOp;
Fraction StrokeAssembler::ResultFrac;

//結果の数値
int StrokeAssembler::resultNum = 0;

//途中経過を保持する用sttoke配列
STROKE StrokeAssembler::strokes[3] = { STROKE_SPACE, STROKE_SPACE, STROKE_SPACE };

//数字を一時保存するための配列
byte StrokeAssembler::numbers[maxDigit];

//式の最初を判別するフラグ（マイナスの値用）
StrokeAssembler::FORMUlA_STATUS StrokeAssembler::formulaStatus = FORMULA_END;

//桁数を数える
byte StrokeAssembler::digit = 0;

//結果のオペレータ一覧
OperatorDivide StrokeAssembler::operatorDivide;
OperatorLeftBracket StrokeAssembler::operatorLeftBracket;
OperatorMinus StrokeAssembler::operatorMinus;
OperatorMultiply StrokeAssembler::operatorMultiply;
OperatorPlus StrokeAssembler::operatorPlus;
OperatorRightBracket StrokeAssembler::operatorRightBracket;

#endif
