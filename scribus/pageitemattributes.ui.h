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

//Basically the same as Document Item Attributes for now... will get comboboxes later etc
void PageItemAttributes::init()
{
	//blah
}

void PageItemAttributes::destroy()
{
	//blah2
}


void PageItemAttributes::setup(ObjAttrVector *docItemAttrs)
{
	localAttributes=*docItemAttrs;
	attributesTable->setNumRows(docItemAttrs->count());
	int row=0;
	for(ObjAttrVector::Iterator it = docItemAttrs->begin(); it!= docItemAttrs->end(); ++it)
	{
		uint i=0;
		QTableItem *item = new QTableItem(attributesTable, QTableItem::WhenCurrent, (*it).name);
		attributesTable->setItem(row, i++, item);
		QTableItem *item1 = new QTableItem(attributesTable, QTableItem::WhenCurrent, (*it).type);
		attributesTable->setItem(row, i++, item1);
		QTableItem *item2 = new QTableItem(attributesTable, QTableItem::WhenCurrent, (*it).value);
		attributesTable->setItem(row, i++, item2);
		QTableItem *item3 = new QTableItem(attributesTable, QTableItem::WhenCurrent, (*it).parameter);
		attributesTable->setItem(row, i++, item3);
		
		attributesTable->verticalHeader()->setLabel(row, QString("%1").arg(row));
		row++;
	}
}

ObjAttrVector* PageItemAttributes::getNewAttributes()
{
	return &localAttributes;
}


void PageItemAttributes::tableItemChanged( int row, int col )
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
