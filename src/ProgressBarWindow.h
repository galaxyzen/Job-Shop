#pragma once
#include <QtWidgets/QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QErrorMessage>
#include "JobshopGUI.h"

class JobshopGUI;

class ProgressBarWindow : public QMainWindow {
	
	Q_OBJECT

public:

	ProgressBarWindow(int maximum, JobshopGUI* gui);

	~ProgressBarWindow();

	QProgressBar* progressBar;

private slots:

	void sendToServiceList();

private:

	JobshopGUI* gui;

	QLabel* quest;

	QLabel* machineLabel;
	QLabel* starttimeLabel;
	QLabel* durationLabel;
	QLineEdit* machineLine;
	QLineEdit* starttimeLine;
	QLineEdit* durationLine;

	QPushButton* submitBut;

};