#ifndef COMBOLINESTYLE_H
#define COMBOLINESTYLE_H

#include <QComboBox>
#include <QObject>
#include <QListWidget>
#include "scribusapi.h"
#include "scribusdoc.h"

class LineStyleValue;

class SCRIBUS_API ComboLineStyle : public QComboBox
{
public:
	ComboLineStyle(QWidget *parent = nullptr);

	void setDoc(ScribusDoc *doc);
	void setIconSize(QSize size);
	void updateLineStyles();

public slots:
	void languageChange();

private:
	ScribusDoc *m_doc { nullptr };
	QSize m_size { QSize(128, 16) };
	QListWidget *m_list {nullptr};
	QPixmap renderPixmap(LineStyleValue item);

};

#endif // COMBOLINESTYLE_H
