/*sample XBee Read Write Interface*/



#include "XBee.h"

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle
ZBRxResponse rx = ZBRxResponse();
ZBRxIoSampleResponse ioSample = ZBRxIoSampleResponse();
XBeeAddress64 Destination = XBeeAddress64(0x0013A200, 0x40F743A5);  //coordinator address '\x00\x13\xA2\x00\x40\xF7\x43\xA5'
char message[] = "TI Row Controller\r";

void setup() { 
  Serial.begin(9600);
  Serial3.begin(115200);
  xbee.setSerial(Serial3);  
}


void loop() {
    receiveXbee();

    Serial.println("Sending to the Coordinator");
    sendXbee(message);
    delay(3000);
}
void sendXbee(const char* command){
  Serial.println("inside send TX");
  ZBTxRequest zbtx = ZBTxRequest(Destination, (uint8_t *)command, strlen(command));
  xbee.send(zbtx);
  Serial.println("Done TX");
}


void receiveXbee ()
{
    xbee.readPacket();
    if (xbee.getResponse().isAvailable()) {
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
        for (int i= 0; i < rx.getDataLength(); i++){  
          if (iscntrl(rx.getData()[i]))
            Serial.write(' ');
          else
            Serial.write(rx.getData()[i]);
        }
        Serial.println();            
   }
          
  }
}
