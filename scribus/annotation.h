/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : 2005
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef ANNOTATION_H
#define ANNOTATION_H

#ifdef HAVE_CONFIG_H
#include "scconfig.h"
#endif

#include <QString>
#include "scribusapi.h"

//FIXME: Someone please fix these variable names..
//They are simply moved here, not all fixed.
//TODO make the usage in various places a little simpler
class SCRIBUS_API Annotation // : public SaxIO
{
	public:
		Annotation() = default;
		enum AnnotationType
		{
			Button		= 2,
			Textfield	= 3,
			Checkbox	= 4,
			Combobox	= 5,
			Listbox		= 6,
			Text		= 10,
			Link		= 11,
			Annot3D		= 12,
			RadioButton	= 13
		};
		enum AnnotationAction
		{
			Action_None				= 0,
			Action_JavaScript		= 1,
			Action_GoTo				= 2,
			Action_SubmitForm		= 3,
			Action_ResetForm		= 4,
			Action_ImportData		= 5,
			Action_GoToR_FileRel	= 7,
			Action_URI				= 8,
			Action_GoToR_FileAbs	= 9,
			Action_Named			= 10
		};
		enum AnnotationScriptEvents
		{
			Java_ReleaseButton	= 0,
			Java_PressButton	= 1,
			Java_EnterWidget	= 2,
			Java_LeaveWidget	= 3,
			Java_FocusIn		= 4,
			Java_FocusOut		= 5,
			Java_SelectionChg	= 6,
			Java_FieldFormat	= 7,
			Java_FieldValidate	= 8,
			Java_FieldCalculate	= 9
		};
		enum AnnotationIconType
		{
			Icon_Note			= 0,
			Icon_Comment		= 1,
			Icon_Key			= 2,
			Icon_Help			= 3,
			Icon_NewParagraph	= 4,
			Icon_Paragraph		= 5,
			Icon_Insert			= 6,
			Icon_Cross			= 7,
			Icon_Circle			= 8
		};
		enum AnnotationFlags
		{
			Flag_ReadOnly			=		   1,	// Bit  1
			Flag_Required			=		   2,	// Bit  2
			Flag_NoExport			=		   4,	// Bit  3
			Flag_Multiline			=		4096,	// Bit 13
			Flag_Password			=		8192,	// Bit 14
			Flag_NoToggleToOff		=	   16384,	// Bit 15
			Flag_Radio				=	   32768,	// Bit 16
			Flag_PushButton			=	   65536,	// Bit 17
			Flag_Combo				=	  131072,	// Bit 18
			Flag_Edit				=	  262144,	// Bit 19
			Flag_Sort				=	  524288,	// Bit 20
			Flag_FileSelect			=	 1048576,	// Bit 21
			Flag_MultiSelect		=	 2097152,	// Bit 22
			Flag_DoNotSpellCheck	=	 4194304,	// Bit 23
			Flag_DoNotScroll		=	 8388608,	// Bit 24
			Flag_Comb				=	16777216,	// Bit 25
			Flag_RichText			=	33554432,	// Bit 26 for Textfields only
			Flag_RadiosInUnison		=	33554432,	// Bit 26 for Radio Buttons only
			Flag_CommitOnSelChange	=	67108864	// Bit 27
		};

		bool isAcroFormField() const
		{
			return m_type == Annotation::Button ||
				m_type == Annotation::Textfield ||
				m_type == Annotation::Checkbox ||
				m_type == Annotation::Combobox ||
				m_type == Annotation::Listbox ||
				m_type == Annotation::RadioButton;
		}

