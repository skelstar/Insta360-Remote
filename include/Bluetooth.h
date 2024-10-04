#pragma once
#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>

#include "Camera.h"

namespace Bluetooth
{
    int create_cmd(uint8_t *cmd, uint32_t *sn, uint8_t mode, uint8_t c1, uint8_t *pb, int pb_len);
    void process_cmd(uint8_t *pData, int length);
    void send_cmd(uint8_t mode, uint8_t c1, uint8_t *pb, int pb_len);
    void decodeStatus2(uint8_t *pData, int length);
    bool connectToServer();
    void handleButtonPacket(ButtonPacket *packet);
    void changeMode();

    void powerOnPrevConnectedCameras();
    void modeButton(BLECharacteristic *characteristic);
    void shutterButton(BLECharacteristic *characteristic);
    void screenToggle(BLECharacteristic *characteristic);
    void powerOff(BLECharacteristic *characteristic);

#define SERVICE_UUID "ce80"
#define CHARACTERISTIC_UUID1 "ce81"
#define CHARACTERISTIC_UUID2 "ce82"
#define CHARACTERISTIC_UUID3 "ce83"

    static BLEUUID serviceUUID_80("0000be80-0000-1000-8000-00805f9b34fb");
    static BLEUUID readCharacteristicUUID("0000be81-0000-1000-8000-00805f9b34fb");
    static BLEUUID writeCharacteristicUUID("0000be82-0000-1000-8000-00805f9b34fb");

    static BLERemoteCharacteristic *pRemoteReadCharacteristic;
    static BLERemoteCharacteristic *pRemoteWriteCharacteristic;
    static BLEAdvertisedDevice *myDevice;

#define SECONDARY_SERVICE_UUID "0000D0FF-3C17-D293-8E48-14FE2E4DA212"
#define SECONDARY_CHARACTERISTIC_UUID1 "ffd1"
#define SECONDARY_CHARACTERISTIC_UUID2 "ffd2"
#define SECONDARY_CHARACTERISTIC_UUID3 "ffd3"
#define SECONDARY_CHARACTERISTIC_UUID4 "ffd4"
#define SECONDARY_CHARACTERISTIC_UUID5 "ffd5"
#define SECONDARY_CHARACTERISTIC_UUID8 "ffd8"
#define SECONDARY_CHARACTERISTIC_UUID9 "fff1"
#define SECONDARY_CHARACTERISTIC_UUID10 "fff2"
#define SECONDARY_CHARACTERISTIC_UUID11 "ffe0"

    unsigned long buttonPacketId = -1;

    BLEServer *pServer = NULL;
    BLE2902 *pDescriptor2902;
    BLEAdvertisedDevice *advertisedDevice;
    BLEClient *client;
    bool clientConnected = false;

    bool deviceConnected = false;
    bool oldDeviceConnected = false;

    BLECharacteristic *pCharacteristicRx;

    unsigned long myTime;

    uint32_t data_32;
    uint16_t data_16;
    uint8_t data_8[30];

    uint8_t manuf_data[30];

    class MyServerCallbacks : public BLEServerCallbacks
    {
        void onConnect(BLEServer *pServer)
        {
            deviceConnected = true;
            myTime = millis();
            BLEDevice::startAdvertising();

            Serial.printf("onConnect... advertising!\n");
        };

        void onDisconnect(BLEServer *pServer)
        {
            deviceConnected = false;
            Serial.printf("onDisconnect...\n");
        }
    };

    class MyClientCallback : public BLEClientCallbacks
    {
        void onConnect(BLEClient *pclient)
        {
            Serial.printf("[info] Connected: %s\n", pclient->getPeerAddress().toString().c_str());
        }

        void onDisconnect(BLEClient *pclient)
        {
            Serial.printf("[info] Disconnected: %s\n", pclient->getPeerAddress().toString().c_str());
        }
    };

