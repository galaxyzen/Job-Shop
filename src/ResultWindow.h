#pragma once
#include "jobshop.h"
#include "jobshopgui_ga.h"
#include "JobshopGUI.h"


class ResultWindow : public QMainWindow {

	Q_OBJECT

public:
	ResultWindow(const DVector2D<MACHINE_TASK>& resTable, const TIME resTableTime,
		const DVector2D<MACHINE_TASK>& resTableWithSvc, const TIME resTableTimeWithSvc,
		double timeUsed, QWidget *parent = Q_NULLPTR);

	~ResultWindow();

private slots:
	void paintEvent(QPaintEvent*);

	void onDynamicButtonClicked();

	void onResetButtonClicked();

	void onServiceButtonClicked();

	void onExPicFileButtonClicked();

	void onExTxtFileButtonClicked();

private:
	/************** PAINT ***************/
	QColor getColor(const int machine) const;

	DVector2D<MACHINE_TASK> restab;
	TIME restime;
	DVector2D<MACHINE_TASK> restabSvc;
	TIME restimeSvc;
	double timeUsed;
	unsigned int machineCount;
	unsigned int procedCount;

	DVector<QLabel*> machineLabel;
	DVector<QLabel*> workingLabel;
	QTimer* timer;
	QPixmap zebra;

	// window control
	const int WID = 1300;
		  int HEI;
	const int MARGINU = 15;
	const int MARGINL = 15;
	const int MARGINR = MARGINL;

	// draw area control
	const int DRAWU = MARGINU + 30;
	const int DRAWL = MARGINL + 150;
	const int AXISH = 32;
	      int COMMN;

	// blocks control
	bool inServiceMode;
	const int VCAPI = 35;
	const int VSIZE = 25;
	double UN; // unit length
	double UNS; // w/ src

	struct MARKPOINT {
		int coord;
		int value;
	};
	MARKPOINT markPoint[7];
	MARKPOINT markPointSvc[7]; // w/src

	int p; // linepos
	int q; // linepos w/svc

	/************* SOMETHING **************/
	QPushButton* dynamicButton;
	QPushButton* resetButton;
	QPushButton* serviceButton;
	QLabel* timeShowLabel;
	bool doingDynamic;

	QPixmap gantt;
	QPixmap ganttSvc;
	QPushButton* exPicFileButton;
	QPushButton* exTxtFileButton;
};