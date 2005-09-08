#include <qwidget.h>
#include <scribusapi.h>

/**
 * \brief A base class for all preferences panels.
 *
 * This class provides a simple interface for prefs panel widgets.
 * They must provide an apply() slot that applies any changes to
 * the app, and they must NOT apply any changes until and unless
 * this slot is called.
 */
class SCRIBUS_API PrefsPanel : public QWidget
{
	Q_OBJECT

	public:
		PrefsPanel(QWidget* parent, const char* name)
			: QWidget(parent, name, 0) {};
		virtual ~PrefsPanel() {};

	public slots:
		/**
		 * \brief applies changes made to prefs to the app.
		 *
		 * All subclasses MUST override this.
		 *
		 * This slot may be called multiple times, once, or not at all;
		 * no assumptions should be made.
		 */
		virtual void apply() = 0;
};
