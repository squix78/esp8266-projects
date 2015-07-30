-- your offset to UTC
local UTC_OFFSET = 2
-- Enter your city, check openweathermap.org
local CITY = "Zurich,ch"
-- Get an APP ID on openweathermap.org
local APPID = "YOUR_APP_ID"
-- Update interval in minutes
local INTERVAL = 10

function init_i2c_display()
     -- SDA and SCL can be assigned freely to available GPIOs
     sda = 5 -- GPIO14
     scl = 6 -- GPIO12
     sla = 0x3c
     i2c.setup(0, sda, scl, i2c.SLOW)
     disp = u8g.ssd1306_128x64_i2c(sla)
end

function prepare()
     disp:setFont(u8g.font_6x10)
     disp:setFontRefHeightExtendedText()
     disp:setDefaultForegroundColor()
     disp:setFontPosTop()
end

function updateWeather()
    print("Updating weather")
    local conn=net.createConnection(net.TCP, 0)
    conn:on("receive", function(conn, payload) 
        local lastUpdate = string.sub(payload,string.find(payload,"Date: ")+23,string.find(payload,"Date: ")+31)
        local hour = string.sub(lastUpdate, 0, 2) + UTC_OFFSET
        lastUpdate = hour..string.sub(lastUpdate, 3, 9)
        local payload = string.match(payload, "{.*}")
        print(payload)
        
        weather = nil
        weather = cjson.decode(payload)
        local icon = weather.weather[1].icon
        icon = string.gsub(icon, "n", "d")
        file.open(icon..".MONO", "r")
        xbm_data = file.read()
        file.close()
        
        payload = nil
        conn:close()
        conn = nil
        drawWeather(xbm_data, weather, lastUpdate)
    end )
    
    conn:connect(80,"95.85.58.189")
    conn:send("GET /data/2.5/weather?q="..CITY.."&units=metric&APPID="..APPID
      .." HTTP/1.1\r\n"
      .."Host: api.openweathermap.org\r\n"
      .."Connection: close\r\n"
      .."Accept: */*\r\n"
      .."User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n"
      .."\r\n")
    conn = nil

end
init_i2c_display()
prepare()
updateWeather()

function drawWeather(xbmData, weather, lastUpdate)
        disp:firstPage()
        repeat
                if weather ~= nil then
                    disp:drawXBM( 0, 0, 60, 60, xbm_data )
                    disp:setScale2x2()
                    disp:drawStr(35,5, math.floor(weather.main.temp).."C")
                    disp:drawStr(35,15, weather.main.humidity.."%")
                    disp:undoScale()
                    disp:drawStr(70,52, lastUpdate)
                end
        until disp:nextPage() == false
end


tmr.alarm(1, INTERVAL * 60000, 1, function() 
   ip = wifi.sta.getip()
   if ip=="0.0.0.0" or ip==nil then
      print("connecting to AP...") 
   else
      print("Loading weather...")
      updateWeather()
   end
end )

