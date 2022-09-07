#include <TimerOne.h>
unsigned long t;
float f=0;// 10f = 15 speed
int speed = 20;//1.5 speed = 1 frequency

void setup(){
    pinMode(11, OUTPUT);
    Serial.begin(9600);
    Timer1.initialize(t);
    Timer1.pwm(11, 512);
}

void loop() { //175hz = 260km/h
    Timer1.setPeriod(t);
    Timer1.pwm(11, 512);
    //f=1000000/t; // calculate the frequency 
    //f = (speed - 2) * 0.6666666; // Adjust the speed such that I actually lines up
    speedConv();
    t=1000000/f;
    Serial.println(speed);
    speed++;
    delay(200);
}

void speedConv(){
    if (speed <= 70){
        f = (speed - 2) * 0.6666666;
    }
    else if (speed <=140){
        f = (speed - 1) * 0.6666666;
    }
    else{
        f = speed * 0.6666666;
    }
}