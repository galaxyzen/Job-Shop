#include "ResultWindow.h"

ResultWindow::ResultWindow(const DVector2D<MACHINE_TASK>& restab_src, const TIME restime_src,
	const DVector2D<MACHINE_TASK>& restabSvc_src, const TIME restimeSvc_src,
	double timeUsed_src, QWidget *parent)
{
	restab = restab_src;
	restime = restime_src;
	restabSvc = restabSvc_src;
	restimeSvc = restimeSvc_src;
	timeUsed = timeUsed_src;
	machineCount = restab.size();
	procedCount = restab[0].size();

	HEI = machineCount * VCAPI + 180;
	inServiceMode = false;
	UN = static_cast<double>(WID - DRAWL - MARGINR) / restime * 0.95;
	UNS = static_cast<double>(WID - DRAWL - MARGINR) / restimeSvc * 0.95;
	COMMN = machineCount * (VCAPI + 1) + 50 + AXISH;

#ifdef DEBUG
	if (!zebra.load("J:\\zebra.jpg")) std::terminate();
#endif
#ifndef DEBUG
	zebra.load("zebra.jpg");
#endif
	zebra = zebra.scaled(9999, VSIZE, Qt::KeepAspectRatio);
	
	for (auto i = 0; i < 7; ++i) {
		markPoint[i].coord = DRAWL + (i + 1) * UN * int(restime / 80 * 10);
		markPoint[i].value = (i + 1) * int(restime / 80 * 10);

		markPointSvc[i].coord = DRAWL + (i + 1) * UNS * int(restimeSvc / 80 * 10);
		markPointSvc[i].value = (i + 1) * int(restimeSvc / 80 * 10);
	}

	machineLabel.resize(machineCount);
	for (auto i = 0; i < machineCount; ++i) {
		machineLabel[i] = new QLabel(this);
		machineLabel[i]->setGeometry(MARGINL + 10, MARGINU + i * VCAPI + 28, 40, 25);
		machineLabel[i]->setFont(QFont("Consolas", 15));
		machineLabel[i]->setAlignment(Qt::AlignCenter);
		machineLabel[i]->setText("M" + QString::number(i));

		workingLabel[i] = new QLabel(this);
		workingLabel[i]->setGeometry(MARGINL + 65, MARGINU + i * VCAPI + 28, 70, 25);
		workingLabel[i]->setFont(QFont("Consolas", 15));
		workingLabel[i]->setAlignment(Qt::AlignCenter);
	}

	this->setFixedSize(WID, HEI);
	this->setWindowTitle("Result");

	QFont commnButFont("Consolas", 15);
	dynamicButton = new QPushButton(this);
	resetButton = new QPushButton(this);
	serviceButton = new QPushButton(this);
	timeShowLabel = new QLabel(this);
	dynamicButton->setGeometry(MARGINL, COMMN + 20, 150, 60);
	resetButton->setGeometry(MARGINL + 155, COMMN + 20, 80, 60);
	serviceButton->setGeometry(WID - MARGINR - 200, COMMN + 20, 200, 60);
	timeShowLabel->setGeometry(WID - MARGINR - 200 - 480, COMMN + 20, 380, 60);
	dynamicButton->setFont(commnButFont);
	resetButton->setFont(commnButFont);
	serviceButton->setFont(commnButFont);
	timeShowLabel->setFont(commnButFont);
	dynamicButton->setText("Play");
	resetButton->setText("Reset");
	serviceButton->setText("Service Mode");
	timeShowLabel->setText("Runtime: " + QString::number(timeUsed)
		+ "sec  Total Time: " + QString::number(restime));

	timer = new QTimer(this);
	timer->setInterval(50);
	doingDynamic = false;
	p = q = 0;
	connect(dynamicButton, SIGNAL(clicked()), this, SLOT(onDynamicButtonClicked()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(onResetButtonClicked()));
	connect(serviceButton, SIGNAL(clicked()), this, SLOT(onServiceButtonClicked()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(update()));
	connect(serviceButton, SIGNAL(clicked()), this, SLOT(update()));
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));

	exPicFileButton = new QPushButton(this);
	exTxtFileButton = new QPushButton(this);
	exPicFileButton->setGeometry(MARGINL + 250, COMMN + 20, 130, 30);
	exTxtFileButton->setGeometry(MARGINL + 250, COMMN + 20 + 30, 130, 30);
	exPicFileButton->setFont(QFont("Consolas", 8));
	exTxtFileButton->setFont(QFont("Consolas", 8));
	exPicFileButton->setText("Export Picture");
	exTxtFileButton->setText("Export Text");
	connect(exPicFileButton, SIGNAL(clicked()), this, SLOT(onExPicFileButtonClicked()));
	connect(exTxtFileButton, SIGNAL(clicked()), this, SLOT(onExTxtFileButtonClicked()));
}
ResultWindow::~ResultWindow() {}


