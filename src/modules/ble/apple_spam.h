#pragma once

#if !defined(LITE_VERSION)
#include <Arduino.h>
#include <NimBLEDevice.h>
#include <NimBLEAdvertising.h>

// Menü sisteminde kullanılacak buton/seçenek yapısı
struct Option {
    const char* name;
    void (*callback)();
};

struct ApplePayload {
    const char* name;
    const uint8_t* data;
    uint8_t length;
};

// Fonksiyon Prototipleri
void appleSubMenu();
void startAppleSpam(int payloadIndex);
void startAppleSpamAll();
void stopAppleSpam();
void quickAppleSpam(int payloadIndex);
bool isAppleSpamRunning();
const char* getApplePayloadName(int index);
int getApplePayloadCount();

#endif
