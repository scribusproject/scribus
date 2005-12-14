#include "scripter2_base.h"

#if defined(HAVE_BOOST_PYTHON)

using namespace boost::python;

int add_five(int x)
{
	return x+5;
}

BOOST_PYTHON_MODULE(scribus2)
{
	def("add_five", add_five);
}

#endif

void scribus2_init()
{
#if defined(HAVE_BOOST_PYTHON)
	initscribus2();
#endif
}
