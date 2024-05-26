#ifndef SECTIONCONTAINEREXTENSIONFACTORY_H
#define SECTIONCONTAINEREXTENSIONFACTORY_H

#include <QtDesigner/QExtensionFactory>

QT_BEGIN_NAMESPACE
class QExtensionManager;
QT_END_NAMESPACE

class SectionContainerExtensionFactory: public QExtensionFactory
{
	Q_OBJECT

public:
	explicit SectionContainerExtensionFactory(QExtensionManager *parent = nullptr);

protected:
	QObject *createExtension(QObject *object, const QString &iid, QObject *parent) const override;
};

#endif // SECTIONCONTAINEREXTENSIONFACTORY_H
