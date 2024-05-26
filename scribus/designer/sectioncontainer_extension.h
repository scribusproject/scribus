#ifndef SECTIONCONTAINEREXTENSION_H
#define SECTIONCONTAINEREXTENSION_H

#include <QtDesigner/QDesignerContainerExtension>

QT_BEGIN_NAMESPACE
class QExtensionManager;
QT_END_NAMESPACE
class SectionContainer;

class SectionContainerExtension: public QObject,
										 public QDesignerContainerExtension
{
	Q_OBJECT
	Q_INTERFACES(QDesignerContainerExtension)

public:
	explicit SectionContainerExtension(SectionContainer *widget, QObject *parent);

	bool canAddWidget() const override;
	void addWidget(QWidget *widget) override;
	int count() const override;
	int currentIndex() const override;
	void insertWidget(int index, QWidget *widget) override;
	bool canRemove(int index) const override;
	void remove(int index) override;
	void setCurrentIndex(int index) override;
	QWidget *widget(int index) const override;

private:
	SectionContainer *myWidget;
};

#endif // SECTIONCONTAINEREXTENSION_H
