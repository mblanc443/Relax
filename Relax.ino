#pragma GCC optimize ("Ofast")
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <Arduino.h>
#include "lgfx_NT35510.h"   // External configuration for LCD and Touch

// Graph and button variables
int graphData[60];          // Stores graph data for 1 minute (60 seconds)
bool isGraphFilled = false;
bool btnStartPressed = false;
bool btnResetPressed = false;
float scaleX = 1.0;         // Horizontal scale (time)
float scaleY = 1.0;         // Vertical scale (value range)
int graphColor = TFT_GREEN; // Default graph color is green (positive)

// Define button positions in full 800x480 resolution
int btnStartX = 100, btnStartY = 350, btnWidth = 200, btnHeight = 80;
int btnResetX = 350, btnResetY = 350;
//
int startTime = 0;
int endTime = 0;

//
void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);  //
  //tft.setSwapBytes(true); // For images, SPIFFS or SD. Some image sources (like SPIFFS, SD cards, or buffers) store color data in Big-Endian (MSB first), while some LCDs expect Little-Endian (LSB first). It ensures that the color data is in the correct byte order before sending it to the display.                           
  DisplayTitle();
  //
  drawButtons();
  drawGrid();
  startTime = millis();  // Start tracking time
}

void loop() {
  //updateGraph();
  //handleTouch();
}

void drawGrid() {
  // Define graph area
  int graphTop = 60;      // from 10 after Title + Underscore    
  int graphBottom = 420;  // Ends at bottom - 60
  int graphLeft = 20;     
  int graphRight = 780;
  int gridXperSquare = (graphRight - graphLeft)/15;
  int gridYperSquare = (graphBottom - graphTop)/10;
 
    // Draw X-Y Axes
    tft.drawLine(graphLeft, graphTop - 40, graphLeft, graphBottom, TFT_WHITE);     // Y
    tft.drawLine(graphLeft, graphBottom, graphRight, graphBottom, TFT_WHITE); // X
    // Draw Grid on Graph Area 760x360 
    for (int i = 0; i <= 10; i++) { // Y lines
        tft.drawLine(graphLeft, graphTop + (i * gridYperSquare), graphRight, graphTop + (i * gridYperSquare), TFT_DARKGREY);
    }
    // X lines
    for (int i = 0; i <= 15; i++) {
        tft.drawLine(graphLeft + (i * gridXperSquare), graphTop, graphLeft + (i * gridXperSquare), graphBottom, TFT_DARKGREY);
    }

}

//
void drawGraph() {
    
    //tft.fillScreen(TFT_BLACK); // Clear screen

    // Define graph area
    int graphTop = 10;      
    int graphBottom = 430;  // Ends at 300 (higher up)
    int graphLeft = 10;     
    int graphRight = 790;
    // repaint black graph area
    tft.fillRect(0, graphTop, 800, graphBottom, TFT_BLACK);
    // Draw Axes
    tft.drawLine(graphLeft, graphTop, graphLeft, graphBottom, TFT_WHITE);  
    tft.drawLine(graphLeft, graphBottom, graphRight, graphBottom, TFT_WHITE);  

    // Draw Grid
    for (int i = 0; i <= 10; i++) {
        tft.drawLine(graphLeft, graphTop + (i * 26), graphRight, graphTop + (i * 26), TFT_DARKGREY);
    }
    for (int i = 0; i <= 10; i++) {
        tft.drawLine(graphLeft + (i * 70), graphTop, graphLeft + (i * 70), graphBottom, TFT_DARKGREY);
    }

    // Graph Scaling (Adjusted for new position)
    float scaleX = 12.5;  
    float scaleY = 2.5;   

    // Draw Graph Data (Moves to top half)
    for (int i = 1; i < 60; i++) {
        int prevData = graphData[i - 1];
        int currentData = graphData[i];

        int x1 = graphLeft + (i - 1) * scaleX;
        int y1 = graphBottom - (prevData * scaleY);
        int x2 = graphLeft + i * scaleX;
        int y2 = graphBottom - (currentData * scaleY);

        graphColor = (currentData >= 0) ? TFT_GREEN : TFT_RED;
        tft.drawLine(x1, y1, x2, y2, graphColor);
    }
    drawButtons(); // Draw buttons LAST
}

//
void updateGraph() {
  static int lastUpdate = 0;
  if (millis() - lastUpdate >= 1000) {
    lastUpdate = millis();
    for (int i = 0; i < 60; i++) {
      graphData[i] = random(-50, 50);
    }
    drawGraph();
  }
}


//
void handleTouch() {
  uint16_t x, y;
  
  if (tft.getTouch(&x, &y)) {  // Use tft.getTouch() instead
    Serial.printf("%d,%d\n", x, y);
    // 
    if (x > btnStartX && x < btnStartX + btnWidth && y > btnStartY && y < btnStartY + btnHeight) {  
      btnStartPressed = !btnStartPressed;
      drawButtons();
    }

    if (x > btnResetX && x < btnResetX + btnWidth && y > btnResetY && y < btnResetY + btnHeight) {
      memset(graphData, 0, sizeof(graphData));
      drawGraph();
    }
  }
}

void zoomGraph(int delta) {
  scaleX *= (delta > 0) ? 1.1 : 0.9;
  scaleY *= (delta > 0) ? 1.1 : 0.9;
}

// TItle
void DisplayTitle() {  // 1Line=30->45
  tft.setFont(&fonts::lgfxJapanMinchoP_28);
  const char* title = "Relax Application ESP32 v0.1";
  int titleWidth = tft.textWidth(title);
  int positionX = (tft.width() - titleWidth) / 2;  // Center the text
  int positionY = 10;   
  //
  tft.setCursor(positionX + 1, (positionY - 1));     //  45 *  (positionY - 1));         // yLine:1-8
  tft.setTextColor(0xF81F);  //magenta, 0xFFFF - white
  tft.println(String(title));  
  //
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
    
    // TMP
    btnStartPressed = false;
    //
    tft.setFont(&fonts::FreeSerif18pt7b); // FreeSerifBold24pt7b);
    // "Start/Stop" Button (Left Side)
    tft.fillRect(1, btnY, btnWidth, btnHeight, (!btnStartPressed) ? TFT_GREEN : TFT_RED);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(1);           // Smaller text
    tft.setCursor(lblOffsetX + lblOffsetX, btnY + lblOffsetY); // Centered inside button
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
    tft.setTextSize(1);
    tft.setCursor(btnWidth*3 + lblOffsetX, btnY + lblOffsetY); // Centered inside button
    tft.print("Zoom Out");
}

