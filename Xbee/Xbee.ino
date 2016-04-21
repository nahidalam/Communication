void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);   
}

void loop()
{
  //Serial.println('H');
  //delay(1000);
  Serial.println('L');
  delay(1000);
  //Serial.println('R1');
  //delay(1000);
  
  if (Serial.available() > 0) {
                // read the incoming byte:
                //incomingByte = Serial.read();
                //if incomingByte='A' blink

           
                //Serial.println(incomingByte, DEC);
                
                digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
                delay(1000);              // wait for a second
                digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
                delay(1000);              // wait for a second
        }
}
