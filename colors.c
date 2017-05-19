#include "colors.h"

TLegoColors getColorNameByHSB(tSensors nDeviceIndex, int lineNumber)
{
	long hue = getColorHue(nDeviceIndex);
	//long sat = getColorSaturation(nDeviceIndex);
	short val = getColorReflected(nDeviceIndex);

	displayString(lineNumber, "H: %d3; S: %d3; V: %d", hue, 0, val);

	if(hue > 20 && hue < 35)
		return colorWhite;
	if(hue >= 35 && hue < 70)
		return colorBlack;
	if(hue > 110 && hue < 130)
		return colorGreen;
	return colorNone;
}

TLegoColors getLegoColorFromRGB(tSensors nDevice, int lineNumber)
{
	long red;
	long green;
	long blue;

	getColorRGB(nDevice, red, green, blue);

	displayString(lineNumber, "R: %d; G: %d; B: %d", red, green, blue);

	if(red > 50){
		displayTextLine(lineNumber+2, "White");
		return colorWhite;
	}
	if(abs(green-red) < 10 && red < 35){
		displayTextLine(lineNumber+2, "Black");
		return colorBlack;
	}
	if(green >15 && red < 15){
		displayTextLine(lineNumber+2, "green");
		return colorGreen;
	}
	displayTextLine(lineNumber+2, "None");
	return colorWhite;
}