    static void notifyCallback(
        BLERemoteCharacteristic *pBLERemoteCharacteristic,
        uint8_t *pData,
        size_t length,
        bool isNotify)
    {
        std::string notifyCharUUID = pBLERemoteCharacteristic->getUUID().toString();

        Serial.printf("notify callback\n");

        // if (notifyCharUUID == hrMeasureCharacteristicUUID.toString())
        // {
        //     handleHeartRate(pData[1]);
        // }
        // else if (notifyCharUUID == controllerCharacteristicUUID.toString())
        // {
        //     handleHudControllerAction(pData[0]);
        //     // Serial.printf("Notify callback! %s \n", "Controller Characteristic");
        // }
    }

    // Function to map UUIDs to human-readable names
    String getCharacteristicName(BLEUUID uuid)
    {
        // Add mappings for known UUIDs
        if (uuid.equals(BLEUUID(CHARACTERISTIC_UUID1)))
        {
            return "CHARACTERISTIC_UUID1";
        }
        else if (uuid.equals(BLEUUID(CHARACTERISTIC_UUID2)))
        {
            return "CHARACTERISTIC_UUID2";
        }
        else if (uuid.equals(BLEUUID(CHARACTERISTIC_UUID3)))
        {
            return "CHARACTERISTIC_UUID3";
        }

        // Add other UUID mappings here
        return "Unknown Characteristic";
    }

    class MyCallbacks : public BLECharacteristicCallbacks
    {
        void onWrite(BLECharacteristic *pCharacteristic)
        {
            BLEUUID uuid = pCharacteristic->getUUID();

            std::string value = pCharacteristic->getValue();

            // video time remaining: feeffe1080901c461203368376d
            // video clock:
            // ␀␐F␁.00:00:0104612e30303a30303a3031���␐�
            // ␀␎F␁.00:00:02 4612e30303a30303a3032���␐�
            // ␀␎F␁.00:00:03 4612e30303a30303a3033���␐�
            // ␀␎F␁.00:00:04 4612e30303a30303a3034���␐�
            // ␀␎F␁.00:00:05 4612e30303a30303a3035���␐�
            // ␀␎F␁.00:00:06 4612e30303a30303a3036���␐�
            // ␀␎F␁.00:00:07 4612e30303a30303a3037���␐�

            if (value.length() > 0)
            {
                Serial.printf("***** New Value Written (%s) *****\n", getCharacteristicName(uuid).c_str());

                uint16_t videotimer = 0x4612;

                char parsedHexStr[5];                      // Enough space for 4 digits + null terminator
                sprintf(parsedHexStr, "%04X", videotimer); // Convert parsedHex to a 4-digit hex string

                // Extract the substring from hexString (the first 4 characters, same length as parsedHex)
                std::string hexSubStr = value.substr(0, 4);

                if (hexSubStr == parsedHexStr)
                {
                    Serial.printf("Video timer\n");
                }
                else
                {

                    for (int i = 0; i < value.length(); i++)
                        Serial.printf("%x", value[i]);

                    Serial.printf(" - ");

                    for (int i = 0; i < value.length(); i++)
                        Serial.print(value[i]);
                    Serial.println();
                }
            }
        }

        void onRead(BLECharacteristic *pCharacteristic)
        {
            Serial.println("***** Characteristic Read *****");
            // You can send some updated data back if necessary
        }
    };

