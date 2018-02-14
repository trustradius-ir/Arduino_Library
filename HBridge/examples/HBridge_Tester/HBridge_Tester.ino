/*
Example for working with H-Bridge
Write by Alireza Noubrakhsh
*/
#include <HBridge.h>

//Forward   6 // connect to forward pin of H-Bridge
//Backward  7  //connect to backward pin of H-Bridge
//hbOneWay or hbTwoWay

HBridge HB(6,7,hbTwoWay,true);

void setup() {
  Serial.begin(9600);
  HB.Initialize(80);  //Motor Start With PWM 80 And Upper
}


void loop() {
  Serial.println("Forward 100 Percent");
  HB.Forward();
  delay(2000);

  Serial.println("Stop 10 Percent Steps");
  HB.Stop(10);
  delay(2000);

  Serial.println("Forward 80 Percent");
  HB.Forward(80);
  delay(2000);
  
  Serial.println("Break With 100 Percent Reverse");
  HB.Break(); 
  delay(3000);
  
  Serial.println("Backward 50 Percent");
  HB.Backward(50); 
  delay(2000);
  
  Serial.println("Backward 100 Percent");
  HB.Backward(); 
  delay(2000);
  
  Serial.println("Break With 50 Percent Reverse");
  HB.Break(50); 
  delay(3000);
}











