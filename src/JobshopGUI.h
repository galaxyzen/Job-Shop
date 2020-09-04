#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_JobshopGUI.h"
#include "jobshop.h"
#include "jobshopgui_ga.h"
#include "ProgressBarWindow.h"
#include "JobshopThread.h"
#include "ResultWindow.h"

#include <QTableWidget>
#include <QErrorMessage>
#include <QFileDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <vector>
#include <fstream>

#define GUI
#define RRLE

class JobshopThread;
class ProgressBarWindow;
class ResultWindow;

// ************ MAIN WINDOW ************ //

class JobshopGUI : public QMainWindow
{
	Q_OBJECT

public:
	JobshopGUI(QWidget *parent = Q_NULLPTR);

	const int RUNTIME = 175;

	// modified by progressBarWindow
	DVector<SRVQUEST> serviceQuest;

signals:

	void progressSignal(int i);

	void finishSignal();

private slots:
	
	void on_confirmButton_clicked();

	void on_nextButtons_clicked();

	void on_importFromFileButton_clicked();

	void onCellClicked(int row, int col);

	void on_calculateButton_clicked();

	void pushProgressBar(int i);

	void calculateFinish();
	
private:
	// functions
	void uiSetupAll();
	void connectAll();
	void refreshCell(int row, int col);

	// variables
	int productCount;
	int procedCount;
	int machineCount;
	int cellCount;
	DVector2D<PROCEDURE> jobtable;

	// control variables
	int row;
	int col;

	// UI elements
	Ui::JobshopGUIClass* ui;

	QTableWidget* jobtableTab;
	
	QLabel* productLabel;
	QLabel* procedLabel;
	QLineEdit* productLine;
	QLineEdit* procedLine;
	QPushButton* ppconfirmButton;

	QFrame* upperFrame;

	QLabel* currentProcedLabel;
	QLabel* machineLabel;
	QLabel* durationLabel;
	QLineEdit* machineLine;
	QLineEdit* durationLine;
	QPushButton* nextProcedButton;
	QPushButton* nextProductButton;

	QFrame* lowerFrame;

	QPushButton* importFromFileButton;
	QPushButton* calculateButton;
	QPushButton* aboutButton;

	ProgressBarWindow* progressWindow;

	JobshopThread* jobshopThread;

	ResultWindow* resultWindow;
};

