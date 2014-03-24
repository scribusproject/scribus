#!/usr/bin/env python
# -*- coding: utf-8 -*-
# File: spacenquotes.py - changes typewriter quotes to typographic quotes with option to add spaces
# © 2010.08.28 Gregory Pittman
# © 2013.10.07 Enhancements by Jean-Luc Girard
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
"""
CHANGELOGS
Original script is quotes.py or autoquotes.py
9 oct 2013 :
The algorythm has been slightly changed for determining
whether a quote is opening or closing
+ it provides the choice for the type of space to add :
none or non breaking or thin or non breaking thin
+ spaces are only aded when not allready there...
+ a french version of the dialogs is included
25 oct 2013 :
choice to change the spaces for allready existing correct doublequotes
if 'none' is choosen, then existing spaces inside double quotes are deleted
+ take into account allready existing doublequotes for the correct choice open / close of next dquote
+ warning if some open/close unconsistency is detected
for example in « texte " suite »

LIMITS
- it only acts on the currently selected frame
(todo : acting on more than one selected frame,
or on whole chain of linked text frame would be nice).
- Spaces are not added outside of the quotes.
(that could be a job for another more general typographic tool)

INSTALL
download file, save it with 'quotenspace.py' name
and enjoy

USAGE
You must have a document open, and a text frame selected.
Run script from scribus menu (Script > Run script...)
There will be a dialog asking for the language for the quotes, 
Detected errors shut down the script with an appropriate message.
A dialog then asks what is your choice as for spaces.
Follow indications and answer 0, 1, 2 or 3 
Another dialog asks wether you want the script to manage 
existing correct doublequotes.


"""
import scribus

non_breaking_space = u"\u00a0"
non_breaking_thin_space = u"\u202f"
thin_space = u"\u2009"

def est_espace(text):
    return (text == ' ') or (text == non_breaking_space) or (text == non_breaking_thin_space)   or (text == thin_space)

if scribus.haveDoc() <= 0:
    scribus.messageBox('Error - (fr) Erreur', 'You need a Document open\n(fr) Ouvrez un document avant de lancer le script', scribus.ICON_WARNING, scribus.BUTTON_OK)
    sys.exit(2)

lang = scribus.valueDialog("Language", 'Choose language or country\n(fr) Choisissez la langue du texte ou le pays :\naf, be, ch, cs, de, en, es, et, fi, fr,\n hu, is, lt, mk, nl, pl, ru, se, sk, sl, sq and uk', 'fr')
if (lang == 'en'):
    ouvrant_double = u"\u201c" #lead_double
    fermant_double = u"\u201d" #follow_double
    lead_single = u"\u2018"
    follow_single = u"\u2019"
elif (lang == 'de'):
    ouvrant_double = u"\u201e"
    fermant_double = u"\u201c"
    lead_single = u"\u2019"
    follow_single = u"\u201a"
elif (lang == 'fr'):
    ouvrant_double = u"\u00ab"  #  &laquo; LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
    fermant_double = u"\u00bb"  #  &raquo; LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
    lead_single = u"\u2018"     # RIGHT SINGLE QUOTATION MARK
    follow_single = u"\u2019"   # LEFT SINGLE QUOTATION MARK
elif (lang == 'pl'):
    ouvrant_double = u"\u201e"
    fermant_double = u"\u201d"
    lead_single = u"\u201a"
    follow_single = u"\u2019"
elif ((lang == 'se') or (lang == 'fi')):
    ouvrant_double = u"\u201d"
    fermant_double = u"\u201d"
    lead_single = u"\u2019"
    follow_single = u"\u2019"
elif (lang == 'af'):
    ouvrant_double = u"\u201c"
    fermant_double = u"\u201d"
    lead_single = u"\u2018"
    follow_single = u"\u2019"
elif (lang == 'sq'):
    ouvrant_double = u"\u201e"
    fermant_double = u"\u201c"
    lead_single = u"\u2018"
    follow_single = u"\u2019"
