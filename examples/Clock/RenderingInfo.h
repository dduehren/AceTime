#ifndef CLOCK_RENDERING_INFO_H
#define CLOCK_RENDERING_INFO_H

#include <AceTime.h>
#include "config.h"

using namespace ace_time;
using namespace ace_time::hw;

static const uint8_t MODE_UNKNOWN = 0; // uninitialized
static const uint8_t MODE_DATE_TIME = 1;
#if DISPLAY_TYPE == DISPLAY_TYPE_LED || DISPLAY_TYPE == DISPLAY_TYPE_OLED
static const uint8_t MODE_HOUR_MINUTE = 2;
static const uint8_t MODE_MINUTE_SECOND = 3;
static const uint8_t MODE_YEAR = 4;
static const uint8_t MODE_MONTH = 5;
static const uint8_t MODE_DAY = 6;
#endif
static const uint8_t MODE_WEEKDAY = 7;
static const uint8_t MODE_TIME_ZONE = 8;
static const uint8_t MODE_TEMPERATURE = 9;
static const uint8_t MODE_CHANGE_YEAR = 10;
static const uint8_t MODE_CHANGE_MONTH = 11;
static const uint8_t MODE_CHANGE_DAY = 12;
static const uint8_t MODE_CHANGE_HOUR = 13;
static const uint8_t MODE_CHANGE_MINUTE = 14;
static const uint8_t MODE_CHANGE_SECOND = 15;
static const uint8_t MODE_CHANGE_TIME_ZONE_HOUR = 16;
static const uint8_t MODE_CHANGE_TIME_ZONE_MINUTE = 17;

/**
 * Data used by the Presenter (the "View") to determine what has changed and
 * what needs to be displayed.
 */ 
struct RenderingInfo {
  uint8_t mode = 0;
  DateTime dateTime;
  HardwareTemperature temperature;
  bool suppressBlink = false; // true if blinking should be suppressed
  bool blinkShowState = true; // true if should be rendered
};

#endif
