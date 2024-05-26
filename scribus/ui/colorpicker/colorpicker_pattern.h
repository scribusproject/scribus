#ifndef COLORPICKER_PATTERN_H
#define COLORPICKER_PATTERN_H

#include <QWidget>
#include "ui_colorpicker_pattern.h"
#include "scribusdoc.h"
#include "colorpicker_enum.h"

class ColorPickerPattern : public QWidget, Ui::ColorPickerPattern
{
	Q_OBJECT

public:
	explicit ColorPickerPattern(QWidget *parent = nullptr);
	~ColorPickerPattern(){};

    void setPatternList(QHash<QString, ScPattern> *docPatterns);

	CPPatternData patternData();

	Context context();
	void setContext(Context config);

	void hideEditedPatterns(QStringList names);

	int type();

	QString toolTipText() const;

public slots:
	void setDoc(ScribusDoc *doc);
	void setPatternData(CPPatternData pattern);
	void languageChange();	
	void iconSetChange();
	void unitChange();

private slots:
	void updatePattern();
	void updateScaleX();
	void updateScaleY();
	void updateName(QListWidgetItem *item);
	void updateSize();
	void toggleScaleRatio();

private:
	ScribusDoc *m_doc {nullptr};
	QBrush m_background;
	Context m_context {Context::Simple};
	CPPatternData m_pattern;
	int currentUnit {0};

	void connectSlots();
	void disconnectSlots();

signals:
	void patternChanged();
	void sizeChanged();

};

#endif // COLORPICKER_PATTERN_H
