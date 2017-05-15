#include "colors.h"

TLegoColors getColorNameByHSB(tSensors nDeviceIndex, int lineNumber)
{
	long hue = getColorHue(nDeviceIndex);
	//long sat = getColorSaturation(nDeviceIndex);
	short val = getColorAmbient(nDeviceIndex);

	displayString(lineNumber, "H: %d; S: %d; V: %d", hue, 0, val);

	if(hue > 20 && hue < 35)
		return colorWhite;
	if(hue >= 35 && hue < 70)
		return colorBlack;
	if(hue > 110 && hue < 130)
		return colorGreen;
	return colorNone;
}

TLegoColors getLegoColorFromRGB(tSensors nDevice)
{
	long red;
	long green;
	long blue;

	getColorRGB(nDevice, red, green, blue);

	displayString(4, "R: %d", red);
	displayString(5, "G: %d", green);
	displayString(6, "B: %d", blue);

	if(red > 225 && green > 225 && blue > 225)
		return colorWhite;
	if(red < 30 && green < 30 && blue < 30)
		return colorBlack;
	if(green > red && green > blue)
		return colorGreen;
	return colorNone;
}
