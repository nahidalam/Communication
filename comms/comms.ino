#include "XBee.h"

#define LED RED_LED

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle
ZBRxResponse rx = ZBRxResponse();
ZBRxIoSampleResponse ioSample = ZBRxIoSampleResponse();
//XBeeAddress64 Broadcast = XBeeAddress64(0x00000000, 0x0000ffff);
XBeeAddress64 Destination = XBeeAddress64(0x0013A200, 0x40F743A5);  //coordinator address '\x00\x13\xA2\x00\x40\xF7\x43\xA5'
char message[] = "TI Row Controller\r";

void setup() { 
  Serial.begin(9600);
  Serial3.begin(9600);
  xbee.setSerial(Serial3);
  //Serial.println("starting up yo!");
  pinMode(LED, OUTPUT);    
}

void loop()
{
      xbee.readPacket();
    if (xbee.getResponse().isAvailable()) {
      
          digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
          delay(1000);               // wait for a second
          digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
          delay(1000);               // wait for a second
          Serial.print("Frame Type is ");
          Serial.println(xbee.getResponse().getApiId(), HEX);
          if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
            xbee.getResponse().getZBRxResponse(rx);
            Serial.print("Got an rx packet from: ");
            XBeeAddress64 senderLongAddress = rx.getRemoteAddress64();
            print32Bits(senderLongAddress.getMsb());
            Serial.print(" ");
            print32Bits(senderLongAddress.getLsb());
            Serial.println(" ");
            Serial.print("packet length is ");
            Serial.println(rx.getPacketLength(), DEC);
      
        Serial.print("data payload length is ");
        Serial.println(rx.getDataLength(),DEC);
      
        Serial.println("Received Data: ");
        /*for (int i = 0; i < rx.getDataLength(); i++) {
          print8Bits(rx.getData()[i]);
          Serial.print(' ');
        }*/
        //Serial.println();
        //representation in string
        
        for (int i= 0; i < rx.getDataLength(); i++){
          //Serial.write(' ');
          if (iscntrl(rx.getData()[i]))
            Serial.write(' ');
          else
            Serial.write(rx.getData()[i]);
          //Serial.write(' ');
        }
        Serial.println();
            
          }
          
          //send from row controller
          Serial.println("Sending to the Coordinator");
          sendXbee(message);
          delay(30000);
  }
}

void sendXbee(const char* command){
  Serial.println("inside send TX");
  ZBTxRequest zbtx = ZBTxRequest(Destination, (uint8_t *)command, strlen(command));
  xbee.send(zbtx);
  Serial.println("Done TX");
}

/*void loop() {
    xbee.readPacket();
    if (xbee.getResponse().isAvailable()) {
      //Serial.print("Frame Type is ");
      //Serial.println(xbee.getResponse().getApiId(), HEX);
    
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(rx);
        //Serial.print("Got an rx packet from: ");
        XBeeAddress64 senderLongAddress = rx.getRemoteAddress64();
        print32Bits(senderLongAddress.getMsb());
        //Serial.print(" ");
        print32Bits(senderLongAddress.getLsb());
      

        if (rx.getOption() & ZB_PACKET_ACKNOWLEDGED)
          Serial.println("packet acknowledged");
        if (rx.getOption() & ZB_BROADCAST_PACKET)
          Serial.println("broadcast Packet");
        
        Serial.print("checksum is ");
        Serial.println(rx.getChecksum(), HEX);
    
        Serial.print("packet length is ");
        Serial.print(rx.getPacketLength(), DEC);
      
        Serial.print(", data payload length is ");
        Serial.println(rx.getDataLength(),DEC);
      
        Serial.println("Received Data: ");
        for (int i = 0; i < rx.getDataLength(); i++) {
          print8Bits(rx.getData()[i]);
          Serial.print(' ');
        }
      
        // and an ascii representation for those of us that send text through the XBee
        Serial.println();
        for (int i= 0; i < rx.getDataLength(); i++){
          Serial.write(' ');
          if (iscntrl(rx.getData()[i]))
            Serial.write(' ');
          else
            Serial.write(rx.getData()[i]);
          Serial.write(' ');
        }
        Serial.println();
        
        handleXbeeRxMessage(rx.getData(), rx.getDataLength());
        Serial.println();
      }
      

      else {
        Serial.print("Got frame id: ");
        Serial.println(xbee.getResponse().getApiId(), HEX);
      }
    }
    else if (xbee.getResponse().isError()) {
      // some kind of error happened, I put the stars in so
      // it could easily be found
      Serial.print("************************************* error code:");
      Serial.println(xbee.getResponse().getErrorCode(),DEC);
    }
    else {
      // I hate else statements that don't have some kind
      // ending.  This is where you handle other things
    }
}

void handleXbeeRxMessage(uint8_t *data, uint8_t length){
  // this is just a stub to show how to get the data,
  // and is where you put your code to do something with
  // it.
  for (int i = 0; i < length; i++){
//    Serial.print(data[i]);
  }
//  Serial.println();
}*/

void showFrameData(){
  Serial.println("Incoming frame data:");
  for (int i = 0; i < xbee.getResponse().getFrameDataLength(); i++) {
    print8Bits(xbee.getResponse().getFrameData()[i]);
    Serial.print(' ');
  }
  Serial.println();
  for (int i= 0; i < xbee.getResponse().getFrameDataLength(); i++){
    Serial.write(' ');
    if (iscntrl(xbee.getResponse().getFrameData()[i]))
      Serial.write(' ');
    else
      Serial.write(xbee.getResponse().getFrameData()[i]);
    Serial.write(' ');
  }
  Serial.println(); 
}

// these routines are just to print the data with
// leading zeros and allow formatting such that it
// will be easy to read.
void print32Bits(uint32_t dw){
  print16Bits(dw >> 16);
  print16Bits(dw & 0xFFFF);
}

void print16Bits(uint16_t w){
  print8Bits(w >> 8);
  print8Bits(w & 0x00FF);
}

void print8Bits(byte c){
  uint8_t nibble = (c >> 4);
  if (nibble <= 9)
    Serial.write(nibble + 0x30);
  else
    Serial.write(nibble + 0x37);
      
  nibble = (uint8_t) (c & 0x0F);
  if (nibble <= 9)
    Serial.write(nibble + 0x30);
  else
    Serial.write(nibble + 0x37);
}
