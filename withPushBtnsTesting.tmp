#pragma GCC optimize ("-O3")  //"-0fast" is too risky for FP calculatoins on ESP32
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <Arduino.h>
#include "lgfx_NT35510.h"
#include <XPT2046_Touchscreen.h> 
#include "ButtonManager.h"

// Graph variables
#define DATA_POINTS 60
int graphData[DATA_POINTS];
int currentDataIndex = 0;
unsigned long lastTouchTime = 0;
const unsigned long touchDebounce = 200;

// Zoom and display variables
float zoomFactor = 1.0;
int visiblePoints = DATA_POINTS;
int scrollOffset = 0;

// Button manager instance
ButtonManager* buttonManager;
bool btnStartPressed = false;

// Graph area definitions
int graphTop = 60;      
int graphBottom = 389;
int graphLeft = 20;     
int graphRight = 780;
int titleHeight = 40;

// SETUP
void setup() {
  Serial.begin(9600);
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  
  // Initialize button manager
  buttonManager = new ButtonManager(&tft);
  buttonManager->initButtons();
  
  DisplayTitle();
  drawGrid(graphTop, graphBottom, graphLeft, graphRight);
  preloadGraphData();
}

// LOOP
void loop() {
  updateGraph(graphTop, graphBottom, graphLeft, graphRight);
  handleTouch();
}

// Updated handleTouch() and drawButtons() moved into ext class
void handleTouch() {
  // debounce touch
  if (millis() - lastTouchTime < touchDebounce) return;
  // check if touched
  uint16_t x, y; // touched coordinates
  if (!tft.getTouch(&x, &y)) return;
  // debounce timing
  lastTouchTime = millis();
  // process pressed button
  switch (buttonManager->checkTouch(x, y)) {
    case ButtonManager::START_STOP:
      Serial.println("Clicked button START/STOP");
      btnStartPressed = !btnStartPressed;
      buttonManager->drawButtons(btnStartPressed);
      break;     
    case ButtonManager::RESET:
      Serial.println("Clicked button RESET!");
      preloadGraphData();
      currentDataIndex = 0;
      scrollOffset = 0;
      zoomFactor = 1.0;
      visiblePoints = DATA_POINTS;
      drawGraph(graphTop, graphBottom, graphLeft, graphRight);
      break;  
    case ButtonManager::ZOOM_IN:
      Serial.println("Clicked button ZoomIn!");
      zoomIn();
      break;
    case ButtonManager::ZOOM_OUT:
      Serial.println("Clicked button ZoomOut!");
      zoomOut();
      break;
    case ButtonManager::NONE:
    default:
    break;    
  }
}

// [Keep all your other existing functions unchanged]