elif ((lang == 'be') or (lang == 'ch') or (lang == 'uk') or (lang == 'ru')):
    ouvrant_double = u"\u00ab"
    fermant_double = u"\u00bb"
    lead_single = u"\u2039"
    follow_single = u"\u203a"
elif (lang == 'uk'):
    ouvrant_double = u"\u00ab"
    fermant_double = u"\u00bb"
    lead_single = u"\u2039"
    follow_single = u"\u203a"
elif (lang == 'es'):
    ouvrant_double = u"\u00ab"
    fermant_double = u"\u00bb"
    fermant_double = u"\u201d"
    lead_single = u"\u2018"
elif ((lang == 'lt') or (lang == 'mk') or (lang == 'is') or (lang == 'sk') or (lang == 'sl') or (lang == 'cs') or (lang == 'et')):
    ouvrant_double = u"\u201e"
    fermant_double = u"\u201c"
    lead_single = u"\u2019"
    follow_single = u"\u201a"
elif ((lang == 'hu') or (lang == 'nl')):
    ouvrant_double = u"\u201e"
    fermant_double = u"\u201d"
    lead_single = u"\u00bb"
    follow_single = u"\u00ab"
else:
    scribus.messageBox('Language Error', 'You need to choose an available language', icon=0, button1=1)
    sys.exit(2)

if scribus.selectionCount() == 0:
    if (lang == 'fr'):
        scribus.messageBox('Scribus - Erreur',
            "Aucun objet n'est sélectionné.\nSélectionnez un cadre de texte et recommencez.",
            scribus.ICON_WARNING, scribus.BUTTON_OK)
    else:
        scribus.messageBox('Scribus - Usage Error',
            "There is no object selected.\nPlease select a text frame and try again.",
            scribus.ICON_WARNING, scribus.BUTTON_OK)
    sys.exit(2)

if scribus.selectionCount() > 1:
    if (lang == 'fr'):
        scribus.messageBox('Scribus - Erreur',
            "Désolé, ce script ne peut pas fonctionner lorsque plusieurs objets sont sélectionnés.\nVeuillez sélectionner un seul cadre de texte, puis recommencez.",
            scribus.ICON_WARNING, scribus.BUTTON_OK)
    else:
        scribus.messageBox('Scribus - Usage Error',
            "You have more than one object selected.\nPlease select one text frame and try again.", scribus.ICON_WARNING, scribus.BUTTON_OK)
    sys.exit(2)

if (lang =='fr'):
    typeespace = scribus.valueDialog("Type d'espace", 
                "Selon les polices de caractère utilisées, choisissez le type d'espace ajouté avec les doubles guillemets français, lorsqu'il n'y en a pas déjà un.\n0 : aucun espace ajouté ; 1 : insécable ; 2 : insécable fine ; 3 : fine", 
                '1')
else :
   typeespace = scribus.valueDialog("Space inside quotes", 
                "Choose whether to add space inside double quotes.\n0: never add a space; 1: non breaking; 2: non breaking thin; 3: thin", 
                '0')

if (typeespace == '3'):
    spacenquotes = thin_space
    spacelen = 1
elif (typeespace == '2'):
    spacenquotes = non_breaking_thin_space
    spacelen = 1
elif (typeespace == '0'):
    spacenquotes = ''
    spacelen = 0
else:
    spacenquotes = non_breaking_space
    spacelen = 1

if ((1==1) or (spacenquotes != '')):
    if (lang =='fr'):
        replace_existing = scribus.valueDialog("Agir sur l'existant ?", 
                "Voulez vous aussi appliquer ce traitement sur les double-guillemets français déjà en place ? Oui: O ; Non : N ", 
                'O')
    else:
       replace_existing = scribus.valueDialog("What about existing quotes?", 
                "Should the script apply your spaces choice ALSO on already existing quotes?\nYes: Y ; No: N", 
                'N')

