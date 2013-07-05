#include <math.h>
#include <assert.h>

#include "Physics.h"

float Convection(float parTemp1, float parTemp2, float parAirViscosity)
{
	return (parTemp2 - parTemp1) / parAirViscosity;
}

float ConductionThoughWall(float parTemp, float parThickness, float parThermalConductivity)
{
	assert(parThermalConductivity != 0.0f);
	return parTemp - parThickness / (parThermalConductivity);
}

float ConductionAirToWall(float parAirTemp, float parWallTemp, float parThermalConductivity)
{
	assert(parThermalConductivity >= 1.0f && parThermalConductivity <= 10.0f);
	return (parWallTemp - parAirTemp) * log10f(parThermalConductivity);
}

float WallAbsorbance(float parWallTemp, float parThermalConductivity)
{
	assert(parThermalConductivity >= 1.0f && parThermalConductivity <= 10.0f);
	float absorbance = 0.01 * parThermalConductivity;
	if (absorbance > parWallTemp)
		return parWallTemp;
	return absorbance;
}