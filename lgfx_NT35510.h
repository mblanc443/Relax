//----------------------------------------------------------------------
// https://github.com/lovyan03/LovyanGFX/blob/master/examples/HowToUse/2_user_setting/2_user_setting.ino
//=====================================================================
class LGFX : public lgfx::LGFX_Device{
  lgfx::Panel_NT35510  _panel_instance;
  lgfx::Bus_Parallel16 _bus_instance;       // Instance of 16-bit Parallel (ESP32)
  lgfx::Touch_XPT2046  _touch_instance;

public:LGFX(void){         
  // Configure bus control
  auto cfg = _bus_instance.config();       // Retrieve structure for bus configuration.
  
  // 16-bit parallel bus settings
  cfg.freq_write = 20000000;               // Transmission clock (maximum 20MHz, rounded to an integer division of 80MHz)

  cfg.pin_wr  = GPIO_NUM_7; 
  cfg.pin_rd  = GPIO_NUM_5;
  cfg.pin_rs  = GPIO_NUM_6; 
  
  cfg.pin_d0  = GPIO_NUM_16; 
  cfg.pin_d1  = GPIO_NUM_15;
  cfg.pin_d2  = GPIO_NUM_18;
  cfg.pin_d3  = GPIO_NUM_17;
  cfg.pin_d4  = GPIO_NUM_46;
  cfg.pin_d5  = GPIO_NUM_8; 
  cfg.pin_d6  = GPIO_NUM_9; 
  cfg.pin_d7  = GPIO_NUM_3; 
  cfg.pin_d8  = GPIO_NUM_47;
  cfg.pin_d9  = GPIO_NUM_48;  
  cfg.pin_d10 = GPIO_NUM_14;
  cfg.pin_d11 = GPIO_NUM_21;
  cfg.pin_d12 = GPIO_NUM_12;
  cfg.pin_d13 = GPIO_NUM_13;
  cfg.pin_d14 = GPIO_NUM_10; 
  cfg.pin_d15 = GPIO_NUM_11; 

  _bus_instance.config(cfg);              // Apply settings to the bus
  _panel_instance.setBus(&_bus_instance); // Set the bus to the panel
  
  { 
    // Configure display panel control
    auto cfg = _panel_instance.config(); // Retrieve structure for display panel configuration.
    cfg.pin_cs       =    4;             // Pin number connected to CS (-1 = disable)
    cfg.pin_rst      =   -1;             // Pin number connected to RST (-1 = disable)
    cfg.pin_busy   =     -1;             // Pin number connected to BUSY (-1 = disable)
    cfg.memory_width =  480;             // Maximum width supported by the driver IC
    cfg.memory_height=  800;             // Maximum height supported by the driver IC
    cfg.panel_width  =  480;             // Actual displayable width
    cfg.panel_height =  800;             // Actual displayable height
    cfg.offset_x     =    0;             // Offset value in the X direction
    cfg.offset_y     =    0;             // Offset value in the Y direction
    cfg.offset_rotation = 0;             // Offset value for rotation (0~7, 4~7 = upside down)
    cfg.dummy_read_pixel= 8;             // Number of dummy reads before reading a pixel
    cfg.dummy_read_bits = 1;             // Number of dummy reads before reading non-pixel data
    cfg.readable    = false;             // Set to true if data reading is possible
    cfg.invert      = false;             // Set to true if panel brightness is inverted
    cfg.rgb_order   =  true;             // Set to true if red and blue are swapped
    cfg.dlen_16bit  =  true;             // Set to true if the panel sends data in 16-bit units
    cfg.bus_shared  = false;             // Set to true if the bus is shared with an SD card
    //
    _panel_instance.config(cfg);
  }

  { 
    // Configure touch screen control (remove if not needed)
    auto cfg = _touch_instance.config();
    cfg.x_min      =  300;   // Min X value obtained from the touchscreen (raw value)
    cfg.x_max      = 3800;   // Max X value obtained from the touchscreen (raw value)
    cfg.y_min      =  180;   // Min Y value obtained from the touchscreen (raw value)
    cfg.y_max      = 3900;   // Max Y value obtained from the touchscreen (raw value)
    cfg.pin_int    =   20;   // Pin number connected to INT
    cfg.bus_shared = true;   // Set to true if the bus is shared with the display
    cfg.offset_rotation = 6; // Adjust if the display and touch orientations do not match (set value between 0~7)
    
    // If using SPI connection
    cfg.spi_host = SPI2_HOST; // Select SPI to use (HSPI_HOST or VSPI_HOST)
    cfg.freq     = 1000000;   // Set SPI clock, Max 2.5MHz, 8-bit (7-bit) mode

    cfg.pin_sclk = GPIO_NUM_42;  // SCLK (TP CLK)
    cfg.pin_mosi = GPIO_NUM_40;  // MOSI (TP DIN)
    cfg.pin_miso = GPIO_NUM_43;  // MISO (TP DOUT)
    cfg.pin_cs   = GPIO_NUM_41;  // CS   (TP CS)
    //cfg.pin_irq = GPIO_NUM_20;

    _touch_instance.config(cfg);
    _panel_instance.setTouch(&_touch_instance); // Set the touchscreen to the panel
  }
  //
  setPanel(&_panel_instance);   // Set the panel to use
}
};
LGFX tft;                       // Create an instance of the prepared class.
