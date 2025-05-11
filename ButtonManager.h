#pragma once
#include <LovyanGFX.hpp>

class ButtonManager {
public:
    enum ButtonType {
        NONE,
        START_STOP,
        RESET,
        ZOOM_IN,
        ZOOM_OUT
    };

    ButtonManager(LGFX* display);
    
    void initButtons();
    ButtonType checkTouch(uint16_t x, uint16_t y);
    void drawButtons(bool startPressed);
    void setButtonColors(uint16_t startColor, uint16_t resetColor, 
                        uint16_t zoomInColor, uint16_t zoomOutColor);

private:
    LGFX* _tft;
    
    // Button dimensions and positions
    int _btnWidth = 200;
    int _btnHeight = 80;
    int _btnStartX = 0, _btnStartY = 400;
    int _btnResetX, _btnResetY;
    int _btnZoomInX, _btnZoomInY;
    int _btnZoomOutX, _btnZoomOutY;
    
    // Button colors
    uint16_t _startColor = TFT_GREEN;
    uint16_t _resetColor = TFT_SKYBLUE;
    uint16_t _zoomInColor = TFT_YELLOW;
    uint16_t _zoomOutColor = TFT_PINK;
};
