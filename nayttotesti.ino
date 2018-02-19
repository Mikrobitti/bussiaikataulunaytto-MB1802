#include <GxEPD.h>
#include <GxGDE0213B1/GxGDE0213B1.cpp>
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

GxIO_Class io(SPI, SS, D3, D4); // Alusta SPI-väylä
GxEPD_Class display(io); // Adafruit GFX -yhteensopiva luokka

void setup() {
  display.init();  // Alusta näyttö
  display.setTextColor(GxEPD_BLACK); // Aseta musta teksti
  
  display.setCursor(30, 60); // Tekstin sijainti: X=30, Y=60
  display.print("Hello Mikrobitti!");
  display.update(); // Päivitä em. komennot näytölle
}

void loop() {}
