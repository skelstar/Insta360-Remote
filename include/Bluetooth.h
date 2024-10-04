#pragma once
#include <Arduino.h>
#include "BLEDevice.h"

namespace Bluetooth
{
    int create_cmd(uint8_t *cmd, uint32_t *sn, uint8_t mode, uint8_t c1, uint8_t *pb, int pb_len);
    void process_cmd(uint8_t *pData, int length);
    void send_cmd(uint8_t mode, uint8_t c1, uint8_t *pb, int pb_len);
    void decodeStatus2(uint8_t *pData, int length);
    bool connectToServer();
    void handleButtonPacket(ButtonPacket *packet);
    void changeMode();

    // static BLEUUID serviceUUID_80_old("0000be80-0000-1000-8000-00805f9b34fb");
    static BLEUUID serviceUUID_80("0000be80-0000-1000-8000-00805f9b34fb");
    static BLEUUID readCharacteristicUUID("0000be81-0000-1000-8000-00805f9b34fb");
    static BLEUUID writeCharacteristicUUID("0000be82-0000-1000-8000-00805f9b34fb");

    // IXSE43CNQHWD8K (X2)

    static bool doConnect = false;
    static bool connected = false;
    static bool doScan = false;
    static BLERemoteCharacteristic *pRemoteReadCharacteristic;
    static BLERemoteCharacteristic *pRemoteWriteCharacteristic;
    static BLEAdvertisedDevice *myDevice;

    uint8_t data_8[30];

