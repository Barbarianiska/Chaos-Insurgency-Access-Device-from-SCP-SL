// Display initialization


#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// Initialize pins
#define TFT_CS     5   // Chip Select (any GPIO)
#define TFT_RST    4   // Reset (any GPIO)
#define TFT_DC     2   // Data/Command (any GPIO)
#define TFT_MOSI   23  // SPI MOSI (fixed usually)
#define TFT_SCLK   18  // SPI SCK (fixed usually)

// Display parameters
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);             
constexpr size_t DX = 240; // Display length
constexpr size_t DY = 320; // Display width
constexpr size_t SSS = 15; // Single cell sprite size
constexpr size_t BX = (DX - MY * SSS) / 2; // Bound x for game frame
constexpr size_t BY = BX; // Bound y for game frame
constexpr size_t BW = MX * SSS + 2; // Width x for game frame
constexpr size_t BH = MY * SSS + 2; // Height y for game frame
uint16_t colorBG = tft.color565(155, 190, 40); // Background color
uint16_t colorL = tft.color565(35, 50, 5); // Loading bar color
uint16_t colorO = tft.color565(0, 0, 0); // Text and objects color