#ifndef COLORPICKER_H
#define COLORPICKER_H

#include "ui_colorpicker.h"
#include <QWidget>
#include "sccolor.h"
#include "scribusdoc.h"
#include "colorpicker_enum.h"

class UndoManager;
class PopupMenu;
class ColorButton;

class ColorPicker : public QWidget, Ui::ColorPicker
{
	Q_OBJECT

public:

	explicit ColorPicker(Context context = Context::Simple, QWidget *parent = nullptr);
	~ColorPicker() {};

	void setGeneralData(CPGeneralData data);
	CPGeneralData generalData();
	void setColorData(CPColorData data);
	CPColorData colorData() const;
	void setGradientData(CPGradientData data);
	CPGradientData gradientData() const;
	void setGradientVectorData(CPGradientVectorData data);
	CPGradientVectorData gradientVectorData() const;
	void setGradientMeshData(CPColorData data);
	CPColorData gradientMeshData();
	void setPatternData(CPPatternData data);
	CPPatternData patternData() const;
	void setHatchData(CPHatchData data);
	CPHatchData hatchData() const;

	void setColorButton(ColorButton *button);
	ColorButton *colorButton();

	QString toolTipText() const;

	Context context();
	void setContext(Context config);

	void setColorList(const ColorList& list);

	Mode mode();

	bool gradientEditPropertiesEnabled();
	void setGradientEditPropertiesEnabled(bool enabled);
	GradientEdit gradientEditMode();

	int type();

	void updateUiByType(int type);
	void updatePreview();

	void hideEditedPatterns(QStringList names);

	void loadLists();
	void unsetDoc();

public slots:

	void setDoc(ScribusDoc *doc);
	void languageChange();
	void iconSetChange();
	void unitChange();
	void parentClosed();
	void endPatchAdd();
	void enableButtonRemovePatch(bool enable);
	void enableMeshColor(bool enable);

private slots:
	void toggleColorLists();

	void updateColor();	
	void updateGradient();
	void updateGradientVector();
	void updateGradientMesh();
	void updateHatch();
	void updatePattern();
	void updateSize();
	void updateOverPrint(int val);

	void changedTab();

	void handleSelectionChanged();
	void handleUpdateRequest(int updateFlags);

private:
	ColorButton * m_colorButton;
	QPointer<ScribusDoc> m_doc;
	Context m_context {Context::Simple};
	Mode m_mode {Mode::Solid};
	QButtonGroup *buttonGroup;
	int m_type;
	QComboBox *comboOverprint;
	QCheckBox *checkboxUsedColors;
	PopupMenu *contextMenu;
	ColorList colorList;
	CPGeneralData m_generalData;

	PageItem * currentItemFromSelection();

	bool eventFilter(QObject *object, QEvent *event);
	void initContextMenu();	
	void setGradientList(const GradientList& list);
	void setPatternList(QHash<QString, ScPattern> *docPatterns);
	void setType(int type);
	void updateUiByMode(Mode mode);	
	void enablePatterns();
	void closeParent();

protected:
	void changeEvent(QEvent *e);

signals:
	void changed();
	void sizeChanged();
	void colorChanged();
	void gradientChanged();
	void gradientVectorChanged();
	void gradientVectorEditMode(GradientEdit);
	void gradientVectorEdit(bool);
	void gradientMeshChanged();
	void hatchChanged();
	void patternChanged();
	void colorListChanged();

	// Gradient Properties
	void createNewMesh();
	void resetMesh();
	void meshToShape();
	void reset1Control();
	void resetAllControl();
	void removePatch();
	void snapToMGrid(bool);

};

#endif // COLORPICKER_H