void ResultWindow::paintEvent(QPaintEvent*)
{	
	QPen pen;
	pen.setWidth(1);
	QPainter painter(this);
	painter.setPen(pen);
	painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
	painter.drawRect(MARGINL, MARGINU, WID - MARGINL - MARGINR, machineCount * (VCAPI + 1) + 50 + AXISH);

	pen.setStyle(Qt::NoPen);
	painter.setPen(pen);
	painter.setFont(QFont("Consolas", static_cast<double>(VSIZE) / 2));

// WARNING : I AM TOOOO LAZY. DON'T WANT TO RESTRUCT THIS PART.
// AND I JUST CTRL+C, CTRL+V.
// AND SLIGHTLY MODIFIED THE CODE.
// CURSE ME :)

	if (!inServiceMode) {
		for (auto i = 0; i < machineCount; ++i) {
			for (auto j = 0; j < procedCount; ++j) {
				painter.setBrush(QBrush(
					getColor(restab[i][j].product),
					Qt::SolidPattern
				));
				painter.drawRect(
					DRAWL + restab[i][j].start * UN - 1, // == x
					DRAWU + i * VCAPI, // == y
					(restab[i][j].end - restab[i][j].start) * UN + 1, // == w
					VSIZE // == h
				); // -1 & +1 is for blocks conjunction w/o cracks
				pen.setStyle(Qt::SolidLine);
				painter.setPen(pen);
				painter.drawText(
					DRAWL + restab[i][j].start * UN,
					DRAWU + i * VCAPI + static_cast<double>(VSIZE) * 0.75,
					QString::number(restab[i][j].product)
				);
				pen.setStyle(Qt::NoPen);
				painter.setPen(pen);
			}
		}
		static const int DOWN = DRAWU + machineCount * VCAPI + AXISH - 6;
		painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
		painter.drawEllipse(DRAWL - 7, DOWN - 12, 14, 14);
		const QPoint triangle[] {
			QPoint(WID * 0.95 - MARGINR + 10, DOWN),
			QPoint(WID * 0.95 - MARGINR - 15, DOWN - 7),
			QPoint(WID * 0.95 - MARGINR - 15, DOWN + 7)
		};
		painter.drawPolygon(triangle, 3);
		pen.setStyle(Qt::SolidLine);
		pen.setWidth(5);
		painter.setPen(pen);
		painter.drawLine(DRAWL - 20, DOWN, WID * 0.95 - MARGINR, DOWN);
		pen.setWidth(3);
		painter.setPen(pen);
		painter.setFont(QFont("Consolas", 12));
		painter.drawText(DRAWL - 5, DOWN + 18, "0");
		for (auto i = 0; i < 7; ++i) {
			painter.drawLine(markPoint[i].coord, DOWN, markPoint[i].coord, DOWN - 10);
			painter.drawText(markPoint[i].coord - 10, DOWN + 18, QString::number(markPoint[i].value));
		}

		// save pic
		static int saved = 0;
		if (saved++ == 3) {
			gantt = QPixmap::grabWidget(this, MARGINL, MARGINU,
				WID - MARGINL - MARGINR, machineCount * (VCAPI + 1) + 50 + AXISH);
		}

		if (doingDynamic) {
			pen.setWidth(2);
			painter.setPen(pen);
			if (p == restime) {
				dynamicButton->setText("Play");
				doingDynamic = false;
				timer->stop();
				p = 0;
			}
			painter.drawLine(DRAWL + p * UN, DRAWU - 20, DRAWL + p * UN, DOWN);
			++p;

			pen.setStyle(Qt::NoPen);
			painter.setPen(pen);
			for (auto i = 0; i < machineCount; ++i) {
				for (auto j = 0; j < procedCount; ++j) {
					if (restab[i][j].start <= p && p <= restab[i][j].end) {
						machineLabel[i]->setStyleSheet("background-color: red;");
						workingLabel[i]->setText(
							QString::number(restab[i][j].product) + " - " +
							QString::number(restab[i][j].proced)
						);
						break;
					}
					else {
						machineLabel[i]->setStyleSheet("background-color: white;");
						workingLabel[i]->setText("");
					}
				}
			}
		}
	} // if !service
	else { // if service
		for (auto i = 0; i < machineCount; ++i) {
			for (auto j = 0; j < restabSvc[i].size(); ++j) {
				if (!restabSvc[i][j].isService) {
					painter.setBrush(QBrush(
						getColor(restabSvc[i][j].product),
						Qt::SolidPattern
					));
					painter.drawRect(
						DRAWL + restabSvc[i][j].start * UNS - 1,
						DRAWU + i * VCAPI,
						(restabSvc[i][j].end - restabSvc[i][j].start) * UNS + 1,
						VSIZE
					);
					pen.setStyle(Qt::SolidLine);
					painter.setPen(pen);
					painter.drawText(
						DRAWL + restabSvc[i][j].start * UNS,
						DRAWU + i * VCAPI + static_cast<double>(VSIZE) * 0.75,
						QString::number(restabSvc[i][j].product)
					);
					pen.setStyle(Qt::NoPen);
					painter.setPen(pen);
				}
				else { // if service block
					painter.drawPixmap(
						DRAWL + restabSvc[i][j].start * UNS - 1,
						DRAWU + i * VCAPI,
						zebra.copy(0, 0, (restabSvc[i][j].end - restabSvc[i][j].start) * UNS + 1, VSIZE)
					);
				}
			}
		}
		static const int DOWN = DRAWU + machineCount * VCAPI + AXISH - 6;
		painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
		painter.drawEllipse(DRAWL - 7, DOWN - 12, 14, 14);
		const QPoint triangle[] {
			QPoint(WID * 0.95 - MARGINR + 10, DOWN),
			QPoint(WID * 0.95 - MARGINR - 15, DOWN - 7),
			QPoint(WID * 0.95 - MARGINR - 15, DOWN + 7)
		};
		painter.drawPolygon(triangle, 3);
		pen.setStyle(Qt::SolidLine);
		pen.setWidth(5);
		painter.setPen(pen);
		painter.drawLine(DRAWL - 20, DOWN, WID * 0.95 - MARGINR, DOWN);
		pen.setWidth(3);
		painter.setPen(pen);
		painter.setFont(QFont("Consolas", 12));
		painter.drawText(DRAWL - 5, DOWN + 18, "0");
		for (auto i = 0; i < 7; ++i) {
			painter.drawLine(markPointSvc[i].coord, DOWN, markPointSvc[i].coord, DOWN - 10);
			painter.drawText(markPointSvc[i].coord - 10, DOWN + 18, QString::number(markPointSvc[i].value));
		}

		// save pic w/svc
		static int saved = 0;
		if (saved++ == 3) {
			ganttSvc = QPixmap::grabWidget(this, MARGINL, MARGINU,
				WID - MARGINL - MARGINR, machineCount * (VCAPI + 1) + 50 + AXISH);
		}

		if (doingDynamic) {
			pen.setWidth(2);
			painter.setPen(pen);
			if (q == restimeSvc) {
				dynamicButton->setText("Play");
				doingDynamic = false;
				timer->stop();
				q = 0;
			}
			painter.drawLine(DRAWL + q * UNS, DRAWU - 20, DRAWL + q * UNS, DOWN);
			++q;

			pen.setStyle(Qt::NoPen);
			painter.setPen(pen);
			for (auto i = 0; i < machineCount; ++i) {
				for (auto j = 0; j < procedCount; ++j) {
					if (!restabSvc[i][j].isService) {
						if (restabSvc[i][j].start <= q && q <= restabSvc[i][j].end) {
							machineLabel[i]->setStyleSheet("background-color: red;");
							workingLabel[i]->setText(
								QString::number(restabSvc[i][j].product) + " - " +
								QString::number(restabSvc[i][j].proced)
							);
							break;
						}
						else {
							machineLabel[i]->setStyleSheet("background-color: white;");
							workingLabel[i]->setText("");
						}
					}
					else { // isService
						if (restabSvc[i][j].start <= q && q <= restabSvc[i][j].end) {
							machineLabel[i]->setStyleSheet("background-color: grey;");
							workingLabel[i]->setText("|SVC|");
							break;
						}
						else {
							machineLabel[i]->setStyleSheet("background-color: white;");
							workingLabel[i]->setText("");
						}
					}
				}
			}
		}
	}
} // paintEvent


