#include "garland_simplification.h"

void main()
{
	GarlandSimplification garland;

	garland.input("bunny.off");
	garland.startSimplification(200);
	garland.drawResult();
}
