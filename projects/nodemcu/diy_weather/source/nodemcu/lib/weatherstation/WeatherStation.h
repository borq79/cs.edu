#ifndef _WEATHERSTATION_H_
#define _WEATHERSTATION_H_
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <SparkFunBME280.h>

#define BME_COMM_MODE           I2C_MODE
#define BME_ADDRESS             0x76
#define BME_SPI_CHIP_SELECT_PIN 10

// ====================================================================================================================
// ====================================================================================================================
// Copyright (c) 2017 Ryan Brock. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root for full license information.
// ====================================================================================================================
// ====================================================================================================================
class WeatherStation
{
  public:
    WeatherStation(String configurationFilePath);
    void begin();
    void applicationLoop();

  private:
    String configurationFilePath;
    String wifiPassword;
    String wifiSSID;
    String thingSpeakAPIKey;

    BME280 bme280;

    void loadConfigurationFile();
    void connectToWifi();
    void initializeBME280();
    // void sampleMeasurements();
    void readBME280();
    // void getLightIntensity();
    // void sendSamplesToThingSpeak();
    // void checkForClientConnections();
    // void updateConfigurationFile();
};


#endif
