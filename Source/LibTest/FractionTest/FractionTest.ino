//#include "malloc.h"
#include "Fraction.h"

Fraction fracA, fracB;

DeclareTaskLoop(BlinkTask);
SemaphoreHandle semaphore;

int blinkCount = 0;

void setup() {
  /*
    // put your setup code here, to run once:
    pinMode(10, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(5, OUTPUT);

    analogWrite(5, 127);
  */
  CreateBinarySemaphore(semaphore);

  Serial.begin(19200);
  Serial.println("Setup");
  Serial.println(RAMEND);
  //Serial.println(mainLoopStackSize);
  InitMainLoopStackSize(500);
  //Serial.println(mainLoopStackSize);


  //CreateTaskLoop(HigherTask, HIGH_PRIORITY);

  delay(3000);
  //DelayWithBlocked(5000);

  fracA(1, 3);
  fracB(1, 4);

  pinMode(13, OUTPUT);

  //CreateTaskLoop(BlinkTask, LOW_PRIORITY);
  CreateTaskLoopWithStackSize(BlinkTask, LOW_PRIORITY, 100);
}

TaskLoop(BlinkTask) {
  unsigned long prevWakeTime = GetTickCount();
  bool sw = false;
  for (;;) {
    DelayUntilWithBlocked(&prevWakeTime, 500);
    digitalWrite(13, sw ^= true);

    if (Acquire(semaphore, 100)) {

      Serial.print("Blink");
      Serial.println(blinkCount++);
      Serial.flush();
      Release(semaphore);
    }
  }
}

/*
  TaskLoop(task){
  char str[] = "Test";
  Serial.println(str);

  delay(1000);
  }
*/

void loop() {


  if (Acquire(semaphore, 100)) {

    int a = 0;
    int b = 0;

    unsigned long aPointer = &a;
    unsigned long bPointer = &b;
    Serial.println(aPointer);
    Serial.println(bPointer);
    /*
      String str("Test");
      Serial.println(str);
    */
    /*
      int *cp = malloc(sizeof(int));
      Serial.println((unsigned long)cp);

      //free(cp);
    */

    String str = fracA.ToString() + " + " + fracB.ToString() + " = ";

    Serial.print(str);

    //Serial.flush();
    Fraction temp = fracA;
    fracA = fracA + fracB;
    fracB = temp;

    Serial.println(fracA.ToString());

    Serial.flush();
    for (int i = 0; i < 10; i++) {

      Serial.println(String(i) + ": " + "This sentence is very long.");
      Serial.flush();
    }
    Release(semaphore);
  }
  /*
    // put your main code here, to run repeatedly:
    digitalWrite(10, HIGH);
    digitalWrite(13, HIGH);
    delay(1000);

    digitalWrite(10, LOW);
    digitalWrite(13, LOW);
    delay(1000);


    //throw("123");
  */


  delay(3000);
  //DelayWithBlocked(3000);
}