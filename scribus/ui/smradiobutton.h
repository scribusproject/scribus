//
// C++ Interface: smradiobutton
//
// Description: a clone of SMCheckBox
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef SMRADIOBUTTON_H
#define SMRADIOBUTTON_H

#include <QRadioButton>

class SMRadioButton : public QRadioButton
{
	Q_OBJECT

	public:
		SMRadioButton(QWidget *parent);
		~SMRadioButton() {}

		void setChecked(bool val);
		void setChecked(bool val, bool isParentVal);

		void setParentValue(bool);
		bool useParentValue();

	private:
		bool m_hasParent = false;
		bool m_useParentValue = false;
		bool m_pValue = false;

		void setFont(bool wantBold);
};


#endif //SMRADIOBUTTON_H

