#include <TFT.h>
#include <SPI.h>
#include "Calculator.h"


#define CS   10
#define DC   9
#define RST  8

OperatorDivide operatorDivide;
OperatorLeftBracket operatorLeftBracket;
OperatorMinus operatorMinus;
OperatorMultiply operatorMultiply;
OperatorPlus operatorPlus;
OperatorRightBracket operatorRightBracket;

Operator *outputOperator;
Fraction outputFraction;

TFT tft = TFT(CS, DC, RST);


Calculator cal(20, 20);


DeclareTaskLoop(Blinker);

Calculator::CAL_STATUS pushResult;


void setup() {
  pinMode(13, OUTPUT);

  Serial.begin(19200);

  tft.begin();
  tft.setRotation(0);
  tft.background(0,0,0);
  tft.stroke(255,255,255);

  InitMainLoopStackSize(200);

  CreateTaskLoop(Blinker, LOW_PRIORITY);

}

void loop() {
  
  cal.Put(Fraction(1, 2));
  pushResult = cal.TopOfOperandStack(&outputFraction);
  if(pushResult == Calculator::CAL_STATUS::CAL_STATUS_SUCCESS){
  tft.print(outputFraction.ToString());
  pushResult = Calculator::CAL_STATUS::CAL_STATUS_SOMETHING_ERROR;
  }
  
  cal.Put(&operatorPlus);
  pushResult = cal.TopOfOperatorPointerStack(&outputOperator);
  if(pushResult == Calculator::CAL_STATUS::CAL_STATUS_SUCCESS){
  tft.print(outputOperator->token);
  Serial.println(outputOperator->token);
  pushResult = Calculator::CAL_STATUS::CAL_STATUS_SOMETHING_ERROR;
  }

  cal.Put(Fraction(1, 3));
  pushResult = cal.TopOfOperandStack(&outputFraction);
  if(pushResult == Calculator::CAL_STATUS::CAL_STATUS_SUCCESS){
  tft.print(outputFraction.ToString());
  pushResult = Calculator::CAL_STATUS::CAL_STATUS_SOMETHING_ERROR;
  }
  
  cal.Put(&operatorMultiply);
  pushResult = cal.TopOfOperatorPointerStack(&outputOperator);
  if(pushResult == Calculator::CAL_STATUS::CAL_STATUS_SUCCESS){
  tft.print(outputOperator->token);
  Serial.println(outputOperator->token);
  pushResult = Calculator::CAL_STATUS::CAL_STATUS_SOMETHING_ERROR;
  }
  
  cal.Put(Fraction(5, 1));
  pushResult = cal.TopOfOperandStack(&outputFraction);
  if(pushResult == Calculator::CAL_STATUS::CAL_STATUS_SUCCESS){
  tft.print(outputFraction.ToString());
  pushResult = Calculator::CAL_STATUS::CAL_STATUS_SOMETHING_ERROR;
  }

  cal.Compute();
  tft.print("=");


  Fraction resFrac;

  cal.TopOfOperandStack(&resFrac);


  tft.println(resFrac.ToString());



  while (true);
}

TaskLoop(Blinker) {
  digitalWrite(13, HIGH);
  DelayWithBlocked(1000);

  digitalWrite(13, LOW);
  DelayWithBlocked(1000);
}
