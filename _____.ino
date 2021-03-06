/**
   BasicHTTPClient.ino

	Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

ESP8266WiFiMulti WiFiMulti;

/* ---config--- */
#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK "your-password"
#endif
// WIFI
const char *ssid = STASSID;
const char *password = STAPSK;
// SERVER
const String serverAddress = "";
const String tempPath = "/temperatureSense";
const String tempQueryParaName = "tActual";
const String swQueryParaName = "sNumber";
//DEVICE
const unsigned short sensorAnalogPin = A0;
const unsigned int bootInterval = 60 * 1000;
const unsigned int sampleTime = 10;
const unsigned int swPin = D3;
int vcc = 3090;
/*--------------*/

void setup()
{

	//pinMode(sensorAnalogPin, INPUT);
	pinMode(swPin, INPUT_PULLUP);

	Serial.begin(115200);
	// Serial.setDebugOutput(true);

	Serial.println();
	Serial.println();
	Serial.println();
	for (uint8_t t = 4; t > 0; t--)
	{
		Serial.printf("[SETUP] WAIT %d...\n", t);
		Serial.flush();
		delay(1000);
	}

	WiFi.mode(WIFI_STA);
	WiFiMulti.addAP("SSID", "PASSWORD");

	WiFi.begin(ssid, password);
	//  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
	//    Serial.println("Connection Failed! Rebooting...");
	//    delay(5000);
	//    ESP.restart();
	//  }

	ArduinoOTA.onStart([]()
					   {
						   String type;
						   if (ArduinoOTA.getCommand() == U_FLASH)
						   {
							   type = "sketch";
						   }
						   else
						   { // U_FS
							   type = "filesystem";
						   }

						   // NOTE: if updating FS this would be the place to unmount FS using FS.end()
						   Serial.println("Start updating " + type);
					   });
	ArduinoOTA.onEnd([]()
					 { Serial.println("\nEnd"); });
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
						  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
	ArduinoOTA.onError([](ota_error_t error)
					   {
						   Serial.printf("Error[%u]: ", error);
						   if (error == OTA_AUTH_ERROR)
						   {
							   Serial.println("Auth Failed");
						   }
						   else if (error == OTA_BEGIN_ERROR)
						   {
							   Serial.println("Begin Failed");
						   }
						   else if (error == OTA_CONNECT_ERROR)
						   {
							   Serial.println("Connect Failed");
						   }
						   else if (error == OTA_RECEIVE_ERROR)
						   {
							   Serial.println("Receive Failed");
						   }
						   else if (error == OTA_END_ERROR)
						   {
							   Serial.println("End Failed");
						   }
					   });
	ArduinoOTA.begin();
	Serial.println("Ready");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
}
float ans, tv;
int temp, swVal;
void loop()
{
	ans = 0;
	for (int i = 0; i < sampleTime; i++)
	{
		ans += analogRead(sensorAnalogPin); // ?????????????????????????????????????????????????????????
	}
	ans = ans / sampleTime;
	tv = ans * 3;
	temp = map(tv, 300, 1600, -30, 100);
	swVal = digitalRead(swPin);
	Serial.print(ans);
	Serial.print(",");
	Serial.print(tv);
	Serial.print(",");
	Serial.println(temp);
	// ????????????

	/* // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

	WiFiClient client;

	HTTPClient http;

	Serial.print("[HTTP] begin...\n");
	if (http.begin(client, "http://" + serverAddress + tempPath + "/?" + tempQueryParaName + "=" + String(temp) + "&" + swQueryParaName + "=" + String(swVal))) {  // HTTP


	  Serial.print("[HTTP] GET...\n");
	  // start connection and send HTTP header
	  int httpCode = http.GET();

	  // httpCode will be negative on error
	  if (httpCode > 0) {
		// HTTP header has been send and Server response header has been handled
		Serial.printf("[HTTP] GET... code: %d\n", httpCode);

		// file found at server
		if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
		  String payload = http.getString();
		  Serial.println(payload);
		}
	  } else {
		Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
	  }

	  http.end();
	} else {
	  Serial.printf("[HTTP} Unable to connect\n");
	}
  }
  ArduinoOTA.handle();
  //DEEP SLEEP?????????????????????
  Serial.println("DEEP SLEEP START!!");

  //1:??????????????????????????????????????????????????  2:??????????????????????????????????????????????????????
  ESP.deepSleep(10 * 1000 * 1000 , WAKE_RF_DEFAULT);

  //deepsleep???????????????????????????????????????
  */
	delay(100);

	//??????????????????????????????????????????
	// Serial.println("DEEP SLEEPing....");
}
