#include "scripter2_base.h"

#include "scribusstructs.h"

#if defined(HAVE_BOOST_PYTHON)

using namespace boost::python;

// Declare the export functions from the implementation
extern void export_QString();
extern void export_QApplication();
extern void export_ScribusStructs();
extern void export_PageItem();
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
	export_QApplication();

	// Export Scribus types
	export_ScribusStructs();
	export_PageItem();

	// Export access functions
	export_styles();

	qDebug("Done");
}

#endif

/**
 * @brief Initialize the module, including calling initscribus2()
 *
 * This method should be a no-op if we've been built without support
 * for Boost::Python
 */
void scribus2_init()
{
#if defined(HAVE_BOOST_PYTHON)
	initscribus2();
#endif
}
