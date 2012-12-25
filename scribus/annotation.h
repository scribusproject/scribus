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
		//Do we need a null or an empty QString for these? Remove the qstring initialisations if null is ok.
		Annotation() : AnType(0), AnActType(0), AnAction(""), An_E_act(""), An_X_act(""), An_D_act(""),
						An_Fo_act(""), An_Bl_act(""), An_K_act(""), An_F_act(""), An_V_act(""), An_C_act(""),
						AnToolTip(""), AnRollOver(""), AnDown(""), AnBColor(""), An_Extern(""), AnBsty(0),
						AnBwid(1), AnFeed(1), AnZiel(0), AnFlag(0), AnMaxChar(-1), AnVis(0), AnChkStil(0),
						AnFont(4), AnIsChk(false), AnAAact(false), AnHTML(0), AnUseIcons(false),
						AnIPlace(1), AnScaleW(0), AnFormat(0), tmp_Chk(false), on_State(false), is_Open(false)
		{
		}
		enum AnnotationType {
			Button			= 2,
			Textfield		= 3,
			Checkbox		= 4,
			Combobox		= 5,
			Listbox			= 6,
			Text			= 10,
			Link			= 11,
			Annot3D		  	= 12,
			RadioButton		= 13
		};
		enum AnnotationAction {
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
		enum AnnotationScriptEvents {
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

		enum AnnotationFlags {
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

	void setType(int newType) { AnType=newType; }
	void setAction(const QString& newAction) { AnAction=newAction; }
	void setE_act(const QString& newE_act) { An_E_act=newE_act; }
	void setX_act(const QString& newX_act) { An_X_act=newX_act; }
	void setD_act(const QString& newD_act) { An_D_act=newD_act; }
	void setFo_act(const QString& newFo_act) { An_Fo_act=newFo_act; }
	void setBl_act(const QString& newBl_act) { An_Bl_act=newBl_act; }
	void setK_act(const QString& newK_act) { An_K_act=newK_act; }
	void setF_act(const QString& newF_act) { An_F_act=newF_act; }
	void setV_act(const QString& newV_act) { An_V_act=newV_act; }
	void setC_act(const QString& newC_act) { An_C_act=newC_act; }
	void setExtern(const QString& newExtern) { An_Extern=newExtern; }
	void setZiel(int newZiel) { AnZiel=newZiel; }
	void setActionType(int newActType) { AnActType=newActType; }
	void setToolTip(const QString& newToolTip) { AnToolTip=newToolTip; }
	void setRollOver(const QString& newRollOver) { AnRollOver=newRollOver; }
	void setDown(const QString& newDown) { AnDown=newDown; }
	void setBorderColor(const QString& newBorderColor) { AnBColor=newBorderColor; }
	void setBwid(int newBwid) { AnBwid=newBwid; }
	void setBsty(int newBsty) { AnBsty=newBsty; }
	void setFeed(int newFeed) { AnFeed=newFeed; }
	void setFlag(int newFlag) { AnFlag=newFlag; }
	void addToFlag(int dFlag) { AnFlag+=dFlag; }
	void setMaxChar(int newMaxChar) { AnMaxChar=newMaxChar; }
	void setVis(int newVis) { AnVis=newVis; }
	void setFont(int newFont) { AnFont=newFont; }
	void setChkStil(int newChkStil) { AnChkStil=newChkStil; }
	void setFormat(int newFormat) { AnFormat=newFormat; }
	void setIsChk(bool newIsChk) { AnIsChk=newIsChk; }
	void setCheckState(bool newIsChk) { tmp_Chk=newIsChk; }
	void setOpen(bool newIsOpen) { is_Open=newIsOpen; }
	void setOnState(bool newIsChk) { on_State=newIsChk; }
	void setAAact(bool newAAct) { AnAAact=newAAct; }
	void setHTML(int newHTML) { AnHTML=newHTML; }
	void setUseIcons(bool newUseIcons) { AnUseIcons=newUseIcons; }
	void setIPlace(int newIPlace) { AnIPlace=newIPlace; }
	void setScaleW(int newScaleW) { AnScaleW=newScaleW; }

	int Type() const { return AnType; }
	QString Action() const { return AnAction; }
	QString E_act() const { return An_E_act; }
	QString X_act() const { return An_X_act; }
	QString D_act() const { return An_D_act; }
	QString Fo_act() const { return An_Fo_act; }
	QString Bl_act() const { return An_Bl_act; }
	QString K_act() const { return An_K_act; }
	QString F_act() const { return An_F_act; }
	QString V_act() const { return An_V_act; }
	QString C_act() const { return An_C_act; }
	QString Extern() const { return An_Extern; }
	int Ziel() const { return AnZiel; }
	int ActionType() const { return AnActType; }
	QString ToolTip() const { return AnToolTip; }
	QString RollOver() const { return AnRollOver; }
	QString Down() const { return AnDown; }
	QString borderColor() const { return AnBColor; }
	int Bwid() const { return AnBwid; }
	int Bsty() const { return AnBsty; }
	int Feed() const { return AnFeed; }
	int Flag() const { return AnFlag; }
	int MaxChar() const { return AnMaxChar; }
	int Vis() const { return AnVis; }
	int Font() const { return AnFont; }
	int ChkStil() const { return AnChkStil; }
	int Format() const { return AnFormat; }
	bool IsChk() const { return AnIsChk; }
	bool IsChecked() const { return tmp_Chk; }
	bool IsOpen() const { return is_Open; }
	bool IsOn() const { return on_State; }
	bool AAact() const { return AnAAact; }
	int HTML() const { return AnHTML; }
	bool UseIcons() const { return AnUseIcons; }
	int IPlace() const { return AnIPlace; }
	int ScaleW() const { return AnScaleW; }
	
	protected:
		int AnType;
		int AnActType;
		QString AnAction;
		QString An_E_act;
		QString An_X_act;
		QString An_D_act;
		QString An_Fo_act;
		QString An_Bl_act;
		QString An_K_act;
		QString An_F_act;
		QString An_V_act;
		QString An_C_act;
		QString AnToolTip;
		QString AnRollOver;
		QString AnDown;
		QString AnBColor;
		QString An_Extern;
		int AnBsty;
		int AnBwid;
		int AnFeed;
		int AnZiel;
		int AnFlag;
		int AnMaxChar;
		int AnVis;
		int AnChkStil;
		int AnFont;
		bool AnIsChk;
		bool AnAAact;
		int AnHTML;
		bool AnUseIcons;
		int AnIPlace;
		int AnScaleW;
		int AnFormat;
		bool tmp_Chk;
		bool on_State;
		bool is_Open;
};

#endif
