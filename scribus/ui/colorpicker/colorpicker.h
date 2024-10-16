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

	void setDoc(ScribusDoc* doc);

	void setGeneralData(const CPGeneralData &data);
	const CPGeneralData& generalData() const;

	void setColorData(const CPColorData& data);
	const CPColorData& colorData() const;

	const CPGradientData& gradientData() const;
	void setGradientData(const CPGradientData& data);

	const CPGradientVectorData& gradientVectorData() const;
	void setGradientVectorData(const CPGradientVectorData& data);

	const CPColorData& gradientMeshData() const;
	void setGradientMeshData(const CPColorData& data);

	const CPPatternData& patternData() const;
	void setPatternData(const CPPatternData& data);

	const CPHatchData& hatchData() const;
	void setHatchData(const CPHatchData& data);

	void setColorButton(ColorButton *button);
	ColorButton *colorButton();

	QString toolTipText() const;

	Context context() const;
	void setContext(Context config);

	void setColorList(const ColorList& list);

	Mode mode() const;

	bool gradientEditPropertiesEnabled();
	void setGradientEditPropertiesEnabled(bool enabled);
	GradientEdit gradientEditMode();

	int type() const;

	void updateUiByType(int type);
	void updatePreview();

	void hideEditedPatterns(QStringList names);

	void loadLists();
	void unsetDoc();

public slots:
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
	ColorButton* m_colorButton { nullptr };
	QButtonGroup* buttonGroup { nullptr };
	QComboBox* comboOverprint { nullptr };
	QCheckBox* checkboxUsedColors { nullptr };
	PopupMenu* contextMenu { nullptr };

	QPointer<ScribusDoc> m_doc;
	Context m_context { Context::Simple };
	Mode m_mode { Mode::Solid };
	int m_type;
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
