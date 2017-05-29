#include "colors.h"

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
