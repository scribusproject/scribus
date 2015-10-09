/*
    Copyright (C)  2008, 2010  Brad Hards <bradh@frogmouth.net>

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RTFREADER_H
#define RTFREADER_H

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QTextFormat>
#include <QStack>
#include <QTextDocument>

#include "Destination.h"
#include "Tokenizer.h"
#include "RtfGroupState.h"
/**
   Namespace for the public interface for the RtfReader classes
*/
namespace RtfReader
{
    class AbstractRtfOutput;
    class RtfProperty;
    class Reader;

  /**
     Reader for RTF formatted documents

     This class provides a simple API for reading documents 
     encoded in the Microsoft Rich Text Format (RTF).

     TODO: add spec reference
  */
  class Reader : public QObject
  {
    Q_OBJECT
  public:

    /**
       Normal constructor
    */
    explicit Reader( QObject *parent = 0 );

    /**
       Open a document

       \param filename the name of the document to open

       \return true if the document was successfully opened, otherwise false
    */
    bool open( const QString &filename );

    /**
       Close the open document

       This has no effect if the document is not open.
    */
    void close();

    /**
       The name of the file that is open.

       Will return an empty string if no file is open.
    */
    QString fileName() const;

    /**
       Parse the document

       \param output pointer to an output implementation       
    */
    bool parseTo( AbstractRtfOutput *output );
	bool parseFromDeviceTo( QIODevice * dev, AbstractRtfOutput *output );

    ~Reader();

    /////////////////////////////////////////////////
    //
    // Callback functions 
    //
    ////////////////////////////////////////////////
    void startInfo();

    void todo( RtfReader::RtfProperty *property );
    void todoDest( RtfReader::RtfProperty *property );

  private:

    /////////////////////////////////////////////////
    //
    // Implementation details
    //
    ////////////////////////////////////////////////

    // parse the RTF file, inserting elements into the document
    // being generated
    void parseFile();

    // parse the file header section, including sanity checks
    bool parseFileHeader();

    // check the file header for format / version compatibility
    bool headerFormatIsKnown( const QString &tokenName, int tokenValue );

    // parse the body of the document
    void parseDocument();

    // Change the destination
    void changeDestination( const QString &destinationName );

    // Destination factory
    Destination *makeDestination( const QString &destinationName );
    
    /////////////////////////////////////////////////
    //
    // Member variables below
    //
    ////////////////////////////////////////////////

    // The name of the file that is open (if any)
    QFile *m_inputDevice;

    // The tokenizer on the RTF document being parsed
    Tokenizer *m_tokenizer;

    // The output strategy
    AbstractRtfOutput *m_output;
   
    // The destination stack
    QStack<Destination*> m_destinationStack;

    bool m_nextSymbolMightBeDestination;

    QStack<RtfGroupState> m_stateStack;

    bool m_nextSymbolIsIgnorable;

    // debug things
    QString m_debugIndent;

  };
}

#endif

