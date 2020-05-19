#include "ledrubikcube3.h"



#include "BLEDevice.h"
//#include "BLEScan.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#define RESET_PIN 26

// The remote service we wish to connect to.
static BLEUUID serviceUUID("a5ad4cec-7b66-11ea-bc55-0242ac130003");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("a5ad4efe-7b66-11ea-bc55-0242ac130003");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;
bool doWriteEEPROM = false;


std::string readValue;
String resetValue = "N";

char recived_turn_char = 'N';

/*static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    //Serial.print(" of data length ");
    //Serial.println(length);
    Serial.print("   data: ");
    Serial.println((char)*pData );
   // recived_turn_char = *pData;
    //notified = true;
    //delay(500);

  }*/

class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {

    }

    void onDisconnect(BLEClient* pclient) {
      connected = false;
      Serial.println("onDisconnect");
    }
};

bool connectToServer() {
  Serial.print("Forming a connection to ");
  Serial.println(myDevice->getAddress().toString().c_str());

  BLEClient*  pClient  = BLEDevice::createClient();
  Serial.println(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());

  // Connect to the remove BLE Server.
  pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  Serial.println(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our service");


  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our characteristic");

  // Read the value of the characteristic.
  if (pRemoteCharacteristic->canRead()) {
    std::string value = pRemoteCharacteristic->readValue();
    Serial.print("The characteristic value was: ");
    Serial.println(value.c_str());
  }

  /*if(pRemoteCharacteristic->canNotify())
    pRemoteCharacteristic->registerForNotify(notifyCallback);*/

  connected = true;
  doWriteEEPROM = true;
  return true;
}
/**
   Scan for BLE servers and find the first one that advertises the service we are looking for.
*/
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**
        Called for each advertising BLE server.
    */
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.print("BLE Advertised Device found: ");
      Serial.println(advertisedDevice.toString().c_str());

      // We have found a device, let us now see if it contains the service we are looking for.
      if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

        BLEDevice::getScan()->stop();
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;


      } // Found our server
    } // onResult
}; // MyAdvertisedDeviceCallbacks


void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.begin(115200);

  pinMode(RESET_PIN, INPUT_PULLUP);


  //----------------------------
  fastLedInit();
  EEPROM.begin(EEPROM_SIZE);
  loadCube_fromEEPROM();
  upload_cube();
  //edge test_edge= {CRGB::Pink,CRGB::Aqua,CRGB::Black};

  //----------------------

  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("RubikBLEClient");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, true);
} // End of setup.


void loop() {

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {


    // Set the characteristic's value to be the array of bytes that is actually a string.
    readValue = pRemoteCharacteristic->readValue();
    char readValueChar = readValue.at(0);
    cli();
    if ( readValueChar != 'N') {
      //if(!turnDone ){
      switch (readValueChar) {
        case 'F':
          turn_side(0, 0);
          upload_cube();
          Serial.println((String)"turn done with: " + readValueChar );
          pRemoteCharacteristic->writeValue(resetValue.c_str(), resetValue.length());
          break;
        case 'f':
          turn_side(0, 1);
          upload_cube();
          Serial.println((String)"turn done with: " + readValueChar );
          pRemoteCharacteristic->writeValue(resetValue.c_str(), resetValue.length());
          break;
        case 'U':
          turn_side(1, 0);
          upload_cube();
          Serial.println((String)"turn done with: " + readValueChar );
          pRemoteCharacteristic->writeValue(resetValue.c_str(), resetValue.length());
          break;
        case 'u':
          turn_side(1, 1);
          upload_cube();
          Serial.println((String)"turn done with: " + readValueChar );
          pRemoteCharacteristic->writeValue(resetValue.c_str(), resetValue.length());
          break;
        case 'R':
          turn_side(2, 0);
          upload_cube();
          Serial.println((String)"turn done with: " + readValueChar );
          pRemoteCharacteristic->writeValue(resetValue.c_str(), resetValue.length());
          break;
        case 'r':
          turn_side(2, 1);
          upload_cube();
          Serial.println((String)"turn done with: " + readValueChar );
          pRemoteCharacteristic->writeValue(resetValue.c_str(), resetValue.length());
          break;
        case 'L':
          turn_side(3, 0);
          upload_cube();
          Serial.println((String)"turn done with: " + readValueChar );
          pRemoteCharacteristic->writeValue(resetValue.c_str(), resetValue.length());
          break;
        case 'l':
          turn_side(3, 1);
          upload_cube();
          Serial.println((String)"turn done with: " + recived_turn_char );
          Serial.println((String)"turn done with: " + readValueChar );
          pRemoteCharacteristic->writeValue(resetValue.c_str(), resetValue.length());
        case 'D':
          turn_side(4, 0);
          upload_cube();
          Serial.println((String)"turn done with: " + readValueChar );
          pRemoteCharacteristic->writeValue(resetValue.c_str(), resetValue.length());
          break;
        case 'd':
          turn_side(4, 1);
          upload_cube();
          Serial.println((String)"turn done with: " + readValueChar );
          pRemoteCharacteristic->writeValue(resetValue.c_str(), resetValue.length());
          break;
        case 'B':
          turn_side(5, 0);
          upload_cube();
          Serial.println((String)"turn done with: " + readValueChar );
          pRemoteCharacteristic->writeValue(resetValue.c_str(), resetValue.length());
          break;
        case 'b':
          turn_side(5, 1);
          upload_cube();
          Serial.println((String)"turn done with: " + readValueChar );
          pRemoteCharacteristic->writeValue(resetValue.c_str(), resetValue.length());
          break;


      }
    }
    sei();

  } else {
    BLEDevice::getScan()->start(0);  // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
    if (doWriteEEPROM) {
      rubikArray_saveEEPROM();
      doWriteEEPROM = false;
    }
    delay(500);
  }

  if (!digitalRead(RESET_PIN )) {
    cube_reset();
    upload_cube ();
    delay(1000);
  }

  delay(50); // Delay a second between loops.
} // End of loop