    void setup()
    {
        vTaskDelay(TICKS_500ms);

        Serial.println("Starting BLE work!");

        deviceConnected = false;
        oldDeviceConnected = false;

        BLEDevice::init("Insta360 GPS Remote");
        pServer = BLEDevice::createServer();
        pServer->setCallbacks(new MyServerCallbacks());

        BLEService *pService = pServer->createService(SERVICE_UUID);
        BLECharacteristic *pCharacteristic1 = pService->createCharacteristic(
            CHARACTERISTIC_UUID1, BLECharacteristic::PROPERTY_WRITE);

        pCharacteristicRx = pService->createCharacteristic(
            CHARACTERISTIC_UUID2, BLECharacteristic::PROPERTY_NOTIFY);

        pDescriptor2902 = new BLE2902();
        pDescriptor2902->setNotifications(true);
        pDescriptor2902->setIndications(true);
        pCharacteristicRx->addDescriptor(pDescriptor2902);
        data_8[0] = 0;
        pCharacteristicRx->setValue(data_8, 1);

        BLECharacteristic *pCharacteristic3 = pService->createCharacteristic(
            CHARACTERISTIC_UUID3, BLECharacteristic::PROPERTY_READ);
        data_16 = 0x0201;
        pCharacteristic3->setValue(data_16);

        BLEService *pService2 = pServer->createService(SECONDARY_SERVICE_UUID);
        BLECharacteristic *secondary_pCharacteristic1 =
            pService2->createCharacteristic(SECONDARY_CHARACTERISTIC_UUID1,
                                            BLECharacteristic::PROPERTY_WRITE);

        BLECharacteristic *secondary_pCharacteristic2 =
            pService2->createCharacteristic(SECONDARY_CHARACTERISTIC_UUID2,
                                            BLECharacteristic::PROPERTY_READ);

        BLECharacteristic *secondary_pCharacteristic3 =
            pService2->createCharacteristic(SECONDARY_CHARACTERISTIC_UUID3,
                                            BLECharacteristic::PROPERTY_READ);
        data_32 = 0x301e9001;
        secondary_pCharacteristic3->setValue(data_32);

        BLECharacteristic *secondary_pCharacteristic4 =
            pService2->createCharacteristic(SECONDARY_CHARACTERISTIC_UUID4,
                                            BLECharacteristic::PROPERTY_READ);

        data_32 = 0x18002001;
        secondary_pCharacteristic4->setValue(data_32);

        BLECharacteristic *secondary_pCharacteristic5 =
            pService2->createCharacteristic(SECONDARY_CHARACTERISTIC_UUID5,
                                            BLECharacteristic::PROPERTY_READ);

        BLECharacteristic *secondary_pCharacteristic8 =
            pService2->createCharacteristic(SECONDARY_CHARACTERISTIC_UUID8,
                                            BLECharacteristic::PROPERTY_WRITE);

        BLECharacteristic *secondary_pCharacteristic9 =
            pService2->createCharacteristic(SECONDARY_CHARACTERISTIC_UUID9,
                                            BLECharacteristic::PROPERTY_READ);

        BLECharacteristic *secondary_pCharacteristic10 =
            pService2->createCharacteristic(SECONDARY_CHARACTERISTIC_UUID10,
                                            BLECharacteristic::PROPERTY_WRITE);

        BLECharacteristic *secondary_pCharacteristic11 =
            pService2->createCharacteristic(SECONDARY_CHARACTERISTIC_UUID11,
                                            BLECharacteristic::PROPERTY_READ);

        pCharacteristic1->setCallbacks(new MyCallbacks());
        secondary_pCharacteristic11->setCallbacks(new MyCallbacks());
        pCharacteristicRx->setCallbacks(new MyCallbacks());
        pCharacteristic3->setCallbacks(new MyCallbacks());

        pService->start();
        pService2->start();
        // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still
        // is working for backward compatibility
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(SERVICE_UUID);
        pAdvertising->addServiceUUID(SECONDARY_SERVICE_UUID);

        pAdvertising->setScanResponse(true);
        pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
        pAdvertising->setMinPreferred(0x12);

        BLEDevice::startAdvertising();
        Serial.println("Characteristic defined! Now you can read it in your phone!");

        /* set the manufacturing data for wakeon packet */
        manuf_data[0] = 0x4c;
        manuf_data[1] = 0x00;
        manuf_data[2] = 0x02;
        manuf_data[3] = 0x15;
        manuf_data[4] = 0x09;
        manuf_data[5] = 0x4f;
        manuf_data[6] = 0x52;
        manuf_data[7] = 0x42;
        manuf_data[8] = 0x49;
        manuf_data[9] = 0x54;
        manuf_data[10] = 0x09;
        manuf_data[11] = 0xff;
        manuf_data[12] = 0x0f;
        manuf_data[13] = 0x00;
        /* note: see powerOnPrevConnectedCameras() for bytes 14-19 */
        manuf_data[20] = 0x00;
        manuf_data[21] = 0x00;
        manuf_data[22] = 0x00;
        manuf_data[23] = 0x00;
        manuf_data[24] = 0xe4;
        manuf_data[25] = 0x01;

        // client = BLEDevice::createClient();

        // connectClient();
    }

