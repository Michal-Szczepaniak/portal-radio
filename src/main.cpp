#include <Arduino.h>
#include <driver/i2s.h>
#include <Audio.h>
#include <BluetoothSerial.h>
#include <SD.h>
#include <FS.h>
#include "../.pio/libdeps/esp32dev/ESP32-audioI2S/src/Audio.h"

// I2S
#define LRCLK 25
#define BCLK 32
#define DIN 33

// microSD Card Reader connections
#define SD_CS         17
#define SPI_MOSI      23
#define SPI_MISO      19
#define SPI_SCK       18

Audio audio;
File root;
BluetoothSerial SerialBT;
bool loopMode = true;
int highestRSSI = -200;
bool transmission = false;
unsigned int scanTimestamp = 0;

void btAdvertisedDeviceFound(BTAdvertisedDevice* pDevice) {
    if (pDevice->getRSSI() > highestRSSI)
        highestRSSI = pDevice->getRSSI();
}

void setup() {
    Serial.begin(115200);

    SerialBT.begin("Portal Radio");
    pinMode(2, INPUT_PULLUP);

    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    if(!SD.begin(SD_CS))
    {
        Serial.println("Error accessing microSD card!");
        while(true);
    }

    audio.setPinout(BCLK, LRCLK, DIN);

    audio.setVolume(12);
    audio.forceMono(true);

    if (digitalRead(2) == LOW) {
        audio.connecttoFS(SD, "/looping_radio_mix_1min.wav");
        audio.setFileLoop(true);

        SerialBT.discoverAsync(btAdvertisedDeviceFound);
        scanTimestamp = millis();
    } else {
        loopMode = false;
        root = SD.open("/");
    }
}

unsigned long volume_timestamp = millis();
void loop() {
    audio.loop();

    if (!audio.isRunning()) {
        File entry = root.openNextFile();
        if (!entry) {
            root.rewindDirectory();
            return;
        }
        String filename = entry.name();
        entry.close();
        if (filename == "looping_radio_mix_1min.wav" || filename == "dinosaur_01.wav")
            return;
        audio.connecttoFS(SD, ("/" + filename).c_str());
    }

    if (millis() - scanTimestamp > 3000 && loopMode) {
        SerialBT.discoverAsyncStop();
        scanTimestamp = millis();
        if (highestRSSI > -55 && !transmission) {
            audio.connecttoFS(SD, "/dinosaur_01.wav");
            audio.setFileLoop(true);
            transmission = true;
        } else if (highestRSSI <= -55 && transmission) {
            audio.connecttoFS(SD, "/looping_radio_mix_1min.wav");
            audio.setFileLoop(true);
            transmission = false;
        }
        highestRSSI = -200;
        SerialBT.discoverAsync(btAdvertisedDeviceFound);
    }
}

