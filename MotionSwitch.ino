#include <Board.h>
#include <ToastBot.h>

#include "MotionSwitch.hpp"
#include "PirSensor.hpp"

// *****************************************************************************
//                                  Arduino
// *****************************************************************************

void setup()
{
   ToastBot::setup(new Esp8266Board());
}

void loop()
{
   ToastBot::loop();
}

