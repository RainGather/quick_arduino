wifi.setmode(wifi.STATION)
station_cfg={}
station_cfg.ssid="hzaxiaomi"
station_cfg.pwd="88888888"
wifi.sta.config(station_cfg)
print(wifi.sta.getip())
gpio.mode(4, gpio.OUTPUT)
gpio.mode(5, gpio.INPUT) -- D1 pin


function cset(key, value)
    http.get("http://iot_test.yiyai.net/api/set?api_id=59c89d4b48a91f00075376eb&token=SOFLs3uXQI&key="..key.."&value="..value, nil, function(code, data)
        if (code < 0) then
            print("HTTP request failed")
        else
            print(code, data)
        end
    end)
end


uart.on(data, "\r", 
    function(data)
        cset("uart", data)
    end, 0)


function at()
    if (gpio.read(5) == gpio.LOW) then
        tmr.stop(0)
    end
    uart.setup(0, 38400, 8, uart.PARITY_NONE, uart.STOPBITS_1, 1)
    uart.write(0, 'AT\r\n')
    uart.write(0, 'AT')
end


tmr.alarm(0, 1000, tmr.ALARM_AUTO, at)
