#ifndef DROPDOWN_BUTTON_H
#define DROPDOWN_BUTTON_H

#include <QToolButton>

class DropdownButton : public QToolButton
{
	Q_OBJECT
public:
	DropdownButton(QWidget *parent = nullptr);

	int currentIndex() { return m_index; };
	void setCurrentIndex(int index);
	void setMenu(QMenu *menu);
	void clear();

	QAction *addAction(const QIcon &icon, const QString &text);

private:
	int m_index = 0;

private slots:
	void indexChanged(QAction *action);

signals:
	void currentIndexChanged(int index);
	void activated(int index);
};

#endif // DROPDOWN_BUTTON_H