		void setType(int newType) { m_type = newType; }
		void setAction(const QString& newAction) { m_action = newAction; }
		void setE_act(const QString& newE_act) { m_E_act = newE_act; }
		void setX_act(const QString& newX_act) { m_X_act = newX_act; }
		void setD_act(const QString& newD_act) { m_D_act = newD_act; }
		void setFo_act(const QString& newFo_act) { m_Fo_act = newFo_act; }
		void setBl_act(const QString& newBl_act) { m_Bl_act = newBl_act; }
		void setK_act(const QString& newK_act) { m_K_act = newK_act; }
		void setF_act(const QString& newF_act) { m_F_act = newF_act; }
		void setV_act(const QString& newV_act) { m_V_act = newV_act; }
		void setC_act(const QString& newC_act) { m_C_act = newC_act; }
		void setExtern(const QString& newExtern) { m_extern = newExtern; }
		void setZiel(int newZiel) { m_ziel = newZiel; }
		void setActionType(int newActType) { m_actType = newActType; }
		void setToolTip(const QString& newToolTip) { m_toolTip = newToolTip; }
		void setRollOver(const QString& newRollOver) { m_rollOver = newRollOver; }
		void setDown(const QString& newDown) { m_down = newDown; }
		void setBorderColor(const QString& newBorderColor) { m_borderColor = newBorderColor; }
		void setBorderWidth(int newBorderWidth) { m_borderWidth = newBorderWidth; }
		void setBorderStyle(int newBorderStyle) { m_borderStyle = newBorderStyle; }
		void setFeed(int newFeed) { m_feed = newFeed; }
		void setFlag(int newFlag) { m_flag = newFlag; }
		void addToFlag(int dFlag) { m_flag += dFlag; }
		void setMaxChar(int newMaxChar) { m_maxChar = newMaxChar; }
		void setVis(int newVis) { m_vis = newVis; }
		void setFont(int newFont) { m_font = newFont; }
		void setChkStil(int newChkStil) { m_chkStil = newChkStil; }
		void setFormat(int newFormat) { m_format = newFormat; }
		void setIsChk(bool newIsChk) { m_isChk = newIsChk; }
		void setCheckState(bool newCheckState) { tmp_Chk = newCheckState; }
		void setOpen(bool newIsOpen) { is_Open = newIsOpen; }
		void setOnState(bool newOnState) { on_State = newOnState; }
		void setAAact(bool newAAct) { m_aAact = newAAct; }
		void setHTML(int newHTML) { m_html = newHTML; }
		void setUseIcons(bool newUseIcons) { m_useIcons = newUseIcons; }
		void setIPlace(int newIPlace) { m_iPlace = newIPlace; }
		void setScaleW(int newScaleW) { m_scaleW = newScaleW; }
		void setAnOpen(bool newIsOpen) { m_isOpen = newIsOpen; }
		void setIcon(int newIcon) { m_icon = newIcon; }

		int Type() const { return m_type; }
		QString Action() const { return m_action; }
		QString E_act() const { return m_E_act; }
		QString X_act() const { return m_X_act; }
		QString D_act() const { return m_D_act; }
		QString Fo_act() const { return m_Fo_act; }
		QString Bl_act() const { return m_Bl_act; }
		QString K_act() const { return m_K_act; }
		QString F_act() const { return m_F_act; }
		QString V_act() const { return m_V_act; }
		QString C_act() const { return m_C_act; }
		QString Extern() const { return m_extern; }
		int Ziel() const { return m_ziel; }
		int ActionType() const { return m_actType; }
		QString ToolTip() const { return m_toolTip; }
		QString RollOver() const { return m_rollOver; }
		QString Down() const { return m_down; }
		QString borderColor() const { return m_borderColor; }
		int borderWidth() const { return m_borderWidth; }
		int borderStyle() const { return m_borderStyle; }
		int Feed() const { return m_feed; }
		int Flag() const { return m_flag; }
		int MaxChar() const { return m_maxChar; }
		int Vis() const { return m_vis; }
		int Font() const { return m_font; }
		int ChkStil() const { return m_chkStil; }
		int Format() const { return m_format; }
		bool IsChk() const { return m_isChk; }
		bool IsChecked() const { return tmp_Chk; }
		bool IsOpen() const { return is_Open; }
		bool IsOn() const { return on_State; }
		bool AAact() const { return m_aAact; }
		int HTML() const { return m_html; }
		bool UseIcons() const { return m_useIcons; }
		int IPlace() const { return m_iPlace; }
		int ScaleW() const { return m_scaleW; }
		bool IsAnOpen() const { return m_isOpen; }
		int Icon() const { return m_icon; }

	private:
		QString m_Bl_act;
		QString m_C_act;
		QString m_D_act;
		QString m_E_act;
		QString m_F_act;
		QString m_Fo_act;
		QString m_K_act;
		QString m_V_act;
		QString m_X_act;
		QString m_action;
		QString m_borderColor;
		QString m_down;
		QString m_extern;
		QString m_rollOver;
		QString m_toolTip;
		bool is_Open {false};
		bool m_aAact {false};
		bool m_isChk {false};
		bool m_isOpen {false};
		bool m_useIcons {false};
		bool on_State {false};
		bool tmp_Chk {false};
		int m_actType {0};
		int m_borderStyle {0};
		int m_borderWidth {1};
		int m_chkStil {0};
		int m_feed {1};
		int m_flag {0};
		int m_font {4};
		int m_format {0};
		int m_html {0};
		int m_iPlace {1};
		int m_icon {0};
		int m_maxChar {-1};
		int m_scaleW {0};
		int m_type {0};
		int m_vis {0};
		int m_ziel {0};
};

#endif
