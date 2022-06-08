package stage.native.phidgets;

typedef HLPhidgetEncoder = hl.Abstract<"PhidgetEncoder">;

@:hlNative("HlPhidgets")
class PhidgetEncoder
{
	/////////////////////////////////////////////////////////////////////////////////////

	var encoder									: HLPhidgetEncoder;
	public var isAttached 						: Bool 					= false;
	public var serial 							: Int;
	public var lastPositionChange				: Int;
	public var lastTimeChange					: hl.F64;
	public var lastIndexTriggered				: Int;
	public var previousPosition					: hl.I64;
	public var position(default,set)			: hl.I64;
	@:isVar
	public var enabled(get,set)					: Bool;
	@:isVar
	public var dataInterval(get,set)			: Int;
	@:isVar
	public var positionChangeTrigger(get,set)	: hl.I64;
	public var minPositionChangeTrigger(get,default): hl.I64;
	public var maxPositionChangeTrigger(get,default): hl.I64;

	public var onPositionChange					: hl.I64->Void;

	/////////////////////////////////////////////////////////////////////////////////////

	/**
	 * This creates an instance of a PhidgetEncoder class. 
	 * Once created the channel may need to be configured to match the physical device and channel index you want to attach to, 
	 * and then be opened by calling open() if not set to autoOpen.
	 * The Phidget channel handle must be released with delete() or dispose.
	 *
	 * @param  serial the serial of the phidget (optional)
	 * @param  autoOpen should the phidget call open on created
	 * @param  autoUpdate should the phidget create a mainloop tick
	 * @return success
	 */
	public function new(?serial:Null<Int>, autoOpen:Bool=true, autoUpdate:Bool=true)
	{
		encoder = _create();

		if (serial!=null)
		{
			this.serial = serial;
			_setDeviceSerialNumber(encoder,serial);
		}

		_setOnAttachHandler(encoder);
		_setOnDetachHandler(encoder);

		if (autoUpdate)
			haxe.MainLoop.add(update);
		if (autoOpen)
			open();
	}

	/////////////////////////////////////////////////////////////////////////////////////


	/**
	 * PhidgetEncoder_create() creates an instance of a Phidget channel. 
	 * Once created the channel may need to be configured to match the physical device and channel index you want to attach to, 
	 * and then be opened by calling Phidget_open().
	 * The Phidget channel handle must be released with PhidgetEncoder_delete().
	 *
	 * @return success
	 */
	public function open():Bool
	{
		return _open(encoder);
	}

	public function close():Bool
	{
		return _close(encoder);
	}

