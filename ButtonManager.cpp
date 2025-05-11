#include "ButtonManager.h"
#include <Arduino.h>

ButtonManager::ButtonManager(LGFX* display) : _tft(display) {
    // Calculate button positions
    _btnResetX = _btnWidth + 1;
    _btnResetY = _btnStartY;
    _btnZoomInX = _btnResetX + 1;
    _btnZoomInY = _btnStartY;
    _btnZoomOutX = _btnZoomInX + 1;
    _btnZoomOutY = _btnStartY;
}

void ButtonManager::initButtons() {
    // Initial drawing of buttons
    drawButtons(false);
}

ButtonManager::ButtonType ButtonManager::checkTouch(uint16_t x, uint16_t y) {
    if (y < _btnStartY || y > _btnStartY + _btnHeight) {
        return NONE;
    }

    if (x > _btnStartX && x < (_btnStartX + _btnWidth)) {
        return START_STOP;
    }
    else if (x > _btnResetX && x < (_btnResetX + _btnWidth)) {
        return RESET;
    }
    else if (x > _btnZoomInX && x < (_btnZoomInX + _btnWidth)) {
        return ZOOM_IN;
    }
    else if (x > _btnZoomOutX && x < (_btnZoomOutX + _btnWidth)) {
        return ZOOM_OUT;
    }

    return NONE;
}

void ButtonManager::drawButtons(bool startPressed) {
    int btnY = _btnStartY;
    int lblOffsetX = 20;
    int lblOffsetY = 15;
    
    _tft->setFont(&fonts::FreeSerif18pt7b);
    
    // Start/Stop Button
    _tft->fillRect(_btnStartX, btnY, _btnWidth, _btnHeight, 
                  startPressed ? TFT_RED : _startColor);
    _tft->setTextColor(TFT_BLACK);
    _tft->setTextSize(1);
    _tft->setCursor(_btnStartX + lblOffsetX * 2, btnY + lblOffsetY);
    _tft->print(startPressed ? "Stop" : "Start");
    
    // Reset Button
    _tft->fillRect(_btnResetX, btnY, _btnWidth, _btnHeight, _resetColor);
    _tft->setCursor(_btnResetX + lblOffsetX * 2, btnY + lblOffsetY);
    _tft->print("Reset");
    
    // Zoom In
    _tft->fillRect(_btnZoomInX, btnY, _btnWidth, _btnHeight, _zoomInColor);
    _tft->setCursor(_btnZoomInX + lblOffsetX, btnY + lblOffsetY);
    _tft->print("Zoom In");
    
    // Zoom Out
    _tft->fillRect(_btnZoomOutX, btnY, _btnWidth, _btnHeight, _zoomOutColor);
    _tft->setTextSize(0);
    _tft->setCursor(_btnZoomOutX + lblOffsetX, btnY + lblOffsetY);
    _tft->print("Zoom Out");
}

void ButtonManager::setButtonColors(uint16_t startColor, uint16_t resetColor, 
                                   uint16_t zoomInColor, uint16_t zoomOutColor) {
    _startColor = startColor;
    _resetColor = resetColor;
    _zoomInColor = zoomInColor;
    _zoomOutColor = zoomOutColor;
}
