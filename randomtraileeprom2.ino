

#include <WebServer.h>
#include <SPI.h>
#include <Ethernet.h>
#include "Streaming.h"
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include <TextFinder.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {192,168,1,129};
byte subnet[] = {255,255,255,0};
byte gateway[] = {192,168,0,254};
byte dnsserver[] = {192,168,1,1};
int incomingByte = 0;
const byte ID = 0x92;



WebServer webserver("", 80);

P(htmlHead) =
 "<!doctype html><html>"
 "<head><meta charset=\"utf-8\">"
 "<title>LAB</title>"
 "</head><body>" ;
 
P(htmlFoot) = "</body></html>";

P(homePage) =
  "<form method=\"post\" action= \"SAVECONF\">"
  "IP：<input type=\"text\" name=\"LocalIPA\" maxlength=\"3\" size=\"3\" value= \"\">."
  "<input type=\"text\" name=\"LocalIPB\" maxlength=\"3\" size=\"3\" value= \"\">."
  "<input type=\"text\" name=\"LocalIPC\" maxlength=\"3\" size=\"3\" value= \"\">."
  "<input type=\"text\" name=\"LocalIPD\" maxlength=\"3\" size=\"3\" value= \"\"><br><br>"
"<br><input type=\"submit\" name=\"send\" value=\"submit and restart\">"
  "</form>";

void defaultCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  server.httpSuccess();
  
  if (type != WebServer::HEAD)
  {
    server.printP(htmlHead);    
    server.printP(homePage);
    server.printP(htmlFoot);
  }
}
void postCmd(WebServer &server, WebServer::ConnectionType type,char *, bool)
{
  char name[16], value[16];
  server.httpSuccess();

  if (type == WebServer::POST)
  {
    server.printP(htmlHead);

    while (server.readPOSTparam(name, 16, value, 16)){

       if(strcmp(name,"LocalIPA")==0){
         EEPROM.write(7,atoi(value));}
         else if(strcmp(name,"LocalIPB")==0){
         EEPROM.write(8,atoi(value));}      
         else if(strcmp(name,"LocalIPC")==0){
         EEPROM.write(9,atoi(value));}
         else if(strcmp(name,"LocalIPD")==0){
         EEPROM.write(10,atoi(value));}
         else if(strcmp(name,"send")==0){
         server << "DONE";}else{server << "error!";}
            
    }
        
         EEPROM.write(0, 0x92);
         server.printP(htmlFoot);
         asm volatile ("  jmp 0");  
                  
  }
}

void setup() {
    Ethernet.begin(mac, ip, dnsserver, gateway, subnet);
    webserver.begin();
    webserver.setDefaultCommand(&defaultCmd);
    webserver.addCommand("SAVECONF", &postCmd);
    Serial.begin(9600);
}

void loop() {
int idcheck = EEPROM.read(0);
  webserver.processConnection();
  byte newip[4];
 if (idcheck == ID){

    for (int i = 0; i < 4; i++){
      newip[i] = EEPROM.read(i+7);
      Serial.print(newip[i]);
    }

}


delay(5000);
}
