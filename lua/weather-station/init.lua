
print("Connecting to wifi...")
wifi.setmode(wifi.STATION)
wifi.sta.config("SSID","PWD")

tmr.alarm(0, 1000, 1, function() 
    print(".") 
    ip = wifi.sta.getip()
    if ( ( ip ~= nil ) and  ( ip ~= "0.0.0.0" ) )then
        print(ip)
        tmr.stop(0)
        sk=net.createConnection(net.TCP, 0)
        sk:dns("www.google.com",dnsIsWorking)
        sk = nil
    end
end )

function dnsIsWorking(conn,ip)
    print("Google IP: "..ip)
    collectgarbage("collect")
    dofile("weatherStation.lc")
end
