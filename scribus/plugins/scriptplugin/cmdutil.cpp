#include "cmdutil.h"

ScribusApp* Carrier;
ScribusDoc* doc;

double PointToValue(double Val)
{
	double ret = 0.0;
	switch (doc->Einheit)
		{
		case 0:
			ret = Val;
			break;
		case 1:
			ret = Val * 0.3527777;
			break;
		case 2:
			ret = Val * (1.0 / 72.0);
			break;
		case 3:
			ret = Val * (1.0 / 12.0);
			break;
		}
	return ret;
}

double ValToPts(double Val, int ein)
{
	double ret = 0.0;
	switch (ein)
		{
		case 0:
			ret = Val;
			break;
		case 1:
			ret = Val / 0.3527777;
			break;
		case 2:
			ret = Val / (1.0 / 72.0);
			break;
		case 3:
			ret = Val / (1.0 / 12.0);
			break;
		}
	return ret;
}

double ValueToPoint(double Val)
{
	return ValToPts(Val, doc->Einheit);
}

int GetItem(QString Name)
{
  if (Name != "")
		{
		for (uint a = 0; a < doc->ActPage->Items.count(); a++)
			{
			if (doc->ActPage->Items.at(a)->AnName == Name)
				return static_cast<int>(a);
			}
		}
	else
		{
		if (doc->ActPage->SelItem.count() != 0)
			return doc->ActPage->SelItem.at(0)->ItemNr;
		}
	return -1;
}

void ReplaceColor(QString col, QString rep)
{
	for (uint b = 0; b < Carrier->view->Pages.count(); b++)
		{
		for (uint c = 0; c < Carrier->view->Pages.at(b)->Items.count(); c++)
			{
			if (Carrier->view->Pages.at(b)->Items.at(c)->PType == 4)
				{
				for (uint d = 0; d < Carrier->view->Pages.at(b)->Items.at(c)->Ptext.count(); d++)
					{
					if (col == Carrier->view->Pages.at(b)->Items.at(c)->Ptext.at(d)->ccolor)
						{
						Carrier->view->Pages.at(b)->Items.at(c)->Ptext.at(d)->ccolor = rep;
						}
					}
				}
			if (col == Carrier->view->Pages.at(b)->Items.at(c)->Pcolor)
				Carrier->view->Pages.at(b)->Items.at(c)->Pcolor = rep;
			if (col == Carrier->view->Pages.at(b)->Items.at(c)->Pcolor2)
				Carrier->view->Pages.at(b)->Items.at(c)->Pcolor2 = rep;
			}
		}
}

