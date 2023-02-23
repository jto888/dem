#ifndef _maintenance_H
#define _maintenance_H

#include "dem.h"

class Maintenance {		
	private:	
		int num;
		int opline;
		double interval;
		double duration;
		double first_interval;

	public:	
		Maintenance(const std::vector<double> &maint_row);
// the next Maint should probably be a std::vector<double> to hold 2 values		
		std::vector<double>  nextMaint();

		int getNum();
		int getOplineNum();
		double getInterval();
		double getDuration();
		double getFirstInterval();
};

#endif	