    void loop()
    {
        // notify changed value
        if (deviceConnected)
        {
            delay(10); // bluetooth stack will go into congestion, if too many packets
        }

        // disconnecting
        if (!deviceConnected && oldDeviceConnected)
        {
            vTaskDelay(TICKS_500ms);     // give the bluetooth stack the chance to get things ready
            pServer->startAdvertising(); // restart advertising
            Serial.println("start advertising");
            oldDeviceConnected = deviceConnected;
        }
        // connecting
        if (deviceConnected && !oldDeviceConnected)
        {
            // do stuff here on connecting
            // Serial.println("Connected");
            oldDeviceConnected = deviceConnected;
        }

        // elapsedMillis sinceCapture = 0;
        // const int captureWindow = 120 * SECONDS;

        // /* this loop just goes through all the features */
        // if (sinceCapture > captureWindow)
        // {
        //     Serial.println("capture");
        //     myTime = millis();
        //     shutterButton(pCharacteristicRx);
        //     vTaskDelay(inTicks(30));

        //     Serial.println("screen off");
        //     screenToggle(pCharacteristicRx);
        //     vTaskDelay(inTicks(5));

        //     Serial.println("screen on");
        //     screenToggle(pCharacteristicRx);
        //     vTaskDelay(inTicks(5));

        //     Serial.println("power off");
        //     powerOff(pCharacteristicRx);
        //     vTaskDelay(inTicks(30));

        //     Serial.println("power on");
        //     BLEDevice::stopAdvertising();
        //     powerOnPrevConnectedCameras();
        // }

        //     unsigned long currentMillis = millis();

        ButtonPacket *buttonPacket = nullptr;

        if (xQueuePeek(xButtonsQueue, &(buttonPacket), TICKS_50ms) == pdTRUE)
        {
            if (buttonPacket->id != buttonPacketId)
            {
                buttonPacketId = buttonPacket->id;

                handleButtonPacket(buttonPacket);
            }
        }
    }

    void powerOnPrevConnectedCameras()
    {
        /* the below might be camera specific and you may need to sniff them yourself for your camera */

        /* used for Insta360 X3 */
        manuf_data[14] = 0x37;
        manuf_data[15] = 0x4b;
        manuf_data[16] = 0x43;
        manuf_data[17] = 0x4d;
        manuf_data[18] = 0x54;
        manuf_data[19] = 0x4b;

        /* used for Insta360 RS 1-inch */
        // manuf_data[14] = 0x38;
        // manuf_data[15] = 0x51;
        // manuf_data[16] = 0x53;
        // manuf_data[17] = 0x4a;
        // manuf_data[18] = 0x38;
        // manuf_data[19] = 0x52;

        // BLEAdvertisementData advertisementData;
        // advertisementData.setName("Insta360 GPS Remote");
        // advertisementData.setFlags(ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_DMT_CONTROLLER_SPT | ESP_BLE_ADV_FLAG_DMT_HOST_SPT);
        // advertisementData.setManufacturerData((char *) manuf_data);

        // BLEAdvertising* advertisement = new BLEAdvertising();
        // advertisement->setAdvertisementData(advertisementData);
        // advertisement->start();

        // TODO
        // custom function that adds manufacturing data and modifies flags of advertisement
        // BLEDevice::startAdvertisingWithManufData(manuf_data);
    }

