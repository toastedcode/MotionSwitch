// *****************************************************************************
// *****************************************************************************
// PirSensor.hpp
//
// Author: Jason Tost
// Date:   5.5.2018
//
// *****************************************************************************
// *****************************************************************************

#pragma once

#include "ComponentFactory.hpp"
#include "Component\Sensor.hpp"

class PirSensor : public Sensor
{

public:

   // Constructor.
   PirSensor(
      // A unique identifier for this sensor.
      const String& id,
      // The GPIO used to read the sensor.
      const int& pin);

   // Constructor.
   PirSensor(
      // A message containing the parameters to use in creating the component.
      MessagePtr message);

   // Destructor.
   virtual ~PirSensor();

   virtual void setup();

   virtual void loop();

   virtual int read();
   
protected:

   virtual void onPoll();

   virtual void onChange();

private:

   int pin;

   int sensitivity;  // time in millis before reacting to change

   int value;

   Timer* changeTimer;
};

REGISTER(PirSensor, pirsensor)
