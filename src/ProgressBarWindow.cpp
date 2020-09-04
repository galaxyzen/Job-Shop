#include "ProgressBarWindow.h"

ProgressBarWindow::ProgressBarWindow(int maximum, JobshopGUI* gui_src)
{
	this->setWindowTitle("Calculating...");
	this->setFixedSize(800, 200);
	gui = gui_src;

	progressBar = new QProgressBar(this);
	quest = new QLabel(this);
	machineLabel = new QLabel(this);
	starttimeLabel = new QLabel(this);
	durationLabel = new QLabel(this);
	machineLine = new QLineEdit(this);
	starttimeLine = new QLineEdit(this);
	durationLine = new QLineEdit(this);
	submitBut = new QPushButton(this);

	progressBar->setOrientation(Qt::Horizontal);
	progressBar->setGeometry(25, 25, 775, 50);
	progressBar->setRange(0, maximum);
	progressBar->setValue(0);

	quest->setGeometry(300, 90, 999, 30);
	machineLabel->setGeometry(50, 130, 999, 30);
	machineLine->setGeometry(130, 130, 100, 35);
	starttimeLabel->setGeometry(250, 130, 999, 30);
	starttimeLine->setGeometry(360, 130, 100, 35);
	durationLabel->setGeometry(480, 130, 999, 30);
	durationLine->setGeometry(570, 130, 100, 35);
	submitBut->setGeometry(680, 130, 80, 30);

	QFont bFont("Consolas", 16);
	QFont uFont("Consolas", 14);
	quest->setText("SERVICE QUEST");
	quest->setFont(bFont);
	machineLabel->setText("Machine");
	machineLabel->setFont(uFont);
	machineLine->setFont(uFont);
	starttimeLabel->setText("Start Time");
	starttimeLabel->setFont(uFont);
	starttimeLine->setFont(uFont);
	durationLabel->setText("Duration");
	durationLabel->setFont(uFont);
	durationLine->setFont(uFont);
	submitBut->setText("Submit");
	submitBut->setFont(uFont);

	machineLine->setAlignment(Qt::AlignCenter);
	starttimeLine->setAlignment(Qt::AlignCenter);
	durationLine->setAlignment(Qt::AlignCenter);

	connect(submitBut, SIGNAL(clicked()), this, SLOT(sendToServiceList()));
}

ProgressBarWindow::~ProgressBarWindow() {}


void ProgressBarWindow::sendToServiceList()
{
	QString mStr = machineLine->text();
	QString sStr = starttimeLine->text();
	QString dStr = durationLine->text();
	if (mStr == "" || sStr == "" || dStr == "") {
		goto ERROR;
	}
	for (auto ch : mStr) {
		if (!('0' <= ch && ch <= '9') || ch == '.') {
			goto ERROR;
		}
	}
	for (auto ch : sStr) {
		if (!('0' <= ch && ch <= '9') || ch == '.') {
			goto ERROR;
		}
	}
	for (auto ch : dStr) {
		if (!('0' <= ch && ch <= '9') || ch == '.') {
			goto ERROR;
		}
	}
	SRVQUEST srvquest = {
		static_cast<int> (mStr.toDouble() + 0.5),
		static_cast<int> (sStr.toDouble() + 0.5),
		static_cast<int> (dStr.toDouble() + 0.5)
	};
	gui->serviceQuest.push_back(srvquest);
	machineLine->clear();
	starttimeLine->clear();
	durationLine->clear();
	return;

	ERROR:
	QErrorMessage * err = new QErrorMessage(this);
	err->showMessage("Wrong Input. Need Integers");
}
