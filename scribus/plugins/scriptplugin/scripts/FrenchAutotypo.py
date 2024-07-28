#!/usr/bin/env python
# -*- coding: utf-8 -*-
# File: fr_autotypo².py Version : 1.1.3 - 2023 10 18
# Fixes typewriter, single and double quotes along with their leading and following spaces 
# + fixes double spaces everywhere 
# + applies french typography (since default langage is french)
#
# © 2020.06 Creation of autotypo by JLuc 
# following © 2013 autoquote2 enhancements by JLuc 
# of © 2010 autoquote.py by Gregory Pittman 
# and including 2023.10 fixes by Vincent
#
# For issues or enhancements, use https://bugs.scribus.net/view.php?id=16804
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# INPUTS
# - choose a text frame, launch script
# - it defaults to french but altering the script enables to choose other language or painfull interactive mode
# - default space to add for typography is thin nonbreakable space. 
# - existing spaces should be replaced
# - multiple spaces should be merged into single space
# FEATURES
# - replaces " with « and » as required
# - warns when « and » dont match or for other such issues
# - adds choosen spaces after « and before »
# - applies some heuristics (some would call that AI) to best deal with ' and "
# - replace multiple following spaces with just one
# - when langages is french, does more typography job :
# - - replaces or adds the choosen space before ! ? ; : and …
# - - doesnt mess urls = doesnt change http://scribus.net
# - - only adds one choosen space before a set of double spaces as !!!?!
# - provides a just-go-ahead-dont-ask-for-options mode for geeks : juste change do_ask value to False in script
# LIMITS
# - recognizes urls with "p:/" or "ww." patterns (=> possible false positive)
# - same choosen space for « » ; ! ; : …
# - space is added or replaced with absolutely no local-font awareness
# - only manages selected frame and doesnt follow links accross linked text frames <-- PR wanted

import scribus

non_breaking_space = u"\u00a0"
non_breaking_thin_space = u"\u202f"
thin_space = u"\u2009"
ensure_space_before_list = '»;!?:…€%';

# Comment whichever line you wish to avoid
# do_ask = True
do_ask=False

# default values when not asking
lang='fr'
space_character=non_breaking_thin_space
space_len=1
replace_existing=True
merge_spaces=True
do_hyphenate=True
with_feedback=False

if do_ask:
	do_hyphenate=False
	with_feedback=True

def is_a_space(text):
    return (text == ' ') or (text == non_breaking_space) or (text == non_breaking_thin_space) or (text == thin_space)

def ensure_space_before(char):
    return (lang == 'fr') and (char in ensure_space_before_list );

if scribus.haveDoc() <= 0:
    scribus.messageBox('Error - (fr) Erreur', 'You need a Document open\n(fr) Ouvrez un document avant de lancer le script', scribus.ICON_WARNING, scribus.BUTTON_OK)
    sys.exit(2)

#
# First choice for langage
#
if do_ask:
    lang = scribus.valueDialog("Language", 'Choose language or country\nChoisissez la langue du texte ou le pays :\n  af, be, ch, cs, de, de-g, en, es, et, fi, fr,\n  hu, is, lt, mk, nl, pl, ru, se, sk, sl, sq and uk', 'fr')

if (lang == 'en'):
    lead_double = u"\u201c" #lead_double
    follow_double = u"\u201d" #follow_double
    lead_single = u"\u2018"
    follow_single = u"\u2019"
elif (lang == 'de'):
    lead_double = u"\u201e"
    follow_double = u"\u201c"
    lead_single = u"\u2019"
    follow_single = u"\u201a"
elif (lang == 'de-g'):          # German with inverted guillemets for double quote
    lead_double = u"\u00bb"
    follow_double = u"\u00ab"
    lead_single = u"\u2019"
    follow_single = u"\u201a"
elif (lang == 'fr'):
    lead_double = u"\u00ab"  #  &laquo; LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
    follow_double = u"\u00bb"  #  &raquo; LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
    lead_single = u"\u2018"     # RIGHT SINGLE QUOTATION MARK
    follow_single = u"\u2019"   # LEFT SINGLE QUOTATION MARK
