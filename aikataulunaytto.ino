#include <time.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJSON.h>
#include <GxEPD.h>
#include <GxGDE0213B1/GxGDE0213B1.cpp>
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>
#include <Fonts/FreeMono9pt7b.h>

// E-paperinäytön alustaminen
GxIO_Class io(SPI, SS, D3, D4);
GxEPD_Class display(io);

// Määritä nukkumisen kesto, oletuksena 900 s eli 15 min
#define SLEEP_SECONDS 90

// HSL:n pysäkki-id:t voi hakea menemällä osoitteeseen
// https://www.hsl.fi/reitit-ja-aikataulut,
// kirjoittamalla pysäkkihakuun pysäkin nimen, ja
// kopioimalla osoitepalkista pysäkin tunnisteen,
// joka on muotoa HSL:<numerosarja>.

// Koko Suomen kattavia pysäkkitunnisteita voi hakea
// samasta rajapinnasta käyttämällä linkistä
// https://goo.gl/cwAC1H löytyvää kyselyä.

// GraphQL-pyyntö Digitransitin rajapintaan. Kokeile rajapintaa täällä: goo.gl/cwAC1H
static const char digitransitQuery[] PROGMEM = "{\"query\":\"{stops(ids:[\\\"HSL:2215255\\\"]){name,stoptimesWithoutPatterns(numberOfDepartures:17){realtimeDeparture,realtime,trip{route{shortName}}}}}\"}";

// ArduinoJSON-puskurin koko. Ks. https://arduinojson.org/assistant/
// Puskurin on oltava suurempi kuin oletettu JSON-vastaus rajapinnasta.
const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(16) + 32 * JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 15 * JSON_OBJECT_SIZE(3) + 1140;

void printTimetableRow(String busName, String departure, bool isRealtime, int idx) {
    /* Funktio tulostaa näytön puskuriin bussiaikataulurivin. Esim.
       110T  21:34~
    */
    display.setCursor(2, 2 + idx * 14);
    display.print(busName);
    display.setCursor(54, 2 + idx * 14);
    display.print(departure);
    if (isRealtime)
    {
        display.setCursor(108, 2 + idx * 14);
        display.print("~");
    }
}

String parseTime(int seconds) {
    /* Funktio parsii Digitransitin sekuntimuotoisesta lähtöajasta
       merkkijonon. Esimerkiksi 78840 -> "21:54" 
    */
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    char buffer[5];
    if (hours == 25)
        hours = 0;
    sprintf(buffer, "%02d:%02d", hours, minutes);
    return buffer;
}

void setup()
{
    Serial.begin(115200);

    WiFi.begin("Wifi-verkkoni", "salasana");

    // Voit myös asettaa itsellesi staattisen IP:n
    // säästääksesi akkua. Tämä lyhentää Wifi-verkkoon yhdistämistä
    // usealla sekunnilla.
    //IPAddress ip(192,168,1,50);
    //IPAddress gateway(192,168,1,1);
    //IPAddress subnet(255,255,255,0);
    //WiFi.config(ip, gateway, subnet);

    // Yhdistetään langattomaan verkkoon
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("connecting...");
        delay(250);
    }

    /* Seuraavilla riveillä luodaan ja lähetetään HTTP-pyyntö Digitransitin rajapintaan */

    HTTPClient http; // Alustetaan HTTP-Client -instanssi

    // Huomaa kaksi vaihtoehtoista osoitetta Digitransitin rajapintoihin,
    // koko Suomen haku, ja HSL:n haku.
    http.begin("http://api.digitransit.fi/routing/v1/routers/hsl/index/graphql"); // <- HSL
    //http.begin("http://api.digitransit.fi/routing/v1/routers/finland/index/graphql"); // <- koko Suomi

    http.addHeader("Content-Type", "application/json"); // Rajapinta vaatii pyynnön JSON-pakettina
    int httpCode = http.POST(digitransitQuery);         // POST-muotoinen pyyntö
    String payload = http.getString();                  // Otetaan Digitransitin lähettämä vastaus talteen muuttujaan 'payload'
    http.end();

    // Parsitaan vastaus helpomminkäsiteltävään muotoon
    DynamicJsonBuffer jsonBuffer(bufferSize);
    JsonObject &root = jsonBuffer.parseObject(payload.c_str());

    // otetaan referenssi JSON-muotoisen vastauksen bussilähdöistä 'departures'
    JsonArray &departures = root["data"]["stops"][0]["stoptimesWithoutPatterns"];

    // Hyödylliset rivit debuggaukseen:
    // if (!root.success()) {
    //      Serial.println("Parsing failed");
    // }

    // Alustetaan E-paperinäyttö
    display.init();
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeMono9pt7b);

    // Käydään kaikki bussilähdöt yksitellen läpi.
    // Jokainen bussilähtö piirretään e-paperinäytön puskuriin.
    int idx = 0;
    for (auto &dep : departures)
    {
        int departureTime = dep["realtimeDeparture"]; // lähtöaika
        String departure = parseTime(departureTime);  // parsittu lähtöaika
        bool realTime = dep["realtime"]; // onko lähtö tarkka (käyttääkö HSL:n GPS-seurantaa?)
        String busName = dep["trip"]["route"]["shortName"]; // Bussin reittinumero
        printTimetableRow(busName, departure, realTime, ++idx); // tulostetaan rivi näytölle oikeaan kohtaan
    }

    display.update(); // Piirrä näyttöpuskurin sisältö E-paperinäytölle

    // Komennetaan ESP8266 syväunitilaan.
    // Herättyään koodi suoritetaan setup()-funktion alusta
    ESP.deepSleep(SLEEP_SECONDS * 1000000);
}

void loop() {
    // loop() jätetään tyhjäksi, sillä deepsleepistä johtuen
    // koodin suoritus ei ikinä pääse tänne asti.
}
