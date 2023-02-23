#include "dem.h"

Summary::Summary(std::shared_ptr<OpLineList>& OLL) {
	_OLL = OLL;
	
//build the vectors that will comprise the summary dataframe	
	for(int i=0; i<OLL->getSize(); i++)  {		
		for(int j=0; j< (int) OLL->getByIndex(i)->getElems().size(); j++) {	
			OpLine.push_back(OLL->getByIndex(i)->getElems()[j]->getOplineNum());
			ElementID.push_back(OLL->getByIndex(i)->getElems()[j]->getID());
/*			
			fcase.push_back(OLL->getByIndex(i)->getElems()[j]->getfcase());
			fp1.push_back(OLL->getByIndex(i)->getElems()[j]->getfp1());
			fp2.push_back(OLL->getByIndex(i)->getElems()[j]->getfp2());
			fp3.push_back(OLL->getByIndex(i)->getElems()[j]->getfp3());
			rcase.push_back(OLL->getByIndex(i)->getElems()[j]->getrcase());
			rp1.push_back(OLL->getByIndex(i)->getElems()[j]->getrp1());
			rp2.push_back(OLL->getByIndex(i)->getElems()[j]->getrp2());
			rp3.push_back(OLL->getByIndex(i)->getElems()[j]->getrp3());
			seed.push_back(OLL->getByIndex(i)->getElems()[j]->getseed());
*/			
			num_fails.push_back(OLL->getByIndex(i)->getElems()[j]->numFails());
			repair_hrs.push_back(OLL->getByIndex(i)->getElems()[j]->sumRepairs());
			rand_size.push_back(OLL->getByIndex(i)->getElems()[j]->numRands());
			fwrap.push_back(OLL->getByIndex(i)->getElems()[j]->getfwrap());	
		}	
	}
}

Rcpp::List Summary::summaryDF() {
	Rcpp::List  retval;
		retval.push_back(OpLine, "OpLine");
		retval.push_back(ElementID, "ElementID");
/*		
		retval.push_back(fcase, "fcase");
		retval.push_back(fp1, "fp1");
		retval.push_back(fp2, "fp2");
		retval.push_back(fp3, "fp3");
		retval.push_back(rcase, "rcase");
		retval.push_back(rp1, "rp1");
		retval.push_back(rp2, "rp2");
		retval.push_back(rp3, "rp3");
		retval.push_back(seed, "seed");
*/		
		retval.push_back(num_fails, "num_fails");
		retval.push_back(repair_hrs, "repair_hrs");
		retval.push_back(rand_size, "rand_size");
		retval.push_back(fwrap, "fwrap");
	return retval;
}