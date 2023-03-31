#ifndef _opline_H
#define _opline_H

#include "dem.h"
//#include "DirectImpact.h"

class OpLine {		
	private:	
		int num;
		int inMaint;
		std::vector<std::shared_ptr<class Element>> elems;
		//std::vector<class DirectImpact> direct_impacts;


	public:	
		OpLine(std::unique_ptr<ElementList> &_elem, int  _num);
		int getStatus();
		std::vector<std::shared_ptr<class Element>> getElems();
		int getNum();
		int getMaintStatus();
		void setInMaint(int);
		//void addDirectImpact(int target, double probability);
		//std::vector<DirectImpact> getDirectImpacts();
};

#endif	
