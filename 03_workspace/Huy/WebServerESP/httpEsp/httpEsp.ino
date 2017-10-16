#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <os_type.h>

#define BUFFERSIZE_MAX 50

String buffer_rec;
const char* ssid = "Happy.Home";// tên wifi mà bạn muốn connect
const char* password = "chuDviethoa";//pass wifi
 
WiFiServer server(80);// Port 80
 
void setup() {
Serial.begin(115200);
delay(10);
 
// Kết nỗi với wifi
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
 
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
 
// Bắt đầu sever
server.begin();
Serial.println("Server started");
 
// In địa chỉ IP 
Serial.print("Use this URL to connect: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/");
 
}
 
void loop() {
// Kiểm tra xem đã connect chưa
  WiFiClient client1 = server.available();
  if (!client1) {
    return;
  }
   
  // Đọc data
  Serial.println("new client");
  while(!client1.available()){
  delay(1);
  }
  while (client1.available()) {
    String buffer_rec_temp = esp_read(client1);
    if (buffer_rec_temp != "") buffer_rec = buffer_rec_temp;
  }
  //Serial.println(request);
  delay(10);
  Serial.println(buffer_rec);
  client1.flush();
  esp_write(buffer_rec, client1);
  delay(1);
  Serial.println("Client disonnected");
  //Serial.println("");
}

void esp_write(String buffer_write, WiFiClient client) {
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  s += buffer_write;
  s += "</html>\n";
  client.print(s);
}

String esp_read(WiFiClient client) {
  char charRecv;
  char BoT = '%';
  char EoT = 'D';
  int sizeofBuffer=0, count = 0, check = 0, check_str = 0;
  String bufferRecv="";
  String bufferSendServer="";
  String bufferSendHard="";
  String buffer3test="";
  char charGet1 = ' ', charGet2 = ' ', charGet3 = ' ';
  char charGet;
  charGet = client.read();
  if (charGet == BoT) {
    charGet2 = charGet;
    charGet = client.read();
    charGet1 = charGet;
    while((charGet != EoT) && (sizeofBuffer <= BUFFERSIZE_MAX)) {
      charGet = client.read();
      //Serial.write(charGet);
      if ( (charGet >= ' ') && (charGet <= '~' ) && (charGet != EoT) ){
        delay(5);
        charGet3 = charGet2;
        charGet2 = charGet1;
        charGet1 = charGet;
        buffer3test = buffer3test + charGet1 + charGet2 + charGet3;
        //Serial.println(buffer3test);
        if (buffer3test == "B7%") {
          charGet1 = '{';
          check = 1;
        }
        else if (buffer3test == "D7%") {
          charGet1 = '}';
          check = 1;
        }
        else if (buffer3test == "22%") {
          charGet1 = '"';
          check = 1;
        }
        if (check == 1) {
          if (count == 1) check = 0; 
          count++;
        }
        else {
          count = 0;
          bufferRecv += charGet3;
          sizeofBuffer++;
          buffer3test = "";
        }
      }
    }
    if ((charGet2 == '%') && (charGet1 == '7')) bufferSendHard = bufferRecv + '}';
    sizeofBuffer = 0;
    bufferRecv="";
  }
  return bufferSendHard;
}

