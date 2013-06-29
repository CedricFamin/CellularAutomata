#include "Physics.h"

float Convection(float parTemp1, float parTemp2, float parAirViscosity)
{
	return (parTemp2 - parTemp1) / parAirViscosity;
}

float Conduction(float parTemp, float parSurface, float parThickness, float parThermalConductivity)
{
	return parTemp - parThickness / (parSurface * parThermalConductivity);
}