#include "JobshopThread.h"

JobshopThread::JobshopThread(const DVector2D<PROCEDURE>& jobtable_src, const JobshopGUI* gui_src, QWidget *parent)
{
	runtime = 0;
	gui = const_cast<JobshopGUI*> (gui_src);
	jobtable = const_cast<DVector2D<PROCEDURE>&> (jobtable_src); // by refer
}

JobshopThread::~JobshopThread() {}

void JobshopThread::run()
{
	clock_t begTime = clock();
	jobshop_GA_main();
	clock_t endTime = clock();
	runtime = static_cast<double>(endTime - begTime) / CLOCKS_PER_SEC;

	emit gui->finishSignal();
}


CHROMO& JobshopThread::result()
{
	return best;
}


double JobshopThread::totalRuntime()
{
	return runtime;
}


void JobshopThread::jobshop_GA_main()
{
	int count = 0;
	CHROMO champion_chromo;
	champion_chromo.time = 1 << 22;

	/*******************************************/
	/*class Progress : public QThread {
	private:
		JobshopGUI * gui;
	public:
		Progress(JobshopGUI* gui_src) : gui(gui_src) {}
		void run()
		{
			for (double i = 1; i <= gui->RUNTIME; i += gui->RUNTIME / 100) {
				emit gui->progressSignal(i);
			}
		}
	};
	Progress* bar = new Progress(gui);
	bar->start();*/
	/*******************************************/

	clock_t t1 = clock();
	clock_t t2 = 0;
	TIME Longest = 0;
	while ((t2 = clock()) - t1 < (gui->RUNTIME - 5 / 4 * Longest) * CLOCKS_PER_SEC) {
		CHROMO best_chromo = jobshop_GA_GUI(jobtable);
		if (champion_chromo.time > best_chromo.time) {
			champion_chromo = best_chromo;
		}
		++count;
		if (Longest < (clock() - t2) / CLOCKS_PER_SEC) {
			Longest = (clock() - t2) / CLOCKS_PER_SEC;
		}
		emit gui->progressSignal((t2 - t1) / CLOCKS_PER_SEC);
	}
	emit gui->progressSignal(gui->RUNTIME);

	best = champion_chromo;

} // jobshop_GA_main

