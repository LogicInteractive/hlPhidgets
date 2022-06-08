#define HL_NAME(n) hlPhidgets_##n

#include "hl.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <cstring>
#include <phidget22.h>

/////////////////////////////////////////////////////////////////////////////////////

class PhidgetRFID
{

public:
	PhidgetRFID()
	{
	}

	~PhidgetRFID()
	{
		//dispose();
	}

	bool attached = false;
	std::string currentTag;
	std::string previousTag;
	int serial = 0;
	PhidgetRFIDHandle rfid = NULL;
	PhidgetDigitalOutputHandle digitalOutput0 = NULL;
	PhidgetDigitalOutputHandle digitalOutput1 = NULL;
	PhidgetDigitalOutputHandle digitalOutput2 = NULL;
	PhidgetReturnCode ret;

private:
	PhidgetReturnCode errorCode;
	const char* errorString;
	char errorDetail[100];
	size_t errorDetailLen = 100;

	static void CCONV onTag(PhidgetRFIDHandle ch, void* ctx, const char* tag, PhidgetRFID_Protocol protocol)
	{
		if (ctx)
		{
			PhidgetRFID* ph = (PhidgetRFID*)ctx;
			ph->currentTag = std::string(tag);
		}
	}

	static void CCONV onTagLost(PhidgetRFIDHandle ch, void* ctx, const char* tag, PhidgetRFID_Protocol protocol)
	{
		if (ctx)
		{
			PhidgetRFID* ph = (PhidgetRFID*)ctx;
			ph->previousTag = ph->currentTag;
			ph->currentTag.clear();
		}
	}

	static void CCONV onAttach(PhidgetHandle ch, void* ctx)
	{
		if (ctx)
		{
			PhidgetRFID* ph = (PhidgetRFID*)ctx;
			ph->attached = true;
		}
	}

	static void CCONV onDetach(PhidgetHandle ch, void* ctx)
	{
		if (ctx)
		{
			PhidgetRFID* ph = (PhidgetRFID*)ctx;
			ph->attached = false;
		}
	}
	
public:
	PhidgetReturnCode setDeviceSerialNumber(int serial)
	{
		this->serial = serial;
		if (rfid)
			ret = Phidget_setDeviceSerialNumber((PhidgetHandle)rfid, serial);
		if (digitalOutput0)
			ret = Phidget_setDeviceSerialNumber((PhidgetHandle)digitalOutput0, serial);
		if (digitalOutput1)
			ret = Phidget_setDeviceSerialNumber((PhidgetHandle)digitalOutput1, serial);
		if (digitalOutput2)
			ret = Phidget_setDeviceSerialNumber((PhidgetHandle)digitalOutput2, serial);
		return ret;
	}

	PhidgetReturnCode open()
	{
		if (rfid)
			ret = Phidget_open((PhidgetHandle)rfid);
		if (digitalOutput0)
			ret = Phidget_open((PhidgetHandle)digitalOutput0);
		if (digitalOutput1)
			ret = Phidget_open((PhidgetHandle)digitalOutput1);
		if (digitalOutput2)
			ret = Phidget_open((PhidgetHandle)digitalOutput2);
		return ret;
	}
	
	PhidgetReturnCode close()
	{
		if (rfid)
			ret = Phidget_close((PhidgetHandle)rfid);
		if (digitalOutput0)
			ret = Phidget_close((PhidgetHandle)digitalOutput0);
		if (digitalOutput1)
			ret = Phidget_close((PhidgetHandle)digitalOutput1);
		if (digitalOutput2)
			ret = Phidget_close((PhidgetHandle)digitalOutput2);
		return ret;
	}	

	void createRFID()
	{
		PhidgetRFID_create(&rfid);
	}

	void createDigitalOut(int channel)
	{
		if (channel ==0)
		{
			PhidgetDigitalOutput_create(&digitalOutput0);
			if (serial>0)
				ret = Phidget_setDeviceSerialNumber((PhidgetHandle)digitalOutput0, serial);
			ret = Phidget_setChannel((PhidgetHandle)digitalOutput0, channel);
			ret = PhidgetDigitalOutput_setDutyCycle(digitalOutput0, 1);
		}
		else if (channel ==1)
		{
			PhidgetDigitalOutput_create(&digitalOutput1);
			if (serial>0)
				ret = Phidget_setDeviceSerialNumber((PhidgetHandle)digitalOutput1, serial);
			ret = Phidget_setChannel((PhidgetHandle)digitalOutput1, channel);
			ret = PhidgetDigitalOutput_setDutyCycle(digitalOutput1, 1);
		}
		else if (channel == 2)
		{
			PhidgetDigitalOutput_create(&digitalOutput2);
			if (serial>0)
				ret = Phidget_setDeviceSerialNumber((PhidgetHandle)digitalOutput2, serial);
			ret = Phidget_setChannel((PhidgetHandle)digitalOutput2, channel);
			ret = PhidgetDigitalOutput_setDutyCycle(digitalOutput2, 1);
		}
	}

