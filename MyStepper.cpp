#include "MyStepper.h"


MyStepper::MyStepper(int pin1, int pin2, int pin3, int pin4):Stepper(2048, pin1, pin2, pin3, pin4) 
{

}

void MyStepper::walk(MyStepper::Direction direction)
{
    Stepper::step(1 * direction);
}
