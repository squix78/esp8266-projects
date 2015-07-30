
-- setup I2c and connect display
function init_i2c_display()
     -- SDA and SCL can be assigned freely to available GPIOs
     sda = 5 -- GPIO14
     scl = 6 -- GPIO12 
     sla = 0x3c
     i2c.setup(0, sda, scl, i2c.SLOW)
     disp = u8g.ssd1306_128x64_i2c(sla)
end

function xbm_picture()
     disp:setFont(u8g.font_6x10)
     disp:drawStr( 0, 62, "Rainy   8C")
     disp:drawXBM( 0, -5, 60, 60, xbm_data )
end

function bitmap_test(delay)
     file.open("clouds.xbm", "r")
     xbm_data = file.read()
     file.close()

      disp:firstPage()
      repeat
           xbm_picture()
      until disp:nextPage() == false

      tmr.wdclr()
end

init_i2c_display()
bitmap_test()