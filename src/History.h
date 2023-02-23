#ifndef _history_H
#define _history_H

#include "dem.h"

class History {		
private:	
	int pos;
	int hist_size;
	Rcpp::NumericVector _time;
	arma::colvec __time;

	Rcpp::NumericVector _duration;
	arma::colvec __duration;
	
	std::vector<Rcpp::IntegerVector> detail;
	arma::umat __detail;

	Rcpp::StringVector _queue;
	std::shared_ptr<OpLineList> _OLL;
	std::shared_ptr<EventQueue> _EQ;
	
public:	
	History(std::shared_ptr<OpLineList>& OLL,
		std::shared_ptr<EventQueue>& EQ,
		SEXP margin_in);
	void setLastDuration(double time);
	int update(std::shared_ptr<DiscreteEvent>& ev);
	Rcpp::List historyList();
	
	// test methods
	int getPos();
	int getSize();
	Rcpp::NumericVector getTime();
	Rcpp::NumericVector getDuration();
	std::vector<Rcpp::IntegerVector> getDetail();
//	Rcpp::IntegerVector getDetailCol(int col);
	Rcpp::IntegerVector getDetailCol(int col);
	Rcpp::StringVector getQueue();
	arma::umat get__detail();
	

		
		
};

#endif	
