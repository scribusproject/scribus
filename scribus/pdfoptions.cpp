#include "pdfoptions.h"
#include "scribusstructs.h"

PDFOptions::VerifyResults PDFOptions::verify(QString* problemDescription)
{
	// TODO: implement this method
	return Verify_OtherError;
}

bool PDFOptions::saveTo(QTextStream& outStream) const
{
	if (!outStream.device()->isWritable())
		return false;
	// TODO: implement this method
	return false;
}

// overload of bool PDFOptions::saveTo(QTextStream& outStream) const
bool PDFOptions::saveTo(QString outFileName) const
{
	QTextStream ts(outFileName, IO_WriteOnly|IO_Truncate);
	return saveTo(ts);
}

// overload of bool PDFOptions::saveTo(QTextStream& outStream) const
bool PDFOptions::saveTo(FILE* outFilePtr) const
{
	QTextStream ts(outFilePtr, IO_WriteOnly|IO_Truncate);
	return saveTo(ts);
}

bool readFrom(QTextStream& inStream)
{
	if (!inStream.device()->isReadable())
		return false;
	// TODO: implement this method
	return false;
}

// overload of bool readFrom(QTextStream& inStream)
bool readFrom(QString inFileName)
{
	QTextStream ts(inFileName, IO_ReadOnly);
	return readFrom(ts);
}

// overload of bool readFrom(QTextStream& inStream)
bool readFrom(FILE* inFilePtr)
{
	QTextStream ts(inFilePtr, IO_ReadOnly);
	return readFrom(ts);
}
