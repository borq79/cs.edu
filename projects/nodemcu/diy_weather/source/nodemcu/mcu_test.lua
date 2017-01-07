THINGSPEAK_API_KEY="YOURKEYHERE"

wifi.setmode(wifi.STATION)
wifi.sta.config("libraryessid","librarypassword")
print(wifi.sta.getip())

randomNumber = math.random(0,100)
print(string.format("Sending fake data for field1 [%s]", randomNumber))

conn=net.createConnection(net.TCP, false) 

conn:connect(80,"api.thingspeak.com")

conn:on("receive", function(conn, pl)   print("Got Response") end)

conn:on("connection", function(sck, c)
    local response = {}

    print("Connected")
    
    -- if you're sending back HTML over HTTP you'll want something like this instead
    -- local response = {"HTTP/1.0 200 OK\r\nServer: NodeMCU on ESP8266\r\nContent-Type: text/html\r\n\r\n"}

    response[#response + 1] = string.format("GET /update?api_key=%s&field1=%s HTTP/1.1\r\n", THINGSPEAK_API_KEY, randomNumber)
    response[#response + 1] = "Host: api.thingspeak.com\r\n"
    response[#response + 1] = "Content-Type: application/x-www-form-urlencoded\r\n"
    response[#response + 1] = "Connection: keep-alive\r\n"
    response[#response + 1] = "Accept: */*\r\n\r\n"

     -- sends and removes the first element from the 'response' table
    local function send(sk)
      if #response > 0 then
        print("sending ...")
        sk:send(table.remove(response, 1))
      else
        print("Closing")
        sk:close()
        response = nil
      end
    end

    -- triggers the send() function again once the first chunk of data was sent
    sck:on("sent", send)

    send(sck)
end)


    
