/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2009 by Franz Schmid                                   *
*   franz.schmid@altmuehlnet.de                                                   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#ifndef GRADIENTEADIALOG_H
#define GRADIENTEADIALOG_H

#include <QDialog>
#include "ui_gradientaddedit.h"
#include "scribusapi.h"
#include "scribusdoc.h"
#include "vgradient.h"

class SCRIBUS_API gradientEditDialog : public QDialog, Ui::gradientAddEditDialog
{
	Q_OBJECT

	public:
		gradientEditDialog(QWidget* parent, QString name, VGradient gradient, ColorList doco, ScribusDoc *doc, QMap<QString, VGradient> *gradients, bool newFlag);
		~gradientEditDialog() {};
		QString name();
		VGradient gradient();
	private slots:
		void quitDialog();
	protected:
		ScribusDoc* m_doc;
		bool isNew;
		QString m_name;
		QMap<QString, VGradient> *m_gradients;
};

#endif
