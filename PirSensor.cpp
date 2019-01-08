// *****************************************************************************
// *****************************************************************************
// PirSensor.cpp
//
// Author: Jason Tost
// Date:   10.21.2016
//
// *****************************************************************************
// *****************************************************************************

#include "Board.hpp"
#include "PirSensor.hpp"

PirSensor::PirSensor(
   const String& id,
   const int& pin) :
      Sensor(id),
      pin(pin),
      value(0)
      
{
  // Nothing to do here.
}

PirSensor::PirSensor(
   MessagePtr message) :
      Sensor(message),
      value(0)
{
   pin = message->isSet("pin") ? message->getInt("pin") : 0;
}

PirSensor::~PirSensor()
{
}

void PirSensor::setup()
{
  Board::getBoard()->pinMode(pin, INPUT);
}

void PirSensor::loop()
{
   int oldValue = value;
   value = Board::getBoard()->digitalRead(pin);

   if (value != oldValue)
   {
      onChange();
   }

   /*
   static int i = 0;
   if (i > 3000)
   {
      i = 0;
      printf("%d\n", value);
   }
   else
   {
     i++;
   }
   */
}

int PirSensor::read()
{
   return (value);
}

void PirSensor::onPoll()
{
   MessagePtr message = Messaging::newMessage();
   if (message)
   {
     message->setTopic("sensorChange");
     message->setMessageId("sensorReading");
     message->setSource(getId());
     message->set("value", value);
     Messaging::publish(message);
   }
}

void PirSensor::onChange()
{
   onPoll();
}
