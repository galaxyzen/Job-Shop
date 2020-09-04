#pragma once
#include "jobshop.h"
#include "JobshopGUI.h"

/**********module global vals ***********/
extern int product_count;
extern int proced_count;
extern int chromo_len;
extern int machine_count;

/**********************************/

CHROMO jobshop_GA_GUI(const DVector2D<PROCEDURE>& jobtable);

/**********public funcs***********/

void init_population(DVector<CHROMO>& population, const DVector2D<PROCEDURE>& jobtable);

void communication(DVector<CHROMO>* pop);

void selfing_doubleseg(const DVector2D<PROCEDURE>& jobtable, DVector<CHROMO>& population);

void filter_by_sort(DVector<CHROMO>& population);

void mutate_six(const DVector2D<PROCEDURE>& jobtable, DVector<CHROMO>& population);

#ifndef GUI
CHROMO jobshop_GA(const DVector2D<PROCEDURE>& jobtable);
void crossover(const DVector2D<PROCEDURE>& jobtable, DVector<CHROMO>& population);
void selfing(const DVector2D<PROCEDURE>& jobtable, DVector<CHROMO>& population);
void selfing_multi(const DVector2D<PROCEDURE>& jobtable, DVector<CHROMO>& population);
void selfing_multi_pickbysort(const DVector2D<PROCEDURE>& jobtable, DVector<CHROMO>& population);
void filter(DVector<CHROMO>& population);
void filter_roulette(DVector<CHROMO>& population);
void filter_tournament(DVector<CHROMO>& population);
void mutate(DVector<CHROMO>& population);
void mutate_all(const DVector2D<PROCEDURE>& jobtable, DVector<CHROMO>& population);
#endif

/**********private funcs**********/

TIME chromo_time(const DVector2D<PROCEDURE>& jobtable, const DVector<int>& chromo, int flag);
TIME chromo_time_with_service(const DVector2D<PROCEDURE>& jobtable, const DVector<SRVQUEST>& srvquest_src,
	const DVector<int>& chromo, int flag);

DVector2D<MACHINE_TASK> get_result_table(const DVector2D<PROCEDURE>& jobtable, const CHROMO& CHROMO);
DVector2D<MACHINE_TASK> get_result_table_with_service(
	const DVector2D<PROCEDURE>& jobtable, const DVector<SRVQUEST>& srvquest, const CHROMO& CHROMO);

// QSort
int partition(DVector<CHROMO>& population, int lo, int hi);
void qsort(DVector<CHROMO>& population, int lo, int hi);