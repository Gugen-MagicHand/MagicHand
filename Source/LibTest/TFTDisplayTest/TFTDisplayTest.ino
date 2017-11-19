#include <TFT.h>
#include <SPI.h>
#include "CalculatorDisplay.h"

//13ピンがCRK, 11ピン(MOSI)がSDA、12ピン(MISO)は空ピン
#define CS   10
#define DC   9
#define RST  8

/*
OperatorDivide operatorDivide;
OperatorLeftBracket operatorLeftBracket;
OperatorMinus operatorMinus;
OperatorMultiply operatorMultiply;
OperatorPlus operatorPlus;
OperatorRightBracket operatorRightBracket;

Operator *outputOperator;
Fraction outputFraction;
*/

CalculatorDisplay tft(CS, DC, RST);


void setup() {
	pinMode(13, OUTPUT);

	Serial.begin(19200);

	tft.Begin();

	//InitMainLoopStackSize(200);

	tft.nextTopStringInAnswer = "3333";
	tft.nextBottomStringInAnswer = "44444";

	tft.DrawAnswer();

	tft.nextCalculatorStatusString = "testtest";
	tft.DrawCalculatorStatus();

	/*
	for (int i = 0; i < 30; i++) {
		tft.FormulaLiteralQueuePush(i % 18);
		tft.DrawFormula();
		delay(100);
		if (i % 2 == 0) {
			LITERAL lit;
			tft.FormulaLiteralQueuePopBack(&lit);
		}
	}
	*/

	for (int i = 0; i <= 12; i++) {
		tft.nextCanvasQueueCount = i;
		tft.DrawCanvasQueueCount();
		tft.nextCanvasQueueCount = i - 4;
		tft.DrawCanvasQueueCount();

		delay(1000);
	}

	
}

void loop() {

}
