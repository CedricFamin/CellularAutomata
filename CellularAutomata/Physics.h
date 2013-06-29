#pragma once


// retourne la temperature transferer par convection, donc dans l'air
// attention Convection(t1, t2, vicosity) != Convection(t2, t1, vicosity) 
// formule : T = (parTemp1- parTemp2) / parAirViscosity
float Convection(float parTemp1, float parTemp2, float parAirViscosity);

// designe la chaleur transferer a travers un mur 
// formule : T = parTemp - parThickness / (parSurface * parThermalConductivity)
float Conduction(float parTemp, float parSurface, float parThickness, float parThermalConductivity);
