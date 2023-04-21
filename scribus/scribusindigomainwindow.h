#ifndef SCRIBUSINDIGOMAINWINDOW_H
#define SCRIBUSINDIGOMAINWINDOW_H

#include <QMainWindow>

#include "scribusapi.h"

class SCRIBUS_API ScribusIndigoMainWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		explicit ScribusIndigoMainWindow(QWidget *parent = nullptr);
		~ScribusIndigoMainWindow();

		int initScIMW();
	signals:
	private:




	};

#endif // SCRIBUSINDIGOMAINWINDOW_H
