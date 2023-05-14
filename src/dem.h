#ifndef _dem_H
#define _dem_H

#include <RcppArmadillo.h>
//#include <Rcpp.h>
#include <vector>
#include <memory>
#include <string>

#include "Element.h"
#include "ElementList.h"
#include "OpLine.h"
#include "OpLineList.h"
#include "DiscreteEvent.h"
#include "EventQueue.h"
#include "History.h"
#include "Summary.h"
#include "FailureRepairAndMaintenance.h"
//#include "Maintenance.h"
//#include "MaintList.h"
//#include "Actions.h"
//#include "DirectImpact.h"

unsigned const END_SIM= 0;
unsigned const FAILURE=1;
unsigned const REPAIR=2;
unsigned const MAINT_START=3;
unsigned const MAINT_END=4;

RcppExport SEXP Basicdem( SEXP, SEXP, SEXP, SEXP);
RcppExport SEXP DEMwDep(SEXP, SEXP, SEXP, SEXP, SEXP);
RcppExport SEXP DEMwMaint(SEXP, SEXP, SEXP, SEXP, SEXP);
RcppExport SEXP DEMwMaintwDep(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);

//RcppExport SEXP demwMaint(SEXP, SEXP, SEXP, SEXP);
//RcppExport SEXP demwActionswMaint(SEXP, SEXP, SEXP, SEXP, SEXP);



/*		
void OnMaintStart(std::shared_ptr<DiscreteEvent>& ev,
		//std::unique_ptr<ElementList>& EL,
		//std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL);	
		
void OnMaintEnd(std::shared_ptr<DiscreteEvent>& ev,
		std::unique_ptr<MaintList>& ML,
		std::unique_ptr<ElementList>& EL,
		std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL);					
*/
		
double getProbability();
		
#endif

