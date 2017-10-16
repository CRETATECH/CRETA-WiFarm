/******* include labraly ********/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include"Json.h"

// port defult 80
ESP8266WebServer server(80);

/******** Init wifi *********/ 
const char* ssid = "Full house";            // wifi name
const char* password =  "12071985";         // password
 

void setup() 
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);              //Connect to the WiFi network
 
    while (WiFi.status() != WL_CONNECTED)    //Wait for connection
    { 
        delay(500);
        Serial.println("Waiting to connect...");
    }
 
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //Print the local IP
 
    server.on("/body", handleBody); //Associate the handler function to the path
 
    server.begin(); //Start the server
    Serial.println("Server listening");
 
}
 
void loop() 
{
    server.handleClient(); //Handling of incoming requests 
}
 
void handleBody() //Handler for the body path
{ 
      if (server.hasArg("plain")== false) //Check if body received
      { 
            server.send(200, "text/plain", "Body not received");
            return;
      }
         
      String  message; 
      message += server.arg("plain"); // get Json from client
              
      String data;
      String address;
      String function;
      
      pasr_json(message, data, address, function);  // parse Json
      
      server.send(200, "text/plain", message);      // send respone to client
      
      Serial.println(data);
      Serial.println(address);
      Serial.println(function);

      // code here for buffer 
      // put data, address, function to buffer
}






