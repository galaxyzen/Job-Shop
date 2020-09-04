#include "JobshopGUI.h"

JobshopGUI::JobshopGUI(QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::JobshopGUIClass)
{
	ui->setupUi(this);

	uiSetupAll();

	update();

	connectAll();

	productCount = procedCount = machineCount = 0;

	cellCount = row = col = 0;
}


/**************************** PRIVATE SLOTS ****************************/

void JobshopGUI::on_confirmButton_clicked()
{
	QString s1 = productLine->text();
	QString s2 = procedLine->text();
	if (s1 == "" || s2 == "") {
		goto ERROR;
	}
	for (auto ch : s1) {
		if (!('0' <= ch && ch <= '9') || ch == '.') {
			goto ERROR;
		}
	}
	for (auto ch : s2) {
		if (!('0' <= ch && ch <= '9') || ch == '.') {
			goto ERROR;
		}
	}
	productCount = static_cast<int> (s1.toDouble() + 0.5);
	procedCount = static_cast<int> (s2.toDouble() + 0.5);
	if (productCount == 0 || procedCount == 0) {
		goto ERROR_ZERO;
	}
	machineCount = procedCount;
	jobtable.resize(productCount);
	for (auto i = 0; i < productCount; ++i) {
		jobtable[i].resize(procedCount);
	}

	productLine->setDisabled(true);
	procedLine->setDisabled(true);
	ppconfirmButton->setDisabled(true);

	machineLine->setDisabled(false);
	nextProductButton->setDisabled(false);
	durationLine->setDisabled(false);
	nextProcedButton->setDisabled(false);

	jobtableTab->setRowCount(productCount);
	jobtableTab->setColumnCount(procedCount);
	jobtableTab->setEditTriggers(QAbstractItemView::NoEditTriggers);

	for (auto i = 0; i < productCount; ++i) {
		QTableWidgetItem* header = new QTableWidgetItem(QString::number(i));
		jobtableTab->setHorizontalHeaderItem(i, header);
	}
	for (auto i = 0; i < procedCount; ++i) {
		QTableWidgetItem* header = new QTableWidgetItem(QString::number(i));
		jobtableTab->setVerticalHeaderItem(i, header);
	}

	for (auto i = 0; i < productCount; ++i) {
		for (auto j = 0; j < procedCount; ++j) {
			jobtableTab->setItem(i, j, new QTableWidgetItem);
			jobtableTab->item(i, j)->setTextAlignment(Qt::AlignCenter);
		}
	}
	jobtableTab->item(0, 0)->setBackgroundColor(Qt::yellow);

	importFromFileButton->setDisabled(true);

	currentProcedLabel->setText("Current: Product 00 / Procedure 00");

	return;

	ERROR:
	QErrorMessage* err = new QErrorMessage(this);
	err->showMessage("Wrong Input. Need Integers");
	return;

	ERROR_ZERO:
	QErrorMessage * err2 = new QErrorMessage(this);
	err2->showMessage("Wrong Input. Can't be 0");
} // on_confirmButton_clicked


void JobshopGUI::on_nextButtons_clicked()
{
	QPushButton* src = qobject_cast<QPushButton*>(sender());

	QString mStr = machineLine->text();
	QString dStr = durationLine->text();
	if (mStr == "" || dStr == "") {
		goto ERROR;
	}
	for (auto ch : mStr) {
		if (!('0' <= ch && ch <= '9') || ch == '.') {
			goto ERROR;
		}
	}
	for (auto ch : dStr) {
		if (!('0' <= ch && ch <= '9') || ch == '.') {
			goto ERROR;
		}
	}

	if (row == productCount) {
		goto ERROR_END;
	}
	jobtable[row][col].machine = static_cast<int> (mStr.toDouble() + 0.5);
	jobtable[row][col].duration = static_cast<int> (dStr.toDouble() + 0.5);
	machineLine->clear();
	durationLine->clear();

	refreshCell(row, col);
	if (row != productCount) {
		jobtableTab->item(row, col)->setBackgroundColor(Qt::white);
	}
	if (jobtableTab->item(row, col)->text() != "") {
		++cellCount;
	}

	// change row, col
	if (src->objectName() == "nextProced") {
		if (col == procedCount - 1) {
			col = 0;
			++row;
		}
		else {
			++col;
		}
	}
	else if (src->objectName() == "nextProduct") {
		++row;
		col = 0;
	}
	// change row, col end
	currentProcedLabel->setText(
		"Current: Product " + QString("%1").arg(row, 2, 10, QChar('0'))
		+ " / Procedure " + QString("%1").arg(col, 2, 10, QChar('0'))
	);

	if (cellCount == productCount * procedCount) {
		calculateButton->setDisabled(false);
	}
	else {
		jobtableTab->item(row, col)->setBackgroundColor(Qt::yellow);
	}

	return;

	ERROR:
	QErrorMessage * err = new QErrorMessage(this);
	err->showMessage("Wrong Input. Need Integers");
	return;

	ERROR_END:
	QErrorMessage * err2 = new QErrorMessage(this);
	err2->showMessage("Editing Reached the End");
} // on_nextButtons_clicked


