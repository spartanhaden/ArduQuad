//Quadrotor

#include <Servo.h>
#include <Wire.h>

Servo motor;
String line = "";
boolean endLine = false;

void setup(){
    //Wire.begin();
    Serial.begin(9600);
    //initMag();
    //initAccel();
    //initMotors();
    motor.attach(44);
}

void loop(){
    //MotorLoop();
    //AccelLoop();
    //MagnoLoop();
    if(endLine){
        if(line == "hello"){
            motor.writeMicroseconds((int)random(1000, 2000));
        }
        line = "";
        endLine = false;
    }
}

void serialEvent(){
    while(Serial.available()){
        char character = (char)Serial.read();
        if(character == '\n'){
            endLine = true;
            break;
        }
        line += character;
    }
}

byte readByte(int DEV_ADDR, byte REG){
    byte Byte;
    Wire.beginTransmission(DEV_ADDR);
    Wire.write(REG);
    Wire.endTransmission();
    Wire.beginTransmission(DEV_ADDR);
    Wire.requestFrom(DEV_ADDR, 1);
    while(Wire.available())
        Byte = Wire.read();
    return Byte;
}

void writeByte(byte DEV_ADDR, byte REG, byte VAL){
    Wire.beginTransmission(DEV_ADDR);
    Wire.write(REG);
    Wire.write(VAL);
    Wire.endTransmission();
}



