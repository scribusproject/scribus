#ifndef PULSING_TOOLBUTTON_H
#define PULSING_TOOLBUTTON_H

/**
 * Change the background color of the ToolButton to the opposite among checked and unchecked,
 * then animate the transition back to the original color.
 * Currently, done by draing a semitransparent rectangle on top of the button.
 *
 * In the future, the widget can be extend to pulse in a loop until mouseover, or other
 * fancy gadgets.
 *
 * The current minimal implementation is tailored for the buttonReferenceGuide button in
 * the align and distribute dialog:
 * - It only pulses once.
 * - It only pulses when it's checked.
 * - The animation time is a constant.
 */

#include <QColor>
#include <QToolButton>
#include <QVariantAnimation>

class QPaintEvent;
class QWidget;

class PulsingToolButton : public QToolButton
{
	Q_OBJECT

public:
	explicit PulsingToolButton(QWidget *parent = nullptr);
	void animatePulsing();

protected:
	void paintEvent(QPaintEvent *) override;

private:
	QVariantAnimation *m_transition { nullptr };
	QColor m_currentColor;
};

#endif
