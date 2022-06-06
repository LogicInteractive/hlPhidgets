#define HL_NAME(n) hlPhidgets_##n

#include "hl.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <cstring>
#include <phidget22.h>

/////////////////////////////////////////////////////////////////////////////////////

class PhidgetDigitalOutput
{

public:
	PhidgetDigitalOutput(int channel)
	{
		create(channel);
	}

	~PhidgetDigitalOutput()
	{
		//dispose();
	}

	bool attached = false;
	int serial = 0;
	int channel = 0;
	int state = false;

private:
	PhidgetDigitalOutputHandle digitalOutput = NULL;
	PhidgetReturnCode ret;
	PhidgetReturnCode errorCode;
	const char* errorString;
	char errorDetail[100];
	size_t errorDetailLen = 100;

	static void CCONV onAttach(PhidgetHandle ch, void* ctx)
	{
		if (ctx)
		{
			PhidgetDigitalOutput* ph = (PhidgetDigitalOutput*)ctx;
			ph->attached = true;
		}
	}

	static void CCONV onDetach(PhidgetHandle ch, void* ctx)
	{
		if (ctx)
		{
			PhidgetDigitalOutput* ph = (PhidgetDigitalOutput*)ctx;
			ph->attached = false;
		}
	}
	
public:
	PhidgetReturnCode setDeviceSerialNumber(int serial)
	{
		this->serial = serial;
		if (digitalOutput)
			ret = Phidget_setDeviceSerialNumber((PhidgetHandle)digitalOutput, serial);
		return ret;
	}

	PhidgetReturnCode open()
	{
		if (digitalOutput)
			ret = Phidget_open((PhidgetHandle)digitalOutput);
		return ret;
	}
	
	PhidgetReturnCode close()
	{
		if (digitalOutput)
			ret = Phidget_close((PhidgetHandle)digitalOutput);
		return ret;
	}	

	void create(int channel)
	{
		this->channel = channel;
		PhidgetDigitalOutput_create(&digitalOutput);
		if (serial>0)
			ret = Phidget_setDeviceSerialNumber((PhidgetHandle)digitalOutput, serial);
		ret = Phidget_setChannel((PhidgetHandle)digitalOutput, channel);
		ret = PhidgetDigitalOutput_setDutyCycle(digitalOutput, 1);
	}

	PhidgetReturnCode setDigitalOutputState(bool state)
	{
		if (digitalOutput)
			ret = PhidgetDigitalOutput_setState(digitalOutput, state);
		return ret;
	}

	bool getDigitalOutputState()
	{
		int state;
		if (digitalOutput)
			ret = PhidgetDigitalOutput_getState(digitalOutput, &state);
		if (ret == 0)
			return state==1?true:false;
		else
			return false;
	}

	double getFrequency()
	{
		double frequency;
		ret = PhidgetDigitalOutput_getFrequency(digitalOutput, &frequency);
		return frequency;
	}

	void setFrequency(double frequency)
	{
		ret = PhidgetDigitalOutput_setFrequency(digitalOutput, frequency);
	}

	double getMinFrequency()
	{
		double maxFrequency;
		ret = PhidgetDigitalOutput_getMinFrequency(digitalOutput, &maxFrequency);
		return maxFrequency;
	}

	double getMaxFrequency()
	{
		double maxFrequency;
		ret = PhidgetDigitalOutput_getMaxFrequency(digitalOutput, &maxFrequency);
		// if (res != EPHIDGET_OK)
		// 	return 1; 
		return maxFrequency;
	}

	void _delete()
	{
		if (digitalOutput)
			PhidgetDigitalOutput_delete(&digitalOutput);
	}

	void setOnAttachHandler()
	{
		if (digitalOutput)	
			Phidget_setOnAttachHandler((PhidgetHandle)digitalOutput, onAttach, this);
	}

	void setOnDetachHandler()
	{
		if (digitalOutput)	
			Phidget_setOnDetachHandler((PhidgetHandle)digitalOutput, onAttach, this);
	}

	void dispose()
	{
		ret = Phidget_close((PhidgetHandle)digitalOutput);
		PhidgetDigitalOutput_delete(&digitalOutput);
	}
};

/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////

HL_PRIM PhidgetDigitalOutput* HL_NAME(digitalOutput_create)(int channel)
{
	PhidgetDigitalOutput* phidgetDigitalOutput = new PhidgetDigitalOutput(channel);
	return phidgetDigitalOutput;
}

HL_PRIM bool HL_NAME(digitalOutput_setDeviceSerialNumber)(PhidgetDigitalOutput* rfid, int serial)
{
	if (!rfid)
		return false;
	return rfid->setDeviceSerialNumber(serial) == 0;
}

