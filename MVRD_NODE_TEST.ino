#include <SoftwareSerial.h>
#include <XBee.h>

#include "MVRD_NODE_TEST.h"
#include <Servo.h> 
 
SoftwareSerial SoftSerial(7, 6); // RX, TX
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 



///////////////////////////////////
// SETUP
///////////////////////////////////
//uint8_t data;
char buf[500];
char *d1, *d2;
char ca, cb, cx, cy;

int val; // variable to read the value from the analog pin 

int servoPin = 2;
Servo servo; 
int servo_val;

int angle = 0;   // servo position in degrees 

void setup() {
  Serial.begin(9600);

  initXBee();
  servo.attach(servoPin); 
}
 
///////////////////////////////////
// LOOP
///////////////////////////////////
void loop() {
    node.readPacket();
   
    if (node.success == true) {

      servo.write(node.packet.r2);
      
      if (node.packet.a) { ca = 'A'; } else { ca = '-'; }
      if (node.packet.b) { cb = 'B'; } else { cb = '-'; }
      if (node.packet.x) { cx = 'X'; } else { cx = '-'; }
      if (node.packet.y) { cy = 'Y'; } else { cy = '-'; }
  
      if (node.packet.d_up) {
        d1 = "UP   ";
      } else if (node.packet.d_down) {
        d1 = "DOWN ";
      } else {
        d1 = "-----";
      }
  
      if (node.packet.d_left) {
        d2 = "LEFT ";
      } else if (node.packet.d_right) {
        d2 = "RIGHT";
      } else {
        d2 = "-----";
      }
      
      sprintf (buf, "Left X: %6d | Left Y: %6d | Right X: %6d | Right Y: %6d | L1:%d L2:%3d L3:%d | R1:%d R2:%3d R3:%d | %c%c%c%c | %s %s | ", 
      node.packet.leftX, node.packet.leftY, node.packet.rightX, node.packet.rightY, 
      node.packet.l1, node.packet.l2, node.packet.l3, 
      node.packet.r1, node.packet.r2, node.packet.r3, 
      ca, cb, cx, cy,
      d1, d2
      );
      Serial.println(buf);
    }
    
}

// Wrapper for recieving messages to the Node
void Node::readPacket() {

  node.success = false;
  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()) {
    // got something
          
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      xbee.getResponse().getRx16Response(node.rx);
      //option = rx16.getOption();
      // if (rx16.getDataLength() == 12) {}; ?? 

      // Joysticks - leftX (2), leftY (2), rightX (2), rightY (2)
      node.packet.leftX = (node.rx.getData(0) << 8) | node.rx.getData(1);
      node.packet.leftY = (node.rx.getData(2) << 8) | node.rx.getData(3);
      node.packet.rightX = (node.rx.getData(4) << 8) | node.rx.getData(5);
      node.packet.rightY = (node.rx.getData(6) << 8) | node.rx.getData(7);
      
      // Triggers - L2 R2
      node.packet.l2 = node.rx.getData(8);
      node.packet.r2 = node.rx.getData(9);
    
      // Buttons - toggle
      node.packet.a = (node.rx.getData(10) >> 7) & 1;
      node.packet.b = (node.rx.getData(10) >> 6) & 1;
      node.packet.x = (node.rx.getData(10) >> 5) & 1;
      node.packet.y = (node.rx.getData(10) >> 4) & 1;

      // Buttons - momentary
      node.packet.l1 = (node.rx.getData(10) >> 3) & 1;
      node.packet.r1 = (node.rx.getData(10) >> 2) & 1;
      node.packet.l3 = (node.rx.getData(10) >> 1) & 1;
      node.packet.r3 = (node.rx.getData(10)) & 1;

      // D Pad - momentary
      node.packet.d_up = node.rx.getData(11) >> 7  & 1;
      node.packet.d_down = node.rx.getData(11) >> 6  & 1;
      node.packet.d_left = node.rx.getData(11) >> 5  & 1;
      node.packet.d_right = node.rx.getData(11) >> 4  & 1;

      // 4 bits left! Lets figure out something cool to do with them. 

      node.success = true;
    }
  
  } else if (xbee.getResponse().isError()) {
    // TODO Serial.println("Error");
  } 
    
}

void initXBee() {
  SoftSerial.begin(9600);
  xbee.setSerial(SoftSerial);
}

