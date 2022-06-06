package stage.native.phidgets;

typedef HLPhidgetDigitalOutput = hl.Abstract<"PhidgetDigitalOutput">;

@:hlNative("HlPhidgets")
class PhidgetDigitalOutput
{
	/////////////////////////////////////////////////////////////////////////////////////

	var digitalOutput					: HLPhidgetDigitalOutput;
	public var isAttached 				: Bool 					= false;
	@:isVar
	public var state(get,set)			: Null<Bool>;	
	public var channel 					: Int;
	public var serial 					: Int;
	public var minFrequency(get,never)	: hl.F64;	
	public var maxFrequency(get,never)	: hl.F64;	
	@:isVar
	public var frequency(get,set)		: hl.F64;	

	/////////////////////////////////////////////////////////////////////////////////////

	public function new(channel:Int = 0, ?serial:Null<Int>, autoOpen:Bool=true, autoUpdate:Bool=true)
	{
		this.channel = channel;
		digitalOutput = _create(this.channel);

		if (serial!=null)
		{
			this.serial = serial;
			_setDeviceSerialNumber(digitalOutput,serial);
		}

		_setOnAttachHandler(digitalOutput);
		_setOnDetachHandler(digitalOutput);

		if (autoUpdate)
			haxe.MainLoop.add(update);
		if (autoOpen)
			open();
	}

	/////////////////////////////////////////////////////////////////////////////////////

	public function open():Bool
	{
		return _open(digitalOutput);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	public function getDigitalOutputState(channel:Int):Bool
	{
		return _getDigitalOutputState(digitalOutput,channel);
	}

	public function setDigitalOutputState(channel:Int,state:Bool):Bool
	{
		return _setDigitalOutputState(digitalOutput,channel,state);
	}

	public function getState():Bool				return _getDigitalOutputState(digitalOutput,channel);
	public function setState(state:Bool):Bool	return _setDigitalOutputState(digitalOutput,channel,state);

	function get_state():Null<Bool> return state = _getDigitalOutputState(digitalOutput,channel);
	function set_state(value: Null<Bool>):Null<Bool>
	{
		_setDigitalOutputState(digitalOutput,channel,value);
		return state = value;
	}

	function get_frequency():hl.F64 return frequency = _getFrequency(digitalOutput);
	function set_frequency(value:hl.F64):hl.F64
	{
		_setFrequency(digitalOutput,value);
		return frequency = value;
	}

	function get_minFrequency():hl.F64 return 
		return _getMinFrequency(digitalOutput);

	function get_maxFrequency():hl.F64 return 
		return _getMaxFrequency(digitalOutput);

	/////////////////////////////////////////////////////////////////////////////////////

	public function update()
	{
		if (digitalOutput!=null)
		{
			if (_getIsAttached(digitalOutput) && !isAttached)
				_onAttach();
			else if (!_getIsAttached(digitalOutput) && isAttached)
				_onDetach();
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////

	function _onAttach()
	{
		isAttached=true;
	}

	function _onDetach()
	{
		isAttached=false;
	}

	/////////////////////////////////////////////////////////////////////////////////////

 	@:hlNative("HlPhidgets","digitalOutput_create")					static function _create(channel0:Int):HLPhidgetDigitalOutput { return null; }
	@:hlNative("HlPhidgets","digitalOutput_setDeviceSerialNumber")	static function _setDeviceSerialNumber(handle:HLPhidgetDigitalOutput,serial:Int):Bool { return false; }
	@:hlNative("HlPhidgets","digitalOutput_setOnAttachHandler")		static function _setOnAttachHandler(handle:HLPhidgetDigitalOutput) {};
	@:hlNative("HlPhidgets","digitalOutput_setOnDetachHandler")		static function _setOnDetachHandler(handle:HLPhidgetDigitalOutput) {};
	@:hlNative("HlPhidgets","digitalOutput_getIsAttached")			static function _getIsAttached(handle:HLPhidgetDigitalOutput):Bool { return false; }
	@:hlNative("HlPhidgets","digitalOutput_setDigitalOutputState")	static function _setDigitalOutputState(handle:HLPhidgetDigitalOutput,channel:Int,state:Bool):Bool { return false; }
	@:hlNative("HlPhidgets","digitalOutput_getDigitalOutputState")	static function _getDigitalOutputState(handle:HLPhidgetDigitalOutput,channel:Int):Bool { return false; }
	@:hlNative("HlPhidgets","digitalOutput_setFrequency")			static function _setFrequency(handle:HLPhidgetDigitalOutput,frequency:hl.F64) { }
	@:hlNative("HlPhidgets","digitalOutput_getFrequency")			static function _getFrequency(handle:HLPhidgetDigitalOutput):hl.F64 { return 0; }
	@:hlNative("HlPhidgets","digitalOutput_getMinFrequency")		static function _getMinFrequency(handle:HLPhidgetDigitalOutput):hl.F64 { return 0; }
	@:hlNative("HlPhidgets","digitalOutput_getMaxFrequency")		static function _getMaxFrequency(handle:HLPhidgetDigitalOutput):hl.F64 { return 0; }
	@:hlNative("HlPhidgets","digitalOutput_open")					static function _open(handle:HLPhidgetDigitalOutput):Bool { return false; }
	@:hlNative("HlPhidgets","digitalOutput_close")					static function _close(handle:HLPhidgetDigitalOutput):Bool { return false; }
	@:hlNative("HlPhidgets","digitalOutput_delete")					static function _delete(handle:HLPhidgetDigitalOutput) { }

	/////////////////////////////////////////////////////////////////////////////////////
}