HL_PRIM bool HL_NAME(digitalOutput_open)(PhidgetDigitalOutput* rfid)
{
	if (!rfid)
		return false;
	return rfid->open() == 0;
}

HL_PRIM bool HL_NAME(digitalOutput_close)(PhidgetDigitalOutput* rfid)
{
	if (!rfid)
		return false;
	return rfid->close() == 0;
}

HL_PRIM void HL_NAME(digitalOutput_delete)(PhidgetDigitalOutput* rfid)
{
	if (!rfid)
		return;
	rfid->_delete();
}

HL_PRIM void HL_NAME(digitalOutput_setOnAttachHandler)(PhidgetDigitalOutput* rfid)
{
	if (!rfid)
		return;
	rfid->setOnAttachHandler();
}

HL_PRIM void HL_NAME(digitalOutput_setOnDetachHandler)(PhidgetDigitalOutput* rfid)
{
	if (!rfid)
		return;
	rfid->setOnDetachHandler();
}

HL_PRIM bool HL_NAME(digitalOutput_getIsAttached)(PhidgetDigitalOutput* rfid)
{
	if (!rfid)
		return false;
	return rfid->attached;
}

HL_PRIM bool HL_NAME(digitalOutput_setDigitalOutputState)(PhidgetDigitalOutput* rfid, int channel, bool state)
{
	if (!rfid)
		return false;
	return rfid->setDigitalOutputState(state) == 0;
}

HL_PRIM bool HL_NAME(digitalOutput_getDigitalOutputState)(PhidgetDigitalOutput* rfid, int channel)
{
	if (!rfid)
		return false;
	return rfid->getDigitalOutputState();
}

HL_PRIM double HL_NAME(digitalOutput_getMinFrequency)(PhidgetDigitalOutput* rfid)
{
	if (!rfid)
		return false;
	return rfid->getMinFrequency();
}

HL_PRIM double HL_NAME(digitalOutput_getMaxFrequency)(PhidgetDigitalOutput* rfid)
{
	if (!rfid)
		return false;
	return rfid->getMaxFrequency();
}

HL_PRIM double HL_NAME(digitalOutput_getFrequency)(PhidgetDigitalOutput* rfid)
{
	if (!rfid)
		return false;
	return rfid->getFrequency();
}

HL_PRIM void HL_NAME(digitalOutput_setFrequency)(PhidgetDigitalOutput* rfid, double frequency)
{
	if (!rfid)
		return;
	rfid->setFrequency(frequency);
}

/////////////////////////////////////////////////////////////////////////////////////

#define _PHIDGETDIGITALOUTPUT _ABSTRACT(PhidgetDigitalOutput)
#define _PHIDGETRETURNCODE _ABSTRACT(PhidgetReturnCode)

DEFINE_PRIM(_PHIDGETDIGITALOUTPUT, digitalOutput_create, _I32);
DEFINE_PRIM(_BOOL, digitalOutput_setDeviceSerialNumber, _PHIDGETDIGITALOUTPUT _I32);
DEFINE_PRIM(_BOOL, digitalOutput_open, _PHIDGETDIGITALOUTPUT);
DEFINE_PRIM(_BOOL, digitalOutput_close, _PHIDGETDIGITALOUTPUT);
DEFINE_PRIM(_VOID, digitalOutput_delete, _PHIDGETDIGITALOUTPUT);
DEFINE_PRIM(_VOID, digitalOutput_setOnAttachHandler, _PHIDGETDIGITALOUTPUT);
DEFINE_PRIM(_VOID, digitalOutput_setOnDetachHandler, _PHIDGETDIGITALOUTPUT);
DEFINE_PRIM(_BOOL, digitalOutput_getIsAttached, _PHIDGETDIGITALOUTPUT);
DEFINE_PRIM(_BOOL, digitalOutput_setDigitalOutputState, _PHIDGETDIGITALOUTPUT _I32 _BOOL);
DEFINE_PRIM(_BOOL, digitalOutput_getDigitalOutputState, _PHIDGETDIGITALOUTPUT _I32);
DEFINE_PRIM(_F64, digitalOutput_getMinFrequency, _PHIDGETDIGITALOUTPUT);
DEFINE_PRIM(_F64, digitalOutput_getMaxFrequency, _PHIDGETDIGITALOUTPUT);
DEFINE_PRIM(_F64, digitalOutput_getFrequency, _PHIDGETDIGITALOUTPUT);
DEFINE_PRIM(_VOID, digitalOutput_setFrequency, _PHIDGETDIGITALOUTPUT _F64);