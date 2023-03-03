#include "dem.h"
#include <string>


History::History(std::shared_ptr<OpLineList>& OLL,
				 std::shared_ptr<EventQueue>& EQ,
				 SEXP margin_in) {
		using namespace Rcpp;					
						
	_OLL = OLL;
	_EQ = EQ;
	double margin=Rcpp::as<double>(margin_in);		
//	Rcout<< "margin    "<< margin << std::endl;
//	showZeros=Rcpp::as<int>(show_zeros);
	showZeros = 1;
	
// an initilization that would be implied on Windows build, but required on Linux	
		pos=0;
// need to get the size of all fail rands multiply by 2 and a head factor
		int rand_size=0;		
		for(int i=0; i<OLL->getSize(); i++)  {		
			for(int j=0; j< (int) OLL->getByIndex(i)->getElems().size(); j++) {	
				rand_size=rand_size + OLL->getByIndex(i)->getElems()[j]->numRands();
			}	
		}
// a headspace of 15% is applied here, perhaps this needs to be part of 
// a simulation control along with years_in			
		//int hist_size = (int)  rand_size*2*1.15;
		hist_size = (int)  rand_size*2*(1+margin);
		
//		std::vector<double> arr1(hist_size,0.0);	
//		_time = Rcpp::wrap(arr1);	
		__time.set_size(hist_size);

//		std::vector<double> arr2(hist_size,0.0);		
//		_duration = Rcpp::wrap(arr2);
		__duration.set_size(hist_size);
			
		for(int i=0; i<OLL->getSize(); i++)  {	
			std::vector<int> arr(hist_size,0);	
			detail.push_back(Rcpp::wrap(arr));
		}
		
		  __detail.zeros(hist_size, OLL->getSize());
		
// This was a test/inspection feature no longer required		
//		std::vector<std::string> arr3(hist_size,"");
//		_queue = Rcpp::wrap(arr3);
		
//update function will incorporate this code given the event time
	double eventTime=0.0;
//	_time[pos] = eventTime;	
	__time(pos) = eventTime;

//	_duration[pos] = 0.0;
	__duration(pos) = 0.0;
	
	for(int i=0; i<OLL->getSize(); i++)  {
		detail[i][pos] = OLL->getByIndex(i)->getStatus();
	}
	
	for(int i=0; i< _OLL->getSize(); i++)  {
		__detail(pos,i) = _OLL->getByIndex(i)->getStatus();
	}
	
// This was a test/inspection feature no longer required	
//	_queue[pos] = EQ->writeQue();
}


// These ndividual vectors were built as stand alone functions for testing purposes
//  but now they are preferred for conversion of arma objects to SEXP for return to R.

Rcpp::NumericVector History::getTime() {	
	//Rcpp::NumericVector get_time(_time.begin(), std::next(_time.begin(), pos+1));
	
	//caused system crash:  __time.shed_rows(pos+1, hist_size);   
	__time.shed_rows(pos+1, hist_size-1);   
	Rcpp::NumericVector get_time(Rcpp::wrap(__time));
	
	return get_time;
}

Rcpp::NumericVector History::getDuration() {	
	//Rcpp::NumericVector get_duration(_duration.begin(), std::next(_duration.begin(), pos+1));
		__duration.shed_rows(pos+1, hist_size-1);   
	Rcpp::NumericVector get_duration(Rcpp::wrap(__duration));

	return get_duration;
}

arma::umat History::get__detail(){	
	__detail.shed_rows(pos+1, hist_size-1); 
	return __detail;
}	

/*
Rcpp::StringVector History::getQueue() {
	Rcpp::StringVector get_queue(_queue.begin(), std::next(_queue.begin(), pos+1));
	return get_queue;
}


//getDetail method
std::vector<Rcpp::IntegerVector> History::getDetail() {
    std::vector<Rcpp::IntegerVector>ret_detail;
	for(int i=0; i< _OLL->getSize(); i++)  {
		Rcpp::IntegerVector temp_detail(detail[i].begin(), std::next(detail[i].begin(), pos+1));
		ret_detail.push_back(temp_detail);
	 }
	 return ret_detail;
}
*/



// for large filled vectors it may be more efficient to erase elements from the end
// rather than copy elements to new containers (but for test development this works!)
Rcpp::List History::historyList() {
	Rcpp::List retval;	
	retval.push_back(getTime(), "Time");	
	retval.push_back(getDuration(), "Duration");
/*	
	for(int i=0; i< _OLL->getSize(); i++) {	
		std::string colName="Train"+ std::to_string(_OLL->getByIndex(i)->getNum() );
		Rcpp::IntegerVector temp_detail(detail[i].begin(), std::next(detail[i].begin(), pos+1));
		retval.push_back(temp_detail, colName);
	}	
*/
	
// This was a test/inspection feature no longer required		
//	Rcpp::StringVector getQueue(_queue.begin(), std::next(_queue.begin(), pos+1));
//	retval.push_back(getQueue, "Queue");
    //retval.push_back(getTime(), "__time");
	return retval;
}

void History::setLastDuration(double time_now) {
	__duration(pos) = time_now - __time(pos);
}

int History::update(std::shared_ptr<DiscreteEvent>& ev ) {
	double time_now = ev->getTime();
	int headspace = hist_size - (pos+1);
	if(headspace > 0) {
		if(ev->getType() > 0) {
			if(__duration[pos]>0 || showZeros==1) {
//			if(__duration[pos]>0) {
				pos = pos +1;
				__time(pos) = time_now;
			}

			for(int i=0; i< _OLL->getSize(); i++)  {
				detail[i][pos] = _OLL->getByIndex(i)->getStatus();
			}
			
			for(int i=0; i< _OLL->getSize(); i++)  {
				__detail(pos,i) = _OLL->getByIndex(i)->getStatus();
			}
			
// This was a test/inspection feature no longer required				
//			_queue[pos] = _EQ->writeQue();
		}
	}
	
	return headspace;	
}
	
	
	

	
	
	
