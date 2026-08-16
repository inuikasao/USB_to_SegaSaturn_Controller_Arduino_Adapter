#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>

USB Usb;
USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);

// A  B  C  X  Y  Z  R   L   UP DOWN LEFT RIGHT START
const int PIN_A     = A0;
const int PIN_B     = 3;
const int PIN_C     = 6;
const int PIN_X     = 1;
const int PIN_Y     = 0;
const int PIN_Z     = 2;
const int PIN_R     = A2;
const int PIN_L     = A5;
const int PIN_UP    = 4;
const int PIN_DOWN  = 5;
const int PIN_LEFT  = A1;
const int PIN_RIGHT = A4;
const int PIN_START = A3;

void press(int pin)   { digitalWrite(pin, LOW); }
void release(int pin) { digitalWrite(pin, HIGH); }

void releaseAll() {
  release(PIN_A); release(PIN_B); release(PIN_C);
  release(PIN_X); release(PIN_Y); release(PIN_Z);
  release(PIN_R); release(PIN_L);
  release(PIN_UP); release(PIN_DOWN);
  release(PIN_LEFT); release(PIN_RIGHT);
  release(PIN_START);
}

struct GamePadData {
  uint8_t X1;
  uint8_t Y1;
  uint8_t X2;
  uint8_t Y2;
  uint8_t Rz;
};

class JoystickEvents {
public:
  void OnGamePadChanged(const GamePadData *evt) {
    releaseAll();

    // ボタン
    if (evt->X1 & 0x01) press(PIN_X);
    if (evt->X1 & 0x02) press(PIN_A);
    if (evt->X1 & 0x04) press(PIN_B);
    if (evt->X1 & 0x08) press(PIN_Y);
    if (evt->X1 & 0x10) press(PIN_R);
    if (evt->X1 & 0x20) press(PIN_Z);
    if (evt->X1 & 0x40) press(PIN_L);
    if (evt->X1 & 0x80) press(PIN_C);
    if (evt->Y1 & 0x02) press(PIN_START);

    // 十字キー
    switch (evt->X2) {
      case 0x00: press(PIN_UP);                      break;
      case 0x02: press(PIN_RIGHT);                   break;
      case 0x04: press(PIN_DOWN);                    break;
      case 0x06: press(PIN_LEFT);                    break;
      case 0x01: press(PIN_UP);   press(PIN_RIGHT);  break;
      case 0x03: press(PIN_DOWN); press(PIN_RIGHT);  break;
      case 0x05: press(PIN_DOWN); press(PIN_LEFT);   break;
      case 0x07: press(PIN_UP);   press(PIN_LEFT);   break;
    }
  }
};

JoystickEvents JoyEvents;

class JoystickReportParser : public HIDReportParser {
  JoystickEvents *joyEvents;
  GamePadData prev;

public:
  JoystickReportParser(JoystickEvents *evt) : joyEvents(evt) {
    memset(&prev, 0xFF, sizeof(GamePadData));
  }

  void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) override {
    if (len < sizeof(GamePadData)) return;
    GamePadData curr;
    memcpy(&curr, buf, sizeof(GamePadData));
    if (memcmp(&curr, &prev, sizeof(GamePadData)) != 0) {
      joyEvents->OnGamePadChanged(&curr);
      memcpy(&prev, &curr, sizeof(GamePadData));
    }
  }
};

JoystickReportParser Joy(&JoyEvents);

void setup() {
  pinMode(PIN_A,     OUTPUT); digitalWrite(PIN_A,     HIGH);
  pinMode(PIN_B,     OUTPUT); digitalWrite(PIN_B,     HIGH);
  pinMode(PIN_C,     OUTPUT); digitalWrite(PIN_C,     HIGH);
  pinMode(PIN_X,     OUTPUT); digitalWrite(PIN_X,     HIGH);
  pinMode(PIN_Y,     OUTPUT); digitalWrite(PIN_Y,     HIGH);
  pinMode(PIN_Z,     OUTPUT); digitalWrite(PIN_Z,     HIGH);
  pinMode(PIN_R,     OUTPUT); digitalWrite(PIN_R,     HIGH);
  pinMode(PIN_L,     OUTPUT); digitalWrite(PIN_L,     HIGH);
  pinMode(PIN_UP,    OUTPUT); digitalWrite(PIN_UP,    HIGH);
  pinMode(PIN_DOWN,  OUTPUT); digitalWrite(PIN_DOWN,  HIGH);
  pinMode(PIN_LEFT,  OUTPUT); digitalWrite(PIN_LEFT,  HIGH);
  pinMode(PIN_RIGHT, OUTPUT); digitalWrite(PIN_RIGHT, HIGH);
  pinMode(PIN_START, OUTPUT); digitalWrite(PIN_START, HIGH);

  if (Usb.Init() == -1) while (1);
  Hid.SetReportParser(0, &Joy);
}

void loop() {
  Usb.Task();
}