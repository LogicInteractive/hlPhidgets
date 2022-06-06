#define HL_NAME(n) hlPhidgets_##n

#include "hl.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <cstring>
#include <phidget22.h>

/////////////////////////////////////////////////////////////////////////////////////

class PhidgetEncoder
{

public:
	PhidgetEncoder()
	{
		create();
	}

	~PhidgetEncoder()
	{
		//dispose();
	}

	PhidgetEncoderHandle handle = NULL;
	PhidgetReturnCode ret;
	bool attached = false;
	int serial = 0;
	int positionChange = 0;
	double timeChange = 0;
	int indexTriggered = 0;
	int64_t position = 0;

private:
	PhidgetReturnCode errorCode;
	const char* errorString;
	char errorDetail[100];
	size_t errorDetailLen = 100;

	static void CCONV onAttach(PhidgetHandle ch, void* ctx)
	{
		if (ctx)
		{
			PhidgetEncoder* ph = (PhidgetEncoder*)ctx;
			ph->attached = true;
		}
	}

	static void CCONV onDetach(PhidgetHandle ch, void* ctx)
	{
		if (ctx)
		{
			PhidgetEncoder* ph = (PhidgetEncoder*)ctx;
			ph->attached = false;
		}
	}

	static void CCONV onPositionChange(PhidgetEncoderHandle ch, void* ctx, int positionChange, double timeChange, int indexTriggered)
	{
		int64_t position;
		//printf("PositionChange: %d\n", positionChange);
		//printf("TimeChange: %lf\n", timeChange);
		//printf("IndexTriggered: %d\n", indexTriggered);

		PhidgetEncoder_getPosition(ch, &position);
		//printf("Position: %lld\n", position);

		if (ctx)
		{
			PhidgetEncoder* ph = (PhidgetEncoder*)ctx;
			ph->positionChange = positionChange;
			ph->timeChange = timeChange;
			ph->indexTriggered = indexTriggered;
			ph->position = position;
		}		
	}	
	
public:
	PhidgetReturnCode setDeviceSerialNumber(int serial)
	{
		this->serial = serial;
		if (handle)
			ret = Phidget_setDeviceSerialNumber((PhidgetHandle)handle, serial);
		return ret;
	}

	PhidgetReturnCode open()
	{
		if (handle)
			ret = Phidget_open((PhidgetHandle)handle);
		return ret;
	}
	
	PhidgetReturnCode close()
	{
		if (handle)
			ret = Phidget_close((PhidgetHandle)handle);
		return ret;
	}	

	void create()
	{
		PhidgetEncoder_create(&handle);
		if (serial>0)
			ret = Phidget_setDeviceSerialNumber((PhidgetHandle)handle, serial);
	}

	void _delete()
	{
		if (handle)
			PhidgetEncoder_delete(&handle);
	}

	void setOnAttachHandler()
	{
		if (handle)
			Phidget_setOnAttachHandler((PhidgetHandle)handle, onAttach, this);
	}

	void setOnDetachHandler()
	{
		if (handle)
			Phidget_setOnDetachHandler((PhidgetHandle)handle, onDetach, this);
	}

	void setOnPositionChangeHandler()
	{
		if (handle)
			PhidgetEncoder_setOnPositionChangeHandler(handle, onPositionChange, this);
	}

	int64_t getPosition()
	{
		int64_t position = 0;
		if (handle)
			PhidgetEncoder_getPosition(handle, &position);
		return position;
	}

	void dispose()
	{
		Phidget_close((PhidgetHandle)handle);
		PhidgetEncoder_delete(&handle);
	}
};

/////////////////////////////////////////////////////////////////////////////////////

HL_PRIM PhidgetEncoder* HL_NAME(encoder_create)()
{
	PhidgetEncoder* encoder = new PhidgetEncoder();
	return encoder;
}

HL_PRIM bool HL_NAME(encoder_setDeviceSerialNumber)(PhidgetEncoder* encoder, int serial)
{
	if (!encoder)
		return false;
	return encoder->setDeviceSerialNumber(serial) == 0;
}

HL_PRIM void HL_NAME(encoder_delete)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return;
	encoder->_delete();
}

HL_PRIM void HL_NAME(encoder_setOnAttachHandler)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return;
	encoder->setOnAttachHandler();
}

HL_PRIM void HL_NAME(encoder_setOnDetachHandler)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return;
	encoder->setOnDetachHandler();
}

HL_PRIM void HL_NAME(encoder_setOnPositionChangeHandler)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return;
	encoder->setOnPositionChangeHandler();
}

