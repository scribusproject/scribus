/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

void DocumentItemAttributes::init()
{
	//blah
}

void DocumentItemAttributes::destroy()
{
	//blah2
}


void DocumentItemAttributes::setup(QMap<int, ObjectAttribute> *docItemAttrs)
{
	localAttributes=*docItemAttrs;
	attributesTable->setNumRows(docItemAttrs->count());
	for(QMap<int, ObjectAttribute>::Iterator it = docItemAttrs->begin(); it!= docItemAttrs->end(); ++it)
	{
		uint i=0;
		QTableItem *item = new QTableItem(attributesTable, QTableItem::WhenCurrent, it.data().name);
		attributesTable->setItem(it.key(), i++, item);
		QTableItem *item1 = new QTableItem(attributesTable, QTableItem::WhenCurrent, it.data().type);
		attributesTable->setItem(it.key(), i++, item1);
		QTableItem *item2 = new QTableItem(attributesTable, QTableItem::WhenCurrent, it.data().value);
		attributesTable->setItem(it.key(), i++, item2);
		QTableItem *item3 = new QTableItem(attributesTable, QTableItem::WhenCurrent, it.data().parameter);
		attributesTable->setItem(it.key(), i++, item3);
		
		attributesTable->verticalHeader()->setLabel(it.key(), QString("%1").arg(it.key()));
	}
}


QMap<int, ObjectAttribute> DocumentItemAttributes::getNewAttributes()
{
	return localAttributes;
}


void DocumentItemAttributes::tableItemChanged( int row, int col )
{
	if (localAttributes.contains(row))
	{
		switch (col)
		{
			case 0:
				localAttributes[row].name=attributesTable->text(row, col);
				break;
			case 1:
				localAttributes[row].type=attributesTable->text(row, col);
				break;
			case 2:
				localAttributes[row].value=attributesTable->text(row, col);
				break;
			case 3:
				localAttributes[row].parameter=attributesTable->text(row, col);
				break;
			default:
				break;
		}
	}
}
