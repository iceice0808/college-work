/*
  ESP32 TCS3200 Color sensor
  http:://www.electronicwings.com
*/

#include <WiFi.h>
#include <WebServer.h>
#include "html.h"

WebServer server(80);

char ssid[] = "TP-Link_6BDA";
char password[] = "06093068";
/*Enter Your Password*/
 /*Enter Your Password*/
//char ssid[] = "STP-Link_6BDA";
//char password[] = "06093068";

#define S2 19          /*Define S2 Pin Number of ESP32*/
#define S3 21         /*Define S3 Pin Number of ESP32*/
#define sensorOut 18  /*Define Sensor Output Pin Number of ESP32*/

/*Enter the Minimum and Maximum Values which getting from Calibration Code*/
int R_Min = 12;  /*Red minimum value*/
int R_Max = 118; /*Red maximum value*/
int G_Min = 12;  /*Green minimum value*/
int G_Max = 122; /*Green maximum value*/
int B_Min = 3;  /*Blue minimum value*/
int B_Max = 36; /*Blue maximum value*/

/*Define int variables*/
int Red = 0;
int Green = 0;
int Blue = 0;

int redValue;
int greenValue;
int blueValue;
int Frequency;

void MainPage() {
  String _html_page = html_page;              /*Read The HTML Page*/
  server.send(200, "text/html", _html_page);  /*Send the code to the web server*/
}

void Colors() {
  String data = "[\""+String(redValue)+"\",\""+String(greenValue)+"\",\""+String(blueValue)+"\"]";
  server.send(200, "text/plane", data);
}

void setup() {
   pinMode(S2, OUTPUT);        /*Define S2 Pin as a OUTPUT*/
  pinMode(S3, OUTPUT);        /*Define S3 Pin as a OUTPUT*/
  pinMode(sensorOut, INPUT);  /*Define Sensor Output Pin as a INPUT*/
  Serial.begin(115200);       /*Set the baudrate to 115200*/
  
  pinMode(sensorOut, INPUT);  /*Define sensorOut as a input Pin*/
  
  WiFi.mode(WIFI_STA);        /*Set the WiFi in STA Mode*/
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  delay(1000);                /*Wait for 1000mS*/
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  } /*Wait while connecting to WiFi*/
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Your Local IP address is: ");
  Serial.println(WiFi.localIP());       /*Print the Local IP*/

  server.on("/", MainPage);             /*Display the Web/HTML Page*/
  server.on("/readColors", Colors);     /*Display the updated Distance value(CM and INCH)*/
  server.begin();                       /*Start Server*/
  delay(1000);                          /*Wait for 1000mS*/
}

void loop() {
  Red = getRed();
  redValue = map(Red, R_Min,R_Max,255,0); /*Map the Red Color Value*/
  delay(200);
  
  Green = getGreen();
  greenValue = map(Green, G_Min,G_Max,255,0); /*Map the Green Color Value*/
  delay(200);
  
  Blue = getBlue();
  blueValue = map(Blue, B_Min,B_Max,255,0);   /*Map the Blue Color Value*/
  delay(200);

  Serial.print("Red = ");
  Serial.print(redValue);   /*Print Red Color Value on Serial Monitor*/
  Serial.print("    ");
  Serial.print("Green = ");
  Serial.print(greenValue); /*Print Green Color Value on Serial Monitor*/
  Serial.print("    ");
  Serial.print("Blue = ");
  Serial.println(blueValue);/*Print Blue Color Value on Serial Monitor*/
  server.handleClient();
  delay(1000);              /*wait a second*/
}

int getRed() {
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  Frequency = pulseIn(sensorOut, LOW); /*Get the Red Color Frequency*/
  return Frequency;
}

int getGreen() {
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  Frequency = pulseIn(sensorOut, LOW); /*Get the Green Color Frequency*/
  return Frequency;
}

int getBlue() {
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  Frequency = pulseIn(sensorOut, LOW); /*Get the Blue Color Frequency*/
  return Frequency;
}
