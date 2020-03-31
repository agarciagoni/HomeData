/*
  UDPSendReceive.pde:
  This sketch receives UDP message strings, prints them to the serial port
  and sends an "acknowledge" string back to the sender

  A Processing sketch is included at the end of file that can be used to send
  and received messages for testing with a computer.

  created 21 Aug 2010
  by Michael Margolis

  This code is in the public domain.

  adapted from Ethernet library examples
*/


#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#ifndef STASSID
#define STASSID "MIT"
#define STAPSK  ""
#endif

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char  ReplyBufferYes[] = "Received\r\n";       // a string to send back
char  ReplyBufferNo[] = " ";       // a string to send back
char ip[14];
char message[60];
WiFiUDP Udp;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
if (packetSize) {
//    Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n",
//                  packetSize,
//                  Udp.remoteIP().toString().c_str(), Udp.remotePort(),
//                  Udp.destinationIP().toString().c_str(), Udp.localPort(),
//                  ESP.getFreeHeap());

    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
//    packetBuffer[n] = 0;
//    Serial.println("Contents:");
//    Serial.println(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
//   ip=Udp.remoteIP();
//   Serial.printf(ip);
//  Serial.printf("A");
    //Serial.print("Sending");
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    if (Serial.available() >= 2){
     
      //Serial.print("Sending");
      //Udp.write(ReplyBufferYes);
      Serial.readStringUntil('\n').toCharArray(message,60); //Reading from arduino.
      Udp.write(message);
     // Udp.write(Serial.read());
     // Udp.write(Serial.read());
     // Udp.write(Serial.read());
    }
    else{
      Udp.write(ReplyBufferNo); 
    }
  //  Udp.write(Serial.read());
    Udp.endPacket();
  
  }
}

/*
  test (shell/netcat):
  --------------------
	  nc -u 192.168.esp.address 8888
*/
