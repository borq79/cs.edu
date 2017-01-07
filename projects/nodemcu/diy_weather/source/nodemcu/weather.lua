-- ====================================================================================================================
-- ====================================================================================================================
-- Copyright (c) 2017 Ryan Brock. All rights reserved.
-- Licensed under the MIT License. See LICENSE file in the project root for full license information.
--
-- This file is run when the NodeMCU boots.
-- This will connect to the WiFi and start up the mini HTTP server
-- ====================================================================================================================
-- ====================================================================================================================

function trim(s)
  return (s:gsub("^%s*(.-)%s*$", "%1"))
end

-- ====================================================================================================================
-- Load the Configuration
-- ====================================================================================================================
print("\n-----------------------------------")
print(" Starting Weather Station ...")
print("-----------------------------------")

if file.open("weather.config") then
  configLine = file.readline()

  while configLine do
    configName, configValue = configLine:match("([^,]+)\:([^,]+)")
    configName = trim(configName)
    configValue = trim(configValue)

    print(string.format("Setting [%s] to [%s]", configName, configValue))
    _G[configName] = configValue
    configLine = file.readline()
  end

  file.close()
end

-- ====================================================================================================================
-- Connect to the wifi
-- ====================================================================================================================
wifi.setmode(wifi.STATION)


station_cfg={}
station_cfg.ssid=ssid
station_cfg.pwd=wifipassword
station_cfg.save=true
station_cfg.auto=true
connectionStatus = wifi.sta.config(station_cfg)

if connectionStatus == true then
    print(string.format("Successfully Connected to [%s]", ssid))
    print("Connected IP Details: ", wifi.sta.getip())
else
    print(string.format("Failed to connect to [%s]", ssid))
end



-- ====================================================================================================================
-- Start the HTTP server
-- ====================================================================================================================
srv=net.createServer(net.TCP)
srv:listen(80,function(conn)
    conn:on("receive",function(conn,payload)

    -- Grab the first line of the HTTP request - ignore the rest
    path = payload:match("^GET (.+) HTTP/%d.%d\r\n.*")

    print(string.format("HTTP Request [%s]", path))

    if path == "/api/w" then
        respondApiWeather(conn)
    else
        conn:send(string.format("<html><head><title>Weather Station</title></head><body>Hello World</body></html>"))
    end
    
    --conn:send(string.format("<h1> Hello, NodeMcu.</h1>\r\nT=%d.%02d C\r\nT=%d.%02d F\r\nhumidity=%d.%03d%%\r\nADC=%d\r\n", TC, TCD, TF, TFD, H/1000, H%1000, adc.read(0)))
    --conn:send(string.format("<html><head><title>Weather Station</title></head><body>Hello World</body></html>"))
    end)
end)


function respondApiWeather(conn)
   print("API/Weather Response") 
   conn:send(string.format("{\"t\": 1483314140,\"tf\": 32,\"tc\": 0,\"p\": 23,\"h\": 32.5,\"i\": 345}"))
end