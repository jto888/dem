#ifndef _dem_H
#define _dem_H

#include <RcppArmadillo.h>
//#include <Rcpp.h>
#include <vector>
#include <memory>
#include <string>

#include "Element.h"
#include "ElementList.h"
#include "EventQueue.h"
#include "OpLine.h"
#include "OpLineList.h"
#include "DiscreteEvent.h"

#include "History.h"
#include "Summary.h"
//#include "Maintenance.h"
//#include "MaintList.h"
//#include "Actions.h"
//#include "DirectImpact.h"

unsigned const END_SIM= 0;
unsigned const FAILURE=1;
unsigned const REPAIR=2;

RcppExport SEXP Basicdem( SEXP, SEXP, SEXP, SEXP);
RcppExport SEXP DEMwDep(SEXP, SEXP, SEXP, SEXP, SEXP);

//RcppExport SEXP demwMaint(SEXP, SEXP, SEXP, SEXP);
//RcppExport SEXP demwActionswMaint(SEXP, SEXP, SEXP, SEXP, SEXP);


void OnFailure(std::shared_ptr<DiscreteEvent>& ev,
		std::unique_ptr<ElementList>& EL,
		std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL);
		
void handleInlineFailure(std::shared_ptr<DiscreteEvent>& ev,
		std::unique_ptr<ElementList>& EL,
		std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL);
void OnRepair(std::shared_ptr<DiscreteEvent>& ev,
		std::unique_ptr<ElementList>& EL,
		std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL);
		
void handleInlineRepair(std::shared_ptr<DiscreteEvent>& ev,
		std::unique_ptr<ElementList>& EL,
		std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL);

void recursive_dependency_activator(
		std::shared_ptr<OpLineList>& OLL, 
		int this_opline);

void recursive_dependency_passivator(
		//std::unique_ptr<ElementList>& EL,
		std::shared_ptr<OpLineList>& OLL, 
		int this_opline);

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

