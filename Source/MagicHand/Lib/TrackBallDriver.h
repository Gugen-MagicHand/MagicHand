//更新履歴
//  2017/11/2:
//    ピン設定をBegin()の引数とした。


#ifndef TRACKBALL_DRIVER_H
#define TRACKBALL_DRIVER_H

class TrackBallDriver
{
public:
    unsigned char pinBTN;
    unsigned char pinLFT;
    unsigned char pinRHT;
    unsigned char pinUP;
    unsigned char pinDWN;
    unsigned char pinWHT;
    unsigned char pinGRN;
    unsigned char pinRED;
    unsigned char pinBLU;

private:
    bool ledMode = false;

protected:
    long up;
    long down;
    long right;
    long left;

    unsigned long timeout = 1000000;




public:
    void Begin(unsigned char pinBTN, unsigned char pinLFT, unsigned char pinRHT, unsigned char pinUP, unsigned char pinDWN) {

        pinMode(pinBTN, INPUT);
        pinMode(pinLFT, INPUT);
        pinMode(pinRHT, INPUT);
        pinMode(pinUP, INPUT);
        pinMode(pinDWN, INPUT);

        this->pinBTN = pinBTN;
        this->pinLFT = pinLFT;
        this->pinRHT = pinRHT;
        this->pinUP = pinUP;
        this->pinDWN = pinDWN;
    }

    void Begin(unsigned char pinBTN, unsigned char pinLFT, unsigned char pinRHT, unsigned char pinUP, unsigned char pinDWN,
        unsigned char pinWHT, unsigned char pinGRN, unsigned char pinRED, unsigned char pinBLU) {

        pinMode(pinBTN, INPUT);
        pinMode(pinLFT, INPUT);
        pinMode(pinRHT, INPUT);
        pinMode(pinUP, INPUT);
        pinMode(pinDWN, INPUT);

        this->pinBTN = pinBTN;
        this->pinLFT = pinLFT;
        this->pinRHT = pinRHT;
        this->pinUP = pinUP;
        this->pinDWN = pinDWN;

        pinMode(pinWHT, OUTPUT);
        pinMode(pinGRN, OUTPUT);
        pinMode(pinRED, OUTPUT);
        pinMode(pinBLU, OUTPUT);

        digitalWrite(pinWHT, LOW);
        digitalWrite(pinGRN, LOW);
        digitalWrite(pinRED, LOW);
        digitalWrite(pinDWN, LOW);

        this->pinWHT = pinWHT;
        this->pinGRN = pinGRN;
        this->pinRED = pinRED;
        this->pinBLU = pinBLU;

    }

    void LEDColor(bool r, bool g, bool b, bool w) {
        digitalWrite(pinRED, r);
        digitalWrite(pinGRN, g);
        digitalWrite(pinBLU, b);
        digitalWrite(pinWHT, w);
    }

    void SetTimeout(unsigned long timeout) {
        this->timeout = timeout;
    }

    long GetLeft(void) {
        return left;
    }

    long GetRight(void) {
        return right;
    }

    long GetUp(void) {
        return up;
    }

    long GetDown(void) {
        return down;
    }

    bool GetButton() {
        return digitalRead(pinBTN);
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
