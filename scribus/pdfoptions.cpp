#include "pdfoptions.h"
#include "scribusstructs.h"

PDFOptions::VerifyResults PDFOptions::verify()
{
	return verify(static_cast<QString*>(NULL));
}

PDFOptions::VerifyResults PDFOptions::verify(QString* /*problemDescription*/)
{
	// TODO: implement this method
	return Verify_NoError;
}