elif (lang == 'pl'):
    lead_double = u"\u201e"
    follow_double = u"\u201d"
    lead_single = u"\u201a"
    follow_single = u"\u2019"
elif ((lang == 'se') or (lang == 'fi')):
    lead_double = u"\u201d"
    follow_double = u"\u201d"
    lead_single = u"\u2019"
    follow_single = u"\u2019"
elif (lang == 'af'):
    lead_double = u"\u201c"
    follow_double = u"\u201d"
    lead_single = u"\u2018"
    follow_single = u"\u2019"
elif (lang == 'sq'):
    lead_double = u"\u201e"
    follow_double = u"\u201c"
    lead_single = u"\u2018"
    follow_single = u"\u2019"
elif ((lang == 'be') or (lang == 'ch') or (lang == 'uk') or (lang == 'ru')):
    lead_double = u"\u00ab"
    follow_double = u"\u00bb"
    lead_single = u"\u2039"
    follow_single = u"\u203a"
elif (lang == 'uk'):
    lead_double = u"\u00ab"
    follow_double = u"\u00bb"
    lead_single = u"\u2039"
    follow_single = u"\u203a"
elif (lang == 'es'):
    lead_double = u"\u00ab"
    follow_double = u"\u00bb"
    lead_single = u"\u2018"
    follow_double = u"\u2019"
elif ((lang == 'lt') or (lang == 'is') or (lang == 'sk') or (lang == 'sl') or (lang == 'cs') or (lang == 'et')):
    lead_double = u"\u201e"
    follow_double = u"\u201c"
    lead_single = u"\u201a"
    follow_single = u"\u2018"
elif (lang == 'mk'):
    lead_double = u"\u201e"
    follow_double = u"\u201c"
    lead_single = u"\u2019"
    follow_single = u"\u2018"
elif ((lang == 'hu') or (lang == 'nl')):
    lead_double = u"\u201e"
    follow_double = u"\u201d"
    lead_single = u"\u00bb"
    follow_single = u"\u00ab"
else:
    scribus.messageBox('Language Error', 'You need to choose an available language', scribus.ICON_WARNING, scribus.BUTTON_OK)
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

# Following dialogs are i18ned
if do_ask:
  if (lang =='fr'):
    space_type = scribus.valueDialog("Type d'espace",
                "Selon les polices de caractère utilisées,\nchoisissez le type d'espace ajouté avec les doubles guillemets français\net avant les signes doubles :\n  0 : aucun espace ajouté\n  1 : insécable fine\n  2 : insécable\n  3 : fine",
                '1')
  else:
    space_type = scribus.valueDialog("Inside quote added space",
                "Depending on the used fonts, choose the space to be added inside \ndouble quotes, in case there are none already.\n  0 : dont add a space\n  1 : non breaking thin\n  2 : non breaking\n  3 : thin",
                '0')

  if (space_type == '3'):
    space_character = thin_space
    space_len = 1
  elif (space_type == '1'):
    space_character = non_breaking_thin_space
    space_len = 1
  elif (space_type == '0'):
    space_character = ''
    space_len = 0
  else:
    space_character = non_breaking_space
    space_len = 1

  if (lang =='fr'):
    replace_existing = scribus.valueDialog("Agir sur l'existant ?",
            "Voulez vous aussi appliquer votre choix d'espaces sur les espaces déjà en place pour les double-guillemets et signes doubles (si cette option est retenue) ?\n  O : Oui\n  N : Non ",
            'O')
  else:
    replace_existing = scribus.valueDialog("What about existing quotes ?",
            "Should the script ALSO apply your spaces-choice on already existing quotes?\n  Y : Yes\n  N : No",
            'N')
  if (replace_existing == 'n') or (replace_existing == 'N'):
    replace_existing=False
  else:
    replace_existing=True

  if (lang =='fr'):
    merge_spaces = scribus.valueDialog("Espaces multiples",
      "Voulez vous dédoublonner les espaces qui se suivent ?\n  O : Oui\n  N : Non ",
      'O')
  else:
    merge_spaces = scribus.valueDialog("Space'n'spaces",
      "Do you want to merge spaces ?\n  Y : Yes\n  N : No ",
      'Y')
  if (merge_spaces == 'N') or (merge_spaces == 'n'):
    merge_spaces = False
  else:
    merge_spaces = True

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
quotes_lastchange = 'close' # la prochaine quote doit être ouvrante
prevchar = ' '
is_in_url = False

