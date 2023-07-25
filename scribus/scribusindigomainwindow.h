#ifndef SCRIBUSINDIGOMAINWINDOW_H
#define SCRIBUSINDIGOMAINWINDOW_H

#include <QMainWindow>

#include "scribusapi.h"

//class DockManager;

class SCRIBUS_API ScribusIndigoMainWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		explicit ScribusIndigoMainWindow(QWidget *parent = nullptr);
		~ScribusIndigoMainWindow();

		int initScIMW();
	signals:
	private:

	//DockManager *dockManager;


	};

#endif // SCRIBUSINDIGOMAINWINDOW_H
