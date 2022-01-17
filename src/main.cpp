#include "garland_simplification.h"

void main()
{
	GarlandSimplification garland;

	garland.inputMesh("bunny.off");
	if (garland.startSimplification(500))
	{
		garland.drawResult();
	}
}