if ((replace_existing=='y') or (replace_existing=='Y') or (replace_existing=='o') or (replace_existing=='O')):
    replace_existing=1
else:
    replace_existing=0

textbox = scribus.getSelectedObject()
boxcount = 1

for item in scribus.getPageItems():
    if (item[0] == textbox):
        if (item[1] != 4):
            if (lang == 'fr'):
                scribus.messageBox('Scribus - Erreur',
                "L'objet sélectionné n'est pas un cadre de texte.\nVeuillez sélectionner un cadre de texte, puis recommencez.",
                scribus.ICON_WARNING, scribus.BUTTON_OK)
            else:
                scribus.messageBox('Scribus - Usage Error', "This is not a textframe. Try again.", scribus.ICON_WARNING, scribus.BUTTON_OK)
                
            sys.exit(2)
            
textlen = scribus.getTextLength(textbox)
c = 0
nbchange = 0
lastchange = 'close'
prevchar = ''

while c <= (textlen -1):
    # si on est à la fin, il faut tricher pour le dernier caractère
    if ((c + 1) > textlen - 1):
        alafin = 1
        nextchar = ' '
    else:
        alafin = 0
        scribus.selectText(c+1, 1, textbox)
        nextchar = scribus.getText(textbox)
        
    scribus.selectText(c, 1, textbox)
    char = scribus.getText(textbox)