	PhidgetReturnCode setDigitalOutputState(int channel, bool state)
	{
		if (channel == 0 && digitalOutput0)
			ret = PhidgetDigitalOutput_setState(digitalOutput0, state);
		else if (channel == 1 && digitalOutput1)
			ret = PhidgetDigitalOutput_setState(digitalOutput1, state);
		else if (channel == 2 && digitalOutput2)
			ret = PhidgetDigitalOutput_setState(digitalOutput2, state);
		return ret;
	}

	bool getDigitalOutputState(int channel)
	{
		int state;
		if (channel == 0 && digitalOutput0)
			ret = PhidgetDigitalOutput_getState(digitalOutput0, &state);
		else if (channel == 1 && digitalOutput1)
			ret = PhidgetDigitalOutput_getState(digitalOutput1, &state);
		else if (channel == 2 && digitalOutput2)
			ret = PhidgetDigitalOutput_getState(digitalOutput2, &state);
		if (ret == 0)
			return state==1?true:false;
		else
			return false;
	}

	PhidgetReturnCode setAntennaEnabled(bool antennaEnabled)
	{
		ret = PhidgetRFID_setAntennaEnabled(rfid, antennaEnabled?1:0);
		return ret;
	}

	bool getAntennaEnabled()
	{
		int antennaEnabled;
		ret = PhidgetRFID_getAntennaEnabled(rfid, &antennaEnabled);
		if (ret == 0)
			return antennaEnabled ==1?true:false;
		else
			return false;
	}

	void _delete()
	{
		if (rfid)
			PhidgetRFID_delete(&rfid);
		if (digitalOutput0)
			PhidgetDigitalOutput_delete(&digitalOutput0);
		if (digitalOutput1)
			PhidgetDigitalOutput_delete(&digitalOutput1);
		if (digitalOutput2)			
			PhidgetDigitalOutput_delete(&digitalOutput2);
	}

	void setOnAttachHandler()
	{
		if (rfid)	
			Phidget_setOnAttachHandler((PhidgetHandle)rfid, onAttach, this);
		if (digitalOutput0)	
			Phidget_setOnAttachHandler((PhidgetHandle)digitalOutput0, onAttach, this);
		if (digitalOutput1)	
			Phidget_setOnAttachHandler((PhidgetHandle)digitalOutput1, onAttach, this);
		if (digitalOutput2)	
			Phidget_setOnAttachHandler((PhidgetHandle)digitalOutput2, onAttach, this);
	}

	void setOnDetachHandler()
	{
		if (rfid)	
			Phidget_setOnDetachHandler((PhidgetHandle)rfid, onDetach, this);
		if (digitalOutput0)	
			Phidget_setOnDetachHandler((PhidgetHandle)digitalOutput0, onAttach, this);
		if (digitalOutput1)	
			Phidget_setOnDetachHandler((PhidgetHandle)digitalOutput1, onAttach, this);
		if (digitalOutput2)	
			Phidget_setOnDetachHandler((PhidgetHandle)digitalOutput2, onAttach, this);
	}

	void setOnTagHandler()
	{
		PhidgetRFID_setOnTagHandler(rfid, onTag, this);
	}

	void setOnTagLostHandler()
	{
		PhidgetRFID_setOnTagLostHandler(rfid, onTagLost, this);
	}

	void dispose()
	{
		ret = Phidget_close((PhidgetHandle)rfid);
		PhidgetRFID_delete(&rfid);
	}
};

/////////////////////////////////////////////////////////////////////////////////////

HL_PRIM PhidgetRFID* HL_NAME(rfid_create)()
{
	PhidgetRFID* phidgetRFID = new PhidgetRFID();
	return phidgetRFID;
}

HL_PRIM bool HL_NAME(rfid_createRFID)(PhidgetRFID* rfid)
{
	if (!rfid)
		return false;
	rfid->createRFID();
	return true;
}

HL_PRIM bool HL_NAME(rfid_createDigitalOut)(PhidgetRFID* rfid, int channel)
{
	if (!rfid)
		return false;
	rfid->createDigitalOut(channel);
	return true;
}

HL_PRIM bool HL_NAME(rfid_setDeviceSerialNumber)(PhidgetRFID* rfid, int serial)
{
	if (!rfid)
		return false;
	return rfid->setDeviceSerialNumber(serial) == 0;
}

