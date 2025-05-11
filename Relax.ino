#pragma GCC optimize ("-O3")  // Fast but standards-compliant
//
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <Arduino.h>
#include "lgfx_NT35510.h"   // External configuration for LCD and Touch
#include <XPT2046_Touchscreen.h> 

// Graph and button variables
#define DATA_POINTS 60  // Total number of points in the graph
int graphData[DATA_POINTS]; // Store preloaded values

bool isGraphFilled =     false;
bool btnStartPressed =   false;
bool btnResetPressed =   false;
bool btnZoomInPressed  = false;
bool btnZoomOutPressed = false;
//
float scaleX = 1.0;         // Horizontal scale (time)
float scaleY = 1.0;         // Vertical scale (value range)
int graphColor = TFT_GREEN; // Default graph color is green (positive)

// Define button positions in full 800x480 resolution
int btnWidth = 200;   // all buttons
int btnHeight = 80;   // all buttons
//
int btnStartX = 0, btnStartY = 400;
int btnResetX = btnWidth + 1 ,    btnResetY   = btnStartY;
int btnZoomInX = btnResetX + 1,   btnZoomInY  = btnStartY;
int btnZoomOutX = btnZoomInX + 1, btnZoomOutY = btnStartY;

// Define graph area
int graphTop    =  60;      
int graphBottom = 389;  // Ends at 300 (higher up)
int graphLeft   =  20;     
int graphRight  = 780;
int graphSideOffset = graphLeft;
//
int titleHeight = 40;
//
int startTime = 0;
int endTime   = 0;

//
void setup() {
  Serial.begin(9600);
  tft.init();
  tft.setRotation(3);         // 270 degrees
  tft.fillScreen(TFT_BLACK);  // Clear screen
  //tft.setSwapBytes(true); // For images, SPIFFS or SD. Some image sources (like SPIFFS, SD cards, or buffers) store color data in Big-Endian (MSB first), while some LCDs expect Little-Endian (LSB first). It ensures that the color data is in the correct byte order before sending it to the display.                           
  DisplayTitle();
  //
  drawButtons();
  drawGrid(graphTop, graphBottom, graphLeft, graphRight);
  // sim data
  preloadGraphData();

  startTime = millis();  // Start tracking time
}

void loop() {
  updateGraph(graphTop, graphBottom, graphLeft, graphRight);
  handleTouch();
}

//
void drawGrid(int grphTop, int grphBottom, int grphLeft, int grphRight) { // 60,389,20,780 - graph area
  int gridXperSquare = (graphRight - graphLeft)/15;
  int gridYperSquare = (graphBottom - graphTop)/10;

    // Draw X-Y Axes
    tft.drawLine(grphLeft, grphTop - 40, grphLeft, grphBottom, TFT_WHITE);     // Y
    tft.drawLine(grphLeft, grphBottom, grphRight + 19, grphBottom, TFT_WHITE); // X

    // Draw main axes
    tft.drawLine(grphRight + graphSideOffset, grphBottom, grphRight + graphSideOffset - 14, grphBottom + 5, TFT_WHITE);   // X-axis arrow left line
    tft.drawLine(grphRight+ graphSideOffset, grphBottom, grphRight + graphSideOffset - 14, grphBottom - 5, TFT_WHITE);   // X-axis arrow right line
    tft.drawLine(grphLeft, grphTop - titleHeight, grphLeft + 5, grphTop - titleHeight + 14, TFT_WHITE); // Y-axis arrow top-left line
    tft.drawLine(grphLeft, grphTop - titleHeight, grphLeft - 5, grphTop - titleHeight + 14, TFT_WHITE); // Y-axis arrow top-right line
    // Draw "100%" label on the Y-axis
    tft.setFont(&fonts::FreeSerif9pt7b); //lgfxJapanMinchoP_28);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.setCursor(grphLeft + 10, grphTop - titleHeight - 10);
    tft.print("100%");
    // Draw Grid on Graph Area 760x360 
    for (int i = 0; i <= 10; i++) { // Y lines
        tft.drawLine(grphLeft, grphTop + (i * gridYperSquare), grphRight, grphTop + (i * gridYperSquare), TFT_DARKGREY);
    }
    // X lines
    for (int i = 0; i <= 15; i++) {
        tft.drawLine(grphLeft + (i * gridXperSquare), grphTop, grphLeft + (i * gridXperSquare), grphBottom, TFT_DARKGREY);
    }
}

//
void drawGraph(int grTop, int grBottom, int grLeft, int grRight) {
    // clear graph area
    tft.fillRect(grLeft, 61, grRight - grLeft, grBottom - grTop, TFT_NAVY); // 0,61,800,340
    //
    drawGrid(grTop, grBottom, grLeft, grRight);

    // Graph Scaling (Adjusted for new position)
    float scaleX = 12.5;  
    float scaleY = 2.5;   

    // Draw Graph Data (Moves to top half
    for (int i = 1; i < 60; i++) {
        int prevData = graphData[i - 1];
        int currentData = graphData[i];

        int x1 = grLeft + (i - 1) * scaleX;
        int y1 = grBottom - (prevData * scaleY);
        int x2 = grLeft + i * scaleX;
        int y2 = grBottom - (currentData * scaleY);
        //
        tft.drawLine(x1, y1-170, x2, y2-170, TFT_GREEN);
    } 
}