    void modeButton(BLECharacteristic *characteristic)
    {
        data_8[0] = 0xfc;
        data_8[1] = 0xef;
        data_8[2] = 0xfe;
        data_8[3] = 0x86;
        data_8[4] = 0x00;
        data_8[5] = 0x03;
        data_8[6] = 0x01;
        data_8[7] = 0x01;
        data_8[8] = 0x00;
        characteristic->setValue(data_8, 9);
        characteristic->notify();
        Serial.printf("Clicked MODE button\n");
    }

    void shutterButton(BLECharacteristic *characteristic)
    {
        data_8[0] = 0xfc;
        data_8[1] = 0xef;
        data_8[2] = 0xfe;
        data_8[3] = 0x86;
        data_8[4] = 0x00;
        data_8[5] = 0x03;
        data_8[6] = 0x01;
        data_8[7] = 0x02;
        data_8[8] = 0x00;
        characteristic->setValue(data_8, 9);
        characteristic->notify();
        Serial.printf("Clicked SHUTTER button\n");
    }

    void screenToggle(BLECharacteristic *characteristic)
    {
        data_8[0] = 0xfc;
        data_8[1] = 0xef;
        data_8[2] = 0xfe;
        data_8[3] = 0x86;
        data_8[4] = 0x00;
        data_8[5] = 0x03;
        data_8[6] = 0x01;
        data_8[7] = 0x00;
        data_8[8] = 0x00;
        characteristic->setValue(data_8, 9);
        characteristic->notify();
    }

    void powerOff(BLECharacteristic *characteristic)
    {
        data_8[0] = 0xfc;
        data_8[1] = 0xef;
        data_8[2] = 0xfe;
        data_8[3] = 0x86;
        data_8[4] = 0x00;
        data_8[5] = 0x03;
        data_8[6] = 0x01;
        data_8[7] = 0x00;
        data_8[8] = 0x03;
        characteristic->setValue(data_8, 9);
        characteristic->notify();
    }

    // void setup2()
    // {
    //     // Serial.begin(115200);

    //     vTaskDelay(TICKS_1s);

    //     BLEDevice::init("Insta360 GPS Remote");
    //     BLEScan *pBLEScan = BLEDevice::getScan();
    //     pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    //     pBLEScan->setInterval(1349);
    //     pBLEScan->setWindow(449);
    //     pBLEScan->setActiveScan(true);
    //     pBLEScan->start(5, false);
    //     // pinMode(RECORD_BUTTON_PIN, INPUT_PULLUP);
    //     // pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
    // }

    elapsedMillis sinceLastConnected = 0;

    // void loop2()
    // {
    //     unsigned long currentMillis = millis();

    //     ButtonPacket *buttonPacket = nullptr;

    //     if (xQueuePeek(xButtonsQueue, &(buttonPacket), TICKS_50ms) == pdTRUE)
    //     {
    //         if (buttonPacket->id != buttonPacketId)
    //         {
    //             buttonPacketId = buttonPacket->id;

    //             handleButtonPacket(buttonPacket);
    //         }
    //     }

    //     if (sinceLastConnected >= CONNECTION_CHECK_INTERVAL)
    //     {
    //         if (doConnect == true)
    //         {
    //             if (connectToServer())
    //             {
    //                 // displayConnected();
    //                 // delay(1000);
    //                 // displayPhoto();
    //                 Serial.println("We are now connected to the BLE Server.");
    //             }
    //             else
    //             {
    //                 Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    //             }
    //             doConnect = false;
    //         }

    //         // maybe maintain connection?
    //         if (connected)
    //         {
    //             if (isVideoRecording == true)
    //             {
    //                 send_cmd(0x04, COMMAND_some_data, NULL, 0);
    //                 // Serial.printf("Maintaining connection?\n");
    //             }
    //             //  read_serial();
    //         }
    //         else if (doScan)
    //         {
    //             Serial.printf("Scanning....\n");
    //             BLEDevice::getScan()->start(0);
    //         }
    //     }
    // }

