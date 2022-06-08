package stage.native.phidgets;

typedef HLPhidget = hl.Abstract<"PhidgetData">;

@:allow(stage.native.phidgets.PhidgetRFID)
@:hlNative("HlPhidgets")
class Phidget
{
	/////////////////////////////////////////////////////////////////////////////////////

	public var isAttached 				: Bool 					= false;
	public var serial 					: Int;
	public var model					: String;
	public var handle					: HLPhidget;

	/////////////////////////////////////////////////////////////////////////////////////

	public function new(?serial:Null<Int>, autoCreateRFID:Bool=true, autoOpen:Bool=true, autoUpdate:Bool=true)
	{
		model = "";
		if (serial!=null)
		{
			this.serial = serial;
			_setDeviceSerialNumber(handle,serial);
		}

		if (autoUpdate)
			haxe.MainLoop.add(update);

		if (autoOpen)
			open();
	}

	/////////////////////////////////////////////////////////////////////////////////////

	public function open():Bool
	{
		return _open(handle);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	public function update()
	{

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

	// @:hlNative("HlPhidgets","hlphidget_open")					static function _open(handle:HLPhidget):Bool { return false; }
	// @:hlNative("HlPhidgets","hlphidget_close")					static function _close(handle:HLPhidget):Bool { return false; }
	// @:hlNative("HlPhidgets","hlphidget_delete")					static function _delete(handle:HLPhidget) { }
	// @:hlNative("HlPhidgets","hlphidget_setDeviceSerialNumber")	static function _setDeviceSerialNumber(handle:HLPhidget,serial:Int):Bool { return false; }
	// // @:hlNative("HlPhidgets","hlphidget_setOnAttachHandler")		static function _setOnAttachHandler(handle:HLPhidget) {};
	// // @:hlNative("HlPhidgets","hlphidget_setOnDetachHandler")		static function _setOnDetachHandler(handle:HLPhidget) {};
	// @:hlNative("HlPhidgets","hlphidget_getIsAttached")			static function _getIsAttached(handle:HLPhidget):Bool { return false; }

	/////////////////////////////////////////////////////////////////////////////////////
}
