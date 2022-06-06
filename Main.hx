package;

import stage.native.phidgets.PhidgetRFID;
import stage.native.phidgets.PhidgetDigitalOutput;
import haxe.Timer;
import h2d.Console;

class Main extends hxd.App
{
	/////////////////////////////////////////////////////////////////////////////////////

	var c : Console;

	static function main()
	{
		new Main();
	}

	/////////////////////////////////////////////////////////////////////////////////////

	override function init()
	{
		super.init();
		// c = new Console(hxd.res.DefaultFont.get(),s2d);
		// c.log("Ready");
		var ph = new PhidgetRFID(601708);
		ph.onTag = (t)->trace('Tag found: $t');
		ph.onTagLost = (t)->trace('Tag lost.');
		// ph.antennaEnabled = false;

		// new Timer(50).run=()->
		// {
		// 	ph.onboardLED = !ph.onboardLED;
		// };

		var phdo:PhidgetDigitalOutput = new PhidgetDigitalOutput(2,601708);
		new Timer(160).run=()->
		{
			phdo.state = !phdo.state;
			trace(phdo.minFrequency,phdo.maxFrequency);
		};		
	}

	/////////////////////////////////////////////////////////////////////////////////////	
}
