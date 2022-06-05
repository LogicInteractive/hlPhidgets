package stage.native.phidgets;

typedef HLPhidgetRFID = hl.Abstract<"PhidgetRFID">;

@:hlNative("HlPhidgets")
class PhidgetRFID
{
	/////////////////////////////////////////////////////////////////////////////////////

	var rfid							: HLPhidgetRFID;
	public var isAttached 				: Bool 					= false;
	public var hasTag	 				: Bool 					= false;
	public var currentTag 				: String;
	public var lastTag 					: String;
	public var serial 					: Int;

	public var onTag					: String->Void;
	public var onTagLost				: String->Void;
	public var model					: String;

	@:isVar
	public var digitalOutput0(get,set)	: Bool;
	@:isVar
	public var digitalOutput1(get,set)	: Bool;
	@:isVar
	public var digitalOutput2(get,set)	: Bool;
	@:isVar
	public var onboardLED(get,set)		: Bool;
	@:isVar
	public var antennaEnabled(get,set)	: Bool					= true;

	/////////////////////////////////////////////////////////////////////////////////////

	public function new(?serial:Null<Int>, createDigitalOutput0:Bool=false, createDigitalOutput1:Bool=false, createDigitalOutput2:Bool=false, autoCreateRFID:Bool=true, autoOpen:Bool=true, autoUpdate:Bool=true)
	{
		model = "PhidgetRFID Read-Write 1024_0";

		rfid = _create();
		if (autoCreateRFID)
			createRFID(false);
		if (createDigitalOutput0)
			createDigitalOut(0);
		if (createDigitalOutput1)
			createDigitalOut(1);
		if (createDigitalOutput2)
			createDigitalOut(2);

		if (serial!=null)
		{
			this.serial = serial;
			_setDeviceSerialNumber(rfid,serial);
		}

		_setOnAttachHandler(rfid);
		_setOnDetachHandler(rfid);
		_setOnTagHandler(rfid);
		_setOnTagLostHandler(rfid);

		if (autoUpdate)
			haxe.MainLoop.add(update);

		if (autoOpen)
			open();
	}

	/////////////////////////////////////////////////////////////////////////////////////

	public function createRFID(autoOpen:Bool):Bool
	{
		var r = _createRFID(rfid);
		var r2 = true;
		if (autoOpen)
			r2 = open();
		return r&&r2;
	}

