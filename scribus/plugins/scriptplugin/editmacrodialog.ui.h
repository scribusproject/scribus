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



void EditMacroDialog::setName( QString name )
{
	this->nameLabel->setText(tr("Editing Macro: <b>") + name + "</b>");
	this->macroName = name;
}


void EditMacroDialog::setSource( QString text )
{
	this->sourceCodeEdit->setText(text);
	// We've just replaced the contents, so clear the modified flag
	this->sourceCodeEdit->setModified(false);
}


QString EditMacroDialog::getSource()
{
	return this->sourceCodeEdit->text();
}


void EditMacroDialog::saveSourceClicked()
{
	QString path = QFileDialog::getSaveFileName(
			QDir::homeDirPath(), tr("Python source files (.py)"), this,
			tr("Save File Dialog"), tr("Save macro source"));
	if (path != "")
	{
		if (QFile::exists(path))
		{
			int result = QMessageBox::question(this, tr("Scribus - Macro Manager"),
					tr("The file '%1' already exists.\n"
						"Are you sure you want to overwrite it?\n")
					.arg(path),
					QMessageBox::Yes|QMessageBox::Default,
					QMessageBox::No|QMessageBox::Escape
					);
			if (result != QMessageBox::Yes)
				return;
		}
		QFile sourceFile(path);
		if (sourceFile.open( IO_WriteOnly ))
		{
			// We could use writeBlock(), but a stream is just as easy
			QTextStream stream(&sourceFile);
			stream << this->getSource();
		}
		sourceFile.close();
	}
}


void EditMacroDialog::loadSourceClicked()
{
	QString path = QFileDialog::getOpenFileName(
			QDir::homeDirPath(), tr("Python source files (.py)"), this,
			tr("Open File Dialog"), tr("Select the source file to load"));
	if (path != "")
	{
		if ((this->getSource() != "") && (this->sourceCodeEdit->isModified()) )
		{
			int result = QMessageBox::question(this, tr("Scribus - Macro Manager"),
					tr("You have already edited this macro.\n"
						"Are you sure you want to discard all your changes?\n"),
					QMessageBox::Yes|QMessageBox::Default,
					QMessageBox::No|QMessageBox::Escape
					);
			if (result != QMessageBox::Yes)
				return;
		}
		QFile sourceFile(path);
		if (sourceFile.open( IO_ReadOnly ))
		{
			QTextStream stream(&sourceFile);
			this->setSource(stream.read());
		}
		sourceFile.close();
	}
}


void EditMacroDialog::compileWorked( QString macroName )
{
	if (macroName != this->macroName)
		return;
	// Set status flags indicating that we've been informed of the compile result
	// and that it succeeded
	this->gotCompileNotification = true;
	this->compileSuccessful = true;
}


// compileFailed expects to be triggered as a result 
void EditMacroDialog::compileFailed(QString macroName, QString /*excType*/,
		QString excText, QString excTraceback )
{
	// Ignore signals for macros other than the one we're looking after
	if (macroName != this->macroName)
		return;
	QString tracebackMessage = "";
	if (excTraceback != QString::null)
		tracebackMessage = tr("A full traceback follows:\n\n%1\n").arg(excTraceback);
	QMessageBox::warning(this, tr("Scribus - Macro Manager"),
			tr("Compilation of the macro failed, so it can not \n"
				"be saved in its current form. The error was:\n%1\n")
			.arg(excText) + tracebackMessage,
			QMessageBox::Ok|QMessageBox::Default,
			QMessageBox::NoButton);
	// Set status flags indicating that we've been informed of the compile result
	// and that it failed
	this->gotCompileNotification = true;
	this->compileSuccessful = false;
}


void EditMacroDialog::accept()
{
	// Compile the macro and make sure it worked before closing
	// the dialog. Don't worry about whether the macro is modified
	// or not - it's much safer and easier to make sure it compiles
	// correctly again anyway.

	// First, clear the status flags we use to check what happened in a compile
	this->gotCompileNotification = false;
	this->compileSuccessful = false;

	// Compile the code. This request will cause MacroManager to emit signals
	// indicating either the success or failure of the compilation.
	emit compile(this->macroName, getSource());

	// We catch the aforementioned signals and set flags to indcate what
	// happened.  Note that if the compilation failed, the user has already
	// been informed, so all we need to do is decide whether or not to actually
	// accept.
	if (!this->gotCompileNotification)
	{
		qDebug("Asked to compile code, but no notification of the result arrived");
		// Just to be safe, clear the flag again
		this->gotCompileNotification = false;
	}
	else
		if (this->compileSuccessful)
			return QDialog::accept();
}


QString EditMacroDialog::getName()
{
	return this->macroName;
}