HL_PRIM bool HL_NAME(encoder_getIsAttached)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return false;
	return encoder->attached;
}

HL_PRIM bool HL_NAME(encoder_open)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return false;
	return encoder->open() == 0;
}

HL_PRIM bool HL_NAME(encoder_close)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return false;
	return encoder->close() == 0;
}

HL_PRIM int HL_NAME(encoder_getPositionChange)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return 0;
	return encoder->positionChange;
}

HL_PRIM double HL_NAME(encoder_getTimeChange)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return 0;
	return encoder->timeChange;
}

HL_PRIM int HL_NAME(encoder_getIndexTriggered)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return 0;
	return encoder->indexTriggered;
}

HL_PRIM bool HL_NAME(encoder_getEnabled)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return 0;
	
	int enabled;
	encoder->ret = PhidgetEncoder_getEnabled(encoder->handle, &enabled);
	return enabled==1?true:false;
}

HL_PRIM int64_t HL_NAME(encoder_setEnabled)(PhidgetEncoder* encoder, bool enabled)
{
	if (!encoder)
		return 0;
	encoder->ret = PhidgetEncoder_setEnabled(encoder->handle, enabled?1:0);
	return encoder->getPosition();
}

HL_PRIM int HL_NAME(encoder_getDataInterval)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return 0;
	
	uint32_t dataInterval;
	encoder->ret = PhidgetEncoder_getDataInterval(encoder->handle, &dataInterval);
	return dataInterval;
}

HL_PRIM void HL_NAME(encoder_setDataInterval)(PhidgetEncoder* encoder, uint32_t datainterval)
{
	if (!encoder)
		return;
	encoder->ret = PhidgetEncoder_setDataInterval(encoder->handle, datainterval);
	return;
}

HL_PRIM int HL_NAME(encoder_getMinDataInterval)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return 0;

	uint32_t minDataInterval;
	encoder->ret = PhidgetEncoder_getMinDataInterval(encoder->handle, &minDataInterval);
	return minDataInterval;
}

HL_PRIM int HL_NAME(encoder_getMaxDataInterval)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return 0;

	uint32_t maxDataInterval;
	encoder->ret = PhidgetEncoder_getMaxDataInterval(encoder->handle, &maxDataInterval);
	return maxDataInterval;
}
/*
HL_PRIM double HL_NAME(encoderHighSpeed_getDataRate)(PhidgetEncoderHighSpeed* encoder)
{
	if (!encoder)
		return 0;

	double dataRate = 0;
	//encoder->ret = PhidgetEncoder_getDataRate(encoder->handle, &dataRate);
	return dataRate;
}

HL_PRIM void HL_NAME(encoderHighSpeed_setDataRate)(PhidgetEncoderHighSpeed* encoder, double dataRate)
{
	if (!encoder)
		return;
	encoder->ret = PhidgetEncoder_setDataRate(encoder->handle, dataRate);
}

HL_PRIM double HL_NAME(encoderHighSpeed_getMinDataRate)(PhidgetEncoderHighSpeed* encoder)
{
	if (!encoder)
		return 0;

	double minDataRate;
	encoder->ret = PhidgetEncoder_getMinDataRate(encoder->handle, &minDataRate);
	return minDataRate;
}

HL_PRIM double HL_NAME(encoderHighSpeed_getMaxDataRate)(PhidgetEncoderHighSpeed* encoder)
{
	if (!encoder)
		return 0;

	double maxDataRate;
	encoder->ret = PhidgetEncoder_getMaxDataRate(encoder->handle, &maxDataRate);
	return maxDataRate;
}
*/

HL_PRIM int64_t HL_NAME(encoder_getIndexPosition)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return 0;

	int64_t indexPosition;
	encoder->ret = PhidgetEncoder_getIndexPosition(encoder->handle, &indexPosition);
	return indexPosition;
}

HL_PRIM int64_t HL_NAME(encoder_getPosition)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return 0;

	int64_t position;
	encoder->ret = PhidgetEncoder_getPosition(encoder->handle, &position);
	return position;
}

HL_PRIM void HL_NAME(encoder_setPosition)(PhidgetEncoder* encoder, int64_t position)
{
	if (!encoder)
		return;
	encoder->ret = PhidgetEncoder_setPosition(encoder->handle, position);
	return;
}

HL_PRIM uint32_t HL_NAME(encoder_getPositionChangeTrigger)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return 0;

	uint32_t positionChangeTrigger;
	encoder->ret = PhidgetEncoder_getPositionChangeTrigger(encoder->handle, &positionChangeTrigger);
	return positionChangeTrigger;
}

