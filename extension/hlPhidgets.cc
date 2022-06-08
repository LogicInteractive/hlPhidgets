#define HL_NAME(n) hlPhidgets_##n

#include "hl.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <cstring>
#include <phidget22.h>

/////////////////////////////////////////////////////////////////////////////////////

#ifndef HL_PHIDGET_DATA
#define HL_PHIDGET_DATA

class PhidgetData
{

public:
	PhidgetData()
	{
	}

	~PhidgetData()
	{
		//dispose();
	}

	bool attached = false;
	int serial = 0;
	PhidgetReturnCode ret;
	PhidgetHandle handle;

private:
	PhidgetReturnCode errorCode;
	const char* errorString;
	char errorDetail[100];
	size_t errorDetailLen = 100;

	static void CCONV onAttach(PhidgetHandle ch, void* ctx)
	{
		if (ctx)
		{
			PhidgetData* ph = (PhidgetData*)ctx;
			ph->attached = true;
		}
	}

	static void CCONV onDetach(PhidgetData ch, void* ctx)
	{
		if (ctx)
		{
			PhidgetData* ph = (PhidgetData*)ctx;
			ph->attached = false;
		}
	}
	
public:
	// void setOnAttachHandler()
	// {
	// 	if (digitalInput)	
	// 		Phidget_setOnAttachHandler((PhidgetHandle)digitalInput, onAttach, this);
	// }

	// void setOnDetachHandler()
	// {
	// 	if (digitalInput)	
	// 		Phidget_setOnDetachHandler((PhidgetHandle)digitalInput, onAttach, this);
	// }

	void dispose()
	{
	}
};

/////////////////////////////////////////////////////////////////////////////////////

HL_PRIM bool HL_NAME(hlphidget_setDeviceSerialNumber)(PhidgetData* phd, int serial)
{
	if (!phd || phd->handle==NULL)
		return false;
	return Phidget_setDeviceSerialNumber((PhidgetHandle)phd->handle, serial) == EPHIDGET_OK;
}

HL_PRIM bool HL_NAME(hlphidget_open)(PhidgetData* phd)
{
	if (!phd || phd->handle==NULL)
		return false;
	return Phidget_open((PhidgetHandle)phd->handle) == EPHIDGET_OK;
}

HL_PRIM bool HL_NAME(hlphidget_close)(PhidgetData* phd)
{
	if (!phd || phd->handle==NULL)
		return false;
	return Phidget_close((PhidgetHandle)phd->handle) == EPHIDGET_OK;
}

HL_PRIM void HL_NAME(hlphidget_delete)(PhidgetData* phd)
{
	if (!phd || phd->handle==NULL)
		return;
}

HL_PRIM bool HL_NAME(hlphidget_getIsAttached)(PhidgetData* phd)
{
	if (!phd)
		return false;
	return phd->attached;
}

/////////////////////////////////////////////////////////////////////////////////////

#define _PHIDGETDATA _ABSTRACT(PhidgetData)
#define _PHIDGETRETURNCODE _ABSTRACT(PhidgetReturnCode)

DEFINE_PRIM(_BOOL, hlphidget_setDeviceSerialNumber, _PHIDGETDATA _I32);
DEFINE_PRIM(_BOOL, hlphidget_open, _PHIDGETDATA);
DEFINE_PRIM(_BOOL, hlphidget_close, _PHIDGETDATA);
DEFINE_PRIM(_VOID, hlphidget_delete, _PHIDGETDATA);
DEFINE_PRIM(_BOOL, hlphidget_getIsAttached, _PHIDGETDATA);
//DEFINE_PRIM(_VOID, digitalOutput_setOnAttachHandler, _PHIDGETDIGITALOUTPUT);
//DEFINE_PRIM(_VOID, digitalOutput_setOnDetachHandler, _PHIDGETDIGITALOUTPUT);

#endif
