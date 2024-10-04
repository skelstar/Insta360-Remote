#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Button2.h>
#include <M5StickCPlus.h>

#include "Types.h"

namespace ButtonsTask
{
#define LONGCLICK_MS 1000

    TaskHandle_t taskHandle = NULL;
    const char *taskName = "ButtonsTask";

    // prototypes
    void sendButtonEvent(ButtonEvent event, ButtonOption button);

    ButtonPacket packet;
    unsigned long packetId;

    namespace ButtonAcc
    {
        Button2 button;

        void clickHandler(Button2 &btn)
        {
            sendButtonEvent(CLICK, ACC_BTN);
        }

        void longClickDetectedHandler(Button2 &btn)
        {
            sendButtonEvent(LONGCLICK, ACC_BTN);
        }

        void doubleTapHandler_cb(Button2 &btn)
        {
            sendButtonEvent(DOUBLE_TAP, ACC_BTN);
        }

        void initialise()
        {
            button.begin(ACC_BUTTON_PIN);
            button.setLongClickTime(LONGCLICK_MS);
            button.setDoubleClickTime(500); // default: BTN_DOUBLECLICK_MS = 300

            // handlers
            button.setClickHandler(&clickHandler);
            button.setLongClickDetectedHandler(&longClickDetectedHandler);
            button.setDoubleClickHandler(&doubleTapHandler_cb);
        }
    }

    namespace ButtonMain
    {
        Button2 button;

        void clickHandler(Button2 &btn)
        {
            sendButtonEvent(CLICK, MAIN_BTN);
        }

        void longClickDetectedHandler(Button2 &btn)
        {
            sendButtonEvent(LONGCLICK, MAIN_BTN);
        }

        void doubleTapHandler_cb(Button2 &btn)
        {
            sendButtonEvent(DOUBLE_TAP, MAIN_BTN);
        }

        void initialise()
        {
            button.begin(MAIN_BUTTON_PIN);
            button.setLongClickTime(LONGCLICK_MS);
            button.setDoubleClickTime(500); // default: BTN_DOUBLECLICK_MS = 300

            // handlers
            button.setClickHandler(&clickHandler);
            button.setLongClickDetectedHandler(&longClickDetectedHandler);
            button.setDoubleClickHandler(&doubleTapHandler_cb);
        }
    }

    namespace ButtonRst
    {
        void clickHandler()
        {
            sendButtonEvent(CLICK, RST_BTN);
        }

        void longClickDetectedHandler()
        {
            sendButtonEvent(LONGCLICK, RST_BTN);
        }

        void loop()
        {
            switch (M5.Axp.GetBtnPress())
            {
            case 0x02:
                clickHandler();
                break;
            case 0x01:
                longClickDetectedHandler();
                break;
            }
        }

        void initialise()
        {
        }
    }

    void task1(void *pvParameters)
    {
        Serial.println("ButtonsTask: Started");

        ButtonMain::initialise();
        ButtonAcc::initialise();
        ButtonRst::initialise();

        while (1)
        {
            ButtonMain::button.loop();
            ButtonAcc::button.loop();
            ButtonRst::loop();

            vTaskDelay(TICKS_5ms);
        }
    }

    void sendButtonEvent(ButtonEvent event, ButtonOption button)
    {
        packet.id++;
        packet.event = event;
        packet.button = button;

        ButtonPacket *data;
        data = &packet;
        xQueueSendToFront(xButtonsQueue, (void *)&data, TICKS_10ms);
    }

    void createTask(int stackDepth)
    {
        xTaskCreatePinnedToCore(
            task1,
            taskName,
            /*stack depth*/ stackDepth,
            /*params*/ NULL,
            /*priority*/ 1,
            &taskHandle,
            /*core*/ 1);
    }
}
