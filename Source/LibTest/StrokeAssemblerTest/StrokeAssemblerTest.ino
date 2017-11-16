#include "StrokeAssembler.h"

STROKE strokes[] = {
	STROKE_HORIZONTAL_LINE,
	STROKE_SPACE,
	STROKE_VERTICAL_LINE,
	STROKE_PART_OF_7,
	STROKE_SPACE,
	STROKE_VERTICAL_LINE,
	STROKE_PART_OF_7,
	STROKE_SPACE,
	STROKE_VERTICAL_LINE,
	STROKE_HORIZONTAL_LINE,
	STROKE_SPACE,
	STROKE_6,
	STROKE_SPACE,
	STROKE_8,
	STROKE_SPACE,
	STROKE_HORIZONTAL_LINE,
	STROKE_HORIZONTAL_LINE,
	STROKE_SPACE,
	STROKE_VERTICAL_LINE,
	STROKE_HORIZONTAL_LINE,
	STROKE_SPACE,
	STROKE_6,
	STROKE_SPACE,
	STROKE_HORIZONTAL_LINE,
	STROKE_HORIZONTAL_LINE,
	STROKE_SPACE,
};

Fraction frac(0);
Operator *op;
Calculator cal(10, 10);


void setup() {
	//InitMainLoopStackSize(200);
	Serial.begin(19200);
	cal.Put(frac);
}

void loop() {
	for (int i = 0; i < (sizeof(strokes) / sizeof(strokes[0])); i++) {

		//Serial.println(StrokeAssembler::formulaStatus);

		StrokeAssembler::Assemble(strokes[i]);

		/*
		Serial.print("count");
		Serial.println(StrokeAssembler::count);
		Serial.print("digit");
		Serial.println(StrokeAssembler::digit);
		Serial.print("status");
		Serial.println(StrokeAssembler::status);
		Serial.print("isOperator");
		Serial.println(StrokeAssembler::resultIsOperator);
		Serial.print("result");
		Serial.println(StrokeAssembler::result);
		Serial.print("negative");
		Serial.println(StrokeAssembler::isNegativeNumber);
		Serial.print("started");
		Serial.println(StrokeAssembler::started);
		*/

		//Serial.println(StrokeAssembler::resultIsOperator);


		if (StrokeAssembler::status == StrokeAssembler::SUCCESS) {

			//Serial.println("SUCCESS");

			if (StrokeAssembler::GetResultIsOperator()) {


				//���̍ŏ��̎�
				if (StrokeAssembler::GetFormulaStatus() == StrokeAssembler::FORMULA_TOP) {
					//Serial.println("TOP");
					op = StrokeAssembler::GetResultOperator();
					cal.Put(op);
					Serial.print(frac.ToString());
					Serial.print(op->token);
				}
				//���̓r���̎�
				else if (StrokeAssembler::GetFormulaStatus() == StrokeAssembler::FORMULA_STARTED) {
					//Serial.println("STARTED");

					frac = StrokeAssembler::GetResultOperand();
					op = StrokeAssembler::GetResultOperator();

					cal.Put(frac);
					cal.Put(op);

					Serial.print(frac.ToString());
					Serial.print(op->token);
				}
				//���̏I���̎�
				else {
					//Serial.println("END");

					frac = StrokeAssembler::GetResultOperand();
					Serial.print(frac.ToString());

					cal.Put(frac);
					cal.Compute();
					cal.TopOfOperandStack(&frac);

					Serial.print("=");
					Serial.println(frac.ToString());
				}
			}
		}

		//Serial.println();

	}

	while (true);
}
