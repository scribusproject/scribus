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

/*! \file managemacrosdialog.ui.h */

/*! \brief Constructor - do some setup Designer doesn't know how to do
*/
void ManageMacrosDialog::init()
{
	// Eliminiate the left header
	this->macroTable->setLeftMargin(0);
	this->macroTable->setColumnStretchable(3,true);
}


/*! \brief Create a new, empty macro
*/
void ManageMacrosDialog::newClicked()
{
	emit newMacro();
}


/*! \brief Display an edit macro dialog with the currently selected macro, and
 *         if accepted save the changes.
*/
void ManageMacrosDialog::editClicked()
{
	QString macroName = this->getNameOfSelection();
	emit editMacro(macroName);
}


/*! \brief Delete the selected macro
*/
void ManageMacrosDialog::deleteClicked()
{
	QString macroName = this->getNameOfSelection();
	emit deleteMacro(macroName);
}


void ManageMacrosDialog::deleteAllClicked()
{
	emit deleteAll();
}


void ManageMacrosDialog::importClicked()
{
	QString path = QFileDialog::getOpenFileName(
			QDir::homeDirPath(), "Scribus Macro Files (*.pymacro)", this,
			"Open File Dialog", "Select the macro file to load.");
	if (path != "")
		emit importMacros(path);
}


void ManageMacrosDialog::exportClicked()
{
	QString path = QFileDialog::getSaveFileName(
			QDir::homeDirPath(), "Scribus Macro Files (*.pymacro)", this,
			"Save File Dialog", "Save all macros");
	if (path != "")
		emit exportMacros(path);
}



void ManageMacrosDialog::renameClicked()
{
	QString macroName = this->getNameOfSelection();
	bool ok = false;
	QString newName = QInputDialog::getText(
			"Scribus - Rename Macro", "Enter new name: ",
			QLineEdit::Normal, macroName, &ok, this);
	if (ok)
		emit(renameMacro(macroName, newName));
}



void ManageMacrosDialog::setAccelClicked()
{
	// FIXME: It'd be nice to use the same interface as the prefs for setting
	// accel keys
	QString macroName = this->getNameOfSelection();
	QString macroAccel = this->getAccelOfSelection();
	bool ok = false;
	QString newAccel = QInputDialog::getText(
			"Scribus - Set Macro Shortcut", "Enter new shortcut: ",
			QLineEdit::Normal, macroAccel, &ok, this);
	if (ok)
		emit(changeMacroAccel(macroName, newAccel));
}



/*! \brief Adjust the table layout after the data has been inserted so that
 *         everything fits nicely
*/
void ManageMacrosDialog::adjust()
{
	this->macroTable->adjustColumn(0);
	this->macroTable->adjustColumn(1);
	this->macroTable->adjustColumn(2);
	this->macroTable->adjustColumn(3);
}


/*! \brief Enable/disable the buttons depending on if there are any rows in the
 *         table and on the current selection.
*/
void ManageMacrosDialog::updateButtonStatus()
{
	bool status = this->macroTable->numRows() ? true : false;
	this->deleteMacroButton->setEnabled(status);
	this->clearButton->setEnabled(status);
	this->exportMacrosButton->setEnabled(status);
	this->setAccelButton->setEnabled(status);
	this->renameButton->setEnabled(status);
	if (status && this->selectionEditable() )
		this->editMacroButton->setEnabled(true);
	else
		this->editMacroButton->setEnabled(false);
}


/* \brief Add a new row to the dialog.
*/
void ManageMacrosDialog::appendRow(QString macroName, QString description,
		QString accel, QString sourceCode)
{
	QString desc = description;
	// Truncate the description if appropriate
	if (desc.length() > 50)
		desc = desc.left(50) + "...";
	bool edit = sourceCode == QString::null ? false : true;
	int numRows = this->macroTable->numRows();
	this->macroTable->insertRows(numRows);
	this->macroTable->setText(numRows, 0, macroName);
	this->macroTable->setText(numRows, 1, edit ? "Yes" : "No");
	this->macroTable->setText(numRows, 2, accel);
	this->macroTable->setText(numRows, 3, desc);
	// re-sort the table by name
	this->macroTable->sortColumn(0,true,true);
}

// This version of appendRow doesn't require source or description args, and
// fills them in with blanks.
void ManageMacrosDialog::appendRow( QString macroName, QString description )
{
	return appendRow(macroName, description, QString::null, QString::null);
}

/*! \brief get the value of the 'macro name' cell of the currently selected row
*/
QString ManageMacrosDialog::getNameOfSelection()
{
	int selectedRow = this->macroTable->selection(0).topRow();
	return this->macroTable->text(selectedRow, 0);
}


QString ManageMacrosDialog::getAccelOfSelection()
{
	int selectedRow = this->macroTable->selection(0).topRow();
	return this->macroTable->text(selectedRow, 2);
}


bool ManageMacrosDialog::selectionEditable()
{
	int selectedRow = this->macroTable->selection(0).topRow();
	return this->macroTable->text(selectedRow, 1) == "Yes" ? true : false;
}


/*! \brief Handle the deletion of the named macro
*/
void ManageMacrosDialog::deleteRow( QString macroName )
{
	// the macro manager has deleted the named macro. Remove it from the table.
	int rowNum = getRowNumByName(macroName);
	if (rowNum == -1)
		qDebug("Tried to delete non-existent row");
	else
	{
		this->macroTable->removeRow(rowNum);
		emit tableModified();
	}
	// Select the row after the deleted row.
	this->macroTable->selectRow(rowNum);
}


/*! \brief delete all rows in the table
*/
void ManageMacrosDialog::clearTable()
{
	int rows = this->macroTable->numRows();
	for (int i = 0; i < rows ; i ++ )
		this->macroTable->removeRow(i);
	emit tableModified();
}


void ManageMacrosDialog::updateName( QString newName, QString oldName )
{
	// The given macro has been renamed. Update the table to reflect this.
	int rowNum = getRowNumByName(oldName);
	this->macroTable->setText(rowNum, 0, newName);
	emit tableModified();
}



void ManageMacrosDialog::updateDescription( QString macroName,
		QString newDescription )
{
	int rowId = getRowNumByName(macroName);
	QString desc = newDescription;
	// Truncate the description if appropriate
	if (desc.length() > 50)
		desc = desc.left(50) + "...";
	this->macroTable->setText(rowId, 3, desc);
}


void ManageMacrosDialog::updateEditable( QString macroName, QString newSource )
{
	int rowId = getRowNumByName(macroName);
	this->macroTable->setText(rowId, 1, newSource == QString::null ? "No" : "Yes");
}


void ManageMacrosDialog::updateAccel( QString macroName, QString accel )
{
	int rowId = getRowNumByName(macroName);
	this->macroTable->setText(rowId, 2, accel);
}


int ManageMacrosDialog::getRowNumByName( QString macroName )
{
	int rows = this->macroTable->numRows();
	for (int i = 0; i < rows ; i ++ )
	{
		if (this->macroTable->text(i, 0) == macroName)
			return i;
	}
	return -1;
}


void ManageMacrosDialog::renameFailed( QString macroName )
{
	int rowId = getRowNumByName(macroName);
	if (rowId == -1)
		qDebug("ManageMacrosDialog::renameFailed() called for an unknown macro");
	QMessageBox::warning(this, tr("Scribus - Macro Manager"),
			tr("Renaming the macro failed because the name is already in use."),
			QMessageBox::Ok|QMessageBox::Default,
			QMessageBox::NoButton);
}
