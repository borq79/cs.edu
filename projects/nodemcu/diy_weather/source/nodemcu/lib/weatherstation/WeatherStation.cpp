// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
#include "WeatherStation.h"

WeatherStation::WeatherStation(String configurationFilePath) {
  this->configurationFilePath = configurationFilePath;
}

void WeatherStation::begin() {
  Serial.println("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~");
  Serial.println("                                Weather Station ");
  Serial.println("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~");
  Serial.println("Starting ...");

  this->loadConfigurationFile();
//  this->connectToWifi();
  this->initializeBME280();
}

void WeatherStation::applicationLoop() {
  this->readBME280();
  delay(10000);
}

void WeatherStation::connectToWifi() {
  Serial.print(String("Connecting to " + this->wifiSSID));

  WiFi.begin(this->wifiSSID.c_str(), this->wifiPassword.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println(String("WiFi connected. IP: " + WiFi.localIP()));
}

void WeatherStation::initializeBME280() {
  // Communication Mode Intialization
  bme280.settings.commInterface = BME_COMM_MODE;

  if (BME_COMM_MODE == SPI_MODE) {
	  bme280.settings.chipSelectPin = BME_SPI_CHIP_SELECT_PIN;
  } else {
    bme280.settings.I2CAddress = BME_ADDRESS;
  }

  bme280.settings.runMode         = 3;
  bme280.settings.tStandby        = 0;
  bme280.settings.filter          = 0;
  bme280.settings.tempOverSample  = 1;
  bme280.settings.pressOverSample = 1;
  bme280.settings.humidOverSample = 1;

  // Give the BME280 enough time to start up (takes 2ms)
  delay(10);
  bme280.begin();
}

void WeatherStation::loadConfigurationFile() {
  Serial.println(String("Loading Configuration File " + this->configurationFilePath + " ..."));

  SPIFFS.begin();
  File configurationFile = SPIFFS.open(this->configurationFilePath, "r");
  if (!configurationFile) {
    Serial.println(String("Failed to open file " + this->configurationFilePath));
  } else {
    while (configurationFile.available()) {
      String configLine = configurationFile.readStringUntil('\n');
      //Serial.println(String("Parsing Configuration Line [" + configLine + "]"));

      int splitPoint = configLine.indexOf(":");
      String key = configLine.substring(0, splitPoint); key.trim();
      String val = configLine.substring(splitPoint + 1); val.trim();

      // Serial.println(String("Key = " + key));
      // Serial.println(String("Val = " + val));

      if (key.equals("ssid")) { this->wifiSSID = val; }
      else if (key.equals("wifipassword")) { this->wifiPassword = val; }
   }

   // close the file
   configurationFile.close();
  }

  SPIFFS.end();
}

void WeatherStation::readBME280() {
  Serial.print("Temperature: ");
	Serial.print(bme280.readTempC(), 2);
	Serial.println(" degrees C");

	Serial.print("Temperature: ");
	Serial.print(bme280.readTempF(), 2);
	Serial.println(" degrees F");

	Serial.print("Pressure: ");
	Serial.print(bme280.readFloatPressure(), 2);
	Serial.println(" Pa");

	Serial.print("Altitude: ");
	Serial.print(bme280.readFloatAltitudeMeters(), 2);
	Serial.println("m");

	Serial.print("Altitude: ");
	Serial.print(bme280.readFloatAltitudeFeet(), 2);
	Serial.println("ft");

	Serial.print("%RH: ");
	Serial.print(bme280.readFloatHumidity(), 2);
	Serial.println(" %");

	Serial.println();
}
