#ifndef STICKYPOPUP_H
#define STICKYPOPUP_H

#include <QToolButton>
#include <QWidget>
#include <QVBoxLayout>

class FloatingWindow : public QWidget
{
	Q_OBJECT
public:

	explicit FloatingWindow(QWidget * child, QWidget *reference, QWidget *parent = nullptr);

	QWidget *child();
	QWidget *reference();

public slots:
	void iconSetChange();

protected:
	bool eventFilter(QObject *obj, QEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void hideEvent(QHideEvent *event);

private:

	QWidget * m_child;
	QWidget * m_reference;
	QWidget * m_handle;
	QVBoxLayout * m_layout;
	QPoint m_mousePos;
	QToolButton *buttonClose;

	/*!
	 * \brief Screen size of all screens
	 * \return
	 */
	QSize screenSize();

	/*!
	 * \brief Calculate screen position relative to reference() widget.
	 */
	void calculatePosition();

signals:
	void closed();

public slots:
	void show(QWidget *reference = nullptr);
	void updateSize();

};

#endif // STICKYPOPUP_H
