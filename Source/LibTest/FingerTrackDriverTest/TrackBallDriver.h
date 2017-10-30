#ifndef TRACKBALL_DRIVER_H
#define TRACKBALL_DRIVER_H

class TrackBallDriver
{
  public:
    byte pinBTN;
    byte pinLFT;
    byte pinRHT;
    byte pinUP;
    byte pinDWN;
    byte pinWHT;
    byte pinGRN;
    byte pinRED;
    byte pinBLU;

  private:
    bool ledMode = false;

  protected:
    long up;
    long down;
    long right;
    long left;
    
    unsigned long timeout = 1000000;




  public:
    void Begin(void) {

      pinMode(pinBTN, INPUT);
      pinMode(pinLFT, INPUT);
      pinMode(pinRHT, INPUT);
      pinMode(pinUP, INPUT);
      pinMode(pinDWN, INPUT);

      if (ledMode) {
        pinMode(pinWHT, OUTPUT);
        pinMode(pinGRN, OUTPUT);
        pinMode(pinRED, OUTPUT);
        pinMode(pinBLU, OUTPUT);

        digitalWrite(pinWHT, LOW);
        digitalWrite(pinGRN, LOW);
        digitalWrite(pinRED, LOW);
        digitalWrite(pinDWN, LOW);
      }
    }

    void SetLEDMode(bool ledMode) {
      this->ledMode = ledMode;
    }

    void SetTimeout(unsigned long timeout) {
      this->timeout = timeout;
    }

    long GetLeft(void){
      return left;
    }

    long GetRight(void){
      return right;
    }

    long GetUp(void){
      return up;
    }

    long GetDown(void){
      return down;
    }


    void ReadLeft(void) {
      left = pulseIn(pinLFT, HIGH, timeout);
    }

    void ReadRight(void) {
      right = pulseIn(pinRHT, HIGH, timeout);
    }

    void ReadUp(void) {
      up = pulseIn(pinUP, HIGH, timeout);
    }

    void ReadDown(void) {
      down = pulseIn(pinDWN, HIGH, timeout);
    }

};



#endif TRACKBALL_DRIVER_H