void JobshopGUI::on_importFromFileButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open File", "C:", "All Files (*.*)");
	std::ifstream fin(fileName.toStdString(), std::ios_base::in);
	if (!fin.is_open()) {
		goto ERROR_OPENFAIL;
	}
	fin >> productCount >> procedCount;
	machineCount = procedCount;

	jobtable.resize(productCount);
	for (auto i = 0; i < productCount; ++i) {
		jobtable[i].resize(procedCount);
	}
	jobtableTab->setRowCount(productCount);
	jobtableTab->setColumnCount(procedCount);
	jobtableTab->setEditTriggers(QAbstractItemView::NoEditTriggers);

	for (auto i = 0; i < productCount; ++i) {
		for (auto j = 0; j < procedCount; ++j) {
			fin >> jobtable[i][j].machine >> jobtable[i][j].duration;
			jobtableTab->setItem(i, j, new QTableWidgetItem);
			jobtableTab->item(i, j)->setTextAlignment(Qt::AlignCenter);
			jobtableTab->item(i, j)->setText(
				QString::number(jobtable[i][j].machine) + " - " + QString::number(jobtable[i][j].duration)
			);
		}
	}
	fin.close();

	productLine->setText(QString::number(productCount));
	productLine->setDisabled(true);
	procedLine->setText(QString::number(procedCount));
	procedLine->setDisabled(true);
	ppconfirmButton->setDisabled(true);
	calculateButton->setDisabled(false);
	jobtableTab->item(row, col)->setBackgroundColor(Qt::white);

	return;

	ERROR_OPENFAIL:
	QErrorMessage * err = new QErrorMessage(this);
	err->showMessage("File Open Failed!");
	return;
} // on_importFromFileButton_clicked


void JobshopGUI::onCellClicked(int r, int c)
{
	if (row != productCount) {
		jobtableTab->item(row, col)->setBackgroundColor(Qt::white);
	}
	row = r;
	col = c;
	jobtableTab->item(row, col)->setBackgroundColor(Qt::yellow);

	currentProcedLabel->setText(
		"Current: Product " + QString("%1").arg(row, 2, 10, QChar('0'))
		+ " / Procedure " + QString("%1").arg(col, 2, 10, QChar('0'))
	);
}


void JobshopGUI::on_calculateButton_clicked()
{
	serviceQuest.clear();
	progressWindow = new ProgressBarWindow(RUNTIME, this);
	progressWindow->show();

	/*for (clock_t t = clock(); clock() - t < 10; ) {
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
	}*/

	jobshopThread = new JobshopThread(jobtable, this);
	jobshopThread->start();
}


void JobshopGUI::pushProgressBar(int i)
{
	progressWindow->progressBar->setValue(i);
}


void JobshopGUI::calculateFinish()
{
	progressWindow->close();
	CHROMO bestChromo = jobshopThread->result();
	double timeUsed = jobshopThread->totalRuntime();
	
	DVector2D<MACHINE_TASK> restabSvc = get_result_table_with_service(jobtable, serviceQuest, bestChromo);
	TIME restimeSvc = -1;
	for (auto i = 0; i < restabSvc.size(); ++i) {
		if (restabSvc[i].back().end > restimeSvc) {
			restimeSvc = restabSvc[i].back().end;
		}
	}
	resultWindow = new ResultWindow(
		get_result_table(jobtable, bestChromo), chromo_time(jobtable, bestChromo.chromo, 1),
		restabSvc, restimeSvc,
		timeUsed, this
	);
	resultWindow->show();
}


/******************************* PRIVATE *******************************/