if do_hyphenate:
    scribus.hyphenateText();

while c <= (textlen - 1):
    # si on est à la fin, il faut tricher pour le dernier caractère
    if ((c + 1) > textlen - 1):
        end_reached = True
        nextchar = ' '
    else:
        end_reached = False
        scribus.selectText(c+1, 1, textbox)
        nextchar = scribus.getText(textbox)
       
    scribus.selectText(c, 1, textbox)
    char = scribus.getText(textbox)
#    scribus.messageBox("Ce qui est", "position:"+str(c) +'\nchar'+str(char)+'\nPrec:'+str(prevchar)+'\nsuiv:'+str(nextchar),
#          scribus.ICON_WARNING, scribus.BUTTON_OK)

    if (char==lead_double):
        if (quotes_lastchange=='open'):
            if (lang=='fr'):
                scribus.messageBox("Oups !", 'Incohérence dans les enchaînements de guillemets ouvrant et fermant. Un guillemet fermant manque avant la position '+str(c) +'\nOn continue quand même',
                        scribus.ICON_WARNING, scribus.BUTTON_OK)
            else:
                scribus.messageBox("Oops !", 'The text is not consistent. Closing doublequote missing before position '+str(c),
                        scribus.ICON_WARNING, scribus.BUTTON_OK)
        quotes_lastchange='open'
        if (replace_existing and (nextchar != space_character) and (not end_reached)):
            if (is_a_space(nextchar)):
                scribus.selectText(c+1, 1, textbox)
                scribus.deleteText(textbox)
            scribus.insertText(space_character, c+1, textbox)
            nbchange = nbchange+1

    elif (char==follow_double):
        if (quotes_lastchange=='close'):
            if (lang=='fr'):
                scribus.messageBox("Oups !", 'Incohérence dans les enchaînements de guillemets ouvrant et fermant. Un guillemet ouvrant manque avant la position '+str(c) +'\nOn continue quand même',
                        scribus.ICON_WARNING, scribus.BUTTON_OK)
            else:
                scribus.messageBox("Oops !", 'The text is not consistent. Opening doublequote missing before position '+str(c),
                        scribus.ICON_WARNING, scribus.BUTTON_OK)
        quotes_lastchange = 'close'
        if (replace_existing  and (prevchar != space_character) and (c > 1)):
            if (is_a_space(prevchar)):
                scribus.selectText(c-1, 1, textbox)
                scribus.deleteText(textbox)
                c=c-1
            scribus.insertText(space_character, c, textbox)
            nbchange = nbchange+1
            c=c+space_len
   
    elif (len(char) != 1):         # en utf8 certains caractères ont len 2, par ex les espaces spéciaux qu'on teste au dessus
         do_nothing = "nothing_at_all"      # et ça ferait planter ord()
         
    elif (char == '"'): # autrement dit : ord (char)==34
        #si on trouve une double guillemet droit " en premier caractère du texte, c'est un ouvrant !
        if (c == 0):
            scribus.deleteText(textbox)
            if (not is_a_space(nextchar)):
                scribus.insertText(space_character, 0, textbox)
            scribus.insertText(lead_double, 0, textbox)
            quotes_lastchange='open'
        elif ((prevchar == '.') or (prevchar == ',') or (prevchar == '?') or (prevchar == '!')):
         # lets close after the end of a sentence
            scribus.deleteText(textbox)
            scribus.insertText(follow_double, c, textbox)
            scribus.insertText(space_character, c, textbox)
            quotes_lastchange='close'
            c=c+space_len
        # 39 = ' straight apostrophe
        elif ((ord(prevchar) == 39) and ((nextchar != ' ') and (nextchar != ',') and (nextchar != ';') and (nextchar != '.'))):
            scribus.deleteText(textbox)
            if (not is_a_space(nextchar)):
                scribus.insertText(space_character, c, textbox)
            scribus.insertText(lead_double, c, textbox)
            quotes_lastchange='open'
        elif ((nextchar == '.') or (nextchar == ',') or (nextchar == ';')):
            scribus.deleteText(textbox)
            scribus.insertText(follow_double, c, textbox)
            if (not is_a_space(prevchar)):
                scribus.insertText(space_character, c, textbox)
                c=c+space_len
            quotes_lastchange='close'
        elif (quotes_lastchange!='open'):
            scribus.deleteText(textbox)
            if (not is_a_space(nextchar)):
                scribus.insertText(space_character, c, textbox)
            scribus.insertText(lead_double, c, textbox)
            quotes_lastchange='open'
        else:
            scribus.deleteText(textbox)
            scribus.insertText(follow_double, c, textbox)
            if (not is_a_space(prevchar)):
                scribus.insertText(space_character, c, textbox)
                c=c+space_len
            quotes_lastchange='close'
        nbchange = nbchange+1

    elif (ord(char) == 39):                   # typewriter quote
        if (c==0):                            # au tout début du texte
            scribus.deleteText(textbox)
            scribus.insertText(lead_single, c, textbox)
            nbchange = nbchange+1    
        if ((prevchar == '.') or (prevchar == ',') or (prevchar == '?') or (prevchar == '!')):
            scribus.deleteText(textbox)
            scribus.insertText(follow_single, c, textbox)
        elif ((ord(prevchar) == 34) and ((nextchar != ' ') and (nextchar != ',') and (nextchar != '.'))):
            # " puis ' puis espace, virgule ou point
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

    elif (c>0) and ensure_space_before(char):
        if (not is_in_url):
            is_in_url = ((prevchar=='p') and (char==':') and (nextchar=='/')) or ((prevchar=='w') and (char=='w') and (nextchar=='.'))
                  
        if (not is_in_url) and (prevchar != space_character) and (not ensure_space_before(prevchar)):

            if (is_a_space(prevchar) and replace_existing):
                c -= 1
                scribus.selectText(c, 1, textbox)    
                scribus.deleteText(textbox)        # deletes previous unwanted space
                scribus.selectText(c, 1, textbox)    
                scribus.insertText(space_character, c, textbox)
                c += space_len
                nbchange = nbchange+1
                scribus.setRedraw(1)

            elif (not is_a_space(prevchar)):
                scribus.insertText(space_character, c, textbox)
                c += space_len
                nbchange = nbchange+1
                scribus.setRedraw(1)

    elif is_a_space(char):
      is_in_url = False
      if merge_spaces and is_a_space(nextchar): # only keeps last space of a set
        scribus.deleteText(textbox)
        c -= 1
        nbchange = nbchange+1
        scribus.setRedraw(1)

    c += 1
    prevchar = char
    textlen = scribus.getTextLength(textbox)

debugmessage = ''
scribus.setRedraw(1)
scribus.docChanged(1)
if with_feedback:
  if (lang == 'fr'):
    scribus.messageBox("Fini", 'Les corrections typographiques ont été faites.\n'+str(nbchange)+' changements' + debugmessage,
                        icon=scribus.ICON_NONE, button1=scribus.BUTTON_OK)
  else:
    scribus.messageBox("Done", 'Successfully ran script\n'+str(nbchange)+' replacements have occurred' + debugmessage, # Change this message to your liking
                        icon=scribus.ICON_NONE, button1=scribus.BUTTON_OK)