    // bool connectToServer()
    // {
    //     Serial.printf("Forming a connection to %s\n", myDevice->getAddress().toString().c_str());
    //     BLEClient *pClient = BLEDevice::createClient();
    //     pClient->setClientCallbacks(new MyClientCallback());
    //     pClient->connect(myDevice);
    //     Serial.println("Connected to server");
    //     pClient->setMTU(517);

    //     BLERemoteService *pRemoteService = pClient->getService(serviceUUID_80);
    //     if (pRemoteService == nullptr)
    //     {
    //         Serial.print("Failed to find our service UUID: ");
    //         Serial.println(serviceUUID_80.toString().c_str());
    //         pClient->disconnect();
    //         return false;
    //     }
    //     Serial.println("Found our service");

    //     // characteristic read
    //     pRemoteReadCharacteristic = pRemoteService->getCharacteristic(readCharacteristicUUID);
    //     if (pRemoteReadCharacteristic == nullptr)
    //     {
    //         Serial.print("Failed to find read characteristic UUID: ");
    //         Serial.println(readCharacteristicUUID.toString().c_str());
    //         pClient->disconnect();
    //         return false;
    //     }
    //     Serial.println("Found our characteristic (read)");

    //     if (pRemoteReadCharacteristic->canRead())
    //     {
    //         std::string value = pRemoteReadCharacteristic->readValue();
    //         uint8_t res[100];
    //         int len = strlen(value.c_str());
    //         memcpy(res, value.c_str(), len);
    //         process_cmd(res, len);
    //     }
    //     if (pRemoteReadCharacteristic->canNotify())
    //     {
    //         pRemoteReadCharacteristic->registerForNotify(notifyReadCallback);
    //         Serial.printf("Registering for notify callback (read)\n");
    //     }

    //     // characteristic write
    //     pRemoteWriteCharacteristic = pRemoteService->getCharacteristic(writeCharacteristicUUID);
    //     if (pRemoteWriteCharacteristic == nullptr)
    //     {
    //         Serial.print("Failed to find write characteristic UUID: ");
    //         Serial.println(writeCharacteristicUUID.toString().c_str());
    //         pClient->disconnect();
    //         return false;
    //     }
    //     Serial.println("Found our characteristic (write)");

    //     if (pRemoteWriteCharacteristic->canRead())
    //     {
    //         std::string value = pRemoteWriteCharacteristic->readValue();
    //         uint8_t res[100];
    //         int len = strlen(value.c_str());
    //         memcpy(res, value.c_str(), len);
    //         process_cmd(res, len);
    //     }
    //     if (pRemoteWriteCharacteristic->canNotify())
    //     {
    //         pRemoteWriteCharacteristic->registerForNotify(notifyWriteCallback);
    //         Serial.printf("Registering for notify callback (write)\n");
    //     }

    //     connected = true;
    //     return true;
    // }

    // void process_cmd(uint8_t *pData, int length)
    // {
    //     Serial.print("Got: size=" + String(sizeof(pData)) + ", len=" + String(length) + ", data=");
    //     for (int i = 0; i < length; i++)
    //     {
    //         Serial.printf("%02X", pData[i]);
    //     }
    //     Serial.println();
    //     Serial.print("Got String: ");
    //     for (int i = 0; i < length; i++)
    //     {
    //         Serial.print(String(pData[i]));
    //         Serial.print(" ");
    //     }
    //     Serial.println();
    //     // decodeStatus(pData);
    //     decodeStatus2(pData, length);
    // }

    // void send_cmd(uint8_t mode, uint8_t c1, uint8_t *pb, int pb_len)
    // {
    //     static uint32_t sn = 5120;
    //     uint8_t cmd[2000];
    //     int len = create_cmd(cmd, &sn, mode, c1, pb, pb_len);
    //     pRemoteReadCharacteristic->writeValue(cmd, len);
    // }

