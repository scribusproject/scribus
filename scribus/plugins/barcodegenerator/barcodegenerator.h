/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef BARCODEGENERATOR_H
#define BARCODEGENERATOR_H

#include "ui_barcodegenerator.h"

#include <QColor>
#include <QDialog>
#include <QLabel>
#include <QString>

#include "sccolor.h"


/*! \brief One Barcode Entity.
\author Petr Vanek <petr@yarpen.cz>
 */
class BarcodeType
{
	public:
		//! \brief Constructor provided for QMap initialization only.
		BarcodeType(){};
		/*! \brief Setup the Barcode entity.
		\todo Make better regular expressions for BCDs.
		\param cmd a Postsript command for given BC type
		\param exa an example of the contents
		\param exaop an example of the options */
		BarcodeType(const QString &cmd, const QString &exa, const QString &exaop);
		~BarcodeType(){};
		//! \brief PostScript encoder
		QString command;
		//! \brief BC example contents
		QString exampleContents;
		//! \brief BC example options
		QString exampleOptions;
};


//! \brief Type for BC name/BC type mapping.
typedef QMap<QString, BarcodeType> BarcodeMap;


/*! \brief Active tasts for BC GUI.
It's inherited from BarcodeGeneratorBase() class which is created
by uic from designer. Don't change anything in BarcodeGeneratorBase
manually! It will be overwritten automatically by uic.
\author Petr Vanek <petr@yarpen.cz>
*/
class BarcodeGenerator : public QDialog
{
	Q_OBJECT

	public:
		/*! \brief Create modal BC dialog.
		\param parent Parent of the dialog.
		\param name name od the QObject to debug */
		BarcodeGenerator(QWidget* parent = 0, const char* name = 0);
		//! \brief Erase the temporary files here.
		~BarcodeGenerator();

	protected:
		//! GUI namespace content. See designer.
		Ui::BarcodeGeneratorBase ui;

		//! \brief BC/BC type mapping. QMap keys are used as BC names.
		BarcodeMap map;

		//! \brief List of available barcode encoders.
		QList<QString> encoderlist;
		//! \brief Descriptions of each encoder.
		QHash<QString, QString> resdescs;
		//! \brief Dependancies of each encoder.
		QHash<QString, QString> resreqs;
		//! \brief Example data input for each encoder.
		QHash<QString, QString> resexams;
		//! \brief Example options input for each encoder.
		QHash<QString, QString> resexops;
		//! \brief Renderers for each encoder.
		QHash<QString, QString> resrndrs;
		//! \brief PS body of each encoder.
		QHash<QString, QString> resbodys;
		//! \brief Symbol Versions of each encoder.
		QHash<QString, QString> resvers;
		//! \brief Label for the versions field.
		QHash<QString, QString> resvlbl;
		//! \brief Error correction levels of each encoder.
		QHash<QString, QString> resecls;
		//! \brief includetext option available for each encoder.
		QHash<QString, bool> resincludetextAvail;
		//! \brief guardwhitespace option available for each encoder.
		QHash<QString, bool> resguardwhitespaceAvail;
		//! \brief includecheck option available for each encoder.
		QHash<QString, bool> resincludecheckAvail;
		//! \brief includecheckintext option available for each encoder.
		QHash<QString, bool> resincludecheckintextAvail;
		//! \brief parse option available for each encoder.
		QHash<QString, bool> resparseAvail;
		//! \brief parsefnc option available for each encoder.
		QHash<QString, bool> resparsefncAvail;

		//! \brief Color of the BC lines.
		ScColor lnColor;
		//! \brief Color of the BC font.
		ScColor txtColor;
		//! \brief Background color of the BC.
		ScColor bgColor;

		//! \brief A temporary png pixmap to store the preview image.
		QString tmpFile;
		//! \brief A temporary PS file to store commants for Ghostscript.
		QString psFile;
		/*! \brief Flag if user modified the codeEditor.
		To stop replacing his code with samples. */
		bool useSamples;
		QColor guiColor;

		/*! \brief Create bitmap for current BC.
		When tt's called for preview, the 72dpi, small PNG image is
		created each time user change GUI. No need to call it for
		final rendering because we have PS file created from "preview" run.
		\param fileName a optional file name to store on user's filesyste,
		\param dpi optional DPI value. Default is 72 for preview.
		\retval bool true on success.
		*/
		bool paintBarcode(const QString &fileName = QString(), int dpi = 72);
		/*! \brief Create color preview.
		Used for Color box feedback.
		\param l A pointer to the sample QLabel
		\param c A color to fill */
		void paintColorSample(QLabel *l, const ScColor & c);
		/*! \brief Perform BarcodeCheckType checks here
		\param s new string */
		bool codeEdit_check(const QString & s);
		bool optionsEdit_check(const QString & s);
		void updateOptions();
		void updateOptionsTextFromUI();
		void updateUIFromOptionsText();

	protected slots:
		void bcComboChanged();
		void bcComboChanged(int);
		//void textCheck_changed();
		//void guardCheck_changed();
		//void includeCheck_stateChanged(int state);
		//void includeCheckInText_stateChanged(int state);
		//void parseCheck_stateChanged(int state);
		//void parsefncCheck_stateChanged(int state);
		void bgColorButton_pressed();
		void lnColorButton_pressed();
		void txtColorButton_pressed();
		void codeEdit_textChanged(const QString& s);
		void optionsEdit_textChanged(const QString& s);
		void resetButton_clicked();
		void okButton_pressed();
		void cancelButton_pressed();
private slots:
		void on_includetextCheck_stateChanged(int arg1);
		void on_includecheckCheck_stateChanged(int arg1);
		void on_includecheckintextCheck_stateChanged(int arg1);
		void on_parseCheck_stateChanged(int arg1);
		void on_parsefncCheck_stateChanged(int arg1);
		void on_formatCombo_currentIndexChanged(int index);
		void on_eccCombo_currentIndexChanged(int index);
		void on_guardwhitespaceCheck_stateChanged(int arg1);
		void on_optionsEdit_textChanged(const QString &arg1);
};

#endif
