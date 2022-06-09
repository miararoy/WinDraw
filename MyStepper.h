#ifndef _MyStepper_h
#define _MyStepper_h

#include <Stepper.h>


class MyStepper:public Stepper {
    public:
        enum Direction {FORWARD=-1, BACKWARD=1};
        enum Orientation {R=1, L=-1};
        double x;
        double y;
        MyStepper(int pin1, int pin2, int pin3, int pin4);
        void walk(MyStepper::Direction direction);
        // void move(int _x, int _y);
};
#endif