void ResultWindow::onDynamicButtonClicked()
{
	if (!doingDynamic) {
		dynamicButton->setText("Pause");
		doingDynamic = true;
		if (!timer->isActive()) {
			timer->start();
		}
	}
	else {
		dynamicButton->setText("Unpause");
		doingDynamic = false;
		if (timer->isActive()) {
			timer->stop();
		}
	}
}


void ResultWindow::onResetButtonClicked()
{
	p = q = 0;
	if (timer->isActive()) {
		timer->stop();
	}
	doingDynamic = false;
	dynamicButton->setText("Play");
}


void ResultWindow::onServiceButtonClicked()
{
	p = q = 0;
	if (timer->isActive()) {
		timer->stop();
	}
	doingDynamic = false;
	inServiceMode = !inServiceMode;
	serviceButton->setText(inServiceMode ? "No Service Mode" : "Service Mode");
	dynamicButton->setText("Play");
	timeShowLabel->setText(
		"Runtime: " +  QString::number(timeUsed) + "sec  Total Time: " +
		(inServiceMode ? QString::number(restimeSvc) : QString::number(restime))
	);
}


void ResultWindow::onExPicFileButtonClicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save To...", "C:", "JPG (*.jpg)");
	if (inServiceMode) {
		if (!ganttSvc.save(fileName)) {
			goto ERROR;
		}
	}
	else {
		if (!gantt.save(fileName)) {
			goto ERROR;
		}
	}
	return;

	ERROR:
	QErrorMessage * err = new QErrorMessage(this);
	err->showMessage("Saving Failed.");
	return;
}


