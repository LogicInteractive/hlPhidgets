#define HL_NAME(n) hlPhidgets_##n

#include "hl.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <cstring>
#include <phidget22.h>

/////////////////////////////////////////////////////////////////////////////////////

class PhidgetDigitalInput
{

public:
	PhidgetDigitalInput(int channel)
	{
		this->channel = channel;
		PhidgetDigitalInput_create(&handle);
		ret = Phidget_setChannel((PhidgetHandle)handle, channel);
	}

	~PhidgetDigitalInput()
	{
		//dispose();
	}

	bool attached = false;
	int serial = 0;
	int channel = 0;
	bool state = false;
	PhidgetDigitalInputHandle handle = NULL;
	PhidgetReturnCode ret;

private:
	PhidgetReturnCode errorCode;
	const char* errorString;
	char errorDetail[100];
	size_t errorDetailLen = 100;

	static void CCONV onStateChange(PhidgetDigitalInputHandle ch, void* ctx, int state)
	{
		if (ctx)
		{
			PhidgetDigitalInput* ph = (PhidgetDigitalInput*)ctx;
			ph->state = state==1?true:false;
		}
	}	

	static void CCONV onAttach(PhidgetHandle ch, void* ctx)
	{
		if (ctx)
		{
			PhidgetDigitalInput* ph = (PhidgetDigitalInput*)ctx;
			ph->attached = true;
		}
	}

	static void CCONV onDetach(PhidgetHandle ch, void* ctx)
	{
		if (ctx)
		{
			PhidgetDigitalInput* ph = (PhidgetDigitalInput*)ctx;
			ph->attached = false;
		}
	}
	
public:
	void setOnAttachHandler()
	{
		if (handle)	
			Phidget_setOnAttachHandler((PhidgetHandle)handle, onAttach, this);
	}

	void setOnDetachHandler()
	{
		if (handle)	
			Phidget_setOnDetachHandler((PhidgetHandle)handle, onAttach, this);
	}

	void setOnStateChangeHandler()
	{
		if (handle)	
			PhidgetDigitalInput_setOnStateChangeHandler(handle, onStateChange, this);
	}
};

/////////////////////////////////////////////////////////////////////////////////////

HL_PRIM bool HL_NAME(digitalInput_setDeviceSerialNumber)(PhidgetDigitalInput* phc, int serial)
{
	if (!phc)
		return false;
	phc->serial = serial;
	return Phidget_setDeviceSerialNumber((PhidgetHandle)phc->handle, serial) == EPHIDGET_OK;
}

HL_PRIM PhidgetDigitalInput* HL_NAME(digitalInput_create)(int channel, int serial = -1)
{
	PhidgetDigitalInput* phidgetDigitalInput = new PhidgetDigitalInput(channel);
	if (serial > 0)
		Phidget_setDeviceSerialNumber((PhidgetHandle)phidgetDigitalInput, serial);
	return phidgetDigitalInput;
}

HL_PRIM bool HL_NAME(digitalInput_open)(PhidgetDigitalInput* phc)
{
	if (!phc)
		return false;

	if (phc->handle)
		return Phidget_open((PhidgetHandle)phc->handle) == EPHIDGET_OK;
	return false;
}

HL_PRIM bool HL_NAME(digitalInput_close)(PhidgetDigitalInput* phc)
{
	if (!phc)
		return false;
	if (phc->handle)
		return Phidget_close((PhidgetHandle)phc->handle) == EPHIDGET_OK;
	return false;
}

HL_PRIM void HL_NAME(digitalInput_delete)(PhidgetDigitalInput* phc)
{
	if (!phc)
		return;
	PhidgetDigitalInput_delete(&phc->handle);
}

HL_PRIM void HL_NAME(digitalInput_setOnAttachHandler)(PhidgetDigitalInput* phc)
{
	if (!phc)
		return;
	phc->setOnAttachHandler();
}

HL_PRIM void HL_NAME(digitalInput_setOnDetachHandler)(PhidgetDigitalInput* phc)
{
	if (!phc)
		return;
	phc->setOnDetachHandler();
}

HL_PRIM bool HL_NAME(digitalInput_getIsAttached)(PhidgetDigitalInput* phc)
{
	if (!phc)
		return false;
	return phc->attached;
}

HL_PRIM void HL_NAME(digitalInput_setOnStateChangeHandler)(PhidgetDigitalInput* phc)
{
	if (!phc)
		return;
	phc->setOnStateChangeHandler();
}

HL_PRIM bool HL_NAME(digitalInput_getState)(PhidgetDigitalInput* phc)
{
	if (!phc)
		return false;
	return phc->state;
}

/////////////////////////////////////////////////////////////////////////////////////

#define _PHIDGETDIGITALINPUT _ABSTRACT(PhidgetDigitalInput)
#define _PHIDGETRETURNCODE _ABSTRACT(PhidgetReturnCode)

DEFINE_PRIM(_PHIDGETDIGITALINPUT, digitalInput_create, _I32 _I32);
DEFINE_PRIM(_BOOL, digitalInput_setDeviceSerialNumber, _PHIDGETDIGITALINPUT _I32);
DEFINE_PRIM(_BOOL, digitalInput_open, _PHIDGETDIGITALINPUT);
DEFINE_PRIM(_BOOL, digitalInput_close, _PHIDGETDIGITALINPUT);
DEFINE_PRIM(_VOID, digitalInput_delete, _PHIDGETDIGITALINPUT);
DEFINE_PRIM(_VOID, digitalInput_setOnAttachHandler, _PHIDGETDIGITALINPUT);
DEFINE_PRIM(_VOID, digitalInput_setOnDetachHandler, _PHIDGETDIGITALINPUT);
DEFINE_PRIM(_BOOL, digitalInput_getIsAttached, _PHIDGETDIGITALINPUT);
DEFINE_PRIM(_VOID, digitalInput_setOnStateChangeHandler, _PHIDGETDIGITALINPUT);
DEFINE_PRIM(_BOOL, digitalInput_getState, _PHIDGETDIGITALINPUT);