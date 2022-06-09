#include <Servo.h>
#include "MyStepper.h"

Servo servo;

MyStepper LS(6, 8, 7, 9);
MyStepper RS(2, 4, 3, 5);

int stepCount = 0;
const double a_cm = 80;
const double b_cm = 18.5;
const double td_cm = 24;
const double r_cm = 1.25;
const int maxY = 40;
const int maxX = 20;

int maxSoundLevel = 0;

double calc_l(double x, double y, MyStepper::Orientation orientation){
  double h = td_cm + y;
  double l = sqrt(pow(h, 2) + pow(a_cm/2 - b_cm/2 + orientation * x, 2));
  return l;
}

void setup() {
  Serial.begin(9600);
  servo.attach(10);
  servo.write(0);
  maxSoundLevel = max(maxSoundLevel, analogRead(A0));
}

void moveTo(MyStepper &L, MyStepper &R, int from_x, int from_y,int to_x, int to_y){
  double ratio;
  double l_L_i = calc_l(from_x, from_y, MyStepper::Orientation::L);
  double l_R_i = calc_l(from_x, from_y, MyStepper::Orientation::R);
  double l_L_f = calc_l(to_x, to_y, MyStepper::Orientation::L);
  double l_R_f = calc_l(to_x, to_y, MyStepper::Orientation::R);

  Serial.println("l_L_i: " + String(l_L_i));
  Serial.println("l_R_i: " + String(l_R_i));
  Serial.println("l_L_f: " + String(l_L_f));
  Serial.println("l_R_f: " + String(l_R_f));

  double delta_l_L = l_L_f - l_L_i;
  double delta_l_R = l_R_f - l_R_i;

  Serial.println("delta_l_L: " + String(delta_l_L));
  Serial.println("delta_l_R: " + String(delta_l_R));

  int stepsL = abs(ceil(2048 * delta_l_L / (r_cm * TWO_PI)));
  int stepsR = abs(ceil(2048 * delta_l_R / (r_cm * TWO_PI)));

  Serial.println("stepsL: " + String(stepsL));
  Serial.println("stepsR: " + String(stepsR));

  int lsign = delta_l_L > 0 ? MyStepper::Direction::FORWARD : MyStepper::Direction::BACKWARD;
  int rsign = delta_l_R > 0 ? MyStepper::Direction::BACKWARD : MyStepper::Direction::FORWARD;

  Serial.println("lsign: " + String(lsign));
  Serial.println("rsign: " + String(rsign));

  if (stepsL > stepsR) {
    ratio = (double)(stepsL - stepsR) / (double)stepsR;
    while (stepsR > 0){

      Serial.println("Steps L left: " + String(stepsL));
      Serial.println("Steps R left: " + String(stepsR));
      
      if (ratio >= 1){
        for (int i = 0; i < (int)ratio; i++) {
          L.step(lsign);
          stepsL--;

          Serial.println("Steps L left: " + String(stepsL));
          Serial.println("Steps R left: " + String(stepsR));

          delay(5);
        }
      }
      else {
        L.step(lsign);
        stepsL--;
        if ((double)random(100) / 100.0 <= ratio) {
          L.step(lsign);
          stepsL--;
        }
      }
      R.step(rsign);
      stepsR--;

      Serial.println("Steps L left: " + String(stepsL));
      Serial.println("Steps R left: " + String(stepsR));

      delay(5);
    }
    while (stepsL > 0){
      
      Serial.println("Steps L left: " + String(stepsL));
      Serial.println("Steps R left: " + String(stepsR));

      L.step(lsign);
      stepsL--;
      delay(5);
    }
  } else {
    ratio = (double)(stepsR - stepsL) / (double)stepsL;
    while (stepsL > 0){
      
      Serial.println("Steps L left: " + String(stepsL));
      Serial.println("Steps R left: " + String(stepsR));

      if (ratio >= 1.0){
        for (int i = 0; i < (int)ratio; i++) {
          R.step(rsign);
          stepsR--;

          Serial.println("Steps L left: " + String(stepsL));
          Serial.println("Steps R left: " + String(stepsR));

          delay(5);
        }
      }
      else {
        R.step(rsign);
        stepsR--;
        if ((double)random(100) / 100.0 <= ratio) {
          R.step(rsign);
        }
      }
      L.step(lsign);
      stepsL--;
      delay(5);
    }
    while (stepsR > 0){
      
      Serial.println("Steps L left: " + String(stepsL));
      Serial.println("Steps R left: " + String(stepsR));

      R.step(rsign);
      stepsR--;
      delay(5);
    }
  }
  
}

void draw(){
  int i;
  for (i = 0; i<=90; i+=2){
    servo.write(i);
    delay(10);
  }
}

void undraw(){
  int i;
  for (i = 90; i>=0; i-=2){
    servo.write(i);
    delay(10);
  }
}

void toAngle(int angle){
  int i = (int) servo.read();
  if (i < angle){
    while (i < angle){
      servo.write(i);
      delay(10);
      i += 2;
    }
  }
  else {
    while (i > angle){
      servo.write(i);
      delay(10);
      i -= 2;
    }
  }
}


void loop() {
  delay(5000);

  int x = 0;
  int y = 0;
  int xf;
  int yf;
  int dx;
  int dy;

  undraw();

  while (true) {
    dx = ceil(pow(random(4), analogRead(A0) / maxSoundLevel));
    dy = ceil(pow(random(3), analogRead(A0) / maxSoundLevel));
    
    if (random(1, 101) / 100.0 > 0.5) {
      dx *= -1;
    }
    if (random(1, 101) / 100.0 > 0.5) {
      dy *= -1;
    }
    if (random(1, 101) / 100.0 <= 0.5) {
      draw();
    }
    else {
      undraw();
    }
    
    if (y + dy > 0) {
      yf = min(maxY, y + dy);
    }
    else {
      yf = max(0, y + dy);
    }

    if (x + dx > 0) {
      xf = min(maxX, x + dx);
    }
    else {
      xf = max(-1 * maxX, x + dx);
    }
    moveTo(LS, RS, x, y, xf, yf);
    x = xf;
    y = yf;
    maxSoundLevel = max(maxSoundLevel, analogRead(A0));
  }
}
