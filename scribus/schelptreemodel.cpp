/****************************************************************************
**
** Copyright (C) 2005-2007 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

/*
    treemodel.cpp

    Provides a simple tree model to show how to create and use hierarchical
    models.
*/

#include <QtGui>
#include <QDomDocument>
#include <QFile>

#include "schelptreemodel.h"

ScHelpTreeModel::ScHelpTreeModel(const QString &dataFile, const QString &col1name, const QString &col2name, QObject *parent)
    : TreeModel(parent)
{
    QList<QVariant> rootData;
    rootData << col1name << col2name;
    rootItem = new TreeItem(rootData);
    setupModelData(dataFile, rootItem);
}

void ScHelpTreeModel::setupModelData(const QString &dataFile, TreeItem *parent)
{
	QDomDocument doc( "menuentries" );
	QFile file( dataFile );
	if ( !file.open( QIODevice::ReadOnly ) )
		return;
	if ( !doc.setContent( &file ) )
	{
		file.close();
		return;
	}
	file.close();

	QList<TreeItem*> parents;
	QList<int> indentations;
	parents << parent;
	indentations << 0;
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	bool haveTutorials=false;
	QList<QVariant> columnData;
	int position=0;
	while( !n.isNull() )
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if( !e.isNull() )
		{
			if (e.hasAttribute( "text" ) && e.hasAttribute( "file" ))
			{
				QDomAttr textAttr = e.attributeNode( "text" );
				QDomAttr fileAttr = e.attributeNode( "file" );
				columnData.clear();
				columnData << textAttr.value() <<  fileAttr.value();
				if (position > indentations.last()) 
				{
					// The last child of the current parent is now the new parent
					// unless the current parent has no children.
		
					if (parents.last()->childCount() > 0) 
					{
						parents << parents.last()->child(parents.last()->childCount()-1);
						indentations << position;
					}
				}
				else 
				{
					while (position < indentations.last() && parents.count() > 0) {
						parents.pop_back();
						indentations.pop_back();
					}
				}
				// Append a new item to the current parent's list of children.
				parents.last()->appendChild(new TreeItem(columnData, parents.last()));
			}

			QDomNodeList nl=n.childNodes();
			if (nl.count()>0)
				position=1;
			for(int i=0 ; i<= nl.count() ; i++)
			{
				QDomNode child=nl.item(i);
				if (child.isElement())
				{
					QDomElement ec = child.toElement();
					if (!ec.isNull())
					{
						if (ec.hasAttribute( "text" ) && ec.hasAttribute( "file" ))
						{
							QDomAttr textAttr = ec.attributeNode( "text" );
							QDomAttr fileAttr = ec.attributeNode( "file" );
							columnData.clear();
							columnData << textAttr.value() <<  fileAttr.value();
							if (position > indentations.last()) 
							{
								// The last child of the current parent is now the new parent
								// unless the current parent has no children.
					
								if (parents.last()->childCount() > 0) 
								{
									parents << parents.last()->child(parents.last()->childCount()-1);
									indentations << position;
								}
							}
							else 
							{
								while (position < indentations.last() && parents.count() > 0) {
									parents.pop_back();
									indentations.pop_back();
								}
							}
							// Append a new item to the current parent's list of children.
							parents.last()->appendChild(new TreeItem(columnData, parents.last()));
						}
						//3rd level
						QDomNodeList nl2=child.childNodes();
						if (nl2.count()>0)
							position=2;
						for(int i2=0 ; i2<= nl2.count() ; i2++)
						{
							QDomNode childchild=nl2.item(i2);
							if (childchild.isElement())
							{
								QDomElement ecc = childchild.toElement();
								if (!ecc.isNull())
								{
									QDomAttr textAttr = ecc.attributeNode( "text" );
									QDomAttr fileAttr = ecc.attributeNode( "file" );
									columnData.clear();
									columnData << textAttr.value() <<  fileAttr.value();
									if (position > indentations.last()) 
									{
										// The last child of the current parent is now the new parent
										// unless the current parent has no children.
							
										if (parents.last()->childCount() > 0) 
										{
											parents << parents.last()->child(parents.last()->childCount()-1);
											indentations << position;
										}
									}
									else 
									{
										while (position < indentations.last() && parents.count() > 0) {
											parents.pop_back();
											indentations.pop_back();
										}
									}
									// Append a new item to the current parent's list of children.
									parents.last()->appendChild(new TreeItem(columnData, parents.last()));
								}
							}
						}
						position=1;
					}
				}
			}
			position=0;
		}
		n = n.nextSibling();
	}
}