//
void updateGraph(int grTop, int grBottom, int grLeft, int grRight) {
  static int lastUpdate = 0;
  if (millis() - lastUpdate >= 1000) {
    lastUpdate = millis();
    for (int i = 0; i < 60; i++) {
      graphData[i] = random(-50, 50);
    }
    //
    drawGraph(grTop, grBottom, grLeft, grRight);
  }
}

//
void handleTouch() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y)) {  
    //Serial.printf("%d,%d\n", x, y); 
    if (x > btnStartX && x < (btnStartX + btnWidth) && y > btnStartY && y < (btnStartY + btnHeight)) { 
      Serial.println("Clicked button START");
      btnStartPressed = !btnStartPressed;
      drawButtons();
      delay(20);
    }
    //
    if (x > btnResetX && x < (btnResetX + btnWidth) && y > btnResetY && y < (btnResetY + btnHeight)) {
      //
      btnResetPressed = true;
      Serial.println("Clicked button RESET!");
      memset(graphData, 0, sizeof(graphData));
      drawGraph(graphTop, graphBottom, graphLeft, graphRight);
      delay(20);
      btnResetPressed = false;
    }
    //
    if (x > btnZoomInX && x < (btnZoomInX + btnWidth) && y > btnZoomInY && y < (btnZoomInY + btnHeight)) {  
      //
      Serial.println("Clicked button ZoomIn!");
      btnZoomInPressed =  true;
      drawButtons();
      delay(20);
      btnZoomInPressed = false;
    }
    //
    if (x > btnZoomOutX && x < (btnZoomOutX + btnWidth) && y > btnZoomOutY && y < (btnZoomOutY + btnHeight)) {  
      //
      Serial.println("Clicked button ZoomOut!");
      btnZoomOutPressed = true;    
      drawButtons();
      delay(20);
      btnZoomOutPressed = false;
    }
  }
}

// ZOOM
void zoomGraph(int delta) {
  scaleX *= (delta > 0) ? 1.1 : 0.9;
  scaleY *= (delta > 0) ? 1.1 : 0.9;
}

// TItle
void DisplayTitle() {  // 1Line=30->45
  tft.setFont(&fonts::lgfxJapanMinchoP_28);
  const char* title = "RELAX v0.2";
  int titleWidth = tft.textWidth(title);
  int positionX = (tft.width() - titleWidth) / 2;  // Center the text
  int positionY = 10;   
  //
  tft.setCursor(positionX + 1, (positionY - 1));     //  45 *  (positionY - 1));         // yLine:1-8
  tft.setTextColor(0xF81F);  //magenta, 0xFFFF - white
  tft.println(String(title));  
  // Draw fat blue line
  int textHeight = tft.fontHeight();
  int lineY = positionY + textHeight + 5;                                               // Position below text
  tft.drawLine(20, lineY, positionX - 10, lineY, TFT_BLUE);                             // Left part
  tft.fillRect(positionX - 5, lineY - 2, titleWidth + 10, 4, TFT_BLUE);                 // Thick center part
  tft.drawLine(positionX + titleWidth + 10, lineY, tft.width() - 20, lineY, TFT_BLUE);  // Right part
}

// BUTTONS
void drawButtons() {
    int btnHeight = 60;  
    int btnWidth = 200;  
    int btnY = 480 - btnHeight;  // Buttons are clearly visible below graph
    int lblOffsetX = 20;
    int lblOffsetY = 15;
    //
    tft.setFont(&fonts::FreeSerif18pt7b); // FreeSerifBold24pt7b);
    // "Start/Stop" Button (Left Side)
    tft.fillRect(1, btnY, btnWidth, btnHeight, (btnStartPressed) ? TFT_RED : TFT_GREEN);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(1);           // Small text
    tft.setCursor(lblOffsetX + lblOffsetX, btnY + lblOffsetY);      // Centered inside button
    tft.print(btnStartPressed ? "Stop" : "Start");
    // "Reset" Button (Right Side)
    tft.fillRect(btnWidth, btnY, btnWidth, btnHeight, TFT_SKYBLUE); //TFT_YELLOW
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(btnWidth + lblOffsetX + lblOffsetX, btnY + lblOffsetY); // Centered inside button
    tft.print("Reset");
    // Zoom In
    tft.fillRect(btnWidth*2, btnY, btnWidth, btnHeight, TFT_YELLOW); //TFT_YELLOW
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(btnWidth*2 + lblOffsetX, btnY + lblOffsetY); // Centered inside button
    tft.print("Zoom In");
    // Zoom Out
    tft.fillRect(btnWidth*3, btnY, btnWidth, btnHeight, TFT_PINK); //TFT_YELLOW
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(0);
    tft.setCursor(btnWidth*3 + lblOffsetX, btnY + lblOffsetY); // Centered inside button
    tft.print("Zoom Out");
}

//
void preloadGraphData() {
    // 1st phase: Smooth growth from 20% to 60%
    for (int i = 0; i < 15; i++) {
        graphData[i] = map(i, 0, 15, 20, 60);
    }

    // 2nd phase: Three slow zigzags (oscillating between 60% and 80%)
    for (int i = 15; i < 45; i++) {
        graphData[i] = 70 + 10 * sin((i - 15) * PI / 15);
    }

    // 3rd phase: Flattening near 80% with slight variations
    for (int i = 45; i < DATA_POINTS; i++) {
        graphData[i] = 78 + random(-2, 3);
    }
}