HL_PRIM void HL_NAME(encoder_setPositionChangeTrigger)(PhidgetEncoder* encoder, uint32_t positionChangeTrigger)
{
	if (!encoder)
		return;
	encoder->ret = PhidgetEncoder_setPositionChangeTrigger(encoder->handle, positionChangeTrigger);
	return;
}

HL_PRIM uint32_t HL_NAME(encoder_getMinPositionChangeTrigger)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return 0;

	uint32_t minPositionChangeTrigger;
	encoder->ret = PhidgetEncoder_getMinPositionChangeTrigger(encoder->handle, &minPositionChangeTrigger);
	return minPositionChangeTrigger;
}

HL_PRIM uint32_t HL_NAME(encoder_getMaxPositionChangeTrigger)(PhidgetEncoder* encoder)
{
	if (!encoder)
		return 0;

	uint32_t maxPositionChangeTrigger;
	encoder->ret = PhidgetEncoder_getMaxPositionChangeTrigger(encoder->handle, &maxPositionChangeTrigger);
	return maxPositionChangeTrigger;
}

/////////////////////////////////////////////////////////////////////////////////////

#define _PHIDGETENCODER _ABSTRACT(PhidgetEncoder)
#define _PHIDGETRETURNCODE _ABSTRACT(PhidgetReturnCode)

DEFINE_PRIM(_PHIDGETENCODER, encoder_create, _NO_ARG);
DEFINE_PRIM(_BOOL, encoder_setDeviceSerialNumber, _PHIDGETENCODER _I32);
DEFINE_PRIM(_BOOL, encoder_open, _PHIDGETENCODER);
DEFINE_PRIM(_BOOL, encoder_close, _PHIDGETENCODER);
DEFINE_PRIM(_VOID, encoder_delete, _PHIDGETENCODER);
DEFINE_PRIM(_VOID, encoder_setOnAttachHandler, _PHIDGETENCODER);
DEFINE_PRIM(_VOID, encoder_setOnDetachHandler, _PHIDGETENCODER);
DEFINE_PRIM(_VOID, encoder_setOnPositionChangeHandler, _PHIDGETENCODER);
DEFINE_PRIM(_BOOL, encoder_getIsAttached, _PHIDGETENCODER);
DEFINE_PRIM(_I32, encoder_getPositionChange, _PHIDGETENCODER);
DEFINE_PRIM(_F64, encoder_getTimeChange, _PHIDGETENCODER);
DEFINE_PRIM(_I32, encoder_getIndexTriggered, _PHIDGETENCODER);
DEFINE_PRIM(_BOOL, encoder_getEnabled, _PHIDGETENCODER);
DEFINE_PRIM(_VOID, encoder_setEnabled, _PHIDGETENCODER _BOOL);
DEFINE_PRIM(_I32, encoder_getDataInterval, _PHIDGETENCODER);
DEFINE_PRIM(_VOID, encoder_setDataInterval, _PHIDGETENCODER _I32);
DEFINE_PRIM(_I32, encoder_getMinDataInterval, _PHIDGETENCODER);
DEFINE_PRIM(_I32, encoder_getMaxDataInterval, _PHIDGETENCODER);
//DEFINE_PRIM(_F64, encoderHighSpeed_getDataRate, _PHIDGETENCODER);
//DEFINE_PRIM(_VOID, encoderHighSpeed_setDataRate, _PHIDGETENCODER _F64);
//DEFINE_PRIM(_F64, encoderHighSpeed_getMinDataRate, _PHIDGETENCODER);
//DEFINE_PRIM(_F64, encoderHighSpeed_getMaxDataRate, _PHIDGETENCODER);
DEFINE_PRIM(_I64, encoder_getIndexPosition, _PHIDGETENCODER);
DEFINE_PRIM(_I64, encoder_getPosition, _PHIDGETENCODER);
DEFINE_PRIM(_VOID, encoder_setPosition, _PHIDGETENCODER _I64);
DEFINE_PRIM(_I64, encoder_getPositionChangeTrigger, _PHIDGETENCODER);
DEFINE_PRIM(_VOID, encoder_setPositionChangeTrigger, _PHIDGETENCODER _I64);
DEFINE_PRIM(_I64, encoder_getMinPositionChangeTrigger, _PHIDGETENCODER);
DEFINE_PRIM(_I64, encoder_getMaxPositionChangeTrigger, _PHIDGETENCODER);
