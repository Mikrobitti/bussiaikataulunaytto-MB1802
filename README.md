# 2-18-aikataulunaytto

Oheisesta repositoriosta löytyy mallikoodi Mikrobitin numerossa 2/18 ilmestyneeseen tee-se-itse -aikataulunäyttöön. Automaattisesti päivittyvä bussiaikataulunäyttö pelastaa kiireisen työn sankarin aamun kertomalla tarkalleen, koska seuraava bussi kotipysäkin ohi pyyhältää.

Bussinäyttö koostuu e-paperinäytöstä, Wemos D1 Mini -mikrokontrollerista, litiumakusta latauspiireineen sekä 3D-tulostettavasta kotelosta. Wemos-mikrokontrollerin voi ohjelmoida helposti tuttuja Arduino-komentoja apuna käyttäen.

## Tiedostot

```
.
├── aikataulunaytto.ino                     # koko bussiaikataulunäytön Arduino-koodi
├── bussiaikataulunaytto_kotelo_bottom.stl  # kotelon pohjan 3d-malli
├── bussiaikataulunaytto_kotelo_top.stl     # kotelon kannen 3d-malli
├── bussiaikataulunaytto_taustalevy.stl     # seinäkiinnikkeen 3d-malli
└── nayttotesti.ino                         # koodi näytön testaamiseen
```

## 3D-mallit

Kotelon 3D-malleja voit tarkastella (ja muokata ottamalla itsellesi kopion) [OnShapessa](https://cad.onshape.com/documents/f1fb8a455b82d920070b3ebc/w/c682447b5a39c870d298b90b/e/590ca089253f38d85d7e398c).


## Asennus ja käyttöönotto

1. Asenna Arduino IDE (versio 1.8 tai uudempi)
2. Avaa **Preferences (Asetukset)** ja lisää osoite *http://arduino.esp8266.com/stable/package_esp8266com_index.json* kenttään *Additional Board Manager URLs*
3. Avaa **Tools > Board** -valikosta *Boards Manager* ja asenna paketti *esp8266*
4. Ota Wemos D1 Mini kohdealustaksi valikosta **Tools > Board**
5. Asenna tarvittavat kirjastot (ArduinoJSON ja Adafruit GFX) *Library Managerilla*. Sen löydät valikosta **Sketch > Include Library > Manage Libraries...**
6. Asenna GxEPD [täältä](https://github.com/ZinggJM/GxEPD) lataamalla se, ja tuomalla se zip-tiedostosta. Se onnistuu valikosta **Sketch > Include Library > Add .ZIP Library...**
7. Muokkaa koodia (ainakin Wifi-tunnuksesi!) ja ohjelmoi se Wemokseen upload-nappia painamalla!
