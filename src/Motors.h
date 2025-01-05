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
    bool debugMode;

public:
    Motors(int m1p1, int m1p2, int m1spd, int m2p1, int m2p2, int m2spd, bool useDebugMode); // Constructor
    bool f(long m1c, long m2c); // Go forwards 
    bool l(int numRotationsM1, int numRotationsM2); // Go Left
    bool r(int numRotationsM1, int numRotationsM2); // Go Right

};

#endif