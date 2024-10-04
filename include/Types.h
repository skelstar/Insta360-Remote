#pragma once
#include <Arduino.h>

const TickType_t TICKS_1ms = 1 / portTICK_PERIOD_MS;
const TickType_t TICKS_5ms = 5 / portTICK_PERIOD_MS;
const TickType_t TICKS_10ms = 10 / portTICK_PERIOD_MS;
const TickType_t TICKS_50ms = 50 / portTICK_PERIOD_MS;
const TickType_t TICKS_100ms = 100 / portTICK_PERIOD_MS;
const TickType_t TICKS_500ms = 500 / portTICK_PERIOD_MS;
const TickType_t TICKS_1s = 1000 / portTICK_PERIOD_MS;
const TickType_t TICKS_2s = 2000 / portTICK_PERIOD_MS;

const uint16_t TWO_THIRDS_SECONDS = 750;
const uint16_t ONE_SECONDS = 1000;
const uint16_t ONE_AND_HALF_SECONDS = 1500;
const uint16_t TWO_SECONDS = 2000;
const uint16_t THREE_SECONDS = 3000;
const uint16_t FOUR_SECONDS = 4000;
const uint16_t FIVE_SECONDS = 5000;
const uint16_t SIX_SECONDS = 6000;
const uint16_t THIRTY_SECONDS = 30 * 1000;
const uint16_t SIXTY_SECONDS = 60 * 1000;
const uint16_t ONE_MINUTE = 60 * 1000;

enum ButtonEvent
{
    CLICK,
    LONGCLICK,
    DOUBLE_TAP,
    DETECTED,
    NOT_DETECTED,
};

class ButtonPacket
{
public:
    unsigned long id = -1;
    uint8_t button;
    uint8_t event;
};

enum ButtonOption
{
    ACC_BTN,
    MAIN_BTN,
    RST_BTN,
};

enum Insta360Mode
{
    PHOTO,
    NORMALVIDEO,
    HDRVIDEO,
    BULLETVIDEO,
    TIMESHIFTVIDEO,
    LOOPVIDEO
};

char *getInsta360Mode(uint8_t mode)
{
    switch (mode)
    {
    case PHOTO:
        return "PHOTO";
    case NORMALVIDEO:
        return "NORMALVIDEO";
    case HDRVIDEO:
        return "HDRVIDEO";
    case BULLETVIDEO:
        return "BULLETVIDEO";
    case TIMESHIFTVIDEO:
        return "TIMESHIFTVIDEO";
    case LOOPVIDEO:
        return "LOOPVIDEO";
    }
}

// Shutter: FCEF-FE86–0003–0102–00
// Mode: FCEF-FE86–0003–0101–00
// Power (short-press): FCEF-FE86–0003–0100–00
// Power (3s-press): FCEF-FE86–0003–0100–03

#define COMMAND_App_mode 0x01
#define COMMAND_switch_close_mode 0x02
#define COMMAND_take_photo 0x03
#define COMMAND_start_video_normal 0x04
#define COMMAND_stop_video 0x05
#define COMMAND_C8_response 0x0E
#define COMMAND_some_data 0x0F
#define COMMAND_whipe_SD 0x18
#define COMMAND_Periodic_message_activation 0x1A
#define COMMAND_F5_response 0x1F
#define COMMAND_reboot 0x20
#define COMMAND_some_data 0x21
#define COMMAND_some_data 0x22
#define COMMAND_24 , 25, 28 C8 0x23
#define COMMAND_video_bullet 0x29
#define COMMAND_stop_video 0x30
#define COMMAND_video_HDR 0x33
#define COMMAND_stop_video 0x34
#define COMMAND_factory_reset 0x39
#define COMMAND_timeshift_video 0x3D
#define COMMAND_stop_video_2 0x3E