    // // Shutter: FCEF-FE86–0003–0102–00
    // // Mode: FCEF-FE86–0003–0101–00
    // // Power (short-press): FCEF-FE86–0003–0100–00
    // // Power (3s-press): FCEF-FE86–0003–0100–03

    // int create_cmd(uint8_t *cmd, uint32_t *sn, uint8_t mode, uint8_t c1, uint8_t *pb, int pb_len)
    // {
    //     cmd[1] = 0;
    //     cmd[2] = 0;
    //     cmd[3] = 0;
    //     cmd[4] = mode;
    //     cmd[5] = 0;
    //     cmd[6] = 0;
    //     int len = 7;
    //     if (c1 != 0xFF)
    //     {
    //         cmd[7] = c1;
    //         cmd[8] = 0;
    //         cmd[9] = (uint8_t)(((*sn) & 0xFF00) >> 8);
    //         cmd[10] = (uint8_t)(((*sn) & 0x00FF));
    //         *sn += 1;
    //         cmd[11] = 0x00;
    //         cmd[12] = 0;
    //         cmd[13] = 0x80;
    //         cmd[14] = 0;
    //         cmd[15] = 0;
    //         len += 9;
    //     }
    //     for (int i = 0; i < pb_len; i++)
    //     {
    //         cmd[i + 16] = pb[i];
    //     }
    //     cmd[0] = len + pb_len;
    //     return cmd[0];
    // }

    // void decodeStatus(uint8_t *pData)
    // {

    //     if (sizeof(pData) == 2)
    //     {
    //         for (int i = 0; i < sizeof(aData); i++)
    //         {
    //             if (pData[1] == aData[i])
    //             {
    //                 Serial.println("batt Percentage:" + String(i));
    //                 batteryPercentage = i;
    //                 break;
    //             }
    //         }
    //     }
    //     else if (sizeof(pData) == 5)
    //     {
    //         int node = 0;
    //         int multiplayer = 1;
    //         for (int i = 0; i < sizeof(aData); i++)
    //         {
    //             if (pData[1] == aData[i])
    //             {
    //                 node = i;
    //                 if (i >= 128)
    //                 {
    //                     for (int j = 0; j < sizeof(aData); j++)
    //                     {
    //                         if (pData[2] == aData[j])
    //                         {
    //                             multiplayer = j;
    //                         }
    //                     }
    //                 }
    //                 int seconds = (node * multiplayer) - ((multiplayer - 1) * (node - 128));
    //                 recordingTimeInSec = seconds;
    //                 Serial.println("Rec Time:" + String(recordingTimeInSec));
    //                 break;
    //             }
    //         }
    //     }
    // }

    // void decodeStatus2(uint8_t *pData, int length)
    // {
    //     // Serial.printf("length == %d\n", length);

    //     if (length == 2)
    //     {
    //         if (String(pData[0]) == "16")
    //         {
    //             // clearDisplayBattery();
    //             batteryPercentage = atoi(String(pData[1]).c_str());
    //             Serial.println("batt Percentage:" + String(batteryPercentage));
    //             // updateDisplayBattery();
    //         }
    //     }
    //     else if (length == 4 || length == 5)
    //     {
    //         int multiplayer = 1;
    //         if (String(pData[0]) == "16")
    //         {
    //             int value = atoi(String(pData[1]).c_str());
    //             if (length == 5)
    //             {
    //                 multiplayer = atoi(String(pData[2]).c_str());
    //             }
    //             int seconds = (value * multiplayer) - ((multiplayer - 1) * (value - 128));
    //             recordingTimeInSec = seconds;
    //             Serial.println("Rec Time:" + String(recordingTimeInSec));
    //             // updateRecordingTime();
    //         }
    //     }
    // }

