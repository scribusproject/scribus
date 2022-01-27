/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pslib.h  -  description
                             -------------------
    begin                : Sat May 26 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PSLIB_H
#define PSLIB_H

#include <vector>
#include <utility>

#include <QDataStream>
#include <QFile>
#include <QList>
#include <QPen>
#include <QString>

#include "scribusapi.h"
#include "scribusstructs.h"
#include "colormgmt/sccolormgmtengine.h"
#include "tableborder.h"


class ScPage;
class ScribusDoc;
class PageItem;
class MultiProgressDialog;
class ScImage;
class ScLayer;
class PSPainter;

/**
  *@author Franz Schmid
  * Diese Klasse erzeugt Postscript-Dateien
  */

class SCRIBUS_API PSLib : public QObject
{
	Q_OBJECT

	friend class PSPainter;

	public:

		enum Optimization
		{
			OptimizeCompat = 0,
			OptimizeSize = 1
		};

		enum OutputFormat
		{
			OutputPS = 0,
			OutputEPS = 1
		};

		PSLib(ScribusDoc* doc, PrintOptions &options, OutputFormat outputFmt, ColorList *docColors = nullptr);
		virtual ~PSLib();

		void setOptimization (Optimization opt) { m_optimization = opt; }
		void setOutputFormat (OutputFormat outputFmt) { m_outputFormat = outputFmt; }

		virtual int   createPS(const QString& outputFileName);
		virtual const QString& errorMessage();
		virtual void PS_set_Info(const QString& art, const QString& was);

	protected:
		virtual void PS_Error(const QString& message);
		virtual void PS_Error_ImageDataWriteFailure();
		virtual void PS_Error_ImageLoadFailure(const QString& fileName);
		virtual void PS_Error_MaskLoadFailure(const QString& fileName);
		virtual void PS_Error_InsufficientMemory();

		virtual bool PS_set_file(const QString& fn);

		virtual bool PS_begin_doc( double x, double y, double width, double height, int numpage);
		virtual void PS_begin_page(ScPage* pg, MarginStruct* Ma, bool clipping);
		virtual void PS_end_page();
		virtual void PS_curve(double x1, double y1, double x2, double y2, double x3, double y3);
		virtual void PS_moveto(double x, double y);
		virtual void PS_lineto(double x, double y);
		virtual void PS_closepath();
		virtual void PS_translate(double x, double y);
		virtual void PS_scale(double x, double y);
		virtual void PS_rotate(double x);
		virtual void PS_clip(bool mu);
		virtual void PS_save();
		virtual void PS_restore();
		virtual void PS_setcmykcolor_fill(double c, double m, double y, double k);
		virtual void PS_setcmykcolor_dummy();
		virtual void PS_setcmykcolor_stroke(double c, double m, double y, double k);
		virtual void PS_setlinewidth(double w);
		virtual void PS_setcapjoin(Qt::PenCapStyle ca, Qt::PenJoinStyle jo);
		virtual void PS_setdash(Qt::PenStyle st, double offset, QVector<double> dash);
		virtual void PS_fill();
		virtual void PS_fillspot(const QString& color, double shade);
		virtual void PS_stroke();
		virtual void PS_strokespot(const QString& color, double shade);
		virtual void PS_fill_stroke();
		virtual void PS_newpath();
		virtual void PS_show(double x, double y);
		virtual void PS_showSub(uint chr, const QString& font, double size, bool stroke);
		virtual bool PS_image(PageItem *item, double x, double y, const QString& fn, double scalex, double scaley, const QString& Prof, bool UseEmbedded, const QString& Name = "");
		virtual bool PS_ImageData(PageItem *item, const QString& fn, const QString& Name, const QString& Prof, bool UseEmbedded);
		virtual void PS_plate(int nr, const QString& name = "");
		virtual void PS_setGray();
		virtual void PDF_Bookmark(const QString& text, uint Seite);
		virtual void PDF_Annotation(PageItem *item, const QString& text, double x, double y, double b, double h);
		virtual void PS_close();
		virtual void PS_insert(const QString& i);
		virtual void PS_TemplateStart(const QString& Name);
		virtual void PS_TemplateEnd();
		virtual void PS_UseTemplate(const QString& Name);
		virtual bool ProcessItem(ScPage* page, PageItem* item, uint PNr, bool master, bool embedded = false, bool useTemplate = false);
		virtual void ProcessPage(ScPage* page, uint PNr);
		virtual bool ProcessMasterPageLayer(ScPage* page, ScLayer& ll, uint PNr);
		virtual bool ProcessPageLayer(ScPage* a, ScLayer& ll, uint PNr);
		virtual void PS_HatchFill(PageItem *currItem);
		virtual void drawArrow(PageItem *ite, QTransform &arrowTrans, int arrowIndex);
		virtual void putColor(const QString& color, double shade, bool fill);
		virtual void putColorNoDraw(const QString& color, double shade);
		virtual void GetBleeds(ScPage* page, double& left, double& right);
		virtual void GetBleeds(ScPage* page, double& left, double& right, double& bottom, double& top);
		virtual void SetClipPath(const FPointArray &points, bool poly = true);
		virtual void SetPathAndClip(const FPointArray &points, bool clipRule);
		virtual void HandleBrushPattern(PageItem *item, QPainterPath &path, ScPage* a, uint PNr, bool master);
		virtual void HandleStrokePattern(PageItem *item);
		virtual void HandleMeshGradient(PageItem* item);
		virtual void HandlePatchMeshGradient(PageItem* item);
		virtual void HandleDiamondGradient(PageItem* item);
		virtual void HandleTensorGradient(PageItem* item);
		virtual void HandleGradientFillStroke(PageItem *item, bool stroke = true, bool forArrow = false);
		virtual void SetColor(const QString& color, double shade, double *c, double *m, double *y, double *k);
		virtual void SetColor(const ScColor& color, double shade, double *c, double *m, double *y, double *k);
		virtual void setTextSt(PageItem* ite, uint a, ScPage* pg, bool master);

