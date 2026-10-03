/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QRegularExpression>
#include <QFontMetrics>

#include "scconfig.h"

#include "api/api_application.h"
#include "iconmanager.h"
#include "splash.h"
#include "util.h"

ScSplashScreen::ScSplashScreen(const QPixmap & pixmap, const QRect messageRect, Qt::WindowFlags f, bool darkMode )
	: QSplashScreen(pixmap, f), m_darkMode(darkMode)
{
#if defined _WIN32
	QFont font("Lucida Sans Unicode", 9);
#elif defined(__INNOTEK_LIBC__)
	QFont font("WarpSans", 8);
#elif defined(Q_OS_MACOS)
	QFont font("Helvetica Regular", 11);
#else
	QFont font("DejaVu Sans", 8);
	if (!font.exactMatch())
		font.setFamily("Bitstream Vera Sans");
#endif
	setFont(font);
	m_messageRect = messageRect;
}

void ScSplashScreen::setDarkMode(bool darkMode)
{
	if (m_darkMode == darkMode)
		return;
	m_darkMode = darkMode;
	update();
}

bool ScSplashScreen::isModernArtwork(const QPixmap& pixmap)
{
	if (pixmap.isNull())
		return false;
	const qreal ratio = pixmap.devicePixelRatioF();
	return qRound(pixmap.width() / ratio) == 720 && qRound(pixmap.height() / ratio) == 360;
}

QPixmap ScSplashScreen::previewPixmap(const QPixmap& background, bool darkMode)
{
	if (!isModernArtwork(background))
		return background;
	QPixmap preview = background;
	QPainter painter(&preview);
	paintBranding(&painter, QApplication::font(), darkMode, QString(), false);
	return preview;
}

void ScSplashScreen::setStatus( const QString &message )
{
	static QRegularExpression rx("&\\S*");
	QString tmp(message);
	qsizetype f = 0;
	while (f != -1)
	{
		f = tmp.indexOf(rx);
		if (f != -1)
		{
			tmp.remove(f, 1);
			f = 0;
		}
	}

	showMessage(tmp);
}

void ScSplashScreen::drawContents(QPainter* painter)
{
	if (!isModernArtwork(pixmap()))
	{
		// Older icon sets already contain their branding in the image.
		if (!message().isEmpty())
		{
			const QSizeF size(pixmap().width() / pixmap().devicePixelRatioF(),
			                  pixmap().height() / pixmap().devicePixelRatioF());
			const QRect statusRect = m_messageRect.isValid()
				? m_messageRect : QRect(20, qRound(size.height()) - 42, qRound(size.width()) - 40, 24);
			painter->setPen(m_darkMode ? Qt::white : Qt::black);
			painter->drawText(statusRect, Qt::AlignLeft | Qt::AlignVCenter,
			                  painter->fontMetrics().elidedText(message(), Qt::ElideRight, statusRect.width()));
		}
		return;
	}
	paintBranding(painter, font(), m_darkMode, message(), true);
}

void ScSplashScreen::paintBranding(QPainter* painter, const QFont& baseFont, bool isDark,
	                              const QString& status, bool showStatus)
{
	painter->setRenderHint(QPainter::Antialiasing, true);
	const QColor textColor = isDark ? QColor(245, 245, 247) : QColor(32, 34, 38);
	const QColor secondaryColor = isDark ? QColor(190, 194, 201) : QColor(84, 88, 96);
	const QColor accentColor(10, 132, 255);

	const QPixmap appIcon = IconManager::instance().loadPixmap("app-icon", QSize(78, 78));
	painter->drawPixmap(QRect(38, 38, 78, 78), appIcon);

	QFont titleFont(baseFont);
	titleFont.setPointSize(32);
	titleFont.setWeight(QFont::DemiBold);
	painter->setFont(titleFont);
	painter->setPen(textColor);
	painter->drawText(QRect(134, 42, 205, 48), Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("Apscribe"));

	QFont taglineFont(baseFont);
	taglineFont.setPointSize(16);
	taglineFont.setWeight(QFont::DemiBold);
	painter->setFont(taglineFont);
	painter->setPen(accentColor);
	painter->drawText(QRect(134, 88, 205, 28), Qt::AlignLeft | Qt::AlignVCenter,
	                  QFontMetrics(taglineFont).elidedText(tr("Publish beautifully."), Qt::ElideRight, 205));

	QFont bodyFont(baseFont);
	bodyFont.setPointSize(13);
	painter->setFont(bodyFont);
	painter->setPen(secondaryColor);
	painter->drawText(QRect(134, 118, 205, 24), Qt::AlignLeft | Qt::AlignVCenter,
	                  QFontMetrics(bodyFont).elidedText(tr("Version %1").arg(ScribusAPI::getVersion()), Qt::ElideRight, 205));
	if (showStatus)
		painter->drawText(QRect(38, 225, 286, 24), Qt::AlignLeft | Qt::AlignVCenter,
		                  QFontMetrics(bodyFont).elidedText(status, Qt::ElideRight, 286));

	QFont footerFont(baseFont);
	footerFont.setPointSize(11);
	painter->setFont(footerFont);
	painter->setPen(secondaryColor);
	painter->drawText(QRect(38, 291, 286, 24), Qt::AlignLeft | Qt::AlignVCenter,
		tr("Open Source Desktop Publishing"));
	if (ScribusAPI::isSVN())
		painter->drawText(QRect(38, 318, 286, 20), Qt::AlignLeft | Qt::AlignVCenter, tr("Development build"));
}
