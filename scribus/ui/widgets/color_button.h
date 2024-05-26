#ifndef COLOR_BUTTON_H
#define COLOR_BUTTON_H

#include <QToolButton>
#include <QPointer>
#include "scribusapi.h"
#include "ui/colorpicker/colorpicker_enum.h"

class ColorListBox;
class FloatingWindow;
class PopupMenu;
class PageItem;
class ScribusDoc;
class ScPattern;
class UndoManager;
class ColorPicker;

class SCRIBUS_API ColorButton : public QToolButton
{
	Q_OBJECT

public:

	enum MenuContextType
	{
		None = 0,
		Floating = 1,
		List = 2
	};

	ColorButton(QWidget *parent = nullptr);
	~ColorButton(){};

	/*!
	 * \brief Show or hide small dot in bottom right corner filled with foreground brush.
	 * \param enabled
	 */
	void setHasDot(bool enabled);
	bool hasDot();

	QSize backgroundDotSize();
	QSize foregroundDotSize();

	/*!
	 * \brief Set type of context to show on click event, e.g. floating or list.
	 * \param type
	 */
	void setMenuContextType(MenuContextType type);
	MenuContextType menuContextType() { return m_contextType; };

	// General Data
	void setGeneral(int overprint);
	void setGeneralData(CPGeneralData data);
	CPGeneralData generalData();

	// Color
	void setColor(QString colorName, double shade = 100.0, double opacity = 0.0);
	void setColorData(CPColorData data);
	CPColorData colorData() const;
	QColor color() const;
	QString colorName() const;

	// Gradient
	void setGradient(QString gradientName, VGradient gradient, VGradient::VGradientRepeatMethod repeatMethod = VGradient::pad,
					 QString color1 = QString(), QString color2 = QString(), QString color3 = QString(), QString color4 = QString(),
					 double color1Shade = 0.0, double color2Shade = 0.0, double color3Shade = 0.0, double color4Shade = 0.0,
					 double color1Alpha = 1.0, double color2Alpha = 1.0, double color3Alpha = 1.0, double color4Alpha = 1.0);
	void setGradientData(CPGradientData data);
	CPGradientData gradientData() const;
	VGradient gradient() const;
	QString gradientName() const;

	// Gradient Mesh
	void setGradientMesh(QString colorName, double shade = 100.0, double opacity = 0.0);
	void setGradientMeshData(CPColorData data);
	CPColorData gradientMeshData() const;

	// Gradient Vector
	void setGradientVector(double StartX, double StartY, double EndX, double EndY, double FocalX, double FocalY, double Scale, double Skew,
						   double C1X = 0.0, double C1Y = 0.0, double C2X = 0.0, double C2Y = 0.0, double C3X = 0.0, double C3Y = 0.0, double C4X = 0.0, double C4Y = 0.0, double C5X = 0.0, double C5Y = 0.0);
	void setGradientVectorData(CPGradientVectorData data);
	CPGradientVectorData gradientVectorData();

	// Pattern
	void setPattern(QString patternName, double offsetX, double offsetY, double scaleX, double scaleY, double skewX, double skewY, double angle, double spacing, bool mirrorX, bool mirrorY, bool onPath);
	void setPatternData(CPPatternData data);
	CPPatternData patternData() const;
	ScPattern pattern() const;
	QString patternName() const;

	// Hatch
	void setHatch(int type, double distance, double angle, bool hasBackground, QString backgroundColor, QString foregroundColor);
	void setHatchData(CPHatchData data);
	CPHatchData hatchData() const;

	/*!
	 * \brief Set context of color button, e.g. fill or line. A context set the right configuration on color picker widget.
	 * \param config
	 */
	void setContext(Context config);
	Context context();	

	Mode mode() { return m_mode; };

	void setType(int type);
	int type() { return m_type; };

	void updatePreview();
	void unsetDoc();

	/*!
	 * \brief Set context widget to show on click event. Set MenuContextType to "List".
	 * \param widget
	 */
	void setContextWidget(QWidget *widget);
	QWidget *contextWidget();

public slots:
	void setDoc(ScribusDoc *doc);

	void setBackground(QBrush background);
	void setForeground(QBrush foreground);

	void toggleFloatingContext();
	void updateFloatingContext();

private slots:	
	void updateColor();
	void updateGradient();
	void updateGradientVector();
	void updateGradientMesh();
	void updateHatch();
	void updatePattern();
	void updateColorPicker(ColorPicker *colorPicker);

private:
	QBrush m_background {Qt::NoBrush};
	QBrush m_foreground {Qt::NoBrush};
	bool m_hasDot {false};
	QWidget *stickyWidget {nullptr};
	FloatingWindow *floatingWidget {nullptr};
	PopupMenu *popupmenu {nullptr};
	QPointer<ScribusDoc> m_doc;
	Context m_context {Context::Simple};
	MenuContextType m_contextType {MenuContextType::None};
	Mode m_mode {Mode::Solid};
	CPColorData m_colorData;
	CPGradientData m_gradientData;
	CPGradientVectorData m_gradientVectorData;
	CPColorData m_meshColorData;
	CPHatchData m_hatchData;
	CPPatternData m_patternData;
	CPGeneralData m_itemData;
	bool m_isMask {false};
	int m_type {0};
	bool m_ignoreItemType {true};

	QBrush background();
	QBrush foreground();
	QBrush renderBrush();
	bool isMask();
	void setModeByType(int type);
	QColor colorFromName(QString colorName, double shade);
	QBrush colorBrush(QSize size, QString colorName, double shade = 100.0, double opacity = 1.0);

	QBrush brushSolid();
	QBrush brushGradient();
	QBrush brushHatch();
	QBrush brushPattern();

protected:

	void paintEvent(QPaintEvent *e);

signals:
	void changed();
	void colorChanged();
	void gradientChanged();
	void gradientVectorChanged();
	void gradientMeshChanged();
	void hatchChanged();
	void patternChanged();

};

#endif // COLOR_BUTTON_H
