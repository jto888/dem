
#include "Maintenance.h"
//#include <math.h>

Maintenance::Maintenance(const std::vector<double> &maint_row)  {								
		
	num = (int) maint_row[0];
	opline =  (int) maint_row[1];
	interval =  (int) maint_row[2];	
	duration =  (int) maint_row[3];					
	first_interval =  (int) maint_row[4];		
				
}				

int Maintenance::getNum() {
	return num;
}

int Maintenance::getOplineNum() {
	return opline;
}

double Maintenance::getInterval() {
	return interval;
}

double Maintenance::getDuration() {
	return duration;
}

double Maintenance::getFirstInterval() {
	return first_interval;
}

// not used
std::vector<double>  Maintenance::nextMaint() {
	std::vector<double> next_maint;
	next_maint.push_back(interval);
	next_maint.push_back(duration);
	return next_maint;	
}

