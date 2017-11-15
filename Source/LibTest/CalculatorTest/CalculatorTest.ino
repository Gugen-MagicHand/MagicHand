#include "Calculator.h"

OperatorDivide operatorDivide;
OperatorLeftBracket operatorLeftBracket;
OperatorMinus operatorMinus;
OperatorMultiply operatorMultiply;
OperatorPlus operatorPlus;
OperatorRightBracket operatorRightBracket;





Calculator cal(20, 20);


DeclareTaskLoop(Blinker);


void setup() {
  pinMode(13, OUTPUT);

  Serial.begin(19200);

  InitMainLoopStackSize(100);

  CreateTaskLoop(Blinker, LOW_PRIORITY);
}

void loop() {
  cal.Put(&operatorLeftBracket);
  cal.Put(&operatorMinus);
  cal.Put(Fraction(1));
  cal.Put(&operatorRightBracket);
  cal.Put(&operatorMultiply);
  cal.Put(&operatorLeftBracket);

  cal.Put(Fraction(2));
  cal.Put(&operatorMinus);

  cal.Put(Fraction(3));
  cal.Put(&operatorMultiply);

  cal.Put(Fraction(5));
  cal.Put(&operatorMinus);

  cal.Put(Fraction(2));
  cal.Put(&operatorRightBracket);

  cal.Compute();



  Fraction resFrac;

  cal.TopOfOperandStack(&resFrac);


  Serial.println(resFrac.ToString());



  while (true);
}

TaskLoop(Blinker) {
  digitalWrite(13, HIGH);
  DelayWithBlocked(1000);

  digitalWrite(13, LOW);
  DelayWithBlocked(1000);
}