    void handleButtonPacket(ButtonPacket *packet)
    {
        if (packet->button == ButtonOption::MAIN_BTN)
        {
            if (packet->event == CLICK)
            {
                shutterButton(pCharacteristicRx);
                // if (isVideoRecording)
                // {
                //     Serial.printf("Video is recording... stop recording\n");
                //     send_cmd(0x04, COMMAND_stop_video, NULL, 0);
                //     isVideoRecording = false;
                // }
                // else
                // {
                //     Serial.printf("Video is recording!\n");
                //     send_cmd(0x04, COMMAND_start_video_normal, NULL, 0);
                //     isVideoRecording = true;
                // }
            }
        }
        else if (packet->button == ButtonOption::ACC_BTN)
        {
            if (packet->event == CLICK)
            {
                // changeMode();
                // send_cmd(0x04, COMMAND_take_photo, NULL, 0);
                // send_cmd(0x04, COMMAND_App_mode, NULL, 0);
                // send_cmd(0x04, COMMAND_switch_close_mode, NULL, 0);
                modeButton(pCharacteristicRx);
            }
        }
    }

    // void sendCommand()
    // {
    //     if (connected)
    //     {
    //         if (isVideoRecording == false)
    //         {
    //             if (selectedMode == Insta360Mode::PHOTO)
    //             {
    //                 Serial.printf("Mode -> Photo\n");
    //                 send_cmd(0x04, COMMAND_take_photo, NULL, 0);
    //                 delay(2000);
    //             }
    //             else if (selectedMode == Insta360Mode::NORMALVIDEO)
    //             {
    //                 isVideoRecording = true;
    //                 Serial.printf("Mode -> NormalVideo\n");
    //                 send_cmd(0x04, COMMAND_start_video_normal, NULL, 0);
    //                 delay(2000);
    //             }
    //             else if (selectedMode == Insta360Mode::HDRVIDEO)
    //             {
    //                 isVideoRecording = true;
    //                 send_cmd(0x04, COMMAND_video_HDR, NULL, 0);
    //                 delay(2000);
    //                 Serial.printf("Mode -> HDRVideo\n");
    //             }
    //             else if (selectedMode == Insta360Mode::BULLETVIDEO)
    //             {
    //                 isVideoRecording = true;
    //                 send_cmd(0x04, 0x29, NULL, 0);
    //                 delay(2000);
    //                 Serial.printf("Mode -> BulletVideo\n");
    //             }
    //             else if (selectedMode == Insta360Mode::TIMESHIFTVIDEO)
    //             {
    //                 isVideoRecording = true;
    //                 send_cmd(0x04, COMMAND_timeshift_video, NULL, 0);
    //                 delay(2000);
    //                 Serial.printf("Mode -> TimeshiftVideo\n");
    //             }
    //             else if (selectedMode == Insta360Mode::LOOPVIDEO)
    //             {
    //                 isVideoRecording = true;
    //                 send_cmd(0x04, 0x45, NULL, 0);
    //                 delay(2000);
    //                 Serial.printf("Mode -> LoopVideo\n");
    //             }
    //         }
    //         // else
    //         // {
    //         //     modeShiftBtnClick = true;
    //         //     isVideoRecording = false;
    //         //     send_cmd(0x04, 0x34, NULL, 0);
    //         // }
    //     }
    // }

    // void changeMode()
    // {
    //     if (isVideoRecording || connected == false)
    //     {
    //         Serial.printf("ERROR: recording %d or connected %d \n", isVideoRecording, connected);
    //         return;
    //     }

    //     if (selectedMode == Insta360Mode::TIMESHIFTVIDEO)
    //         selectedMode = Insta360Mode::PHOTO;
    //     selectedMode = selectedMode + 1;

    //     Serial.printf("Mode -> %s\n", getInsta360Mode(selectedMode));

    //     send_cmd(0x04, 0x34, NULL, 0);

    //     delay(250);
    // }
}