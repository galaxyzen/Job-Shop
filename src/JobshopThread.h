#pragma once
#include <QThread>
#include "jobshop.h"
#include "jobshopgui_ga.h"
#include "JobshopGUI.h"

class JobshopGUI;

class JobshopThread : public QThread {
	
	Q_OBJECT

protected:

	JobshopGUI* gui;

	CHROMO best;

	DVector2D<PROCEDURE> jobtable;

	unsigned int runtime;

	void jobshop_GA_main();

public:

	JobshopThread(const DVector2D<PROCEDURE>& jobtable, const JobshopGUI* gui,
		QWidget *parent = Q_NULLPTR);

	~JobshopThread();

	void run();

	double totalRuntime();
	
	CHROMO& result();
};

