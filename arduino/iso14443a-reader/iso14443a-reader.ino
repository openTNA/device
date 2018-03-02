#include <PN532_HSU.h>
#include <PN532.h>

PN532_HSU pn532hsu(Serial1);
PN532 nfc(pn532hsu);

#define BUZZER_PIN 5

void setup(void) {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);
  Serial.begin(115200);
  nfc.begin();
  uint32_t ver = nfc.getFirmwareVersion();
  if (! ver) {
    while (1);
  }
  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();
}

void buzzer(void) {
  digitalWrite(BUZZER_PIN, LOW);
  delay(200);
  digitalWrite(BUZZER_PIN, HIGH);
}

void loop(void) {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t len;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &len);
  if (success) {
    long result = 0;
    if (len == 4) {
      for (int i = len - 1; i >= 0; i--) {
        result = (result << 8) + uid[i];
      }
    }
    if (result < 0) {
      result = result * -1;
    }
    Serial.println(result);
    buzzer();
    delay(800);
  }
}