#    scribus.messageBox("ce qui est", "position:"+str(c) +'\nchar'+char+'\nPrec:'+prevchar+'\nsuiv:'+nextchar, 
#           scribus.ICON_WARNING, scribus.BUTTON_OK)

    if (char==ouvrant_double):
        if (lastchange=='open'):
            if (lang=='fr'):
                scribus.messageBox("Oups !", 'Incohérence dans les enchainements de guillemets ouvrant et fermant. Une guillement fermante manque avant la position '+str(c) +'\nOn continue quand même', 
                        scribus.ICON_WARNING, scribus.BUTTON_OK)
            else:
                scribus.messageBox("Oops!", 'The text is inconsistent. Closing double quotes missing before position '+str(c), 
                        scribus.ICON_WARNING, scribus.BUTTON_OK)
        lastchange='open'
        if ((replace_existing == 1) and (nextchar != spacenquotes) and (alafin==0)):
            if (est_espace(nextchar)):
                scribus.selectText(c+1, 1, textbox)
                scribus.deleteText(textbox)
            scribus.insertText(spacenquotes, c+1, textbox)
            nbchange = nbchange+1

    elif (char==fermant_double):
        if (lastchange=='close'):
            if (lang=='fr'):
                scribus.messageBox("Oups !", 'Incohérence dans les enchainements de guillemets ouvrant et fermant. Une guillemet ouvrante manque avant la position '+str(c) +'\nOn continue quand même', 
                        scribus.ICON_WARNING, scribus.BUTTON_OK)
            else:
                scribus.messageBox("Oops!", 'The text is inconsistent. Opening double quotes missing before position '+str(c), 
                        scribus.ICON_WARNING, scribus.BUTTON_OK)
        lastchange = 'close'
        if ((replace_existing == 1)  and (prevchar != spacenquotes) and (c > 1)):
            if (est_espace(prevchar)):
                scribus.selectText(c-1, 1, textbox)
                scribus.deleteText(textbox)
                c=c-1
            scribus.insertText(spacenquotes, c, textbox)
            nbchange = nbchange+1
            c=c+spacelen
    
    elif (len(char) != 1): #en utf8 certains caractères ont len 2, par ex les espaces spéciaux qu'on teste au dessus
         rien="rien"          # et ça ferait planter ord()
         
    elif (char == '"'): # autrement dit : ord (char)==34
        #si on trouve une double guillemet droit " en premier caractère du texte, c'est un ouvrant !
        if (c == 0):
            scribus.deleteText(textbox)
            if (not est_espace(nextchar)):
                scribus.insertText(spacenquotes, 0, textbox)
            scribus.insertText(ouvrant_double, 0, textbox)
            lastchange='open'
        elif ((prevchar == '.') or (prevchar == ',') or (prevchar == '?') or (prevchar == '!')):
         # lets close after the end of a sentence
            scribus.deleteText(textbox)
            scribus.insertText(fermant_double, c, textbox)
            scribus.insertText(spacenquotes, c, textbox)
            lastchange='close'
            c=c+spacelen
        # 39 = ' straight apostrophe 
        elif ((ord(prevchar) == 39) and ((nextchar != ' ') and (nextchar != ',') and (nextchar != ';') and (nextchar != '.'))):
            scribus.deleteText(textbox)
            if (not est_espace(nextchar)):
                scribus.insertText(spacenquotes, c, textbox)
            scribus.insertText(ouvrant_double, c, textbox)
            lastchange='open'
        elif ((nextchar == '.') or (nextchar == ',') or (nextchar == ';')):
            scribus.deleteText(textbox)
            scribus.insertText(fermant_double, c, textbox)
            if (not est_espace(prevchar)):
                scribus.insertText(spacenquotes, c, textbox)
                c=c+spacelen
            lastchange='close'
        elif (lastchange!='open'):
            scribus.deleteText(textbox)
            if (not est_espace(nextchar)):
                scribus.insertText(spacenquotes, c, textbox)
            scribus.insertText(ouvrant_double, c, textbox)
            lastchange='open'
        else:
            scribus.deleteText(textbox)
            scribus.insertText(fermant_double, c, textbox)
            if (not est_espace(prevchar)):
                scribus.insertText(spacenquotes, c, textbox)
                c=c+spacelen
            lastchange='close'
        nbchange = nbchange+1
        
    elif ((ord(char) == 39) and (c == 0)):
        scribus.deleteText(textbox)
        scribus.insertText(lead_single, c, textbox)
        nbchange = nbchange+1
    elif (ord(char) == 39):
        if ((prevchar == '.') or (prevchar == ',') or (prevchar == '?') or (prevchar == '!')):
            scribus.deleteText(textbox)
            scribus.insertText(follow_single, c, textbox)
        elif ((ord(prevchar) == 34) and ((nextchar != ' ') and (nextchar != ',') and (nextchar != '.'))):
            scribus.deleteText(textbox)
            scribus.insertText(lead_single, c, textbox)
        elif ((prevchar != ' ') and (ord(prevchar) != 34) and (nextchar != ' ')):
            scribus.deleteText(textbox)
            scribus.insertText(follow_single, c, textbox)
        elif ((prevchar == ' ') or ((nextchar != ' ') and (ord(nextchar) != 34))):
            scribus.deleteText(textbox)
            scribus.insertText(lead_single, c, textbox)
        else:
            scribus.deleteText(textbox)
            scribus.insertText(follow_single, c, textbox)
        nbchange = nbchange+1

    c += 1
#   memprevchar = prevchar # pour message de fin seulement
    prevchar = char
    textlen = scribus.getTextLength(textbox)


debugmessage = ''
# debugmessage = '\n\nVérif : les 2 derniers caractères lus étaient <'+str(memprevchar)+'> et <'+str(char)+'>'
# debugmessage = '\n\nCheck : 2 last analysed characters were <'+str(memprevchar)+'> and <'+str(char)+'>'

scribus.setRedraw(1)
scribus.docChanged(1)

if (lang == 'fr'):
    scribus.messageBox("Fini", 'La préparation des quillemets et espaces est faite.\n'+str(nbchange)+' occurences ont été remplacées' + debugmessage, 
                        icon=0,button1=1)
else:
    scribus.messageBox("Done", 'Successfully ran script\n'+str(nbchange)+' replacements have occurred' + debugmessage, # Change this message to your liking
                        icon=0,button1=1)