void JobshopGUI::refreshCell(int row, int col)
{
	jobtableTab->setEditTriggers(QAbstractItemView::DoubleClicked);

	jobtableTab->item(row, col)->setText(
		QString::number(jobtable[row][col].machine) + " - " + QString::number(jobtable[row][col].duration)
	);

	jobtableTab->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void JobshopGUI::connectAll()
{
	connect(ppconfirmButton, SIGNAL(clicked()), this, SLOT(on_confirmButton_clicked()));
	
	connect(nextProcedButton, SIGNAL(clicked()), this, SLOT(on_nextButtons_clicked()));

	connect(nextProductButton, SIGNAL(clicked()), this, SLOT(on_nextButtons_clicked()));

	connect(importFromFileButton, SIGNAL(clicked()), this, SLOT(on_importFromFileButton_clicked()));

	connect(jobtableTab, SIGNAL(cellClicked(int, int)), this, SLOT(onCellClicked(int, int)));

	connect(calculateButton, SIGNAL(clicked()), this, SLOT(on_calculateButton_clicked()));

	connect(this, SIGNAL(progressSignal(int)), SLOT(pushProgressBar(int)));

	connect(this, SIGNAL(finishSignal()), SLOT(calculateFinish()));
}


void JobshopGUI::uiSetupAll()
{
	this->setWindowTitle("Jobshop-211");
	this->setFixedSize(750, 720);

	QFont globalFont("Consolas", 15);
	this->setFont(globalFont);

	jobtableTab = new QTableWidget(this);

	productLabel = new QLabel(this);
	procedLabel = new QLabel(this);
	productLine = new QLineEdit(this);
	procedLine = new QLineEdit(this);
	ppconfirmButton = new QPushButton(this);
	upperFrame = new QFrame(this);

	currentProcedLabel = new QLabel(this);
	machineLabel = new QLabel(this);
	machineLine = new QLineEdit(this);
	durationLabel = new QLabel(this);
	durationLine = new QLineEdit(this);
	nextProcedButton = new QPushButton(this);
	nextProductButton = new QPushButton(this);
	lowerFrame = new QFrame(this);

	importFromFileButton = new QPushButton(this);
	calculateButton = new QPushButton(this);
	aboutButton = new QPushButton(this);

	static const int UNLMT = 9999;
	jobtableTab->setGeometry(25, 30, 700, 350);

	productLabel->setGeometry(40, 400, UNLMT, 30);
	productLine->setGeometry(200, 400, 85, 30);
	procedLabel->setGeometry(300, 400, UNLMT, 30);
	procedLine->setGeometry(480, 400, 85, 30);
	ppconfirmButton->setGeometry(600, 400, 100, 30);

	upperFrame->setGeometry(25, 445, 700, 10);
	upperFrame->setFrameStyle(QFrame::HLine);

	currentProcedLabel->setGeometry(160, 460, UNLMT, 40);
	machineLabel->setGeometry(70, 520, UNLMT, 30);
	machineLine->setGeometry(240, 520, 130, 30);
	durationLabel->setGeometry(415, 520, UNLMT, 30);
	durationLine->setGeometry(530, 520, 130, 30);

	nextProcedButton->setGeometry(150, 580, 180, 30);
	nextProductButton->setGeometry(420, 580, 170, 30);

	lowerFrame->setGeometry(25, 630, 700, 10);
	lowerFrame->setFrameStyle(QFrame::HLine);

	QFont smallButtonFont("Consolas", 12);
	QFont bigButtonFont("Consolas", 18);
	importFromFileButton->setGeometry(100, 665, 150, 25);
	importFromFileButton->setFont(smallButtonFont);
	calculateButton->setGeometry(300, 660, 150, 35);
	calculateButton->setFont(bigButtonFont);
	aboutButton->setGeometry(500, 665, 150, 25);
	aboutButton->setFont(smallButtonFont);

	productLine->setAlignment(Qt::AlignCenter);
	procedLine->setAlignment(Qt::AlignCenter);
	machineLine->setAlignment(Qt::AlignCenter);
	durationLine->setAlignment(Qt::AlignCenter);

	productLabel->setText("Product Number");
	procedLabel->setText("Procedure Number");
	ppconfirmButton->setText("Confirm");
	currentProcedLabel->setText("Current: Product -- / Procedure --");
	machineLabel->setText("Machine Demand");
	durationLabel->setText("Time Cost");
	nextProcedButton->setText("Next Procedure");
	nextProductButton->setText("Next Product");
	importFromFileButton->setText("Import From File");
	calculateButton->setText("Calculate");
	aboutButton->setText("About Authors");

	machineLine->setDisabled(true);
	nextProductButton->setDisabled(true);
	durationLine->setDisabled(true);
	nextProcedButton->setDisabled(true);
	calculateButton->setDisabled(true);

	nextProcedButton->setObjectName("nextProced");
	nextProductButton->setObjectName("nextProduct");

} // uiSetupAll
