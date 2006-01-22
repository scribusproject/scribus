/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdvar.h"

#include "scribusstructs.h"

using namespace boost::python;

// Declare the export functions from the implementation
extern void export_QString();
extern void export_QObject();
extern void export_QWidget();
extern void export_QFrame();
extern void export_QScrollView();
extern void export_QMainWindow();
extern void export_QApplication();

extern void export_ScribusStructs();
extern void export_PageItem();
extern void export_ScribusMainWindow();
extern void export_ScribusQApp();
extern void export_ScribusWin();
extern void export_ScribusDoc();
extern void export_ScribusView();

extern void export_styles();

/**
 * @brief A simple test function to wrap with Boost::Python
 */
int add_five(int x)
{
	return x+5;
}

/**
 * @brief Define the scribus2 module
 */
BOOST_PYTHON_MODULE(scribus2)
{
	qDebug("Exporting scribus2");

	// Export our basic testing function
	def("add_five", add_five);

	// Export type converters
	export_QString();

	// And Qt classes
	export_QObject();
	export_QApplication();
	export_QWidget();
	export_QFrame();
	export_QScrollView();
	export_QMainWindow();

	// Export Scribus types
	export_ScribusStructs();
	export_ScribusQApp();
	export_ScribusDoc();
	export_ScribusView();
	export_ScribusWin();
	export_ScribusMainWindow();
	export_PageItem();

	// Export access functions and dirty hacks
	export_styles();

	qDebug("Done");
}

/**
 * @brief Initialize the module, including calling initscribus2()
 *
 * This method should be a no-op if we've been built without support
 * for Boost::Python
 */
void scripter2_init()
{
	initscribus2();
}
