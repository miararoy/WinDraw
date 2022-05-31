#include "MyStepper.h"

MyStepper LS(6, 8, 7, 9);
MyStepper RS(2, 4, 3, 5);

int stepCount = 0;
const double a_cm = 100;
const double b_cm = 15;
const double td_cm = 10;
const double r_cm = 2;

double calc_l(double x, double y, MyStepper::Orientation orientation){
  double h = td_cm + y;
  double l = sqrt(pow(h, 2) + pow(a_cm/2 - b_cm/2 + orientation * x, 2));
  return l;
}

void setup() {
  Serial.begin(9600);
}

void moveTo(MyStepper &L, MyStepper &R, int from_x, int from_y,int to_x, int to_y){
  double ratio;
  double l_L_i = calc_l(from_x, from_y, MyStepper::Orientation::L);
  double l_R_i = calc_l(from_x, from_y, MyStepper::Orientation::R);
  double l_L_f = calc_l(to_x, to_y, MyStepper::Orientation::L);
  double l_R_f = calc_l(to_x, to_y, MyStepper::Orientation::R);

  // Serial.println("l_L_i: " + String(l_L_i));
  // Serial.println("l_R_i: " + String(l_R_i));
  // Serial.println("l_L_f: " + String(l_L_f));
  // Serial.println("l_R_f: " + String(l_R_f));

  double delta_l_L = l_L_f - l_L_i;
  double delta_l_R = l_R_f - l_R_i;

  // Serial.println("delta_l_L: " + String(delta_l_L));
  // Serial.println("delta_l_R: " + String(delta_l_R));

  int stepsL = abs(ceil(2048 * delta_l_L / (r_cm * TWO_PI)));
  int stepsR = abs(ceil(2048 * delta_l_R / (r_cm * TWO_PI)));

  // Serial.println("stepsL: " + String(stepsL));
  // Serial.println("stepsR: " + String(stepsR));

  int lsign = delta_l_L > 0 ? MyStepper::Direction::FORWARD : MyStepper::Direction::BACKWARD;
  int rsign = delta_l_R > 0 ? MyStepper::Direction::FORWARD : MyStepper::Direction::BACKWARD;

  // Serial.println("lsign: " + String(lsign));
  // Serial.println("rsign: " + String(rsign));

  // TODO: fill this up
  if (stepsL > stepsR) {
    ratio = (double)(stepsL - stepsR) / (double)stepsL;
    while (stepsR > 0){

      // Serial.println("Steps L left: " + String(stepsL));
      // Serial.println("Steps R left: " + String(stepsR));
      
      L.step(lsign);
      stepsL--;
      if ((double)random(100) / 100.0 <= ratio) {
        L.step(lsign);
        stepsL--;
      }
      R.step(rsign);
      stepsR--;
      delay(5);
    }
    while (stepsL > 0){
      
      // Serial.println("Steps L left: " + String(stepsL));
      // Serial.println("Steps R left: " + String(stepsR));

      L.step(lsign);
      stepsL--;
      delay(5);
    }
  } else {
    ratio = (double)(stepsR - stepsL) / (double)stepsR;
    while (stepsL > 0){
      
      // Serial.println("Steps L left: " + String(stepsL));
      // Serial.println("Steps R left: " + String(stepsR));

      R.step(rsign);
      if ((double)random(100) / 100.0 <= ratio) {
        R.step(rsign);
      }
      L.step(lsign);
      stepsL--;
      stepsR--;
      delay(5);
    }
    while (stepsR > 0){
      
      // Serial.println("Steps L left: " + String(stepsL));
      // Serial.println("Steps R left: " + String(stepsR));

      R.step(rsign);
      stepsR--;
      delay(5);
    }
  }
  
}

void loop() {
  moveTo(LS, RS, 0, 0, 10, 10);
  delay(2000);
  moveTo(LS, RS, 10, 10, 0, 0);
  delay(2000);
}