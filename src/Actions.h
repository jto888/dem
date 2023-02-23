#ifndef _actions_H
#define _actions_H

#include "dem.h"

class Actions{	
	private:	
	std::vector<std::vector<double>> action_rows;
	
	public:
	Actions(std::shared_ptr<OpLineList>& OLL, SEXP actions_in);
	std::vector<std::vector<double>> getActionRows();
};
/*
class	DirectImpact  {
	private:
	int target;
	double probability;
	
	public:
	
	void DirectImpact(int _target, double  _probability);
// these will be called within OnFailure,  OnRepair, OnMaintStart, and OnMaintEnd	
	int getTarget() ;
	double getProbability();
	
};	


class	CombinationAction  {
	private:
	std::vector<int> combining_oplines;
	int target;
	double probability;
	
	public:
	CombinationAction(std::vector<int> combiners, int _target, double _probability);
	
	std::vector<int> getCombiners();
	intgetTarget();
	double getProbability();
	
};	
*/
#endif	
