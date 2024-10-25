#ifndef MOTORS_H
#define MOTORS_H

class Motors {
private:
    int pinM1p1;
    int pinM1p2;
    int pinM1spd;
    int pinM2p1;
    int pinM2p2;
    int pinM2spd;

public:
    Motors(int m1p1, int m1p2, int m1spd, int m2p1, int m2p2, int m2spd); // Constructor
    void f(int spd); // Go forwards 
    void b(); // Go Backwards
    void l(); // Go Left
    void r(); // Go Right

};

#endif