/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
//CB: 2014

#ifndef PP_PROXY_H
#define PP_PROXY_H

#include <QObject>

class PageItem;
class ScribusDoc;


class PP_Proxy : public QObject
{
		Q_OBJECT
	public:
		explicit PP_Proxy(QObject *parent = 0);

	signals:

	public slots:
		void setPageItem(PageItem* item);
		void setDoc(ScribusDoc* doc);

	protected:
		PageItem *m_pageItem;
		ScribusDoc *m_scribusDoc;

		void connectDoc();
		void connectItem();

		void publishDocChange();
		void publishItemChange();
};

#endif // PP_PROXY_H
