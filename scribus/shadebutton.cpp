#include "shadebutton.h"
#include "shadebutton.moc"
#include "query.h"


ShadeButton::ShadeButton(QWidget* parent) : QToolButton(parent, "ShadeButton")
{
	char *tmp[] = {"0 %", "10 %", "20 %", "30 %", "40 %", "50 %", "60 %", "70 %", "80 %", "90 %", "100 %"};
	size_t array = sizeof(tmp) / sizeof(*tmp);
	FillSh = new QPopupMenu();
	FillSh->insertItem( tr("Other..."));
	for (uint a = 0; a < array; ++a)
		FillSh->insertItem(tmp[a]);
	setBackgroundMode(PaletteBackground);
	setPopup(FillSh);
	setPopupDelay(1);
	setText("100 %");
	FillSh->setItemChecked(FillSh->idAt(11), true);
	setAutoRaise(true);
	connect( FillSh, SIGNAL(activated(int)), this, SLOT(setShade(int)));
}

void ShadeButton::setShade(int id)
{
	bool ok = false;
	uint a;
	int c, b;
	for (a = 0; a < FillSh->count(); ++a)
		{
		FillSh->setItemChecked(FillSh->idAt(a), false);
		}
	c = FillSh->indexOf(id);
	FillSh->setItemChecked(id, true);
	if (c > 0)
		b = (c-1) * 10;
	if (c == 0)
		{
		Query* dia = new Query(this, "New", 1, 0, tr("Shade:"), tr("Shade"));
		if (dia->exec())
    		{
			c = dia->Answer->text().toInt(&ok);
			if (ok)
				b = c;
			else
				b = 100;
			delete dia;
			}
		else
			{
			delete dia;
			return;
			}
		}
	setText(QString::number(b)+" %");
	emit clicked();
}

int ShadeButton::getValue()
{
	return text().remove(" %").toInt();
}

void ShadeButton::setValue(int val)
{
	for (uint a = 0; a < FillSh->count(); ++a)
		{
		FillSh->setItemChecked(FillSh->idAt(a), false);
		}
	if ((val % 10) == 0)
		FillSh->setItemChecked(FillSh->idAt(val/10+1), true);
	else
		FillSh->setItemChecked(FillSh->idAt(0), true);
	setText(QString::number(val)+" %");
}