	private:
		void PutStream (const QString& c);
		void PutStream (const QByteArray& array, bool hexEnc);
		void PutStream (const char* in, int length, bool hexEnc);

		bool PutImageToStream(const ScImage& image, int plate);
		bool PutImageToStream(const ScImage& image, const QByteArray& mask, int plate);

		bool PutImageDataToStream(const QByteArray& image);
		bool PutInterleavedImageMaskToStream(const QByteArray& image, const QByteArray& mask, bool gray);

		void WriteASCII85Bytes(const QByteArray& array);
		void WriteASCII85Bytes(const unsigned char* array, int length);

		void paintBorder(const TableBorder& border, const QPointF& start, const QPointF& end, const QPointF& startOffsetFactors, const QPointF& endOffsetFactors);
		
		ScribusDoc *m_Doc { nullptr };
		ScPage*      m_currentPage { nullptr };
		Optimization m_optimization { OptimizeCompat };
		OutputFormat m_outputFormat { OutputPS };

		QString ToStr(double c) const;
		QString IToStr(int c) const;
		QString MatrixToStr(double m11, double m12, double m21, double m22, double x, double y) const;
		QString PSEncode(const QString& in) const;

		QString ErrorMessage;
		QString Creator;
		QString User;
		QString Title;
		bool GraySc { false };
		int  PageIndex { 0 };
		QString FillColor { "0.0 0.0 0.0 0.0" };
		QString StrokeColor { "0.0 0.0 0.0 0.0" };
		double LineW { 1.0 };
		QString FontDesc;
		QMap<QString, QString> FontSubsetMap;
		QFile Spool;
		QDataStream spoolStream;
		int  Plate { -1 };
		bool DoSep { false };
		bool fillRule { true };
		ScColorTransform solidTransform;
		QString currentSpot;
		ColorList colorsToUse;
		QString colorDesc;
		QString cmykCustomColors;
		QString docCustomColors;
		QMap<QString, QString> spotMap;
		MultiProgressDialog* progressDialog { nullptr };
		bool abortExport { false };
		PrintOptions Options;

	protected slots:
		void cancelRequested();
};

#endif
