#ifndef _directimpact_H
#define _directimpact_H

#include "dem.h"


class	DirectImpact{
	private:
	int target;
	double probability;
	
	public:
	
	DirectImpact(int _target, double  _probability);
// these will be called within OnFailure,  OnRepair, OnMaintStart, and OnMaintEnd	
	int getTarget() ;
	double getProbability();
	
};

#endif	
