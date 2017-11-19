#ifndef CALCULATOR_DISPLAY_H
#define CALCULAtOR_DISPLAY_H


#include <TFT.h>
#include "LiteralQueue.h"
#include "Stroke.h"

class CalculatorDisplay : public TFT
{

public:

	int nextCanvasQueueCount;
	bool nextIsFraction;

	String nextTopStringInAnswer;
	String nextBottomStringInAnswer;

	String nextCalculatorStatusString;

	static const int FORMULA_LITERAL_QUEUE_CAPACITY = 19;


private:
	int canvasQueueCount;
	bool isFraction;
	String topStringInAnswer;
	String bottomStringInAnswer;
	String calculatorStatusString;

	Queue<STROKE> strokeQueue;

	LiteralQueue formulaLiteralQueue;

	bool isFormulaUpdated;


public:
	CalculatorDisplay(uint8_t CS, uint8_t RS, uint8_t RST) : TFT(CS, RS, RST), strokeQueue(10), formulaLiteralQueue(FORMULA_LITERAL_QUEUE_CAPACITY)
	{};


	void Begin() {
		begin();
		setRotation(1);
		background(0, 0, 0);
		stroke(255, 255, 255);
		SetFrame();
		canvasQueueCount = 0;
		nextCanvasQueueCount = 0;
		isFraction = true;
		DrawDisplayMode();
		nextTopStringInAnswer = "";
		nextBottomStringInAnswer = "";
		isFormulaUpdated = false;
		setTextWrap(true);
	}


	//private:

	void SetFrame() {
		drawFastHLine(0, 42, width(), ST7735_WHITE);
		drawFastVLine(100, 0, width(), ST7735_WHITE);
		drawFastHLine(101, 84, 60, ST7735_WHITE);
		text("Formula", 1, 1);
		text("Answer", 1, 43);
		for (int i = 0; i < 5; i++) {
			drawCircle(106 + 12 * i, 4, 4, ST7735_WHITE);
			drawCircle(106 + 12 * i, 14, 4, ST7735_WHITE);
		}
		text("Mode", 102, 44);
		text("Calculator", 102, 86);
	}


	void DrawCanvasQueueCount() {

		if (nextCanvasQueueCount < 0) {
			nextCanvasQueueCount = 0;
		}

		if (nextCanvasQueueCount > 10) {
			nextCanvasQueueCount = 10;
		}
	

		while (canvasQueueCount != nextCanvasQueueCount) {

			//表示している方が多い時（表示を減らす）
			if (canvasQueueCount > nextCanvasQueueCount) {
				if (canvasQueueCount <= 5) {
					fillCircle(106 + 12 * (canvasQueueCount - 1), 4, 4, ST7735_BLACK);
					drawCircle(106 + 12 * (canvasQueueCount - 1), 4, 4, ST7735_WHITE);
				}
				else {
					fillCircle(106 + 12 * (canvasQueueCount - 6), 14, 4, ST7735_BLACK);
					drawCircle(106 + 12 * (canvasQueueCount - 6), 14, 4, ST7735_WHITE);
				}

				canvasQueueCount--;
			}
			//表示している方が少ない時（表示を増やす）
			else {
				if (canvasQueueCount < 5) {
					fillCircle(106 + 12 * canvasQueueCount, 4, 4, ST7735_WHITE);
				}
				else {
					fillCircle(106 + 12 * (canvasQueueCount - 5), 14, 4, ST7735_WHITE);
				}
				canvasQueueCount++;
			}

		}
	}

	void FormulaLiteralQueuePush(LITERAL literalToPush) {
		while (!formulaLiteralQueue.Push(literalToPush))
		{
			LITERAL lit;
			formulaLiteralQueue.Pop(&lit);
		}

		isFormulaUpdated = true;
	}

	bool FormulaLiteralQueuePopBack(LITERAL *literalToPop) {
		return formulaLiteralQueue.PopBack(literalToPop);
		isFormulaUpdated = true;
	}


	void DrawFormula() {
		if (isFormulaUpdated) {

			fillRect(1, 10, 99, 32, ST7735_BLACK);
			for (int i = 0; i < formulaLiteralQueue.Count(); i++) {
				LITERAL lit = formulaLiteralQueue[i];
				text(LiteralToString(lit).c_str(), 1 + i * 5, 16);
			}
			isFormulaUpdated = false;
		}
	}

	void DrawDisplayMode() {
		if (isFraction != nextIsFraction) {
			if (nextIsFraction) {
				text("Fraction", 104, 54);
			}
			else {
				text("Decimal", 104, 54);
			}

			isFraction = nextIsFraction;
		}
	}



	void DrawAnswer() {
		if ((nextTopStringInAnswer != topStringInAnswer) || (nextBottomStringInAnswer != bottomStringInAnswer)) {
			//answer画面の初期化
			fillRect(0, 50, 99, 79, ST7735_BLACK);

			//分母が1の時
			if (nextBottomStringInAnswer == "") {
				setTextSize(3);
				text(nextTopStringInAnswer.c_str(), 10, 55);
			}
			else {
				setTextSize(2);
				text(nextTopStringInAnswer.c_str(), 10, 70);
				text(nextBottomStringInAnswer.c_str(), 10, 92);
				int maxLength = max(nextTopStringInAnswer.length(), nextBottomStringInAnswer.length());
				drawFastHLine(10, 90, 12 * maxLength, ST7735_WHITE);
			}

			topStringInAnswer = nextTopStringInAnswer;
			bottomStringInAnswer = nextBottomStringInAnswer;

			setTextSize(1);

		}
	}

	void DrawCalculatorStatus() {
		if (calculatorStatusString != nextCalculatorStatusString) {
			//answer画面の初期化
			fillRect(102, 94, 59, 27, ST7735_BLACK);

			text(nextCalculatorStatusString.c_str(), 102, 96);

			calculatorStatusString = nextCalculatorStatusString;
		}
	}

	/*
	void SetAnswerPosition(uint8_t stringLength) {

	}
	*/

};






#endif // !CALCULATOR_DISPLAY_H