	public function open():Bool
	{
		return _open(rfid);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	public function createDigitalOut(channel:Int)
	{
		_createDigitalOut(rfid,channel);
	}

	public function getDigitalOutputState(channel:Int):Bool
	{
		return _getDigitalOutputState(rfid,channel);
	}

	public function setDigitalOutputState(channel:Int,state:Bool):Bool
	{
		return _setDigitalOutputState(rfid,channel,state);
	}

	public function getDigitalOutput0():Bool			return _getDigitalOutputState(rfid,0);
	public function setDigitalOutput0(state:Bool):Bool	return _setDigitalOutputState(rfid,0,state);
	public function getDigitalOutput1():Bool			return _getDigitalOutputState(rfid,1);
	public function setDigitalOutput1(state:Bool):Bool	return _setDigitalOutputState(rfid,1,state);
	public function getDigitalOutput2():Bool			return _getDigitalOutputState(rfid,2);
	public function setDigitalOutput2(state:Bool):Bool	return _setDigitalOutputState(rfid,2,state);

	function get_digitalOutput0():Bool return digitalOutput0 = _getDigitalOutputState(rfid,0);
	function set_digitalOutput0(value:Bool):Bool
	{
		_setDigitalOutputState(rfid,0,value);
		return digitalOutput0 = value;
	}

	function get_digitalOutput1():Bool return digitalOutput1 = _getDigitalOutputState(rfid,1);
	function set_digitalOutput1(value:Bool):Bool
	{
		_setDigitalOutputState(rfid,1,value);
		return digitalOutput1 = value;
	}

	function get_digitalOutput2():Bool return digitalOutput2 = _getDigitalOutputState(rfid,2);
	function set_digitalOutput2(value:Bool):Bool
	{
		_setDigitalOutputState(rfid,2,value);
		return digitalOutput2 = value;
	}

	function get_onboardLED():Bool return onboardLED = digitalOutput2 = _getDigitalOutputState(rfid,2);
	function set_onboardLED(value:Bool):Bool
	{
		_setDigitalOutputState(rfid,2,value);
		return onboardLED = digitalOutput2 = value;
	}


	function get_antennaEnabled():Bool
	{
		return _getAntennaEnabled(rfid);
	}

	function set_antennaEnabled(value:Bool):Bool
	{
		_setAntennaEnabled(rfid,value);
		return antennaEnabled=value;
	}

	/////////////////////////////////////////////////////////////////////////////////////

	public function update()
	{
		if (rfid!=null)
		{
			if (_getIsAttached(rfid) && !isAttached)
				_onAttach();
			else if (!_getIsAttached(rfid) && isAttached)
				_onDetach();

			if (isAttached)
			{
				var ct:String = _getCurrentTag(rfid);
				if (ct!=null && ct!=currentTag)
					_onTag(ct);
				else if (ct==null && currentTag!=null)
					_onTagLost(lastTag);
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////

	function _onTag(tag:String)
	{
		currentTag = tag;
		lastTag = tag;
		hasTag = true;
		if (onTag!=null)
			onTag(tag);
	}

	function _onTagLost(tag:String)
	{
		currentTag = null;
		hasTag = false;
		if (onTagLost!=null)
			onTagLost(tag);
	}

	function _onAttach()
	{
		isAttached=true;
	}

	function _onDetach()
	{
		isAttached=false;
	}

	/////////////////////////////////////////////////////////////////////////////////////

 	@:hlNative("HlPhidgets","rfid_create")					static function _create():HLPhidgetRFID { return null; }
	@:hlNative("HlPhidgets","rfid_createRFID")				static function _createRFID(handle:HLPhidgetRFID):Bool { return false; }
	@:hlNative("HlPhidgets","rfid_setDeviceSerialNumber")	static function _setDeviceSerialNumber(handle:HLPhidgetRFID,serial:Int):Bool { return false; }
	@:hlNative("HlPhidgets","rfid_setOnAttachHandler")		static function _setOnAttachHandler(handle:HLPhidgetRFID) {};
	@:hlNative("HlPhidgets","rfid_setOnDetachHandler")		static function _setOnDetachHandler(handle:HLPhidgetRFID) {};
	@:hlNative("HlPhidgets","rfid_setOnTagHandler")			static function _setOnTagHandler(handle:HLPhidgetRFID) {};
	@:hlNative("HlPhidgets","rfid_setOnTagLostHandler")		static function _setOnTagLostHandler(handle:HLPhidgetRFID) {};
	@:hlNative("HlPhidgets","rfid_getIsAttached")			static function _getIsAttached(handle:HLPhidgetRFID):Bool { return false; }
	@:hlNative("HlPhidgets","rfid_createDigitalOut")		static function _createDigitalOut(handle:HLPhidgetRFID,channel:Int) { }
	@:hlNative("HlPhidgets","rfid_setDigitalOutputState")	static function _setDigitalOutputState(handle:HLPhidgetRFID,channel:Int,state:Bool):Bool { return false; }
	@:hlNative("HlPhidgets","rfid_getDigitalOutputState")	static function _getDigitalOutputState(handle:HLPhidgetRFID,channel:Int):Bool { return false; }
	@:hlNative("HlPhidgets","rfid_setAntennaEnabled")		static function _setAntennaEnabled(handle:HLPhidgetRFID,antennaEnabled:Bool):Bool { return false; }
	@:hlNative("HlPhidgets","rfid_getAntennaEnabled")		static function _getAntennaEnabled(handle:HLPhidgetRFID):Bool { return false; }
	@:hlNative("HlPhidgets","rfid_open")					static function _open(handle:HLPhidgetRFID):Bool { return false; }
	@:hlNative("HlPhidgets","rfid_close")					static function _close(handle:HLPhidgetRFID):Bool { return false; }
	@:hlNative("HlPhidgets","rfid_delete")					static function _delete(handle:HLPhidgetRFID) { }

	@:hlNative("HlPhidgets","rfid_getCurrentTag") static function __getCurrentTag(handle:HLPhidgetRFID):hl.Bytes { return null; }
	static inline function _getCurrentTag(handle:HLPhidgetRFID):String
	{
		var b:hl.Bytes = __getCurrentTag(handle);
		var s:String = @:privateAccess String.fromUTF8(b);
		return s=="" ? null : s;
	}

	// @:hlNative("HlPhidgets","testCallback")
	// public static function testCallback(callback:Void->Void) {};

	/////////////////////////////////////////////////////////////////////////////////////
}
