#include "Element.h"
#include "ElementList.h"
#include "OpLine.h"

	OpLine::OpLine(std::unique_ptr<ElementList> &_elems, int  _num) {
		num = _num;
		inMaint = 0;
		for(int i=0; i<_elems->getSize(); i++)  {
			if(_elems->getByIndex(i)->getOplineNum() == num) {
				elems.push_back(_elems->getByIndex(i));
			}
		}
	}
	
	
	std::vector<std::shared_ptr<class Element>> OpLine::getElems() {
		return elems;
	}

	int OpLine::getNum() {
		return num;
	}
	
	int OpLine::getStatus() {
		int status=1;
		for(int i=0; i<(int) elems.size(); i++) {
			status = status * elems[i]->getOperable() * elems[i]->getActive();
		}
		return status;
	}
			
	int OpLine::getMaintStatus() {
		return inMaint;
	}
	
	void OpLine::setInMaint(int in_maint) {
		inMaint = in_maint;
	}
	
	Rcpp::IntegerVector OpLine::getCoEnabledDependentRow(int row_num){
		return co_enabled_dependents[row_num];
	}
	
	void OpLine::addDirectDependent(int dep) {
		direct_dependents.push_back(dep);
	}
	
	void OpLine::addCoEnabledDependent(Rcpp::IntegerVector co_enabled_row){
		co_enabled_dependents.push_back(co_enabled_row);
	}

	
/*	
	void OpLine::addDirectImpact(int target, double probability) {
		direct_impacts.push_back(DirectImpact(target, probability));
	}
	
	std::vector<DirectImpact> OpLine::getDirectImpacts() {
		return direct_impacts;
	}
*/