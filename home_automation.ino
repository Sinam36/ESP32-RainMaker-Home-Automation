#include <EEPROM.h>
#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"
#include <AceButton.h>
using namespace ace_button;

// ======== CONFIGURABLE FLAGS ==========
#define ENABLE_EEPROM true
#define EEPROM_SIZE 10

// ======== DEVICE SETTINGS =============
const char *service_name = "PROV_12345";
const char *pop = "1234567";

char deviceName_1[] = "Switch1";
char deviceName_2[] = "Switch2";
char deviceName_3[] = "Switch3";
char deviceName_4[] = "Switch4";

// ===== GPIO SETUP =====
static uint8_t LoadPin1 = 13;
static uint8_t LoadPin2 = 12;
static uint8_t LoadPin3 = 14;
static uint8_t LoadPin4 = 27;

static uint8_t SwitchPin1 = 18;
static uint8_t SwitchPin2 = 19;
static uint8_t SwitchPin3 = 21;
static uint8_t SwitchPin4 = 22;

static uint8_t wifiLed = 2;
static uint8_t gpio_reset = 25;

bool toggleState_1 = LOW;
bool toggleState_2 = LOW;
bool toggleState_3 = LOW;
bool toggleState_4 = LOW;

// ===== BUTTONS =====
ButtonConfig config1, config2, config3, config4;
AceButton button1(&config1);
AceButton button2(&config2);
AceButton button3(&config3);
AceButton button4(&config4);

// ===== RAINMAKER SWITCHES =====
static Switch my_switch1(deviceName_1, &LoadPin1);
static Switch my_switch2(deviceName_2, &LoadPin2);
static Switch my_switch3(deviceName_3, &LoadPin3);
static Switch my_switch4(deviceName_4, &LoadPin4);

// ===== EEPROM =====
void writeEEPROM(int addr, bool state) {
  if (ENABLE_EEPROM) {
    EEPROM.write(addr, state);
    EEPROM.commit();
  }
}

bool readEEPROM(int addr) {
  return ENABLE_EEPROM ? EEPROM.read(addr) : false;
}

// ===== OUTPUT CONTROL =====
void setOutput(uint8_t pin, int addr, bool state) {
  digitalWrite(pin, state);
  writeEEPROM(addr, state);
}

// ===== BUTTON HANDLER (PUSHBUTTON TOGGLE) =====
void buttonHandler(AceButton*,
                   uint8_t eventType,
                   uint8_t,
                   uint8_t loadPin,
                   int eepromAddr,
                   Switch &sw,
                   bool &state) {

  if (eventType != AceButton::kEventClicked) return;

  bool newState = !state;   // TOGGLE

  setOutput(loadPin, eepromAddr, newState);
  state = newState;

  sw.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, state);

  Serial.printf("Toggled pin %d to %d\n", loadPin, state);
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  toggleState_1 = readEEPROM(0);
  toggleState_2 = readEEPROM(1);
  toggleState_3 = readEEPROM(2);
  toggleState_4 = readEEPROM(3);

  pinMode(LoadPin1, OUTPUT);
  pinMode(LoadPin2, OUTPUT);
  pinMode(LoadPin3, OUTPUT);
  pinMode(LoadPin4, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  pinMode(wifiLed, OUTPUT);
  pinMode(gpio_reset, INPUT);

  setOutput(LoadPin1, 0, toggleState_1);
  setOutput(LoadPin2, 1, toggleState_2);
  setOutput(LoadPin3, 2, toggleState_3);
  setOutput(LoadPin4, 3, toggleState_4);

  // Enable CLICK feature
  config1.setFeature(ButtonConfig::kFeatureClick);
  config2.setFeature(ButtonConfig::kFeatureClick);
  config3.setFeature(ButtonConfig::kFeatureClick);
  config4.setFeature(ButtonConfig::kFeatureClick);

  config1.setDebounceDelay(50);
  config2.setDebounceDelay(50);
  config3.setDebounceDelay(50);
  config4.setDebounceDelay(50);

  config1.setEventHandler([](AceButton* b, uint8_t e, uint8_t s) {
    buttonHandler(b, e, s, LoadPin1, 0, my_switch1, toggleState_1);
  });
  config2.setEventHandler([](AceButton* b, uint8_t e, uint8_t s) {
    buttonHandler(b, e, s, LoadPin2, 1, my_switch2, toggleState_2);
  });
  config3.setEventHandler([](AceButton* b, uint8_t e, uint8_t s) {
    buttonHandler(b, e, s, LoadPin3, 2, my_switch3, toggleState_3);
  });
  config4.setEventHandler([](AceButton* b, uint8_t e, uint8_t s) {
    buttonHandler(b, e, s, LoadPin4, 3, my_switch4, toggleState_4);
  });

  button1.init(SwitchPin1);
  button2.init(SwitchPin2);
  button3.init(SwitchPin3);
  button4.init(SwitchPin4);

  Node my_node = RMaker.initNode("ESP32_Load_4");

  my_switch1.addCb(write_callback);
  my_switch2.addCb(write_callback);
  my_switch3.addCb(write_callback);
  my_switch4.addCb(write_callback);

  my_node.addDevice(my_switch1);
  my_node.addDevice(my_switch2);
  my_node.addDevice(my_switch3);
  my_node.addDevice(my_switch4);

  RMaker.start();
  WiFi.onEvent(sysProvEvent);
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE,
                          WIFI_PROV_SCHEME_HANDLER_FREE_BTDM,
                          WIFI_PROV_SECURITY_1,
                          pop,
                          service_name);
}

// ===== LOOP =====
void loop() {
  digitalWrite(wifiLed, WiFi.status() == WL_CONNECTED);

  button1.check();
  button2.check();
  button3.check();
  button4.check();
}

// ===== RAINMAKER CALLBACK =====
void write_callback(Device *device, Param *param,
                    const param_val_t val,
                    void *, write_ctx_t *) {

  bool newState = val.val.b;

  if (!strcmp(device->getDeviceName(), deviceName_1)) {
    setOutput(LoadPin1, 0, newState);
    toggleState_1 = newState;
  } else if (!strcmp(device->getDeviceName(), deviceName_2)) {
    setOutput(LoadPin2, 1, newState);
    toggleState_2 = newState;
  } else if (!strcmp(device->getDeviceName(), deviceName_3)) {
    setOutput(LoadPin3, 2, newState);
    toggleState_3 = newState;
  } else if (!strcmp(device->getDeviceName(), deviceName_4)) {
    setOutput(LoadPin4, 3, newState);
    toggleState_4 = newState;
  }
}

// ===== PROVISION EVENT =====
void sysProvEvent(arduino_event_t *sys_event) {
  if (sys_event->event_id == ARDUINO_EVENT_PROV_START) {
    printQR(service_name, pop, "ble");
  }
}