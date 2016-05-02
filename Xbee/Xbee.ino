
int incomingByte = 0;   // for incoming serial data
#define LED RED_LED

void setup()
{
  Serial3.begin(9600);
  //pinMode(13, OUTPUT); 
  pinMode(LED, OUTPUT);    
}

void loop()
{
  //Serial3.println('H');
  //delay(1000);
  //Serial3.println('L');
  Serial3.println("Sending command to the XBee");
  delay(1000);
  Serial3.println("I am R1");
  delay(1000);
  
  if (Serial3.available() > 0) {
                // read the incoming byte:
                Serial3.println("got ack");
                delay(1000);
                incomingByte = Serial3.read();
                Serial3.println("read done");
                delay(1000);

                // say what you got:
                //Serial.print("I received: ");
                //Serial.println(incomingByte, DEC);
                Serial3.println("I got");
                delay(1000);
                Serial3.println(incomingByte);
                delay(1000);
                //digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
                //delay(1000);               // wait for a second
                //digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
                //delay(1000);               // wait for a second
        }
  
}