    uint8_t aData[256] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x1f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};

    bool onReconnect = false;
    static void notifyReadCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify)
    {
        static uint8_t data[10];
        if (memcmp(data, pData, length) != 0)
        {
            memcpy(data, pData, length);
            Serial.printf("--------- notify! (read) --------- \n");
            process_cmd(pData, length);
        }
    }

    static void notifyWriteCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify)
    {
        static uint8_t data[10];
        if (memcmp(data, pData, length) != 0)
        {
            memcpy(data, pData, length);
            Serial.printf("--------- notify! (write) --------- \n");
            process_cmd(pData, length);
        }
    }

    class MyClientCallback : public BLEClientCallbacks
    {
        void onConnect(BLEClient *pclient)
        {
            Serial.println("--------- onConnect ---------");
        }

        void onDisconnect(BLEClient *pclient)
        {
            connected = false;
            Serial.println("--------- onDisconnect ---------");
            onReconnect = true;
        }
    };

    class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
    {
        void onResult(BLEAdvertisedDevice advertisedDevice)
        {
            Serial.printf("----------------------------\n");
            Serial.printf("BLE Advertised Device found: %s\n", advertisedDevice.toString().c_str());
            if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID_80))
            {
                onReconnect == false;
                BLEDevice::getScan()->stop();
                myDevice = new BLEAdvertisedDevice(advertisedDevice);
                doConnect = true;
                doScan = true;
            }
            Serial.printf("----------------------------\n");
        }
    };

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define CONNECTION_CHECK_INTERVAL 1000
#define RECONNECT_INTERVAL 6000
#define MODE_BUTTON_PIN 18
#define RECORD_BUTTON_PIN 19
    // unsigned long connectionCheckPreviousMillis = 0;
    // unsigned long reconnectCheckPreviousMillis = 0;

    bool isVideoRecording = false;
    int batteryPercentage = 100;
    int recordingTimeInSec = 0;
    int selectedMode = 1;
    bool modeShiftBtnClick = false;

    unsigned long buttonPacketId = -1;

    void setup()
    {
        // Serial.begin(115200);

        vTaskDelay(TICKS_1s);

        BLEDevice::init("Insta360 GPS Remote");
        BLEScan *pBLEScan = BLEDevice::getScan();
        pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
        pBLEScan->setInterval(1349);
        pBLEScan->setWindow(449);
        pBLEScan->setActiveScan(true);
        pBLEScan->start(5, false);
        // pinMode(RECORD_BUTTON_PIN, INPUT_PULLUP);
        // pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
    }

    elapsedMillis sinceLastConnected = 0;

    void loop()
    {
        unsigned long currentMillis = millis();

        ButtonPacket *buttonPacket = nullptr;

        if (xQueuePeek(xButtonsQueue, &(buttonPacket), TICKS_50ms) == pdTRUE)
        {
            if (buttonPacket->id != buttonPacketId)
            {
                buttonPacketId = buttonPacket->id;

                handleButtonPacket(buttonPacket);
            }
        }

        if (sinceLastConnected >= CONNECTION_CHECK_INTERVAL)
        {
            if (doConnect == true)
            {
                if (connectToServer())
                {
                    // displayConnected();
                    // delay(1000);
                    // displayPhoto();
                    Serial.println("We are now connected to the BLE Server.");
                }
                else
                {
                    Serial.println("We have failed to connect to the server; there is nothin more we will do.");
                }
                doConnect = false;
            }

            // maybe maintain connection?
            if (connected)
            {
                if (isVideoRecording == true)
                {
                    send_cmd(0x04, COMMAND_some_data, NULL, 0);
                    // Serial.printf("Maintaining connection?\n");
                }
                //  read_serial();
            }
            else if (doScan)
            {
                Serial.printf("Scanning....\n");
                BLEDevice::getScan()->start(0);
            }
        }
    }

    bool connectToServer()
    {
        Serial.printf("Forming a connection to %s\n", myDevice->getAddress().toString().c_str());
        BLEClient *pClient = BLEDevice::createClient();
        pClient->setClientCallbacks(new MyClientCallback());
        pClient->connect(myDevice);
        Serial.println("Connected to server");
        pClient->setMTU(517);

        BLERemoteService *pRemoteService = pClient->getService(serviceUUID_80);
        if (pRemoteService == nullptr)
        {
            Serial.print("Failed to find our service UUID: ");
            Serial.println(serviceUUID_80.toString().c_str());
            pClient->disconnect();
            return false;
        }
        Serial.println("Found our service");

        // characteristic read
        pRemoteReadCharacteristic = pRemoteService->getCharacteristic(readCharacteristicUUID);
        if (pRemoteReadCharacteristic == nullptr)
        {
            Serial.print("Failed to find read characteristic UUID: ");
            Serial.println(readCharacteristicUUID.toString().c_str());
            pClient->disconnect();
            return false;
        }
        Serial.println("Found our characteristic (read)");

        if (pRemoteReadCharacteristic->canRead())
        {
            std::string value = pRemoteReadCharacteristic->readValue();
            uint8_t res[100];
            int len = strlen(value.c_str());
            memcpy(res, value.c_str(), len);
            process_cmd(res, len);
        }
        if (pRemoteReadCharacteristic->canNotify())
        {
            pRemoteReadCharacteristic->registerForNotify(notifyReadCallback);
            Serial.printf("Registering for notify callback (read)\n");
        }

        // characteristic write
        pRemoteWriteCharacteristic = pRemoteService->getCharacteristic(writeCharacteristicUUID);
        if (pRemoteWriteCharacteristic == nullptr)
        {
            Serial.print("Failed to find write characteristic UUID: ");
            Serial.println(writeCharacteristicUUID.toString().c_str());
            pClient->disconnect();
            return false;
        }
        Serial.println("Found our characteristic (write)");

        if (pRemoteWriteCharacteristic->canRead())
        {
            std::string value = pRemoteWriteCharacteristic->readValue();
            uint8_t res[100];
            int len = strlen(value.c_str());
            memcpy(res, value.c_str(), len);
            process_cmd(res, len);
        }
        if (pRemoteWriteCharacteristic->canNotify())
        {
            pRemoteWriteCharacteristic->registerForNotify(notifyWriteCallback);
            Serial.printf("Registering for notify callback (write)\n");
        }

        connected = true;
        return true;
    }

    void process_cmd(uint8_t *pData, int length)
    {
        Serial.print("Got: size=" + String(sizeof(pData)) + ", len=" + String(length) + ", data=");
        for (int i = 0; i < length; i++)
        {
            Serial.printf("%02X", pData[i]);
        }
        Serial.println();
        Serial.print("Got String: ");
        for (int i = 0; i < length; i++)
        {
            Serial.print(String(pData[i]));
            Serial.print(" ");
        }
        Serial.println();
        // decodeStatus(pData);
        decodeStatus2(pData, length);
    }

    void send_cmd(uint8_t mode, uint8_t c1, uint8_t *pb, int pb_len)
    {
        static uint32_t sn = 5120;
        uint8_t cmd[2000];
        int len = create_cmd(cmd, &sn, mode, c1, pb, pb_len);
        pRemoteReadCharacteristic->writeValue(cmd, len);
    }

    // Shutter: FCEF-FE86–0003–0102–00
    // Mode: FCEF-FE86–0003–0101–00
    // Power (short-press): FCEF-FE86–0003–0100–00
    // Power (3s-press): FCEF-FE86–0003–0100–03

    int create_cmd(uint8_t *cmd, uint32_t *sn, uint8_t mode, uint8_t c1, uint8_t *pb, int pb_len)
    {
        cmd[1] = 0;
        cmd[2] = 0;
        cmd[3] = 0;
        cmd[4] = mode;
        cmd[5] = 0;
        cmd[6] = 0;
        int len = 7;
        if (c1 != 0xFF)
        {
            cmd[7] = c1;
            cmd[8] = 0;
            cmd[9] = (uint8_t)(((*sn) & 0xFF00) >> 8);
            cmd[10] = (uint8_t)(((*sn) & 0x00FF));
            *sn += 1;
            cmd[11] = 0x00;
            cmd[12] = 0;
            cmd[13] = 0x80;
            cmd[14] = 0;
            cmd[15] = 0;
            len += 9;
        }
        for (int i = 0; i < pb_len; i++)
        {
            cmd[i + 16] = pb[i];
        }
        cmd[0] = len + pb_len;
        return cmd[0];
    }

    void decodeStatus(uint8_t *pData)
    {

        if (sizeof(pData) == 2)
        {
            for (int i = 0; i < sizeof(aData); i++)
            {
                if (pData[1] == aData[i])
                {
                    Serial.println("batt Percentage:" + String(i));
                    batteryPercentage = i;
                    break;
                }
            }
        }
        else if (sizeof(pData) == 5)
        {
            int node = 0;
            int multiplayer = 1;
            for (int i = 0; i < sizeof(aData); i++)
            {
                if (pData[1] == aData[i])
                {
                    node = i;
                    if (i >= 128)
                    {
                        for (int j = 0; j < sizeof(aData); j++)
                        {
                            if (pData[2] == aData[j])
                            {
                                multiplayer = j;
                            }
                        }
                    }
                    int seconds = (node * multiplayer) - ((multiplayer - 1) * (node - 128));
                    recordingTimeInSec = seconds;
                    Serial.println("Rec Time:" + String(recordingTimeInSec));
                    break;
                }
            }
        }
    }

    void decodeStatus2(uint8_t *pData, int length)
    {
        // Serial.printf("length == %d\n", length);

        if (length == 2)
        {
            if (String(pData[0]) == "16")
            {
                // clearDisplayBattery();
                batteryPercentage = atoi(String(pData[1]).c_str());
                Serial.println("batt Percentage:" + String(batteryPercentage));
                // updateDisplayBattery();
            }
        }
        else if (length == 4 || length == 5)
        {
            int multiplayer = 1;
            if (String(pData[0]) == "16")
            {
                int value = atoi(String(pData[1]).c_str());
                if (length == 5)
                {
                    multiplayer = atoi(String(pData[2]).c_str());
                }
                int seconds = (value * multiplayer) - ((multiplayer - 1) * (value - 128));
                recordingTimeInSec = seconds;
                Serial.println("Rec Time:" + String(recordingTimeInSec));
                // updateRecordingTime();
            }
        }
    }

    void handleButtonPacket(ButtonPacket *packet)
    {
        if (packet->button == ButtonOption::MAIN_BTN)
        {
            if (packet->event == CLICK)
            {
                if (isVideoRecording)
                {
                    Serial.printf("Video is recording... stop recording\n");
                    send_cmd(0x04, COMMAND_stop_video, NULL, 0);
                    isVideoRecording = false;
                }
                else
                {
                    Serial.printf("Video is recording!\n");
                    send_cmd(0x04, COMMAND_start_video_normal, NULL, 0);
                    isVideoRecording = true;
                }
            }
        }
        else if (packet->button == ButtonOption::ACC_BTN)
        {
            // changeMode();
            send_cmd(0x04, COMMAND_take_photo, NULL, 0);
            // send_cmd(0x04, COMMAND_App_mode, NULL, 0);
            // send_cmd(0x04, COMMAND_switch_close_mode, NULL, 0);
        }
    }

    void sendCommand()
    {
        if (connected)
        {
            if (isVideoRecording == false)
            {
                if (selectedMode == Insta360Mode::PHOTO)
                {
                    Serial.printf("Mode -> Photo\n");
                    send_cmd(0x04, COMMAND_take_photo, NULL, 0);
                    delay(2000);
                }
                else if (selectedMode == Insta360Mode::NORMALVIDEO)
                {
                    isVideoRecording = true;
                    Serial.printf("Mode -> NormalVideo\n");
                    send_cmd(0x04, COMMAND_start_video_normal, NULL, 0);
                    delay(2000);
                }
                else if (selectedMode == Insta360Mode::HDRVIDEO)
                {
                    isVideoRecording = true;
                    send_cmd(0x04, COMMAND_video_HDR, NULL, 0);
                    delay(2000);
                    Serial.printf("Mode -> HDRVideo\n");
                }
                else if (selectedMode == Insta360Mode::BULLETVIDEO)
                {
                    isVideoRecording = true;
                    send_cmd(0x04, 0x29, NULL, 0);
                    delay(2000);
                    Serial.printf("Mode -> BulletVideo\n");
                }
                else if (selectedMode == Insta360Mode::TIMESHIFTVIDEO)
                {
                    isVideoRecording = true;
                    send_cmd(0x04, COMMAND_timeshift_video, NULL, 0);
                    delay(2000);
                    Serial.printf("Mode -> TimeshiftVideo\n");
                }
                else if (selectedMode == Insta360Mode::LOOPVIDEO)
                {
                    isVideoRecording = true;
                    send_cmd(0x04, 0x45, NULL, 0);
                    delay(2000);
                    Serial.printf("Mode -> LoopVideo\n");
                }
            }
            // else
            // {
            //     modeShiftBtnClick = true;
            //     isVideoRecording = false;
            //     send_cmd(0x04, 0x34, NULL, 0);
            // }
        }
    }

    void changeMode()
    {
        if (isVideoRecording || connected == false)
        {
            Serial.printf("ERROR: recording %d or connected %d \n", isVideoRecording, connected);
            return;
        }

        if (selectedMode == Insta360Mode::TIMESHIFTVIDEO)
            selectedMode = Insta360Mode::PHOTO;
        selectedMode = selectedMode + 1;

        Serial.printf("Mode -> %s\n", getInsta360Mode(selectedMode));

        send_cmd(0x04, 0x34, NULL, 0);

        delay(250);
    }
}