void ResultWindow::onExTxtFileButtonClicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save To...", "C:", "Text (*.txt)");
	std::ofstream fout;
	fout.open(fileName.toStdString(), std::ios_base::out);
	if (!fout.is_open()) {
		goto ERROR;
	}
	if (!inServiceMode) {
		for (auto i = 0; i < machineCount; ++i) {
			fout << "M" << i << " ";
			for (auto j = 0; j < restab[i].size(); ++j) {
				fout << "(" << restab[i][j].start << "," << restab[i][j].product << "-"
					<< restab[i][j].proced << "," << restab[i][j].end << ")"
					<< ((j == restab[i].size() - 1) ? "" : " ");
			}
			fout << std::endl;
		}
		fout << "Time Used: " << timeUsed << "s" << std::endl
			<< "End Time: " << restime << std::endl;
	}
	else {
		for (auto i = 0; i < machineCount; ++i) {
			fout << "M" << i << " ";
			for (auto j = 0; j < restabSvc[i].size(); ++j) {
				if (!restabSvc[i][j].isService) {
					fout << "(" << restabSvc[i][j].start << "," << restabSvc[i][j].product << "-"
						<< restabSvc[i][j].proced << "," << restabSvc[i][j].end << ")"
						<< ((j == restabSvc[i].size() - 1) ? "" : " ");
				}
				else {
					fout << "(" << restabSvc[i][j].start << ",SVC," << restabSvc[i][j].end << ")"
						<< ((j == restabSvc[i].size() - 1) ? "" : " ");
				}
			}
			fout << std::endl;
		}
		fout << "Time Used: " << timeUsed << "s" << std::endl
			<< "End Time: " << restimeSvc << std::endl;
	}
	fout.close();
	return;

	ERROR:
	QErrorMessage * err = new QErrorMessage(this);
	err->showMessage("Saving Failed.");
	return;
}