HL_PRIM bool HL_NAME(rfid_open)(PhidgetRFID* rfid)
{
	if (!rfid)
		return false;
	return rfid->open() == 0;
}

HL_PRIM bool HL_NAME(rfid_close)(PhidgetRFID* rfid)
{
	if (!rfid)
		return false;
	return rfid->close() == 0;
}

HL_PRIM void HL_NAME(rfid_delete)(PhidgetRFID* rfid)
{
	if (!rfid)
		return;
	rfid->_delete();
}

HL_PRIM void HL_NAME(rfid_setOnAttachHandler)(PhidgetRFID* rfid)
{
	if (!rfid)
		return;
	rfid->setOnAttachHandler();
}

HL_PRIM void HL_NAME(rfid_setOnDetachHandler)(PhidgetRFID* rfid)
{
	if (!rfid)
		return;
	rfid->setOnDetachHandler();
}

HL_PRIM void HL_NAME(rfid_setOnTagHandler)(PhidgetRFID* rfid)
{
	if (!rfid)
		return;
	rfid->setOnTagHandler();
}

HL_PRIM void HL_NAME(rfid_setOnTagLostHandler)(PhidgetRFID* rfid)
{
	if (!rfid)
		return;
	rfid->setOnTagLostHandler();
}

HL_PRIM bool HL_NAME(rfid_getIsAttached)(PhidgetRFID* rfid)
{
	if (!rfid)
		return false;
	return rfid->attached;
}

HL_PRIM const char* HL_NAME(rfid_getCurrentTag)(PhidgetRFID* rfid)
{
	if (!rfid)
		return NULL;
	return rfid->currentTag.c_str();
}

HL_PRIM bool HL_NAME(rfid_setDigitalOutputState)(PhidgetRFID* rfid, int channel, bool state)
{
	if (!rfid)
		return false;
	return rfid->setDigitalOutputState(channel, state) == 0;
}

HL_PRIM bool HL_NAME(rfid_getDigitalOutputState)(PhidgetRFID* rfid, int channel)
{
	if (!rfid)
		return false;
	return rfid->getDigitalOutputState(channel);
}

HL_PRIM bool HL_NAME(rfid_setAntennaEnabled)(PhidgetRFID* rfid, bool antennaEnabled)
{
	if (!rfid)
		return false;
	return rfid->setAntennaEnabled(antennaEnabled) == 0;
}

HL_PRIM bool HL_NAME(rfid_getAntennaEnabled)(PhidgetRFID* rfid)
{
	if (!rfid)
		return false;
	return rfid->getAntennaEnabled();
}

/////////////////////////////////////////////////////////////////////////////////////

#define _PhidgetRFID _ABSTRACT(PhidgetRFID)
#define _PHIDGETRFIDHANDLE _ABSTRACT(PhidgetRFIDHandle)
#define _PHIDGETRETURNCODE _ABSTRACT(PhidgetReturnCode)

DEFINE_PRIM(_PhidgetRFID, rfid_create, _NO_ARG);
DEFINE_PRIM(_BOOL, rfid_createRFID, _PhidgetRFID);
DEFINE_PRIM(_BOOL, rfid_setDeviceSerialNumber, _PhidgetRFID _I32);
DEFINE_PRIM(_BOOL, rfid_open, _PhidgetRFID);
DEFINE_PRIM(_BOOL, rfid_close, _PhidgetRFID);
DEFINE_PRIM(_VOID, rfid_delete, _PhidgetRFID);
DEFINE_PRIM(_VOID, rfid_setOnAttachHandler, _PhidgetRFID);
DEFINE_PRIM(_VOID, rfid_setOnDetachHandler, _PhidgetRFID);
DEFINE_PRIM(_VOID, rfid_setOnTagHandler, _PhidgetRFID);
DEFINE_PRIM(_VOID, rfid_setOnTagLostHandler, _PhidgetRFID);
DEFINE_PRIM(_VOID, rfid_createDigitalOut, _PhidgetRFID _I32);
DEFINE_PRIM(_BOOL, rfid_getIsAttached, _PhidgetRFID);
DEFINE_PRIM(_BYTES, rfid_getCurrentTag, _PhidgetRFID);
DEFINE_PRIM(_BOOL, rfid_setDigitalOutputState, _PhidgetRFID _I32 _BOOL);
DEFINE_PRIM(_BOOL, rfid_getDigitalOutputState, _PhidgetRFID _I32);
DEFINE_PRIM(_BOOL, rfid_setAntennaEnabled, _PhidgetRFID _BOOL);
DEFINE_PRIM(_BOOL, rfid_getAntennaEnabled, _PhidgetRFID);