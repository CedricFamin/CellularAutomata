#pragma once


// retourne la temperature transferer par convection, donc dans l'air
// attention Convection(t1, t2, vicosity) != Convection(t2, t1, vicosity) 
// formule : T = (parTemp1- parTemp2) / parAirViscosity
float Convection(float parTemp1, float parTemp2, float parAirViscosity);

// designe la chaleur transferer a travers un mur 
// formule : T = parTemp - parThickness / (parSurface * parThermalConductivity)
float ConductionThoughWall(float parTemp, float parSurface, float parThickness, float parThermalConductivity);

// chaleur transferer Air => mur
// Attention ConductionAirToWall(t1, t2, conductivity) != ConductionAirToWall(t2, t1, conductivity)
// forumule : =(parAirTemp -parWallTemp) * Log10(parThermalConductivity)
float ConductionAirToWall(float parAirTemp, float parWallTemp, float parThermalConductivity);

// temperature prelever par la mur quand de la temperature rentre dedans
// formule : absorbance = 1 - 0.1 * parThermalConductivity
float WallAbsorbance(float parWallTemp, float parThermalConductivity);