/*********************** PRIVATE **************************/

QColor ResultWindow::getColor(const int machine) const
{
	switch (machine)
	{
	case 0: return QColor(0xff, 0x40, 0x40); //red
	case 1: return QColor(0x7c, 0xfc, 0x00); //green
	case 2: return QColor(0x33, 0xcc, 0xff); //blue
	case 3: return QColor(0xff, 0xff, 0x00); //yellow
	case 4: return QColor(0xff, 0x99, 0x33); //orange
	case 5: return QColor(0x99, 0x33, 0xff); //violet
	case 6: return QColor(0xff, 0x33, 0xcc); //pink
	case 7: return QColor(0x66, 0xff, 0xff); //cyan
	case 8: return QColor(0xff, 0xd7, 0x00); //gold
	case 9: return QColor(0xee, 0x82, 0x62); //reeed
	case 10: return QColor(0xcc, 0xcc, 0x33); //yellowgreen
	case 11: return QColor(0x99, 0x33, 0xcc); //purple
	case 12: return QColor(0xff, 0x99, 0xcc); //slipink
	case 13: return QColor(0xcc, 0xcc, 0xcc); //grey
	case 14: return QColor(0xcc, 0x66, 0x00); //chocolate
	case 15: return QColor(0xcc, 0x00, 0x66); //rose
	case 16: return QColor(0x99, 0x33, 0x33); //crimson
	case 17: return QColor(0xff, 0x66, 0x66); //idk
	case 18: return QColor(0x33, 0x99, 0x99); //almost
	case 19: return QColor(0xcc, 0x33, 0x33); //finished
	default: return QColor(
		dtl::xorshiftRand() % 256,
		dtl::xorshiftRand() % 256,
		dtl::xorshiftRand() % 256);
	};
	//case 0: return QColor(Qt::red);
	//case 1: return QColor(Qt::yellow);
	//case 2: return QColor(Qt::blue);
	//case 3: return QColor(Qt::green);
	//case 4: return QColor(Qt::cyan);
	//case 5: return QColor(Qt::magenta);
	//case 6: return QColor(Qt::darkRed);
	//case 7: return QColor(Qt::darkYellow);
	//case 8: return QColor(Qt::darkBlue);
	//case 9: return QColor(Qt::darkGreen);
	//case 10: return QColor(Qt::darkCyan);
	//case 11: return QColor(Qt::darkMagenta);
	//case 12: return QColor(Qt::gray);
	//case 13: return QColor(Qt::darkGray);
	//case 14: return QColor(Qt::black);
	//case 15: return QColor(255, 222, 173); // NavajoWhite
	//case 16: return QColor(70, 130, 180); // SteelBlue
	//case 17: return QColor(104, 205, 50); // OliveDrab
	//case 18: return QColor(105, 92, 92); // IndianRed
	//case 19: return QColor(205, 133, 63); // Tan
}

