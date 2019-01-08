#pragma once

#include "Adapter.hpp"
#include "Component.hpp"
#include "ComponentFactory.hpp"
#include "Messaging.hpp"
#include "Timer.hpp"

class MotionSwitch : public Component, public TimerListener
{

public:

   // Switch states.
   static const bool ON = true;
   static const bool OFF = false;

   // Constructor.
   MotionSwitch(
      const String& id,
      const int& resetPeriod,
      const String& iftttKey,
      const String& onEvent,
      const String& offEvent);

   // Constructor.
   MotionSwitch(
      // A message containing the parameters to use in creating the component.
      MessagePtr message);      

   // Destructor.
   virtual ~MotionSwitch();

   virtual void setup();

   // This operation handles a message directed to this sensor.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);

   virtual void timeout(
      Timer* timer);

protected:

   void setState(
     const bool& state);

   void onSensorUpdate(
      int value);

   void iftttRequest(
      const bool& state) const;

   String getIftttRequestUrl(
      const bool& state) const;

private:

   bool state;

   int resetPeriod;

   String iftttKey;

   String onEvent;
   
   String offEvent;

   Timer* timer;
   
   Adapter* adapter;
};

REGISTER(MotionSwitch, motionswitch)
