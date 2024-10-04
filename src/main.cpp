#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <M5StickCPlus.h>

#include "Types.h"

#include "elapsedMillis.h"

// Queues
QueueHandle_t xBluetoothQueue;
QueueHandle_t xButtonsQueue;
// QueueHandle_t xCommandQueue;
// QueueHandle_t xClipDetectQueue;

#include "Bluetooth.h"
#include "Tasks/ButtonsTask.h"

void setup()
{
    vTaskDelay(TICKS_1s);

    M5.begin(/*lcd*/ false, /*power*/ true, /*serial*/ false);

    Serial.begin(115200);

    Bluetooth::setup();

    // xBluetoothQueue = xQueueCreate(1, sizeof(Bluetooth::Packet *));
    xButtonsQueue = xQueueCreate(1, sizeof(ButtonPacket *));
    // xCommandQueue = xQueueCreate(1, sizeof(Command *));
    // xClipDetectQueue = xQueueCreate(1, sizeof(ClipDetectPacket *));

    // CommandCentre::createTask(2048);
    // BluetoothTask::createTask(4096);
    // Leds::createTask(8000);
    ButtonsTask::createTask(2048);
    // RedLedTask::createTask(2048);
    // DisplayTask::createTask(2048);
    // BuzzerTask::createTask(2048);
    // ClipDetectTask::createTask(2048);
    // AccelerometerTask::createTask(2048);
}

void loop()
{
    Bluetooth::loop();

    vTaskDelay(TICKS_10ms);
}