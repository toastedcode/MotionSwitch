#pragma once

#include "MotionSwitch.hpp"
#include "Adapter/HttpClientAdapter.hpp"
#include "Messaging/RestfulProtocol.hpp"

// *****************************************************************************
//                                  Public
// *****************************************************************************

const bool MotionSwitch::ON;
const bool MotionSwitch::OFF;

MotionSwitch::MotionSwitch(
   const String& id,
   const int& resetPeriod,
   const String& iftttKey,
   const String& onEvent,
   const String& offEvent) :
      Component(id),
      resetPeriod(resetPeriod),
      iftttKey(iftttKey),
      onEvent(onEvent),
      offEvent(offEvent),
      timer(0),
      adapter(0)
{
}

MotionSwitch::MotionSwitch(
    MessagePtr message) :
       Component(message),
       timer(0),
       adapter(0)
{
   resetPeriod = message->getInt("resetPeriod");
   iftttKey = message->getString("iftttKey");
   onEvent = message->getString("onEvent");
   offEvent = message->getString("offEvent");
}
    
MotionSwitch::~MotionSwitch()
{
   Timer::freeTimer(timer);
}

void MotionSwitch::setup()
{
   timer = Timer::newTimer("resetTimer", resetPeriod, Timer::ONE_SHOT, this);

   adapter = new HttpClientAdapter("httpAdapter", new RestfulProtocol());
}

void MotionSwitch::handleMessage(
   MessagePtr message)
{
   if (message->getMessageId() == "sensorReading")
   {
      int value = message->getInt("value");
      
      Logger::logDebug("MotionSwitch::handleMessage: sensor = %d", value);

      onSensorUpdate(value);

      Messaging::freeMessage(message);
   }
   else
   {
      Component::handleMessage(message);
   }
}

void MotionSwitch::timeout(
   Timer* timer)
{
   if (state == ON)
   {
      setState(OFF);
   }
}

// *****************************************************************************
//                                  Protected
// *****************************************************************************

void MotionSwitch::setState(
  const bool& state)
{
   if (this->state != state)
   {
      Logger::logDebug("MotionSwitch::setState: %s",
                       ((state == ON) ? "ON" : "OFF"));

      this->state = state;

      iftttRequest(state);
   }
}

void MotionSwitch::onSensorUpdate(
   int value)
{
   if (value == HIGH)
   {
      if (state == OFF)
      {
         setState(ON);
      }
  
      if (timer->isStarted())
      {
         timer->reset();
      }
      else
      {
         timer->start();
      }
   }
}

void MotionSwitch::iftttRequest(
   const bool& state) const
{
   String url = getIftttRequestUrl(state);

   MessagePtr message = Messaging::newMessage();

   if (!adapter)
   {
      Logger::logWarning("MotionSwitch::iftttRequest: Invalid adapter.");
   }
   else if (url.length() == 0)
   {
      Logger::logWarning("MotionSwitch::iftttRequest: Invalid URL.");
   }
   else if (!message)
   {
      Logger::logWarning("MotionSwitch::iftttRequest: Message pool exhausted.");
   }
   else
   {
      message->set("url", url);
      adapter->sendRemoteMessage(message);

      Logger::logDebug("MotionSwitch::iftttRequest: Sent %s request.", (state == ON) ? "ON" : "OFF");

      Messaging::freeMessage(message);
   }
}

String MotionSwitch::getIftttRequestUrl(
   const bool& state) const
{
   //https://maker.ifttt.com/trigger/<event id>/with/key/<key>?value1=<value>&value2=<value> ...

   String url = "";

   if (iftttKey.length() == 0)
   {
      Logger::logWarning("MotionSwitch::getIftttRequestUrl: Invalid IFTTT key.");
   }
   else
   {
      String eventId = (state == ON) ? onEvent : offEvent;
      url = "https://maker.ifttt.com/trigger/" + eventId + "/with/" + iftttKey;
   }

   return (url);
}
