#include "colors.h"

TLegoColors getColorNameByHSB(tSensors nDeviceIndex)
{
	long hue = getColorHue(nDeviceIndex);
	long sat = getColorSaturation(nDeviceIndex);
	long val = getColorAmbient(nDeviceIndex);

	displayString(2, "H: %d; S: %d; V: %d", hue, sat, val);

	if (val < 30)
		return colorBlack;

	else if (hue >= 90
		&& hue <= 150)
	{
		return colorGreen;
	}

	return colorWhite;
}
