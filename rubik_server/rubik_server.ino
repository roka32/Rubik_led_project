#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#include "encoder.h"
#include "buffer.h"

using namespace std;

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t txValue = 0;

int sleepInterval = 16 * 1000;
int timeToSleep = sleepInterval;

RTC_DATA_ATTR Buffer _buffer;
RTC_DATA_ATTR volatile int8_t  encoderPos = 0;
volatile uint8_t turn = 0;
volatile bool senseOfNewTurn = false;
std::string currentValue = "N"; //N like nothing to do or NO value

// define  task for Encoder. BLE runs on core 1 as default
void TaskEncoders( void *pvParameters );

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "a5ad4cec-7b66-11ea-bc55-0242ac130003"
#define CHARACTERISTIC_UUID "a5ad4efe-7b66-11ea-bc55-0242ac130003"
#define SLEEP_PIN 26

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }

};

class MySecurity : public BLESecurityCallbacks {

    bool onConfirmPIN(uint32_t pin) {
      return false;
    }

    uint32_t onPassKeyRequest() {
      Serial.println( "PassKeyRequest");
      return 123456;
    }

    void onPassKeyNotify(uint32_t pass_key) {
      Serial.println((String) "On passkey Notify number:  " + pass_key);
    }

    bool onSecurityRequest() {
      Serial.println( "On Security Request");
      return true;
    }

    void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl) {
      Serial.println( "Starting BLE work!");
      if (cmpl.success) {
        uint16_t length;
        esp_ble_gap_get_whitelist_size(&length);
        Serial.println((String) "size:   " + length);
      }
    }
};

// the setup function runs once when you press reset or power the board
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  /*------------------- Settings to deepsleep -----------------*/
  pinMode(SLEEP_PIN, INPUT);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_26, 1);
  /*------------------- Createing a task for encoders -----------------*/
  xTaskCreatePinnedToCore(
    TaskEncoders
    ,  "Encoders"
    ,  1024  // Stack size
    ,  NULL
    ,  2  // Priority
    ,  NULL
    ,  0); // Core

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.

  // Create the BLE Device
  BLEDevice::init("RubikServer");
  BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEDevice::setSecurityCallbacks(new MySecurity());
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  //|
                      // BLECharacteristic::PROPERTY_NOTIFY |
                      //BLECharacteristic::PROPERTY_INDICATE
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  //pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  //pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  //pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND);
  pSecurity->setCapability(ESP_IO_CAP_OUT);
  pSecurity->setStaticPIN(421916);
  pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);


  Serial.println("Waiting a client connection to notify...");
  Serial.print("BLE task run on core : ");
  Serial.println(xPortGetCoreID());
  pCharacteristic->setValue("N");
}

void loop()
{
  if (deviceConnected) {


    currentValue = pCharacteristic->getValue();
    char currentValue_char = currentValue.at(0);

    if (!_buffer.isEmpty() && currentValue_char == 'N') {
      txValue = _buffer.popFirst();
      pCharacteristic->setValue((uint8_t*)&txValue, 1);
      Serial.print((String)"current valuechar: " + currentValue_char + " sent value: ");
      Serial.println((char)txValue);
      /*Serial.print("all data in buffer: ");
        for(int i =0;i<20;i++){
        Serial.print(_buffer.getElement(i));
        Serial.print(" , ");*/


      delay(50);
    }
  }
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(100); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
}


/*------------------- Encoder Task -----------------*/

void TaskEncoders(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  setup_encoders();
  Serial.print("Encoder task core: ");
  Serial.println(xPortGetCoreID()); // the task run on the core 1

  for (;;)
  {

    if (senseOfNewTurn) {
      Serial.print((char)turn);
      _buffer.push(turn);
      senseOfNewTurn = false;
      timeToSleep = millis() + sleepInterval;
    }


    if (timeToSleep < millis()) {
      esp_deep_sleep_start();
    }

    vTaskDelay(50);
  }
}