	public function delete()
	{
		_delete(encoder);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	public function update()
	{
		if (encoder!=null)
		{
			if (_getIsAttached(encoder) && !isAttached)
				_onAttach();
			else if (!_getIsAttached(encoder) && isAttached)
				_onDetach();

			if (isAttached)
			{
				var newPos:hl.I64 = _getPosition(encoder);
				if (newPos!=position)
				{
					lastPositionChange = _getPositionChange(encoder);
					lastTimeChange = _getTimeChange(encoder);
					lastIndexTriggered = _getIndexTriggered(encoder);
					previousPosition = position;
					position = newPos;
					if (onPositionChange!=null)
						onPositionChange(position);
				}
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////

	function set_position(value:hl.I64):hl.I64
	{
		position = value;
		_setPosition(encoder,position);
		return position;
	}

	function get_enabled():Bool
	{
		enabled = _getEnabled(encoder);
		return enabled;
	}
	function set_enabled(value:Bool):Bool
	{
		enabled = value;
		_setEnabled(encoder,enabled);
		return enabled;
	}

	function get_dataInterval():Int
	{
		dataInterval = _getDataInterval(encoder);
		return dataInterval;
	}
	function set_dataInterval(value:Int):Int
	{
		dataInterval = value;
		_setDataInterval(encoder,dataInterval);
		return dataInterval;
	}

	function get_positionChangeTrigger():hl.I64 
	{
		positionChangeTrigger = _getPositionChangeTrigger(encoder);
		return positionChangeTrigger;
	}
	function set_positionChangeTrigger(value:hl.I64):hl.I64 
	{
		positionChangeTrigger = value;
		_setPositionChangeTrigger(encoder,positionChangeTrigger);
		return positionChangeTrigger;
	}

	function get_minPositionChangeTrigger():hl.I64 
	{
		minPositionChangeTrigger = _getMinPositionChangeTrigger(encoder);
		return minPositionChangeTrigger;
	}

	function get_maxPositionChangeTrigger():hl.I64 
	{
		maxPositionChangeTrigger = _getMaxPositionChangeTrigger(encoder);
		return maxPositionChangeTrigger;
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

 	@:hlNative("HlPhidgets","encoder_create")						static function _create():HLPhidgetEncoder { return null; }
	@:hlNative("HlPhidgets","encoder_setDeviceSerialNumber")		static function _setDeviceSerialNumber(handle:HLPhidgetEncoder,serial:Int):Bool { return false; }
	@:hlNative("HlPhidgets","encoder_setOnAttachHandler")			static function _setOnAttachHandler(handle:HLPhidgetEncoder) {};
	@:hlNative("HlPhidgets","encoder_setOnDetachHandler")			static function _setOnDetachHandler(handle:HLPhidgetEncoder) {};
	@:hlNative("HlPhidgets","encoder_getIsAttached")				static function _getIsAttached(handle:HLPhidgetEncoder):Bool { return false; }
	@:hlNative("HlPhidgets","encoder_open")							static function _open(handle:HLPhidgetEncoder):Bool { return false; }
	@:hlNative("HlPhidgets","encoder_close")						static function _close(handle:HLPhidgetEncoder):Bool { return false; }
	@:hlNative("HlPhidgets","encoder_delete")						static function _delete(handle:HLPhidgetEncoder) { }
	@:hlNative("HlPhidgets","encoder_getPositionChange")			static function _getPositionChange(handle:HLPhidgetEncoder):Int { return 0; }
	@:hlNative("HlPhidgets","encoder_getTimeChange")				static function _getTimeChange(handle:HLPhidgetEncoder):hl.F64 { return 0; }
	@:hlNative("HlPhidgets","encoder_getIndexTriggered")			static function _getIndexTriggered(handle:HLPhidgetEncoder):Int { return 0; }
	@:hlNative("HlPhidgets","encoder_getEnabled")					static function _getEnabled(handle:HLPhidgetEncoder):Bool { return false; }
	@:hlNative("HlPhidgets","encoder_setEnabled")					static function _setEnabled(handle:HLPhidgetEncoder,enabled:Bool) { }
	@:hlNative("HlPhidgets","encoder_getDataInterval")				static function _getDataInterval(handle:HLPhidgetEncoder):Int { return 0; }
	@:hlNative("HlPhidgets","encoder_setDataInterval")				static function _setDataInterval(handle:HLPhidgetEncoder,dataInterval:Int) { }
	@:hlNative("HlPhidgets","encoder_getMinDataInterval")			static function _getMinDataInterval(handle:HLPhidgetEncoder):Int { return 0; }
	@:hlNative("HlPhidgets","encoder_getMaxDataInterval")			static function _getMaxDataInterval(handle:HLPhidgetEncoder):Int { return 0; }
	// @:hlNative("HlPhidgets","encoder_getDataRate")				static function _getDataRate(handle:HLPhidgetEncoder):hl.F64 { return 0; }
	// @:hlNative("HlPhidgets","encoder_setDataRate")				static function _setDataRate(handle:HLPhidgetEncoder,dataRate:hl.F64) { } 
	// @:hlNative("HlPhidgets","encoder_getMinDataRate")			static function _getMinDataRate(handle:HLPhidgetEncoder):hl.F64 { return 0; }
	// @:hlNative("HlPhidgets","encoder_getMaxDataRate")			static function _getMaxDataRate(handle:HLPhidgetEncoder):hl.F64 { return 0; }
	@:hlNative("HlPhidgets","encoder_getIndexPosition")				static function _getIndexPosition(handle:HLPhidgetEncoder):hl.I64 { return 0; }
	@:hlNative("HlPhidgets","encoder_getPosition")					static function _getPosition(handle:HLPhidgetEncoder):hl.I64 { return 0; }
	@:hlNative("HlPhidgets","encoder_setPosition")					static function _setPosition(handle:HLPhidgetEncoder,position:hl.I64) { }
	@:hlNative("HlPhidgets","encoder_getPositionChangeTrigger")		static function _getPositionChangeTrigger(handle:HLPhidgetEncoder):hl.I64 { return 0; }
	@:hlNative("HlPhidgets","encoder_setPositionChangeTrigger")		static function _setPositionChangeTrigger(handle:HLPhidgetEncoder,position:hl.I64) { }
	@:hlNative("HlPhidgets","encoder_getMinPositionChangeTrigger")	static function _getMinPositionChangeTrigger(handle:HLPhidgetEncoder):hl.I64 { return 0; }
	@:hlNative("HlPhidgets","encoder_getMaxPositionChangeTrigger")	static function _getMaxPositionChangeTrigger(handle:HLPhidgetEncoder):hl.I64 { return 0; }

	/////////////////////////////////////////////////////////////////////////////////////
}
