<!DOCTYPE TS>
<TS>
  <context>
    <name></name>
    <message>
      <source>getColorNames() -> list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
      <translation>getColorNames() -> list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.</translation>
    </message>
    <message>
      <source>newDocDialog() -> bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
      <translation>getColorNames() -> list

Näyttää dialogin &quot;Uusi asiakirja&quot;. Luo uuden asiakirjan, jos käyttäjä hyväksyy
asetukset. Asiakirjaa ei luoda, jos käyttäjä painaa Peruuta-painiketta.
Palauttaa true, jos uusi asiakirja luotiin.</translation>
    </message>
    <message>
      <source>getFillColor([&quot;name&quot;]) -> string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getFillColor([&quot;name&quot;]) -> string

Palauttaa täyttövärin nimen kohteelle &quot;name&quot;.
Jos &quot;name&quot; ei ole annettu käytetään valittuna olevaa kohdetta.</translation>
    </message>
    <message>
      <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
      <translation>moveObject(dx, dy [, &quot;name&quot;])

Siirtää kohdetta &quot;name&quot; arvoilla dx ja dy  suhteessa nykyiseen sijaintiin. Etäisyydet
ilmaistaan asikirjassa käytössä olevalla yksiköllä (katso UNIT-vakio). Jos &quot;name&quot; ei
ole määritelty, käytetään valittuna olevaa kohdetta. Jos kohde &quot;name&quot; kuuluu 
ryhmään, koko ryhmää siirretään.</translation>
    </message>
    <message>
      <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
      <translation>setRedraw(bool)

Estää sivun uudelleenpiirron, kun bool=False. Muuten uudelleen piirto on 
käytössä. Tämä asetus jää voimaan vaikka skriptistä poistutaan. Varmista,
että kutsut setRedraw(True) finally: -lauseessa ylimmällä tasolla skriptissäsi.</translation>
    </message>
    <message>
      <source>createRect(x, y, width, height, [&quot;name&quot;]) -> string

Creates a new rectangle on the current page and returns its name. The
coordinates are given in the current measurement units of the document
(see UNIT constants). &quot;name&quot; should be a unique identifier for the object
because you need this name to reference that object in future. If &quot;name&quot;
is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
</source>
      <translation>createRect(x, y, width, height, [&quot;name&quot;]) -> string

Luo uuden nelikulmion nykyiselle sivulle ja palauttaa luodun kohteen nimen.
Koordinaatit on annettava asikirjassa käytössä olevalla yksiköllä. Katso (UNIT vakiot).
&quot;name&quot;:n tulee olla yksilöllinen tunniste kohteelle, koska tarvitset nimeä myöhemmin
viitatessasi nelikulmioon. Jos &quot;name&quot;:a ei anneta Scribus luo sen.

Saattaa nostaa NameExistsError:n, jos annettu nimi on jo olemassa.</translation>
    </message>
    <message>
      <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type>).
</source>
      <translation>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Asettaa liukuväritäytön kohteelle &quot;name&quot;. Värin kuvukset ovat samat kuin
setFillColor():lla ja setFillShade():lla. Katso vakiot FILL_&lt;type>.</translation>
    </message>
    <message>
      <source>getFontSize([&quot;name&quot;]) -> float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getFontSize([&quot;name&quot;]) -> float

Palauttaa kirjaintyypin koon pisteinä tekstikehykselle &quot;name&quot;. Jos tekstikehyksessä
on tekstiä valittuna palautetaan valinnan ensimmäisen kirjaintyypin koko.
Jos &quot;name&quot; ei ole määritelty, käytetään valittuna olevaa kohdetta.</translation>
    </message>
    <message>
      <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or 'unicode' string(recommended).
</source>
      <translation>messagebarText(&quot;string&quot;)

Asettaa tekstin &quot;string&quot; Scribuksen tilariville. Teksti täytyy olla UTF8 koodattu tai
'unicode' teksti (suositeltavaa).</translation>
    </message>
    <message>
      <source>newPage(where [,&quot;masterpage&quot;])

Creates a new page. If &quot;where&quot; is -1 the new Page is appended to the
document, otherwise the new page is inserted before &quot;where&quot;. Page numbers are
counted from 1 upwards, no matter what the displayed first page number of your
document is. The optional parameter &quot;masterpage&quot; specifies the name of the
master page for the new page.

May raise IndexError if the page number is out of range
</source>
      <translation>newPage(where [,&quot;masterpage&quot;])

Creates a new page. If &quot;where&quot; is -1 the new Page is appended to the
document, otherwise the new page is inserted before &quot;where&quot;. Page numbers are
counted from 1 upwards, no matter what the displayed first page number of your
document is. The optional parameter &quot;masterpage&quot; specifies the name of the
master page for the new page.

May raise IndexError if the page number is out of range
</translation>
    </message>
    <message>
      <source>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or 'unicode' string(recommended).
</source>
      <translation>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or 'unicode' string(recommended).</translation>
    </message>
    <message>
      <source>newDocument(size, margins, orientation, firstPageNumber,
                        unit, pagesType, firstPageOrder) -> bool

Creates a new document and returns true if successful. The parameters have the
following meaning:

size = A tuple (width, height) describing the size of the document. You can
use predefined constants named PAPER_&lt;paper_type> e.g. PAPER_A4 etc.

margins = A tuple (left, right, top, bottom) describing the document
margins

orientation = the page orientation - constants PORTRAIT, LANDSCAPE

firstPageNumer = is the number of the first page in the document used for
pagenumbering. While you'll usually want 1, it's useful to have higher
numbers if you're creating a document in several parts.

unit: this value sets the measurement units used by the document. Use a
predefined constant for this, one of: UNIT_INCHES, UNIT_MILLIMETERS,
UNIT_PICAS, UNIT_POINTS.

pagesType = One of the predefined constants PAGE_n. PAGE_1 is single page,
PAGE_2 is for double sided documents, PAGE_3 is for 3 pages fold and
PAGE_4 is 4-fold.

firstPageOrder = What is position of first page in the document.
Indexed from 0 (0 = first).

The values for width, height and the margins are expressed in the given unit
for the document. PAPER_* constants are expressed in points. If your document
is not in points, make sure to account for this.

example: newDocument(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 7, UNIT_POINTS,
PAGE_4, 3)

May raise ScribusError if is firstPageOrder bigger than allowed by pagesType.
</source>
      <translation>newDocument(size, margins, orientation, firstPageNumber,
                        unit, pagesType, firstPageOrder) -> bool

Creates a new document and returns true if successful. The parameters have the
following meaning:

size = A tuple (width, height) describing the size of the document. You can
use predefined constants named PAPER_&lt;paper_type> e.g. PAPER_A4 etc.

margins = A tuple (left, right, top, bottom) describing the document
margins

orientation = the page orientation - constants PORTRAIT, LANDSCAPE

firstPageNumer = is the number of the first page in the document used for
pagenumbering. While you'll usually want 1, it's useful to have higher
numbers if you're creating a document in several parts.

unit: this value sets the measurement units used by the document. Use a
predefined constant for this, one of: UNIT_INCHES, UNIT_MILLIMETERS,
UNIT_PICAS, UNIT_POINTS.

pagesType = One of the predefined constants PAGE_n. PAGE_1 is single page,
PAGE_2 is for double sided documents, PAGE_3 is for 3 pages fold and
PAGE_4 is 4-fold.

firstPageOrder = What is position of first page in the document.
Indexed from 0 (0 = first).

The values for width, height and the margins are expressed in the given unit
for the document. PAPER_* constants are expressed in points. If your document
is not in points, make sure to account for this.

example: newDocument(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 7, UNIT_POINTS,
PAGE_4, 3)

May raise ScribusError if is firstPageOrder bigger than allowed by pagesType.
</translation>
    </message>
  </context>
  <context>
    <name>@default</name>
    <message>
      <source>getColor(&quot;name&quot;) -> tuple

Returns a tuple (C, M, Y, K) containing the four color components of the
color &quot;name&quot; from the current document. If no document is open, returns
the value of the named color from the default document colors.

May raise NotFoundError if the named color wasn't found.
May raise ValueError if an invalid color name is specified.
</source>
      <translation>getColor(&quot;name&quot;) -> tuple

Palauttaa tuplen (C, M, Y, K) värille &quot;name&quot; nykyisestä asiakirjasta. Jos asiakirjaa
ei ole avoinna, palauttaa värin oletusväreistä.

Saattaa nostaa NotFoundError:n, jos väriä ei löydy.
Saattaa nostaa ValueError:n, jos värin nimi on viallinen.</translation>
    </message>
    <message>
      <source>changeColor(&quot;name&quot;, c, m, y, k)

Changes the color &quot;name&quot; to the specified CMYK value. The color value is
defined via four components c = Cyan, m = Magenta, y = Yellow and k = Black.
Color components should be in the range from 0 to 255.

May raise NotFoundError if the named color wasn't found.
May raise ValueError if an invalid color name is specified.
</source>
      <translation>changeColor(&quot;name&quot;, c, m, y, k)

Vaihtaa värin &quot;name&quot; vastaamaan väriä c, m, y, k (kaikki määritelty välillä 0, 255).</translation>
    </message>
    <message>
      <source>deleteColor(&quot;name&quot;, &quot;replace&quot;)

Deletes the color &quot;name&quot;. Every occurence of that color is replaced by the
color &quot;replace&quot;. If not specified, &quot;replace&quot; defaults to the color
&quot;None&quot; - transparent.

deleteColor works on the default document colors if there is no document open.
In that case, &quot;replace&quot;, if specified, has no effect.

May raise NotFoundError if a named color wasn't found.
May raise ValueError if an invalid color name is specified.
</source>
      <translation>deleteColor(&quot;name&quot;, &quot;replace&quot;)

Poistaa värin &quot;nimi&quot;, joka korvataan värillä &quot;replace&quot;. Oletuksena &quot;replace&quot; on None (läpinäkyvä).

deleteColors toimii asiakirjassa määritellyillä väreillä, asiakirja on avoinna.

Saattaa nostaa NotFoundError:n, jos nimettyä väriä ei löydy.
Saattaa nostaa ValueError:n, jos värin nimi on viallinen.</translation>
    </message>
    <message>
      <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn't found.
May raise ValueError if an invalid color name is specified.
</source>
      <translation>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Jokainen ilmentymä väristä &quot;name&quot; korvataan värillä &quot;replace&quot;.

Saattaa nostaa NotFoundError:n, jos nimettyä väriä ei löydy.
Saattaa nostaa ValueError:n, jos värin nimi on viallinen.</translation>
    </message>
    <message>
      <source>messageBox(&quot;caption&quot;, &quot;message&quot;,
    icon=ICON_NONE, button1=BUTTON_OK|BUTTONOPT_DEFAULT,
    button2=BUTTON_NONE, button3=BUTTON_NONE) -> integer

Displays a message box with the title &quot;caption&quot;, the message &quot;message&quot;, and
an icon &quot;icon&quot; and up to 3 buttons. By default no icon is used and a single
button, OK, is displayed. Only the caption and message arguments are required,
though setting an icon and appropriate button(s) is strongly
recommended. The message text may contain simple HTML-like markup.

Returns the number of the button the user pressed. Button numbers start
at 1.

For the icon and the button parameters there are predefined constants available
with the same names as in the Qt Documentation. These are the BUTTON_* and
ICON_* constants defined in the module. There are also two extra constants that
can be binary-ORed with button constants:
    BUTTONOPT_DEFAULT   Pressing enter presses this button.
    BUTTONOPT_ESCAPE    Pressing escape presses this button.

Usage examples:
result = messageBox('Script failed',
                    'This script only works when you have a text frame selected.',
                    ICON_ERROR)
result = messageBox('Monkeys!', 'Something went ook! &lt;i>Was it a monkey?&lt;/i>',
                    ICON_WARNING, BUTTON_YES|BUTTONOPT_DEFAULT,
                    BUTTON_NO, BUTTON_IGNORE|BUTTONOPT_ESCAPE)

Defined button and icon constants:
BUTTON_NONE, BUTTON_ABORT, BUTTON_CANCEL, BUTTON_IGNORE, BUTTON_NO,
BUTTON_NOALL, BUTTON_OK, BUTTON_RETRY, BUTTON_YES, BUTTON_YESALL,
ICON_NONE, ICON_INFORMATION, ICON_WARNING, ICON_CRITICAL.
</source>
      <translation>messageBox(&quot;caption&quot;, &quot;message&quot;, 
    icon=ICON_NONE, button1=BUTTON_OK|BUTTONOPT_DEFAULT, 
    button2=BUTTON_NONE, button3=BUTTON_NONE) -> integer 
 
Displays a message box with the title &quot;caption&quot;, the message &quot;message&quot;, and 
an icon &quot;icon&quot; and up to 3 buttons. By default no icon is used and a single 
button, OK, is displayed. Only the caption and message arguments are required, 
though setting an icon and appropriate button(s) is strongly 
recommended. The message text may contain simple HTML-like markup. 
 
Returns the number of the button the user pressed. Button numbers start 
at 1. 
 
For the icon and the button parameters there are predefined constants available 
with the same names as in the Qt Documentation. These are the BUTTON_* and 
ICON_* constants defined in the module. There are also two extra constants that 
can be binary-ORed with button constants: 
    BUTTONOPT_DEFAULT   Pressing enter presses this button. 
    BUTTONOPT_ESCAPE    Pressing escape presses this button. 
 
Usage examples: 
result = messageBox('Script failed', 
                    'This script only works when you have a text frame selected.', 
                    ICON_ERROR) 
result = messageBox('Monkeys!', 'Something went ook! &lt;i>Was it a monkey?&lt;/i>', 
                    ICON_WARNING, BUTTON_YES|BUTTONOPT_DEFAULT, 
                    BUTTON_NO, BUTTON_IGNORE|BUTTONOPT_ESCAPE) 
 
Defined button and icon constants: 
BUTTON_NONE, BUTTON_ABORT, BUTTON_CANCEL, BUTTON_IGNORE, BUTTON_NO, 
BUTTON_NOALL, BUTTON_OK, BUTTON_RETRY, BUTTON_YES, BUTTON_YESALL, 
ICON_NONE, ICON_INFORMATION, ICON_WARNING, ICON_CRITICAL.</translation>
    </message>
    <message>
      <source>valueDialog(caption, message [,defaultvalue]) -> string

Shows the common 'Ask for string' dialog and returns its value as a string
Parameters: window title, text in the window and optional 'default' value.

Example: valueDialog('title', 'text in the window', 'optional')
</source>
      <translation>valueDialog(caption, message [,defaultvalue]) -> string

Näyttää yleisen &quot;Kysy string&quot;-dialogin ja palauttaa sen arvon. Parametrit: ikkunan
otsikko, ikkunan teksti ja oletusarvo</translation>
    </message>
    <message>
      <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
      <translation>closeDoc()

Sulkee aktiivisen asiakirjan, ilman tallennusdialogia.

Saattaa heittää NoDocOpenError, jos suljettavia asiakirjoja ei ole</translation>
    </message>
    <message>
      <source>haveDoc() -> bool

Returns true if there is a document open.
</source>
      <translation>haveDoc() -> bool

Palauttaa true, jos yksikin asiakirja on avoinna.</translation>
    </message>
    <message>
      <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
      <translation>openDoc(&quot;name&quot;)

Avaa asiakirjan &quot;name&quot;.

Saattaa aiheuttaa ScribusErrorin, jos asiakirjaa ei voida avata.</translation>
    </message>
    <message>
      <source>saveDoc()

Saves the current document with its current name, returns true if successful.
If the document has not already been saved, this may bring up an interactive
save file dialog.

If the save fails, there is currently no way to tell.
</source>
      <translation>saveDoc()

Tallennusdialogi. Tallennuksen epäonnistumista ei voi tällä hetkellä päätellä mistään.</translation>
    </message>
    <message>
      <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
      <translation>saveDocAs(&quot;name&quot;)

Tallentaa aktiivisen asiakirjan nimellä &quot;nimi&quot;, joka voi olla täydellinen tai suhteellinen polku.

Saattaa aiheuttaa ScribusError:n, jos tallennus epäonnistuu.</translation>
    </message>
    <message>
      <source>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -> bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
      <translation>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -> bool

Asettaa asiakirjan tiedot. &quot;author&quot; (tekijä), &quot;info&quot; (tietoja) ja &quot;description&quot; (kuvaus) ovat stringejä.</translation>
    </message>
    <message>
      <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type>
constants.
</source>
      <translation>setMargins(lr, rr, tr, br)

Asettaa asiakirjan reunukset Vasen (lr), Oikea (rr), ylä (tr) ja ala (br). Reunukset annetaan
asiakirjan käyttämissä yksiköissä. Katso UNIT_&lt;type> vakiot.</translation>
    </message>
    <message>
      <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type>.

May raise ValueError if an invalid unit is passed.
</source>
      <translation>setUnit(type)

Vaihtaa asiakirjan mittayksikköä. Mahdolliset yksiköt on määritelty vakiona UNIT_&lt;type>.

Saattaa aiheuttaa ValueError:n, jos käytetään virheellistä yksikköä.</translation>
    </message>
    <message>
      <source>getUnit() -> integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
      <translation>getUnit() -> integer (Scribus unit constant)

Palauttaa asiakirjan mittayksikön. Arvo on yksi UNIT_* vakioista.
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</translation>
    </message>
    <message>
      <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
      <translation>loadStylesFromFile(&quot;filename&quot;)

Lataa kappaletyylit Scribus-asiakirjasta &quot;filename&quot; aktiiviseen asiakirjaan.</translation>
    </message>
    <message>
      <source>setDocType(facingPages, firstPageLeft)

Sets the document type. To get facing pages set the first parameter to
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for
a right page use FIRSTPAGERIGHT.
</source>
      <translation>setDocType(facingPages, firstPageLeft) 
 
Sets the document type. To get facing pages set the first parameter to 
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want 
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for 
a right page use FIRSTPAGERIGHT.

</translation>
    </message>
    <message>
      <source>getLineColor([&quot;name&quot;]) -> string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getLineColor([&quot;name&quot;]) -> string

Palauttaa kohteen &quot;name&quot; viivan värin. Ilman &quot;name&quot;:a palauttaa
valittuna olevan kohteen kehyksen värin.</translation>
    </message>
    <message>
      <source>getLineWidth([&quot;name&quot;]) -> integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
      <translation>getLineWidth([&quot;name&quot;]) -> integer

Palauttaa kohteen &quot;name&quot; viivan leveyden. Jos &quot;name&quot;:a
ei anneta käytetään valittuna olevaa kohdetta.</translation>
    </message>
    <message>
      <source>getLineShade([&quot;name&quot;]) -> integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getLineShade([&quot;name&quot;]) -> integer

Palauttaa kohteen &quot;name&quot; viivan värin sävyn. Jos &quot;name&quot;:a ei
anneta käytetään valittuna olevaa kohdetta.</translation>
    </message>
    <message>
      <source>getLineJoin([&quot;name&quot;]) -> integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
      <translation>getLineJoin([&quot;name&quot;]) -> integer (see contants)
 
Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given 
the currently selected item is used.  The join types are: 
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND 
</translation>
    </message>
    <message>
      <source>getLineEnd([&quot;name&quot;]) -> integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
      <translation>getLineEnd([&quot;name&quot;]) -> integer (see constants)
 
Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the 
currently selected item is used. The cap types are: 
CAP_FLAT, CAP_ROUND, CAP_SQUARE </translation>
    </message>
    <message>
      <source>getLineStyle([&quot;name&quot;]) -> integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
      <translation>getLineStyle([&quot;name&quot;]) -> integer (see constants)
 
Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the 
currently selected item is used. Line style constants are: 
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID 
</translation>
    </message>
    <message>
      <source>getFillShade([&quot;name&quot;]) -> integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getFillShade([&quot;name&quot;]) -> integer
 
Returns the shading value of the fill color of the object &quot;name&quot;. 
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>getImageScale([&quot;name&quot;]) -> (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getImageScale([&quot;name&quot;]) -> (x,y)
 
Returns a (x, y) tuple containing the scaling values of the image frame 
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>getImageName([&quot;name&quot;]) -> string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
      <translation>getImageName([&quot;name&quot;]) -> string
 
Returns the filename for the image in the image frame. If &quot;name&quot; is not 
given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>getSize([&quot;name&quot;]) -> (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type>
for reference.
</source>
      <translation>getSize([&quot;name&quot;]) -> (width,height)
 
Returns a (width, height) tuple with the size of the object &quot;name&quot;. 
If &quot;name&quot; is not given the currently selected item is used. The size is 
expressed in the current measurement unit of the document - see UNIT_&lt;type> 
for reference.
</translation>
    </message>
    <message>
      <source>getRotation([&quot;name&quot;]) -> integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</source>
      <translation>getRotation([&quot;name&quot;]) -> integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees, 
and clockwise is positive. If &quot;name&quot; is not given the currently selected item 
is used.</translation>
    </message>
    <message>
      <source>getAllObjects() -> list

Returns a list containing the names of all objects on the current page.
</source>
      <translation>getAllObjects() -> list

Returns a list containing the names of all objects on the current page.</translation>
    </message>
    <message>
      <source>moveObjectAbs(x, y [, &quot;name&quot;])

Moves the object &quot;name&quot; to a new location. The coordinates are expressed in
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot;
is not given the currently selected item is used.  If the object &quot;name&quot;
belongs to a group, the whole group is moved.
</source>
      <translation>moveObjectAbs(x, y [, &quot;name&quot;])
 
Moves the object &quot;name&quot; to a new location. The coordinates are expressed in 
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot; 
is not given the currently selected item is used.  If the object &quot;name&quot; 
belongs to a group, the whole group is moved.
</translation>
    </message>
    <message>
      <source>rotateObject(rot [, &quot;name&quot;])

Rotates the object &quot;name&quot; by &quot;rot&quot; degrees relatively. The object is
rotated by the vertex that is currently selected as the rotation point - by
default, the top left vertext at zero rotation. Positive values mean counter
clockwise rotation when the default rotation point is used. If &quot;name&quot; is not
given the currently selected item is used.
</source>
      <translation>rotateObject(rot [, &quot;name&quot;])
 
Rotates the object &quot;name&quot; by &quot;rot&quot; degrees relatively. The object is 
rotated by the vertex that is currently selected as the rotation point - by 
default, the top left vertext at zero rotation. Positive values mean counter 
clockwise rotation when the default rotation point is used. If &quot;name&quot; is not 
given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</source>
      <translation>sizeObject(width, height [, &quot;name&quot;])
 
Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot; 
is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>getSelectedObject([nr]) -> string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
      <translation>getSelectedObject([nr]) -> string
 
Returns the name of the selected object. &quot;nr&quot; if given indicates the number 
of the selected object, e.g. 0 means the first selected object, 1 means the 
second selected Object and so on.
</translation>
    </message>
    <message>
      <source>selectionCount() -> integer

Returns the number of selected objects.
</source>
      <translation>selectionCount() -> integer
 
Returns the number of selected objects.
</translation>
    </message>
    <message>
      <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
      <translation>selectObject(&quot;name&quot;)
 
Selects the object with the given &quot;name&quot;.
</translation>
    </message>
    <message>
      <source>deselectAll()

Deselects all objects in the whole document.
</source>
      <translation>deselectAll()
 
Deselects all objects in the whole document.
</translation>
    </message>
    <message>
      <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
      <translation>groupObjects(list)
 
Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names 
of the objects to be grouped. If &quot;list&quot; is not given the currently selected 
items are used.
</translation>
    </message>
    <message>
      <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
      <translation>unGroupObjects(&quot;name&quot;)
 
Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>scaleGroup(factor [,&quot;name&quot;])

Scales the group the object &quot;name&quot; belongs to. Values greater than 1 enlarge
the group, values smaller than 1 make the group smaller e.g a value of 0.5
scales the group to 50 % of its original size, a value of 1.5 scales the group
to 150 % of its original size.  The value for &quot;factor&quot; must be greater than
0. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if an invalid scale factor is passed.
</source>
      <translation>scaleGroup(factor [,&quot;name&quot;])
 
Scales the group the object &quot;name&quot; belongs to. Values greater than 1 enlarge 
the group, values smaller than 1 make the group smaller e.g a value of 0.5 
scales the group to 50 % of its original size, a value of 1.5 scales the group 
to 150 % of its original size.  The value for &quot;factor&quot; must be greater than 
0. If &quot;name&quot; is not given the currently selected item is used. 
 
May raise ValueError if an invalid scale factor is passed.
</translation>
    </message>
    <message>
      <source>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is
not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
      <translation>loadImage(&quot;filename&quot; [, &quot;name&quot;])
 
Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is 
not given the currently selected item is used. 
 
May raise WrongFrameTypeError if the target frame is not an image frame
</translation>
    </message>
    <message>
      <source>scaleImage(x, y [, &quot;name&quot;])

Sets the scaling factors of the picture in the image frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. A number of 1
means 100 %.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
      <translation>scaleImage(x, y [, &quot;name&quot;])
 
Sets the scaling factors of the picture in the image frame &quot;name&quot;. 
If &quot;name&quot; is not given the currently selected item is used. A number of 1 
means 100 %. 
 
May raise WrongFrameTypeError if the target frame is not an image frame 
</translation>
    </message>
    <message>
      <source>lockObject([&quot;name&quot;]) -> bool

Locks the object &quot;name&quot; if it's unlocked or unlock it if it's locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
      <translation>lockObject([&quot;name&quot;]) -> bool
 
Locks the object &quot;name&quot; if it's unlocked or unlock it if it's locked. 
If &quot;name&quot; is not given the currently selected item is used. Returns true 
if locked.
</translation>
    </message>
    <message>
      <source>isLocked([&quot;name&quot;]) -> bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
      <translation>isLocked([&quot;name&quot;]) -> bool
 
Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the 
currently selected item is used. 
</translation>
    </message>
    <message>
      <source>getFontNames() -> list

Returns a list with the names of all available fonts.
</source>
      <translation>getFontNames() -> list
 
Returns a list with the names of all available fonts. 
</translation>
    </message>
    <message>
      <source>getXFontNames() -> list of tuples

Returns a larger font info. It's a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
      <translation>getXFontNames() -> list of tuples
 
Returns a larger font info. It's a list of the tuples with: 
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ] 
</translation>
    </message>
    <message>
      <source>getLayers() -> list

Returns a list with the names of all defined layers.
</source>
      <translation>getLayers() -> list

 
Returns a list with the names of all defined layers. </translation>
    </message>
    <message>
      <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>setActiveLayer(&quot;name&quot;)
 
Sets the active layer to the layer named &quot;name&quot;. 
 
May raise NotFoundError if the layer can't be found. 
May raise ValueError if the layer name isn't acceptable. 
</translation>
    </message>
    <message>
      <source>getActiveLayer() -> string

Returns the name of the current active layer.
</source>
      <translation>getActiveLayer() -> string
 
Returns the name of the current active layer. 
</translation>
    </message>
    <message>
      <source>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])
 
Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist. 
If &quot;name&quot; is not given the currently selected item is used. 
 
May raise NotFoundError if the layer can't be found. 
May raise ValueError if the layer name isn't acceptable. 
</translation>
    </message>
    <message>
      <source>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>setLayerVisible(&quot;layer&quot;, visible)
 
Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false 
the layer is invisible. 
 
May raise NotFoundError if the layer can't be found. 
May raise ValueError if the layer name isn't acceptable. 
</translation>
    </message>
    <message>
      <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won't be printed.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>setLayerPrintable(&quot;layer&quot;, printable)
 
Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to 
false the layer won't be printed. 
 
May raise NotFoundError if the layer can't be found. 
May raise ValueError if the layer name isn't acceptable. 
</translation>
    </message>
    <message>
      <source>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn't
exists or if it's the only layer in the document.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>deleteLayer(&quot;layer&quot;)

 
Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn't 
exists or if it's the only layer in the document. 
 
May raise NotFoundError if the layer can't be found. 
May raise ValueError if the layer name isn't acceptable. </translation>
    </message>
    <message>
      <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>createLayer(layer)
 
Creates a new layer with the name &quot;name&quot;. 
 
May raise ValueError if the layer name isn't acceptable. 
</translation>
    </message>
    <message>
      <source>getGuiLanguage() -> string

Returns a string with the -lang value.
</source>
      <translation>getGuiLanguage() -> string
 
Returns a string with the -lang value. 
</translation>
    </message>
    <message>
      <source>createEllipse(x, y, width, height, [&quot;name&quot;]) -> string

Creates a new ellipse on the current page and returns its name.
The coordinates are given in the current measurement units of the document
(see UNIT constants). &quot;name&quot; should be a unique identifier for the object
because you need this name for further referencing of that object. If &quot;name&quot;
is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
</source>
      <translation>createEllipse(x, y, width, height, [&quot;name&quot;]) -> string
 
Creates a new ellipse on the current page and returns its name. 
The coordinates are given in the current measurement units of the document 
(see UNIT constants). &quot;name&quot; should be a unique identifier for the object 
because you need this name for further referencing of that object. If &quot;name&quot; 
is not given Scribus will create one for you. 
 
May raise NameExistsError if you explicitly pass a name that's already used. 
</translation>
    </message>
    <message>
      <source>createImage(x, y, width, height, [&quot;name&quot;]) -> string

Creates a new picture frame on the current page and returns its name. The
coordinates are given in the current measurement units of the document.
&quot;name&quot; should be a unique identifier for the object because you need this
name for further access to that object. If &quot;name&quot; is not given Scribus will
create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
</source>
      <translation>createImage(x, y, width, height, [&quot;name&quot;]) -> string
 
Creates a new picture frame on the current page and returns its name. The 
coordinates are given in the current measurement units of the document. 
&quot;name&quot; should be a unique identifier for the object because you need this 
name for further access to that object. If &quot;name&quot; is not given Scribus will 
create one for you. 
 
May raise NameExistsError if you explicitly pass a name that's already used. 
</translation>
    </message>
    <message>
      <source>createText(x, y, width, height, [&quot;name&quot;]) -> string

Creates a new text frame on the actual page and returns its name.
The coordinates are given in the actual measurement unit of the document (see
UNIT constants). &quot;name&quot; should be a unique identifier for the object because
you need this name for further referencing of that object. If &quot;name&quot; is not
given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
</source>
      <translation>createText(x, y, width, height, [&quot;name&quot;]) -> string
 
Creates a new text frame on the actual page and returns its name. 
The coordinates are given in the actual measurement unit of the document (see 
UNIT constants). &quot;name&quot; should be a unique identifier for the object because 
you need this name for further referencing of that object. If &quot;name&quot; is not 
given Scribus will create one for you. 
 
May raise NameExistsError if you explicitly pass a name that's already used. 
</translation>
    </message>
    <message>
      <source>createLine(x1, y1, x2, y2, [&quot;name&quot;]) -> string

Creates a new line from the point(x1, y1) to the point(x2, y2) and returns
its name. The coordinates are given in the current measurement unit of the
document (see UNIT constants). &quot;name&quot; should be a unique identifier for the
object because you need this name for further access to that object. If
&quot;name&quot; is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
</source>
      <translation>createLine(x1, y1, x2, y2, [&quot;name&quot;]) -> string
 
Creates a new line from the point(x1, y1) to the point(x2, y2) and returns 
its name. The coordinates are given in the current measurement unit of the 
document (see UNIT constants). &quot;name&quot; should be a unique identifier for the 
object because you need this name for further access to that object. If 
&quot;name&quot; is not given Scribus will create one for you. 
 
May raise NameExistsError if you explicitly pass a name that's already used. 
</translation>
    </message>
    <message>
      <source>createPolyLine(list, [&quot;name&quot;]) -> string

Creates a new polyline and returns its name. The points for the polyline are
stored in the list &quot;list&quot; in the following order: [x1, y1, x2, y2...xn. yn].
The coordinates are given in the current measurement units of the document (see
UNIT constants). &quot;name&quot; should be a unique identifier for the object because
you need this name for further access to that object. If &quot;name&quot; is not given
Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
May raise ValueError if an insufficient number of points is passed or if
the number of values passed don't group into points without leftovers.
</source>
      <translation>createPolyLine(list, [&quot;name&quot;]) -> string
 
Creates a new polyline and returns its name. The points for the polyline are 
stored in the list &quot;list&quot; in the following order: [x1, y1, x2, y2...xn. yn]. 
The coordinates are given in the current measurement units of the document (see 
UNIT constants). &quot;name&quot; should be a unique identifier for the object because 
you need this name for further access to that object. If &quot;name&quot; is not given 
Scribus will create one for you. 
 
May raise NameExistsError if you explicitly pass a name that's already used. 
May raise ValueError if an insufficient number of points is passed or if 
the number of values passed don't group into points without leftovers. 
</translation>
    </message>
    <message>
      <source>createPolygon(list, [&quot;name&quot;]) -> string

Creates a new polygon and returns its name. The points for the polygon are
stored in the list &quot;list&quot; in the following order: [x1, y1, x2, y2...xn. yn].
At least three points are required. There is no need to repeat the first point
to close the polygon. The polygon is automatically closed by connecting the
first and the last point.  The coordinates are given in the current measurement
units of the document (see UNIT constants).  &quot;name&quot; should be a unique
identifier for the object because you need this name for further access to that
object. If &quot;name&quot; is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
May raise ValueError if an insufficient number of points is passed or if
the number of values passed don't group into points without leftovers.
</source>
      <translation>createPolygon(list, [&quot;name&quot;]) -> string
 
Creates a new polygon and returns its name. The points for the polygon are 
stored in the list &quot;list&quot; in the following order: [x1, y1, x2, y2...xn. yn]. 
At least three points are required. There is no need to repeat the first point 
to close the polygon. The polygon is automatically closed by connecting the 
first and the last point.  The coordinates are given in the current measurement 
units of the document (see UNIT constants).  &quot;name&quot; should be a unique 
identifier for the object because you need this name for further access to that 
object. If &quot;name&quot; is not given Scribus will create one for you. 
 
May raise NameExistsError if you explicitly pass a name that's already used. 
May raise ValueError if an insufficient number of points is passed or if 
the number of values passed don't group into points without leftovers. 
</translation>
    </message>
    <message>
      <source>createBezierLine(list, [&quot;name&quot;]) -> string

Creates a new bezier curve and returns its name. The points for the bezier
curve are stored in the list &quot;list&quot; in the following order:
[x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn. yn, kxn. kyn]
In the points list, x and y mean the x and y coordinates of the point and kx
and ky meaning the control point for the curve.  The coordinates are given in
the current measurement units of the document (see UNIT constants). &quot;name&quot;
should be a unique identifier for the object because you need this name for
further access to that object. If &quot;name&quot; is not given Scribus will create one
for you.

May raise NameExistsError if you explicitly pass a name that's already used.
May raise ValueError if an insufficient number of points is passed or if
the number of values passed don't group into points without leftovers.
</source>
      <translation>createBezierLine(list, [&quot;name&quot;]) -> string

 
Creates a new bezier curve and returns its name. The points for the bezier 
curve are stored in the list &quot;list&quot; in the following order: 
[x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn. yn, kxn. kyn] 
In the points list, x and y mean the x and y coordinates of the point and kx 
and ky meaning the control point for the curve.  The coordinates are given in 
the current measurement units of the document (see UNIT constants). &quot;name&quot; 
should be a unique identifier for the object because you need this name for 
further access to that object. If &quot;name&quot; is not given Scribus will create one 
for you. 
 
May raise NameExistsError if you explicitly pass a name that's already used. 
May raise ValueError if an insufficient number of points is passed or if 
the number of values passed don't group into points without leftovers. </translation>
    </message>
    <message>
      <source>createPathText(x, y, &quot;textbox&quot;, &quot;beziercurve&quot;, [&quot;name&quot;]) -> string

Creates a new pathText by merging the two objects &quot;textbox&quot; and
&quot;beziercurve&quot; and returns its name. The coordinates are given in the current
measurement unit of the document (see UNIT constants). &quot;name&quot; should be a
unique identifier for the object because you need this name for further access
to that object. If &quot;name&quot; is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that's already used.
May raise NotFoundError if one or both of the named base object don't exist.
</source>
      <translation>createPathText(x, y, &quot;textbox&quot;, &quot;beziercurve&quot;, [&quot;name&quot;]) -> string
 
Creates a new pathText by merging the two objects &quot;textbox&quot; and 
&quot;beziercurve&quot; and returns its name. The coordinates are given in the current 
measurement unit of the document (see UNIT constants). &quot;name&quot; should be a 
unique identifier for the object because you need this name for further access 
to that object. If &quot;name&quot; is not given Scribus will create one for you. 
 
May raise NameExistsError if you explicitly pass a name that's already used. 
May raise NotFoundError if one or both of the named base object don't exist. 
</translation>
    </message>
    <message>
      <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
      <translation>deleteObject([&quot;name&quot;])

 
Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently 
selected item is deleted. </translation>
    </message>
    <message>
      <source>textFlowsAroundFrame(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot;
is not passed, text flow is toggled.
</source>
      <translation>textFlowsAroundFrame(&quot;name&quot; [, state])
 
Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;. 
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot; 
is not passed, text flow is toggled. 
</translation>
    </message>
    <message>
      <source>objectExists([&quot;name&quot;]) -> bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
      <translation>objectExists([&quot;name&quot;]) -> bool
 
Test if an object with specified name really exists in the document. 
The optional parameter is the object name. When no object name is given, 
returns True if there is something selected. 
</translation>
    </message>
    <message>
      <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it's applied on the selected object.
</source>
      <translation>setStyle(&quot;style&quot; [, &quot;name&quot;])
 
Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name 
given, it's applied on the selected object. 
</translation>
    </message>
    <message>
      <source>getAllStyles() -> list

Return a list of the names of all paragraph styles in the current document.
</source>
      <translation>getAllStyles() -> list
 
Return a list of the names of all paragraph styles in the current document. 
</translation>
    </message>
    <message>
      <source>currentPage() -> integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
      <translation>currentPage() -> integer
 
Returns the number of the current working page. Page numbers are counted from 1 
upwards, no matter what the displayed first page number of your document is. 
</translation>
    </message>
    <message>
      <source>redrawAll()

Redraws all pages.
</source>
      <translation>redrawAll()
 
Redraws all pages. 
</translation>
    </message>
    <message>
      <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
      <translation>savePageAsEPS(&quot;name&quot;)
 
Saves the current page as an EPS to the file &quot;name&quot;. 
 
May raise ScribusError if the save failed. 
</translation>
    </message>
    <message>
      <source>deletePage(nr)

Deletes the given page. Does nothing if the document contains only one page.
Page numbers are counted from 1 upwards, no matter what the displayed first
page number is.

May raise IndexError if the page number is out of range
</source>
      <translation>deletePage(nr)
 
Deletes the given page. Does nothing if the document contains only one page. 
Page numbers are counted from 1 upwards, no matter what the displayed first 
page number is. 
 
May raise IndexError if the page number is out of range 
</translation>
    </message>
    <message>
      <source>gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn't (curently) change the page the user's view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</source>
      <translation>gotoPage(nr)
 
Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that 
gotoPage doesn't (curently) change the page the user's view is displaying, it 
just sets the page that script commands will operates on. 
 
May raise IndexError if the page number is out of range. 
</translation>
    </message>
    <message>
      <source>pageCount() -> integer

Returns the number of pages in the document.
</source>
      <translation>pageCount() -> integer
 
Returns the number of pages in the document. 
</translation>
    </message>
    <message>
      <source>getHGuides() -> list

Returns a list containing positions of the horizontal guides. Values are in the
document's current units - see UNIT_&lt;type> constants.
</source>
      <translation>getHGuides() -> list
 
Returns a list containing positions of the horizontal guides. Values are in the 
document's current units - see UNIT_&lt;type> constants. 
</translation>
    </message>
    <message>
      <source>setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type> constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</source>
      <translation>setHGuides(list)
 
Sets horizontal guides. Input parameter must be a list of guide positions 
measured in the current document units - see UNIT_&lt;type> constants. 
 
Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost 
         setHGuides([90,250]) # replace current guides entirely 
</translation>
    </message>
    <message>
      <source>getVGuides()

See getHGuides.
</source>
      <translation>getVGuides()
 
See getHGuides. 
</translation>
    </message>
    <message>
      <source>setVGuides()

See setHGuides.
</source>
      <translation>setVGuides()

 
See setHGuides. </translation>
    </message>
    <message>
      <source>getPageSize() -> tuple

Returns a tuple with page dimensions measured in the document's current units.
See UNIT_&lt;type> constants and getPageMargins()
</source>
      <translation>getPageSize() -> tuple
 
Returns a tuple with page dimensions measured in the document's current units. 
See UNIT_&lt;type> constants and getPageMargins() 
</translation>
    </message>
    <message>
      <source>getPageItems() -> list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [('Text1', 4, 0), ('Image1', 2, 1)]
means that object named 'Text1' is a text frame (type 4) and is the first at
the page...
</source>
      <translation>getPageItems() -> list
 
Returns a list of tuples with items on the current page. The tuple is: 
(name, objectType, order) E.g. [('Text1', 4, 0), ('Image1', 2, 1)] 
means that object named 'Text1' is a text frame (type 4) and is the first at 
the page... 
</translation>
    </message>
    <message>
      <source>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
      <translation>setFillColor(&quot;color&quot;, [&quot;name&quot;])
 
Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot; 
is the name of one of the defined colors. If &quot;name&quot; is not given the 
currently selected item is used. 
</translation>
    </message>
    <message>
      <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
      <translation>setLineColor(&quot;color&quot;, [&quot;name&quot;])
 
Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot; 
is not given the currently selected item is used. 
</translation>
    </message>
    <message>
      <source>setLineWidth(width, [&quot;name&quot;])

Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not
given the currently selected item is used.

May raise ValueError if the line width is out of bounds.
</source>
      <translation>setLineWidth(width, [&quot;name&quot;])
 
Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the 
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not 
given the currently selected item is used. 
 
May raise ValueError if the line width is out of bounds. 
</translation>
    </message>
    <message>
      <source>setLineShade(shade, [&quot;name&quot;])

Sets the shading of the line color of the object &quot;name&quot; to &quot;shade&quot;.
&quot;shade&quot; must be an integer value in the range from 0 (lightest) to 100
(full color intensity). If &quot;name&quot; is not given the currently selected item
is used.

May raise ValueError if the line shade is out of bounds.
</source>
      <translation>setLineShade(shade, [&quot;name&quot;])
 
Sets the shading of the line color of the object &quot;name&quot; to &quot;shade&quot;. 
&quot;shade&quot; must be an integer value in the range from 0 (lightest) to 100 
(full color intensity). If &quot;name&quot; is not given the currently selected item 
is used. 
 
May raise ValueError if the line shade is out of bounds. 
</translation>
    </message>
    <message>
      <source>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type>.
</source>
      <translation>setLineJoin(join, [&quot;name&quot;])
 
Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;. 
If &quot;name&quot; is not given the currently selected item is used. There are 
predefined constants for join - JOIN_&lt;type>. 
</translation>
    </message>
    <message>
      <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type>.
</source>
      <translation>setLineEnd(endtype, [&quot;name&quot;])
 
Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;. 
If &quot;name&quot; is not given the currently selected item is used. There are 
predefined constants for &quot;cap&quot; - CAP_&lt;type>. 
</translation>
    </message>
    <message>
      <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style>.
</source>
      <translation>setLineStyle(style, [&quot;name&quot;])
 
Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot; 
is not given the currently selected item is used. There are predefined 
constants for &quot;style&quot; - LINE_&lt;style>. 
</translation>
    </message>
    <message>
      <source>setFillShade(shade, [&quot;name&quot;])

Sets the shading of the fill color of the object &quot;name&quot; to &quot;shade&quot;.
&quot;shade&quot; must be an integer value in the range from 0 (lightest) to 100
(full Color intensity). If &quot;name&quot; is not given the currently selected
Item is used.

May raise ValueError if the fill shade is out of bounds.
</source>
      <translation>setFillShade(shade, [&quot;name&quot;])
 
Sets the shading of the fill color of the object &quot;name&quot; to &quot;shade&quot;. 
&quot;shade&quot; must be an integer value in the range from 0 (lightest) to 100 
(full Color intensity). If &quot;name&quot; is not given the currently selected 
Item is used. 
 
May raise ValueError if the fill shade is out of bounds. 
</translation>
    </message>
    <message>
      <source>setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</source>
      <translation>setCornerRadius(radius, [&quot;name&quot;])

 
Sets the corner radius of the object &quot;name&quot;. The radius is expressed 
in points. If &quot;name&quot; is not given the currently selected item is used. 
 
May raise ValueError if the corner radius is negative. </translation>
    </message>
    <message>
      <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn't exist.
</source>
      <translation>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])
 
Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;. 
If &quot;name&quot; is not given the currently selected item is used. 
 
May raise NotFoundError if the line style doesn't exist. 
</translation>
    </message>
    <message>
      <source>getFont([&quot;name&quot;]) -> string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation>getFont([&quot;name&quot;]) -> string
 
Returns the font name for the text frame &quot;name&quot;. If this text frame 
has some text selected the value assigned to the first character 
of the selection is returned. If &quot;name&quot; is not given the currently 
selected item is used. 
</translation>
    </message>
    <message>
      <source>getTextLength([&quot;name&quot;]) -> integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getTextLength([&quot;name&quot;]) -> integer
 
Returns the length of the text in the text frame &quot;name&quot;. 
If &quot;name&quot; is not given the currently selected item is used. 
</translation>
    </message>
    <message>
      <source>getText([&quot;name&quot;]) -> string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation>getText([&quot;name&quot;]) -> string
 
Returns the text of the text frame &quot;name&quot;. If this text frame has some text 
selected, the selected text is returned. All text in the frame, not just 
currently visible text, is returned. If &quot;name&quot; is not given the currently 
selected item is used. 
</translation>
    </message>
    <message>
      <source>getAllText([&quot;name&quot;]) -> string

Returns the text of the text frame &quot;name&quot; and of all text frames which are
linked with this frame. If this textframe has some text selected, the selected
text is returned. If &quot;name&quot; is not given the currently selected item is
used.
</source>
      <translation>getAllText([&quot;name&quot;]) -> string

 
Returns the text of the text frame &quot;name&quot; and of all text frames which are 
linked with this frame. If this textframe has some text selected, the selected 
text is returned. If &quot;name&quot; is not given the currently selected item is 
used. </translation>
    </message>
    <message>
      <source>getLineSpacing([&quot;name&quot;]) -> float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getLineSpacing([&quot;name&quot;]) -> float
 
Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in 
points. If &quot;name&quot; is not given the currently selected item is used. 
</translation>
    </message>
    <message>
      <source>getColumnGap([&quot;name&quot;]) -> float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getColumnGap([&quot;name&quot;]) -> float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If 
&quot;name&quot; is not given the currently selected item is used. </translation>
    </message>
    <message>
      <source>getColumns([&quot;name&quot;]) -> integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
      <translation>getColumns([&quot;name&quot;]) -> integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not 
given the currently selected item is used. </translation>
    </message>
    <message>
      <source>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, 'iso-8859-2'). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</source>
      <translation>setText(&quot;text&quot;, [&quot;name&quot;])
 
Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;. 
Text must be UTF8 encoded - use e.g. unicode(text, 'iso-8859-2'). See the FAQ 
for more details. If &quot;name&quot; is not given the currently selected item is 
used. 
</translation>
    </message>
    <message>
      <source>setFont(&quot;font&quot;, [&quot;name&quot;])

Sets the font of the text frame &quot;name&quot; to &quot;font&quot;. If there is some text
selected only the selected text is changed.  If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError if the font cannot be found.
</source>
      <translation>setFont(&quot;font&quot;, [&quot;name&quot;])
 
Sets the font of the text frame &quot;name&quot; to &quot;font&quot;. If there is some text 
selected only the selected text is changed.  If &quot;name&quot; is not given the 
currently selected item is used. 
 
May throw ValueError if the font cannot be found. </translation>
    </message>
    <message>
      <source>setFontSize(size, [&quot;name&quot;])

Sets the font size of the text frame &quot;name&quot; to &quot;size&quot;. &quot;size&quot; is treated
as a value in points. If there is some text selected only the selected text is
changed. &quot;size&quot; must be in the range 1 to 512. If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError for a font size that's out of bounds.
</source>
      <translation>setFontSize(size, [&quot;name&quot;])
 
Sets the font size of the text frame &quot;name&quot; to &quot;size&quot;. &quot;size&quot; is treated 
as a value in points. If there is some text selected only the selected text is 
changed. &quot;size&quot; must be in the range 1 to 512. If &quot;name&quot; is not given the 
currently selected item is used. 
 
May throw ValueError for a font size that's out of bounds. e(size, [&quot;name&quot;]).</translation>
    </message>
    <message>
      <source>setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</source>
      <translation>setLineSpacing(size, [&quot;name&quot;])
 
Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;. 
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected 
item is used. 
 
May throw ValueError if the line spacing is out of bounds. </translation>
    </message>
    <message>
      <source>setColumnGap(size, [&quot;name&quot;])

Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If
&quot;name&quot; is not given the currently selected item is used.

May throw ValueError if the column gap is out of bounds (must be positive).
</source>
      <translation>setColumnGap(size, [&quot;name&quot;])
 
Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If 
&quot;name&quot; is not given the currently selected item is used. 
 
May throw ValueError if the column gap is out of bounds (must be positive). </translation>
    </message>
    <message>
      <source>setColumns(nr, [&quot;name&quot;])

Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May throw ValueError if number of columns is not at least one.
</source>
      <translation>setColumns(nr, [&quot;name&quot;])
 
Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;. 
If &quot;name&quot; is not given the currently selected item is used. 
 
May throw ValueError if number of columns is not at least one. </translation>
    </message>
    <message>
      <source>setTextAlignment(align, [&quot;name&quot;])

Sets the text alignment of the text frame &quot;name&quot; to the specified alignment.
If &quot;name&quot; is not given the currently selected item is used. &quot;align&quot; should
be one of the ALIGN_ constants defined in this module - see dir(scribus).

May throw ValueError for an invalid alignment constant.
</source>
      <translation>setTextAlignment(align, [&quot;name&quot;])
 
Sets the text alignment of the text frame &quot;name&quot; to the specified alignment. 
If &quot;name&quot; is not given the currently selected item is used. &quot;align&quot; should 
be one of the ALIGN_ constants defined in this module - see dir(scribus). 
 
May throw ValueError for an invalid alignment constant. </translation>
    </message>
    <message>
      <source>deleteText([&quot;name&quot;])

Deletes any text in the text frame &quot;name&quot;. If there is some text selected,
only the selected text will be deleted. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation>deleteText([&quot;name&quot;])

 
Deletes any text in the text frame &quot;name&quot;. If there is some text selected, 
only the selected text will be deleted. If &quot;name&quot; is not given the currently 
selected item is used. </translation>
    </message>
    <message>
      <source>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there
is some text selected only the selected text is changed. If &quot;name&quot; is not
given the currently selected item is used.
</source>
      <translation>setTextColor(&quot;color&quot;, [&quot;name&quot;])
 
Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there 
is some text selected only the selected text is changed. If &quot;name&quot; is not 
given the currently selected item is used. 
</translation>
    </message>
    <message>
      <source>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation>setTextStroke(&quot;color&quot;, [&quot;name&quot;])
 
Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently 
selected item is used. 
</translation>
    </message>
    <message>
      <source>setTextShade(shade, [&quot;name&quot;])

Sets the shading of the text color of the object &quot;name&quot; to &quot;shade&quot;. If
there is some text selected only the selected text is changed. &quot;shade&quot; must
be an integer value in the range from 0 (lightest) to 100 (full color
intensity). If &quot;name&quot; is not given the currently selected item is
used.
</source>
      <translation>setTextShade(shade, [&quot;name&quot;])
 
Sets the shading of the text color of the object &quot;name&quot; to &quot;shade&quot;. If 
there is some text selected only the selected text is changed. &quot;shade&quot; must 
be an integer value in the range from 0 (lightest) to 100 (full color 
intensity). If &quot;name&quot; is not given the currently selected item is 
used. 
</translation>
    </message>
    <message>
      <source>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Link two text frames. The frame named &quot;fromname&quot; is linked to the
frame named &quot;toname&quot;. The target frame must be an empty text frame
and must not link to or be linked from any other frames already.

May throw ScribusException if linking rules are violated.
</source>
      <translation>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)
 
Link two text frames. The frame named &quot;fromname&quot; is linked to the 
frame named &quot;toname&quot;. The target frame must be an empty text frame 
and must not link to or be linked from any other frames already. 
 
May throw ScribusException if linking rules are violated. 
</translation>
    </message>
    <message>
      <source>unlinkTextFrames(&quot;name&quot;)

Remove the specified (named) object from the text frame flow/linkage. If the
frame was in the middle of a chain, the previous and next frames will be
connected, eg 'a->b->c' becomes 'a->c' when you unlinkTextFrames(b)'

May throw ScribusException if linking rules are violated.
</source>
      <translation>unlinkTextFrames(&quot;name&quot;)
 
Remove the specified (named) object from the text frame flow/linkage. If the 
frame was in the middle of a chain, the previous and next frames will be 
connected, eg 'a->b->c' becomes 'a->c' when you unlinkTextFrames(b)' 
 
May throw ScribusException if linking rules are violated. 
</translation>
    </message>
    <message>
      <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
      <translation>traceText([&quot;name&quot;])

Muuta tekstikehys &quot;name&quot; ääriviivoiksi. Jos nimeä ei anneta, käytetään
valittuna olevaa kohdetta.</translation>
    </message>
    <message>
      <source>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</source>
      <translation>progressReset()

Tyhjentää etenemispalkin (progress bar). Katso progressSet.</translation>
    </message>
    <message>
      <source>progressTotal(max)

Sets the progress bar's maximum steps value to the specified number.
See progressSet.
</source>
      <translation>progressTotal(max)

Asettaa etenemispalkin suurimman mahdollisen arvon. Katso progressSet.</translation>
    </message>
    <message>
      <source>progressSet(nr)

Set the progress bar position to &quot;nr&quot;, a value relative to the previously set
progressTotal. The progress bar uses the concept of steps; you give it the
total number of steps and the number of steps completed so far and it will
display the percentage of steps that have been completed. You can specify the
total number of steps with progressTotal(). The current number of steps is set
with progressSet(). The progress bar can be rewound to the beginning with
progressReset(). [based on info taken from Trolltech's Qt docs]
</source>
      <translation>progressSet(nr)

Aseta etenemispalkki nr:n osoittamaan tilaan.
The progress bar uses the concept of steps; you give it the
total number of steps and the number of steps completed so far and it will
display the percentage of steps that have been completed. You can specify the
total number of steps with progressTotal(). The current number of steps is set
with progressSet(). The progress bar can be rewound to the beginning with
progressReset(). [based on info taken from Trolltech's Qt docs]</translation>
    </message>
    <message>
      <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
      <translation>setCursor()

[Ei tuettu!] Tämä saattaa rikkoa asoita. Eli älä käytä ainakaan vielä.</translation>
    </message>
    <message>
      <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It's
useful to call this procedure when you're changing the document, because Scribus
won't automatically notice when you change the document using a script.
</source>
      <translation>docChanged(bool)

Ota käyttöön/Poista käytöstä tallennuskuvake työkalurivillä ja Tiedosto-valikossa.
On hyvä kutsua tätä funktiota, kun olet muokannut asiakirjaa. Scribus ei huomaa 
automaattisesti skriptin tekemiä muutoksia.</translation>
    </message>
    <message>
      <source>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection>)

Sets the scale to frame on the selected or specified image frame to `scaletoframe'.
If `proportional' is specified, set fixed aspect ratio scaling to `proportional'.
Both `scaletoframe' and `proportional' are boolean.

May raise WrongFrameTypeError.
</source>
      <translation>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection>) 
 
Sets the scale to frame on the selected or specified image frame to `scaletoframe'. 
If `proportional' is specified, set fixed aspect ratio scaling to `proportional'. 
Both `scaletoframe' and `proportional' are boolean. 
 
May raise WrongFrameTypeError. </translation>
    </message>
    <message>
      <source>renderFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size, format=&quot;PPM&quot;) -> bool

Creates an image preview of font &quot;name&quot; with given text &quot;sample&quot; and size.
If &quot;filename&quot; is not &quot;&quot;, image is saved into &quot;filename&quot;. Otherwise
image data is returned as a string. The optional &quot;format&quot; argument
specifies the image format to generate, and supports any format allowed
by QPixmap.save(). Common formats are PPM, JPEG, PNG and XPM.

May raise NotFoundError if the specified font can't be found.
May raise ValueError if an empty sample or filename is passed.
</source>
      <translation>renderFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size, format=&quot;PPM&quot;) -> bool 
 
Creates an image preview of font &quot;name&quot; with given text &quot;sample&quot; and size. 
If &quot;filename&quot; is not &quot;&quot;, image is saved into &quot;filename&quot;. Otherwise 
image data is returned as a string. The optional &quot;format&quot; argument 
specifies the image format to generate, and supports any format allowed 
by QPixmap.save(). Common formats are PPM, JPEG, PNG and XPM. 
 
May raise NotFoundError if the specified font can't be found. 
May raise ValueError if an empty sample or filename is passed. </translation>
    </message>
    <message>
      <source>isLayerPrintable(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is printable or not, a value of True means
that the layer &quot;layer&quot; can be printed, a value of False means that printing
the layer &quot;layer&quot; is disabled.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>isLayerPrintable(&quot;layer&quot;) -> bool 
 
Returns whether the layer &quot;layer&quot; is printable or not, a value of True means 
that the layer &quot;layer&quot; can be printed, a value of False means that printing 
the layer &quot;layer&quot; is disabled. 
 
May raise NotFoundError if the layer can't be found. 
May raise ValueError if the layer name isn't acceptable. </translation>
    </message>
    <message>
      <source>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets wether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
      <translation>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;]) 
 
Sets wether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not. 
If &quot;name&quot; is not given the currently selected item is used. 
 
May raise WrongFrameTypeError if the target frame is not a text frame </translation>
    </message>
    <message>
      <source>isPDFBookmark([&quot;name&quot;]) -> bool

Returns true if the text frame &quot;name&quot; is a PDF bookmark.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
      <translation>isPDFBookmark([&quot;name&quot;]) -> bool 
 
Returns true if the text frame &quot;name&quot; is a PDF bookmark. 
If &quot;name&quot; is not given the currently selected item is used. 
 
May raise WrongFrameTypeError if the target frame is not a text frame </translation>
    </message>
    <message>
      <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type> constants and getPageSize().
</source>
      <translation>getPageMargins() 
 
Returns the page margins as a (top, left, right, bottom) tuple in the current 
units. See UNIT_&lt;type> constants and getPageSize(). </translation>
    </message>
    <message>
      <source>getColorAsRGB(&quot;name&quot;) -> tuple

Returns a tuple (R,G,B) containing the three color components of the
color &quot;name&quot; from the current document, converted to the RGB color
space. If no document is open, returns the value of the named color
from the default document colors.

May raise NotFoundError if the named color wasn't found.
May raise ValueError if an invalid color name is specified.
</source>
      <translation>getColorAsRGB(&quot;name&quot;) -> tuple

Returns a tuple (R,G,B) containing the three color components of the
color &quot;name&quot; from the current document, converted to the RGB color
space. If no document is open, returns the value of the named color
from the default document colors.

May raise NotFoundError if the named color wasn't found.
May raise ValueError if an invalid color name is specified.
</translation>
    </message>
    <message>
      <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yello and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
      <translation>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yello and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.</translation>
    </message>
    <message>
      <source>fileDialog(&quot;caption&quot;, [&quot;filter&quot;, &quot;defaultname&quot;, haspreview, issave, isdir]) -> string with filename

Shows a File Open dialog box with the caption &quot;caption&quot;. Files are filtered
with the filter string &quot;filter&quot;. A default filename or file path can also
supplied, leave this string empty when you don't want to use it.  A value of
True for haspreview enables a small preview widget in the FileSelect box.  When
the issave parameter is set to True the dialog acts like a &quot;Save As&quot; dialog
otherwise it acts like a &quot;File Open Dialog&quot;. When the isdir parameter is True
the dialog shows and returns only directories. The default for all of the
opional parameters is False.

The filter, if specified, takes the form 'comment (*.type *.type2 ...)'.
For example 'Images (*.png *.xpm *.jpg)'.

Refer to the Qt-Documentation for QFileDialog for details on filters.

Example: fileDialog('Open input', 'CSV files (*.csv)')
Example: fileDialog('Save report', defaultname='report.txt', issave=True)
</source>
      <translation>fileDialog(&quot;caption&quot;, [&quot;filter&quot;, &quot;defaultname&quot;, haspreview, issave, isdir]) -> string with filename

Shows a File Open dialog box with the caption &quot;caption&quot;. Files are filtered
with the filter string &quot;filter&quot;. A default filename or file path can also
supplied, leave this string empty when you don't want to use it.  A value of
True for haspreview enables a small preview widget in the FileSelect box.  When
the issave parameter is set to True the dialog acts like a &quot;Save As&quot; dialog
otherwise it acts like a &quot;File Open Dialog&quot;. When the isdir parameter is True
the dialog shows and returns only directories. The default for all of the
opional parameters is False.

The filter, if specified, takes the form 'comment (*.type *.type2 ...)'.
For example 'Images (*.png *.xpm *.jpg)'.

Refer to the Qt-Documentation for QFileDialog for details on filters.

Example: fileDialog('Open input', 'CSV files (*.csv)')
Example: fileDialog('Save report', defaultname='report.txt', issave=True)
</translation>
    </message>
    <message>
      <source>getCornerRadius([&quot;name&quot;]) -> integer

Returns the corner radius of the object &quot;name&quot;. The radius isexpressed in points. If &quot;name&quot; is not given the currentlyselected item is used.
</source>
      <translation>getCornerRadius([&quot;name&quot;]) -> integer

Returns the corner radius of the object &quot;name&quot;. The radius isexpressed in points. If &quot;name&quot; is not given the currentlyselected item is used.</translation>
    </message>
    <message>
      <source>getPosition([&quot;name&quot;]) -> (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type> for reference.
</source>
      <translation>getPosition([&quot;name&quot;]) -> (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type> for reference.</translation>
    </message>
    <message>
      <source>getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property' of `object'. See getProperty()
for details of arguments.

If `includesuper' is true, search inherited properties too.
</source>
      <translation>getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property' of `object'. See getProperty()
for details of arguments.

If `includesuper' is true, search inherited properties too.</translation>
    </message>
    <message>
      <source>getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object'.
If `includesuper' is true, return properties supported
by parent classes as well.
</source>
      <translation>getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object'.
If `includesuper' is true, return properties supported
by parent classes as well.</translation>
    </message>
    <message>
      <source>getProperty(object, property)

Return the value of the property `property' of the passed `object'.

The `object' argument may be a string, in which case the named PageItem
is searched for. It may also be a PyCObject, which may point to any
C++ QObject instance.

The `property' argument must be a string, and is the name of the property
to look up on `object'.

The return value varies depending on the type of the property.
</source>
      <translation>getProperty(object, property)

Return the value of the property `property' of the passed `object'.

The `object' argument may be a string, in which case the named PageItem
is searched for. It may also be a PyCObject, which may point to any
C++ QObject instance.

The `property' argument must be a string, and is the name of the property
to look up on `object'.

The return value varies depending on the type of the property.
</translation>
    </message>
    <message>
      <source>setProperty(object, property, value)

Set `property' of `object' to `value'. If `value' cannot be converted to a type
compatible with the type of `property', an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.
</source>
      <translation>setProperty(object, property, value)

Set `property' of `object' to `value'. If `value' cannot be converted to a type
compatible with the type of `property', an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.
</translation>
    </message>
    <message>
      <source>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object', possibly restricted to children
of class named `ofclass' or children named `ofname'. If `recursive' is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.
</source>
      <translation>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object', possibly restricted to children
of class named `ofclass' or children named `ofname'. If `recursive' is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.
</translation>
    </message>
    <message>
      <source>getChild(object, childname, ofclass=None, recursive=True)

Return the first child of `object' named `childname', possibly restricting
the search to children of type name `ofclass'. If `recursive' is true,
search recursively through children, grandchildren, etc.
</source>
      <translation>getChild(object, childname, ofclass=None, recursive=True)

Return the first child of `object' named `childname', possibly restricting
the search to children of type name `ofclass'. If `recursive' is true,
search recursively through children, grandchildren, etc.
</translation>
    </message>
    <message>
      <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positve values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positve values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</translation>
    </message>
    <message>
      <source>isLayerPrintable(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>isLayerPrintable(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference) The first character has an
index of 0. Inserting at position -1 appends text to the frame. If &quot;name&quot; is
not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</source>
      <translation>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference) The first character has an
index of 0. Inserting at position -1 appends text to the frame. If &quot;name&quot; is
not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</translation>
    </message>
    <message>
      <source>selectText(start, count, [&quot;name&quot;])

Selects &quot;count&quot; characters of text in the text frame &quot;name&quot; starting from the
character &quot;start&quot;. Character counting starts at 0. If &quot;count&quot; is zero, any
text selection will be cleared.  If &quot;name&quot; is not given the currently
selected item is used.

May throw IndexError if the selection is outside the bounds of the text.
</source>
      <translation>selectText(start, count, [&quot;name&quot;])

Selects &quot;count&quot; characters of text in the text frame &quot;name&quot; starting from the
character &quot;start&quot;. Character counting starts at 0. If &quot;count&quot; is zero, any
text selection will be cleared.  If &quot;name&quot; is not given the currently
selected item is used.

May throw IndexError if the selection is outside the bounds of the text.
</translation>
    </message>
    <message>
      <source>textOverflows([&quot;name&quot;, nolinks]) -> integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn't
use text frame linking. Without this parameter it search all linking chain.

May raise WrongFrameTypeError if the target frame is not an text frame
</source>
      <translation>textOverflows([&quot;name&quot;, nolinks]) -> integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn't
use text frame linking. Without this parameter it search all linking chain.

May raise WrongFrameTypeError if the target frame is not an text frame
</translation>
    </message>
    <message>
      <source>newStyleDialog() -> string

Shows 'Create new paragraph style' dialog. Function returns real
style name or None when user cancels the dialog.
</source>
      <translation>newStyleDialog() -> string

Shows 'Create new paragraph style' dialog. Function returns real
style name or None when user cancels the dialog.
</translation>
    </message>
    <message>
      <source>newDoc(size, margins, orientation, firstPageNumber,
                   unit, facingPages, firstSideLeft) -> bool

WARNING: Obsolete procedure! Use newDocument instead.

Creates a new document and returns true if successful. The parameters have the
following meaning:

    size = A tuple (width, height) describing the size of the document. You can
    use predefined constants named PAPER_&lt;paper_type> e.g. PAPER_A4 etc.

    margins = A tuple (left, right, top, bottom) describing the document
    margins

    orientation = the page orientation - constants PORTRAIT, LANDSCAPE

    firstPageNumer = is the number of the first page in the document used for
    pagenumbering. While you'll usually want 1, it's useful to have higher
    numbers if you're creating a document in several parts.

    unit: this value sets the measurement units used by the document. Use a
    predefined constant for this, one of: UNIT_INCHES, UNIT_MILLIMETERS,
    UNIT_PICAS, UNIT_POINTS.

    facingPages = FACINGPAGES, NOFACINGPAGES

    firstSideLeft = FIRSTPAGELEFT, FIRSTPAGERIGHT

The values for width, height and the margins are expressed in the given unit
for the document. PAPER_* constants are expressed in points. If your document
is not in points, make sure to account for this.

example: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS,
                FACINGPAGES, FIRSTPAGERIGHT)
</source>
      <translation>newDoc(size, margins, orientation, firstPageNumber,
                   unit, facingPages, firstSideLeft) -> bool

WARNING: Obsolete procedure! Use newDocument instead.

Creates a new document and returns true if successful. The parameters have the
following meaning:

    size = A tuple (width, height) describing the size of the document. You can
    use predefined constants named PAPER_&lt;paper_type> e.g. PAPER_A4 etc.

    margins = A tuple (left, right, top, bottom) describing the document
    margins

    orientation = the page orientation - constants PORTRAIT, LANDSCAPE

    firstPageNumer = is the number of the first page in the document used for
    pagenumbering. While you'll usually want 1, it's useful to have higher
    numbers if you're creating a document in several parts.

    unit: this value sets the measurement units used by the document. Use a
    predefined constant for this, one of: UNIT_INCHES, UNIT_MILLIMETERS,
    UNIT_PICAS, UNIT_POINTS.

    facingPages = FACINGPAGES, NOFACINGPAGES

    firstSideLeft = FIRSTPAGELEFT, FIRSTPAGERIGHT

The values for width, height and the margins are expressed in the given unit
for the document. PAPER_* constants are expressed in points. If your document
is not in points, make sure to account for this.

example: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS,
                FACINGPAGES, FIRSTPAGERIGHT)
</translation>
    </message>
    <message>
      <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
      <translation>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</translation>
    </message>
    <message>
      <source>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</source>
      <translation>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</translation>
    </message>
    <message>
      <source>masterPageNames()

Returns a list of the names of all master pages in the document.
</source>
      <translation>masterPageNames()

Returns a list of the names of all master pages in the document.
</translation>
    </message>
    <message>
      <source>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</source>
      <translation>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</translation>
    </message>
    <message>
      <source>createMasterPage(pageName)

Creates a new master page named pageName and opens it for
editing.
</source>
      <translation>createMasterPage(pageName)

Creates a new master page named pageName and opens it for
editing.
</translation>
    </message>
    <message>
      <source>deleteMasterPage(pageName)

Delete the named master page.
</source>
      <translation>deleteMasterPage(pageName)

Delete the named master page.
</translation>
    </message>
  </context>
  <context>
    <name>About</name>
    <message>
      <source>Contributions from:</source>
      <translation>Työhön osallistuneita:</translation>
    </message>
    <message>
      <source>&amp;About</source>
      <translation>T&amp;ietoja</translation>
    </message>
    <message>
      <source>A&amp;uthors</source>
      <translation>&amp;Tekijät</translation>
    </message>
    <message>
      <source>&amp;Translations</source>
      <translation>&amp;Käännökset</translation>
    </message>
    <message>
      <source>&amp;Online</source>
      <translation>&amp;Verkossa</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Sulje</translation>
    </message>
    <message>
      <source>Development Team:</source>
      <translation>Kehitysryhmä:</translation>
    </message>
    <message>
      <source>Official Documentation:</source>
      <translation>Virallinen dokumentaatio:</translation>
    </message>
    <message>
      <source>Other Documentation:</source>
      <translation>Muu dokumentaatio:</translation>
    </message>
    <message>
      <source>Homepage</source>
      <translation>Kotisivu</translation>
    </message>
    <message>
      <source>Online Reference</source>
      <translation>Materiaali verkossa</translation>
    </message>
    <message>
      <source>Bugs and Feature Requests</source>
      <translation>Bugit ja parannusehdotukset</translation>
    </message>
    <message>
      <source>Mailing List</source>
      <translation>Postituslista</translation>
    </message>
    <message>
      <source>Official Translations and Translators:</source>
      <translation>Käännökset ja kääntäjät:</translation>
    </message>
    <message>
      <source>Previous Translation Contributors:</source>
      <translation>Aikaisempia kääntäjiä:</translation>
    </message>
    <message>
      <source>About Scribus %1</source>
      <translation>Tietoja Scribuksesta %1</translation>
    </message>
    <message>
      <source>Wiki</source>
      <translation>Wiki</translation>
    </message>
    <message>
      <source>%1 %2 %3</source>
      <translation>%1 %2 %3</translation>
    </message>
    <message>
      <source>%3-%2-%1 %4 %5</source>
      <translation>%3-%2-%1 %4 %5</translation>
    </message>
    <message>
      <source>Using Ghostscript version %1</source>
      <translation>Käytetään Ghostscriptin versiota %1</translation>
    </message>
    <message>
      <source>No Ghostscript version available</source>
      <translation>Ghostscriptiä ei löytynyt</translation>
    </message>
    <message>
      <source>&lt;b>Scribus Version %1&lt;/b>&lt;p>%2&lt;br/>%3 %4&lt;br/>%5&lt;/p></source>
      <translation>&lt;b>Scribus versio %1&lt;/b>&lt;p>%2&lt;br/>%3 %4&lt;br/>%5&lt;/p></translation>
    </message>
    <message>
      <source>Build ID:</source>
      <translation>Käännöstunnus</translation>
    </message>
    <message>
      <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
      <translation>Tämä osa kertoo version, käännöspäivämäärän sekä mukaan käännetyt kirjastot. C-C-T-F-C/A muodostuu seuraavasti: C=LittleCMS - C=CUPS - T=TIFF - F=Fontconfig.- C=Cairo/A=libart. Kirjaston puuttuessa on sitä osoittavan kirjaimen tilalla merkki *. Scribuksen löytämä Ghostscript ja sen versio ovat myös esillä.</translation>
    </message>
    <message>
      <source>January</source>
      <translation>tammikuu</translation>
    </message>
    <message>
      <source>Mac OS&amp;#174; X Aqua Port:</source>
      <translation>Mac OS&amp;#174; X Aqua -versio:</translation>
    </message>
    <message>
      <source>Windows&amp;#174; Port:</source>
      <translation>Windows&amp;#174; -versio:</translation>
    </message>
  </context>
  <context>
    <name>AboutPlugins</name>
    <message>
      <source>Yes</source>
      <translation>Kyllä</translation>
    </message>
    <message>
      <source>No</source>
      <translation>Ei</translation>
    </message>
    <message>
      <source>Filename:</source>
      <translation>Tiedostonimi:</translation>
    </message>
    <message>
      <source>Version:</source>
      <translation>Versio:</translation>
    </message>
    <message>
      <source>Enabled:</source>
      <translation>Käytössä:</translation>
    </message>
    <message>
      <source>Release Date:</source>
      <translation>Julkaisupäivämäärä:</translation>
    </message>
    <message>
      <source>Description:</source>
      <translation>Kuvaus:</translation>
    </message>
    <message>
      <source>Author(s):</source>
      <translation>Tekijät:</translation>
    </message>
    <message>
      <source>Copyright:</source>
      <translation>Copyright:</translation>
    </message>
    <message>
      <source>License:</source>
      <translation>Lisenssi:</translation>
    </message>
  </context>
  <context>
    <name>AboutPluginsBase</name>
    <message>
      <source>Scribus: About Plug-ins</source>
      <translation>Scribus: Tietoja liitännäisistä</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Sulje</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
  </context>
  <context>
    <name>ActionManager</name>
    <message>
      <source>&amp;New</source>
      <translation>&amp;Uusi</translation>
    </message>
    <message>
      <source>&amp;Open...</source>
      <translation>&amp;Avaa...</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Sulje</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Tallenna</translation>
    </message>
    <message>
      <source>Save &amp;As...</source>
      <translation>Tallenna &amp;nimellä...</translation>
    </message>
    <message>
      <source>Re&amp;vert to Saved</source>
      <translation>&amp;Palauta tallennettu</translation>
    </message>
    <message>
      <source>Collect for O&amp;utput...</source>
      <translation>&amp;Kerää asiakirja...</translation>
    </message>
    <message>
      <source>Get Text...</source>
      <translation>Hae teksti...</translation>
    </message>
    <message>
      <source>Append &amp;Text...</source>
      <translation>Jatka &amp;tekstiä...</translation>
    </message>
    <message>
      <source>Get Image...</source>
      <translation>Hae kuva...</translation>
    </message>
    <message>
      <source>Save &amp;Text...</source>
      <translation>Tallenna &amp;teksti...</translation>
    </message>
    <message>
      <source>Save Page as &amp;EPS...</source>
      <translation>Tallenna sivu &amp;EPS:nä...</translation>
    </message>
    <message>
      <source>Save as P&amp;DF...</source>
      <translation>Tallenna P&amp;DF:nä...</translation>
    </message>
    <message>
      <source>Document &amp;Setup...</source>
      <translation>A&amp;siakirjan asettelu...</translation>
    </message>
    <message>
      <source>&amp;Print...</source>
      <translation>T&amp;ulosta...</translation>
    </message>
    <message>
      <source>&amp;Quit</source>
      <translation>&amp;Lopeta</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>&amp;Kumoa</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>&amp;Tee uudelleen</translation>
    </message>
    <message>
      <source>&amp;Item Action Mode</source>
      <translation>Objektikohtainen toimintohistoria</translation>
    </message>
    <message>
      <source>Cu&amp;t</source>
      <translation>&amp;Leikkaa</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>&amp;Kopioi</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>L&amp;iitä</translation>
    </message>
    <message>
      <source>C&amp;lear Contents</source>
      <translation>&amp;Tyhjennä</translation>
    </message>
    <message>
      <source>Select &amp;All</source>
      <translation>V&amp;alitse kaikki</translation>
    </message>
    <message>
      <source>&amp;Deselect All</source>
      <translation>&amp;Poista valinnat</translation>
    </message>
    <message>
      <source>&amp;Search/Replace...</source>
      <translation>Et&amp;si/Korvaa...</translation>
    </message>
    <message>
      <source>Edit Image...</source>
      <translation>Muokkaa kuvaa...</translation>
    </message>
    <message>
      <source>C&amp;olors...</source>
      <translation>&amp;Värit...</translation>
    </message>
    <message>
      <source>&amp;Paragraph Styles...</source>
      <translation>Ka&amp;ppaletyylit...</translation>
    </message>
    <message>
      <source>&amp;Line Styles...</source>
      <translation>Viivatyy&amp;lit...</translation>
    </message>
    <message>
      <source>&amp;Master Pages...</source>
      <translation>&amp;Mallisivut...</translation>
    </message>
    <message>
      <source>P&amp;references...</source>
      <translation>&amp;Asetukset...</translation>
    </message>
    <message>
      <source>%1 pt</source>
      <translation>%1 pt</translation>
    </message>
    <message>
      <source>&amp;Other...</source>
      <translation>M&amp;uu...</translation>
    </message>
    <message>
      <source>&amp;Left</source>
      <translation>&amp;Vasen</translation>
    </message>
    <message>
      <source>&amp;Center</source>
      <translation>&amp;Keskitetty</translation>
    </message>
    <message>
      <source>&amp;Right</source>
      <translation>&amp;Oikea</translation>
    </message>
    <message>
      <source>&amp;Block</source>
      <translation>&amp;Molemmat reunat</translation>
    </message>
    <message>
      <source>&amp;Forced</source>
      <translation>&amp;Pakotettu</translation>
    </message>
    <message>
      <source>&amp;%1 %</source>
      <translation>&amp;%1 %</translation>
    </message>
    <message>
      <source>&amp;Normal</source>
      <translation>&amp;Normaali</translation>
    </message>
    <message>
      <source>&amp;Underline</source>
      <translation>&amp;Alleviivattu</translation>
    </message>
    <message>
      <source>Underline &amp;Words</source>
      <translation>Alleviivaa &amp;sanat</translation>
    </message>
    <message>
      <source>&amp;Strike Through</source>
      <translation>&amp;Yliviivaa</translation>
    </message>
    <message>
      <source>&amp;All Caps</source>
      <translation>&amp;Versaalit</translation>
    </message>
    <message>
      <source>Small &amp;Caps</source>
      <translation>&amp;Kapiteelit</translation>
    </message>
    <message>
      <source>Su&amp;perscript</source>
      <translation>&amp;Yläindeksi</translation>
    </message>
    <message>
      <source>Su&amp;bscript</source>
      <translation>&amp;Alaindeksi</translation>
    </message>
    <message>
      <source>&amp;Outline</source>
      <translation>&amp;Rakenne</translation>
    </message>
    <message>
      <source>S&amp;hadow</source>
      <translation>&amp;Varjo</translation>
    </message>
    <message>
      <source>&amp;Image Effects</source>
      <translation>&amp;Kuvaefektit</translation>
    </message>
    <message>
      <source>&amp;Tabulators...</source>
      <translation>&amp;Sarkaimet...</translation>
    </message>
    <message>
      <source>D&amp;uplicate</source>
      <translation>&amp;Kopioi</translation>
    </message>
    <message>
      <source>&amp;Multiple Duplicate</source>
      <translation>&amp;Useita kopioita</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Poista</translation>
    </message>
    <message>
      <source>&amp;Group</source>
      <translation>&amp;Ryhmitä</translation>
    </message>
    <message>
      <source>&amp;Ungroup</source>
      <translation>Pur&amp;a ryhmitys</translation>
    </message>
    <message>
      <source>Is &amp;Locked</source>
      <translation>On &amp;lukittu</translation>
    </message>
    <message>
      <source>Si&amp;ze is Locked</source>
      <translation>Koko on l&amp;ukittu</translation>
    </message>
    <message>
      <source>Lower to &amp;Bottom</source>
      <translation>Vie &amp;alimmaiseksi</translation>
    </message>
    <message>
      <source>Raise to &amp;Top</source>
      <translation>Tuo &amp;päällimmäiseksi</translation>
    </message>
    <message>
      <source>&amp;Lower</source>
      <translation>Vie a&amp;laspäin</translation>
    </message>
    <message>
      <source>&amp;Raise</source>
      <translation>&amp;Tuo ylöspäin</translation>
    </message>
    <message>
      <source>Send to S&amp;crapbook</source>
      <translation>&amp;Vie leikekirjaan</translation>
    </message>
    <message>
      <source>&amp;Attributes...</source>
      <translation>&amp;Muuttujat...</translation>
    </message>
    <message>
      <source>I&amp;mage Visible</source>
      <translation>Kuva &amp;näkyvissä</translation>
    </message>
    <message>
      <source>&amp;Update Image</source>
      <translation>&amp;Päivitä kuva</translation>
    </message>
    <message>
      <source>Adjust Frame to Image</source>
      <translation>Sovita kehys kuvaan</translation>
    </message>
    <message>
      <source>Extended Image Properties</source>
      <translation>Kuvan ominaisuudet</translation>
    </message>
    <message>
      <source>&amp;Low Resolution</source>
      <translation>&amp;Alhainen tarkkuus</translation>
    </message>
    <message>
      <source>&amp;Normal Resolution</source>
      <translation>&amp;Normaali tarkkuus</translation>
    </message>
    <message>
      <source>&amp;Full Resolution</source>
      <translation>&amp;Täysi tarkkuus</translation>
    </message>
    <message>
      <source>Is PDF &amp;Bookmark</source>
      <translation>&amp;On PDF-kirjanmerkki</translation>
    </message>
    <message>
      <source>Is PDF A&amp;nnotation</source>
      <translation>On PDF-&amp;merkintä</translation>
    </message>
    <message>
      <source>Annotation P&amp;roperties</source>
      <translation>Merkinnän &amp;ominaisuudet</translation>
    </message>
    <message>
      <source>Field P&amp;roperties</source>
      <translation>&amp;Kentän ominaisuudet</translation>
    </message>
    <message>
      <source>&amp;Edit Shape...</source>
      <translation>&amp;Muokkaa kuviota...</translation>
    </message>
    <message>
      <source>&amp;Attach Text to Path</source>
      <translation>K&amp;iinnitä teksti polkuun</translation>
    </message>
    <message>
      <source>&amp;Detach Text from Path</source>
      <translation>Poi&amp;sta teksti polulta</translation>
    </message>
    <message>
      <source>&amp;Combine Polygons</source>
      <translation>Y&amp;hdistä monikulmiot</translation>
    </message>
    <message>
      <source>Split &amp;Polygons</source>
      <translation>&amp;Erota monikulmiot</translation>
    </message>
    <message>
      <source>&amp;Bezier Curve</source>
      <translation>&amp;Bezier-viiva</translation>
    </message>
    <message>
      <source>&amp;Image Frame</source>
      <translation>&amp;Kuvakehys</translation>
    </message>
    <message>
      <source>&amp;Outlines</source>
      <translation>&amp;Ääriviivat</translation>
    </message>
    <message>
      <source>&amp;Polygon</source>
      <translation>&amp;Monikulmio</translation>
    </message>
    <message>
      <source>&amp;Text Frame</source>
      <translation>&amp;Tekstikehys</translation>
    </message>
    <message>
      <source>&amp;Glyph...</source>
      <translation>&amp;Merkki</translation>
    </message>
    <message>
      <source>Sample Text</source>
      <translation>Esimerkkiteksti</translation>
    </message>
    <message>
      <source>&amp;Insert...</source>
      <translation>&amp;Lisää...</translation>
    </message>
    <message>
      <source>Im&amp;port...</source>
      <translation>&amp;Tuo</translation>
    </message>
    <message>
      <source>&amp;Delete...</source>
      <translation>&amp;Poista...</translation>
    </message>
    <message>
      <source>&amp;Copy...</source>
      <translation>&amp;Kopioi...</translation>
    </message>
    <message>
      <source>&amp;Move...</source>
      <translation>&amp;Siirrä...</translation>
    </message>
    <message>
      <source>&amp;Apply Master Page...</source>
      <translation>&amp;Käytä mallisivua...</translation>
    </message>
    <message>
      <source>Manage &amp;Guides...</source>
      <translation>&amp;Apuviivojen hallinta...</translation>
    </message>
    <message>
      <source>&amp;Fit in window</source>
      <translation>&amp;Sovita ikkunaan</translation>
    </message>
    <message>
      <source>&amp;50%</source>
      <translation>&amp;50 %</translation>
    </message>
    <message>
      <source>&amp;75%</source>
      <translation>&amp;75 %</translation>
    </message>
    <message>
      <source>&amp;100%</source>
      <translation>&amp;100 %</translation>
    </message>
    <message>
      <source>&amp;200%</source>
      <translation>&amp;200 %</translation>
    </message>
    <message>
      <source>&amp;Thumbnails</source>
      <translation>&amp;Esikatselukuvat</translation>
    </message>
    <message>
      <source>Show &amp;Margins</source>
      <translation>Näytä &amp;reunukset</translation>
    </message>
    <message>
      <source>Show &amp;Frames</source>
      <translation>Näytä &amp;kehykset</translation>
    </message>
    <message>
      <source>Show &amp;Images</source>
      <translation>Näytä &amp;kuvat</translation>
    </message>
    <message>
      <source>Show &amp;Grid</source>
      <translation>Näytä ruu&amp;dukko</translation>
    </message>
    <message>
      <source>Show G&amp;uides</source>
      <translation>Näytä &amp;apuviivat</translation>
    </message>
    <message>
      <source>Show &amp;Baseline Grid</source>
      <translation>Näytä p&amp;eruslinja</translation>
    </message>
    <message>
      <source>Show &amp;Text Chain</source>
      <translation>Näytä &amp;tekstin kulku</translation>
    </message>
    <message>
      <source>Show Control Characters</source>
      <translation>Näytä tulostumattomat merkit</translation>
    </message>
    <message>
      <source>Sn&amp;ap to Grid</source>
      <translation>Kiinnitä r&amp;uudukkoon</translation>
    </message>
    <message>
      <source>Sna&amp;p to Guides</source>
      <translation>Kiinnitä a&amp;puviivoihin</translation>
    </message>
    <message>
      <source>&amp;Properties</source>
      <translation>&amp;Ominaisuudet</translation>
    </message>
    <message>
      <source>&amp;Scrapbook</source>
      <translation>&amp;Leikekirja</translation>
    </message>
    <message>
      <source>&amp;Layers</source>
      <translation>&amp;Tasot</translation>
    </message>
    <message>
      <source>&amp;Arrange Pages</source>
      <translation>&amp;Järjestä sivuja</translation>
    </message>
    <message>
      <source>&amp;Bookmarks</source>
      <translation>&amp;Kirjanmerkit</translation>
    </message>
    <message>
      <source>&amp;Measurements</source>
      <translation>&amp;Mittaus</translation>
    </message>
    <message>
      <source>Action &amp;History</source>
      <translation>T&amp;oimintohistoria</translation>
    </message>
    <message>
      <source>Preflight &amp;Verifier</source>
      <translation>&amp;Tarkastustyökalu</translation>
    </message>
    <message>
      <source>&amp;Align and Distribute</source>
      <translation>&amp;Jaa ja tasaa</translation>
    </message>
    <message>
      <source>&amp;Tools</source>
      <translation>Työ&amp;kalut</translation>
    </message>
    <message>
      <source>P&amp;DF Tools</source>
      <translation>&amp;PDF-työkalut</translation>
    </message>
    <message>
      <source>Select Item</source>
      <translation>Valitse kohde</translation>
    </message>
    <message>
      <source>T&amp;able</source>
      <translation>T&amp;aulukko</translation>
    </message>
    <message>
      <source>&amp;Shape</source>
      <translation>&amp;Kuvio</translation>
    </message>
    <message>
      <source>&amp;Line</source>
      <translation>&amp;Viiva</translation>
    </message>
    <message>
      <source>&amp;Freehand Line</source>
      <translation>&amp;Vapaa viiva</translation>
    </message>
    <message>
      <source>Rotate Item</source>
      <translation>Kierrä kohdetta</translation>
    </message>
    <message>
      <source>Zoom in or out</source>
      <translation>Lähennä tai loitonna</translation>
    </message>
    <message>
      <source>Zoom in</source>
      <translation>Lähennä</translation>
    </message>
    <message>
      <source>Zoom out</source>
      <translation>Loitonna</translation>
    </message>
    <message>
      <source>Edit Contents of Frame</source>
      <translation>Muokkaa kehyksen sisältöä</translation>
    </message>
    <message>
      <source>Edit Text...</source>
      <translation>Muokkaa tekstiä...</translation>
    </message>
    <message>
      <source>Link Text Frames</source>
      <translation>Linkitä tekstikehykset</translation>
    </message>
    <message>
      <source>Unlink Text Frames</source>
      <translation>Pura tekstikehysten linkitys</translation>
    </message>
    <message>
      <source>&amp;Eye Dropper</source>
      <translation>&amp;Värivalitsin</translation>
    </message>
    <message>
      <source>Copy Item Properties</source>
      <translation>Kopioi kohteen ominaisuudet</translation>
    </message>
    <message>
      <source>Edit the text with the Story Editor</source>
      <translation>Muokkaa tekstiä editorilla</translation>
    </message>
    <message>
      <source>Insert Text Frame</source>
      <translation>Lisää tekstikehys</translation>
    </message>
    <message>
      <source>Insert Image Frame</source>
      <translation>Lisää kuvakehys</translation>
    </message>
    <message>
      <source>Insert Table</source>
      <translation>Lisää taulukko</translation>
    </message>
    <message>
      <source>Insert Shape</source>
      <translation>Lisää kuvio</translation>
    </message>
    <message>
      <source>Insert Polygon</source>
      <translation>Lisää monikulmio</translation>
    </message>
    <message>
      <source>Insert Line</source>
      <translation>Lisää viiva</translation>
    </message>
    <message>
      <source>Insert Bezier Curve</source>
      <translation>Lisää Bezier-viiva</translation>
    </message>
    <message>
      <source>Insert Freehand Line</source>
      <translation>Lisää vapaa viiva</translation>
    </message>
    <message>
      <source>&amp;Manage Pictures</source>
      <translation>&amp;Kuvien hallinta</translation>
    </message>
    <message>
      <source>&amp;Hyphenate Text</source>
      <translation>&amp;Tavuta teksti</translation>
    </message>
    <message>
      <source>&amp;Generate Table Of Contents</source>
      <translation>&amp;Luo sisällysluettelo</translation>
    </message>
    <message>
      <source>&amp;About Scribus</source>
      <translation>&amp;Tietoja Scribuksesta</translation>
    </message>
    <message>
      <source>About &amp;Qt</source>
      <translation>Tietoja &amp;Qt:sta</translation>
    </message>
    <message>
      <source>Toolti&amp;ps</source>
      <translation>&amp;Työkaluvihjeet</translation>
    </message>
    <message>
      <source>Scribus &amp;Manual...</source>
      <translation>Scribus &amp;ohje...</translation>
    </message>
    <message>
      <source>Smart &amp;Hyphen</source>
      <translation>&amp;Tavuviiva</translation>
    </message>
    <message>
      <source>Non Breaking Dash</source>
      <translation>Sitova viiva</translation>
    </message>
    <message>
      <source>Non Breaking &amp;Space</source>
      <translation>Sitova &amp;välilyönti</translation>
    </message>
    <message>
      <source>Page &amp;Number</source>
      <translation>Sivu&amp;numero</translation>
    </message>
    <message>
      <source>New Line</source>
      <translation>Rivinvaihto</translation>
    </message>
    <message>
      <source>Frame Break</source>
      <translation>Kehyksen vaihto</translation>
    </message>
    <message>
      <source>Column Break</source>
      <translation>Palstan vaihto</translation>
    </message>
    <message>
      <source>Copyright</source>
      <translation>Copyright</translation>
    </message>
    <message>
      <source>Registered Trademark</source>
      <translation>Rekisteröity tavaramerkki</translation>
    </message>
    <message>
      <source>Trademark</source>
      <translation>Tavaramerkki</translation>
    </message>
    <message>
      <source>Bullet</source>
      <translation>Luetelmapallo</translation>
    </message>
    <message>
      <source>Em Dash</source>
      <translation>Pitkä viiva (em)</translation>
    </message>
    <message>
      <source>En Dash</source>
      <translation>Lyhyt viiva (en)</translation>
    </message>
    <message>
      <source>Figure Dash</source>
      <translation>Numeroviiva</translation>
    </message>
    <message>
      <source>Quotation Dash</source>
      <translation>Ajatusviiva</translation>
    </message>
    <message>
      <source>Apostrophe</source>
      <translation>Ascii-heittomerkki</translation>
    </message>
    <message>
      <source>Straight Double</source>
      <translation>Ascii-lainausmerkki</translation>
    </message>
    <message>
      <source>Single Left</source>
      <translation>Ylösalainen puolilainausmerkki</translation>
    </message>
    <message>
      <source>Single Right</source>
      <translation>Puolilainausmerkki</translation>
    </message>
    <message>
      <source>Double Left</source>
      <translation>Ylösalainen kokolainausmerkki</translation>
    </message>
    <message>
      <source>Double Right</source>
      <translation>Kokolainausmerkki</translation>
    </message>
    <message>
      <source>Single Reversed</source>
      <translation>Käännetty puolilainausmerkki</translation>
    </message>
    <message>
      <source>Double Reversed</source>
      <translation>Käännetty kokolainausmerkki</translation>
    </message>
    <message>
      <source>Single Left Guillemet</source>
      <translation>Vasen kulmapuolilainausmerkki</translation>
    </message>
    <message>
      <source>Single Right Guillemet</source>
      <translation>Oikea kulmapuolilainausmerkki</translation>
    </message>
    <message>
      <source>Double Left Guillemet</source>
      <translation>Vasen kulmalainausmerkki</translation>
    </message>
    <message>
      <source>Double Right Guillemet</source>
      <translation>Oikea kulmalainausmerkki</translation>
    </message>
    <message>
      <source>Low Single Comma</source>
      <translation>Rivinalinen puolilainausmerkki</translation>
    </message>
    <message>
      <source>Low Double Comma</source>
      <translation>Rivinalinen kokolainausmerkki</translation>
    </message>
    <message>
      <source>CJK Single Left</source>
      <translation>CJK vasen puolilainausmerkki</translation>
    </message>
    <message>
      <source>CJK Single Right</source>
      <translation>CJK oikea puolilainausmerkki</translation>
    </message>
    <message>
      <source>CJK Double Left</source>
      <translation>CJK vasen kokolainausmerkki</translation>
    </message>
    <message>
      <source>CJK Double Right</source>
      <translation>CJK oikea kokolainausmerkki</translation>
    </message>
    <message>
      <source>Toggle Palettes</source>
      <translation>Näytä/piilota ikkunat</translation>
    </message>
    <message>
      <source>Toggle Guides</source>
      <translation>Näytä/piilota apuviivat</translation>
    </message>
    <message>
      <source>Dehyphenate Text</source>
      <translation>Poista tavutus</translation>
    </message>
    <message>
      <source>Print Previe&amp;w</source>
      <translation>&amp;Esikatselu</translation>
    </message>
    <message>
      <source>&amp;JavaScripts...</source>
      <translation>&amp;JavaScriptit...</translation>
    </message>
    <message>
      <source>More Info...</source>
      <translation>Lisätietoja...</translation>
    </message>
    <message>
      <source>Convert to Master Page...</source>
      <translation>Lisää mallisivuksi...</translation>
    </message>
    <message>
      <source>Manage Page Properties...</source>
      <translation>Sivun asetukset...</translation>
    </message>
    <message>
      <source>Rulers relative to Page</source>
      <translation>Viivaimet suhteessa sivuun</translation>
    </message>
    <message>
      <source>&amp;Cascade</source>
      <translation>&amp;Lomittain</translation>
    </message>
    <message>
      <source>&amp;Tile</source>
      <translation>&amp;Vierekkäin</translation>
    </message>
    <message>
      <source>&amp;About Plug-ins</source>
      <translation>&amp;Tietoja laajennuksista</translation>
    </message>
  </context>
  <context>
    <name>AlignDistributePalette</name>
    <message>
      <source>Align and Distribute</source>
      <translation>Jaa ja tasaa</translation>
    </message>
    <message>
      <source>Align</source>
      <translation>Tasaa</translation>
    </message>
    <message>
      <source>&amp;Relative to:</source>
      <translation>&amp;Suhteessa:</translation>
    </message>
    <message>
      <source>First Selected</source>
      <translation>Ensin valittuun</translation>
    </message>
    <message>
      <source>Last Selected</source>
      <translation>Viimeiseksi valittuun</translation>
    </message>
    <message>
      <source>Page</source>
      <translation>Sivuun</translation>
    </message>
    <message>
      <source>Margins</source>
      <translation>Reunukseen</translation>
    </message>
    <message>
      <source>Selection</source>
      <translation>Valintaan</translation>
    </message>
    <message>
      <source>Align right sides of objects to left side of anchor</source>
      <translation>Tasaa kohteitten oikeat reunat valintaan</translation>
    </message>
    <message>
      <source>Align left sides of objects to right side of anchor</source>
      <translation>Tasaa kohteitten vasemmat reunat valintaan</translation>
    </message>
    <message>
      <source>Align bottoms</source>
      <translation>Tasaa alareunat</translation>
    </message>
    <message>
      <source>Align right sides</source>
      <translation>Tasaa oikeat reunat</translation>
    </message>
    <message>
      <source>Align tops of objects to bottom of anchor</source>
      <translation>Tasaa kohteitten yläreunat valinnan alapuolelle</translation>
    </message>
    <message>
      <source>Center on vertical axis</source>
      <translation>Keskitä pystysuoralle akselille</translation>
    </message>
    <message>
      <source>Align left sides</source>
      <translation>Tasaa vasemmat reunat</translation>
    </message>
    <message>
      <source>Center on horizontal axis</source>
      <translation>Keskitä vaakasuoralle akselille</translation>
    </message>
    <message>
      <source>Align bottoms of objects to top of anchor</source>
      <translation>Tasaa kohteitten alareunat valinnan yläpuolelle</translation>
    </message>
    <message>
      <source>Align tops</source>
      <translation>Tasaa yläreunat</translation>
    </message>
    <message>
      <source>Distribute</source>
      <translation>Jaa</translation>
    </message>
    <message>
      <source>Make horizontal gaps between objects equal</source>
      <translation>Jaa kohteitten vaakasuorat välit tasaisesti</translation>
    </message>
    <message>
      <source>Make horizontal gaps between objects equal to the value specified</source>
      <translation>Jaa kohteitten vaakasuorat välit arvon mukaan</translation>
    </message>
    <message>
      <source>Distribute right sides equidistantly</source>
      <translation>Jaa oikeat sivut tasaisesti</translation>
    </message>
    <message>
      <source>Distribute bottoms equidistantly</source>
      <translation>Jaa alareunat tasaisesti</translation>
    </message>
    <message>
      <source>Distribute centers equidistantly horizontally</source>
      <translation>Jaa keskikohdat tasaiset vaakasuunnassa</translation>
    </message>
    <message>
      <source>Make vertical gaps between objects equal</source>
      <translation>Tee pystysuunnan välit yhtä suuriksi</translation>
    </message>
    <message>
      <source>Make vertical gaps between objects equal to the value specified</source>
      <translation>Jaa kohteitten pystysuunnan välit arvon mukaan</translation>
    </message>
    <message>
      <source>Distribute left sides equidistantly</source>
      <translation>Jaa vasemmat reunat tasaisesti</translation>
    </message>
    <message>
      <source>Distribute centers equidistantly vertically</source>
      <translation>Jaa keskikohdat tasaisesti</translation>
    </message>
    <message>
      <source>Distribute tops equidistantly</source>
      <translation>Jaa yläreunat tasaisesti</translation>
    </message>
    <message>
      <source>&amp;Distance:</source>
      <translation>&amp;Etäisyys:</translation>
    </message>
    <message>
      <source>Distribute the items with the distance specified</source>
      <translation>Jaa kohteet määritellyn etäisyyden perusteella</translation>
    </message>
    <message>
      <source>Some objects are locked.</source>
      <translation>Jotkin kohteet ovat lukittuina.</translation>
    </message>
    <message>
      <source>&amp;Unlock All</source>
      <translation>&amp;Vapauta kaikki</translation>
    </message>
    <message>
      <source>Guide</source>
      <translation>Apuviiva</translation>
    </message>
    <message>
      <source>&amp;Selected Guide:</source>
      <translation>&amp;Valittu apuviiva:</translation>
    </message>
    <message>
      <source>None Selected</source>
      <translation>Ei valittuja</translation>
    </message>
    <message>
      <source>Y: %1%2</source>
      <translation>Y: %1%2</translation>
    </message>
    <message>
      <source>X: %1%2</source>
      <translation>X: %1%2</translation>
    </message>
  </context>
  <context>
    <name>AlignSelect</name>
    <message>
      <source>Align Text Left</source>
      <translation>Tasaa teksti vasemmalle</translation>
    </message>
    <message>
      <source>Align Text Right</source>
      <translation>Tasaa teksti oikealle</translation>
    </message>
    <message>
      <source>Align Text Center</source>
      <translation>Keskitä teksti</translation>
    </message>
    <message>
      <source>Align Text Justified</source>
      <translation>Tasaa tekstin molemmat reunat</translation>
    </message>
    <message>
      <source>Align Text Forced Justified</source>
      <translation>Pakota tekstin molemmat reunat</translation>
    </message>
  </context>
  <context>
    <name>Annot</name>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>sum</source>
      <translation>summa</translation>
    </message>
    <message>
      <source>Destination</source>
      <translation>Kohde</translation>
    </message>
    <message>
      <source>Date</source>
      <translation>Päivämäärä</translation>
    </message>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Avaa</translation>
    </message>
    <message>
      <source>Push</source>
      <translation>Paina</translation>
    </message>
    <message>
      <source>Star</source>
      <translation>Tähti</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>Teksti</translation>
    </message>
    <message>
      <source>Thin</source>
      <translation>Ohut</translation>
    </message>
    <message>
      <source>Time</source>
      <translation>Aika</translation>
    </message>
    <message>
      <source>Wide</source>
      <translation>Leveä</translation>
    </message>
    <message>
      <source>Prepend Currency Symbol</source>
      <translation>Sijoita valuuttamerkki eteen</translation>
    </message>
    <message>
      <source>Submit Form</source>
      <translation>Lähetä lomake</translation>
    </message>
    <message>
      <source>Limit of</source>
      <translation>Yläraja</translation>
    </message>
    <message>
      <source>Check</source>
      <translation>Rasti</translation>
    </message>
    <message>
      <source>Cross</source>
      <translation>Risti</translation>
    </message>
    <message>
      <source>On Focus</source>
      <translation>Kohdistettaessa</translation>
    </message>
    <message>
      <source>Go To</source>
      <translation>Siirry</translation>
    </message>
    <message>
      <source>Icons</source>
      <translation>Kuvakkeet</translation>
    </message>
    <message>
      <source>Inset</source>
      <translation>Upotettu</translation>
    </message>
    <message>
      <source>Name:</source>
      <translation>Nimi:</translation>
    </message>
    <message>
      <source>Page:</source>
      <translation>Sivu:</translation>
    </message>
    <message>
      <source>Other</source>
      <translation>Muu</translation>
    </message>
    <message>
      <source>Plain</source>
      <translation>Paljas</translation>
    </message>
    <message>
      <source>Solid</source>
      <translation>Kiinteä</translation>
    </message>
    <message>
      <source>Type:</source>
      <translation>Tyyppi:</translation>
    </message>
    <message>
      <source>List Box</source>
      <translation>Listavalikko</translation>
    </message>
    <message>
      <source>Use Currency Symbol</source>
      <translation>Käytä valuuttamerkkiä</translation>
    </message>
    <message>
      <source>Number Format</source>
      <translation>Numeron muoto</translation>
    </message>
    <message>
      <source>Check Style:</source>
      <translation>Valintatyyli:</translation>
    </message>
    <message>
      <source>To File:</source>
      <translation>Tiedostoon:</translation>
    </message>
    <message>
      <source>No Print</source>
      <translation>Ei tulosteta</translation>
    </message>
    <message>
      <source>Submit to URL:</source>
      <translation>Lähetä URL:iin:</translation>
    </message>
    <message>
      <source>Calculate</source>
      <translation>Laske</translation>
    </message>
    <message>
      <source>Roll Over</source>
      <translation>Ylivienti</translation>
    </message>
    <message>
      <source>Value is not validated</source>
      <translation>Arvoa ei tarkasteta</translation>
    </message>
    <message>
      <source>maximum</source>
      <translation>maksimi</translation>
    </message>
    <message>
      <source>Formatting</source>
      <translation>Muotoilu</translation>
    </message>
    <message>
      <source>Combo Box</source>
      <translation>Alasvetovalikko</translation>
    </message>
    <message>
      <source>Custom validate script:</source>
      <translation>Oma tarkastusskripti:</translation>
    </message>
    <message>
      <source>minimum</source>
      <translation>minimi</translation>
    </message>
    <message>
      <source>Action</source>
      <translation>Toiminta</translation>
    </message>
    <message>
      <source>Border</source>
      <translation>Reuna</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Peruuta</translation>
    </message>
    <message>
      <source>Date Format</source>
      <translation>Päivämäärän muoto</translation>
    </message>
    <message>
      <source>Button</source>
      <translation>Painike</translation>
    </message>
    <message>
      <source>Circle</source>
      <translation>Ympyrä</translation>
    </message>
    <message>
      <source>Color:</source>
      <translation>Väri:</translation>
    </message>
    <message>
      <source>Field Properties</source>
      <translation>Kentän ominaisuudet</translation>
    </message>
    <message>
      <source>Dashed</source>
      <translation>Katkoviiva</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>Oma</translation>
    </message>
    <message>
      <source>Time Format</source>
      <translation>Ajan muoto</translation>
    </message>
    <message>
      <source>Event:</source>
      <translation>Tapahtuma:</translation>
    </message>
    <message>
      <source>Value is the</source>
      <translation>Arvo on</translation>
    </message>
    <message>
      <source>Format</source>
      <translation>Muotoilu</translation>
    </message>
    <message>
      <source>Hidden</source>
      <translation>Piilotettu</translation>
    </message>
    <message>
      <source>Custom calculation script:</source>
      <translation>Oma laskuskripti:</translation>
    </message>
    <message>
      <source>Enter a comma separated list of fields here</source>
      <translation>Anna kenttien nimet pilkuin eroteltuina</translation>
    </message>
    <message>
      <source>Custom Scripts</source>
      <translation>Omat skriptit</translation>
    </message>
    <message>
      <source>Invert</source>
      <translation>Käännä</translation>
    </message>
    <message>
      <source>No View</source>
      <translation>Ei näytetä</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>Normaali</translation>
    </message>
    <message>
      <source>Number</source>
      <translation>Numero</translation>
    </message>
    <message>
      <source>Remove</source>
      <translation>Poista</translation>
    </message>
    <message>
      <source>Square</source>
      <translation>Nelikulmio</translation>
    </message>
    <message>
      <source>Style:</source>
      <translation>Tyyli:</translation>
    </message>
    <message>
      <source>X-Pos:</source>
      <translation>X:</translation>
    </message>
    <message>
      <source>Field is formatted as:</source>
      <translation>Kenttä on muotoiltu:</translation>
    </message>
    <message>
      <source>Y-Pos:</source>
      <translation>Y:</translation>
    </message>
    <message>
      <source>Width:</source>
      <translation>Leveys:</translation>
    </message>
    <message>
      <source>Tool-Tip:</source>
      <translation>Työkaluvihje:</translation>
    </message>
    <message>
      <source>On Blur</source>
      <translation>On Blur</translation>
    </message>
    <message>
      <source>Don't Export Value</source>
      <translation>Älä vie arvoa</translation>
    </message>
    <message>
      <source>Submit Data as HTML</source>
      <translation>Lähetä data HTML:na</translation>
    </message>
    <message>
      <source>Text Field</source>
      <translation>Tekstikenttä</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>Asetukset</translation>
    </message>
    <message>
      <source>Icon Placement...</source>
      <translation>Kuvakkeen sijoittelu...</translation>
    </message>
    <message>
      <source>Mouse Up</source>
      <translation>Hiiri ylös</translation>
    </message>
    <message>
      <source>Properties</source>
      <translation>Ominaisuudet</translation>
    </message>
    <message>
      <source>Pick...</source>
      <translation>Valitse...</translation>
    </message>
    <message>
      <source>Mouse Enter</source>
      <translation>Hiiri saapuu</translation>
    </message>
    <message>
      <source>Use Icons</source>
      <translation>Käytä kuvakkeita</translation>
    </message>
    <message>
      <source>Read Only</source>
      <translation>Vain luku</translation>
    </message>
    <message>
      <source>Pressed</source>
      <translation>Painettu</translation>
    </message>
    <message>
      <source>product</source>
      <translation>tulo</translation>
    </message>
    <message>
      <source>Text for Roll Over</source>
      <translation>Teksti yliviennille</translation>
    </message>
    <message>
      <source>Validate</source>
      <translation>Tarkastus</translation>
    </message>
    <message>
      <source>Change...</source>
      <translation>Muuta...</translation>
    </message>
    <message>
      <source>Percent Format</source>
      <translation>Prosenttimuoto</translation>
    </message>
    <message>
      <source>Characters</source>
      <translation>kirjainmerkkiä</translation>
    </message>
    <message>
      <source>Password</source>
      <translation>Salasana</translation>
    </message>
    <message>
      <source>Visibility:</source>
      <translation>Näkyvyys:</translation>
    </message>
    <message>
      <source>average</source>
      <translation>keskiarvo</translation>
    </message>
    <message>
      <source>Beveled</source>
      <translation>Viisto</translation>
    </message>
    <message>
      <source>Import Data</source>
      <translation>Tuo data</translation>
    </message>
    <message>
      <source>Mouse Exit</source>
      <translation>Hiiri poistuu</translation>
    </message>
    <message>
      <source>Mouse Down</source>
      <translation>Hiiri alas</translation>
    </message>
    <message>
      <source>Script:</source>
      <translation>Skripti:</translation>
    </message>
    <message>
      <source>Percentage</source>
      <translation>Prosentti</translation>
    </message>
    <message>
      <source>Decimals:</source>
      <translation>Desimaalit:</translation>
    </message>
    <message>
      <source>of the following fields:</source>
      <translation>seuraavista kentistä:</translation>
    </message>
    <message>
      <source>Import Data from:</source>
      <translation>Tuo data:</translation>
    </message>
    <message>
      <source>and less or equal to:</source>
      <translation>ja pienempi tai yhtä suuri kuin:</translation>
    </message>
    <message>
      <source>Keystroke:</source>
      <translation>Näppäimenlyönti:</translation>
    </message>
    <message>
      <source>Multi-Line</source>
      <translation>Monirivinen</translation>
    </message>
    <message>
      <source>Underline</source>
      <translation>Alleviivattu</translation>
    </message>
    <message>
      <source>Diamond</source>
      <translation>Timantti</translation>
    </message>
    <message>
      <source>Editable</source>
      <translation>Muokattava</translation>
    </message>
    <message>
      <source>Appearance</source>
      <translation>Ulkonäkö</translation>
    </message>
    <message>
      <source>Check Box</source>
      <translation>Valintaruutu</translation>
    </message>
    <message>
      <source>Value is not calculated</source>
      <translation>Arvoa ei lasketa</translation>
    </message>
    <message>
      <source>Edit...</source>
      <translation>Muokkaa...</translation>
    </message>
    <message>
      <source>Reset Form</source>
      <translation>Palauta lomake</translation>
    </message>
    <message>
      <source>Required</source>
      <translation>Pakollinen</translation>
    </message>
    <message>
      <source>Do Not Spell Check</source>
      <translation>Älä oikolue</translation>
    </message>
    <message>
      <source>Outlined</source>
      <translation>Rajattu äärivoin</translation>
    </message>
    <message>
      <source>Visible</source>
      <translation>Näkyvissä</translation>
    </message>
    <message>
      <source>Value must be greater than or equal to:</source>
      <translation>Arvon täytyy olla suurempi tai yhtä suuri kuin:</translation>
    </message>
    <message>
      <source>Format:</source>
      <translation>Muotoilu:</translation>
    </message>
    <message>
      <source>Highlight</source>
      <translation>Korosta</translation>
    </message>
    <message>
      <source>Default is Checked</source>
      <translation>Oletusarvona valittu</translation>
    </message>
    <message>
      <source>Example:</source>
      <translation>Esimerkki:</translation>
    </message>
    <message>
      <source>Do Not Scroll</source>
      <translation>Älä vieritä</translation>
    </message>
    <message>
      <source>Text for Button Down</source>
      <translation>Teksti painikkeen ollessa painettuna</translation>
    </message>
    <message>
      <source>You need at least the Icon for Normal to use Icons for Buttons</source>
      <translation>Tarvitset ainakin normaalin kuvakkeen käyttääksesi painikkeiden kuvakkeita</translation>
    </message>
    <message>
      <source>Selection Change</source>
      <translation>Valinta muuttui</translation>
    </message>
    <message>
      <source>Font for use with PDF 1.3:</source>
      <translation>PDF 1.3:n kanssa käytettävä kirjaintyyppi:</translation>
    </message>
    <message>
      <source>Flag is ignored for PDF 1.3</source>
      <translation>Merkintä jätetään huomioimatta PDF 1.3 -tiedostomuotoa käytettäessä</translation>
    </message>
    <message>
      <source>PDF Files (*.pdf);;All Files (*)</source>
      <translation>PDF-tiedostot (*.pdf);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>JavaScript</source>
      <translation>JavaScript</translation>
    </message>
    <message>
      <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
      <translation>Kuvat (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;Kaikki tiedostot (*)</translation>
    </message>
  </context>
  <context>
    <name>Annota</name>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>Destination</source>
      <translation>Kohde</translation>
    </message>
    <message>
      <source>Link</source>
      <translation>Linkki</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Avaa</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>Teksti</translation>
    </message>
    <message>
      <source>External Web-Link</source>
      <translation>Ulkoinen web-linkki</translation>
    </message>
    <message>
      <source>Annotation Properties</source>
      <translation>Kommenttien asetukset</translation>
    </message>
    <message>
      <source>PDF-Documents (*.pdf);;All Files (*)</source>
      <translation>PDF-tiedostot (*.pdf);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>External Link</source>
      <translation>Ulkoinen linkki</translation>
    </message>
    <message>
      <source>&amp;Type:</source>
      <translation>&amp;Tyyppi:</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>&amp;Muuta...</translation>
    </message>
    <message>
      <source>&amp;Page:</source>
      <translation>&amp;Sivu:</translation>
    </message>
    <message>
      <source>&amp;X-Pos</source>
      <translation>&amp;X:</translation>
    </message>
    <message>
      <source>&amp;Y-Pos:</source>
      <translation>&amp;Y:</translation>
    </message>
  </context>
  <context>
    <name>ApplyMasterPageDialog</name>
    <message>
      <source>Normal</source>
      <translation>Normaali</translation>
    </message>
    <message>
      <source>Apply Master Page</source>
      <translation>Käytä mallisivua</translation>
    </message>
    <message>
      <source>&amp;Master Page:</source>
      <translation>&amp;Mallisivu:</translation>
    </message>
    <message>
      <source>Apply To</source>
      <translation>Käytä</translation>
    </message>
    <message>
      <source>Current &amp;page</source>
      <translation>&amp;Nykyiselle sivulle</translation>
    </message>
    <message>
      <source>Alt+P</source>
      <translation>Alt+P</translation>
    </message>
    <message>
      <source>&amp;Even pages</source>
      <translation>&amp;Parillisille sivuille</translation>
    </message>
    <message>
      <source>Alt+E</source>
      <translation>Alt+E</translation>
    </message>
    <message>
      <source>O&amp;dd pages</source>
      <translation>P&amp;arittomille sivuille</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>&amp;All pages</source>
      <translation>&amp;Kaikille sivuille</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>&amp;Within range</source>
      <translation>&amp;Sivuille</translation>
    </message>
    <message>
      <source>Alt+W</source>
      <translation>Alt+W</translation>
    </message>
    <message>
      <source>to</source>
      <translation>mihin</translation>
    </message>
    <message>
      <source>Alt+O</source>
      <translation>Alt+O</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>Apply the selected template to even, odd or all pages within the following range</source>
      <translation>Käytä valittua mallisivua parittomille, parillisille tai kaikille sivuille seuraavalla sivuvälillä</translation>
    </message>
  </context>
  <context>
    <name>ArrowChooser</name>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
  </context>
  <context>
    <name>Barcode</name>
    <message>
      <source>&amp;Barcode Generator...</source>
      <translation>&amp;Viivakoodi...</translation>
    </message>
    <message>
      <source>Scribus frontend for Pure Postscript Barcode Writer</source>
      <translation>Scribus-liitäntä Pure Postscript Barcode -ohjelmaan</translation>
    </message>
  </context>
  <context>
    <name>BarcodeGenerator</name>
    <message>
      <source>Error opening file: %1</source>
      <translation>Virhe avattaessa tiedostoa: %1</translation>
    </message>
    <message>
      <source>Error creating preview</source>
      <translation>Virhe luotaessa esikatselua</translation>
    </message>
  </context>
  <context>
    <name>BarcodeGeneratorBase</name>
    <message>
      <source>Barcode Creator</source>
      <translation>Viivakoodi</translation>
    </message>
    <message>
      <source>Barcode</source>
      <translation>Viivakoodi</translation>
    </message>
    <message>
      <source>&amp;Type:</source>
      <translation>&amp;Tyyppi:</translation>
    </message>
    <message>
      <source>C&amp;ode:</source>
      <translation>K&amp;oodi:</translation>
    </message>
    <message>
      <source>Select one of the available barcode type here</source>
      <translation>Valitse viivakoodityyppi</translation>
    </message>
    <message>
      <source>The numeric representation of the code itself. See the help message below</source>
      <translation>Koodin numeerinen eistys. Lisää tietoa ohjeessa alapuolella.</translation>
    </message>
    <message>
      <source>Reset the barcode samples</source>
      <translation>Tyhjennä viivakoodiesimerkit</translation>
    </message>
    <message>
      <source>&amp;Include text in barcode</source>
      <translation>&amp;Sisällytä teksti viivakoodiin</translation>
    </message>
    <message>
      <source>Alt+I</source>
      <translation>Alt+I</translation>
    </message>
    <message>
      <source>If checked, there will be numbers in the barcode too</source>
      <translation>Valittuna viivakoodiin sisällytetään myös numerot</translation>
    </message>
    <message>
      <source>&amp;Guard whitespace</source>
      <translation>&amp;Varmista tyhjä tila</translation>
    </message>
    <message>
      <source>Alt+G</source>
      <translation>Alt+G</translation>
    </message>
    <message>
      <source>Draw arrows to be sure of space next the code</source>
      <translation>Piirrä nuolet varmistaaksesi tilan koodin vieressä</translation>
    </message>
    <message>
      <source>Colors</source>
      <translation>Värit</translation>
    </message>
    <message>
      <source>&amp;Background</source>
      <translation>&amp;Tausta</translation>
    </message>
    <message>
      <source>Alt+B</source>
      <translation>Alt+B</translation>
    </message>
    <message>
      <source>Background color - under the code lines</source>
      <translation>Koodiviivojen taustan väri</translation>
    </message>
    <message>
      <source>&amp;Lines</source>
      <translation>&amp;Viivat</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation>Alt+L</translation>
    </message>
    <message>
      <source>Color of the lines in barcode</source>
      <translation>Viivakoodin viivojen väri</translation>
    </message>
    <message>
      <source>&amp;Text</source>
      <translation>&amp;Teksti</translation>
    </message>
    <message>
      <source>Alt+T</source>
      <translation>Alt+T</translation>
    </message>
    <message>
      <source>Color of the text and numbers</source>
      <translation>Tekstin ja numeroiden väri</translation>
    </message>
    <message>
      <source>Hints and help is shown here</source>
      <translation>Ohje ja vihjeet näytetään tässä</translation>
    </message>
    <message>
      <source>Preview of the result. 72dpi sample.</source>
      <translation>Viivakoodin esikatselu (72 dpi)</translation>
    </message>
  </context>
  <context>
    <name>Biblio</name>
    <message>
      <source>Delete</source>
      <translation>Poista</translation>
    </message>
    <message>
      <source>Object</source>
      <translation>Kohde</translation>
    </message>
    <message>
      <source>Scrapbooks (*.scs);;All Files (*)</source>
      <translation>Leikekirjat (*.scs);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Scrapbook</source>
      <translation>Leikekirja</translation>
    </message>
    <message>
      <source>New Entry</source>
      <translation>Uusi leike</translation>
    </message>
    <message>
      <source>Rename</source>
      <translation>Nimeä uudelleen</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;Uusi</translation>
    </message>
    <message>
      <source>&amp;Load...</source>
      <translation>&amp;Lataa...</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Tallenna</translation>
    </message>
    <message>
      <source>Save &amp;As...</source>
      <translation>Tallenna &amp;nimellä...</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Sulje</translation>
    </message>
    <message>
      <source>&amp;Small</source>
      <translation>&amp;Pieni</translation>
    </message>
    <message>
      <source>&amp;Medium</source>
      <translation>&amp;Keskikokoinen</translation>
    </message>
    <message>
      <source>&amp;Large</source>
      <translation>&amp;Iso</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>Tie&amp;dosto</translation>
    </message>
    <message>
      <source>&amp;Preview</source>
      <translation>&amp;Esikatselu</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation>&amp;Nimi:</translation>
    </message>
    <message>
      <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
      <translation>Nimi &quot;%1&quot; on jo olemassa. 
Valitse toinen nimi.</translation>
    </message>
  </context>
  <context>
    <name>BookMView</name>
    <message>
      <source>Move Bookmark</source>
      <translation>Siirrä kirjanmerkki</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Peruuta</translation>
    </message>
    <message>
      <source>Bookmarks</source>
      <translation>Kirjanmerkit</translation>
    </message>
    <message>
      <source>Insert Bookmark</source>
      <translation>Lisää kirjanmerkki</translation>
    </message>
  </context>
  <context>
    <name>BookPalette</name>
    <message>
      <source>Bookmarks</source>
      <translation>Kirjanmerkit</translation>
    </message>
  </context>
  <context>
    <name>ButtonIcon</name>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Icon</source>
      <translation>Kuvake</translation>
    </message>
    <message>
      <source>Never</source>
      <translation>Ei koskaan</translation>
    </message>
    <message>
      <source>Reset</source>
      <translation>Palauta alkuperäiseen muotoon</translation>
    </message>
    <message>
      <source>Caption overlays Icon</source>
      <translation>Teksti kuvakkeen päällä</translation>
    </message>
    <message>
      <source>Scale How:</source>
      <translation>Miten skaalataan:</translation>
    </message>
    <message>
      <source>Layout:</source>
      <translation>Asettelu:</translation>
    </message>
    <message>
      <source>Caption only</source>
      <translation>Ainoastaan teksti</translation>
    </message>
    <message>
      <source>Caption below Icon</source>
      <translation>Teksti kuvakkeen alapuolella</translation>
    </message>
    <message>
      <source>Always</source>
      <translation>Aina</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Peruuta</translation>
    </message>
    <message>
      <source>Scale:</source>
      <translation>Skaalaa:</translation>
    </message>
    <message>
      <source>Icon only</source>
      <translation>Ainoastaan kuvake</translation>
    </message>
    <message>
      <source>When Icon is too small</source>
      <translation>Kun kuvake on liian pieni</translation>
    </message>
    <message>
      <source>Caption left to Icon</source>
      <translation>Teksti kuvakkeen vasemmalla puolella</translation>
    </message>
    <message>
      <source>Caption right to Icon</source>
      <translation>Teksti kuvakkeen oikealla puolella</translation>
    </message>
    <message>
      <source>When Icon is too big</source>
      <translation>Kun kuvake on liian suuri</translation>
    </message>
    <message>
      <source>Non Proportional</source>
      <translation>Ei tasasuhtainen</translation>
    </message>
    <message>
      <source>Proportional</source>
      <translation>Tasasuhtainen</translation>
    </message>
    <message>
      <source>Icon Placement</source>
      <translation>Kuvakkeen sijoittelu</translation>
    </message>
    <message>
      <source>Caption above Icon</source>
      <translation>Teksti kuvakkeen yläpuolella</translation>
    </message>
  </context>
  <context>
    <name>CMSPrefs</name>
    <message>
      <source>Absolute Colorimetric</source>
      <translation>Absolute Colorimetric</translation>
    </message>
    <message>
      <source>System Profiles</source>
      <translation>Järjestelmän profiilit</translation>
    </message>
    <message>
      <source>Rendering Intents</source>
      <translation>Sovitustavat</translation>
    </message>
    <message>
      <source>Perceptual</source>
      <translation>Perceptual</translation>
    </message>
    <message>
      <source>Relative Colorimetric</source>
      <translation>Relative Colorimetric</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>Saturaation</translation>
    </message>
    <message>
      <source>Default color profile for solid colors on the page</source>
      <translation>Väriprofiili asiakirjan tasaisille väreille</translation>
    </message>
    <message>
      <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
      <translation>Itse tekemäsi tai näyttösi valmistajan toimittama väriprofiili. 
Profiilin tulee olla näytöllesi tarkoitettu ei yleinen väriprofiili kuten sRGB.</translation>
    </message>
    <message>
      <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
      <translation>Tulostimesi valmistajan toimittama väriprofiili tulostimellesi. 
Profiilin tulee olla tulostimellesi tarkoitettu ei yleinen väriprofiili kuten sRGB.</translation>
    </message>
    <message>
      <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
      <translation>Tumman pään tasaus on keino parantaa kuvien kontrastia. 
On suositeltavaa että valitsen tämän, jos asiakirjasi sisältää kuvia.</translation>
    </message>
    <message>
      <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
      <translation>Sovitustapa näytöllesi. Ellet tiedä mikä tapa valita, tulisi käyttää 
sovitustapoja Relative Colorimetric tai Perceptual.</translation>
    </message>
    <message>
      <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
      <translation>Sovitustapa tulostimellesi. Ellet tiedä mikä tapa valita, tulisi käyttää 
sovitustapoja Relative Colorimetric tai Perceptual.</translation>
    </message>
    <message>
      <source>Enable 'soft proofing' of how your document colors will print,
based on the chosen printer profile.</source>
      <translation>Mahdollistaa tulosteen värien jäljittelyn näytöllä 
perustuen käytettävään väriprofiiliin.</translation>
    </message>
    <message>
      <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
      <translation>Toiminto näyttää värit, jotka saattavat tulostua väärin. 
Toiminnon käyttö vaatii tarkat profiilit ja se tulee huomioida ainoastaan varoituksena.</translation>
    </message>
    <message>
      <source>&amp;Activate Color Management</source>
      <translation>Käytä &amp;värien hallintaa</translation>
    </message>
    <message>
      <source>&amp;Solid Colors:</source>
      <translation>&amp;Tasaiset värit:</translation>
    </message>
    <message>
      <source>&amp;Monitor:</source>
      <translation>&amp;Näyttö:</translation>
    </message>
    <message>
      <source>P&amp;rinter:</source>
      <translation>Tu&amp;lostin:</translation>
    </message>
    <message>
      <source>M&amp;onitor:</source>
      <translation>N&amp;äyttö:</translation>
    </message>
    <message>
      <source>Pr&amp;inter:</source>
      <translation>T&amp;ulostin:</translation>
    </message>
    <message>
      <source>Sim&amp;ulate Printer on the Screen</source>
      <translation>&amp;Jäljittele tulostinta näytöllä</translation>
    </message>
    <message>
      <source>Mark Colors out of &amp;Gamut</source>
      <translation>&amp;Merkkaa asteikon ulkopuoliset värit</translation>
    </message>
    <message>
      <source>Use &amp;Blackpoint Compensation</source>
      <translation>Käytä &amp;Tumman pään tasausta (BPC)</translation>
    </message>
    <message>
      <source>&amp;RGB Pictures:</source>
      <translation>&amp;RGB-kuvat:</translation>
    </message>
    <message>
      <source>&amp;CMYK Pictures:</source>
      <translation>&amp;CMYK-kuvat:</translation>
    </message>
    <message>
      <source>Default color profile for imported CMYK images</source>
      <translation>Oletusväriprofiili tuoduille CMYK-kuville</translation>
    </message>
    <message>
      <source>Default color profile for imported RGB images</source>
      <translation>Oletusväriprofiili tuoduille RGB-kuville</translation>
    </message>
  </context>
  <context>
    <name>CMYKChoose</name>
    <message>
      <source>B:</source>
      <translation>B:</translation>
    </message>
    <message>
      <source>C:</source>
      <translation>C:</translation>
    </message>
    <message>
      <source>G:</source>
      <translation>G:</translation>
    </message>
    <message>
      <source>K:</source>
      <translation>K:</translation>
    </message>
    <message>
      <source>M:</source>
      <translation>M:</translation>
    </message>
    <message>
      <source>R:</source>
      <translation>R:</translation>
    </message>
    <message>
      <source>Y:</source>
      <translation>Y:</translation>
    </message>
    <message>
      <source>New</source>
      <translation>Uusi</translation>
    </message>
    <message>
      <source>Old</source>
      <translation>Vanha</translation>
    </message>
    <message>
      <source>RGB</source>
      <translation>RGB</translation>
    </message>
    <message>
      <source>CMYK</source>
      <translation>CMYK</translation>
    </message>
    <message>
      <source>Web Safe RGB</source>
      <translation>Web-yhteensopiva RGB</translation>
    </message>
    <message>
      <source>Edit Color</source>
      <translation>Muokkaa värejä</translation>
    </message>
    <message>
      <source>Static Color Bars</source>
      <translation>Staattiset väripalkit</translation>
    </message>
    <message>
      <source>Dynamic Color Bars</source>
      <translation>Dynaamiset väripalkit</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>HSV-Colormap</source>
      <translation>HSV-värikartta</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation>&amp;Nimi:</translation>
    </message>
    <message>
      <source>Color &amp;Model</source>
      <translation>&amp;Värimalli</translation>
    </message>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Is Spot Color</source>
      <translation>On spottiväri</translation>
    </message>
    <message>
      <source>Is Registration Color</source>
      <translation>On prosessiväri</translation>
    </message>
    <message>
      <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
      <translation>Nimi &quot;%1&quot; on varattu läpinäkyvyydelle. 
Sitä ei voi käyttää.</translation>
    </message>
    <message>
      <source>Name of the color is not unique</source>
      <translation>Värin nimi on jo olemassa</translation>
    </message>
  </context>
  <context>
    <name>ChTable</name>
    <message>
      <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
      <translation>Esikatselun näet painamalla hiiren oikeaa painiketta. &quot;Lisää&quot; valitsee merkin valinta-alueelle. &quot;Poista&quot; poistaa viimeiseksi lisätyn.</translation>
    </message>
  </context>
  <context>
    <name>CharSelect</name>
    <message>
      <source>Select Character:</source>
      <translation>Valitse merkki:</translation>
    </message>
    <message>
      <source>Font:</source>
      <translation>Kirjaintyyppi:</translation>
    </message>
    <message>
      <source>Character Class:</source>
      <translation>Merkkiluokka:</translation>
    </message>
    <message>
      <source>&amp;Insert</source>
      <translation>&amp;Lisää</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>&amp;Tyhjennä</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Sulje</translation>
    </message>
    <message>
      <source>Insert the characters at the cursor in the text</source>
      <translation>Lisää merkit kursorin kohdalle tekstiin</translation>
    </message>
    <message>
      <source>Delete the current selection(s).</source>
      <translation>Poista valinta.</translation>
    </message>
    <message>
      <source>Close this dialog and return to text editing.</source>
      <translation>Sulje ikkuna ja palaa muokkaamaan tekstiä.</translation>
    </message>
    <message>
      <source>Full Character Set</source>
      <translation>Kaikki merkit</translation>
    </message>
    <message>
      <source>Basic Latin</source>
      <translation>Latinalainen perusosa</translation>
    </message>
    <message>
      <source>Latin-1 Supplement</source>
      <translation>Latin-1-täydennys</translation>
    </message>
    <message>
      <source>Latin Extended-A</source>
      <translation>Latin laajennusosa A</translation>
    </message>
    <message>
      <source>Latin Extended-B</source>
      <translation>Latin laajennusosa B</translation>
    </message>
    <message>
      <source>General Punctuation</source>
      <translation>Yleiset välimerkit</translation>
    </message>
    <message>
      <source>Super- and Subscripts</source>
      <translation>Ylä- ja alaindeksit</translation>
    </message>
    <message>
      <source>Currency Symbols</source>
      <translation>Valuuttamerkit</translation>
    </message>
    <message>
      <source>Letterlike Symbols</source>
      <translation>Kirjainsymbolit</translation>
    </message>
    <message>
      <source>Number Forms</source>
      <translation>Lukuilmaisuja</translation>
    </message>
    <message>
      <source>Arrows</source>
      <translation>Nuolet</translation>
    </message>
    <message>
      <source>Mathematical Operators</source>
      <translation>Matemaattiset operaattorit</translation>
    </message>
    <message>
      <source>Box Drawing</source>
      <translation>Viivapiirrosmerkit</translation>
    </message>
    <message>
      <source>Block Elements</source>
      <translation>Lohko-osat</translation>
    </message>
    <message>
      <source>Geometric Shapes</source>
      <translation>Geometriset muodot</translation>
    </message>
    <message>
      <source>Miscellaneous Symbols</source>
      <translation>Sekalaiset symbolit</translation>
    </message>
    <message>
      <source>Dingbats</source>
      <translation>Dingbats</translation>
    </message>
    <message>
      <source>Small Form Variants</source>
      <translation>Pienikokoisia muunnelmia</translation>
    </message>
    <message>
      <source>Ligatures</source>
      <translation>Ligatuurit</translation>
    </message>
    <message>
      <source>Specials</source>
      <translation>Erikoismerkit</translation>
    </message>
    <message>
      <source>Greek</source>
      <translation>Kreikkalaiset merkit</translation>
    </message>
    <message>
      <source>Greek Extended</source>
      <translation>Kreikkalaiset merkit (laajennus)</translation>
    </message>
    <message>
      <source>Cyrillic</source>
      <translation>Kyrilliset merkit</translation>
    </message>
    <message>
      <source>Cyrillic Supplement</source>
      <translation>Kyrilliset merkit (laajennus)</translation>
    </message>
    <message>
      <source>Arabic</source>
      <translation>Arabialaiset merkit</translation>
    </message>
    <message>
      <source>Arabic Extended A</source>
      <translation>Arabialaiset merkit laajennus A</translation>
    </message>
    <message>
      <source>Arabic Extended B</source>
      <translation>Arabialaiset merkit laajennus B</translation>
    </message>
    <message>
      <source>Hebrew</source>
      <translation>Heprealaiset merkit</translation>
    </message>
  </context>
  <context>
    <name>CheckDocument</name>
    <message>
      <source>Glyphs missing</source>
      <translation>Puuttuvat merkit</translation>
    </message>
    <message>
      <source>Text overflow</source>
      <translation>Teksti ylivuotaa</translation>
    </message>
    <message>
      <source>Object is not on a Page</source>
      <translation>Kohde ei ole sivulla</translation>
    </message>
    <message>
      <source>Missing Image</source>
      <translation>Puuttuva kuva</translation>
    </message>
    <message>
      <source>Image has a DPI-Value less than %1 DPI</source>
      <translation>Kuvan DPI-arvo on vähemmän kuin %1</translation>
    </message>
    <message>
      <source>Object has transparency</source>
      <translation>Kohteessa on käytetty läpinäkyvyyttä</translation>
    </message>
    <message>
      <source>Object is a PDF Annotation or Field</source>
      <translation>Kohde on PDF-merkintä tai -kenttä</translation>
    </message>
    <message>
      <source>Object is a placed PDF</source>
      <translation>Kohde on sijoitettu PDF</translation>
    </message>
    <message>
      <source>Document</source>
      <translation>Asiakirja</translation>
    </message>
    <message>
      <source>No Problems found</source>
      <translation>Ongelmia ei löytynyt</translation>
    </message>
    <message>
      <source>Page </source>
      <translation>Sivu</translation>
    </message>
    <message>
      <source>Free Objects</source>
      <translation>Vapaat kohteet</translation>
    </message>
    <message>
      <source>Problems found</source>
      <translation>Ongelmia löytyi</translation>
    </message>
    <message>
      <source>Preflight Verifier</source>
      <translation>Tarkastustyökalu</translation>
    </message>
    <message>
      <source>Items</source>
      <translation>Kohteet</translation>
    </message>
    <message>
      <source>Problems</source>
      <translation>Ongelmat</translation>
    </message>
    <message>
      <source>Current Profile:</source>
      <translation>Valittu profiili:</translation>
    </message>
    <message>
      <source>&amp;Ignore Errors</source>
      <translation>Ohita virheet</translation>
    </message>
  </context>
  <context>
    <name>ChooseStyles</name>
    <message>
      <source>Choose Styles</source>
      <translation>Valitse tyylit</translation>
    </message>
    <message>
      <source>Available Styles</source>
      <translation>Käytettävissä olevat tyylit</translation>
    </message>
  </context>
  <context>
    <name>CollectForOutput</name>
    <message>
      <source>Choose a Directory</source>
      <translation>Valitse hakemisto</translation>
    </message>
    <message>
      <source>Collecting...</source>
      <translation>Kerätään...</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Varoitus</translation>
    </message>
    <message>
      <source>Cannot collect all files for output for file:
%1</source>
      <translation>Ei pysty keräämään kaikkia tiedostoja:\n
%1</translation>
    </message>
    <message>
      <source>Cannot collect the file: 
%1</source>
      <translation>Ei voi kerätä tiedostoa:\n
%1</translation>
    </message>
  </context>
  <context>
    <name>ColorListBox</name>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
  </context>
  <context>
    <name>ColorManager</name>
    <message>
      <source>Colors</source>
      <translation>Värit</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>T&amp;uo</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;Uusi</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Muokkaa</translation>
    </message>
    <message>
      <source>D&amp;uplicate</source>
      <translation>&amp;Kopioi</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Poista</translation>
    </message>
    <message>
      <source>&amp;Remove Unused</source>
      <translation>Poista &amp;käyttämättömät</translation>
    </message>
    <message>
      <source>Color Sets</source>
      <translation>Värisarjat</translation>
    </message>
    <message>
      <source>Current Color Set:</source>
      <translation>Nykyinen värisarja:</translation>
    </message>
    <message>
      <source>&amp;Save Color Set</source>
      <translation>Tallenna &amp;värisarja</translation>
    </message>
    <message>
      <source>Choose a color set to load</source>
      <translation>Valitse ladattava värisarja</translation>
    </message>
    <message>
      <source>Save the current color set</source>
      <translation>Tallenna nykyinen värisarja</translation>
    </message>
    <message>
      <source>Remove unused colors from current document's color set</source>
      <translation>Poista käyttämättömät värit asiakirjan värisarjasta</translation>
    </message>
    <message>
      <source>Import colors to the current set from an existing document</source>
      <translation>Tuo värejä nykyiseen kokoelmaan asiakirjasta</translation>
    </message>
    <message>
      <source>Create a new color within the current set</source>
      <translation>Luo uusi väri nykyiseen värisarjaan</translation>
    </message>
    <message>
      <source>Edit the currently selected color</source>
      <translation>Muokkaa valittua väriä</translation>
    </message>
    <message>
      <source>Make a copy of the currently selected color</source>
      <translation>Kopioi valittu väri</translation>
    </message>
    <message>
      <source>Delete the currently selected color</source>
      <translation>Poista valittu väri</translation>
    </message>
    <message>
      <source>Make the current colorset the default color set</source>
      <translation>Tee valitusta värisarjasta oletussarja</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation>&amp;Nimi:</translation>
    </message>
    <message>
      <source>Choose a Name</source>
      <translation>Valitse nimi</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Avaa</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
      <translation>Asiakirjat (*.sla *.sla.gz *.scd *.scd.gz);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Documents (*.sla *.scd);;All Files (*)</source>
      <translation>Asiakirjat (*.sla *.scd);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Copy of %1</source>
      <translation>Kopio-%1</translation>
    </message>
    <message>
      <source>New Color</source>
      <translation>Uusi väri</translation>
    </message>
  </context>
  <context>
    <name>ColorWheel</name>
    <message>
      <source>Monochromatic</source>
      <translation>Monokromaattinen</translation>
    </message>
    <message>
      <source>Analogous</source>
      <translation>Analoginen</translation>
    </message>
    <message>
      <source>Complementary</source>
      <translation>Komplementaarinen</translation>
    </message>
    <message>
      <source>Split Complementary</source>
      <translation>Jaettu komplimentaarinen</translation>
    </message>
    <message>
      <source>Triadic</source>
      <translation>Triadinen</translation>
    </message>
    <message>
      <source>Tetradic (Double Complementary)</source>
      <translation>Tetradic (tuplakomplementaarinen)</translation>
    </message>
    <message>
      <source>Base Color</source>
      <translation>Perusväri</translation>
    </message>
    <message>
      <source>Monochromatic Light</source>
      <translation>Monokromaattinen vaalea</translation>
    </message>
    <message>
      <source>Monochromatic Dark</source>
      <translation>Monokromaattinen tumma</translation>
    </message>
    <message>
      <source>1st. Analogous</source>
      <translation>1. analoginen</translation>
    </message>
    <message>
      <source>2nd. Analogous</source>
      <translation>2. analoginen</translation>
    </message>
    <message>
      <source>1st. Split</source>
      <translation>1. jaettu</translation>
    </message>
    <message>
      <source>2nd. Split</source>
      <translation>2. jaettu</translation>
    </message>
    <message>
      <source>3rd. Split</source>
      <translation>3. jaettu</translation>
    </message>
    <message>
      <source>4th. Split</source>
      <translation>4. jaettu</translation>
    </message>
    <message>
      <source>1st. Triadic</source>
      <translation>1. Triadinen</translation>
    </message>
    <message>
      <source>2nd. Triadic</source>
      <translation>2. Triadinen</translation>
    </message>
    <message>
      <source>1st. Tetradic (base opposite)</source>
      <translation>1. Tetradic (perus vastakohta)</translation>
    </message>
    <message>
      <source>2nd. Tetradic (angle)</source>
      <translation>2. Tetradic (kulma)</translation>
    </message>
    <message>
      <source>3rd. Tetradic (angle opposite)</source>
      <translation>3. Tetradic (kulma vastakohta)</translation>
    </message>
  </context>
  <context>
    <name>ColorWheelDialog</name>
    <message>
      <source>Color Wheel</source>
      <translation>Väripyörä</translation>
    </message>
    <message>
      <source>Color</source>
      <translation>Väri</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Nimi</translation>
    </message>
    <message>
      <source>C</source>
      <translation>C</translation>
    </message>
    <message>
      <source>M</source>
      <translation>M</translation>
    </message>
    <message>
      <source>Y</source>
      <translation>Y</translation>
    </message>
    <message>
      <source>K</source>
      <translation>K</translation>
    </message>
    <message>
      <source>Select Method:</source>
      <translation>Valintamenetelmä:</translation>
    </message>
    <message>
      <source>Angle (0 - 90 degrees):</source>
      <translation>Kulma (0-90 astetta):</translation>
    </message>
    <message>
      <source>&amp;Merge Colors</source>
      <translation>&amp;Lisää värit</translation>
    </message>
    <message>
      <source>&amp;Replace Colors</source>
      <translation>&amp;Korvaa värit</translation>
    </message>
    <message>
      <source>Merge created colors into the document colors</source>
      <translation>Lisää luodut värit asiakirjan väreihin</translation>
    </message>
    <message>
      <source>Replace created colors in the document colors</source>
      <translation>Korvaa luodut värit asiakirjan väreissä</translation>
    </message>
    <message>
      <source>Leave colors untouched</source>
      <translation>Älä tee muutoksia väreihin</translation>
    </message>
    <message>
      <source>Merging colors</source>
      <translation>Lisätään värejä</translation>
    </message>
    <message>
      <source>Error: </source>
      <translation>Virhe: </translation>
    </message>
    <message>
      <source>Now opening the color manager.</source>
      <translation>Avataan värien hallinta.</translation>
    </message>
    <message>
      <source>Color Merging</source>
      <translation>Värien lisäys</translation>
    </message>
    <message>
      <source>Cr&amp;eate color...</source>
      <translation>&amp;Luo väri...</translation>
    </message>
    <message>
      <source>C&amp;olor Components...</source>
      <translation>&amp;Värien komponentit...</translation>
    </message>
    <message>
      <source>&amp;Import existing color...</source>
      <translation>&amp;Tuo värejä...</translation>
    </message>
    <message>
      <source>&amp;Merge colors</source>
      <translation>&amp;Lisää värejä</translation>
    </message>
    <message>
      <source>&amp;Replace colors</source>
      <translation>&amp;Korvaa värejä</translation>
    </message>
    <message>
      <source>E&amp;xit</source>
      <translation>&amp;Poistu</translation>
    </message>
    <message>
      <source>C&amp;olor</source>
      <translation>&amp;Väri</translation>
    </message>
    <message>
      <source>Normal Vision</source>
      <translation>Normaali näkö</translation>
    </message>
    <message>
      <source>Protanopia (Red)</source>
      <translation>Punasokeus</translation>
    </message>
    <message>
      <source>Deuteranopia (Green)</source>
      <translation>Vihersokeus</translation>
    </message>
    <message>
      <source>Tritanopia (Blue)</source>
      <translation>Sinisokeus</translation>
    </message>
    <message>
      <source>Full Color Blindness</source>
      <translation>Täysi värisokeus</translation>
    </message>
    <message>
      <source>Vision Defect:</source>
      <translation>Näön vääristymä:</translation>
    </message>
    <message>
      <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
      <translation>Erotus valitun arvon ja laskettujen välillä. Lisää tietoa löytyy dokumentaatiosta.</translation>
    </message>
    <message>
      <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
      <translation>Valitse hiirellä perusväri väripyörästä. HSV-tilassa se on sävy.</translation>
    </message>
    <message>
      <source>Sample color scheme</source>
      <translation>Värikarttaesimerkki</translation>
    </message>
    <message>
      <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
      <translation>Valitse jokin tyyli luoda väriteema. Lisää tietoa löytyy dokumentaatiosta.</translation>
    </message>
    <message>
      <source>Colors of your chosen color scheme</source>
      <translation>Valitun värikartan värit</translation>
    </message>
    <message>
      <source>Simulate common vision defects here. Select type of the defect.</source>
      <translation>Mallinna yleisiä näön rajoituksia. Valitse rajoituksen tyyppi.</translation>
    </message>
    <message>
      <source>Color %1 exists already!</source>
      <translation>Väri %1 on jo olemassa</translation>
    </message>
    <message>
      <source>Color %1 appended.</source>
      <translation>Väri %1 lisättiin.</translation>
    </message>
    <message>
      <source>New Color</source>
      <translation>Uusi väri</translation>
    </message>
    <message>
      <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
      <translation>Haluttua väriä ei löydy. Olet luultavasti valinnut mustan, harmaan tai valkoisen. Tätä väriä ei voida prosessoida.</translation>
    </message>
  </context>
  <context>
    <name>ColorWheelPlugin</name>
    <message>
      <source>&amp;Color Wheel...</source>
      <translation>&amp;Väripyörä...</translation>
    </message>
    <message>
      <source>Color setting helper</source>
      <translation>Väriasetusavustaja</translation>
    </message>
    <message>
      <source>Color selector with color theory included.</source>
      <translation>Värivalitsin väriteorialla</translation>
    </message>
  </context>
  <context>
    <name>CommonStrings</name>
    <message>
      <source>&amp;Apply</source>
      <translation>&amp;Käytä</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Peruuta</translation>
    </message>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Tallenna</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Varoitus</translation>
    </message>
  </context>
  <context>
    <name>Cpalette</name>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>None</source>
      <translation>Ei väriä</translation>
    </message>
    <message>
      <source>Radial Gradient</source>
      <translation>Säteittäinen väriliuku</translation>
    </message>
    <message>
      <source>Horizontal Gradient</source>
      <translation>Vaakasuora väriliuku</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>Tasainen</translation>
    </message>
    <message>
      <source>Opacity:</source>
      <translation>Läpinäkyvyys:</translation>
    </message>
    <message>
      <source>Vertical Gradient</source>
      <translation>Pystyväriliuku</translation>
    </message>
    <message>
      <source>Cross Diagonal Gradient</source>
      <translation>Lävistäjän (oikealta vasemmalle) suuntainen väriliuku</translation>
    </message>
    <message>
      <source>Diagonal Gradient</source>
      <translation>Lävistäjän (vasemmalta oikealle) suuntainen väriliuku</translation>
    </message>
    <message>
      <source>Shade:</source>
      <translation>Sävy:</translation>
    </message>
    <message>
      <source>Edit Line Color Properties</source>
      <translation>Muokkaa viivan väriä</translation>
    </message>
    <message>
      <source>Edit Fill Color Properties</source>
      <translation>Muokkaa täyttöväriä</translation>
    </message>
    <message>
      <source>Saturation of color</source>
      <translation>Värin kylläisyys</translation>
    </message>
    <message>
      <source>Normal or gradient fill method</source>
      <translation>Normaali täyttö tai väriliuku</translation>
    </message>
    <message>
      <source>Set the transparency for the color selected</source>
      <translation>Aseta valitun värin läpinäkyvyys</translation>
    </message>
    <message>
      <source>Free linear Gradient</source>
      <translation>Vapaa lineaarinen väriliuku</translation>
    </message>
    <message>
      <source>X1:</source>
      <translation>X1:</translation>
    </message>
    <message>
      <source>Y1:</source>
      <translation>Y1:</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>X2:</source>
      <translation>X2:</translation>
    </message>
    <message>
      <source>Y2:</source>
      <translation>Y2:</translation>
    </message>
    <message>
      <source>Free radial Gradient</source>
      <translation>Vapaa säteittäinen väriliuku</translation>
    </message>
    <message>
      <source>Move Vector</source>
      <translation>Siirrä vektoria</translation>
    </message>
    <message>
      <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
      <translation>Siirrä liukuvärin alkuvektoria hiiren vasen painike painettuna ja loppuvektoria oikea painike painettuna</translation>
    </message>
  </context>
  <context>
    <name>CsvDialog</name>
    <message>
      <source>CSV Importer Options</source>
      <translation>CSV-tuojan ominaisuudet</translation>
    </message>
    <message>
      <source>Field delimiter:</source>
      <translation>Kentän erotin:</translation>
    </message>
    <message>
      <source>(TAB)</source>
      <translation>(SARKAIN)</translation>
    </message>
    <message>
      <source>Value delimiter:</source>
      <translation>Arvon erotin:</translation>
    </message>
    <message>
      <source>First row is a header</source>
      <translation>Ensimmäinen rivi on otsikko</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Peruuta</translation>
    </message>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
  </context>
  <context>
    <name>CupsOptions</name>
    <message>
      <source>No</source>
      <translation>Ei</translation>
    </message>
    <message>
      <source>Yes</source>
      <translation>Kyllä</translation>
    </message>
    <message>
      <source>Even Pages only</source>
      <translation>Vain parilliset sivut</translation>
    </message>
    <message>
      <source>Value</source>
      <translation>Arvo</translation>
    </message>
    <message>
      <source>Page per Sheet</source>
      <translation>Sivu arkille</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>Vaakasuora</translation>
    </message>
    <message>
      <source>Mirror</source>
      <translation>Peili</translation>
    </message>
    <message>
      <source>Option</source>
      <translation>Asetus</translation>
    </message>
    <message>
      <source>Orientation</source>
      <translation>Suunta</translation>
    </message>
    <message>
      <source>Printer Options</source>
      <translation>Tulostimen asetukset</translation>
    </message>
    <message>
      <source>Odd Pages only</source>
      <translation>Vain parittomat sivut</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>Pystysuora</translation>
    </message>
    <message>
      <source>Page Set</source>
      <translation>Sivukokonaisuus</translation>
    </message>
    <message>
      <source>Pages per Sheet</source>
      <translation>Sivua arkille</translation>
    </message>
    <message>
      <source>N-Up Printing</source>
      <translation>Tulosta useita sivuja arkille</translation>
    </message>
    <message>
      <source>All Pages</source>
      <translation>Kaikki sivut</translation>
    </message>
    <message>
      <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help > About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
      <translation>Tämä paneeli näyttää CUPS-asetukset tulostettaessa. Näytettävät ominaisuudet riippuvat tulostimesi ajurista. CUPS-tuen voi varmistaa menemällä kohtaan Ohje>Tietoja Scribuksesta ja tarkastamalla kirjastotuki merkeistä C-C-T; C=CUPS, C=LittleCMS ja T=TIFF. Puuttuva tuki on merkitty * -merkillä.</translation>
    </message>
  </context>
  <context>
    <name>CustomFDialog</name>
    <message>
      <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
      <translation>Siirry asiakirjahakemistoosi. 
Voit määrittää sen asetuksissa.</translation>
    </message>
    <message>
      <source>Encoding:</source>
      <translation>Koodaus:</translation>
    </message>
    <message>
      <source>&amp;Compress File</source>
      <translation>&amp;Pakkaa tiedosto</translation>
    </message>
    <message>
      <source>&amp;Include Fonts</source>
      <translation>&amp;Sisällytä kirjaintyypit</translation>
    </message>
  </context>
  <context>
    <name>CwSetColor</name>
    <message>
      <source>Set Color Components</source>
      <translation>Aseta osavärit</translation>
    </message>
    <message>
      <source>CMYK</source>
      <translation>CMYK</translation>
    </message>
    <message>
      <source>RGB</source>
      <translation>RGB</translation>
    </message>
    <message>
      <source>HSV</source>
      <translation>HSV</translation>
    </message>
    <message>
      <source>H:</source>
      <translation>H:</translation>
    </message>
    <message>
      <source>S:</source>
      <translation>S:</translation>
    </message>
    <message>
      <source>V:</source>
      <translation>V:</translation>
    </message>
    <message>
      <source>R:</source>
      <translation>R:</translation>
    </message>
    <message>
      <source>G:</source>
      <translation>G:</translation>
    </message>
    <message>
      <source>B:</source>
      <translation>B:</translation>
    </message>
    <message>
      <source>C:</source>
      <translation>C:</translation>
    </message>
    <message>
      <source>M:</source>
      <translation>M:</translation>
    </message>
    <message>
      <source>Y:</source>
      <translation>Y:</translation>
    </message>
    <message>
      <source>K:</source>
      <translation>K:</translation>
    </message>
    <message>
      <source>Set &amp;RGB</source>
      <translation>Aseta &amp;RGB</translation>
    </message>
    <message>
      <source>Set C&amp;MYK</source>
      <translation>Aseta &amp;CMYK</translation>
    </message>
    <message>
      <source>Set &amp;HSV</source>
      <translation>Aseta &amp;HSV</translation>
    </message>
  </context>
  <context>
    <name>DeferredTask</name>
    <message>
      <source>Cancelled by user</source>
      <translation>Käyttäjän peruuttama</translation>
    </message>
  </context>
  <context>
    <name>DelColor</name>
    <message>
      <source>Delete Color</source>
      <translation>Poista väri</translation>
    </message>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Delete Color:</source>
      <translation>Poista väri:</translation>
    </message>
    <message>
      <source>Replace With:</source>
      <translation>Korvaa värillä:</translation>
    </message>
  </context>
  <context>
    <name>DelPages</name>
    <message>
      <source>to:</source>
      <translation>minne:</translation>
    </message>
    <message>
      <source>Delete Pages</source>
      <translation>Poista sivut</translation>
    </message>
    <message>
      <source>Delete From:</source>
      <translation>Mistä poistetaan:</translation>
    </message>
  </context>
  <context>
    <name>DelStyle</name>
    <message>
      <source>Delete Style</source>
      <translation>Poista tyyli</translation>
    </message>
    <message>
      <source>Delete Style:</source>
      <translation>Poista tyyli:</translation>
    </message>
    <message>
      <source>Replace With:</source>
      <translation>Millä korvataan:</translation>
    </message>
    <message>
      <source>No Style</source>
      <translation>Ei tyyliä</translation>
    </message>
  </context>
  <context>
    <name>DocInfos</name>
    <message>
      <source>Document Information</source>
      <translation>Asiakirjan tiedot</translation>
    </message>
    <message>
      <source>&amp;Title:</source>
      <translation>O&amp;tsikko:</translation>
    </message>
    <message>
      <source>&amp;Author:</source>
      <translation>&amp;Tekijä:</translation>
    </message>
    <message>
      <source>&amp;Keywords:</source>
      <translation>A&amp;vainsanat:</translation>
    </message>
    <message>
      <source>Descri&amp;ption:</source>
      <translation>Kuvau&amp;s:</translation>
    </message>
    <message>
      <source>P&amp;ublisher:</source>
      <translation>&amp;Julkaisija:</translation>
    </message>
    <message>
      <source>&amp;Contributors:</source>
      <translation>Av&amp;ustajat:</translation>
    </message>
    <message>
      <source>Dat&amp;e:</source>
      <translation>&amp;Päivämäärä:</translation>
    </message>
    <message>
      <source>T&amp;ype:</source>
      <translation>&amp;Tyyppi:</translation>
    </message>
    <message>
      <source>F&amp;ormat:</source>
      <translation>&amp;Muoto:</translation>
    </message>
    <message>
      <source>Identi&amp;fier:</source>
      <translation>&amp;Tunniste:</translation>
    </message>
    <message>
      <source>&amp;Source:</source>
      <translation>L&amp;ähde:</translation>
    </message>
    <message>
      <source>&amp;Language:</source>
      <translation>&amp;Kieli:</translation>
    </message>
    <message>
      <source>&amp;Relation:</source>
      <translation>&amp;Suhde:</translation>
    </message>
    <message>
      <source>Co&amp;verage:</source>
      <translation>&amp;Kattavuus:</translation>
    </message>
    <message>
      <source>Ri&amp;ghts:</source>
      <translation>O&amp;ikeudet:</translation>
    </message>
    <message>
      <source>Further &amp;Information</source>
      <translation>&amp;Lisätiedot</translation>
    </message>
    <message>
      <source>A person or organisation responsible for making the document available</source>
      <translation>Henkilö tai järjestö, joka on vastuussa asiakirjan jakelusta</translation>
    </message>
    <message>
      <source>A person or organisation responsible for making contributions to the content of the document</source>
      <translation>Henkilö tai järjestö, joka on avustanut asiakirjan sisällön luonnissa</translation>
    </message>
    <message>
      <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
      <translation>Päivämäärä, joka on yhteydessä asiakirjan elinkaareen. VVVV-KK-PP ISO 9601</translation>
    </message>
    <message>
      <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
      <translation>Asiakirjan lajityyppi tai luonne. Esimerkiksi: kategoriat, käyttötarkoitukset</translation>
    </message>
    <message>
      <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
      <translation>Yksiselitteinen tunniste asiakirjaan kuten ISBN tai URI</translation>
    </message>
    <message>
      <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
      <translation>Viittaus yhteenkuuluvaan asiakirjaan mahdollisesti käyttäen yksiselitteistä tunnistetta kuten ISBN tai URI</translation>
    </message>
    <message>
      <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
      <translation>Asiakirjan sisällön laajuus sisältäen mahdollisesti paikan, ajan ja lainsäädölliset asiat</translation>
    </message>
    <message>
      <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
      <translation>Tietoa oikeuksista jotka liittyvät asiakirjaan, esimerkiksi tekijänoikeus, patentti tai tavaramerkki</translation>
    </message>
    <message>
      <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
      <translation>Viittaus asiakirjaan josta nykyinen asiakirja on johdettu, esim. SBN tai URI</translation>
    </message>
    <message>
      <source>Documen&amp;t</source>
      <translation>&amp;Asiakirja</translation>
    </message>
    <message>
      <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
      <translation>Henkilö tai järjestö, joka on vastuussa asiakirjan sisällöstä. Tämä kenttä voidaan upottaa Scribus-asiakirjaan sekä PDF-metadataksi.</translation>
    </message>
    <message>
      <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
      <translation>Asiakirjan nimi. Tämä kenttä voidaan upottaa Scribus-asiakirjaan sekä PDF-metadataksi.</translation>
    </message>
    <message>
      <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
      <translation>Selostus asiakirjan sisällöstä. Tämä kenttä voidaan upottaa Scribus-asiakirjaan sekä PDF-metadataksi.</translation>
    </message>
    <message>
      <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
      <translation>Aihe, jota asiakirjan sisältö käsittelee. Tämä kenttä on avainsanoja varten, jotka haluat upottaa PDF-tiedostoon avustamaan arkistointia ja etsimistä.</translation>
    </message>
    <message>
      <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
      <translation>Asiakirjan fyysinen tai digitaalinen olemus. Koko ja tyyppi on hyvä mainita. RFC2045, RFC2046 MIME-tyypeille saattaa olla hyödyllinen tässä.</translation>
    </message>
    <message>
      <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
      <translation>Asiakirjan sisällön kieli. Yleensä käytetään standardissa ISO-639 määriteltyjä kieliä laajennettuna standardissa ISO-3166 määritellyllä maakoodilla esim. fi-FI, en-GB.</translation>
    </message>
  </context>
  <context>
    <name>DocSections</name>
    <message>
      <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
      <translation>Lisää sivunumeroinnin osa asiakirjaan. Uusi osa lisätään tällä hetkellä valittuna olevan osan jälkeen.</translation>
    </message>
    <message>
      <source>Delete the currently selected section.</source>
      <translation>Poista tällä hetkellä valittuna oleva osa.</translation>
    </message>
    <message>
      <source>1, 2, 3, ...</source>
      <translation>1, 2, 3, ...</translation>
    </message>
    <message>
      <source>i, ii, iii, ...</source>
      <translation>i, ii, iii, ...</translation>
    </message>
    <message>
      <source>I, II, III, ...</source>
      <translation>I, II, III, ...</translation>
    </message>
    <message>
      <source>a, b, c, ...</source>
      <translation>a, b, c, ...</translation>
    </message>
    <message>
      <source>A, B, C, ...</source>
      <translation>A, B, C, ...</translation>
    </message>
    <message>
      <source>Name: Optional name for section eg. Index&lt;br/>Shown: Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/>From: The page index for this section to start at.&lt;br/>To: The page index for this section to stop at.&lt;br/>Style: Select the page number style to be used.&lt;br/>Start: The index within the Style's range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b.</source>
      <translation>Nimi: Valinnainen nimi alueelle&lt;br/>Näytä: Näytä sivunumero tällä alueella, jos yksi tai useampi tekstikehys on asetettu sivunumeroa varten.&lt;br/>Mistä: sivunumero, josta tämä alue alkaa (sivunumero on sivun järjestysnumero siten, että ensimmäinen sivu on 0)</translation>
    </message>
    <message>
      <source>Page Number Out Of Bounds</source>
      <translation>Sivunumero sallitun alueen ulkopuolella</translation>
    </message>
    <message>
      <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
      <translation>Antamasi sivunumero on asiakirjan sallittujen arvojen ulkopuolella (%1-%2).</translation>
    </message>
  </context>
  <context>
    <name>DocSectionsBase</name>
    <message>
      <source>Document Sections</source>
      <translation>Asiakirjan osat</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Nimi</translation>
    </message>
    <message>
      <source>From</source>
      <translation>Mistä</translation>
    </message>
    <message>
      <source>To</source>
      <translation>Minne</translation>
    </message>
    <message>
      <source>Style</source>
      <translation>Tyyli</translation>
    </message>
    <message>
      <source>Start</source>
      <translation>Aloitus</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation>&amp;Lisää</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Poista</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>Shown</source>
      <translation>Näytä</translation>
    </message>
  </context>
  <context>
    <name>DocumentItemAttributes</name>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Relates To</source>
      <translation>Suhteessa</translation>
    </message>
    <message>
      <source>Is Parent Of</source>
      <translation>On vanhempi</translation>
    </message>
    <message>
      <source>Is Child Of</source>
      <translation>On lapsi</translation>
    </message>
    <message>
      <source>Text Frames</source>
      <translation>Tekstikehykset</translation>
    </message>
    <message>
      <source>Image Frames</source>
      <translation>Kuvakehykset</translation>
    </message>
    <message>
      <source>Boolean</source>
      <translation>boolean</translation>
    </message>
    <message>
      <source>Integer</source>
      <translation>kokonaisluku</translation>
    </message>
    <message>
      <source>String</source>
      <translation>merkkijono</translation>
    </message>
    <message>
      <source>Real Number</source>
      <translation>reaaliluku</translation>
    </message>
  </context>
  <context>
    <name>DocumentItemAttributesBase</name>
    <message>
      <source>Document Item Attributes</source>
      <translation>Kohteen muuttujat</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Nimi</translation>
    </message>
    <message>
      <source>Type</source>
      <translation>Tyyppi</translation>
    </message>
    <message>
      <source>Value</source>
      <translation>Arvo</translation>
    </message>
    <message>
      <source>Parameter</source>
      <translation>Parametri</translation>
    </message>
    <message>
      <source>Relationship</source>
      <translation>Suhde</translation>
    </message>
    <message>
      <source>Relationship To</source>
      <translation>Suhteessa</translation>
    </message>
    <message>
      <source>Auto Add To</source>
      <translation>Lisää automaattisesti</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation>&amp;Lisää</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>&amp;Kopioi</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Poista</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>T&amp;yhjennä</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation>Alt+L</translation>
    </message>
  </context>
  <context>
    <name>Druck</name>
    <message>
      <source>All</source>
      <translation>Kaikki</translation>
    </message>
    <message>
      <source>File</source>
      <translation>Tiedosto</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>Asetukset</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>Tallenna nimellä</translation>
    </message>
    <message>
      <source>Setup Printer</source>
      <translation>Aseta tulostin</translation>
    </message>
    <message>
      <source>Cyan</source>
      <translation>Syaani</translation>
    </message>
    <message>
      <source>Magenta</source>
      <translation>Magenta</translation>
    </message>
    <message>
      <source>Yellow</source>
      <translation>Keltainen</translation>
    </message>
    <message>
      <source>Black</source>
      <translation>Musta</translation>
    </message>
    <message>
      <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
      <translation>Syötä pilkuin eroteltu lista sivuista. Pelkän 
sivunumeron lisäksi voit käyttää merkkiä * 
kuvaamaan kaikkia sivuja tai 1-5 sivuväliä.</translation>
    </message>
    <message>
      <source>Print Destination</source>
      <translation>Tulosteen kohde</translation>
    </message>
    <message>
      <source>&amp;Options...</source>
      <translation>&amp;Asetukset...</translation>
    </message>
    <message>
      <source>&amp;File:</source>
      <translation>&amp;Tiedosto:</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>&amp;Muuta...</translation>
    </message>
    <message>
      <source>A&amp;lternative Printer Command</source>
      <translation>Oma tulo&amp;stuskomento</translation>
    </message>
    <message>
      <source>Co&amp;mmand:</source>
      <translation>&amp;Komento:</translation>
    </message>
    <message>
      <source>Range</source>
      <translation>Väli</translation>
    </message>
    <message>
      <source>Print &amp;All</source>
      <translation>Tulosta &amp;kaikki</translation>
    </message>
    <message>
      <source>Print Current Pa&amp;ge</source>
      <translation>Tulosta n&amp;ykyinen sivu</translation>
    </message>
    <message>
      <source>Print &amp;Range</source>
      <translation>Tulosta &amp;väli</translation>
    </message>
    <message>
      <source>N&amp;umber of Copies:</source>
      <translation>K&amp;opioiden määrä:</translation>
    </message>
    <message>
      <source>&amp;Print</source>
      <translation>T&amp;ulosta</translation>
    </message>
    <message>
      <source>Print Normal</source>
      <translation>Normaali tulostus</translation>
    </message>
    <message>
      <source>Print Separations</source>
      <translation>Tulosta erottelu</translation>
    </message>
    <message>
      <source>Print in Color if Available</source>
      <translation>Väritulostus, jos mahdollista</translation>
    </message>
    <message>
      <source>Print in Grayscale</source>
      <translation>Harmaasävytulostus</translation>
    </message>
    <message>
      <source>PostScript Level 1</source>
      <translation>PostScript taso 1</translation>
    </message>
    <message>
      <source>PostScript Level 2</source>
      <translation>PostScript taso 2</translation>
    </message>
    <message>
      <source>PostScript Level 3</source>
      <translation>PostScript taso 3</translation>
    </message>
    <message>
      <source>Page</source>
      <translation>Sivu</translation>
    </message>
    <message>
      <source>Mirror Page(s) Horizontal</source>
      <translation>Peilaa sivut vaakasuunnassa</translation>
    </message>
    <message>
      <source>Mirror Page(s) Vertical</source>
      <translation>Peilaa sivut pystysuunnassa</translation>
    </message>
    <message>
      <source>Set Media Size</source>
      <translation>Aseta kohteen koko</translation>
    </message>
    <message>
      <source>Color</source>
      <translation>Väri</translation>
    </message>
    <message>
      <source>Apply Under Color Removal</source>
      <translation>Käytä alivärinpoistoa (UCR)</translation>
    </message>
    <message>
      <source>Convert Spot Colors to Process Colors</source>
      <translation>Muunna spottivärit prosessiväreiksi</translation>
    </message>
    <message>
      <source>Apply ICC Profiles</source>
      <translation>Käytä ICC-profiileja</translation>
    </message>
    <message>
      <source>Advanced Options</source>
      <translation>Lisäasetukset</translation>
    </message>
    <message>
      <source>Preview...</source>
      <translation>Esikatselu...</translation>
    </message>
    <message>
      <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
      <translation>Käytä vaihtoehtoista tulostuksen hallintaa kuten kprinter tai gtklp</translation>
    </message>
    <message>
      <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
      <translation>Asettaa PostScript-tason. 
Tasot 1 ja 2 saattavat luoda suuria tiedostoja</translation>
    </message>
    <message>
      <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
      <translation>Tapa vaihtaa C, Y ja M värien tekemä harmaa mustalla tehdyksi. 
UCR vaikuttaa kuvien kohdissa, jotka ovat neutraaleja tai lähellä harmaata 
olevia tummia värejä. UCR:n käyttäminen saattaa parantaa joidenkin kuvien
tulostamista. UCR parantaa kykyä välttää värien liikakylläisyyttä CMY-musteilla. 
Parhaan tuloksen saavuttamiseksi tarvitaan tapauskohtaista testausta. UCR 
vähentää ylikylläisyyden vaaraa CMY-musteilla.</translation>
    </message>
    <message>
      <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
      <translation>Mahdollistaa spottivärien muuntamisen prosessiväreiksi. Jos et aio painaa käyttäen spottivärejä, tämä on paras jättää valituksi.</translation>
    </message>
    <message>
      <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
      <translation>Mahdollistaa ICC-profiilien upottamisen tulosteeseen värien hallinnan ollessa käytössä</translation>
    </message>
    <message>
      <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
      <translation>Aseta PostScript-tiedoston median koko eksplisiittisesti. Käyttö on suositeltavaa ainoastaan jos paino pyysi sitä.</translation>
    </message>
    <message>
      <source>PostScript Files (*.ps);;All Files (*)</source>
      <translation>PostScript-tiedostot (*.ps);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Failed to retrieve printer settings</source>
      <translation>Tulostimen asetusten hakeminen epäonnistui</translation>
    </message>
  </context>
  <context>
    <name>EPSPlug</name>
    <message>
      <source>Importing File:
%1
failed!</source>
      <translation>Tiedoston %1 tuonti epäonnistui!</translation>
    </message>
    <message>
      <source>Fatal Error</source>
      <translation>Vakava virhe</translation>
    </message>
  </context>
  <context>
    <name>EditStyle</name>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>Edit Style</source>
      <translation>Muokkaa tyyliä</translation>
    </message>
    <message>
      <source>Character</source>
      <translation>Teksti</translation>
    </message>
    <message>
      <source>Line Spacing</source>
      <translation>Riviväli</translation>
    </message>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Name of your paragraph style</source>
      <translation>Kappaletyylisi nimi</translation>
    </message>
    <message>
      <source>Font of selected text or object</source>
      <translation>Valitun tekstin tai kohteen kirjaintyyppi</translation>
    </message>
    <message>
      <source>Font Size</source>
      <translation>Kirjaintyypin koko</translation>
    </message>
    <message>
      <source>Color of text fill</source>
      <translation>Tekstin täytön väri</translation>
    </message>
    <message>
      <source>Color of text stroke</source>
      <translation>Tekstin reunan väri</translation>
    </message>
    <message>
      <source>Determines the overall height, in line numbers, of the Drop Caps</source>
      <translation>Anfangin korkeus riveinä</translation>
    </message>
    <message>
      <source>Spacing above the paragraph</source>
      <translation>Tila kappaleen yläpuolella</translation>
    </message>
    <message>
      <source>Spacing below the paragraph</source>
      <translation>Tila kappaleen alapuolella</translation>
    </message>
    <message>
      <source>Tabulators and Indentation</source>
      <translation>Sarkaimet ja sisennykset</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation>&amp;Nimi:</translation>
    </message>
    <message>
      <source>&amp;Lines:</source>
      <translation>R&amp;ivejä:</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Distances</source>
      <translation>Etäisyydet</translation>
    </message>
    <message>
      <source>Fixed Linespacing</source>
      <translation>Kiinteä riviväli</translation>
    </message>
    <message>
      <source>Automatic Linespacing</source>
      <translation>Automaattinen riviväli</translation>
    </message>
    <message>
      <source>Align to Baseline Grid</source>
      <translation>Kiinnitä peruslinjaan</translation>
    </message>
    <message>
      <source>Drop Caps</source>
      <translation>Anfangit</translation>
    </message>
    <message>
      <source>Distance from Text:</source>
      <translation>Etäisyys tekstistä</translation>
    </message>
    <message>
      <source>Preview of the Paragraph Style</source>
      <translation>Kappaletyylin esikatselu</translation>
    </message>
    <message>
      <source>Determines the gap between the DropCaps and the Text</source>
      <translation>Väli anfangin ja tekstin välillä</translation>
    </message>
    <message>
      <source>Toggles sample text of this paragraph style</source>
      <translation>Näytä/piilota kappaletyylin esimerkki</translation>
    </message>
    <message>
      <source>Name of the style is not unique</source>
      <translation>Tyylin nimi on jo käytössä</translation>
    </message>
    <message>
      <source>Background</source>
      <translation>Tausta</translation>
    </message>
  </context>
  <context>
    <name>Editor</name>
    <message>
      <source>Editor</source>
      <translation>Editori</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>Uu&amp;si</translation>
    </message>
    <message>
      <source>&amp;Open...</source>
      <translation>&amp;Avaa...</translation>
    </message>
    <message>
      <source>Save &amp;As...</source>
      <translation>Tallenna &amp;nimellä...</translation>
    </message>
    <message>
      <source>&amp;Save and Exit</source>
      <translation>Tallenna ja &amp;poistu</translation>
    </message>
    <message>
      <source>&amp;Exit without Saving</source>
      <translation>P&amp;oistu tallentamatta</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>&amp;Kumoa</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>Tee &amp;uudelleen</translation>
    </message>
    <message>
      <source>Cu&amp;t</source>
      <translation>&amp;Leikkaa</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>&amp;Kopioi</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>L&amp;iitä</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>T&amp;yhjennä</translation>
    </message>
    <message>
      <source>&amp;Get Field Names</source>
      <translation>&amp;Hae kenttien nimet</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>&amp;Tiedosto</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Muokkaa</translation>
    </message>
    <message>
      <source>JavaScripts (*.js);;All Files (*)</source>
      <translation>JavaScript-tiedostot (*.js);;Kaikki tiedostot (*)</translation>
    </message>
  </context>
  <context>
    <name>EffectsDialog</name>
    <message>
      <source>Image Effects</source>
      <translation>Kuvan muokkaus</translation>
    </message>
    <message>
      <source>Options:</source>
      <translation>Asetukset:</translation>
    </message>
    <message>
      <source>Color:</source>
      <translation>Väri:</translation>
    </message>
    <message>
      <source>Shade:</source>
      <translation>Sävy:</translation>
    </message>
    <message>
      <source>Brightness:</source>
      <translation>Kirkkaus:</translation>
    </message>
    <message>
      <source>Contrast:</source>
      <translation>&amp;Kontrasti:</translation>
    </message>
    <message>
      <source>Radius:</source>
      <translation>Säde:</translation>
    </message>
    <message>
      <source>Value:</source>
      <translation>Arvo:</translation>
    </message>
    <message>
      <source>Posterize:</source>
      <translation>Porrasta</translation>
    </message>
    <message>
      <source>Available Effects</source>
      <translation>Saatavilla olevat tehosteet</translation>
    </message>
    <message>
      <source>Blur</source>
      <translation>Heikennä terävyyttä</translation>
    </message>
    <message>
      <source>Brightness</source>
      <translation>Kirkkaus</translation>
    </message>
    <message>
      <source>Colorize</source>
      <translation>Väritä</translation>
    </message>
    <message>
      <source>Contrast</source>
      <translation>Kontrasti</translation>
    </message>
    <message>
      <source>Grayscale</source>
      <translation>Harmaasävy</translation>
    </message>
    <message>
      <source>Invert</source>
      <translation>Käänteinen</translation>
    </message>
    <message>
      <source>Posterize</source>
      <translation>Porrasta</translation>
    </message>
    <message>
      <source>Sharpen</source>
      <translation>Terävöitä</translation>
    </message>
    <message>
      <source>>></source>
      <translation>>></translation>
    </message>
    <message>
      <source>&lt;&lt;</source>
      <translation>&lt;&lt;</translation>
    </message>
    <message>
      <source>Effects in use</source>
      <translation>Käytössä olevat tehosteet</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Peruuta</translation>
    </message>
  </context>
  <context>
    <name>ExportForm</name>
    <message>
      <source>&amp;All pages</source>
      <translation>&amp;Kaikki sivut</translation>
    </message>
    <message>
      <source>Change the output directory</source>
      <translation>Muuta kohdehakemistoa</translation>
    </message>
    <message>
      <source>Available export formats</source>
      <translation>Käytettävät tiedostotyypit</translation>
    </message>
    <message>
      <source>Choose a Export Directory</source>
      <translation>Valitse Kohdehakemisto</translation>
    </message>
    <message>
      <source>The output directory - the place to store your images.
Name of the export file will be 'documentname-pagenumber.filetype'</source>
      <translation>Kohdehakemisto, jonne kuvat tallennetaan. Vietyjen tiedostojen 
nimet ovat muotoa asiakirjan_nimi-sivunumero.tiedostotyyppi</translation>
    </message>
    <message>
      <source>Export only the current page</source>
      <translation>Vie ainoastaan nykyinen sivu</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>&amp;Muuta...</translation>
    </message>
    <message>
      <source>&amp;Export to Directory:</source>
      <translation>&amp;Vie hakemistoon:</translation>
    </message>
    <message>
      <source>Image &amp;Type:</source>
      <translation>Kuvan &amp;Tyyppi:</translation>
    </message>
    <message>
      <source>&amp;Quality:</source>
      <translation>&amp;Laatu:</translation>
    </message>
    <message>
      <source>Export as Image(s)</source>
      <translation>Vie kuvana</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>Asetukset</translation>
    </message>
    <message>
      <source>&amp;Resolution:</source>
      <translation>Ta&amp;rkkuus:</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source> dpi</source>
      <translation> dpi</translation>
    </message>
    <message>
      <source>Range</source>
      <translation>Väli</translation>
    </message>
    <message>
      <source>&amp;Current page</source>
      <translation>&amp;Nykyinen sivu</translation>
    </message>
    <message>
      <source>&amp;Range</source>
      <translation>&amp;Alue</translation>
    </message>
    <message>
      <source>C</source>
      <translation>C</translation>
    </message>
    <message>
      <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
      <translation>Syötä pilkuin eroteltu lista, jossa * merkitsee 
kaikkia sivuja ja 1-5 sivuväliä. Voit myös syöttää 
yksittäisen sivunumeron.</translation>
    </message>
    <message>
      <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
      <translation>Kuvien resoluutio. 72 dpi on hyvä valinta kuville,
joita on tarkoitus katsella näytöltä</translation>
    </message>
    <message>
      <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
      <translation>Kuviesi laatu. 100 % on paras ja 1 % heikoin</translation>
    </message>
    <message>
      <source>Export a range of pages</source>
      <translation>Vie valikoima sivuja</translation>
    </message>
    <message>
      <source>Export all pages</source>
      <translation>Vie kaikki sivut</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Koko:</translation>
    </message>
    <message>
      <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
      <translation>Kuvien koko, 100 % ei muutosta, 200 % kaksi kertaa suurempi jne.</translation>
    </message>
  </context>
  <context>
    <name>ExtImageProps</name>
    <message>
      <source>Extended Image Properties</source>
      <translation>Kuvan ominaisuudet</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>Normaali</translation>
    </message>
    <message>
      <source>Darken</source>
      <translation>Tummenna</translation>
    </message>
    <message>
      <source>Lighten</source>
      <translation>Vaalenna</translation>
    </message>
    <message>
      <source>Hue</source>
      <translation>Sävy</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>Kylläisyys</translation>
    </message>
    <message>
      <source>Color</source>
      <translation>Väri</translation>
    </message>
    <message>
      <source>Luminosity</source>
      <translation>Kirkkaus</translation>
    </message>
    <message>
      <source>Multiply</source>
      <translation>Kerroin</translation>
    </message>
    <message>
      <source>Screen</source>
      <translation>Rasterointi</translation>
    </message>
    <message>
      <source>Dissolve</source>
      <translation>Häivytä</translation>
    </message>
    <message>
      <source>Overlay</source>
      <translation>Sulautus</translation>
    </message>
    <message>
      <source>Hard Light</source>
      <translation>Kova valo</translation>
    </message>
    <message>
      <source>Soft Light</source>
      <translation>Pehmeä valo</translation>
    </message>
    <message>
      <source>Difference</source>
      <translation>Erotus</translation>
    </message>
    <message>
      <source>Exclusion</source>
      <translation>Poisto</translation>
    </message>
    <message>
      <source>Color Dodge</source>
      <translation>Valotus</translation>
    </message>
    <message>
      <source>Color Burn</source>
      <translation>Varjostus</translation>
    </message>
    <message>
      <source>Exlusion</source>
      <translation>Poisto</translation>
    </message>
    <message>
      <source>Blend Mode:</source>
      <translation>Sekoitustila:</translation>
    </message>
    <message>
      <source>Opacity:</source>
      <translation>Läpinäkyvyys:</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Nimi</translation>
    </message>
    <message>
      <source>Background</source>
      <translation>Tausta</translation>
    </message>
    <message>
      <source>Layers</source>
      <translation>Tasot</translation>
    </message>
    <message>
      <source>Don't use any Path</source>
      <translation>Älä käytä polkuja</translation>
    </message>
    <message>
      <source>Paths</source>
      <translation>Hakemistot</translation>
    </message>
  </context>
  <context>
    <name>FDialogPreview</name>
    <message>
      <source>Size:</source>
      <translation>Koko:</translation>
    </message>
    <message>
      <source>No Title</source>
      <translation>Ei otsikkoa</translation>
    </message>
    <message>
      <source>Title:</source>
      <translation>Otsikko:</translation>
    </message>
    <message>
      <source>Author:</source>
      <translation>Tekijä:</translation>
    </message>
    <message>
      <source>Unknown</source>
      <translation>Tuntematon</translation>
    </message>
    <message>
      <source>Scribus Document</source>
      <translation>Scribus asiakirja</translation>
    </message>
    <message>
      <source>Resolution:</source>
      <translation>Tarkkuus:</translation>
    </message>
    <message>
      <source>DPI</source>
      <translation>DPI</translation>
    </message>
    <message>
      <source>RGB</source>
      <translation>RGB</translation>
    </message>
    <message>
      <source>CMYK</source>
      <translation>CMYK</translation>
    </message>
    <message>
      <source>Grayscale</source>
      <translation>Harmaasävy</translation>
    </message>
    <message>
      <source>Colorspace:</source>
      <translation>Väriavaruus:</translation>
    </message>
  </context>
  <context>
    <name>FileLoader</name>
    <message>
      <source>Some fonts used by this document have been substituted:</source>
      <translation>Jotkin kirjaintyypit dokumentissa vaihdettiin:</translation>
    </message>
    <message>
      <source> was replaced by: </source>
      <translation>korvattiin tyypillä: </translation>
    </message>
  </context>
  <context>
    <name>FontPrefs</name>
    <message>
      <source>Font Name</source>
      <translation>Kirjaintyypin nimi</translation>
    </message>
    <message>
      <source>Font Substitutions</source>
      <translation>Korvaavat kirjaintyypit</translation>
    </message>
    <message>
      <source>Replacement</source>
      <translation>Korvaava</translation>
    </message>
    <message>
      <source>Choose a Directory</source>
      <translation>Valitse hakemisto</translation>
    </message>
    <message>
      <source>Additional Paths</source>
      <translation>Lisätyt polut</translation>
    </message>
    <message>
      <source>Available Fonts</source>
      <translation>Mahdolliset kirjaintyypit</translation>
    </message>
    <message>
      <source>&amp;Available Fonts</source>
      <translation>L&amp;öydetyt kirjaintyypit</translation>
    </message>
    <message>
      <source>Font &amp;Substitutions</source>
      <translation>&amp;Korvaavat kirjaintyypit</translation>
    </message>
    <message>
      <source>Additional &amp;Paths</source>
      <translation>Lisätyt p&amp;olut</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>P&amp;oista</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>&amp;Muuta...</translation>
    </message>
    <message>
      <source>A&amp;dd...</source>
      <translation>&amp;Lisää...</translation>
    </message>
    <message>
      <source>&amp;Remove</source>
      <translation>Po&amp;ista</translation>
    </message>
    <message>
      <source>PostScript</source>
      <translation>PostScript</translation>
    </message>
    <message>
      <source>Font Name</source>
      <comment>font preview</comment>
      <translation>Kirjaintyypin nimi</translation>
    </message>
    <message>
      <source>Use Font</source>
      <comment>font preview</comment>
      <translation>Käytä kirjaintyyppiä</translation>
    </message>
    <message>
      <source>Embed in:</source>
      <comment>font preview</comment>
      <translation>Upota:</translation>
    </message>
    <message>
      <source>Subset</source>
      <comment>font preview</comment>
      <translation>Osajoukko</translation>
    </message>
    <message>
      <source>Path to Font File</source>
      <comment>font preview</comment>
      <translation>Polku kirjaintyyppitiedostoon</translation>
    </message>
    <message>
      <source>Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit->Settings to change the font search path.</source>
      <translation>Kirjaintyyppien hakupolut voi asettaa ainoastaan asetuksissa, kun yhtään asiakirjaa ei ole avoinna. Sulje kaikki avoimet asiakirjat ja avaa Muokkaa>Asetukset muuttaaksesi hakemistoja, joista kirjaintyyppejä etsitään.</translation>
    </message>
  </context>
  <context>
    <name>FontPreview</name>
    <message>
      <source>Doc</source>
      <comment>font preview</comment>
      <translation>Asiak.</translation>
    </message>
    <message>
      <source>Access</source>
      <comment>font preview</comment>
      <translation>Löytyi</translation>
    </message>
    <message>
      <source>Font Size:</source>
      <translation>Kirjaintyypin koko:</translation>
    </message>
    <message>
      <source>Quick Search: </source>
      <translation>Haku: </translation>
    </message>
    <message>
      <source>&amp;Search</source>
      <translation>&amp;Etsi</translation>
    </message>
    <message>
      <source>Typing the text here provides quick searching in the font names. E.g. 'bold' shows all fonts with Bold in name. Searching is case insensitive.</source>
      <translation>Etsi kirjaintyyppejä kirjoittamalla kirjaintyypin nimestä osa. Esimerkiksi kirjoittamalla &quot;bold&quot; saat kaikki kirjaintyypit joiden nimessä esiintyy sana bold.</translation>
    </message>
    <message>
      <source>Start searching</source>
      <translation>Aloita etsintä</translation>
    </message>
    <message>
      <source>Size of the selected font</source>
      <translation>Valitun kirjaintyypin koko</translation>
    </message>
    <message>
      <source>User</source>
      <comment>font preview</comment>
      <translation>Käyttäjä</translation>
    </message>
    <message>
      <source>System</source>
      <comment>font preview</comment>
      <translation>Järjestelmä</translation>
    </message>
    <message>
      <source>&amp;Append</source>
      <translation>&amp;Tuo</translation>
    </message>
    <message>
      <source>Font Name</source>
      <comment>font preview</comment>
      <translation>Kirjaintyypin nimi</translation>
    </message>
    <message>
      <source>Type</source>
      <comment>font preview</comment>
      <translation>Tyyppi</translation>
    </message>
    <message>
      <source>Subset</source>
      <comment>font preview</comment>
      <translation>Osajoukko</translation>
    </message>
    <message>
      <source>Fonts Preview</source>
      <comment>font preview</comment>
      <translation>Kirjaintyyppien esikatselu</translation>
    </message>
    <message>
      <source>Alt+O</source>
      <comment>font preview</comment>
      <translation>Alt+O</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <comment>font preview</comment>
      <translation>&amp;Sulje</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <comment>font preview</comment>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>Append selected font into Style, Font menu</source>
      <comment>font preview</comment>
      <translation>Lisää valittu kirjaintyyppi Tyyli-Kirjaintyyppi-valikkoon</translation>
    </message>
    <message>
      <source>Leave preview</source>
      <comment>font preview</comment>
      <translation>Poistu esikatselusta</translation>
    </message>
    <message>
      <source>Woven silk pyjamas exchanged for blue quartz</source>
      <comment>font preview</comment>
      <translation>Törkylempijä vongahdus on suomenkielinen pangram</translation>
    </message>
  </context>
  <context>
    <name>FontPreviewPlugin</name>
    <message>
      <source>&amp;Font Preview...</source>
      <translation>&amp;Kirjaintyyppien esikatselu...</translation>
    </message>
    <message>
      <source>Font Preview dialog</source>
      <translation>Kirjaintyyppien esikatselun dialogi</translation>
    </message>
    <message>
      <source>Sorting, searching and browsing available fonts.</source>
      <translation>Järjestä, etsi ja selaa saatavilla olevia kirjaintyyppejä.</translation>
    </message>
  </context>
  <context>
    <name>FontReplaceDialog</name>
    <message>
      <source>Font Substitution</source>
      <translation>Korvaa kirjaintyyppi</translation>
    </message>
    <message>
      <source>Original Font</source>
      <translation>Alkuperäinen kirjaintyyppi</translation>
    </message>
    <message>
      <source>Substitution Font</source>
      <translation>Korvaava kirjaintyyppi</translation>
    </message>
    <message>
      <source>Make these substitutions permanent</source>
      <translation>Tee korvauksista pysyviä</translation>
    </message>
    <message>
      <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
      <translation>Asiakirja sisältää kirjaintyyppejä, joita ei löydy järjestelmästäsi. Valitse ne korvaavat kirjaintyypit. Peruuttaminen lopettaa asiakirjan latauksen.</translation>
    </message>
    <message>
      <source>Cancels these font substitutions and stops loading the document.</source>
      <translation>Peruuta kirjaintyyppien korvaus ja keskeytä asikirjan lataaminen.</translation>
    </message>
    <message>
      <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit > Preferences > Fonts.</source>
      <translation>Jos käytät tätä Scribus korvaa kaikissa asiakirjoissa nämä fontit valinnoillasi. Kirjaintyyppien korvaavuuksia voi myöhemmin säätää valikon kohdassa Muokkaa->Asetukset->Kirjaintyypit.</translation>
    </message>
    <message>
      <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
      <translation>Jos valitset OK ja tallennat nämä korvaukset jäävät asiakirjaan pysyvästi.</translation>
    </message>
  </context>
  <context>
    <name>GradientEditor</name>
    <message>
      <source>Position:</source>
      <translation>Sijainti:</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Here you can add, change or remove Color-Stops.</source>
      <translation>Lisää, muuta tai poista värien paikkoja.</translation>
    </message>
  </context>
  <context>
    <name>GuideManager</name>
    <message>
      <source>Horizontal Guides</source>
      <translation>Vaakasuorat apuviivat</translation>
    </message>
    <message>
      <source>Manage Guides</source>
      <translation>Apuviivojen hallinta</translation>
    </message>
    <message>
      <source>Vertical Guides</source>
      <translation>Pystysuorat apuviivat</translation>
    </message>
    <message>
      <source>&amp;Y-Pos:</source>
      <translation>&amp;Y:</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation>&amp;Lisää</translation>
    </message>
    <message>
      <source>D&amp;elete</source>
      <translation>P&amp;oista</translation>
    </message>
    <message>
      <source>&amp;X-Pos:</source>
      <translation>&amp;X:</translation>
    </message>
    <message>
      <source>A&amp;dd</source>
      <translation>L&amp;isää</translation>
    </message>
    <message>
      <source>De&amp;lete</source>
      <translation>Poi&amp;sta</translation>
    </message>
    <message>
      <source>&amp;Lock Guides</source>
      <translation>L&amp;ukitse apuviivat</translation>
    </message>
    <message>
      <source>Rows and Columns - Automatic Guides</source>
      <translation>Rivit ja palstat - Automaattiset apuviivat</translation>
    </message>
    <message>
      <source>&amp;Rows:</source>
      <translation>&amp;Rivit:</translation>
    </message>
    <message>
      <source>C&amp;olumns:</source>
      <translation>&amp;Palstat:</translation>
    </message>
    <message>
      <source>Row &amp;Gap</source>
      <translation>&amp;Riviväli</translation>
    </message>
    <message>
      <source>Colum&amp;n Gap</source>
      <translation>&amp;Sarakeväli</translation>
    </message>
    <message>
      <source>Refer to:</source>
      <translation>Suhteessa:</translation>
    </message>
    <message>
      <source>&amp;Page</source>
      <translation>&amp;Sivu</translation>
    </message>
    <message>
      <source>&amp;Margins</source>
      <translation>&amp;Reunuksiin</translation>
    </message>
    <message>
      <source>&amp;Selection</source>
      <translation>&amp;Valintaan</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Sulje</translation>
    </message>
    <message>
      <source>&amp;Update</source>
      <translation>&amp;Päivitä</translation>
    </message>
    <message>
      <source>&amp;Apply to All Pages</source>
      <translation>&amp;Käytä kaikkiin sivuihin</translation>
    </message>
    <message>
      <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
      <comment>guide manager</comment>
      <translation>Aseta asiakirjan apuviivoja. Ikkuna pysyy yhä avoinna, mutta muutokset tulevat voimaan.</translation>
    </message>
    <message>
      <source>Guide</source>
      <translation>Apuviiva</translation>
    </message>
    <message>
      <source>Unit</source>
      <translation>Yksikkö</translation>
    </message>
    <message>
      <source>Preview</source>
      <translation>Esikatselu</translation>
    </message>
    <message>
      <source>There is empty (0.0) guide already</source>
      <translation>Tyhjä apuviiva (0.0) on jo olemassa</translation>
    </message>
  </context>
  <context>
    <name>HelpBrowser</name>
    <message>
      <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
      <translation>Ohjetta ei ole vielä saatavissa tässä muodossa. Osoitteesta http://docs.scribus.net 
löydät viimeisimmän version dokumentaatiosta. Lisää materiaalia löydät myös 
osoitteesta www.scribus.net.</translation>
    </message>
    <message>
      <source>Contents</source>
      <translation>Sisällys</translation>
    </message>
    <message>
      <source>Link</source>
      <translation>Linkki</translation>
    </message>
    <message>
      <source>Scribus Online Help</source>
      <translation>Scribus ohje verkossa</translation>
    </message>
    <message>
      <source>&amp;Contents</source>
      <translation>&amp;Sisällys</translation>
    </message>
    <message>
      <source>&amp;Search</source>
      <translation>&amp;Etsi</translation>
    </message>
    <message>
      <source>unknown</source>
      <translation>tuntematon</translation>
    </message>
    <message>
      <source>Find</source>
      <translation>Etsi</translation>
    </message>
    <message>
      <source>Search Term:</source>
      <translation>Hakusana:</translation>
    </message>
    <message>
      <source>Se&amp;arch</source>
      <translation>&amp;Etsi</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;Uusi</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Poista</translation>
    </message>
    <message>
      <source>De&amp;lete All</source>
      <translation>Poista &amp;kaikki</translation>
    </message>
    <message>
      <source>Book&amp;marks</source>
      <translation>&amp;Kirjanmerkit</translation>
    </message>
    <message>
      <source>&amp;Print...</source>
      <translation>T&amp;ulosta...</translation>
    </message>
    <message>
      <source>E&amp;xit</source>
      <translation>&amp;Lopeta</translation>
    </message>
    <message>
      <source>Searching is case unsensitive</source>
      <translation>Etsiminen ei erottele versaaleja ja gemenoja</translation>
    </message>
    <message>
      <source>New Bookmark</source>
      <translation>Uusi kirjanmerkki</translation>
    </message>
    <message>
      <source>New Bookmark's Title:</source>
      <translation>Kirjanmerkin otsikko:</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>&amp;Tiedosto</translation>
    </message>
    <message>
      <source>&amp;Find...</source>
      <translation>&amp;Etsi...</translation>
    </message>
    <message>
      <source>Find &amp;Next</source>
      <translation>Etsi &amp;seuraava</translation>
    </message>
    <message>
      <source>Find &amp;Previous</source>
      <translation>Etsi &amp;edellinen</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Muokkaa</translation>
    </message>
    <message>
      <source>&amp;Add Bookmark</source>
      <translation>&amp;Lisää kirjanmerkki</translation>
    </message>
    <message>
      <source>D&amp;elete All</source>
      <translation>&amp;Poista kaikki</translation>
    </message>
    <message>
      <source>&amp;Bookmarks</source>
      <translation>&amp;Kirjanmerkit</translation>
    </message>
  </context>
  <context>
    <name>HyAsk</name>
    <message>
      <source>Skip</source>
      <translation>Ohita</translation>
    </message>
    <message>
      <source>Accept</source>
      <translation>Hyväksy</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Peruuta</translation>
    </message>
    <message>
      <source>Possible Hyphenation</source>
      <translation>Mahdollinen tavutus</translation>
    </message>
  </context>
  <context>
    <name>HySettings</name>
    <message>
      <source>Length of the smallest word to be hyphenated.</source>
      <translation>Lyhimmän tavutettavan sanan pituus.</translation>
    </message>
    <message>
      <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
      <translation>Suurin määrä tavutuksia peräkkäin.
0 tarkoittaa rajoittamatonta määrää tavutuksia.</translation>
    </message>
    <message>
      <source>&amp;Language:</source>
      <translation>&amp;Kieli:</translation>
    </message>
    <message>
      <source>&amp;Smallest Word:</source>
      <translation>&amp;Lyhin sana:</translation>
    </message>
    <message>
      <source>Hyphenate Text Automatically &amp;During Typing</source>
      <translation>Tavuta teksti automaattisesti &amp;kirjoitettaessa</translation>
    </message>
    <message>
      <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
      <translation>Dialogi-ikkuna, joka näyttää kaikki mahdolliset tavutettavat sanat tavutusvaihtoehtoineen, 
ilmestyy, kun käytät Työkalut, Tavuta teksti.</translation>
    </message>
    <message>
      <source>Enables automatic hyphenation of your text while typing.</source>
      <translation>Mahdollistaa automaattiset tavutuksen kirjoittaessasi.</translation>
    </message>
    <message>
      <source>&amp;Hyphenation Suggestions</source>
      <translation>&amp;Tavutusehdotukset</translation>
    </message>
    <message>
      <source>Consecutive Hyphenations &amp;Allowed:</source>
      <translation>&amp;Sallitut perättäiset tavutukset:</translation>
    </message>
  </context>
  <context>
    <name>ImageInfoDialog</name>
    <message>
      <source>Image Info</source>
      <translation>Kuvan tiedot</translation>
    </message>
    <message>
      <source>General Info</source>
      <translation>Yleiset tiedot</translation>
    </message>
    <message>
      <source>Date / Time:</source>
      <translation>Päivämäärä/aika:</translation>
    </message>
    <message>
      <source>Has Embedded Profile:</source>
      <translation>Upotettu profiili:</translation>
    </message>
    <message>
      <source>Yes</source>
      <translation>Kyllä</translation>
    </message>
    <message>
      <source>No</source>
      <translation>Ei</translation>
    </message>
    <message>
      <source>Profile Name:</source>
      <translation>Profiilin nimi:</translation>
    </message>
    <message>
      <source>Has Embedded Paths:</source>
      <translation>Upotettuja polkuja:</translation>
    </message>
    <message>
      <source>Has Layers:</source>
      <translation>Tasoja:</translation>
    </message>
    <message>
      <source>EXIF Info</source>
      <translation>EXIF info</translation>
    </message>
    <message>
      <source>Artist:</source>
      <translation>Kuvaaja:</translation>
    </message>
    <message>
      <source>Comment:</source>
      <translation>Kommentti:</translation>
    </message>
    <message>
      <source>User Comment:</source>
      <translation>Käyttäjän kommentti:</translation>
    </message>
    <message>
      <source>Camera Model:</source>
      <translation>Kameran malli:</translation>
    </message>
    <message>
      <source>Camera Manufacturer:</source>
      <translation>Kameran valmistaja:</translation>
    </message>
    <message>
      <source>Description:</source>
      <translation>Kuvaus:</translation>
    </message>
    <message>
      <source>Copyright:</source>
      <translation>Copyright:</translation>
    </message>
    <message>
      <source>Scanner Model:</source>
      <translation>Skannerin malli:</translation>
    </message>
    <message>
      <source>Scanner Manufacturer:</source>
      <translation>Skannerin valmistaja:</translation>
    </message>
  </context>
  <context>
    <name>ImportPSPlugin</name>
    <message>
      <source>Import &amp;EPS/PS...</source>
      <translation>Tuo E&amp;PS/PS...</translation>
    </message>
    <message>
      <source>Imports EPS Files</source>
      <translation>Tuo EPS-tiedostoja</translation>
    </message>
    <message>
      <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
      <translation>Tuo useimmat EPS-tiedostot asiakirjaan, muuttaen 
vektoridatan Scribus-objekteiksi.</translation>
    </message>
    <message>
      <source>PostScript</source>
      <translation>PostScript</translation>
    </message>
  </context>
  <context>
    <name>InsPage</name>
    <message>
      <source>Normal</source>
      <translation>Normaali</translation>
    </message>
    <message>
      <source>at End</source>
      <translation>Loppuun</translation>
    </message>
    <message>
      <source>before Page</source>
      <translation>Ennen sivua</translation>
    </message>
    <message>
      <source>Insert Page</source>
      <translation>Lisää sivu</translation>
    </message>
    <message>
      <source>after Page</source>
      <translation>Jälkeen sivun</translation>
    </message>
    <message>
      <source>Page(s)</source>
      <translation>sivu(a)</translation>
    </message>
    <message>
      <source>&amp;Insert</source>
      <translation>&amp;Lisää</translation>
    </message>
    <message>
      <source>&amp;Master Page:</source>
      <translation>&amp;Mallisivu:</translation>
    </message>
    <message>
      <source>Master Pages</source>
      <translation>Mallisivut</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>Sivun koko</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Koko:</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>Oma</translation>
    </message>
    <message>
      <source>Orie&amp;ntation:</source>
      <translation>&amp;Suunta:</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>Pystysuora</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>Vaakasuora</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>&amp;Leveys:</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>&amp;Korkeus:</translation>
    </message>
    <message>
      <source>Move Objects with their Page</source>
      <translation>Siirrä kohteet sivun mukana</translation>
    </message>
  </context>
  <context>
    <name>InsertTable</name>
    <message>
      <source>Insert Table</source>
      <translation>Lisää taulukko</translation>
    </message>
    <message>
      <source>Number of rows:</source>
      <translation>Rivien lukumäärä:</translation>
    </message>
    <message>
      <source>Number of columns:</source>
      <translation>Palstojen lukumäärä:</translation>
    </message>
  </context>
  <context>
    <name>JavaDocs</name>
    <message>
      <source>New Script</source>
      <translation>Uusi skripti</translation>
    </message>
    <message>
      <source>Edit JavaScripts</source>
      <translation>Muokkaa javaskriptejä</translation>
    </message>
    <message>
      <source>&amp;Edit...</source>
      <translation>&amp;Muokkaa...</translation>
    </message>
    <message>
      <source>&amp;Add...</source>
      <translation>&amp;Lisää...</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Poista</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Sulje</translation>
    </message>
    <message>
      <source>&amp;New Script:</source>
      <translation>&amp;Uusi skripti:</translation>
    </message>
    <message>
      <source>&amp;No</source>
      <translation>&amp;Ei</translation>
    </message>
    <message>
      <source>&amp;Yes</source>
      <translation>&amp;Kyllä</translation>
    </message>
    <message>
      <source>Do you really want to delete this script?</source>
      <translation>Haluatko poistaa tämän skriptin?</translation>
    </message>
    <message>
      <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
      <translation>Lisää uuden skriptin, määrittelee funktion samalla nimellä. Jos haluat käyttää tätä skriptiä avaustoimintaskriptinä älä muuta funktion nimeä.</translation>
    </message>
  </context>
  <context>
    <name>KeyManager</name>
    <message>
      <source>Alt</source>
      <translation>Alt</translation>
    </message>
    <message>
      <source>Alt+</source>
      <translation>Alt+</translation>
    </message>
    <message>
      <source>Ctrl</source>
      <translation>Ctrl</translation>
    </message>
    <message>
      <source>Ctrl+</source>
      <translation>Ctrl+</translation>
    </message>
    <message>
      <source>Shift</source>
      <translation>Shift</translation>
    </message>
    <message>
      <source>Action</source>
      <translation>Toiminta</translation>
    </message>
    <message>
      <source>Shift+</source>
      <translation>Shift+</translation>
    </message>
    <message>
      <source>ALT+SHIFT+T</source>
      <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
      <source>Select a Key for this Action</source>
      <translation>Valitse näppäinoikopolku toiminnolle</translation>
    </message>
    <message>
      <source>Current Key</source>
      <translation>Nykyinen näppäin</translation>
    </message>
    <message>
      <source>&amp;No Key</source>
      <translation>&amp;Ei näppäintä</translation>
    </message>
    <message>
      <source>&amp;User Defined Key</source>
      <translation>O&amp;ma näppäin</translation>
    </message>
    <message>
      <source>Set &amp;Key</source>
      <translation>&amp;Aseta näppäin</translation>
    </message>
    <message>
      <source>Loadable Shortcut Sets</source>
      <translation>Ladattavat näppäinoikopolut</translation>
    </message>
    <message>
      <source>&amp;Load</source>
      <translation>&amp;Lataa</translation>
    </message>
    <message>
      <source>&amp;Import...</source>
      <translation>&amp;Tuo...</translation>
    </message>
    <message>
      <source>&amp;Export...</source>
      <translation>&amp;Vie...</translation>
    </message>
    <message>
      <source>&amp;Reset</source>
      <translation>&amp;Palauta</translation>
    </message>
    <message>
      <source>Keyboard shortcut sets available to load</source>
      <translation>Ladattavissa olevat näppäinoikopolut</translation>
    </message>
    <message>
      <source>Load the selected shortcut set</source>
      <translation>Lataa valittu näppäinoikopolku</translation>
    </message>
    <message>
      <source>Import a shortcut set into the current configuration</source>
      <translation>Tuo näppäinoikopolku nykyisiin asetuksiin</translation>
    </message>
    <message>
      <source>Export the current shortcuts into an importable file</source>
      <translation>Vie nykyiset näppäinoikopolut tiedostoon</translation>
    </message>
    <message>
      <source>Reload the default Scribus shortcuts</source>
      <translation>Lataa oletusnäppäinoikopolut</translation>
    </message>
    <message>
      <source>Key Set XML Files (*.ksxml)</source>
      <translation>Näppäinoikopolkutiedosto (*.ksxml)</translation>
    </message>
    <message>
      <source>This key sequence is already in use</source>
      <translation>Näppäinyhdistelmä on jo käytössä</translation>
    </message>
    <message>
      <source>Meta</source>
      <translation>Meta</translation>
    </message>
    <message>
      <source>Meta+</source>
      <translation>Meta+</translation>
    </message>
  </context>
  <context>
    <name>LayerPalette</name>
    <message>
      <source>Delete Layer</source>
      <translation>Poista taso</translation>
    </message>
    <message>
      <source>Layers</source>
      <translation>Tasot</translation>
    </message>
    <message>
      <source>Do you want to delete all objects on this layer too?</source>
      <translation>Haluatko myös poistaa kaikki kohteet tältä tasolta?</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Nimi</translation>
    </message>
    <message>
      <source>Add a new layer</source>
      <translation>Uusi taso</translation>
    </message>
    <message>
      <source>Delete layer</source>
      <translation>Poista taso</translation>
    </message>
    <message>
      <source>Raise layer</source>
      <translation>Nosta tasoa</translation>
    </message>
    <message>
      <source>Lower layer</source>
      <translation>Laske tasoa</translation>
    </message>
  </context>
  <context>
    <name>LineFormate</name>
    <message>
      <source>Open</source>
      <translation>Avaa</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
      <translation>Asiakirjat (*.sla *.sla.gz *.scd *.scd.gz);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Edit Line Styles</source>
      <translation>Muokkaa viivatyylejä</translation>
    </message>
    <message>
      <source>Documents (*.sla *.scd);;All Files (*)</source>
      <translation>Asiakirjat (*.sla *.scd);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>New Style</source>
      <translation>Uusi tyyli</translation>
    </message>
    <message>
      <source>Copy of %1</source>
      <translation>Kopio-%1</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;Uusi</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Muokkaa</translation>
    </message>
    <message>
      <source>D&amp;uplicate</source>
      <translation>&amp;Kopioi</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>P&amp;oista</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Tallenna</translation>
    </message>
    <message>
      <source>&amp;No</source>
      <translation>&amp;Ei</translation>
    </message>
    <message>
      <source>&amp;Yes</source>
      <translation>Ky&amp;llä</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>T&amp;uo</translation>
    </message>
    <message>
      <source>Do you really want to delete this style?</source>
      <translation>Haluatko poistaa tämän tyylin?</translation>
    </message>
  </context>
  <context>
    <name>LoadSavePlugin</name>
    <message>
      <source>All Files (*)</source>
      <translation>Kaikki tiedostot (*)</translation>
    </message>
  </context>
  <context>
    <name>LoremManager</name>
    <message>
      <source>Select Lorem Ipsum</source>
      <translation>Valitse Lorem Ipsum</translation>
    </message>
    <message>
      <source>Author:</source>
      <translation>Tekijä:</translation>
    </message>
    <message>
      <source>Get More:</source>
      <translation>Hae lisää:</translation>
    </message>
    <message>
      <source>XML File:</source>
      <translation>XML-tiedosto:</translation>
    </message>
    <message>
      <source>Lorem Ipsum</source>
      <translation>Lorem Ipsum</translation>
    </message>
    <message>
      <source>Paragraphs:</source>
      <translation>Kappaleet:</translation>
    </message>
    <message>
      <source>Alt+O</source>
      <translation>Alt+O</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
  </context>
  <context>
    <name>MarginDialog</name>
    <message>
      <source>Margin Guides</source>
      <translation>Reunaviivat</translation>
    </message>
    <message>
      <source>Manage Page Properties</source>
      <translation>Hallitse sivun ominaisuuksia</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>Sivun koko</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Koko:</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>Oma</translation>
    </message>
    <message>
      <source>Orie&amp;ntation:</source>
      <translation>&amp;Suunta:</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>Pystysuora</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>Vaakasuora</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>&amp;Leveys:</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>&amp;Korkeus:</translation>
    </message>
    <message>
      <source>Move Objects with their Page</source>
      <translation>Siirrä kohteet sivun mukana</translation>
    </message>
    <message>
      <source>Type:</source>
      <translation>Tyyppi:</translation>
    </message>
  </context>
  <context>
    <name>MarginWidget</name>
    <message>
      <source>&amp;Bottom:</source>
      <translation>&amp;Alhaalla:</translation>
    </message>
    <message>
      <source>&amp;Top:</source>
      <translation>&amp;Ylhäällä:</translation>
    </message>
    <message>
      <source>&amp;Right:</source>
      <translation>&amp;Oikealla:</translation>
    </message>
    <message>
      <source>&amp;Left:</source>
      <translation>&amp;Vasemmalla:</translation>
    </message>
    <message>
      <source>Distance between the top margin guide and the edge of the page</source>
      <translation>Paperin reunan ja yläreunuksen välinen etäisyys</translation>
    </message>
    <message>
      <source>Distance between the bottom margin guide and the edge of the page</source>
      <translation>Paperin reunan ja alareunuksen välinen etäisyys</translation>
    </message>
    <message>
      <source>&amp;Inside:</source>
      <translation>&amp;Sisäpuolella:</translation>
    </message>
    <message>
      <source>O&amp;utside:</source>
      <translation>&amp;Ulkopuolella:</translation>
    </message>
    <message>
      <source>Preset Layouts:</source>
      <translation>Valmiit asettelut:</translation>
    </message>
    <message>
      <source>Apply margin settings to all pages</source>
      <translation>Käytä reunusten asetuksia kaikkiin sivuihin</translation>
    </message>
    <message>
      <source>Apply the margin changes to all existing pages in the document</source>
      <translation>Käytä reunusten muutoksia asiakirjan kaikkiin sivuihin.</translation>
    </message>
    <message>
      <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
      <translation>Etäisyys vasemman reunuksen ja paperin reunan välillä.</translation>
    </message>
    <message>
      <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
      <translation>Etäisyys oikean reunuksen ja paperin reunan välillä.</translation>
    </message>
  </context>
  <context>
    <name>MasterPagesPalette</name>
    <message>
      <source>Edit Master Pages</source>
      <translation>Muokka mallisivuja</translation>
    </message>
    <message>
      <source>Do you really want to delete this master page?</source>
      <translation>Haluatko poistaa tämän mallisivun?</translation>
    </message>
    <message>
      <source>&amp;No</source>
      <translation>&amp;Ei</translation>
    </message>
    <message>
      <source>&amp;Yes</source>
      <translation>&amp;Kyllä</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation>&amp;Nimi:</translation>
    </message>
    <message>
      <source>New Master Page</source>
      <translation>Uusi mallisivu</translation>
    </message>
    <message>
      <source>Copy of %1</source>
      <translation>Kopio-%1</translation>
    </message>
    <message>
      <source>Name:</source>
      <translation>Nimi:</translation>
    </message>
    <message>
      <source>New MasterPage</source>
      <translation>Uusi mallisivu</translation>
    </message>
    <message>
      <source>Copy #%1 of </source>
      <translation>Kopio #%1/</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>Normaali</translation>
    </message>
    <message>
      <source>Duplicate the selected master page</source>
      <translation>Kopioi valittu mallisivu</translation>
    </message>
    <message>
      <source>Delete the selected master page</source>
      <translation>Poista valittu mallisivu</translation>
    </message>
    <message>
      <source>Add a new master page</source>
      <translation>Lisää uusi mallisivu</translation>
    </message>
    <message>
      <source>Import master pages from another document</source>
      <translation>Tuo mallisivuja toisesta asiakirjasta</translation>
    </message>
    <message>
      <source>New Master Page %1</source>
      <translation>Uusi mallisivu %1</translation>
    </message>
  </context>
  <context>
    <name>Mdup</name>
    <message>
      <source>Multiple Duplicate</source>
      <translation>Kopioi</translation>
    </message>
    <message>
      <source>&amp;Number of Copies:</source>
      <translation>&amp;Kopioiden lukumäärä:</translation>
    </message>
    <message>
      <source>&amp;Horizontal Shift:</source>
      <translation>&amp;Vaakasuora siirtymä:</translation>
    </message>
    <message>
      <source>&amp;Vertical Shift:</source>
      <translation>P&amp;ystysuora siirtymä:</translation>
    </message>
  </context>
  <context>
    <name>Measurements</name>
    <message>
      <source>Distances</source>
      <translation>Etäisyydet</translation>
    </message>
    <message>
      <source>X1:</source>
      <translation>X1:</translation>
    </message>
    <message>
      <source>Y1:</source>
      <translation>Y1:</translation>
    </message>
    <message>
      <source>X2:</source>
      <translation>X2:</translation>
    </message>
    <message>
      <source>Y2:</source>
      <translation>Y2:</translation>
    </message>
    <message>
      <source>DX:</source>
      <translation>DX:</translation>
    </message>
    <message>
      <source>DY:</source>
      <translation>DY:</translation>
    </message>
    <message>
      <source>Angle:</source>
      <translation>Kulma:</translation>
    </message>
    <message>
      <source>Length:</source>
      <translation>Pituus:</translation>
    </message>
    <message>
      <source>pt</source>
      <translation>pt</translation>
    </message>
  </context>
  <context>
    <name>MergeDoc</name>
    <message>
      <source>Open</source>
      <translation>Avaa</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
      <translation>Asiakirjat (*.sla *.sla.gz *.scd *.scd.gz);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Documents (*.sla *.scd);;All Files (*)</source>
      <translation>Asiakirjat (*.sla *.scd);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Import Page(s)</source>
      <translation>Tuo sivu(ja)</translation>
    </message>
    <message>
      <source> from 0</source>
      <translation>/ 0</translation>
    </message>
    <message>
      <source>Create Page(s)</source>
      <translation>Luo sivu(t)</translation>
    </message>
    <message>
      <source> from %1</source>
      <translation>/ %1</translation>
    </message>
    <message>
      <source>Import Master Page</source>
      <translation>Tuo mallisivu</translation>
    </message>
    <message>
      <source>&amp;From Document:</source>
      <translation>&amp;Asiakirjasta:</translation>
    </message>
    <message>
      <source>Chan&amp;ge...</source>
      <translation>&amp;Muuta...</translation>
    </message>
    <message>
      <source>&amp;Import Page(s):</source>
      <translation>&amp;Tuo sivuja:</translation>
    </message>
    <message>
      <source>&amp;Import Master Page</source>
      <translation>&amp;Tuo mallisivu</translation>
    </message>
    <message>
      <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
      <translation>Anna pilkuin eroteltu lista. Listan elementti voi olla yksittäinen sivunumero, * joka tarkoittaa kaikkia sivuja tai sivualue alku- ja loppusivu eroteltuna merkillä - (esim. 1-5).</translation>
    </message>
    <message>
      <source>Before Page</source>
      <translation>Ennen sivua</translation>
    </message>
    <message>
      <source>After Page</source>
      <translation>Jälkeen sivun</translation>
    </message>
    <message>
      <source>At End</source>
      <translation>Loppuun</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>T&amp;uo</translation>
    </message>
  </context>
  <context>
    <name>MissingFont</name>
    <message>
      <source>Missing Font</source>
      <translation>Puuttuva kirjaintyyppi</translation>
    </message>
    <message>
      <source>The Font %1 is not installed.</source>
      <translation>Kirjaintyyppiä %1 ei ole asennettu.</translation>
    </message>
    <message>
      <source>Use</source>
      <translation>Käytä</translation>
    </message>
    <message>
      <source>instead</source>
      <translation>sijasta</translation>
    </message>
  </context>
  <context>
    <name>MovePages</name>
    <message>
      <source>Move Page(s):</source>
      <translation>Siirrä sivu(t):</translation>
    </message>
    <message>
      <source>Move Pages</source>
      <translation>Siirrä sivut</translation>
    </message>
    <message>
      <source>Copy Page</source>
      <translation>Kopioi sivu</translation>
    </message>
    <message>
      <source>Move Page(s)</source>
      <translation>Siirrä sivuja</translation>
    </message>
    <message>
      <source>Before Page</source>
      <translation>Ennen sivua</translation>
    </message>
    <message>
      <source>After Page</source>
      <translation>Jälkeen sivun</translation>
    </message>
    <message>
      <source>At End</source>
      <translation>Loppuun</translation>
    </message>
    <message>
      <source>To:</source>
      <translation>Minne:</translation>
    </message>
    <message>
      <source>Number of copies:</source>
      <translation>Kopioiden lukumäärä:</translation>
    </message>
  </context>
  <context>
    <name>Mpalette</name>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Nimi</translation>
    </message>
    <message>
      <source>None</source>
      <translation>Ei väriä</translation>
    </message>
    <message>
      <source>Level</source>
      <translation>Kerros</translation>
    </message>
    <message>
      <source>Absolute Colorimetric</source>
      <translation>Absolute Colorimetric</translation>
    </message>
    <message>
      <source>No Style</source>
      <translation>Ei tyyliä</translation>
    </message>
    <message>
      <source>Flat Cap</source>
      <translation>Litteä</translation>
    </message>
    <message>
      <source>Distance from Curve:</source>
      <translation>Etäisyys viivasta:</translation>
    </message>
    <message>
      <source>Round Join</source>
      <translation>Pyöreä</translation>
    </message>
    <message>
      <source>Rendering Intent:</source>
      <translation>Sovitustapa:</translation>
    </message>
    <message>
      <source>Font Size</source>
      <translation>Kirjaintyypin koko</translation>
    </message>
    <message>
      <source>End Points</source>
      <translation>Päätepisteet</translation>
    </message>
    <message>
      <source>Shape:</source>
      <translation>Kuvio:</translation>
    </message>
    <message>
      <source>Left Point</source>
      <translation>Vasen piste</translation>
    </message>
    <message>
      <source>Properties</source>
      <translation>Ominaisuudet</translation>
    </message>
    <message>
      <source>Square Cap</source>
      <translation>Neliö</translation>
    </message>
    <message>
      <source>Show Curve</source>
      <translation>Näytä viiva</translation>
    </message>
    <message>
      <source>Perceptual</source>
      <translation>Perceptual</translation>
    </message>
    <message>
      <source>Relative Colorimetric</source>
      <translation>Relative Colorimetric</translation>
    </message>
    <message>
      <source>Bevel Join</source>
      <translation>Tasainen</translation>
    </message>
    <message>
      <source>Miter Join</source>
      <translation>Viisto</translation>
    </message>
    <message>
      <source>Round Cap</source>
      <translation>Pyöreä</translation>
    </message>
    <message>
      <source>Geometry</source>
      <translation>Sijainti ja koko</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>Saturaatio</translation>
    </message>
    <message>
      <source>Distance of Text</source>
      <translation>Tekstin etäisyys kehyksestä</translation>
    </message>
    <message>
      <source>Line Spacing</source>
      <translation>Riviväli</translation>
    </message>
    <message>
      <source>Start Offset:</source>
      <translation>Alkupisteen siirtymä:</translation>
    </message>
    <message>
      <source>Basepoint:</source>
      <translation>Toimintapiste:</translation>
    </message>
    <message>
      <source>Input Profile:</source>
      <translation>Tuloprofiili:</translation>
    </message>
    <message>
      <source>Name of selected object</source>
      <translation>Valitun kohteen nimi</translation>
    </message>
    <message>
      <source>Horizontal position of current basepoint</source>
      <translation>Toimintapisteen vaakasuora sijainti</translation>
    </message>
    <message>
      <source>Vertical position of current basepoint</source>
      <translation>Toimintapisteen pystysuora sijainti</translation>
    </message>
    <message>
      <source>Width</source>
      <translation>Leveys</translation>
    </message>
    <message>
      <source>Height</source>
      <translation>Korkeus</translation>
    </message>
    <message>
      <source>Rotation of object at current basepoint</source>
      <translation>Kohteen kierto valitun toimintapisteen ympäri</translation>
    </message>
    <message>
      <source>Point from which measurements or rotation angles are referenced</source>
      <translation>Piste, josta kohteen sijainti ja kierto lasketaan</translation>
    </message>
    <message>
      <source>Select top left for basepoint</source>
      <translation>Valitse vasen yläkulma toimintapisteeksi</translation>
    </message>
    <message>
      <source>Select top right for basepoint</source>
      <translation>Valitse oikea yläkulma toimintapisteeksi</translation>
    </message>
    <message>
      <source>Select bottom left for basepoint</source>
      <translation>Valitse vasen alakulma toimintapisteeksi</translation>
    </message>
    <message>
      <source>Select bottom right for basepoint</source>
      <translation>Valitse oikea alakulma toimintapisteeksi</translation>
    </message>
    <message>
      <source>Select center for basepoint</source>
      <translation>Valitse keskusta toimintapisteeksi</translation>
    </message>
    <message>
      <source>Flip Horizontal</source>
      <translation>Käännä vaakatasossa</translation>
    </message>
    <message>
      <source>Flip Vertical</source>
      <translation>Käännä pystytasossa</translation>
    </message>
    <message>
      <source>Move one level up</source>
      <translation>Siirrä yksi kerros ylöspäin</translation>
    </message>
    <message>
      <source>Move one level down</source>
      <translation>Siirrä yksi kerros alaspäin</translation>
    </message>
    <message>
      <source>Move to front</source>
      <translation>Siirrä päällimmäiseksi</translation>
    </message>
    <message>
      <source>Move to back</source>
      <translation>Siirrä alimmaiseksi</translation>
    </message>
    <message>
      <source>Lock or unlock the object</source>
      <translation>Lukitse tai vapauta kohde</translation>
    </message>
    <message>
      <source>Lock or unlock the size of the object</source>
      <translation>Lukitse tai vapauta kohteen koko</translation>
    </message>
    <message>
      <source>Enable or disable printing of the object</source>
      <translation>Tulosta/Älä tulosta kohdetta</translation>
    </message>
    <message>
      <source>Font of selected text or object</source>
      <translation>Valitun kohteen kirjaintyyppi</translation>
    </message>
    <message>
      <source>Scaling width of characters</source>
      <translation>Merkkien leveyssuuntainen skaalaus</translation>
    </message>
    <message>
      <source>Saturation of color of text stroke</source>
      <translation>Tekstin reunan värin kylläisyys</translation>
    </message>
    <message>
      <source>Saturation of color of text fill</source>
      <translation>Tekstin täytön värin kylläisyys</translation>
    </message>
    <message>
      <source>Style of current paragraph</source>
      <translation>Valitun kappaleen tyyli</translation>
    </message>
    <message>
      <source>Change settings for left or end points</source>
      <translation>Muuta viivan vasemman pisteen tai päätepisteitten asetuksia</translation>
    </message>
    <message>
      <source>Pattern of line</source>
      <translation>Viivan kuvio</translation>
    </message>
    <message>
      <source>Thickness of line</source>
      <translation>Viivan paksuus</translation>
    </message>
    <message>
      <source>Type of line joins</source>
      <translation>Kulmien tyyli</translation>
    </message>
    <message>
      <source>Type of line end</source>
      <translation>Viivan pään tyyli</translation>
    </message>
    <message>
      <source>Line style of current object</source>
      <translation>Valitun kohteen viivatyyli</translation>
    </message>
    <message>
      <source>Choose the shape of frame...</source>
      <translation>Valitse kehyksen kuvio...</translation>
    </message>
    <message>
      <source>Edit shape of the frame...</source>
      <translation>Muokkaa kehyksen kuviota...</translation>
    </message>
    <message>
      <source>Set radius of corner rounding</source>
      <translation>Aseta kulman pyöristyksen säde</translation>
    </message>
    <message>
      <source>Number of columns in text frame</source>
      <translation>Tekstikehyksen palstojen lukumäärä</translation>
    </message>
    <message>
      <source>Distance between columns</source>
      <translation>Palstojen välinen etäisyys</translation>
    </message>
    <message>
      <source>Distance of text from top of frame</source>
      <translation>Tekstin etäisyys kehyksen yläreunasta</translation>
    </message>
    <message>
      <source>Distance of text from bottom of frame</source>
      <translation>Tekstin etäisyys kehyksen alareunasta</translation>
    </message>
    <message>
      <source>Distance of text from left of frame</source>
      <translation>Tekstin etäisyys kehyksen vasemmasta reunasta</translation>
    </message>
    <message>
      <source>Distance of text from right of frame</source>
      <translation>Tekstin etäisyys oikeasta reunasta</translation>
    </message>
    <message>
      <source>Edit tab settings of text frame...</source>
      <translation>Muokkaa tekstikehyksen sarkainasetuksia...</translation>
    </message>
    <message>
      <source>Allow the image to be a different size to the frame</source>
      <translation>Salli kuvan koon poiketa kehyksen koosta</translation>
    </message>
    <message>
      <source>Horizontal offset of image within frame</source>
      <translation>Kuvan siirtymä vaakatasossa kehyksen sisällä</translation>
    </message>
    <message>
      <source>Vertical offset of image within frame</source>
      <translation>Kuvan siirtymä pystytasossa kehyksen sisällä</translation>
    </message>
    <message>
      <source>Resize the image horizontally</source>
      <translation>Muuta kuvan kokoa vaakasuunnassa</translation>
    </message>
    <message>
      <source>Resize the image vertically</source>
      <translation>Muuta kuvan kokoa pystysuunnassa</translation>
    </message>
    <message>
      <source>Keep the X and Y scaling the same</source>
      <translation>Säilytä vaaka- ja pystysivujen suhde</translation>
    </message>
    <message>
      <source>Make the image fit within the size of the frame</source>
      <translation>Sovita kuva kehyksen kokoon</translation>
    </message>
    <message>
      <source>Use image proportions rather than those of the frame</source>
      <translation>Käytä kuvan sivujen suhteita kehyksen suhteiden sijaan</translation>
    </message>
    <message>
      <source>Cell Lines</source>
      <translation>Solun reunat</translation>
    </message>
    <message>
      <source>Line at Top</source>
      <translation>Viiva ylhäällä</translation>
    </message>
    <message>
      <source>Line at the Left</source>
      <translation>Viiva vasemmalla</translation>
    </message>
    <message>
      <source>Line at the Right </source>
      <translation>Viiva oikealla</translation>
    </message>
    <message>
      <source>Line at Bottom</source>
      <translation>Viiva alhaalla</translation>
    </message>
    <message>
      <source>Keep the aspect ratio</source>
      <translation>Säilytä suhteet</translation>
    </message>
    <message>
      <source>Source profile of the image</source>
      <translation>Kuvan profiili</translation>
    </message>
    <message>
      <source>Rendering intent for the image</source>
      <translation>Kuvan sovitustapa</translation>
    </message>
    <message>
      <source>Switches between Gap or Column width</source>
      <translation>Vaihda palstan tai palstojen välin leveys</translation>
    </message>
    <message>
      <source>Column width</source>
      <translation>Palstan leveys</translation>
    </message>
    <message>
      <source>Path Text Properties</source>
      <translation>Polulla olevan tekstin ominaisuudet</translation>
    </message>
    <message>
      <source>Make text in lower frames flow around the object shape</source>
      <translation>Pakota alempien kehysten teksti kiertämään kohteen muoto</translation>
    </message>
    <message>
      <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
      <translation>Ilmaisee kerroksen jolla kohde on. 0 kertoo kohteen olevan alimmaisena</translation>
    </message>
    <message>
      <source>X, Y, &amp;Z</source>
      <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
      <source>&amp;Shape</source>
      <translation>&amp;Kuvio</translation>
    </message>
    <message>
      <source>&amp;Text</source>
      <translation>&amp;Teksti</translation>
    </message>
    <message>
      <source>&amp;Image</source>
      <translation>K&amp;uva</translation>
    </message>
    <message>
      <source>&amp;Line</source>
      <translation>&amp;Viiva</translation>
    </message>
    <message>
      <source>&amp;Colors</source>
      <translation>V&amp;ärit</translation>
    </message>
    <message>
      <source>&amp;X-Pos:</source>
      <translation>&amp;X:</translation>
    </message>
    <message>
      <source>&amp;Y-Pos:</source>
      <translation>&amp;Y:</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>&amp;Leveys:</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>&amp;Korkeus:</translation>
    </message>
    <message>
      <source>&amp;Rotation:</source>
      <translation>Ki&amp;erto:</translation>
    </message>
    <message>
      <source>Text &amp;Flows Around Frame</source>
      <translation>&amp;Teksti kiertää kehyksen</translation>
    </message>
    <message>
      <source>Use &amp;Bounding Box</source>
      <translation>Käytä &amp;rajausaluetta</translation>
    </message>
    <message>
      <source>&amp;Use Contour Line</source>
      <translation>&amp;Käytä reunusta</translation>
    </message>
    <message>
      <source>&amp;Edit Shape...</source>
      <translation>&amp;Muokkaa kuviota...</translation>
    </message>
    <message>
      <source>R&amp;ound
Corners:</source>
      <translation>&amp;Pyöristä kulmia:</translation>
    </message>
    <message>
      <source>Colu&amp;mns:</source>
      <translation>&amp;Palstoja:</translation>
    </message>
    <message>
      <source>&amp;Gap:</source>
      <translation>&amp;Väli:</translation>
    </message>
    <message>
      <source>To&amp;p:</source>
      <translation>&amp;Ylhäällä:</translation>
    </message>
    <message>
      <source>&amp;Bottom:</source>
      <translation>&amp;Alhaalla:</translation>
    </message>
    <message>
      <source>&amp;Left:</source>
      <translation>&amp;Vasen:</translation>
    </message>
    <message>
      <source>&amp;Right:</source>
      <translation>&amp;Oikea:</translation>
    </message>
    <message>
      <source>T&amp;abulators...</source>
      <translation>&amp;Sarkaimet...</translation>
    </message>
    <message>
      <source>St&amp;yle:</source>
      <translation>T&amp;yyli:</translation>
    </message>
    <message>
      <source>Lan&amp;guage:</source>
      <translation>&amp;Kieli:</translation>
    </message>
    <message>
      <source>&amp;Free Scaling</source>
      <translation>&amp;Ei skaalausta</translation>
    </message>
    <message>
      <source>X-Sc&amp;ale:</source>
      <translation>Skaalaa &amp;vaakatasossa:</translation>
    </message>
    <message>
      <source>Y-Scal&amp;e:</source>
      <translation>Skaalaa &amp;pystytasossa:</translation>
    </message>
    <message>
      <source>Scale &amp;To Frame Size</source>
      <translation>Skaalaa &amp;kehyksen kokoon</translation>
    </message>
    <message>
      <source>P&amp;roportional</source>
      <translation>&amp;Säilytä suhteet</translation>
    </message>
    <message>
      <source>&amp;Basepoint:</source>
      <translation>&amp;Toimintapiste:</translation>
    </message>
    <message>
      <source>T&amp;ype of Line:</source>
      <translation>&amp;Viivan tyyli:</translation>
    </message>
    <message>
      <source>Line &amp;Width:</source>
      <translation>Viivan &amp;leveys:</translation>
    </message>
    <message>
      <source>Ed&amp;ges:</source>
      <translation>&amp;Kulmat:</translation>
    </message>
    <message>
      <source>&amp;Endings:</source>
      <translation>&amp;Päät:</translation>
    </message>
    <message>
      <source>&amp;X1:</source>
      <translation>&amp;X1:</translation>
    </message>
    <message>
      <source>X&amp;2:</source>
      <translation>X&amp;2:</translation>
    </message>
    <message>
      <source>Y&amp;1:</source>
      <translation>Y&amp;1:</translation>
    </message>
    <message>
      <source>&amp;Y2:</source>
      <translation>&amp;Y2:</translation>
    </message>
    <message>
      <source>Hyphenation language of frame</source>
      <translation>Kehyksen tavutuksen kieli</translation>
    </message>
    <message>
      <source>Use a surrounding box instead of the frame's shape for text flow</source>
      <translation>Käytä ympäröivää kehystä kohteen muodon sijaan tekstin kierron rajana</translation>
    </message>
    <message>
      <source>Use a second line originally based on the frame's shape for text flow</source>
      <translation>Käytä vapaasti muokattavaa reunusta tekstin kierron rajana</translation>
    </message>
    <message>
      <source>Right to Left Writing</source>
      <translation>Kirjoitus oikealta vasemmalle</translation>
    </message>
    <message>
      <source>Manual Tracking</source>
      <translation>Oma merkkiväli</translation>
    </message>
    <message>
      <source>Fixed Linespacing</source>
      <translation>Riviväli kiinteällä leveydellä</translation>
    </message>
    <message>
      <source>Automatic Linespacing</source>
      <translation>Riviväli automaattisella leveydellä</translation>
    </message>
    <message>
      <source>Align to Baseline Grid</source>
      <translation>Kiinnitä peruslinjaan</translation>
    </message>
    <message>
      <source>Actual X-DPI:</source>
      <translation>Todellinen X-DPI:</translation>
    </message>
    <message>
      <source>Actual Y-DPI:</source>
      <translation>Todellinen Y-DPI:</translation>
    </message>
    <message>
      <source>Start Arrow:</source>
      <translation>Nuolen alku:</translation>
    </message>
    <message>
      <source>End Arrow:</source>
      <translation>Nuolen loppu:</translation>
    </message>
    <message>
      <source>Offset to baseline of characters</source>
      <translation>Siirtymä merkkien peruslinjaan verrattuna</translation>
    </message>
    <message>
      <source>Scaling height of characters</source>
      <translation>Merkkien korkeus</translation>
    </message>
    <message>
      <source>Name &quot;%1&quot; isn't unique.&lt;br/>Please choose another.</source>
      <translation>Nimi %1 on jo käytössä.&lt;br/>Ole hyvä ja valitse toinen.</translation>
    </message>
    <message>
      <source>Color of text stroke. Only available with &quot;outline&quot; text decoration.</source>
      <translation>Tekstin ääriviivan väri. Käytettävissä tekstille ääriviivaefektin kanssa.</translation>
    </message>
    <message>
      <source>Color of text fill. Only available with &quot;outline&quot; text decoration.</source>
      <translation>Tekstin täytön väri. Käytettävissä tekstille ääriviivaefektin kanssa.</translation>
    </message>
  </context>
  <context>
    <name>MultiLine</name>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source> pt </source>
      <translation> pt </translation>
    </message>
    <message>
      <source>Flat Cap</source>
      <translation>Litteä</translation>
    </message>
    <message>
      <source>Dash Dot Dot Line</source>
      <translation>Viiva-piste-piste-viiva</translation>
    </message>
    <message>
      <source>Round Join</source>
      <translation>Pyöreä</translation>
    </message>
    <message>
      <source>Line Width:</source>
      <translation>Viivan leveys:</translation>
    </message>
    <message>
      <source>Edit Style</source>
      <translation>Muokkaa tyylejä</translation>
    </message>
    <message>
      <source>Square Cap</source>
      <translation>Neliö</translation>
    </message>
    <message>
      <source>Dashed Line</source>
      <translation>Katkoviiva</translation>
    </message>
    <message>
      <source>Dotted Line</source>
      <translation>Pisteviiva</translation>
    </message>
    <message>
      <source>Bevel Join</source>
      <translation>Tasainen</translation>
    </message>
    <message>
      <source>Miter Join</source>
      <translation>Viisto</translation>
    </message>
    <message>
      <source>Round Cap</source>
      <translation>Pyöreä</translation>
    </message>
    <message>
      <source>Dash Dot Line</source>
      <translation>Viiva-piste-viiva</translation>
    </message>
    <message>
      <source>Solid Line</source>
      <translation>Yhtenäinen viiva</translation>
    </message>
    <message>
      <source>Name &quot;%1&quot; isn't unique.&lt;br/>Please choose another.</source>
      <translation>Nimi %1 on jo käytössä.&lt;br/>Ole hyvä ja valitse toinen.</translation>
    </message>
  </context>
  <context>
    <name>MultiProgressDialogBase</name>
    <message>
      <source>Progress</source>
      <translation>Edistyminen</translation>
    </message>
    <message>
      <source>Overall Progress:</source>
      <translation>Kokonaisedistyminen:</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Peruuta</translation>
    </message>
  </context>
  <context>
    <name>MyPlugin</name>
    <message>
      <source>My &amp;Plugin</source>
      <translation>&amp;Oma liitännäinen</translation>
    </message>
  </context>
  <context>
    <name>MyPluginImpl</name>
    <message>
      <source>Scribus - My Plugin</source>
      <translation>Scribus - Oma liitännäinen</translation>
    </message>
    <message>
      <source>The plugin worked!</source>
      <translation>Liitännäinen toimi</translation>
    </message>
  </context>
  <context>
    <name>NewDoc</name>
    <message>
      <source>New Document</source>
      <translation>Uusi asiakirja</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>Vaakasuora</translation>
    </message>
    <message>
      <source>Options</source>
      <translation>Asetukset</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>Pystysuora</translation>
    </message>
    <message>
      <source>Margin Guides</source>
      <translation>Sivun reunukset</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>Sivun koko</translation>
    </message>
    <message>
      <source>Document page size, either a standard size or a custom size</source>
      <translation>Asiakirjan sivun koko. Standardi tai itse määrittämäsi koko</translation>
    </message>
    <message>
      <source>Orientation of the document's pages</source>
      <translation>Asiakirjan sivujen suunta</translation>
    </message>
    <message>
      <source>Width of the document's pages, editable if you have chosen a custom page size</source>
      <translation>Asiakirjan sivujen leveys. Muokattavissa, jos valitset kooksi &quot;Oma&quot;</translation>
    </message>
    <message>
      <source>Height of the document's pages, editable if you have chosen a custom page size</source>
      <translation>Asiakirjan sivujen korkeus. Muokattavissa jos valitset kooksi &quot;Oma&quot;</translation>
    </message>
    <message>
      <source>First page number of the document</source>
      <translation>Asiakirjan ensimmäinen sivunumero</translation>
    </message>
    <message>
      <source>Default unit of measurement for document editing</source>
      <translation>Asiakirjan muokkauksessa käytettävä mittayksikkö</translation>
    </message>
    <message>
      <source>Create text frames automatically when new pages are added</source>
      <translation>Luo tekstikehykset automaattisesti uusia sivuja lisättäessä</translation>
    </message>
    <message>
      <source>Distance between automatically created columns</source>
      <translation>Automaattisesti luotujen tekstikehysten palstojen väli</translation>
    </message>
    <message>
      <source>Number of columns to create in automatically created text frames</source>
      <translation>Automaattisesti luotujen tekstikehysten palstojen lukumäärä</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Koko:</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>&amp;Leveys:</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>Ko&amp;rkeus:</translation>
    </message>
    <message>
      <source>&amp;Automatic Text Frames</source>
      <translation>A&amp;utomaattiset tekstikehykset</translation>
    </message>
    <message>
      <source>&amp;Gap:</source>
      <translation>&amp;Väli:</translation>
    </message>
    <message>
      <source>Colu&amp;mns:</source>
      <translation>Pal&amp;stoja:</translation>
    </message>
    <message>
      <source>Orie&amp;ntation:</source>
      <translation>Suu&amp;nta:</translation>
    </message>
    <message>
      <source>F&amp;irst Page Number:</source>
      <translation>&amp;Ensimmäisen sivun numero:</translation>
    </message>
    <message>
      <source>&amp;Default Unit:</source>
      <translation>Ol&amp;etusyksikkö:</translation>
    </message>
    <message>
      <source>Do not show this dialog again</source>
      <translation>Älä näytä tätä ikkunaa uudestaan</translation>
    </message>
    <message>
      <source>Initial number of pages of the document</source>
      <translation>Asiakirjan sivujen lukumäärä</translation>
    </message>
    <message>
      <source>N&amp;umber of Pages:</source>
      <translation>&amp;Sivujen lukumäärä:</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Avaa</translation>
    </message>
    <message>
      <source>&amp;New Document</source>
      <translation>&amp;Uusi asiakirja</translation>
    </message>
    <message>
      <source>Open &amp;Existing Document</source>
      <translation>&amp;Avaa asiakirja</translation>
    </message>
    <message>
      <source>Open Recent &amp;Document</source>
      <translation>Avaa &amp;viimeaikainen asiakirja</translation>
    </message>
  </context>
  <context>
    <name>NewFromTemplatePlugin</name>
    <message>
      <source>New &amp;from Template...</source>
      <translation>Uusi &amp;mallista...</translation>
    </message>
    <message>
      <source>Load documents with predefined layout</source>
      <translation>Lataa asiakirja esimääritetyllä asettelulla</translation>
    </message>
    <message>
      <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
      <translation>Aloita asiakirja mallista.</translation>
    </message>
  </context>
  <context>
    <name>NodePalette</name>
    <message>
      <source>Nodes</source>
      <translation>Solmut</translation>
    </message>
    <message>
      <source>Reset this Control Point</source>
      <translation>Palauta tämä hallintapiste</translation>
    </message>
    <message>
      <source>Delete Nodes</source>
      <translation>Poista solmu</translation>
    </message>
    <message>
      <source>Add Nodes</source>
      <translation>Lisää solmu</translation>
    </message>
    <message>
      <source>Move Nodes</source>
      <translation>Siirrä solmuja</translation>
    </message>
    <message>
      <source>Move Control Points</source>
      <translation>Siirrä hallintapisteitä</translation>
    </message>
    <message>
      <source>Reset Control Points</source>
      <translation>Palauta hallintapisteet</translation>
    </message>
    <message>
      <source>&amp;Absolute Coordinates</source>
      <translation>&amp;Absoluuttiset koordinaatit</translation>
    </message>
    <message>
      <source>&amp;X-Pos:</source>
      <translation>&amp;X:</translation>
    </message>
    <message>
      <source>&amp;Y-Pos:</source>
      <translation>&amp;Y:</translation>
    </message>
    <message>
      <source>Edit &amp;Contour Line</source>
      <translation>Muokkaa &amp;reunusta</translation>
    </message>
    <message>
      <source>&amp;Reset Contour Line</source>
      <translation>&amp;Palauta reunus</translation>
    </message>
    <message>
      <source>&amp;End Editing</source>
      <translation>&amp;Lopeta muokkaus</translation>
    </message>
    <message>
      <source>Move Control Points Independently</source>
      <translation>Siirrä hallintapisteitä itsenäisesti</translation>
    </message>
    <message>
      <source>Move Control Points Symmetrical</source>
      <translation>Siirrä hallintapisteitä symmetrisesti</translation>
    </message>
    <message>
      <source>Open a Polygon or Cuts a Bezier Curve</source>
      <translation>Avaa monikulmio tai katkaise Bezier-viiva</translation>
    </message>
    <message>
      <source>Close this Bezier Curve</source>
      <translation>Sulje tämä Bezier-viiva</translation>
    </message>
    <message>
      <source>Mirror the Path Horizontally</source>
      <translation>Peilaa polku pystysuunnassa</translation>
    </message>
    <message>
      <source>Mirror the Path Vertically</source>
      <translation>Peilaa polku vaakasuunnassa</translation>
    </message>
    <message>
      <source>Shear the Path Horizontally to the Left</source>
      <translation>Taivuta polkua vasemmalle</translation>
    </message>
    <message>
      <source>Shear the Path Vertically Up</source>
      <translation>Taivuta polkua ylöspäin</translation>
    </message>
    <message>
      <source>Shear the Path Vertically Down</source>
      <translation>Taivuta polkua alaspäin</translation>
    </message>
    <message>
      <source>Rotate the Path Counter-Clockwise</source>
      <translation>Kierrä polkua vastapäivään</translation>
    </message>
    <message>
      <source>Rotate the Path Clockwise</source>
      <translation>Kierrä polkua myötäpäivään</translation>
    </message>
    <message>
      <source>Reduce the Size of the Path by shown %</source>
      <translation>Pienennä polun kokoa</translation>
    </message>
    <message>
      <source>Enlarge the Size of the Path by shown %</source>
      <translation>Suurenna polun kokoa</translation>
    </message>
    <message>
      <source>Angle of Rotation</source>
      <translation>Kierron kulma</translation>
    </message>
    <message>
      <source>% to Enlarge or Reduce By</source>
      <translation>Pienennyksen ja suurennoksen suuruus</translation>
    </message>
    <message>
      <source>Activate Contour Line Editing Mode</source>
      <translation>Muokkaa tekstin kierron reunusta</translation>
    </message>
    <message>
      <source>Reset the Contour Line to the Original Shape of the Frame</source>
      <translation>Palauta tekstin kierron reunus alkuperäiseen muotoonsa</translation>
    </message>
    <message>
      <source>Shear the Path Horizontally to the Right</source>
      <translation>Taivuta polkua oikealle</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
      <translation>Koordinaatit ovat suhteessa sivuun, muuten suhteessa objektiin.</translation>
    </message>
  </context>
  <context>
    <name>OODPlug</name>
    <message>
      <source>This document does not seem to be an OpenOffice Draw file.</source>
      <translation>Valittu asiakirja ei ole OpenOffice Draw -tiedosto.</translation>
    </message>
    <message>
      <source>OpenOffice Draw 2.0 documents are currently not supported.</source>
      <translation>OpenOffice Draw 2.0 -asiakirjat ei ole tuettuja.</translation>
    </message>
  </context>
  <context>
    <name>OODrawImportPlugin</name>
    <message>
      <source>Import &amp;OpenOffice.org Draw...</source>
      <translation>Tuo &amp;OpenOffice.org Draw -piirros...</translation>
    </message>
    <message>
      <source>Imports OpenOffice.org Draw Files</source>
      <translation>Tuo OpenOffice.org Draw -tiedostoja</translation>
    </message>
    <message>
      <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
      <translation>Tuo OpenOffice.org Draw -tiedoston asiakirjaan. Vektorigrafiikka muutetaan Scribus-objekteiksi.</translation>
    </message>
    <message>
      <source>OpenDocument 1.0 Draw</source>
      <comment>Import/export format name</comment>
      <translation>OpenDocument 1.0 Draw</translation>
    </message>
    <message>
      <source>OpenOffice.org 1.x Draw</source>
      <comment>Import/export format name</comment>
      <translation>OpenOffice.org 1.x Draw</translation>
    </message>
  </context>
  <context>
    <name>OdtDialog</name>
    <message>
      <source>Use document name as a prefix for paragraph styles</source>
      <translation>Käytä asiakirjan nimeä kappaletyylin edessä</translation>
    </message>
    <message>
      <source>Do not ask again</source>
      <translation>Älä kysy uudestaan</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>OpenDocument Importer Options</source>
      <translation>OpenDocument-tuojan asetukset</translation>
    </message>
    <message>
      <source>Enabling this will overwrite existing styles in the current Scribus document</source>
      <translation>Ylikirjoita olemassa olevat kappaletyylit</translation>
    </message>
    <message>
      <source>Merge Paragraph Styles</source>
      <translation>Yhdistä kappaletyylit</translation>
    </message>
    <message>
      <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document's styles are named differently.</source>
      <translation>Yhdistää kappaletyylit tyylien asetusten perusteella. Tuloksena on vähemmän kappaletyylejä.</translation>
    </message>
    <message>
      <source>Prepend the document name to the paragraph style name in Scribus.</source>
      <translation>Käytä tuotavan asiakirjan nimeä kappaletyylin nimessä.</translation>
    </message>
    <message>
      <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
      <translation>Aseta tämänhetkiset valinnat oletusarvoiksi äläkä kysy asetuksia uudestaan tuotaessa OASIS OpenDocument-tiedostosta.</translation>
    </message>
    <message>
      <source>Overwrite Paragraph Styles</source>
      <translation>Korvaa kappaletyylit</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Peruuta</translation>
    </message>
  </context>
  <context>
    <name>OldScribusFormat</name>
    <message>
      <source>Scribus Document</source>
      <translation>Scribus-asiakirja</translation>
    </message>
    <message>
      <source>Scribus 1.2.x Document</source>
      <translation>Scribus 1.2.x -asiakirja</translation>
    </message>
  </context>
  <context>
    <name>OutlineValues</name>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Linewidth</source>
      <translation>Viivan leveys</translation>
    </message>
  </context>
  <context>
    <name>PDFExportDialog</name>
    <message>
      <source>Save as PDF</source>
      <translation>Tallenna PDF-tiedostoon</translation>
    </message>
    <message>
      <source>O&amp;utput to File:</source>
      <translation>&amp;Tulosta tiedostoon:</translation>
    </message>
    <message>
      <source>Cha&amp;nge...</source>
      <translation>&amp;Muuta...</translation>
    </message>
    <message>
      <source>Output one file for eac&amp;h page</source>
      <translation>&amp;Vie yksi sivu per tiedosto</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Tallenna</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>Tallenna nimellä</translation>
    </message>
    <message>
      <source>PDF Files (*.pdf);;All Files (*)</source>
      <translation>PDF-tiedostot (*.pdf);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
      <translation>Tallentaa jokaisen sivun omaan PDF-tiedostoon. Sivunumerot lisätään automaattisesti. Tämä saattaa helpottaa sivujen asemointia.</translation>
    </message>
  </context>
  <context>
    <name>PDFlib</name>
    <message>
      <source>Saving PDF</source>
      <translation>PDF-tallennus</translation>
    </message>
    <message>
      <source>Exporting Master Pages:</source>
      <translation>Viedään mallisivuja:</translation>
    </message>
    <message>
      <source>Exporting Pages:</source>
      <translation>Viedään sivuja:</translation>
    </message>
    <message>
      <source>Exporting Items on Current Page:</source>
      <translation>Viedään kohteita sivulta:</translation>
    </message>
  </context>
  <context>
    <name>PPreview</name>
    <message>
      <source>All</source>
      <translation>Kaikki</translation>
    </message>
    <message>
      <source>Print Preview</source>
      <translation>Tulostuksen esikatselu</translation>
    </message>
    <message>
      <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
      <translation>Mahdollistaa asiakirjan läpinäkyvyyden näyttämisen. Vaatii Ghostscript 7.07:n tai uudemman version</translation>
    </message>
    <message>
      <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
      <translation>Esikatselu jäljittelee CMYK musteita RGB-värien sijaan</translation>
    </message>
    <message>
      <source>Enable/disable the C (Cyan) ink plate</source>
      <translation>Näytä/Älä näytä C (syaani) muste</translation>
    </message>
    <message>
      <source>Enable/disable the M (Magenta) ink plate</source>
      <translation>Näytä/Älä näytä M (magenta) muste</translation>
    </message>
    <message>
      <source>Enable/disable the Y (Yellow) ink plate</source>
      <translation>Näytä/Älä näytä Y (keltainen) muste</translation>
    </message>
    <message>
      <source>Enable/disable the K (Black) ink plate</source>
      <translation>Näytä/Älä näytä K (musta) muste</translation>
    </message>
    <message>
      <source>Anti-alias &amp;Text</source>
      <translation>&amp;Reunanpehmennys tekstiin</translation>
    </message>
    <message>
      <source>Anti-alias &amp;Graphics</source>
      <translation>Reunanpehmennys &amp;grafiikkaan</translation>
    </message>
    <message>
      <source>Display Trans&amp;parency</source>
      <translation>Näytä &amp;läpinäkyvyys</translation>
    </message>
    <message>
      <source>&amp;Display CMYK</source>
      <translation>Näytä &amp;CMYK</translation>
    </message>
    <message>
      <source>&amp;C</source>
      <translation>&amp;C</translation>
    </message>
    <message>
      <source>&amp;M</source>
      <translation>&amp;M</translation>
    </message>
    <message>
      <source>&amp;Y</source>
      <translation>&amp;Y</translation>
    </message>
    <message>
      <source>&amp;K</source>
      <translation>&amp;K</translation>
    </message>
    <message>
      <source>&amp;Under Color Removal</source>
      <translation>&amp;Alivärin poisto (UCR)</translation>
    </message>
    <message>
      <source>Separation Name</source>
      <translation>Osavärin nimi</translation>
    </message>
    <message>
      <source>Cyan</source>
      <translation>Syaani</translation>
    </message>
    <message>
      <source>Magenta</source>
      <translation>Magenta</translation>
    </message>
    <message>
      <source>Yellow</source>
      <translation>Keltainen</translation>
    </message>
    <message>
      <source>Black</source>
      <translation>Musta</translation>
    </message>
    <message>
      <source>Scaling:</source>
      <translation>Skaalaus:</translation>
    </message>
    <message>
      <source>Close</source>
      <translation>Sulje</translation>
    </message>
    <message>
      <source>Print...</source>
      <translation>Tulosta...</translation>
    </message>
    <message>
      <source>Provides a more pleasant view of text items in the viewer, at the expense of a slight slowdown in previewing. This only affects Type 1 fonts</source>
      <translation>Parantaa tekstikohteitten ulkonäkö esikatselun nopeuden kustannuksella. Koskee ainoastaan Type 1 -kirjaintyyppejä</translation>
    </message>
    <message>
      <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
      <translation>Parantaa True Type sekä Open Type kirjaintyyppien että EPS, PDF ja vektorigrafiikan esitystä nopeuden kustannuksella.</translation>
    </message>
    <message>
      <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
      <translation>Tapa vaihtaa C, Y ja M värien tekemä harmaa mustalla tehdyksi. 
UCR vaikuttaa kuvien kohdissa, jotka ovat neutraaleja tai lähellä harmaata 
olevia tummia värejä. UCR:n käyttäminen saattaa parantaa joidenkin kuvien
tulostamista. UCR parantaa kykyä välttää värien liikakylläisyyttä CMY-musteilla. 
Parhaan tuloksen saavuttamiseksi tarvitaan tapauskohtaista testausta. UCR 
vähentää ylikylläisyyden vaaraa CMY-musteilla.</translation>
    </message>
    <message>
      <source>Resize the scale of the page.</source>
      <translation>Muuta sivun kokoa.</translation>
    </message>
    <message>
      <source>File</source>
      <translation>Tiedosto</translation>
    </message>
  </context>
  <context>
    <name>PSLib</name>
    <message>
      <source>Processing Master Pages:</source>
      <translation>Työstetään mallisivuja:</translation>
    </message>
    <message>
      <source>Exporting Pages:</source>
      <translation>Viedään sivuja:</translation>
    </message>
  </context>
  <context>
    <name>PageItem</name>
    <message>
      <source>Image</source>
      <translation>Kuva</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>Teksti</translation>
    </message>
    <message>
      <source>Line</source>
      <translation>Viiva</translation>
    </message>
    <message>
      <source>Polygon</source>
      <translation>Monikulmio</translation>
    </message>
    <message>
      <source>Polyline</source>
      <translation>Kulmikas viiva</translation>
    </message>
    <message>
      <source>PathText</source>
      <translation>Teksti polulla</translation>
    </message>
    <message>
      <source>Copy of</source>
      <translation>Kopio</translation>
    </message>
  </context>
  <context>
    <name>PageItemAttributes</name>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Relates To</source>
      <translation>Suhteessa</translation>
    </message>
    <message>
      <source>Is Parent Of</source>
      <translation>On vanhempi</translation>
    </message>
    <message>
      <source>Is Child Of</source>
      <translation>On lapsi</translation>
    </message>
  </context>
  <context>
    <name>PageItemAttributesBase</name>
    <message>
      <source>Page Item Attributes</source>
      <translation>Kohteen muuttujat</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Nimi</translation>
    </message>
    <message>
      <source>Type</source>
      <translation>Tyyppi</translation>
    </message>
    <message>
      <source>Value</source>
      <translation>Arvo</translation>
    </message>
    <message>
      <source>Parameter</source>
      <translation>Parametri</translation>
    </message>
    <message>
      <source>Relationship</source>
      <translation>Suhde</translation>
    </message>
    <message>
      <source>Relationship To</source>
      <translation>Suhteessa</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation>&amp;Lisää</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>&amp;Kopioi</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation>Alt+C</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>P&amp;oista</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>T&amp;yhjennä</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation>Alt+L</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation>&amp;Peruuta</translation>
    </message>
  </context>
  <context>
    <name>PageLayouts</name>
    <message>
      <source>Page Layout</source>
      <translation>Sivun asettelu</translation>
    </message>
    <message>
      <source>First Page is:</source>
      <translation>Ensimmäinen sivu on:</translation>
    </message>
  </context>
  <context>
    <name>PagePalette</name>
    <message>
      <source>Double sided</source>
      <translation>Kaksipuolinen</translation>
    </message>
    <message>
      <source>Middle Right</source>
      <translation>Keskimmäinen oikea</translation>
    </message>
    <message>
      <source>Drag pages or master pages onto the trashbin to delete them</source>
      <translation>Raahaa sivut tai mallisivut roskakoriin tuhotaksesi ne</translation>
    </message>
    <message>
      <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
      <translation>Tässä ovat kaikki mallisivusi. Luodaksesi uuden sivun raahaa mallisivu paikkaan johon haluat uuden sivun</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>Normaali</translation>
    </message>
    <message>
      <source>Arrange Pages</source>
      <translation>Sivujen hallinta</translation>
    </message>
    <message>
      <source>Available Master Pages:</source>
      <translation>Käytössä olevat mallisivut:</translation>
    </message>
    <message>
      <source>Document Pages:</source>
      <translation>Asiakirjan sivut:</translation>
    </message>
  </context>
  <context>
    <name>PageSelector</name>
    <message>
      <source>%1 of %1</source>
      <translation>%1/%1</translation>
    </message>
    <message>
      <source>%1 of %2</source>
      <translation>%1/%2</translation>
    </message>
  </context>
  <context>
    <name>PageSize</name>
    <message>
      <source>Quarto</source>
      <translation>Quarto</translation>
    </message>
    <message>
      <source>Foolscap</source>
      <translation>Foolscap</translation>
    </message>
    <message>
      <source>Letter</source>
      <translation>Letter</translation>
    </message>
    <message>
      <source>Government Letter</source>
      <translation>Government Letter</translation>
    </message>
    <message>
      <source>Legal</source>
      <translation>Legal</translation>
    </message>
    <message>
      <source>Ledger</source>
      <translation>Ledger</translation>
    </message>
    <message>
      <source>Executive</source>
      <translation>Executive</translation>
    </message>
    <message>
      <source>Post</source>
      <translation>Post</translation>
    </message>
    <message>
      <source>Crown</source>
      <translation>Crown</translation>
    </message>
    <message>
      <source>Large Post</source>
      <translation>Large Post</translation>
    </message>
    <message>
      <source>Demy</source>
      <translation>Demy</translation>
    </message>
    <message>
      <source>Medium</source>
      <translation>Keskikoko</translation>
    </message>
    <message>
      <source>Royal</source>
      <translation>Royal</translation>
    </message>
    <message>
      <source>Elephant</source>
      <translation>Elephant</translation>
    </message>
    <message>
      <source>Double Demy</source>
      <translation>Double Demy</translation>
    </message>
    <message>
      <source>Quad Demy</source>
      <translation>Quad Demy</translation>
    </message>
    <message>
      <source>STMT</source>
      <translation>STMT</translation>
    </message>
    <message>
      <source>A</source>
      <translation>A</translation>
    </message>
    <message>
      <source>B</source>
      <translation>B</translation>
    </message>
    <message>
      <source>C</source>
      <translation>C</translation>
    </message>
    <message>
      <source>D</source>
      <translation>D</translation>
    </message>
    <message>
      <source>E</source>
      <translation>E</translation>
    </message>
  </context>
  <context>
    <name>PicSearch</name>
    <message>
      <source>Cancel</source>
      <translation>Peruuta</translation>
    </message>
    <message>
      <source>Result</source>
      <translation>Tulos</translation>
    </message>
    <message>
      <source>Select</source>
      <translation>Valitse</translation>
    </message>
    <message>
      <source>Search Results for: </source>
      <translation>Etsi tuloksia: </translation>
    </message>
    <message>
      <source>Preview</source>
      <translation>Esikatselu</translation>
    </message>
  </context>
  <context>
    <name>PicStatus</name>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Yes</source>
      <translation>Kyllä</translation>
    </message>
    <message>
      <source>Goto</source>
      <translation>Mene</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Nimi</translation>
    </message>
    <message>
      <source>Page</source>
      <translation>Sivu</translation>
    </message>
    <message>
      <source>Path</source>
      <translation>Polku</translation>
    </message>
    <message>
      <source>Print</source>
      <translation>Tulosta</translation>
    </message>
    <message>
      <source>Missing</source>
      <translation>Puuttuu</translation>
    </message>
    <message>
      <source>Search</source>
      <translation>Etsi</translation>
    </message>
    <message>
      <source>Status</source>
      <translation>Tila</translation>
    </message>
    <message>
      <source>Cancel Search</source>
      <translation>Peruuta etsintä</translation>
    </message>
    <message>
      <source>Manage Pictures</source>
      <translation>Kuvien hallinta</translation>
    </message>
    <message>
      <source>Scribus - Image Search</source>
      <translation>Scribus - Kuvien etsintä</translation>
    </message>
    <message>
      <source>The search failed: %1</source>
      <translation>Etsintä epäonnistui: %1</translation>
    </message>
    <message>
      <source>No images named &quot;%1&quot; were found.</source>
      <translation>Kuvia nimellä &quot;%1&quot; ei löytynyt.</translation>
    </message>
    <message>
      <source>Select a base directory for search</source>
      <translation>Valitse juurihakemisto etsinnälle</translation>
    </message>
  </context>
  <context>
    <name>PixmapExportPlugin</name>
    <message>
      <source>Save as &amp;Image...</source>
      <translation>Tallenna &amp;kuvana...</translation>
    </message>
    <message>
      <source>Export As Image</source>
      <translation>Vie kuvana</translation>
    </message>
    <message>
      <source>Exports selected pages as bitmap images.</source>
      <translation>Vie valitut sivut bittikarttakuvina.</translation>
    </message>
  </context>
  <context>
    <name>PluginManager</name>
    <message>
      <source>Cannot find plugin</source>
      <comment>plugin manager</comment>
      <translation>Liitännäistä ei löydy</translation>
    </message>
    <message>
      <source>unknown error</source>
      <comment>plugin manager</comment>
      <translation>tuntematon virhe</translation>
    </message>
    <message>
      <source>Cannot find symbol (%1)</source>
      <comment>plugin manager</comment>
      <translation>Symbolia &quot;%1&quot; ei löydy</translation>
    </message>
    <message>
      <source>Plugin: loading %1</source>
      <comment>plugin manager</comment>
      <translation>Liitännäistä %1 ladataan</translation>
    </message>
    <message>
      <source>init failed</source>
      <comment>plugin load error</comment>
      <translation>alustus epäonnistui</translation>
    </message>
    <message>
      <source>unknown plugin type</source>
      <comment>plugin load error</comment>
      <translation>tuntematon laajennuksen tyyppi</translation>
    </message>
    <message>
      <source>Plugin: %1 loaded</source>
      <comment>plugin manager</comment>
      <translation>Liitännäinen %1 ladattu</translation>
    </message>
    <message>
      <source>Plugin: %1 failed to load: %2</source>
      <comment>plugin manager</comment>
      <translation>Liitännäisen: lataaminen epäonnistui %1 %2</translation>
    </message>
  </context>
  <context>
    <name>PluginManagerPrefsGui</name>
    <message>
      <source>Plugin Manager</source>
      <translation>Liitännäisten hallinta</translation>
    </message>
    <message>
      <source>Plugin</source>
      <translation>Liitännäinen</translation>
    </message>
    <message>
      <source>How to run</source>
      <translation>Miten ajetaan</translation>
    </message>
    <message>
      <source>Type</source>
      <translation>Tyyppi</translation>
    </message>
    <message>
      <source>Load it?</source>
      <translation>Lataa?</translation>
    </message>
    <message>
      <source>Plugin ID</source>
      <translation>Laajennoksen ID</translation>
    </message>
    <message>
      <source>File</source>
      <translation>Tiedosto</translation>
    </message>
    <message>
      <source>Yes</source>
      <translation>Kyllä</translation>
    </message>
    <message>
      <source>No</source>
      <translation>Ei</translation>
    </message>
    <message>
      <source>You need to restart the application to apply the changes.</source>
      <translation>Ohjelma täytyy käynnistää uudestaan jotta muutokset tulevat voimaan.</translation>
    </message>
  </context>
  <context>
    <name>PolygonProps</name>
    <message>
      <source>Polygon Properties</source>
      <translation>Monikulmion ominaisuudet</translation>
    </message>
  </context>
  <context>
    <name>PolygonWidget</name>
    <message>
      <source>Corn&amp;ers:</source>
      <translation>&amp;Kulmat:</translation>
    </message>
    <message>
      <source>&amp;Rotation:</source>
      <translation>Ki&amp;erto:</translation>
    </message>
    <message>
      <source>Apply &amp;Factor</source>
      <translation>Käytä &amp;kerrointa</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>&amp;Factor:</source>
      <translation>&amp;Kerroin:</translation>
    </message>
    <message>
      <source>Number of corners for polygons</source>
      <translation>Monikulmion kulmien lukumäärä</translation>
    </message>
    <message>
      <source>Degrees of rotation for polygons</source>
      <translation>Monikulmion kierron aste</translation>
    </message>
    <message>
      <source>Apply Convex/Concave Factor to change shape of Polygons</source>
      <translation>Käytä koveruus-/kuperuuskerrointa muuttaaksesi kohteen muotoa.</translation>
    </message>
    <message>
      <source>Sample Polygon</source>
      <translation>Esimerkki monikulmiosta</translation>
    </message>
    <message>
      <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
      <translation>Negatiivinen arvo tekee monikulmiosta koveran (tai tähden muotoisen), positiivinen arvo tekee siitä kuperan.</translation>
    </message>
  </context>
  <context>
    <name>Preferences</name>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>GUI</source>
      <translation>Käyttöliittymä</translation>
    </message>
    <message>
      <source>min</source>
      <translation>min</translation>
    </message>
    <message>
      <source>Paths</source>
      <translation>Hakemistot</translation>
    </message>
    <message>
      <source>Small</source>
      <translation>Pieni</translation>
    </message>
    <message>
      <source>Tools</source>
      <translation>Työkalut</translation>
    </message>
    <message>
      <source>Other Options</source>
      <translation>Muut asetukset</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>Oma</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>Vaakasuora</translation>
    </message>
    <message>
      <source>Guides</source>
      <translation>Apuviivat</translation>
    </message>
    <message>
      <source>Medium</source>
      <translation>Keskikoko</translation>
    </message>
    <message>
      <source>Document</source>
      <translation>Asiakirja</translation>
    </message>
    <message>
      <source>Typography</source>
      <translation>Typografia</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>Pystysuora</translation>
    </message>
    <message>
      <source>Margin Guides</source>
      <translation>Sivun reunukset</translation>
    </message>
    <message>
      <source>Preview</source>
      <translation>Esikatselu</translation>
    </message>
    <message>
      <source>Scrapbook</source>
      <translation>Leikekirja</translation>
    </message>
    <message>
      <source>Display</source>
      <translation>Näyttö</translation>
    </message>
    <message>
      <source>Choose a Directory</source>
      <translation>Valitse hakemisto</translation>
    </message>
    <message>
      <source>Autosave</source>
      <translation>Automaattinen tallennus</translation>
    </message>
    <message>
      <source>Preferences</source>
      <translation>Asetukset</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>Sivun koko</translation>
    </message>
    <message>
      <source>General</source>
      <translation>Yleiset</translation>
    </message>
    <message>
      <source>External Tools</source>
      <translation>Ulkoiset työkalut</translation>
    </message>
    <message>
      <source>Image Processing Tool</source>
      <translation>Kuvanmuokkaustyökalu</translation>
    </message>
    <message>
      <source>Printing</source>
      <translation>Tulostus</translation>
    </message>
    <message>
      <source>Default font size for the menus and windows</source>
      <translation>Valikoissa ja ikkunoissa käytettävän kirjaintyypin koko</translation>
    </message>
    <message>
      <source>Default unit of measurement for document editing</source>
      <translation>Asiakirjan muokkauksessa käytettävä mittayksikkö</translation>
    </message>
    <message>
      <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
      <translation>Askellettavien rivien lukumäärä hiiren rullaa käytettäessä</translation>
    </message>
    <message>
      <source>Number of recently edited documents to show in the File menu</source>
      <translation>Viimeisten asiakirjojen lukumäärä Tiedosto-valikossa</translation>
    </message>
    <message>
      <source>Default documents directory</source>
      <translation>Oletushakemisto asiakirjoille</translation>
    </message>
    <message>
      <source>Default Scripter scripts directory</source>
      <translation>Oletushakemisto skripteille</translation>
    </message>
    <message>
      <source>Default page size, either a standard size or a custom size</source>
      <translation>Oletuskoko sivulle, joko standardi tai itse määritelty koko</translation>
    </message>
    <message>
      <source>Default orientation of document pages</source>
      <translation>Asiakirjan sivujen suunta</translation>
    </message>
    <message>
      <source>Width of document pages, editable if you have chosen a custom page size</source>
      <translation>Asiakirjan sivun leveys, muokattavissa jos valitset sivun kooksi &quot;Oma&quot;</translation>
    </message>
    <message>
      <source>Height of document pages, editable if you have chosen a custom page size</source>
      <translation>Asiakirjan sivun korkeus, muokattavissa jos valitset sivun kooksi &quot;Oma&quot;</translation>
    </message>
    <message>
      <source>Time period between saving automatically</source>
      <translation>Automaattisen tallennuksen aikaväli</translation>
    </message>
    <message>
      <source>Choose the size of the preview in the scrapbook palette</source>
      <translation>Valitse esikatselukuvan koko leikekirjassa</translation>
    </message>
    <message>
      <source>Color for paper</source>
      <translation>Paperin väri</translation>
    </message>
    <message>
      <source>Mask the area outside the margins in the margin color</source>
      <translation>Merkkaa marginaalien ulkopuolinen alue marginaalin värillä</translation>
    </message>
    <message>
      <source>Set the default zoom level</source>
      <translation>Aseta näytettävä sivun koko</translation>
    </message>
    <message>
      <source>Antialias text for EPS and PDF onscreen rendering</source>
      <translation>Käytä reunanpehmennystä EPS- ja PDF-tekstiin näytöllä</translation>
    </message>
    <message>
      <source>Antialias graphics for EPS and PDF onscreen rendering</source>
      <translation>Käytä reunanpehmennystä EPS- ja PDF-grafiikkaan näytöllä</translation>
    </message>
    <message>
      <source>Do not show objects outside the margins on the printed page or exported file</source>
      <translation>Älä vie tai tulosta kohteita, jotka ovat asiakirjan marginaalin ulkopuolella</translation>
    </message>
    <message>
      <source>Save the scrapbook contents everytime after a change</source>
      <translation>Tallenna leikekirjan sisältö aina sen muuttuessa</translation>
    </message>
    <message>
      <source>&amp;Theme:</source>
      <translation>&amp;Teema:</translation>
    </message>
    <message>
      <source>&amp;Wheel Jump:</source>
      <translation>&amp;Rullan askellus:</translation>
    </message>
    <message>
      <source>&amp;Recent Documents:</source>
      <translation>&amp;Viimeaikaiset asiakirjat:</translation>
    </message>
    <message>
      <source>&amp;Documents:</source>
      <translation>&amp;Asiakirjat:</translation>
    </message>
    <message>
      <source>&amp;Change...</source>
      <translation>&amp;Muuta...</translation>
    </message>
    <message>
      <source>&amp;ICC Profiles:</source>
      <translation>&amp;ICC-profiilit:</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation>&amp;Muuta...</translation>
    </message>
    <message>
      <source>&amp;Scripts:</source>
      <translation>&amp;Skriptit:</translation>
    </message>
    <message>
      <source>Ch&amp;ange...</source>
      <translation>&amp;Muuta...</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Koko:</translation>
    </message>
    <message>
      <source>Orie&amp;ntation:</source>
      <translation>&amp;Suunta:</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>&amp;Leveys:</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>&amp;Korkeus:</translation>
    </message>
    <message>
      <source>&amp;Bottom:</source>
      <translation>&amp;Alhaalla:</translation>
    </message>
    <message>
      <source>&amp;Top:</source>
      <translation>&amp;Ylhäällä:</translation>
    </message>
    <message>
      <source>&amp;Right:</source>
      <translation>&amp;Oikea:</translation>
    </message>
    <message>
      <source>&amp;Left:</source>
      <translation>&amp;Vasen:</translation>
    </message>
    <message>
      <source>&amp;Interval:</source>
      <translation>&amp;Väli:</translation>
    </message>
    <message>
      <source>Sa&amp;ve Contents on Changes</source>
      <translation>&amp;Tallenna muutosten jälkeen</translation>
    </message>
    <message>
      <source>Large</source>
      <translation>Iso</translation>
    </message>
    <message>
      <source>Display &amp;Unprintable Area in Margin Color</source>
      <translation>N&amp;äytä tulostumaton alue reunuksen värillä</translation>
    </message>
    <message>
      <source>&amp;Adjust Display Size</source>
      <translation>&amp;Säädä näytön koko</translation>
    </message>
    <message>
      <source>&amp;Name of Executable:</source>
      <translation>&amp;Ajettavan tiedoston nimi:</translation>
    </message>
    <message>
      <source>Antialias &amp;Text</source>
      <translation>Reunanpehmennys &amp;tekstiin</translation>
    </message>
    <message>
      <source>Antialias &amp;Graphics</source>
      <translation>Reunanpehmennys &amp;grafiikkaan</translation>
    </message>
    <message>
      <source>Name of &amp;Executable:</source>
      <translation>&amp;Ajettavan tiedoston nimi:</translation>
    </message>
    <message>
      <source>Clip to Page &amp;Margins</source>
      <translation>&amp;Leikkaa sivu reunuksiin</translation>
    </message>
    <message>
      <source>Apply &amp;Under Color Removal</source>
      <translation>&amp;Käytä alivärinpoistoa (UCR)</translation>
    </message>
    <message>
      <source>Cha&amp;nge...</source>
      <translation>&amp;Muuta...</translation>
    </message>
    <message>
      <source>&amp;Language:</source>
      <translation>&amp;Kieli:</translation>
    </message>
    <message>
      <source>Document T&amp;emplates:</source>
      <translation>&amp;Malliasiakirjat:</translation>
    </message>
    <message>
      <source>Units:</source>
      <translation>Yksiköt:</translation>
    </message>
    <message>
      <source>Undo/Redo</source>
      <translation>Kumoa/Tee uudelleen</translation>
    </message>
    <message>
      <source>Action history length</source>
      <translation>Toimintohistorian pituus</translation>
    </message>
    <message>
      <source>Hyphenator</source>
      <translation>Tavutus</translation>
    </message>
    <message>
      <source>Fonts</source>
      <translation>Kirjaintyypit</translation>
    </message>
    <message>
      <source>Color Management</source>
      <translation>Värien hallinta</translation>
    </message>
    <message>
      <source>PDF Export</source>
      <translation>PDF-vienti</translation>
    </message>
    <message>
      <source>Document Item Attributes</source>
      <translation>Kohteen muuttujat</translation>
    </message>
    <message>
      <source>Table of Contents and Indexes</source>
      <translation>Sisällysluettelo ja hakemistot</translation>
    </message>
    <message>
      <source>Keyboard Shortcuts</source>
      <translation>Näppäinoikopolut</translation>
    </message>
    <message>
      <source>Page Display</source>
      <translation>Sivun näyttö</translation>
    </message>
    <message>
      <source>Color:</source>
      <translation>Väri:</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation>Alt+U</translation>
    </message>
    <message>
      <source>Show Pictures</source>
      <translation>Näytä kuvat</translation>
    </message>
    <message>
      <source>Show Text Chains</source>
      <translation>Näytä tekstiketjut</translation>
    </message>
    <message>
      <source>Show Text Control Characters</source>
      <translation>Näytä tulostumattomat merkit</translation>
    </message>
    <message>
      <source>Show Frames</source>
      <translation>Näytä kehykset</translation>
    </message>
    <message>
      <source>Scratch Space</source>
      <translation>Työalue</translation>
    </message>
    <message>
      <source>To adjust the display drag the ruler below with the slider.</source>
      <translation>Säädä näyttösi viivaimella.</translation>
    </message>
    <message>
      <source>Always ask before fonts are replaced when loading a document</source>
      <translation>Kysy aina ennen kirjaintyyppien korvaamista</translation>
    </message>
    <message>
      <source>Preview of current Paragraph Style visible when editing Styles</source>
      <translation>Kappaletyylin esikatselu näkyvissä muokattaessa kappaletyyliä</translation>
    </message>
    <message>
      <source>Miscellaneous</source>
      <translation>Muut</translation>
    </message>
    <message>
      <source>Plugins</source>
      <translation>Laajennokset</translation>
    </message>
    <message>
      <source>Display non-printing characters such as paragraph markers in text frames</source>
      <translation>Näytä tulostumattomat merkit kuten kappalemerkki tekstikehyksissä</translation>
    </message>
    <message>
      <source>Turns the display of frames on or off</source>
      <translation>Näytä tai piilota kehykset</translation>
    </message>
    <message>
      <source>Turns the display of pictures on or off</source>
      <translation>Näytä tai piilota kuvat</translation>
    </message>
    <message>
      <source>Additional directory for document templates</source>
      <translation>Hakemisto asiakirjamalleille</translation>
    </message>
    <message>
      <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
      <translation>Aseta viivain näytölle ja säädä pituus oikeaksi. Tämä mahdollistaa sivujen ja objektien näytön oikean kokoisina.</translation>
    </message>
    <message>
      <source>Filesystem location for the Ghostscript interpreter</source>
      <translation>Ghostscript-tulkin sijainti tiedostojärjestelmässä</translation>
    </message>
    <message>
      <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
      <translation>Määrittelee tyhjän tilan määrän asiakirjan vasemmalla puolella, jota voi käyttää objektien säilytykseen ja työskentelyalueena sivua tehtäessä.</translation>
    </message>
    <message>
      <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
      <translation>Määrittelee tyhjän tilan määrän asiakirjan oikealla puolella, jota voi käyttää objektien säilytykseen ja työskentelyalueena sivua tehtäessä.</translation>
    </message>
    <message>
      <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
      <translation>Määrittelee tyhjän tilan määrän asiakirjan yläpuolella, jota voi käyttää objektien säilytykseen ja työskentelyalueena sivua tehtäessä.</translation>
    </message>
    <message>
      <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
      <translation>Määrittelee tyhjän tilan määrän asiakirjan alapuolella, jota voi käyttää objektien säilytykseen ja työskentelyalueena sivua tehtäessä.</translation>
    </message>
    <message>
      <source>Preflight Verifier</source>
      <translation>Tarkastustyökalu</translation>
    </message>
    <message>
      <source>dpi</source>
      <translation>dpi</translation>
    </message>
    <message>
      <source>Resolution:</source>
      <translation>Tarkkuus:</translation>
    </message>
    <message>
      <source>Show Startup Dialog</source>
      <translation>Näytä latausikkuna</translation>
    </message>
    <message>
      <source>Lorem Ipsum</source>
      <translation>Lorem Ipsum</translation>
    </message>
    <message>
      <source>Always use standard Lorem Ipsum</source>
      <translation>Käytä aina perus Lorem ipsumia</translation>
    </message>
    <message>
      <source>Count of the Paragraphs:</source>
      <translation>Kappaleiden lukumäärä:</translation>
    </message>
    <message>
      <source>Rulers relative to Page</source>
      <translation>Viivaimet suhteessa sivuun</translation>
    </message>
    <message>
      <source>Gaps between Pages</source>
      <translation>Sivujen välin pituus</translation>
    </message>
    <message>
      <source>Horizontal:</source>
      <translation>Vaakasuora:</translation>
    </message>
    <message>
      <source>Vertical:</source>
      <translation>Pystysuora:</translation>
    </message>
    <message>
      <source>PostScript Interpreter</source>
      <translation>PostScript-tulkki</translation>
    </message>
    <message>
      <source>Enable or disable  the display of linked frames.</source>
      <translation>Näytä tai piilota linkitetyt kehykset.</translation>
    </message>
    <message>
      <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
      <translation>Valitse Scribuksen käyttöliittymän kieli. Jos jätetään tyhjäksi, kieli valitaan ympäristömuuttujien perusteella. Valinta on mahdollista ohittaa käyttämällä komentoriviparametriä Scribusta käynnistettäessä.</translation>
    </message>
    <message>
      <source>Locate Ghostscript</source>
      <translation>Ghostscriptin sijainti</translation>
    </message>
    <message>
      <source>Locate your image editor</source>
      <translation>Kuvanmuokkausohjelman sijainti</translation>
    </message>
    <message>
      <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
      <translation>Scribus tallentaa kopion asiakirjastasi bak-päätteellä aina kuna aika edellisestä tallennuksesta täyttyy.</translation>
    </message>
    <message>
      <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
      <translation>Toimintohistorian pituus. 0 ei rajoita pituutta.</translation>
    </message>
    <message>
      <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend 'gimp-remote', as it allows you to edit the image in an already running instance of gimp.</source>
      <translation>Kuvan muokkauksessa käyttämäsi ohjelman polku. Jos käytät GIMPiä, suosittelemme tähän gimp-remote -komentoa, jolla kuva avataan jo käynnissä olevaan GIMPiin.</translation>
    </message>
    <message>
      <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
      <translation>Tapa vaihtaa C, Y ja M värien tekemä harmaa mustalla tehdyksi. 
UCR vaikuttaa kuvien kohdissa, jotka ovat neutraaleja tai lähellä harmaata 
olevia tummia värejä. UCR:n käyttäminen saattaa parantaa joidenkin kuvien
tulostamista. UCR parantaa kykyä välttää värien liikakylläisyyttä CMY-musteilla. 
Parhaan tuloksen saavuttamiseksi tarvitaan tapauskohtaista testausta. UCR 
vähentää ylikylläisyyden vaaraa CMY-musteilla.</translation>
    </message>
    <message>
      <source>&amp;Font Size (Menus):</source>
      <translation>Fonttikoko (valikot):</translation>
    </message>
    <message>
      <source>Font Size (&amp;Palettes):</source>
      <translation>Fonttikoko (työkaluikkunat):</translation>
    </message>
    <message>
      <source>Default font size for the tool windows</source>
      <translation>Oletuskoko työkaluikkunoissa käytettäville fonteille</translation>
    </message>
    <message>
      <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
      <translation>Valitse ulkonäkö. Scribus mahdollistaa KDE:n ja Qt:n teemojen käytön, jos Qt on asetettu etsimään KDE-liitännäisiä.</translation>
    </message>
    <message>
      <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
      <translation>Oletushakemisto ICC-profiileille. Tätä ei voi muuttaa asiakirjan ollessa avoinna. Oletuksena Scribus hakee profiileja järjestelmähakemistoista Windowsissa ja Mac OSX:ssä. Linuxissa ja Unixeissa profiileja etsitään hakemistoista $home/.color/icc,/usr/share/color/icc ja /usr/local/share/color/icc </translation>
    </message>
  </context>
  <context>
    <name>PrefsDialogBase</name>
    <message>
      <source>&amp;Defaults</source>
      <translation>&amp;Oletusarvot</translation>
    </message>
    <message>
      <source>Save...</source>
      <translation>Tallenna...</translation>
    </message>
    <message>
      <source>Save Preferences</source>
      <translation>Tallenna asetukset</translation>
    </message>
  </context>
  <context>
    <name>PrefsManager</name>
    <message>
      <source>Postscript</source>
      <translation>Postscript</translation>
    </message>
    <message>
      <source>PDF 1.3</source>
      <translation>PDF 1.3</translation>
    </message>
    <message>
      <source>PDF 1.4</source>
      <translation>PDF 1.4</translation>
    </message>
    <message>
      <source>PDF/X-3</source>
      <translation>PDF/X-3</translation>
    </message>
    <message>
      <source>Migrate Old Scribus Settings?</source>
      <translation>Tuo vanhat Scribuksen asetukset?</translation>
    </message>
    <message>
      <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
      <translation>Scribus on löytänyt asetukset versiolle 1.2. 
Haluatko tuoda ne uuteen Scribuksen versioon?</translation>
    </message>
    <message>
      <source>Single Page</source>
      <translation>Yksittäinen sivu</translation>
    </message>
    <message>
      <source>Double sided</source>
      <translation>Kaksi sivua</translation>
    </message>
    <message>
      <source>Left Page</source>
      <translation>Vasen sivu</translation>
    </message>
    <message>
      <source>Right Page</source>
      <translation>Oikea sivu</translation>
    </message>
    <message>
      <source>3-Fold</source>
      <translation>Kolme sivua</translation>
    </message>
    <message>
      <source>Middle</source>
      <translation>Keskimmäinen</translation>
    </message>
    <message>
      <source>4-Fold</source>
      <translation>Neljä sivua</translation>
    </message>
    <message>
      <source>Middle Left</source>
      <translation>Keskimmäinen vasen</translation>
    </message>
    <message>
      <source>Middle Right</source>
      <translation>Keskimmäinen oikea</translation>
    </message>
    <message>
      <source>PostScript</source>
      <translation>PostScript</translation>
    </message>
    <message>
      <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
      <translation>Asetustiedostoa &quot;%1&quot; ei voitu avata kirjoittamista varten: %2</translation>
    </message>
    <message>
      <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
      <translation>Asetustiedostoon &quot;%1&quot; kirjoittaminen epäonnistui. QIODevice statuskoodi %2</translation>
    </message>
    <message>
      <source>Failed to open prefs file &quot;%1&quot;: %2</source>
      <translation>Asetustiedoston &quot;%1&quot; avaaminen epäonnistui: %2</translation>
    </message>
    <message>
      <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
      <translation>Asetusten luku tiedostosta %1 epäonnistui: %2 rivi %3, sarake %4</translation>
    </message>
    <message>
      <source>Error Writing Preferences</source>
      <translation>Virhe kirjoitettaessa asetuksia</translation>
    </message>
    <message>
      <source>Scribus was not able to save its preferences:&lt;br>%1&lt;br>Please check file and directory permissions and available disk space.</source>
      <comment>scribus app error</comment>
      <translation>Scribus ei pystynyt tallentamaan asetuksia:&lt;br>%1&lt;br>Tarkasta tiedosto- ja hakemisto-oikeudet ja käytettävissä oleva levytila.</translation>
    </message>
    <message>
      <source>Error Loading Preferences</source>
      <translation>Virhe luettaessa asetuksia</translation>
    </message>
    <message>
      <source>Scribus was not able to load its preferences:&lt;br>%1&lt;br>Default settings will be loaded.</source>
      <translation>Scribus ei pystynyt lataamaan asetuksia:&lt;br>%1&lt;br>Oletusasetukset ladataan.</translation>
    </message>
  </context>
  <context>
    <name>PresetLayout</name>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Magazine</source>
      <translation>Magazine</translation>
    </message>
    <message>
      <source>Fibonacci</source>
      <translation>Fibonacci</translation>
    </message>
    <message>
      <source>Golden Mean</source>
      <translation>Kultainen leikkaus</translation>
    </message>
    <message>
      <source>Nine Parts</source>
      <translation>Nine Parts</translation>
    </message>
    <message>
      <source>Gutenberg</source>
      <translation>Gutenberg</translation>
    </message>
    <message>
      <source>You can select predefined page layout here. 'None' leave margins as is, Gutenberg sets margins classically. 'Magazine' sets all margins for same value. Leading is Left/Inside value.</source>
      <translation>Voit valita valmiin asettelun sivuillesi. &quot;Ei mitään&quot; jättää marginaalit asetettuihin arvoihin. Gutenberg asettaa ne klassiseen tyyliin. Magazine antaa kaikilla reunuksille saman arvon. Ensimmäinen kohta on vasemman tai sisäpuolen arvo.</translation>
    </message>
  </context>
  <context>
    <name>PythonConsole</name>
    <message>
      <source>&amp;Open...</source>
      <translation>&amp;Avaa...</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Tallenna</translation>
    </message>
    <message>
      <source>&amp;Exit</source>
      <translation>&amp;Poistu</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>&amp;Tiedosto</translation>
    </message>
    <message>
      <source>&amp;Run</source>
      <translation>&amp;Aja</translation>
    </message>
    <message>
      <source>&amp;Save Output...</source>
      <translation>&amp;Tallenna tuloste...</translation>
    </message>
    <message>
      <source>&amp;Script</source>
      <translation>&amp;Skripti</translation>
    </message>
    <message>
      <source>Script Console</source>
      <translation>Skriptin konsoli</translation>
    </message>
    <message>
      <source>Write your commands here. A selection is processed as script</source>
      <translation>Kirjoita komennot tässä. Valinta suoritetaan skriptinä</translation>
    </message>
    <message>
      <source>Output of your script</source>
      <translation>Skriptin tulostus</translation>
    </message>
    <message>
      <source>Python Scripts (*.py)</source>
      <translation>Python-skriptit (*.py)</translation>
    </message>
    <message>
      <source>Save the Python Commands in File</source>
      <translation>Tallenna Python-komennot tiedostoon</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Varoitus</translation>
    </message>
    <message>
      <source>Text Files (*.txt)</source>
      <translation>Tekstitiedostot (*.txt)</translation>
    </message>
    <message>
      <source>Save Current Output</source>
      <translation>Tallenna nykyinen tuloste</translation>
    </message>
    <message>
      <source>Save &amp;As...</source>
      <translation>Tallenna &amp;nimellä...</translation>
    </message>
    <message>
      <source>Run As &amp;Console</source>
      <translation>Aja &amp;konsolissa</translation>
    </message>
    <message>
      <source>Scribus Python Console</source>
      <translation>Scribus Python-konsoli</translation>
    </message>
    <message>
      <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
      <translation>Tämä on periytetty standardista Python-konsolista joten siinä on muutamia rajoituksia, erityisesti whitespace-merkkien käsitelyssä. Lisää tietoa löytyy dokumentaatiosta.</translation>
    </message>
    <message>
      <source>Open Python Script File</source>
      <translation>Avaa Python-skriptitiedosto</translation>
    </message>
  </context>
  <context>
    <name>QColorDialog</name>
    <message>
      <source>Hu&amp;e:</source>
      <translation>&amp;H:</translation>
    </message>
    <message>
      <source>&amp;Sat:</source>
      <translation>&amp;S:</translation>
    </message>
    <message>
      <source>&amp;Val:</source>
      <translation>&amp;V:</translation>
    </message>
    <message>
      <source>&amp;Red:</source>
      <translation>&amp;Punainen:</translation>
    </message>
    <message>
      <source>&amp;Green:</source>
      <translation>V&amp;ihreä:</translation>
    </message>
    <message>
      <source>Bl&amp;ue:</source>
      <translation>Si&amp;ninen:</translation>
    </message>
    <message>
      <source>A&amp;lpha channel:</source>
      <translation>&amp;Alpha kanava:</translation>
    </message>
    <message>
      <source>&amp;Basic colors</source>
      <translation>Pe&amp;rusvärit</translation>
    </message>
    <message>
      <source>&amp;Custom colors</source>
      <translation>&amp;Omat värit</translation>
    </message>
    <message>
      <source>&amp;Define Custom Colors >></source>
      <translation>&amp;Määritä omat värit >></translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Peruuta</translation>
    </message>
    <message>
      <source>&amp;Add to Custom Colors</source>
      <translation>&amp;Lisää omiin väreihin</translation>
    </message>
    <message>
      <source>Select color</source>
      <translation>Valitse väri</translation>
    </message>
  </context>
  <context>
    <name>QFileDialog</name>
    <message>
      <source>Copy or Move a File</source>
      <translation>Kopioi tai siirrä tiedosto</translation>
    </message>
    <message>
      <source>Read: %1</source>
      <translation>Lue: %1</translation>
    </message>
    <message>
      <source>Write: %1</source>
      <translation>Kirjoita: %1</translation>
    </message>
    <message>
      <source>File &amp;name:</source>
      <translation>Tiedoston &amp;nimi:</translation>
    </message>
    <message>
      <source>File &amp;type:</source>
      <translation>Tiedoston &amp;tyyppi:</translation>
    </message>
    <message>
      <source>One directory up</source>
      <translation>Yksi hakemisto ylöspäin</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Peruuta</translation>
    </message>
    <message>
      <source>All Files (*)</source>
      <translation>Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Nimi</translation>
    </message>
    <message>
      <source>Size</source>
      <translation>Koko</translation>
    </message>
    <message>
      <source>Type</source>
      <translation>Tyyppi</translation>
    </message>
    <message>
      <source>Date</source>
      <translation>Päivämäärä</translation>
    </message>
    <message>
      <source>Attributes</source>
      <translation>Muuttujat</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Look &amp;in:</source>
      <translation>Nykyinen &amp;hakemisto:</translation>
    </message>
    <message>
      <source>Back</source>
      <translation>Takaisin</translation>
    </message>
    <message>
      <source>Create New Folder</source>
      <translation>Luo uusi hakemisto</translation>
    </message>
    <message>
      <source>List View</source>
      <translation>Listanäkymä</translation>
    </message>
    <message>
      <source>Detail View</source>
      <translation>Yksityiskohtainen näkymä</translation>
    </message>
    <message>
      <source>Preview File Info</source>
      <translation>Katso tiedoston tiedot</translation>
    </message>
    <message>
      <source>Preview File Contents</source>
      <translation>Katso tiedoston sisältö</translation>
    </message>
    <message>
      <source>Read-write</source>
      <translation>Luku-kirjoitus</translation>
    </message>
    <message>
      <source>Read-only</source>
      <translation>Ainoastaan luku</translation>
    </message>
    <message>
      <source>Write-only</source>
      <translation>Ainoastaan kirjoitus</translation>
    </message>
    <message>
      <source>Inaccessible</source>
      <translation>Tavoittamaton</translation>
    </message>
    <message>
      <source>Symlink to File</source>
      <translation>Symbolinen linkki tiedostoon</translation>
    </message>
    <message>
      <source>Symlink to Directory</source>
      <translation>Symbolinen linkki hakemistoon</translation>
    </message>
    <message>
      <source>Symlink to Special</source>
      <translation>Symbolinen linkki to Special</translation>
    </message>
    <message>
      <source>File</source>
      <translation>Tiedosto</translation>
    </message>
    <message>
      <source>Dir</source>
      <translation>Hakemisto</translation>
    </message>
    <message>
      <source>Special</source>
      <translation>Special</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Avaa</translation>
    </message>
    <message>
      <source>Save As</source>
      <translation>Tallenna nimellä</translation>
    </message>
    <message>
      <source>&amp;Open</source>
      <translation>&amp;Avaa</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Tallenna</translation>
    </message>
    <message>
      <source>&amp;Rename</source>
      <translation>Nimeä &amp;uudelleen</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>P&amp;oista</translation>
    </message>
    <message>
      <source>R&amp;eload</source>
      <translation>P&amp;äivitä</translation>
    </message>
    <message>
      <source>Sort by &amp;Name</source>
      <translation>Järjestä &amp;nimen mukaan</translation>
    </message>
    <message>
      <source>Sort by &amp;Size</source>
      <translation>Järjestä &amp;koon mukaan</translation>
    </message>
    <message>
      <source>Sort by &amp;Date</source>
      <translation>Järjestä &amp;päivämäärän mukaan</translation>
    </message>
    <message>
      <source>&amp;Unsorted</source>
      <translation>&amp;Järjestämätön</translation>
    </message>
    <message>
      <source>Sort</source>
      <translation>Järjestä</translation>
    </message>
    <message>
      <source>Show &amp;hidden files</source>
      <translation>Nä&amp;ytä piilotetut tiedostot</translation>
    </message>
    <message>
      <source>the file</source>
      <translation>tiedoston</translation>
    </message>
    <message>
      <source>the directory</source>
      <translation>hakemiston</translation>
    </message>
    <message>
      <source>the symlink</source>
      <translation>symbolisen linkin</translation>
    </message>
    <message>
      <source>Delete %1</source>
      <translation>Poista %1</translation>
    </message>
    <message>
      <source>&lt;qt>Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt></source>
      <translation>&lt;qt>Haluatko varmasti poistaa %1 &quot;%2&quot;?&lt;/qt></translation>
    </message>
    <message>
      <source>&amp;Yes</source>
      <translation>Ky&amp;llä</translation>
    </message>
    <message>
      <source>&amp;No</source>
      <translation>&amp;Ei</translation>
    </message>
    <message>
      <source>New Folder 1</source>
      <translation>Uusi hakemisto 1</translation>
    </message>
    <message>
      <source>New Folder</source>
      <translation>Uusi hakemisto</translation>
    </message>
    <message>
      <source>New Folder %1</source>
      <translation>Uusi hakemisto %1</translation>
    </message>
    <message>
      <source>Find Directory</source>
      <translation>Etsi hakemisto</translation>
    </message>
    <message>
      <source>Directories</source>
      <translation>Hakemistot</translation>
    </message>
    <message>
      <source>Save</source>
      <translation>Tallenna</translation>
    </message>
    <message>
      <source>Error</source>
      <translation>Virhe</translation>
    </message>
    <message>
      <source>%1
File not found.
Check path and filename.</source>
      <translation>%1
Tiedostoa ei löydy.
Tarkasta polku ja tiedostonimi.</translation>
    </message>
    <message>
      <source>All Files (*.*)</source>
      <translation>Kaikki tiedostot (*.*)</translation>
    </message>
    <message>
      <source>Select a Directory</source>
      <translation>Valitse hakemisto</translation>
    </message>
    <message>
      <source>Directory:</source>
      <translation>Hakemisto:</translation>
    </message>
  </context>
  <context>
    <name>QFontDialog</name>
    <message>
      <source>&amp;Font</source>
      <translation>&amp;Kirjaintyyppi</translation>
    </message>
    <message>
      <source>Font st&amp;yle</source>
      <translation>Kirjaintyypin &amp;tyyli</translation>
    </message>
    <message>
      <source>&amp;Size</source>
      <translation>K&amp;oko</translation>
    </message>
    <message>
      <source>Effects</source>
      <translation>Efektit</translation>
    </message>
    <message>
      <source>Stri&amp;keout</source>
      <translation>&amp;Yliviivattu</translation>
    </message>
    <message>
      <source>&amp;Underline</source>
      <translation>&amp;Alleviivattu</translation>
    </message>
    <message>
      <source>&amp;Color</source>
      <translation>&amp;Väri</translation>
    </message>
    <message>
      <source>Sample</source>
      <translation>Esimerkki</translation>
    </message>
    <message>
      <source>Scr&amp;ipt</source>
      <translation>Sk&amp;ripti</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Apply</source>
      <translation>Käytä</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Peruuta</translation>
    </message>
    <message>
      <source>Close</source>
      <translation>Sulje</translation>
    </message>
    <message>
      <source>Select Font</source>
      <translation>Valitse kirjaintyyppi</translation>
    </message>
  </context>
  <context>
    <name>QLineEdit</name>
    <message>
      <source>Clear</source>
      <translation>Tyhjennä</translation>
    </message>
    <message>
      <source>Select All</source>
      <translation>Valitse kaikki</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>K&amp;umoa</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>&amp;Tee uudelleen</translation>
    </message>
    <message>
      <source>Cu&amp;t</source>
      <translation>&amp;Leikkaa</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>&amp;Kopioi</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>L&amp;iitä</translation>
    </message>
  </context>
  <context>
    <name>QMainWindow</name>
    <message>
      <source>Line up</source>
      <translation>Järjestä</translation>
    </message>
    <message>
      <source>Customize...</source>
      <translation>Muokkaa...</translation>
    </message>
  </context>
  <context>
    <name>QMessageBox</name>
    <message>
      <source>&lt;h3>About Qt&lt;/h3>&lt;p>This program uses Qt version %1.&lt;/p>&lt;p>Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p>&lt;p>Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br>Qt is also available for embedded devices.&lt;/p>&lt;p>Qt is a Trolltech product. See &lt;tt>http://www.trolltech.com/qt/&lt;/tt> for more information.&lt;/p></source>
      <translation>&lt;h3>About Qt&lt;/h3>&lt;p>This program uses Qt version %1.&lt;/p>&lt;p>Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p>&lt;p>Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br>Qt is also available for embedded devices.&lt;/p>&lt;p>Qt is a Trolltech product. See &lt;tt>http://www.trolltech.com/qt/&lt;/tt> for more information.&lt;/p></translation>
    </message>
  </context>
  <context>
    <name>QObject</name>
    <message>
      <source>No</source>
      <translation>Ei</translation>
    </message>
    <message>
      <source>Yes</source>
      <translation>Kyllä</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Avaa</translation>
    </message>
    <message>
      <source>Do you really want to overwrite the File:
%1 ?</source>
      <translation>Haluatko korvata tiedoston %1?</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>Tallenna nimellä</translation>
    </message>
    <message>
      <source>Warning</source>
      <translation>Varoitus</translation>
    </message>
    <message>
      <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
      <translation>SVG-kuvat (*.svg *.svgz);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>SVG-Images (*.svg);;All Files (*)</source>
      <translation>SVG-kuvat (*.svg);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Initializing...</source>
      <translation>Alustetaan...</translation>
    </message>
    <message>
      <source>Background</source>
      <translation>Tausta</translation>
    </message>
    <message>
      <source>File exists. Overwrite?</source>
      <translation>Tiedosto on jo olemassa. Ylikirjoitetaanko?</translation>
    </message>
    <message>
      <source>exists already. Overwrite?</source>
      <translation>on jo olemassa. Ylikirjoitetaanko?</translation>
    </message>
    <message>
      <source>Yes all</source>
      <translation>Kyllä kaikkiin</translation>
    </message>
    <message>
      <source>Error writing the output file(s).</source>
      <translation>Virhe kirjoitettaessa kohdetiedostoa.</translation>
    </message>
    <message>
      <source>Save as Image</source>
      <translation>Tallenna kuvana</translation>
    </message>
    <message>
      <source>Export successful.</source>
      <translation>Vienti onnistui.</translation>
    </message>
    <message>
      <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
      <translation>Kaikki tuetut tiedostotyypit (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
      <source>All Files (*)</source>
      <translation>Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Newsletters</source>
      <translation>Uutislehdet</translation>
    </message>
    <message>
      <source>Brochures</source>
      <translation>Esitteet</translation>
    </message>
    <message>
      <source>Catalogs</source>
      <translation>Tuoteluettelot</translation>
    </message>
    <message>
      <source>Flyers</source>
      <translation>Lehtiset</translation>
    </message>
    <message>
      <source>Signs</source>
      <translation>Merkit</translation>
    </message>
    <message>
      <source>Cards</source>
      <translation>Kortit</translation>
    </message>
    <message>
      <source>Letterheads</source>
      <translation>Kirjeen reunukset</translation>
    </message>
    <message>
      <source>Envelopes</source>
      <translation>Kirjekuoret</translation>
    </message>
    <message>
      <source>Business Cards</source>
      <translation>Käyntikortit</translation>
    </message>
    <message>
      <source>Calendars</source>
      <translation>Kalenterit</translation>
    </message>
    <message>
      <source>Advertisements</source>
      <translation>Mainokset</translation>
    </message>
    <message>
      <source>Labels</source>
      <translation>Tarrat</translation>
    </message>
    <message>
      <source>Menus</source>
      <translation>Ruokalistat</translation>
    </message>
    <message>
      <source>Programs</source>
      <translation>Ohjelmat</translation>
    </message>
    <message>
      <source>PDF Forms</source>
      <translation>PDF-lomakkeet</translation>
    </message>
    <message>
      <source>Magazines</source>
      <translation>Lehdet</translation>
    </message>
    <message>
      <source>Posters</source>
      <translation>Julisteet</translation>
    </message>
    <message>
      <source>Announcements</source>
      <translation>Ilmoitukset</translation>
    </message>
    <message>
      <source>Text Documents</source>
      <translation>Tekstiasiakirjat</translation>
    </message>
    <message>
      <source>Folds</source>
      <translation>Taitokset</translation>
    </message>
    <message>
      <source>Own Templates</source>
      <translation>Omat mallit</translation>
    </message>
    <message>
      <source>PDF Presentations</source>
      <translation>PDF-esitykset</translation>
    </message>
    <message>
      <source>Importing text</source>
      <translation>Tuodaan tekstiä</translation>
    </message>
    <message>
      <source>All Supported Formats</source>
      <translation>Kaikki tuetut tiedostotyypit</translation>
    </message>
    <message>
      <source>HTML Files</source>
      <translation>HTML-tiedostot</translation>
    </message>
    <message>
      <source>html</source>
      <translation>html</translation>
    </message>
    <message>
      <source>Text Files</source>
      <translation>Tekstitiedostot</translation>
    </message>
    <message>
      <source>Comma Separated Value Files</source>
      <translation>Pilkuin eroteltu tekstitiedosto</translation>
    </message>
    <message>
      <source>CSV_data</source>
      <translation>CSV_sisältö</translation>
    </message>
    <message>
      <source>CSV_header</source>
      <translation>CSV_otsikko</translation>
    </message>
    <message>
      <source>
External Links
</source>
      <translation>Linkit</translation>
    </message>
    <message>
      <source>Font %1 is broken, discarding it</source>
      <translation>Kirjaintyyppi %1 on viallinen ja sitä ei käytetä</translation>
    </message>
    <message>
      <source>Text Filters</source>
      <translation>Tekstisuotimet</translation>
    </message>
    <message>
      <source>Media Cases</source>
      <translation>Kannet</translation>
    </message>
    <message>
      <source>Albanian</source>
      <translation>albania</translation>
    </message>
    <message>
      <source>Basque</source>
      <translation>baski</translation>
    </message>
    <message>
      <source>Bulgarian</source>
      <translation>bulgaria</translation>
    </message>
    <message>
      <source>Brazilian</source>
      <translation>brasilia</translation>
    </message>
    <message>
      <source>Catalan</source>
      <translation>katalonia</translation>
    </message>
    <message>
      <source>Chinese</source>
      <translation>kiina</translation>
    </message>
    <message>
      <source>Czech</source>
      <translation>tsekki</translation>
    </message>
    <message>
      <source>Danish</source>
      <translation>tanska</translation>
    </message>
    <message>
      <source>Dutch</source>
      <translation>hollanti</translation>
    </message>
    <message>
      <source>English</source>
      <translation>englanti</translation>
    </message>
    <message>
      <source>English (British)</source>
      <translation>englanti (Britti)</translation>
    </message>
    <message>
      <source>Esperanto</source>
      <translation>esperanto</translation>
    </message>
    <message>
      <source>German</source>
      <translation>saksa</translation>
    </message>
    <message>
      <source>Finnish</source>
      <translation>suomi</translation>
    </message>
    <message>
      <source>French</source>
      <translation>ranska</translation>
    </message>
    <message>
      <source>Galician</source>
      <translation>galician</translation>
    </message>
    <message>
      <source>Greek</source>
      <translation>kreikka</translation>
    </message>
    <message>
      <source>Hungarian</source>
      <translation>unkari</translation>
    </message>
    <message>
      <source>Indonesian</source>
      <translation>indonesia</translation>
    </message>
    <message>
      <source>Italian</source>
      <translation>italia</translation>
    </message>
    <message>
      <source>Korean</source>
      <translation>korea</translation>
    </message>
    <message>
      <source>Lithuanian</source>
      <translation>liettua</translation>
    </message>
    <message>
      <source>Norwegian (Bokmaal)</source>
      <translation>norja (Bokmaal)</translation>
    </message>
    <message>
      <source>Norwegian (Nnyorsk)</source>
      <translation>norja (nnyorsk)</translation>
    </message>
    <message>
      <source>Norwegian</source>
      <translation>norja</translation>
    </message>
    <message>
      <source>Polish</source>
      <translation>puola</translation>
    </message>
    <message>
      <source>Russian</source>
      <translation>venäjä</translation>
    </message>
    <message>
      <source>Swedish</source>
      <translation>ruotsi</translation>
    </message>
    <message>
      <source>Spanish</source>
      <translation>espanja</translation>
    </message>
    <message>
      <source>Spanish (Latin)</source>
      <translation>espanja (latina)</translation>
    </message>
    <message>
      <source>Slovak</source>
      <translation>slovakki</translation>
    </message>
    <message>
      <source>Slovenian</source>
      <translation>slovenia</translation>
    </message>
    <message>
      <source>Serbian</source>
      <translation>serbia</translation>
    </message>
    <message>
      <source>Cannot get a color with an empty name.</source>
      <comment>python error</comment>
      <translation>Haettavalla värillä täytyy olla nimi.</translation>
    </message>
    <message>
      <source>Cannot change a color with an empty name.</source>
      <comment>python error</comment>
      <translation>Nimetöntä väriä ei voi muuttaa.</translation>
    </message>
    <message>
      <source>Cannot create a color with an empty name.</source>
      <comment>python error</comment>
      <translation>Nimetöntä väriä ei voi luoda.</translation>
    </message>
    <message>
      <source>Cannot delete a color with an empty name.</source>
      <comment>python error</comment>
      <translation>Nimetöntä väriä ei voi poistaa.</translation>
    </message>
    <message>
      <source>Cannot replace a color with an empty name.</source>
      <comment>python error</comment>
      <translation>Nimetöntä väriä ei voi korvata.</translation>
    </message>
    <message>
      <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
      <comment>python error</comment>
      <translation>Yksikkö sallittujen rajojen ulkopuolella. Käytä jotain scribus.UNIT_* vakioista.</translation>
    </message>
    <message>
      <source>Target is not an image frame.</source>
      <comment>python error</comment>
      <translation>Kohde ei ole kuvakehys.</translation>
    </message>
    <message>
      <source>Corner radius must be a positive number.</source>
      <comment>python error</comment>
      <translation>Kulman säde täytyy olla positiivinen numero.</translation>
    </message>
    <message>
      <source>Cannot get font size of non-text frame.</source>
      <comment>python error</comment>
      <translation>Kirjaintyypin kokoa ei voi saada muusta kuin tekstikehyksestä.</translation>
    </message>
    <message>
      <source>Cannot get font of non-text frame.</source>
      <comment>python error</comment>
      <translation>Kirjaintyyppiä ei voi saada muusta kuin tekstikehyksestä.</translation>
    </message>
    <message>
      <source>Cannot get text size of non-text frame.</source>
      <comment>python error</comment>
      <translation>Kirjasimen kokoa ei voi saada muusta kuin tekstikehyksestä.</translation>
    </message>
    <message>
      <source>Cannot get column count of non-text frame.</source>
      <comment>python error</comment>
      <translation>Palstojen lukumäärää ei voi saada muusta kuin tekstikehyksestä.</translation>
    </message>
    <message>
      <source>Cannot get line space of non-text frame.</source>
      <comment>python error</comment>
      <translation>Riviväliä ei voi saada muusta kuin tekstikehyksestä.</translation>
    </message>
    <message>
      <source>Cannot get column gap of non-text frame.</source>
      <comment>python error</comment>
      <translation>Palstojen väliä ei voi saada muusta kuin tekstikehyksestä.</translation>
    </message>
    <message>
      <source>Cannot get text of non-text frame.</source>
      <comment>python error</comment>
      <translation>Tekstiä ei voi saada muusta kuin tekstikehyksestä.</translation>
    </message>
    <message>
      <source>Cannot set text of non-text frame.</source>
      <comment>python error</comment>
      <translation>Tekstiä ei voi tuoda kuin tekstikehykseen.</translation>
    </message>
    <message>
      <source>Cannot insert text into non-text frame.</source>
      <comment>python error</comment>
      <translation>Tekstiä ei voi lisätä muuhun kuin tekstikehykseen.</translation>
    </message>
    <message>
      <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
      <comment>python error</comment>
      <translation>Tasaus rajojen ulkopuolella. Käytä jotain scribus.ALIGN* vakioista.</translation>
    </message>
    <message>
      <source>Selection index out of bounds</source>
      <comment>python error</comment>
      <translation>Valinnan indeksi sallittujen rajojen ulkopuolella</translation>
    </message>
    <message>
      <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
      <translation>OpenOffice.org Draw (*.sxd);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>OpenOffice.org Writer Documents</source>
      <translation>OpenOffice.org Writer asiakirjat</translation>
    </message>
    <message>
      <source>Afrikaans</source>
      <translation>afrikaans</translation>
    </message>
    <message>
      <source>Turkish</source>
      <translation>turkki</translation>
    </message>
    <message>
      <source>Ukranian</source>
      <translation>ukraina</translation>
    </message>
    <message>
      <source>Welsh</source>
      <translation>wales</translation>
    </message>
    <message>
      <source>Color not found - python error</source>
      <comment>python error</comment>
      <translation>Väriä ei löydy - python virhe</translation>
    </message>
    <message>
      <source>The filename must be a string.</source>
      <comment>python error</comment>
      <translation>Tiedostonimen täytyy olla string.</translation>
    </message>
    <message>
      <source>Cannot delete image type settings.</source>
      <comment>python error</comment>
      <translation>Kuvan tyypin asetuksia ei voi poistaa</translation>
    </message>
    <message>
      <source>The image type must be a string.</source>
      <comment>python error</comment>
      <translation>Kuvan tyypin täytyy olla string.</translation>
    </message>
    <message>
      <source>'allTypes' attribute is READ-ONLY</source>
      <comment>python error</comment>
      <translation>'allTypes' parametri on ainoastaan lukua varten</translation>
    </message>
    <message>
      <source>Failed to export image</source>
      <comment>python error</comment>
      <translation>Kuvan vienti epäonnistui</translation>
    </message>
    <message>
      <source>Custom (optional) configuration: </source>
      <comment>short words plugin</comment>
      <translation>Oma (valinnainen) asetus:</translation>
    </message>
    <message>
      <source>Standard configuration: </source>
      <comment>short words plugin</comment>
      <translation>Perusasetus: </translation>
    </message>
    <message>
      <source>Short Words processing. Wait please...</source>
      <comment>short words plugin</comment>
      <translation>Short Words. Odota hetki...</translation>
    </message>
    <message>
      <source>Short Words processing. Done.</source>
      <comment>short words plugin</comment>
      <translation>Short Words valmis.</translation>
    </message>
    <message>
      <source>Color not found.</source>
      <comment>python error</comment>
      <translation>Väriä ei löydy.</translation>
    </message>
    <message>
      <source>Color not found in document.</source>
      <comment>python error</comment>
      <translation>Väriä ei löydy asiakirjasta.</translation>
    </message>
    <message>
      <source>Color not found in default colors.</source>
      <comment>python error</comment>
      <translation>Väriä ei löydy oletusväreistä</translation>
    </message>
    <message>
      <source>Cannot scale by 0%.</source>
      <comment>python error</comment>
      <translation>Kokoa ei voi skaalta 0 %:lla</translation>
    </message>
    <message>
      <source>Specified item not an image frame.</source>
      <comment>python error</comment>
      <translation>Valittu kohde ei ole kuvakehys.</translation>
    </message>
    <message>
      <source>Font not found.</source>
      <comment>python error</comment>
      <translation>Kirjaintyyppiä ei löydy.</translation>
    </message>
    <message>
      <source>Cannot render an empty sample.</source>
      <comment>python error</comment>
      <translation>Tyhjää esimerkkiä ei voi renderöidä</translation>
    </message>
    <message>
      <source>Cannot have an empty layer name.</source>
      <comment>python error</comment>
      <translation>Tasolla täytyy olla nimi</translation>
    </message>
    <message>
      <source>Layer not found.</source>
      <comment>python error</comment>
      <translation>Tasoa ei löydy.</translation>
    </message>
    <message>
      <source>Cannot remove the last layer.</source>
      <comment>python error</comment>
      <translation>Viimeistä tasoa ei voi poistaa</translation>
    </message>
    <message>
      <source>Cannot create layer without a name.</source>
      <comment>python error</comment>
      <translation>Tasoa ei voi luoda ilman nimeä</translation>
    </message>
    <message>
      <source>Insert index out of bounds.</source>
      <comment>python error</comment>
      <translation>Lisäysindeksi virheellinen.</translation>
    </message>
    <message>
      <source>Cannot set text alignment on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Tekstin tasauksen voi asettaa ainoastaan tekstikehykselle</translation>
    </message>
    <message>
      <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
      <comment>python error</comment>
      <translation>Kirjaintyypin koko virheellinen. Täytyy olla 1 &lt;= koko &lt;= 512.</translation>
    </message>
    <message>
      <source>Cannot set font size on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Kirjaintyypin koon voi asettaa vain tekstikehyksessä.</translation>
    </message>
    <message>
      <source>Cannot set font on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Kirjaintyypin voi asettaa vain tekstikehyksessä.</translation>
    </message>
    <message>
      <source>Line space out of bounds, must be >= 0.1.</source>
      <comment>python error</comment>
      <translation>Riviväli virheellinen. Täytyy olla >= 0,1.</translation>
    </message>
    <message>
      <source>Cannot set line spacing on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Rivivälin voi asettaa ainoastaan tekstikehykselle.</translation>
    </message>
    <message>
      <source>Column gap out of bounds, must be positive.</source>
      <comment>python error</comment>
      <translation>Palstojen väli virheellinen. Täytyy olla >= 0.</translation>
    </message>
    <message>
      <source>Cannot set column gap on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Palstojen välin voi asettaa ainoastaan tekstikehyksellä</translation>
    </message>
    <message>
      <source>Column count out of bounds, must be > 1.</source>
      <comment>python error</comment>
      <translation>Palstojen lukumäärä virheellinen. Täytyy olla > 1.</translation>
    </message>
    <message>
      <source>Cannot set number of columns on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Palstojen lukumäärän voi asettaa ainoastaan tekstikehykselle</translation>
    </message>
    <message>
      <source>Cannot select text in a non-text frame</source>
      <comment>python error</comment>
      <translation>Tekstiä voi valita ainoastaan tekstikehyksestä</translation>
    </message>
    <message>
      <source>Cannot delete text from a non-text frame.</source>
      <comment>python error</comment>
      <translation>Tekstiä ei voi poistaa muusta kuin tekstikehyksestä</translation>
    </message>
    <message>
      <source>Cannot set text fill on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Tekstin täyttövärin voi asettaa ainoastaan tekstikehykselle</translation>
    </message>
    <message>
      <source>Cannot set text stroke on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Tekstin reunuksen voi asettaa vain tekstikehykselle</translation>
    </message>
    <message>
      <source>Cannot set text shade on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Tekstin täyttövärin sävyn voi asettaa ainoastaan tekstikehykselle</translation>
    </message>
    <message>
      <source>Can only link text frames.</source>
      <comment>python error</comment>
      <translation>Ainoastaan tekstikehyksiä voi linkittää</translation>
    </message>
    <message>
      <source>Target frame must be empty.</source>
      <comment>python error</comment>
      <translation>Kohdekehyksen täytyy olla tyhjä.</translation>
    </message>
    <message>
      <source>Target frame links to another frame.</source>
      <comment>python error</comment>
      <translation>Kohdekehys linkitetään toiseen kehykseen.</translation>
    </message>
    <message>
      <source>Target frame is linked to by another frame.</source>
      <comment>python error</comment>
      <translation>Kohdekehys on linkitetty toiseen kehykseen.</translation>
    </message>
    <message>
      <source>Source and target are the same object.</source>
      <comment>python error</comment>
      <translation>Lähde ja kohde ovat samat.</translation>
    </message>
    <message>
      <source>Cannot unlink a non-text frame.</source>
      <comment>python error</comment>
      <translation>Ainoastaan tekstikehyksestä linkitys voidaan poistaa</translation>
    </message>
    <message>
      <source>Cannot convert a non-text frame to outlines.</source>
      <comment>python error</comment>
      <translation>Ainoastaan tekstikehyksen voi muuntaa ääriviivoiksi</translation>
    </message>
    <message>
      <source>Can't set bookmark on a non-text frame</source>
      <comment>python error</comment>
      <translation>Kirjanmerkkiä ei voi asettaa kuin tekstikehykselle</translation>
    </message>
    <message>
      <source>Can't get info from a non-text frame</source>
      <comment>python error</comment>
      <translation>Informaatiota ei voi saada muista kuin tekstikehyksistä</translation>
    </message>
    <message>
      <source>OpenDocument Text Documents</source>
      <translation>OpenDocument tekstiasiakirjat</translation>
    </message>
    <message>
      <source>Croatian</source>
      <translation>kroatia</translation>
    </message>
    <message>
      <source>Portuguese</source>
      <translation>portugali</translation>
    </message>
    <message>
      <source>Portuguese (BR)</source>
      <translation>portugali (BR)</translation>
    </message>
    <message>
      <source>Scribus Crash</source>
      <translation>Scribus kaatui</translation>
    </message>
    <message>
      <source>Scribus crashes due to Signal #%1</source>
      <translation>Scribus kaatui: Signal #%1</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation>&amp;OK</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>Oma</translation>
    </message>
    <message>
      <source>Page</source>
      <translation>Sivu</translation>
    </message>
    <message>
      <source>Master Page </source>
      <translation>Mallisivu </translation>
    </message>
    <message>
      <source>4A0</source>
      <translation>4A0</translation>
    </message>
    <message>
      <source>2A0</source>
      <translation>2A0</translation>
    </message>
    <message>
      <source>Comm10E</source>
      <translation>Comm10E</translation>
    </message>
    <message>
      <source>DLE</source>
      <translation>DLE</translation>
    </message>
    <message>
      <source>Could not open output file %1</source>
      <translation>Tiedostoa %1 ei voitu avata kirjoittamista varten</translation>
    </message>
    <message>
      <source>Output stream not writeable</source>
      <translation>Ulosmenovirtaan ei voi kirjoittaa</translation>
    </message>
    <message>
      <source>Verification of settings failed: %1</source>
      <translation>Asetusten varmistus epäonnistui: %1</translation>
    </message>
    <message>
      <source>Could not open input file %1</source>
      <translation>Tiedosto %1 ei voitu avata lukemista varten</translation>
    </message>
    <message>
      <source>Unable to read settings XML:</source>
      <translation>Asetustiedostoa ei voitu lukea</translation>
    </message>
    <message>
      <source>%1 (line %2 col %3)</source>
      <comment>Load PDF settings</comment>
      <translation>%1 (rivi %2 sarake %3)</translation>
    </message>
    <message>
      <source>Unable to read settings XML: %1</source>
      <translation>Asetustiedostoa ei voitu lukea: %1</translation>
    </message>
    <message>
      <source>null root node</source>
      <comment>Load PDF settings</comment>
      <translation>juurisolmu on null</translation>
    </message>
    <message>
      <source>&lt;pdfVersion> invalid</source>
      <comment>Load PDF settings</comment>
      <translation>&lt;pdfVersion> virheellinen</translation>
    </message>
    <message>
      <source>found %1 &lt;%2> nodes, need 1.</source>
      <comment>Load PDF settings</comment>
      <translation>löytyi %1 &lt;%2> solmua, tarvitaan 1</translation>
    </message>
    <message>
      <source>unexpected null &lt;%2> node</source>
      <comment>Load PDF settings</comment>
      <translation>odottamaton null &lt;%2> solmu</translation>
    </message>
    <message>
      <source>node &lt;%1> not an element</source>
      <comment>Load PDF settings</comment>
      <translation>solmu &lt;%1> ei ole elementti</translation>
    </message>
    <message>
      <source>element &lt;%1> lacks `value' attribute</source>
      <comment>Load PDF settings</comment>
      <translation>elementistä &lt;%1> puuttuu value-muuttuja</translation>
    </message>
    <message>
      <source>element &lt;%1> value must be `true' or `false'</source>
      <comment>Load PDF settings</comment>
      <translation>elementin &lt;%1> arvo täytyy olla true tai false</translation>
    </message>
    <message>
      <source>element &lt;lpiSettingsEntry> lacks `name' attribute</source>
      <comment>Load PDF settings</comment>
      <translation>elementistä &lt;lpiSettingsEntry> puuttuu name-muuttuja</translation>
    </message>
    <message>
      <source>Freetype2 library not available</source>
      <translation>Freetype2-kirjastoa ei löydy</translation>
    </message>
    <message>
      <source>Font %1 is broken, no embedding</source>
      <translation>Kirjaintyyppi %1 on viallinen, ei upoteta</translation>
    </message>
    <message>
      <source>Font %1 is broken (read stream), no embedding</source>
      <translation>Kirjaintyyppi %1 on viallinen, ei upoteta</translation>
    </message>
    <message>
      <source>Font %1 is broken (FreeType2), discarding it</source>
      <translation>Kirjaintyyppi %1 on viallinen (FreeType2), ohitetaan se</translation>
    </message>
    <message>
      <source>Font %1 is broken (no Face), discarding it</source>
      <translation>Kirjaintyyppi %1 on viallinen, ohitetaan se</translation>
    </message>
    <message>
      <source>Font %1 has broken glyph %2 (charcode %3)</source>
      <translation>Kirjaintyypissä %1 on viallinen merkki %2 (merkkikoodi %3)</translation>
    </message>
    <message>
      <source>Font %1 is broken and will be discarded</source>
      <translation>Kirjaintyyppi %1 on viallinen. Sitä ei käytetä</translation>
    </message>
    <message>
      <source>Font %1 cannot be read, no embedding</source>
      <translation>Kirjaintyyppiä %1 ei voida lukea, ei upotusta</translation>
    </message>
    <message>
      <source>Failed to load font %1 - font type unknown</source>
      <translation>Kirjaintyypin %1 lataus epäonnistui, tuntematon tiedostotyyppi</translation>
    </message>
    <message>
      <source>Font %1 loaded from %2(%3)</source>
      <translation>Kirjaintyyppi %1 ladattiin %2(%3)</translation>
    </message>
    <message>
      <source>Font %1(%2) is duplicate of %3</source>
      <translation>Kirjaintyyppi %1(%2) on kopio tyypistä %3</translation>
    </message>
    <message>
      <source>Loading font %1 (found using fontconfig)</source>
      <translation>Ladataan kirjaintyyppiä %1 (löydettiin fontconfigilla)</translation>
    </message>
    <message>
      <source>Font %1 (found using fontconfig) is broken, discarding it</source>
      <translation>Kirjaintyyppi %1 (löydettiin fontconfigilla) on viallinen. Sitä ei käytetä</translation>
    </message>
    <message>
      <source>Failed to load a font - freetype2 couldn't find the font file</source>
      <translation>Kirjaintyypin lataus epäonnistui. Freetype2 ei löytänyt tiedostoa</translation>
    </message>
    <message>
      <source>Font %1 is broken (FreeType), discarding it</source>
      <translation>Kirjaintyyppi %1 on viallinen (FreeType). Sitä ei käytetä</translation>
    </message>
    <message>
      <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
      <translation>Kirjaintyypillä %1 on viallinen merkki %2 (merkkikoodi %3). Sitä ei käytetä</translation>
    </message>
    <message>
      <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
      <translation>haetaan perhettä %1 kirjaintyypistä %1 (sirrtymä=%3, nTaulu=%4)</translation>
    </message>
    <message>
      <source>memcpy header: %1 %2 %3</source>
      <translation>memcpy otsikko: %1 %2 %3</translation>
    </message>
    <message>
      <source>table '%1'</source>
      <translation>taulukko '%1'</translation>
    </message>
    <message>
      <source>memcpy table: %1 %2 %3</source>
      <translation>memcpy taulukko: %1 %2 %3</translation>
    </message>
    <message>
      <source>memcpy offset: %1 %2 %3</source>
      <translation>memcpy offset: %1 %2 %3</translation>
    </message>
    <message>
      <source>Scribus Development Version</source>
      <translation>Scribus kehitysversio</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source> mm</source>
      <translation> mm</translation>
    </message>
    <message>
      <source> in</source>
      <translation> in</translation>
    </message>
    <message>
      <source> p</source>
      <translation> p</translation>
    </message>
    <message>
      <source> cm</source>
      <translation> cm</translation>
    </message>
    <message>
      <source> c</source>
      <translation> c</translation>
    </message>
    <message>
      <source>pt</source>
      <translation>pt</translation>
    </message>
    <message>
      <source>mm</source>
      <translation>mm</translation>
    </message>
    <message>
      <source>in</source>
      <translation>in</translation>
    </message>
    <message>
      <source>p</source>
      <translation>p</translation>
    </message>
    <message>
      <source>cm</source>
      <translation>cm</translation>
    </message>
    <message>
      <source>c</source>
      <translation>c</translation>
    </message>
    <message>
      <source>Points (pt)</source>
      <translation>Pisteet (pt)</translation>
    </message>
    <message>
      <source>Millimeters (mm)</source>
      <translation>Millimetrit (mm)</translation>
    </message>
    <message>
      <source>Inches (in)</source>
      <translation>Tuumat (in)</translation>
    </message>
    <message>
      <source>Picas (p)</source>
      <translation>Picat (p)</translation>
    </message>
    <message>
      <source>Centimeters (cm)</source>
      <translation>Senttimetrit (cm)</translation>
    </message>
    <message>
      <source>Cicero (c)</source>
      <translation>Cicerot (c)</translation>
    </message>
    <message>
      <source>File exists</source>
      <translation>Tiedosto on olemassa</translation>
    </message>
    <message>
      <source>&amp;Replace</source>
      <translation>&amp;Korvaa</translation>
    </message>
    <message>
      <source>All</source>
      <translation>Kaikki</translation>
    </message>
    <message>
      <source>Document Template: </source>
      <translation>Asiakirjamalli:</translation>
    </message>
    <message>
      <source>Failed to open document.</source>
      <comment>python error</comment>
      <translation>Asiakirjan avaaminen epäonnistui</translation>
    </message>
    <message>
      <source>Failed to save document.</source>
      <comment>python error</comment>
      <translation>Asiakirjan tallentaminen epäonnistui</translation>
    </message>
    <message>
      <source>Argument must be page item name, or PyCObject instance</source>
      <translation>Parametri täytyy olla jonkin kohteen nimi tai ilmentymä PyCObjectista</translation>
    </message>
    <message>
      <source>Property not found</source>
      <translation>Ominaisuutta ei löydy</translation>
    </message>
    <message>
      <source>Child not found</source>
      <translation>Lasta ei löydy</translation>
    </message>
    <message>
      <source>Couldn't convert result type '%1'.</source>
      <translation>Tuloksen tyyppiä &quot;%1&quot; ei voitu muuntaa</translation>
    </message>
    <message>
      <source>Property type '%1' not supported</source>
      <translation>Ominaisuuden tyyppi &quot;%1&quot; ei ole tuettu</translation>
    </message>
    <message>
      <source>Couldn't convert '%1' to property type '%2'</source>
      <translation>Tyyppiä &quot;%1&quot; ei voitu muuttaa tyypiksi &quot;%2&quot;</translation>
    </message>
    <message>
      <source>Types matched, but setting property failed.</source>
      <translation>Tyypit olivat samat, mutta ominaisuuden asetus epäonnistui</translation>
    </message>
    <message>
      <source>Cannot group less than two items</source>
      <comment>python error</comment>
      <translation>Kahta kohdetta vähempää ei voi ryhmittää</translation>
    </message>
    <message>
      <source>Can't group less than two items</source>
      <comment>python error</comment>
      <translation>Kahta kohdetta vähempää ei voi ryhmittää</translation>
    </message>
    <message>
      <source>Need selection or argument list of items to group</source>
      <comment>python error</comment>
      <translation>Tarvitaan valinta tai lista kohteista ryhmitettäväksi</translation>
    </message>
    <message>
      <source>Unable to save pixmap</source>
      <comment>scripter error</comment>
      <translation>Bittikarttakuvaa ei voi tallentaa</translation>
    </message>
    <message>
      <source>An object with the requested name already exists.</source>
      <comment>python error</comment>
      <translation>Kohde halutulla nimellä on jo olemassa</translation>
    </message>
    <message>
      <source>Point list must contain at least two points (four values).</source>
      <comment>python error</comment>
      <translation>Pistelistan täytyy sisältää vähintään kaksi pistettä (neljä arvoa).</translation>
    </message>
    <message>
      <source>Point list must contain an even number of values.</source>
      <comment>python error</comment>
      <translation>Pistelistan täytyy sisältää parillinen määrä arvoja.</translation>
    </message>
    <message>
      <source>Point list must contain at least three points (six values).</source>
      <comment>python error</comment>
      <translation>Pistelistan täytyy sisältää vähintään kolme pistettä (kuusi arvoa).</translation>
    </message>
    <message>
      <source>Point list must contain at least four points (eight values).</source>
      <comment>python error</comment>
      <translation>Pistelistan täytyy sisältää vähintään neljä pistettä (kahdeksan arvoa).</translation>
    </message>
    <message>
      <source>Point list must have a multiple of six values.</source>
      <comment>python error</comment>
      <translation>Pistelistan täytyy sisältää arvoja kuuden monikerran verran.</translation>
    </message>
    <message>
      <source>Object not found.</source>
      <comment>python error</comment>
      <translation>Kohdetta ei löydy.</translation>
    </message>
    <message>
      <source>Style not found.</source>
      <comment>python error</comment>
      <translation>Tyyliä ei löydy.</translation>
    </message>
    <message>
      <source>Cannot set style on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Tyyliä ei voi asettaa muulle kuin tekstikentälle.</translation>
    </message>
    <message>
      <source>Failed to save EPS.</source>
      <comment>python error</comment>
      <translation>EPS-tiedoston tallennus epäonnistui.</translation>
    </message>
    <message>
      <source>Page number out of range.</source>
      <comment>python error</comment>
      <translation>Sivunumero on virheellinen.</translation>
    </message>
    <message>
      <source>argument is not list: must be list of float values.</source>
      <comment>python error</comment>
      <translation>parametri ei ole lista: vaaditaan lista float-arvoja.</translation>
    </message>
    <message>
      <source>argument contains non-numeric values: must be list of float values.</source>
      <comment>python error</comment>
      <translation>parametri sisältää muita kuin numeroarvoja: vaaditaan lista float-arvoja.</translation>
    </message>
    <message>
      <source>argument contains no-numeric values: must be list of float values.</source>
      <comment>python error</comment>
      <translation>parametri sisältää muita kuin numeroarvoja: vaaditaan lista float-arvoja</translation>
    </message>
    <message>
      <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
      <comment>python error</comment>
      <translation>Viivan leveys sallittujen rajojen ulkopuolella, täytyy olla välillä 0 &lt;= viivan leveys &lt;= 12</translation>
    </message>
    <message>
      <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
      <comment>python error</comment>
      <translation>Viivan sävy sallitun rajan ulkopuolella. Täytyy olla välillä 0 &lt;= viivan sävy &lt;= 100</translation>
    </message>
    <message>
      <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
      <comment>python error</comment>
      <translation>Täyttövärin sävy sallitun rajan ulkopuolella. Täytyy olla välillä 0 &lt;= sävy &lt;= 100</translation>
    </message>
    <message>
      <source>Line style not found.</source>
      <comment>python error</comment>
      <translation>Viivatyyliä ei löydy</translation>
    </message>
    <message>
      <source>Object is not a linked text frame, can't unlink.</source>
      <comment>python error</comment>
      <translation>Kohde ei ole linkitetty tekstikehys. Ei voi poistaa linkitystä.</translation>
    </message>
    <message>
      <source>Object the last frame in a series, can't unlink. Unlink the previous frame instead.</source>
      <comment>python error</comment>
      <translation>Kohde on viimeinen kehys sarjassa. Ei voi poistaa linkitystä. Poista linkitys edellisestä kehyksestä tämän sijaan.</translation>
    </message>
    <message>
      <source>Only text frames can be checked for overflowing</source>
      <comment>python error</comment>
      <translation>Ainoastaan tekstikehyksestä voi tarkastaa ylivuodon </translation>
    </message>
    <message>
      <source>The filename should not be empty string.</source>
      <comment>python error</comment>
      <translation>Tiedostonimi ei voi olla tyhjä merkkijono.</translation>
    </message>
    <message>
      <source>&amp;Execute Script...</source>
      <translation>&amp;Aja skripti...</translation>
    </message>
    <message>
      <source>Show &amp;Console</source>
      <translation>Näytä &amp;konsoli</translation>
    </message>
    <message>
      <source>&amp;About Script...</source>
      <translation>&amp;Tietoja skriptistä...</translation>
    </message>
    <message>
      <source>&amp;Script</source>
      <translation>&amp;Skripti</translation>
    </message>
    <message>
      <source>&amp;Scribus Scripts</source>
      <translation>&amp;Scribus-skriptit</translation>
    </message>
    <message>
      <source>&amp;Recent Scripts</source>
      <translation>Vii&amp;meaikaiset skriptit</translation>
    </message>
    <message>
      <source>About Script</source>
      <translation>Tietoja skriptistä</translation>
    </message>
    <message>
      <source>Scribus Python interface module

This module is the Python interface for Scribus. It provides functions
to control scribus and to manipulate objects on the canvas. Each
function is documented individually below.

A few things are common across most of the interface.

Most functions operate on frames. Frames are identified by their name,
a string - they are not real Python objects. Many functions take an
optional (non-keyword) parameter, a frame name.
Many exceptions are also common across most functions. These are
not currently documented in the docstring for each function.
- Many functions will raise a NoDocOpenError if you try to use them
without a document to operate on.
- If you do not pass a frame name to a function that requires one,
the function will use the currently selected frame, if any, or
raise a NoValidObjectError if it can't find anything to operate
on.
- Many functions will raise WrongFrameTypeError if you try to use them
on a frame type that they do not make sense with. For example, setting
the text color on a graphics frame doesn't make sense, and will result
in this exception being raised.
- Errors resulting from calls to the underlying Python API will be
passed through unaltered. As such, the list of exceptions thrown by
any function as provided here and in its docstring is incomplete.

Details of what exceptions each function may throw are provided on the
function's documentation, though as with most Python code this list
is not exhaustive due to exceptions from called functions.
</source>
      <translation>Scribus Python interface module

This module is the Python interface for Scribus. It provides functions
to control scribus and to manipulate objects on the canvas. Each
function is documented individually below.

A few things are common across most of the interface.

Most functions operate on frames. Frames are identified by their name,
a string - they are not real Python objects. Many functions take an
optional (non-keyword) parameter, a frame name.
Many exceptions are also common across most functions. These are
not currently documented in the docstring for each function.
- Many functions will raise a NoDocOpenError if you try to use them
without a document to operate on.
- If you do not pass a frame name to a function that requires one,
the function will use the currently selected frame, if any, or
raise a NoValidObjectError if it can't find anything to operate
on.
- Many functions will raise WrongFrameTypeError if you try to use them
on a frame type that they do not make sense with. For example, setting
the text color on a graphics frame doesn't make sense, and will result
in this exception being raised.
- Errors resulting from calls to the underlying Python API will be
passed through unaltered. As such, the list of exceptions thrown by
any function as provided here and in its docstring is incomplete.

Details of what exceptions each function may throw are provided on the
function's documentation, though as with most Python code this list
is not exhaustive due to exceptions from called functions.
</translation>
    </message>
    <message>
      <source>Copy #%1 of </source>
      <translation>Kopio #%1/</translation>
    </message>
    <message>
      <source>Black</source>
      <translation>Musta</translation>
    </message>
    <message>
      <source>Cyan</source>
      <translation>Syaani</translation>
    </message>
    <message>
      <source>Magenta</source>
      <translation>Magenta</translation>
    </message>
    <message>
      <source>Yellow</source>
      <translation>Keltainen</translation>
    </message>
    <message>
      <source>Creating Font Cache</source>
      <translation>Luodaan välimuistia kirjaintyypeille</translation>
    </message>
    <message>
      <source>New Font found, checking...</source>
      <translation>Uusi kirjaintyyppi löydetty, tarkastetaan...</translation>
    </message>
    <message>
      <source>Modified Font found, checking...</source>
      <translation>Muokattu kirjaintyyppi löytyi, tarkastetaan...</translation>
    </message>
    <message>
      <source>Reading Font Cache</source>
      <translation>Luetaan kirjaintyyppien välimuistia</translation>
    </message>
    <message>
      <source>Writing updated Font Cache</source>
      <translation>Päivitetään kirjaintyyppien välimuistia</translation>
    </message>
    <message>
      <source>Searching for Fonts</source>
      <translation>Etsitään kirjaintyyppejä</translation>
    </message>
    <message>
      <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File->Save As. Are you sure you wish to proceed with this operation?</source>
      <translation>Käytät Scribuksen kehitysversiota 1.3.x. Asiakirja, jonka parissa työskentelet luotiin Scribuksen versiolla 1.2.3 tai aikaisempi. Jos tallennat asiakirjan sitä ei voi enää avata Scribuksen versiossa 1.2.3, ellet valitse Tiedost->Tallenna nimellä. Haluatko jatkaa tallentamista?</translation>
    </message>
    <message>
      <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
      <translation>Muutoksia asiakirjaan ei ole tallennettu ja olet pyytänyt palauttamaan viimeksi tallennetun. Haluatko kumota muutokset ja palauttaa tallennetun asiakirjan?</translation>
    </message>
    <message>
      <source>A file named '%1' already exists.&lt;br/>Do you want to replace it with the file you are saving?</source>
      <translation>Tiedosto &quot;%1&quot; on jo olemassa &lt;br/>Haluatko korvata sen?</translation>
    </message>
    <message>
      <source>page</source>
      <comment>page export</comment>
      <translation>sivu</translation>
    </message>
    <message>
      <source>Color Wheel</source>
      <translation>Väripyörä</translation>
    </message>
    <message>
      <source>Font Preview</source>
      <translation>Kirjaintyypin esikatselu</translation>
    </message>
    <message>
      <source>My Plugin</source>
      <translation>Oma liitännäinen</translation>
    </message>
    <message>
      <source>New From Template</source>
      <translation>Uusi mallista</translation>
    </message>
    <message>
      <source>Export As Image</source>
      <translation>Vie kuvana</translation>
    </message>
    <message>
      <source>PS/EPS Importer</source>
      <translation>PS/EPS-tuoja</translation>
    </message>
    <message>
      <source>Save As Template</source>
      <translation>Tallenna mallina</translation>
    </message>
    <message>
      <source>firstPageOrder is bigger than allowed.</source>
      <comment>python error</comment>
      <translation>firstPageOrder on sallittua arvoa suurempi</translation>
    </message>
    <message>
      <source>Scripter</source>
      <translation>Skripteri</translation>
    </message>
    <message>
      <source>Short Words</source>
      <translation>Short Words</translation>
    </message>
    <message>
      <source>SVG Export</source>
      <translation>SVG-vienti</translation>
    </message>
    <message>
      <source>SVG Import</source>
      <translation>SVG-tuonti</translation>
    </message>
    <message>
      <source>Old .sla format support</source>
      <translation>Vanhan .sla tiedostomuodon tuki</translation>
    </message>
    <message>
      <source>OpenOffice.org Draw Importer</source>
      <translation>OpenOffice.org Draw -tuoja</translation>
    </message>
    <message>
      <source>Scribus crashes due to the following exception : %1</source>
      <translation>Scribus kaatui seuraavaan poikkeukseen: %1</translation>
    </message>
    <message>
      <source>German (Trad.)</source>
      <translation>saksa (perint.)</translation>
    </message>
    <message>
      <source>Thai</source>
      <translation>thai</translation>
    </message>
    <message>
      <source>Exporting PostScript File</source>
      <translation>Viedään PostScript-tiedosto</translation>
    </message>
    <message>
      <source>Printing File</source>
      <translation>Tulostetaan tiedostoa</translation>
    </message>
    <message>
      <source>&lt;p>You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p>Choose one of the following:&lt;br>&lt;ul>&lt;li>&lt;b>Create&lt;/b> missing pages&lt;/li>&lt;li>&lt;b>Import&lt;/b> pages until the last page&lt;/li>&lt;li>&lt;b>Cancel&lt;/b>&lt;/li>&lt;/ul></source>
      <translation>&lt;p>Yrität tuoda enemmän sivuja kuin asiakirjassa tällä hetkellä on aktiivisesta sivusta alkaen.&lt;/p>Valitse yksi seuraavista:&lt;br>&lt;ul>&lt;li>&lt;b>Luo&lt;/b> puuttuvat sivut&lt;/li>&lt;li>&lt;b>Tuo&lt;/b> sivut viimeiseen sivuun saakka&lt;/li>&lt;li>&lt;b>Peruuta&lt;/b>&lt;/li>&lt;/ul></translation>
    </message>
    <message>
      <source>C&amp;reate</source>
      <translation>&amp;Luo</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>T&amp;uo</translation>
    </message>
    <message>
      <source>Barcode Generator</source>
      <translation>Viivakoodi</translation>
    </message>
  </context>
  <context>
    <name>QTextEdit</name>
    <message>
      <source>Clear</source>
      <translation>Tyhjennä</translation>
    </message>
    <message>
      <source>Select All</source>
      <translation>Valitse kaikki</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>K&amp;umoa</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>Tee &amp;uudelleen</translation>
    </message>
    <message>
      <source>Cu&amp;t</source>
      <translation>&amp;Leikkaa</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>&amp;Kopioi</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>L&amp;iitä</translation>
    </message>
  </context>
  <context>
    <name>QTitleBar</name>
    <message>
      <source>System Menu</source>
      <translation>Järjestelmän valikko</translation>
    </message>
    <message>
      <source>Shade</source>
      <translation>Varjosta</translation>
    </message>
    <message>
      <source>Unshade</source>
      <translation>Paljasta</translation>
    </message>
    <message>
      <source>Normalize</source>
      <translation>Palauta</translation>
    </message>
    <message>
      <source>Minimize</source>
      <translation>Pienennä</translation>
    </message>
    <message>
      <source>Maximize</source>
      <translation>Suurenna</translation>
    </message>
    <message>
      <source>Close</source>
      <translation>Sulje</translation>
    </message>
  </context>
  <context>
    <name>QWorkspace</name>
    <message>
      <source>&amp;Restore</source>
      <translation>P&amp;alauta</translation>
    </message>
    <message>
      <source>&amp;Move</source>
      <translation>S&amp;iirrä</translation>
    </message>
    <message>
      <source>&amp;Size</source>
      <translation>&amp;Koko</translation>
    </message>
    <message>
      <source>Mi&amp;nimize</source>
      <translation>&amp;Pienennä</translation>
    </message>
    <message>
      <source>Ma&amp;ximize</source>
      <translation>S&amp;uurenna</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Sulje</translation>
    </message>
    <message>
      <source>Stay on &amp;Top</source>
      <translation>Pää&amp;llimmäisenä</translation>
    </message>
    <message>
      <source>Minimize</source>
      <translation>Pienennä</translation>
    </message>
    <message>
      <source>Restore Down</source>
      <translation>Palauta</translation>
    </message>
    <message>
      <source>Close</source>
      <translation>Sulje</translation>
    </message>
    <message>
      <source>Sh&amp;ade</source>
      <translation>Va&amp;rjosta</translation>
    </message>
    <message>
      <source>%1 - [%2]</source>
      <translation>%1 - [%2]</translation>
    </message>
    <message>
      <source>&amp;Unshade</source>
      <translation>Paljas&amp;ta</translation>
    </message>
  </context>
  <context>
    <name>ReformDoc</name>
    <message>
      <source>Document Setup</source>
      <translation>Asiakirjan asettelu</translation>
    </message>
    <message>
      <source>Margin Guides</source>
      <translation>Reunukset</translation>
    </message>
    <message>
      <source>&amp;Top:</source>
      <translation>&amp;Ylhäällä:</translation>
    </message>
    <message>
      <source>&amp;Left:</source>
      <translation>&amp;Vasen:</translation>
    </message>
    <message>
      <source>&amp;Bottom:</source>
      <translation>&amp;Alhaalla:</translation>
    </message>
    <message>
      <source>&amp;Right:</source>
      <translation>O&amp;ikea:</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>Sivun koko</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>Oma</translation>
    </message>
    <message>
      <source>Portrait</source>
      <translation>Pystysuora</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation>Vaakasuora</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Koko:</translation>
    </message>
    <message>
      <source>Orie&amp;ntation:</source>
      <translation>&amp;Suunta:</translation>
    </message>
    <message>
      <source>&amp;Width:</source>
      <translation>&amp;Leveys:</translation>
    </message>
    <message>
      <source>&amp;Height:</source>
      <translation>&amp;Korkeus:</translation>
    </message>
    <message>
      <source>&amp;Unit:</source>
      <translation>&amp;Yksikkö:</translation>
    </message>
    <message>
      <source>Autosave</source>
      <translation>Automaattinen tallennus</translation>
    </message>
    <message>
      <source>min</source>
      <translation>min</translation>
    </message>
    <message>
      <source>&amp;Interval:</source>
      <translation>&amp;Väli:</translation>
    </message>
    <message>
      <source>Document</source>
      <translation>Asiakirja</translation>
    </message>
    <message>
      <source>Document Information</source>
      <translation>Asiakirjan tiedot</translation>
    </message>
    <message>
      <source>Guides</source>
      <translation>Apuviivat</translation>
    </message>
    <message>
      <source>Page Display</source>
      <translation>Sivun näyttö</translation>
    </message>
    <message>
      <source>Color:</source>
      <translation>Väri:</translation>
    </message>
    <message>
      <source>Display &amp;Unprintable Area in Margin Color</source>
      <translation>N&amp;äytä tulostumaton alue reunuksen värillä</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation>Alt+U</translation>
    </message>
    <message>
      <source>Show Pictures</source>
      <translation>Näytä kuvat</translation>
    </message>
    <message>
      <source>Show Text Chains</source>
      <translation>Näytä tekstiketjut</translation>
    </message>
    <message>
      <source>Show Text Control Characters</source>
      <translation>Näytä tulostumattomat merkit</translation>
    </message>
    <message>
      <source>Show Frames</source>
      <translation>Näytä kehykset</translation>
    </message>
    <message>
      <source>Display</source>
      <translation>Näyttö</translation>
    </message>
    <message>
      <source>Typography</source>
      <translation>Typografia</translation>
    </message>
    <message>
      <source>Tools</source>
      <translation>Työkalut</translation>
    </message>
    <message>
      <source>Hyphenator</source>
      <translation>Tavutus</translation>
    </message>
    <message>
      <source>Fonts</source>
      <translation>Kirjaintyypit</translation>
    </message>
    <message>
      <source>PDF Export</source>
      <translation>PDF-vienti</translation>
    </message>
    <message>
      <source>Document Item Attributes</source>
      <translation>Kohteen muuttujat</translation>
    </message>
    <message>
      <source>Table of Contents and Indexes</source>
      <translation>Sisällysluettelo ja hakemistot</translation>
    </message>
    <message>
      <source>Color Management</source>
      <translation>Värien hallinta</translation>
    </message>
    <message>
      <source>Display non-printing characters such as paragraph markers in text frames</source>
      <translation>Näytä tulostumattomat merkit kuten kappalemerkki tekstikehyksissä</translation>
    </message>
    <message>
      <source>Turns the display of frames on or off</source>
      <translation>Näytä tai piilota kehykset</translation>
    </message>
    <message>
      <source>Turns the display of pictures on or off</source>
      <translation>Näytä tai piilota kuvat</translation>
    </message>
    <message>
      <source>Color for paper</source>
      <translation>Paperin väri</translation>
    </message>
    <message>
      <source>Mask the area outside the margins in the margin color</source>
      <translation>Merkkaa marginaalien ulkopuolinen alue marginaalin värillä</translation>
    </message>
    <message>
      <source>Preflight Verifier</source>
      <translation>Tarkastustyökalu</translation>
    </message>
    <message>
      <source>Rulers relative to Page</source>
      <translation>Viivaimet suhteessa sivuun</translation>
    </message>
    <message>
      <source>Minimum Scratch Space</source>
      <translation>Pienin mahdollinen työalue</translation>
    </message>
    <message>
      <source>Gaps between Pages</source>
      <translation>Sivujen välin pituus</translation>
    </message>
    <message>
      <source>Horizontal:</source>
      <translation>Vaakasuora:</translation>
    </message>
    <message>
      <source>Vertical:</source>
      <translation>Pystysuora:</translation>
    </message>
    <message>
      <source>Sections</source>
      <translation>Alueet</translation>
    </message>
    <message>
      <source>Enable or disable the display of linked text frames.</source>
      <translation>Näytä tai piilota linkitettyjen tekstikehysten merkkaus.</translation>
    </message>
    <message>
      <source>Adjusting Colors</source>
      <translation>Värien säätäminen</translation>
    </message>
    <message>
      <source>Apply size settings to all pages</source>
      <translation>Käytä kokoasetusta kaikkiin sivuihin</translation>
    </message>
    <message>
      <source>Apply the page size changes to all existing pages in the document</source>
      <translation>Käytä kokoasetusta asiakirjan jokaiseen sivuun</translation>
    </message>
  </context>
  <context>
    <name>RunScriptDialog</name>
    <message>
      <source>Python Scripts (*.py);; All Files (*)</source>
      <translation>Python skriptit (*.py);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Run as Extension Script</source>
      <comment>run script dialog</comment>
      <translation>Aja laajennusskriptinä</translation>
    </message>
  </context>
  <context>
    <name>SToolBAlign</name>
    <message>
      <source>Style of current paragraph</source>
      <translation>Valitun kappaleen tyyli</translation>
    </message>
    <message>
      <source>Style Settings</source>
      <translation>Tyylin asetukset</translation>
    </message>
  </context>
  <context>
    <name>SToolBColorF</name>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Color of text fill</source>
      <translation>Tekstin täytön väri</translation>
    </message>
    <message>
      <source>Saturation of color of text fill</source>
      <translation>Tekstin täytön värin kylläisyys</translation>
    </message>
    <message>
      <source>Fill Color Settings</source>
      <translation>Täyttövärin asetukset</translation>
    </message>
  </context>
  <context>
    <name>SToolBColorS</name>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Color of text stroke</source>
      <translation>Tekstin reunan väri</translation>
    </message>
    <message>
      <source>Saturation of color of text stroke</source>
      <translation>Tekstin reunan värin kylläisyys</translation>
    </message>
    <message>
      <source>Stroke Color Settings</source>
      <translation>Ääriviivan värin asetukset</translation>
    </message>
  </context>
  <context>
    <name>SToolBFont</name>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Font of selected text</source>
      <translation>Valitun tekstin kirjaintyyppi</translation>
    </message>
    <message>
      <source>Font Size</source>
      <translation>Kirjaintyypin koko</translation>
    </message>
    <message>
      <source>Scaling width of characters</source>
      <translation>Merkkien leveyssuuntainen skaalaus</translation>
    </message>
    <message>
      <source>Font Settings</source>
      <translation>Kirjaintyypin asetukset</translation>
    </message>
    <message>
      <source>Scaling height of characters</source>
      <translation>Merkkien korkeuden muutos</translation>
    </message>
  </context>
  <context>
    <name>SToolBStyle</name>
    <message>
      <source>Character Settings</source>
      <translation>Kirjaintyypin asetukset</translation>
    </message>
    <message>
      <source>Manual Tracking</source>
      <translation>Oma merkkiväli</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
  </context>
  <context>
    <name>SVGExportPlugin</name>
    <message>
      <source>Save Page as &amp;SVG...</source>
      <translation>Tallenna sivu &amp;SVG:na...</translation>
    </message>
    <message>
      <source>Exports SVG Files</source>
      <translation>Vie SVG-tiedostoja</translation>
    </message>
    <message>
      <source>Exports the current page into an SVG file.</source>
      <translation>Vie nykyinen sivu SVG-tiedostoon.</translation>
    </message>
  </context>
  <context>
    <name>SVGImportPlugin</name>
    <message>
      <source>Import &amp;SVG...</source>
      <translation>Tuo S&amp;VG...</translation>
    </message>
    <message>
      <source>Imports SVG Files</source>
      <translation>Tuo SVG-tiedostoja</translation>
    </message>
    <message>
      <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
      <translation>Tuo SVG-tiedostoja asiakirjaan muuntaen SVG:n vektorigrafiikan Scribus-objekteiksi.</translation>
    </message>
    <message>
      <source>Scalable Vector Graphics</source>
      <translation>Scalable Vector Graphics</translation>
    </message>
    <message>
      <source>SVG file contains some unsupported features</source>
      <translation>SVG-tiedosto sisältää joitain tomintoja, joita Scribus ei tue</translation>
    </message>
  </context>
  <context>
    <name>SWDialog</name>
    <message>
      <source>Short Words</source>
      <comment>short words plugin</comment>
      <translation>Short Words</translation>
    </message>
    <message>
      <source>Apply unbreakable space on:</source>
      <comment>short words plugin</comment>
      <translation>Käytä sitovaa välilyöntiä:</translation>
    </message>
    <message>
      <source>&amp;Selected frames</source>
      <comment>short words plugin</comment>
      <translation>&amp;Valitut kehykset</translation>
    </message>
    <message>
      <source>Active &amp;page</source>
      <comment>short words plugin</comment>
      <translation>Valittu &amp;sivu</translation>
    </message>
    <message>
      <source>&amp;All items</source>
      <comment>short words plugin</comment>
      <translation>&amp;Kaikki kohteet</translation>
    </message>
    <message>
      <source>Only selected frames processed.</source>
      <comment>short words plugin</comment>
      <translation>Ainoastaan valitut kehykset käyty läpi.</translation>
    </message>
    <message>
      <source>Only actual page processed.</source>
      <comment>short words plugin</comment>
      <translation>Vain valittu sivu käyty läpi.</translation>
    </message>
    <message>
      <source>All items in document processed.</source>
      <comment>short words plugin</comment>
      <translation>Kaikki asiakirjan kohteet käyty läpi.</translation>
    </message>
  </context>
  <context>
    <name>SWPrefsGui</name>
    <message>
      <source>User settings</source>
      <translation>Käyttäjän asetukset</translation>
    </message>
    <message>
      <source>System wide configuration</source>
      <translation>Järjestelmän laajuiset asetukset</translation>
    </message>
    <message>
      <source>&amp;Save</source>
      <translation>&amp;Tallenna</translation>
    </message>
    <message>
      <source>&amp;Reset</source>
      <translation>&amp;Palauta</translation>
    </message>
    <message>
      <source>Save user configuration</source>
      <translation>Tallenna käyttäjäasetukset</translation>
    </message>
    <message>
      <source>Reload system wide configuration and remove user defined one</source>
      <translation>Lataa järjestelmän laajuiset asetukset uudestaan ja poista käyttäjän määrittelemät</translation>
    </message>
    <message>
      <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
      <translation>Muokkaa omia asetuksia. Jos tallennat ne, niitä tullaan käyttämään järjestelmän laajuisten asetusten sijaan.</translation>
    </message>
    <message>
      <source>Short Words</source>
      <translation>Short Words</translation>
    </message>
    <message>
      <source>User configuration exists elready. Do you really want to overwrite it?</source>
      <translation>Käyttäjän asetukset ovat jo olemassa. Haluatko korvata ne?</translation>
    </message>
    <message>
      <source>Cannot write file %1.</source>
      <translation>Tiedostoa %1 ei voi kirjoittaa</translation>
    </message>
    <message>
      <source>User settings saved</source>
      <translation>Käyttäjäasetukset tallennettu</translation>
    </message>
    <message>
      <source>System wide configuration reloaded</source>
      <translation>Järjestelmän laajuiset asetukset ladattu uudelleen</translation>
    </message>
    <message>
      <source>Cannot open file %1</source>
      <translation>Tiedostoa %1 ei voi avata</translation>
    </message>
  </context>
  <context>
    <name>SaveAsTemplatePlugin</name>
    <message>
      <source>Save as &amp;Template...</source>
      <translation>Tall&amp;enna mallina...</translation>
    </message>
    <message>
      <source>Save a document as a template</source>
      <translation>Tallenna asiakirja malliksi</translation>
    </message>
    <message>
      <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
      <translation>Tallenna asiakirja mallina. Hyvä tapa säästää alustustoimenpiteissä asiakirjoissa, joilla on yhtenevä ulkoasu.</translation>
    </message>
  </context>
  <context>
    <name>ScPlugin</name>
    <message>
      <source>Load/Save/Import/Export</source>
      <translation>Lataa/Tallenna/Tuo/Vie</translation>
    </message>
    <message>
      <source>Persistent</source>
      <comment>plugin manager plugin type</comment>
      <translation>Pysyvä</translation>
    </message>
    <message>
      <source>Unknown</source>
      <translation>Tuntematon</translation>
    </message>
    <message>
      <source>Action</source>
      <comment>plugin manager plugin type</comment>
      <translation>Toiminta</translation>
    </message>
  </context>
  <context>
    <name>ScWinPrint</name>
    <message>
      <source>Printing...</source>
      <translation>Tulostaa...</translation>
    </message>
  </context>
  <context>
    <name>ScriXmlDoc</name>
    <message>
      <source>Copy #%1 of </source>
      <translation>Kopio #%1/</translation>
    </message>
    <message>
      <source>Background</source>
      <translation>Tausta</translation>
    </message>
  </context>
  <context>
    <name>ScribusColorList</name>
    <message>
      <source>Document Colors</source>
      <translation>Asiakirjan värit</translation>
    </message>
  </context>
  <context>
    <name>ScribusDoc</name>
    <message>
      <source>Document</source>
      <translation>Asiakirja</translation>
    </message>
    <message>
      <source>Background</source>
      <translation>Tausta</translation>
    </message>
    <message>
      <source>New Layer</source>
      <translation>Uusi taso</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>Normaali</translation>
    </message>
  </context>
  <context>
    <name>ScribusMainWindow</name>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Initializing Plugins</source>
      <translation>Alustetaan liitännäisiä</translation>
    </message>
    <message>
      <source>Initializing Keyboard Shortcuts</source>
      <translation>Alustetaan näppäinoikopolkuja</translation>
    </message>
    <message>
      <source>Reading Preferences</source>
      <translation>Luetaan asetuksia</translation>
    </message>
    <message>
      <source>Initializing Story Editor</source>
      <translation>Alustetaan tekstieditoria</translation>
    </message>
    <message>
      <source>Reading ICC Profiles</source>
      <translation>Luetaan ICC-profiileja</translation>
    </message>
    <message>
      <source>Initializing Hyphenator</source>
      <translation>Alustetaan tavutusta</translation>
    </message>
    <message>
      <source>Reading Scrapbook</source>
      <translation>Luetaan leikekirjaa</translation>
    </message>
    <message>
      <source>Setting up Shortcuts</source>
      <translation>Laaditaan oikopolkuja</translation>
    </message>
    <message>
      <source>File</source>
      <translation>Tiedosto</translation>
    </message>
    <message>
      <source>Edit</source>
      <translation>Muokkaa</translation>
    </message>
    <message>
      <source>Searching for Fonts</source>
      <translation>Etsitään kirjaintyyppejä</translation>
    </message>
    <message>
      <source>There are no fonts found on your system.</source>
      <translation>Järjestelmästä ei löytynyt kirjaintyyppejä</translation>
    </message>
    <message>
      <source>Exiting now.</source>
      <translation>Poistutaan</translation>
    </message>
    <message>
      <source>Fatal Error</source>
      <translation>Vakava virhe</translation>
    </message>
    <message>
      <source>Font System Initialized</source>
      <translation>Kirjaintyypit alustettu</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>&amp;Tiedosto</translation>
    </message>
    <message>
      <source>Open &amp;Recent</source>
      <translation>Avaa &amp;viimeaikainen</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>T&amp;uo</translation>
    </message>
    <message>
      <source>&amp;Export</source>
      <translation>&amp;Vie</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Muokkaa</translation>
    </message>
    <message>
      <source>St&amp;yle</source>
      <translation>T&amp;yyli</translation>
    </message>
    <message>
      <source>&amp;Color</source>
      <translation>&amp;Väri</translation>
    </message>
    <message>
      <source>&amp;Size</source>
      <translation>&amp;Koko</translation>
    </message>
    <message>
      <source>&amp;Shade</source>
      <translation>&amp;Sävy</translation>
    </message>
    <message>
      <source>&amp;Font</source>
      <translation>&amp;Kirjaintyyppi</translation>
    </message>
    <message>
      <source>&amp;Effects</source>
      <translation>&amp;Tehosteet</translation>
    </message>
    <message>
      <source>&amp;Item</source>
      <translation>Ko&amp;hde</translation>
    </message>
    <message>
      <source>Preview Settings</source>
      <translation>Esikatseluasetukset</translation>
    </message>
    <message>
      <source>Level</source>
      <translation>Kerros</translation>
    </message>
    <message>
      <source>Send to La&amp;yer</source>
      <translation>Vie &amp;tasoon</translation>
    </message>
    <message>
      <source>&amp;PDF Options</source>
      <translation>&amp;PDF-asetukset</translation>
    </message>
    <message>
      <source>&amp;Shape</source>
      <translation>&amp;Kuvio</translation>
    </message>
    <message>
      <source>C&amp;onvert To</source>
      <translation>&amp;Tee kohteesta</translation>
    </message>
    <message>
      <source>I&amp;nsert</source>
      <translation>&amp;Lisää</translation>
    </message>
    <message>
      <source>Character</source>
      <translation>Teksti</translation>
    </message>
    <message>
      <source>Quote</source>
      <translation>Lainaus</translation>
    </message>
    <message>
      <source>Space</source>
      <translation>Välilyönti</translation>
    </message>
    <message>
      <source>&amp;Page</source>
      <translation>&amp;Sivu</translation>
    </message>
    <message>
      <source>&amp;View</source>
      <translation>&amp;Näytä</translation>
    </message>
    <message>
      <source>E&amp;xtras</source>
      <translation>&amp;Apuvälineet</translation>
    </message>
    <message>
      <source>&amp;Windows</source>
      <translation>&amp;Ikkuna</translation>
    </message>
    <message>
      <source>&amp;Help</source>
      <translation>&amp;Ohje</translation>
    </message>
    <message>
      <source>&amp;Alignment</source>
      <translation>T&amp;asaus</translation>
    </message>
    <message>
      <source>Ready</source>
      <translation>Valmis</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Avaa</translation>
    </message>
    <message>
      <source>Importing Pages...</source>
      <translation>Tuodaan sivuja...</translation>
    </message>
    <message>
      <source>Import Page(s)</source>
      <translation>Tuo sivu</translation>
    </message>
    <message>
      <source>Import done</source>
      <translation>Tuonti tehty</translation>
    </message>
    <message>
      <source>Found nothing to import</source>
      <translation>Tuotavaa ei löytynyt</translation>
    </message>
    <message>
      <source>File %1 is not in an acceptable format</source>
      <translation>Tiedosto %1 ei ole tunnetussa muodossa</translation>
    </message>
    <message>
      <source>Loading...</source>
      <translation>Ladataan...</translation>
    </message>
    <message>
      <source>PostScript</source>
      <translation>PostScript</translation>
    </message>
    <message>
      <source>Some ICC profiles used by this document are not installed:</source>
      <translation>Joitain tämän asiakirjan käyttämiä ICC-profiileja ei ole asennettu:</translation>
    </message>
    <message>
      <source> was replaced by: </source>
      <translation>korvattiin tyypillä: </translation>
    </message>
    <message>
      <source>(converted)</source>
      <translation>(muutettu)</translation>
    </message>
    <message>
      <source>All Supported Formats</source>
      <translation>Kaikki tuetut tiedostotyypit</translation>
    </message>
    <message>
      <source>All Files (*)</source>
      <translation>Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Cannot write the file: 
%1</source>
      <translation>Tiedostoon ei voi kirjoittaa:
%1</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
      <translation>Asiakirjat (*.sla *.sla.gz *.scd *scd.gz);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Documents (*.sla *.scd);;All Files (*)</source>
      <translation>Asiakirjat (*.sla *.scd);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Save As</source>
      <translation>Tallenna nimellä</translation>
    </message>
    <message>
      <source>Saving...</source>
      <translation>Tallentaa...</translation>
    </message>
    <message>
      <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
      <translation>Scribus havaitsi joitain virheitä. Käytä tarkastustyökalua korjataksesi ne</translation>
    </message>
    <message>
      <source>&amp;Ignore</source>
      <translation>&amp;Ohita</translation>
    </message>
    <message>
      <source>&amp;Abort</source>
      <translation>&amp;Keskeytä</translation>
    </message>
    <message>
      <source>Printing...</source>
      <translation>Tulostaa...</translation>
    </message>
    <message>
      <source>Document</source>
      <translation>Asiakirja</translation>
    </message>
    <message>
      <source>Printing failed!</source>
      <translation>Tulostus epäonnistui!</translation>
    </message>
    <message>
      <source>Cannot Cut In-Use Item</source>
      <translation>Ei voi leikata käytössä olevaa kohdetta</translation>
    </message>
    <message>
      <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
      <translation>Kohde %1 on avoinna tekstieditorissa. Leikkaus peruutetaan.</translation>
    </message>
    <message>
      <source>About Qt</source>
      <translation>Tietoja Qt:stä</translation>
    </message>
    <message>
      <source>Scribus Manual</source>
      <translation>Scribus ohje</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>Tallenna nimellä</translation>
    </message>
    <message>
      <source>Text Files (*.txt);;All Files(*)</source>
      <translation>Tekstitiedostot (*.txt);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Normal</source>
      <translation>Normaali</translation>
    </message>
    <message>
      <source>Name:</source>
      <translation>Nimi:</translation>
    </message>
    <message>
      <source>Convert Page to Master Page</source>
      <translation>Muunna sivu mallisivuksi</translation>
    </message>
    <message>
      <source>New Master Page</source>
      <translation>Uusi mallisivu</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Koko:</translation>
    </message>
    <message>
      <source>Size</source>
      <translation>Koko</translation>
    </message>
    <message>
      <source>&amp;Shade:</source>
      <translation>&amp;Sävy:</translation>
    </message>
    <message>
      <source>Shade</source>
      <translation>Sävy</translation>
    </message>
    <message>
      <source>No Style</source>
      <translation>Ei tyyliä</translation>
    </message>
    <message>
      <source>The following programs are missing:</source>
      <translation>Seuraavat ohjelmat puuttuvat:</translation>
    </message>
    <message>
      <source>Ghostscript : You cannot use EPS images or Print Preview</source>
      <translation>Ghostscript: Et voi käyttää EPS-kuvia tai tulostuksen esikatselua</translation>
    </message>
    <message>
      <source>All</source>
      <translation>Kaikki</translation>
    </message>
    <message>
      <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
      <translation>Scribus havaitsi virheitä 
Käytä tarkastustyökalua korjataksesi ne.</translation>
    </message>
    <message>
      <source>EPS Files (*.eps);;All Files (*)</source>
      <translation>EPS-tiedostot (*.eps);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
      <translation>Virheitä havaittiin. 
Käytä tarkastustyökalua korjataksesi ne.</translation>
    </message>
    <message>
      <source>-Page%1</source>
      <translation>-sivu %1</translation>
    </message>
    <message>
      <source>Some objects are locked.</source>
      <translation>Jotkin kohteet ovat lukittuina.</translation>
    </message>
    <message>
      <source>&amp;Lock All</source>
      <translation>&amp;Lukitse kaikki</translation>
    </message>
    <message>
      <source>&amp;Unlock All</source>
      <translation>&amp;Vapauta kaikki</translation>
    </message>
    <message>
      <source>Information</source>
      <translation>Tiedot</translation>
    </message>
    <message>
      <source>The program %1 is already running!</source>
      <translation>Ohjelma %1 on jo käynnissä</translation>
    </message>
    <message>
      <source>The program %1 is missing!</source>
      <translation>Ohjelmaa %1 ei löydy</translation>
    </message>
    <message>
      <source>The selected color does not exist in the document's color set. Please enter a name for this new color.</source>
      <translation>Valittua väriä ei löydy asiakirjan väreistä. Valitse nimi uudelle värille.</translation>
    </message>
    <message>
      <source>Color Not Found</source>
      <translation>Väriä ei löydy</translation>
    </message>
    <message>
      <source>The name you have selected already exists. Please enter a different name for this new color.</source>
      <translation>Valitsemasi nimi on jo olemassa. Valitse uusi nimi värille.</translation>
    </message>
    <message>
      <source>&amp;Level</source>
      <translation>&amp;Kerros</translation>
    </message>
    <message>
      <source>Send to Layer</source>
      <translation>Vie tasoon</translation>
    </message>
    <message>
      <source>Previe&amp;w Settings</source>
      <translation>&amp;Esikatseluasetukset</translation>
    </message>
    <message>
      <source>&amp;Tools</source>
      <translation>Työ&amp;kalut</translation>
    </message>
    <message>
      <source>X-Pos:</source>
      <translation>X:</translation>
    </message>
    <message>
      <source>Y-Pos:</source>
      <translation>Y:</translation>
    </message>
    <message>
      <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
      <translation>Ghostscript: EPS-kuvia ja PostScript-esikatselua ei voi käyttää</translation>
    </message>
    <message>
      <source>Ghostscript is missing : Postscript Print Preview is not available</source>
      <translation>Ghostscript puuttuu: Postscript-esikatselu ei ole käytössä
</translation>
    </message>
  </context>
  <context>
    <name>ScribusQApp</name>
    <message>
      <source>Invalid argument: </source>
      <translation>Virheellinen argumentti</translation>
    </message>
    <message>
      <source>File %1 does not exist, aborting.</source>
      <translation>Tiedostoa %1 ei ole olemassa. Poistutaan.</translation>
    </message>
    <message>
      <source>Usage: scribus [option ... ] [file]</source>
      <translation>Käyttö: scribus [valitsin...] [tiedosto]</translation>
    </message>
    <message>
      <source>Options:</source>
      <translation>Asetukset:</translation>
    </message>
    <message>
      <source>Print help (this message) and exit</source>
      <translation>Tulosta ohje (tämä teksti) ja poistu</translation>
    </message>
    <message>
      <source>Uses xx as shortcut for a language, eg `en' or `de'</source>
      <translation>Käyttää merkintää xx kielen lyhenteenä esim.&quot;en&quot; tai &quot;de&quot;</translation>
    </message>
    <message>
      <source>List the currently installed interface languages</source>
      <translation>Listaa asennetut käännökset</translation>
    </message>
    <message>
      <source>Show information on the console when fonts are being loaded</source>
      <translation>Näytä tietoa kirjaintyypeistä niitä ladattaessa</translation>
    </message>
    <message>
      <source>Do not show the splashscreen on startup</source>
      <translation>Älä näytä latausikkunaa käynnistyksen yhteydessä</translation>
    </message>
    <message>
      <source>Output version information and exit</source>
      <translation>Tulosta versio ja poistu</translation>
    </message>
    <message>
      <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
      <translation>Käytä painikkeiden järjestystä oikealta vasemmalle (esim. Peruuta/Ei/Kyllä Kyllä/Ei/Peruuta sijaan)</translation>
    </message>
    <message>
      <source>filename</source>
      <translation>tiedostonimi</translation>
    </message>
    <message>
      <source>Use filename as path for user given preferences</source>
      <translation>Käytä tiedostonimeä polkuna käyttäjän asetuksille</translation>
    </message>
    <message>
      <source>Installed interface languages for Scribus are as follows:</source>
      <translation>Asennetut käyttöliittymän käännökset Scribukselle ovat:</translation>
    </message>
    <message>
      <source>To override the default language choice:</source>
      <translation>Ohita oletuskieli valinnalla:</translation>
    </message>
    <message>
      <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
      <translation>scribus -l xx tai scribus --lang xx, missä xx on haluamasi kieli.</translation>
    </message>
    <message>
      <source>Scribus Version</source>
      <translation>Scribus versio</translation>
    </message>
    <message>
      <source>Scribus, Open Source Desktop Publishing</source>
      <translation>Scribus, Open Source Desktop Publishing</translation>
    </message>
    <message>
      <source>Homepage</source>
      <translation>Kotisivu</translation>
    </message>
    <message>
      <source>Documentation</source>
      <translation>Dokumentaatio</translation>
    </message>
    <message>
      <source>Wiki</source>
      <translation>Wiki</translation>
    </message>
    <message>
      <source>Issues</source>
      <translation>Bugit ja kehitysideat</translation>
    </message>
  </context>
  <context>
    <name>ScribusView</name>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Layer</source>
      <translation>Taso</translation>
    </message>
    <message>
      <source>Copy Here</source>
      <translation>Kopioi tähän</translation>
    </message>
    <message>
      <source>Move Here</source>
      <translation>Siirrä tähän</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Peruuta</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>L&amp;iitä</translation>
    </message>
    <message>
      <source>Picture</source>
      <translation>Kuva</translation>
    </message>
    <message>
      <source>File: </source>
      <translation>Tiedosto: </translation>
    </message>
    <message>
      <source>Original PPI: </source>
      <translation>Alkuperäinen PPI:</translation>
    </message>
    <message>
      <source>Actual PPI: </source>
      <translation>Todellinen PPI:</translation>
    </message>
    <message>
      <source>Linked Text</source>
      <translation>Linkitetty teksti</translation>
    </message>
    <message>
      <source>Text Frame</source>
      <translation>Tekstikehys</translation>
    </message>
    <message>
      <source>Text on a Path</source>
      <translation>Teksti polulla</translation>
    </message>
    <message>
      <source>Paragraphs: </source>
      <translation>Kappaleet: </translation>
    </message>
    <message>
      <source>Words: </source>
      <translation>Sanat: </translation>
    </message>
    <message>
      <source>Chars: </source>
      <translation>Merkit: </translation>
    </message>
    <message>
      <source>Print: </source>
      <translation>Tulosta:</translation>
    </message>
    <message>
      <source>Enabled</source>
      <translation>Kyllä</translation>
    </message>
    <message>
      <source>Disabled</source>
      <translation>Ei</translation>
    </message>
    <message>
      <source>In&amp;fo</source>
      <translation>Tie&amp;dot</translation>
    </message>
    <message>
      <source>Preview Settings</source>
      <translation>Esikatseluasetukset</translation>
    </message>
    <message>
      <source>&amp;PDF Options</source>
      <translation>&amp;PDF-asetukset</translation>
    </message>
    <message>
      <source>Send to La&amp;yer</source>
      <translation>Vie &amp;tasoon</translation>
    </message>
    <message>
      <source>Le&amp;vel</source>
      <translation>&amp;Kerros</translation>
    </message>
    <message>
      <source>Conve&amp;rt to</source>
      <translation>Tee &amp;kohteesta</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>P&amp;oista</translation>
    </message>
    <message>
      <source>Cannot Delete In-Use Item</source>
      <translation>Käytössä olevaa kohdetta ei voi poistaa</translation>
    </message>
    <message>
      <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
      <translation>Kohde %1 on parhaillaan avoinna tekstieditorissa. Poisto-operaatio peruutetaan.</translation>
    </message>
    <message>
      <source>Colorspace: </source>
      <translation>Väriavaruus: </translation>
    </message>
    <message>
      <source>Unknown</source>
      <translation>Tuntematon</translation>
    </message>
    <message>
      <source>RGB</source>
      <translation>RGB</translation>
    </message>
    <message>
      <source>CMYK</source>
      <translation>CMYK</translation>
    </message>
    <message>
      <source>Grayscale</source>
      <translation>Harmaasävy</translation>
    </message>
    <message>
      <source>Linking Text Frames</source>
      <translation>Tekstikehysten linkitys</translation>
    </message>
    <message>
      <source>You are trying to link to a filled frame, or a frame to itself.</source>
      <translation>Yrität linkittää tekstikehykseen, jossa on testiä tai kehykseen itseensä.</translation>
    </message>
    <message>
      <source>Do you really want to clear all your text?</source>
      <translation>Haluatko poistaa kaiken tekstin?</translation>
    </message>
    <message>
      <source>Page %1 to %2</source>
      <translation>Sivut %1- %2</translation>
    </message>
    <message>
      <source>Cannot Convert In-Use Item</source>
      <translation>Käytössä olevaa kohdetta ei voi muuttaa</translation>
    </message>
    <message>
      <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
      <translation>Kohde %1 on parhaillaan avoinna tekstieditorissa. Muutosoperaatio peruutetaan.</translation>
    </message>
  </context>
  <context>
    <name>ScribusWin</name>
    <message>
      <source>has been changed since the last save.</source>
      <translation>on muuttunut tallennuksen jälkeen.</translation>
    </message>
    <message>
      <source>Document:</source>
      <translation>Asiakirja:</translation>
    </message>
    <message>
      <source>&amp;Discard</source>
      <translation>&amp;Hylkää</translation>
    </message>
  </context>
  <context>
    <name>ScriptPlugin</name>
    <message>
      <source>Embedded Python scripting support.</source>
      <translation>Python-skriptaustuki.</translation>
    </message>
    <message>
      <source>Scripter</source>
      <translation>Skripteri</translation>
    </message>
  </context>
  <context>
    <name>ScripterCore</name>
    <message>
      <source>Script error</source>
      <translation>Skriptivirhe</translation>
    </message>
    <message>
      <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;>bugs.scribus.net&lt;/a> please.</source>
      <translation>Jos ajat ohjelman mukana toimitettua skriptiä ilmoita siitä &lt;a href=&quot;http://bugs.scribus.net&quot;>bugs.scribus.net&lt;/a>. Kiitos.</translation>
    </message>
    <message>
      <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
      <translation>Tämä viesti on myös leikepöydälläsi. Voit liittää sen virheilmoitukseesi näppäinyhdistelmällä CTRL+V.</translation>
    </message>
    <message>
      <source>Examine Script</source>
      <translation>Tutki skripti</translation>
    </message>
    <message>
      <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
      <translation>Virhe yritettäessä kirjoittamaasi komentoa. Yksityiskohdat tulostettiin stderr:iin.</translation>
    </message>
    <message>
      <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
      <translation>Python-laajennuksen alustus epäonnistui. Yksityiskohdat tulostettiin stderr:iin.</translation>
    </message>
    <message>
      <source>Python Scripts (*.py);;All Files (*)</source>
      <translation>Python-skriptit (*.py);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Documentation for:</source>
      <translation>Dokumentaatio:</translation>
    </message>
    <message>
      <source>Script</source>
      <translation>Skripti</translation>
    </message>
    <message>
      <source> doesn't contain any docstring!</source>
      <translation> ei sisällä docstringiä.</translation>
    </message>
  </context>
  <context>
    <name>ScripterPrefsGui</name>
    <message>
      <source>Extensions</source>
      <translation>Laajennukset</translation>
    </message>
    <message>
      <source>Console</source>
      <translation>Konsoli</translation>
    </message>
    <message>
      <source>Scripter Preferences</source>
      <translation>Skripterin asetukset</translation>
    </message>
    <message>
      <source>Enable Extension Scripts</source>
      <translation>Käytä laajennusskriptejä</translation>
    </message>
    <message>
      <source>Startup Script:</source>
      <translation>Käynnistysskripti:</translation>
    </message>
    <message>
      <source>Errors:</source>
      <comment>syntax highlighting</comment>
      <translation>Virheet:</translation>
    </message>
    <message>
      <source>Comments:</source>
      <comment>syntax highlighting</comment>
      <translation>Kommentit:</translation>
    </message>
    <message>
      <source>Keywords:</source>
      <comment>syntax highlighting</comment>
      <translation>Avainsanat:</translation>
    </message>
    <message>
      <source>Signs:</source>
      <comment>syntax highlighting</comment>
      <translation>Merkit:</translation>
    </message>
    <message>
      <source>Numbers:</source>
      <comment>syntax highlighting</comment>
      <translation>Numerot:</translation>
    </message>
    <message>
      <source>Strings:</source>
      <comment>syntax highlighting</comment>
      <translation>Merkkijonot:</translation>
    </message>
    <message>
      <source>Base Texts:</source>
      <comment>syntax highlighting</comment>
      <translation>Perustekstit:</translation>
    </message>
    <message>
      <source>Select Color</source>
      <translation>Valitse väri</translation>
    </message>
  </context>
  <context>
    <name>SeList</name>
    <message>
      <source>Show Page Previews</source>
      <translation>Näytä sivun esikatselu</translation>
    </message>
  </context>
  <context>
    <name>SearchReplace</name>
    <message>
      <source>Search/Replace</source>
      <translation>Etsi/Korvaa</translation>
    </message>
    <message>
      <source>Search for:</source>
      <translation>Etsi:</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>Teksti</translation>
    </message>
    <message>
      <source>Paragraph Style</source>
      <translation>Kappaletyyli</translation>
    </message>
    <message>
      <source>Font</source>
      <translation>Kirjaintyyppi</translation>
    </message>
    <message>
      <source>Font Size</source>
      <translation>Kirjaintyypin koko</translation>
    </message>
    <message>
      <source>Font Effects</source>
      <translation>Kirjaintyyppien tehosteet</translation>
    </message>
    <message>
      <source>Fill Color</source>
      <translation>Täyttöväri</translation>
    </message>
    <message>
      <source>Stroke Color</source>
      <translation>Viivan väri</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Replace with:</source>
      <translation>Korvaa:</translation>
    </message>
    <message>
      <source>Search finished</source>
      <translation>Etsintä päättyi</translation>
    </message>
    <message>
      <source>Fill Shade</source>
      <translation>Täytön sävy</translation>
    </message>
    <message>
      <source>Stroke Shade</source>
      <translation>Reunan sävy</translation>
    </message>
    <message>
      <source>Left</source>
      <translation>Vasen</translation>
    </message>
    <message>
      <source>Center</source>
      <translation>Keskitetty</translation>
    </message>
    <message>
      <source>Right</source>
      <translation>Oikea</translation>
    </message>
    <message>
      <source>Block</source>
      <translation>Molemmat reunat</translation>
    </message>
    <message>
      <source>Forced</source>
      <translation>Pakotettu</translation>
    </message>
    <message>
      <source>&amp;Whole Word</source>
      <translation>K&amp;okonainen sana</translation>
    </message>
    <message>
      <source>&amp;Ignore Case</source>
      <translation>&amp;Älä huomioi kokoa</translation>
    </message>
    <message>
      <source>&amp;Search</source>
      <translation>&amp;Etsi</translation>
    </message>
    <message>
      <source>&amp;Replace</source>
      <translation>&amp;Korvaa</translation>
    </message>
    <message>
      <source>Replace &amp;All</source>
      <translation>Ko&amp;rvaa kaikki</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation>&amp;Sulje</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>&amp;Tyhjennä</translation>
    </message>
    <message>
      <source>Search finished, found %1 matches</source>
      <translation>Etsintä valmis, löytyi %1 kohtaa</translation>
    </message>
  </context>
  <context>
    <name>SelectFields</name>
    <message>
      <source>Available Fields</source>
      <translation>Mahdolliset kentät</translation>
    </message>
    <message>
      <source>Selected Fields</source>
      <translation>Valitut kentät</translation>
    </message>
    <message>
      <source>Select Fields</source>
      <translation>Valitse kentät</translation>
    </message>
    <message>
      <source>&amp;>></source>
      <translation>&amp;>></translation>
    </message>
    <message>
      <source>&amp;&lt;&lt;</source>
      <translation>&amp;&lt;&lt;</translation>
    </message>
  </context>
  <context>
    <name>ShadeButton</name>
    <message>
      <source>Other...</source>
      <translation>Muu...</translation>
    </message>
    <message>
      <source>Shade</source>
      <translation>Sävy</translation>
    </message>
    <message>
      <source>&amp;Shade:</source>
      <translation>&amp;Sävy:</translation>
    </message>
  </context>
  <context>
    <name>ShadowValues</name>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>X-Offset</source>
      <translation>X-siirtymä</translation>
    </message>
    <message>
      <source>Y-Offset</source>
      <translation>Y-siirtymä</translation>
    </message>
  </context>
  <context>
    <name>ShortWordsPlugin</name>
    <message>
      <source>Short Words</source>
      <translation>Short Words</translation>
    </message>
    <message>
      <source>Short &amp;Words...</source>
      <comment>short words plugin</comment>
      <translation>Short &amp;Words...</translation>
    </message>
  </context>
  <context>
    <name>SideBar</name>
    <message>
      <source>No Style</source>
      <translation>Ei tyyliä</translation>
    </message>
    <message>
      <source>Edit Styles...</source>
      <translation>Muokkaa tyylejä...</translation>
    </message>
  </context>
  <context>
    <name>Spalette</name>
    <message>
      <source>No Style</source>
      <translation>Ei tyyliä</translation>
    </message>
  </context>
  <context>
    <name>StilFormate</name>
    <message>
      <source>Open</source>
      <translation>Avaa</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
      <translation>Asiakirjat (*.sla *.sla.gz *.scd *.scd.gz);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Documents (*.sla *.scd);;All Files (*)</source>
      <translation>Asiakirjat (*.sla *.scd);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Edit Styles</source>
      <translation>Muokkaa tyylejä</translation>
    </message>
    <message>
      <source>New Style</source>
      <translation>Uusi tyyli</translation>
    </message>
    <message>
      <source>Copy of %1</source>
      <translation>Kopio-%1</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;Uusi</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Muokkaa</translation>
    </message>
    <message>
      <source>D&amp;uplicate</source>
      <translation>&amp;Kopioi</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>P&amp;oista</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>T&amp;uo</translation>
    </message>
  </context>
  <context>
    <name>StoryEditor</name>
    <message>
      <source>Story Editor</source>
      <translation>Tekstieditori</translation>
    </message>
    <message>
      <source>Current Paragraph:</source>
      <translation>Nykyinen kappale:</translation>
    </message>
    <message>
      <source>Chars: </source>
      <translation>Merkit: </translation>
    </message>
    <message>
      <source>Totals:</source>
      <translation>Yhteensä:</translation>
    </message>
    <message>
      <source>Paragraphs: </source>
      <translation>Kappaleet: </translation>
    </message>
    <message>
      <source>Words: </source>
      <translation>Sanat: </translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Avaa</translation>
    </message>
    <message>
      <source>Text Files (*.txt);;All Files(*)</source>
      <translation>Tekstitiedostot (*.txt);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>Tallenna nimellä</translation>
    </message>
    <message>
      <source>Do you want to save your changes?</source>
      <translation>Haluatko tallentaa muutokset?</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;Uusi</translation>
    </message>
    <message>
      <source>&amp;Reload Text from Frame</source>
      <translation>&amp;Lataa kehyksen teksti uudestaan</translation>
    </message>
    <message>
      <source>&amp;Save to File...</source>
      <translation>&amp;Tallenna tiedostoon...</translation>
    </message>
    <message>
      <source>&amp;Load from File...</source>
      <translation>&amp;Lataa tiedostosta...</translation>
    </message>
    <message>
      <source>Save &amp;Document</source>
      <translation>Tallenna &amp;asiakirja</translation>
    </message>
    <message>
      <source>&amp;Update Text Frame and Exit</source>
      <translation>&amp;Päivitä tekstikehys ja poistu</translation>
    </message>
    <message>
      <source>&amp;Exit Without Updating Text Frame</source>
      <translation>&amp;Poistu päivittämättä tekstikehystä</translation>
    </message>
    <message>
      <source>Cu&amp;t</source>
      <translation>&amp;Leikkaa</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation>&amp;Kopioi</translation>
    </message>
    <message>
      <source>&amp;Paste</source>
      <translation>L&amp;iitä</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>&amp;Tyhjennä</translation>
    </message>
    <message>
      <source>&amp;Update Text Frame</source>
      <translation>&amp;Päivitä tekstikehys</translation>
    </message>
    <message>
      <source>&amp;File</source>
      <translation>&amp;Tiedosto</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Muokkaa</translation>
    </message>
    <message>
      <source>Select &amp;All</source>
      <translation>V&amp;alitse kaikki</translation>
    </message>
    <message>
      <source>File</source>
      <translation>Tiedosto</translation>
    </message>
    <message>
      <source>Load Text from File</source>
      <translation>Lataa teksti tiedostosta</translation>
    </message>
    <message>
      <source>Save Text to File</source>
      <translation>Tallenna teksti tiedostoon</translation>
    </message>
    <message>
      <source>Update Text Frame and Exit</source>
      <translation>Päivitä tekstikehys ja poistu</translation>
    </message>
    <message>
      <source>Exit Without Updating Text Frame</source>
      <translation>Poistu tallentamatta tekstikehystä</translation>
    </message>
    <message>
      <source>Reload Text from Frame</source>
      <translation>Lataa kehyksen teksti uudestaan</translation>
    </message>
    <message>
      <source>Update Text Frame</source>
      <translation>Päivitä tekstikehys</translation>
    </message>
    <message>
      <source>&amp;Edit Styles...</source>
      <translation>&amp;Muokkaa tyylejä...</translation>
    </message>
    <message>
      <source>&amp;Search/Replace...</source>
      <translation>Et&amp;si/Korvaa...</translation>
    </message>
    <message>
      <source>&amp;Background...</source>
      <translation>&amp;Tausta...</translation>
    </message>
    <message>
      <source>&amp;Display Font...</source>
      <translation>&amp;Käytettävä kirjaintyyppi...</translation>
    </message>
    <message>
      <source>&amp;Settings</source>
      <translation>As&amp;etukset</translation>
    </message>
    <message>
      <source>Search/Replace</source>
      <translation>Etsi/Korvaa</translation>
    </message>
    <message>
      <source>&amp;Fonts Preview...</source>
      <translation>&amp;Kirjaintyyppien esikatselu...</translation>
    </message>
    <message>
      <source>&amp;Smart text selection</source>
      <translation>&amp;Älykäs tekstin valitseminen</translation>
    </message>
    <message>
      <source>&amp;Insert Glyph...</source>
      <translation>&amp;Lisää merkki...</translation>
    </message>
    <message>
      <source>Clear All Text</source>
      <translation>Poista kaikki teksti</translation>
    </message>
    <message>
      <source>Story Editor - %1</source>
      <translation>Tekstieditori - %1</translation>
    </message>
    <message>
      <source>Do you really want to lose all your changes?</source>
      <translation>Haluatko menettää kaikki tekemäsi muutokset?</translation>
    </message>
    <message>
      <source>Do you really want to clear all your text?</source>
      <translation>Haluatko poistaa kaiken tekstin?</translation>
    </message>
  </context>
  <context>
    <name>StrikeValues</name>
    <message>
      <source>Auto</source>
      <translation>Auto</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Displacement</source>
      <translation>Siirtymä</translation>
    </message>
    <message>
      <source>Linewidth</source>
      <translation>Viivan leveys</translation>
    </message>
  </context>
  <context>
    <name>StyleSelect</name>
    <message>
      <source>Underline</source>
      <translation>Alleviivattu</translation>
    </message>
    <message>
      <source>Small Caps</source>
      <translation>Kapiteelit</translation>
    </message>
    <message>
      <source>Subscript</source>
      <translation>Alaindeksi</translation>
    </message>
    <message>
      <source>Superscript</source>
      <translation>Yläindeksi</translation>
    </message>
    <message>
      <source>Strike Out</source>
      <translation>Yliviivattu</translation>
    </message>
    <message>
      <source>Underline Words Only</source>
      <translation>Alleviivaa ainoastaan sanat</translation>
    </message>
    <message>
      <source>All Caps</source>
      <translation>Kaikki versaalina</translation>
    </message>
    <message>
      <source>Outline</source>
      <translation>Rakenne</translation>
    </message>
    <message>
      <source>Shadow</source>
      <translation>Varjo</translation>
    </message>
  </context>
  <context>
    <name>SxwDialog</name>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Use document name as a prefix for paragraph styles</source>
      <translation>Käytä asiakirjan nimeä kappaletyylin edessä</translation>
    </message>
    <message>
      <source>Do not ask again</source>
      <translation>Älä kysy uudestaan</translation>
    </message>
    <message>
      <source>OpenOffice.org Writer Importer Options</source>
      <translation>OpenOffice.org Writer tuojan asetukset</translation>
    </message>
    <message>
      <source>Enabling this will overwrite existing styles in the current Scribus document</source>
      <translation>Ylikirjoita olemassa olevat kappaletyylit</translation>
    </message>
    <message>
      <source>Merge Paragraph Styles</source>
      <translation>Yhdistä kappaletyylit</translation>
    </message>
    <message>
      <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document's styles are named differently.</source>
      <translation>Yhdistää kappaletyylit tyylien asetusten perusteella. Tuloksena on vähemmän kappaletyylejä.</translation>
    </message>
    <message>
      <source>Prepend the document name to the paragraph style name in Scribus.</source>
      <translation>Käytä tuotavan asiakirjan nimeä kappaletyylin nimessä.</translation>
    </message>
    <message>
      <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
      <translation>Aseta tämänhetkiset valinnat oletusarvoiksi äläkä kysy asetuksia uudestaan tuotaessa OpenOffice.org 1.x asiakirjaa.</translation>
    </message>
    <message>
      <source>Overwrite Paragraph Styles</source>
      <translation>Korvaa kappaletyylit</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Peruuta</translation>
    </message>
  </context>
  <context>
    <name>TOCIndexPrefs</name>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>At the beginning</source>
      <translation>Alussa</translation>
    </message>
    <message>
      <source>At the end</source>
      <translation>Lopussa</translation>
    </message>
    <message>
      <source>Not Shown</source>
      <translation>Ei näytetä</translation>
    </message>
    <message>
      <source>Table of Contents and Indexes</source>
      <translation>Sisällysluettelo ja hakemistot</translation>
    </message>
    <message>
      <source>Table Of Contents</source>
      <translation>Sisällysluettelo</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation>&amp;Lisää</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>P&amp;oista</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>The frame the table of contents will be placed into</source>
      <translation>Kehys johon sisällysluettelo sijoitetaan</translation>
    </message>
    <message>
      <source>Page Numbers Placed:</source>
      <translation>Sivunumeroitten sijainti:</translation>
    </message>
    <message>
      <source>Item Attribute Name:</source>
      <translation>Kohteen muuttujan nimi:</translation>
    </message>
    <message>
      <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
      <translation>Kohteen muuttujan nimi, jota käytetään merkitsemään kehyksiä, joista sisällysluettelo muodostuu</translation>
    </message>
    <message>
      <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
      <translation>Sijoita sivunumerot alkuun, loppuun tai jätä ne pois.</translation>
    </message>
    <message>
      <source>List Non-Printing Entries</source>
      <translation>Listaa tulostumattomat kohteet</translation>
    </message>
    <message>
      <source>Include frames that are set to not print as well</source>
      <translation>Sisällytä kehykset, joita ei tulosteta</translation>
    </message>
    <message>
      <source>The paragraph style used for the entry lines</source>
      <translation>Kappaletyyli sisällysluettelolle</translation>
    </message>
    <message>
      <source>Paragraph Style:</source>
      <translation>Kappaletyyli:</translation>
    </message>
    <message>
      <source>Destination Frame:</source>
      <translation>Kohdekehys:</translation>
    </message>
    <message>
      <source>Inde&amp;x</source>
      <translation>&amp;Hakemisto</translation>
    </message>
  </context>
  <context>
    <name>TOCIndexPrefsBase</name>
    <message>
      <source>Table of Contents and Indexes</source>
      <translation>Sisällysluettelo ja hakemistot</translation>
    </message>
    <message>
      <source>Table Of Contents</source>
      <translation>Sisällysluettelo</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation>&amp;Lisää</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation>Alt+A</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>P&amp;oista</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation>Alt+D</translation>
    </message>
    <message>
      <source>The frame the table of contents will be placed into</source>
      <translation>Kehys johon sisällysluettelo sijoitetaan</translation>
    </message>
    <message>
      <source>Page Numbers Placed:</source>
      <translation>Sivunumeroitten sijainti:</translation>
    </message>
    <message>
      <source>Item Attribute Name:</source>
      <translation>Muuttujan nimi:</translation>
    </message>
    <message>
      <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
      <translation>Kohteen muuttujan nimi, jota käytetään merkitsemään kehyksiä, joista sisällysluettelo muodostuu</translation>
    </message>
    <message>
      <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
      <translation>Sijoita sivunumerot alkuun, loppuun tai jätä ne pois.</translation>
    </message>
    <message>
      <source>List Non-Printing Entries</source>
      <translation>Listaa tulostumattomat kohteet</translation>
    </message>
    <message>
      <source>Include frames that are set to not print as well</source>
      <translation>Sisällytä kehykset, joita ei tulosteta</translation>
    </message>
    <message>
      <source>The paragraph style used for the entry lines</source>
      <translation>Kappaletyyli sisällysluettelolle</translation>
    </message>
    <message>
      <source>Paragraph Style:</source>
      <translation>Kappaletyyli:</translation>
    </message>
    <message>
      <source>Destination Frame:</source>
      <translation>Kohdekehys:</translation>
    </message>
    <message>
      <source>Inde&amp;x</source>
      <translation>&amp;Hakemisto</translation>
    </message>
  </context>
  <context>
    <name>TabCheckDoc</name>
    <message>
      <source>Ignore all errors</source>
      <translation>Ohita virheet</translation>
    </message>
    <message>
      <source>Automatic check before printing or exporting</source>
      <translation>Automaattinen tarkastus ennen tulostusta tai vientiä</translation>
    </message>
    <message>
      <source>Check for missing glyphs</source>
      <translation>Tarkasta puuttuvat merkit</translation>
    </message>
    <message>
      <source>Check for objects not on a page</source>
      <translation>Tarkasta sivun ulkopuoliset kohteet</translation>
    </message>
    <message>
      <source>Check for overflow in text frames</source>
      <translation>Tarkasta tekstikehysten ylivuoto</translation>
    </message>
    <message>
      <source>Check for transparencies used</source>
      <translation>Tarkasta onko läpinäkyvyyttä käytetty</translation>
    </message>
    <message>
      <source>Check for missing images</source>
      <translation>Tarkasta puuttuvat kuvat</translation>
    </message>
    <message>
      <source>Check image resolution</source>
      <translation>Tarkasta kuvien tarkkuus</translation>
    </message>
    <message>
      <source>Lowest allowed resolution</source>
      <translation>Matalin sallittu tarkkuus</translation>
    </message>
    <message>
      <source> dpi</source>
      <translation> dpi</translation>
    </message>
    <message>
      <source>Check for placed PDF Files</source>
      <translation>Tarkasta sijoitetut PDF-tiedostot</translation>
    </message>
    <message>
      <source>Check for PDF Annotations and Fields</source>
      <translation>Tarkasta PDF-huomautukset ja kentät</translation>
    </message>
    <message>
      <source>Add Profile</source>
      <translation>Lisää profiili</translation>
    </message>
    <message>
      <source>Remove Profile</source>
      <translation>Poista profiili</translation>
    </message>
  </context>
  <context>
    <name>TabGuides</name>
    <message>
      <source>Common Settings</source>
      <translation>Yleiset asetukset</translation>
    </message>
    <message>
      <source>Placing in Documents</source>
      <translation>Asiakirjaan sijoittaminen</translation>
    </message>
    <message>
      <source>In the Background</source>
      <translation>Taustalla</translation>
    </message>
    <message>
      <source>In the Foreground</source>
      <translation>Etualalla</translation>
    </message>
    <message>
      <source>Snapping</source>
      <translation>Kiinnitys</translation>
    </message>
    <message>
      <source>Snap Distance:</source>
      <translation>Kiinnitysetäisyys</translation>
    </message>
    <message>
      <source>Grab Radius:</source>
      <translation>Tartuntasäde:</translation>
    </message>
    <message>
      <source> px</source>
      <translation> px</translation>
    </message>
    <message>
      <source>Show Guides</source>
      <translation>Näytä apuviivat</translation>
    </message>
    <message>
      <source>Color:</source>
      <translation>Väri:</translation>
    </message>
    <message>
      <source>Show Margins</source>
      <translation>Näytä kehykset</translation>
    </message>
    <message>
      <source>Show Page Grid</source>
      <translation>Näytä apuruudukko</translation>
    </message>
    <message>
      <source>Major Grid</source>
      <translation>Pääruudukko</translation>
    </message>
    <message>
      <source>Spacing:</source>
      <translation>Välit:</translation>
    </message>
    <message>
      <source>Minor Grid</source>
      <translation>Apuruudukko</translation>
    </message>
    <message>
      <source>Show Baseline Grid</source>
      <translation>Näytä peruslinja</translation>
    </message>
    <message>
      <source>Baseline Settings</source>
      <translation>Peruslinjan asetukset</translation>
    </message>
    <message>
      <source>Baseline &amp;Grid:</source>
      <translation>&amp;Peruslinja:</translation>
    </message>
    <message>
      <source>Baseline &amp;Offset:</source>
      <translation>Peruslinjan &amp;siirtymä:</translation>
    </message>
    <message>
      <source>Guides are not visible through objects on the page</source>
      <translation>Apuviivat eivät näy kohteitten alta</translation>
    </message>
    <message>
      <source>Guides are visible above all objects on the page</source>
      <translation>Apuviivat ovat kohteitten päällä näkyvissä</translation>
    </message>
    <message>
      <source>Distance between the minor grid lines</source>
      <translation>Apuruudukon viivojen välinen etäisyys</translation>
    </message>
    <message>
      <source>Distance between the major grid lines</source>
      <translation>Pääruudukon viivojen välinen etäisyys</translation>
    </message>
    <message>
      <source>Distance within which an object will snap to your placed guides</source>
      <translation>Etäisyys, jolta kohde kiinnittyy apuviivoihin</translation>
    </message>
    <message>
      <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
      <translation>Etäisyys, jonka sisällä Scribus mahdollistaa kohteeseen tarttumisen</translation>
    </message>
    <message>
      <source>Color of the minor grid lines</source>
      <translation>Apuruudukon viivojen väri</translation>
    </message>
    <message>
      <source>Color of the major grid lines</source>
      <translation>Pääruudukon viivojen väri</translation>
    </message>
    <message>
      <source>Color of the guide lines you insert</source>
      <translation>Apuviivojen väri</translation>
    </message>
    <message>
      <source>Color for the margin lines</source>
      <translation>Sivun reunusten väri</translation>
    </message>
    <message>
      <source>Color for the baseline grid</source>
      <translation>Peruslinjan väri</translation>
    </message>
    <message>
      <source>Turns the basegrid on or off</source>
      <translation>Näyttää tai piilottaa peruslinjan</translation>
    </message>
    <message>
      <source>Distance between the lines of the baseline grid</source>
      <translation>Viivojen etäisyys peruslinjassa</translation>
    </message>
    <message>
      <source>Distance from the top of the page for the first baseline</source>
      <translation>Ensimmäisen viivan etäisyys sivun yläreunasta</translation>
    </message>
    <message>
      <source>Turns the gridlines on or off</source>
      <translation>Näytä tai piilota apuruudukko</translation>
    </message>
    <message>
      <source>Turns the guides on or off</source>
      <translation>Näyttää tai piilottaa apuviivat</translation>
    </message>
    <message>
      <source>Turns the margins on or off</source>
      <translation>Näyttää tai piilottaa reunukset</translation>
    </message>
  </context>
  <context>
    <name>TabManager</name>
    <message>
      <source>Manage Tabulators</source>
      <translation>Sarkainten hallinta</translation>
    </message>
  </context>
  <context>
    <name>TabPDFOptions</name>
    <message>
      <source>Export Range</source>
      <translation>Tallennettavat sivut</translation>
    </message>
    <message>
      <source>&amp;All Pages</source>
      <translation>&amp;Kaikki sivut</translation>
    </message>
    <message>
      <source>C&amp;hoose Pages</source>
      <translation>&amp;Valitse sivut</translation>
    </message>
    <message>
      <source>&amp;Rotation:</source>
      <translation>Ki&amp;erto:</translation>
    </message>
    <message>
      <source>File Options</source>
      <translation>Tiedoston ominaisuudet</translation>
    </message>
    <message>
      <source>Compatibilit&amp;y:</source>
      <translation>&amp;Yhteensopivuus:</translation>
    </message>
    <message>
      <source>&amp;Binding:</source>
      <translation>&amp;Sidonta:</translation>
    </message>
    <message>
      <source>Left Margin</source>
      <translation>Vasen reuna</translation>
    </message>
    <message>
      <source>Right Margin</source>
      <translation>Oikea reuna</translation>
    </message>
    <message>
      <source>Generate &amp;Thumbnails</source>
      <translation>Luo &amp;esikatselukuvat</translation>
    </message>
    <message>
      <source>Save &amp;Linked Text Frames as PDF Articles</source>
      <translation>Tallenna &amp;linkitetyt tekstikehykset PDF-artikkeleina</translation>
    </message>
    <message>
      <source>&amp;Include Bookmarks</source>
      <translation>Sisällytä &amp;kirjanmerkit</translation>
    </message>
    <message>
      <source> dpi</source>
      <translation> dpi</translation>
    </message>
    <message>
      <source>&amp;Resolution for EPS Graphics:</source>
      <translation>EPS-grafiikan tarkkuus</translation>
    </message>
    <message>
      <source>Com&amp;press Text and Vector Graphics</source>
      <translation>Pakkaa teksti ja &amp;vektorigrafiikka</translation>
    </message>
    <message>
      <source>Image Settings</source>
      <translation>Kuvien asetukset</translation>
    </message>
    <message>
      <source>Automatic</source>
      <translation>Automaattinen</translation>
    </message>
    <message>
      <source>JPEG</source>
      <translation>JPEG</translation>
    </message>
    <message>
      <source>Zip</source>
      <translation>Zip</translation>
    </message>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Maximum</source>
      <translation>Paras</translation>
    </message>
    <message>
      <source>High</source>
      <translation>Korkea</translation>
    </message>
    <message>
      <source>Medium</source>
      <translation>Keskitaso</translation>
    </message>
    <message>
      <source>Low</source>
      <translation>Matala</translation>
    </message>
    <message>
      <source>Minimum</source>
      <translation>Heikoin</translation>
    </message>
    <message>
      <source>&amp;General</source>
      <translation>&amp;Yleiset</translation>
    </message>
    <message>
      <source>&amp;Embed all Fonts</source>
      <translation>&amp;Upota kaikki kirjaintyypit</translation>
    </message>
    <message>
      <source>&amp;Subset all Fonts</source>
      <translation>Tee osa&amp;joukko kirjaintyypeistä</translation>
    </message>
    <message>
      <source>Embedding</source>
      <translation>Upottaminen</translation>
    </message>
    <message>
      <source>Available Fonts:</source>
      <translation>Käytetyt kirjaintyypit:</translation>
    </message>
    <message>
      <source>&amp;>></source>
      <translation>&amp;>></translation>
    </message>
    <message>
      <source>&amp;&lt;&lt;</source>
      <translation>&amp;&lt;&lt;</translation>
    </message>
    <message>
      <source>Fonts to embed:</source>
      <translation>Upotettavat kirjaintyypit:</translation>
    </message>
    <message>
      <source>Fonts to subset:</source>
      <translation>Kirjaintyypit osajoukoiksi:</translation>
    </message>
    <message>
      <source>&amp;Fonts</source>
      <translation>K&amp;irjaintyypit</translation>
    </message>
    <message>
      <source>Enable &amp;Presentation Effects</source>
      <translation>Käytä &amp;esitysefektejä</translation>
    </message>
    <message>
      <source>Page</source>
      <translation>Sivu</translation>
    </message>
    <message>
      <source>Show Page Pre&amp;views</source>
      <translation>Näytä &amp;esikatselu sivuista</translation>
    </message>
    <message>
      <source>Effects</source>
      <translation>Efektit</translation>
    </message>
    <message>
      <source>&amp;Display Duration:</source>
      <translation>&amp;Näytön kesto:</translation>
    </message>
    <message>
      <source>Effec&amp;t Duration:</source>
      <translation>&amp;Efektin kesto:</translation>
    </message>
    <message>
      <source>Effect T&amp;ype:</source>
      <translation>Efektin &amp;tyyppi:</translation>
    </message>
    <message>
      <source>&amp;Moving Lines:</source>
      <translation>&amp;Suunta:</translation>
    </message>
    <message>
      <source>F&amp;rom the:</source>
      <translation>&amp;Alkaen:</translation>
    </message>
    <message>
      <source>D&amp;irection:</source>
      <translation>&amp;Suunta:</translation>
    </message>
    <message>
      <source> sec</source>
      <translation> sek</translation>
    </message>
    <message>
      <source>No Effect</source>
      <translation>Ei efektiä</translation>
    </message>
    <message>
      <source>Blinds</source>
      <translation>Kaihtimet</translation>
    </message>
    <message>
      <source>Box</source>
      <translation>Laatikko</translation>
    </message>
    <message>
      <source>Dissolve</source>
      <translation>Häivytä</translation>
    </message>
    <message>
      <source>Glitter</source>
      <translation>Kimallus</translation>
    </message>
    <message>
      <source>Split</source>
      <translation>Jaa</translation>
    </message>
    <message>
      <source>Wipe</source>
      <translation>Pyyhi</translation>
    </message>
    <message>
      <source>Horizontal</source>
      <translation>Vaakasuora</translation>
    </message>
    <message>
      <source>Vertical</source>
      <translation>Pystysuora</translation>
    </message>
    <message>
      <source>Inside</source>
      <translation>Sisäpuoli</translation>
    </message>
    <message>
      <source>Outside</source>
      <translation>Ulkopuoli</translation>
    </message>
    <message>
      <source>Left to Right</source>
      <translation>Vasemmalta oikealle</translation>
    </message>
    <message>
      <source>Top to Bottom</source>
      <translation>Ylhäältä alas</translation>
    </message>
    <message>
      <source>Bottom to Top</source>
      <translation>Alhaalta ylös</translation>
    </message>
    <message>
      <source>Right to Left</source>
      <translation>Oikealta vasemmalle</translation>
    </message>
    <message>
      <source>Top-left to Bottom-Right</source>
      <translation>Ylävasemmalta alaoikealle</translation>
    </message>
    <message>
      <source>&amp;Apply Effect on all Pages</source>
      <translation>Kä&amp;ytä efektiä kaikkiin sivuihin</translation>
    </message>
    <message>
      <source>E&amp;xtras</source>
      <translation>&amp;Apuvälineet</translation>
    </message>
    <message>
      <source>&amp;Use Encryption</source>
      <translation>&amp;Käytä salausta</translation>
    </message>
    <message>
      <source>Passwords</source>
      <translation>Salasanat</translation>
    </message>
    <message>
      <source>&amp;User:</source>
      <translation>K&amp;äyttäjä:</translation>
    </message>
    <message>
      <source>&amp;Owner:</source>
      <translation>&amp;Omistaja:</translation>
    </message>
    <message>
      <source>Settings</source>
      <translation>Asetukset</translation>
    </message>
    <message>
      <source>Allow &amp;Printing the Document</source>
      <translation>Salli asiakirjan &amp;tulostus</translation>
    </message>
    <message>
      <source>Allow &amp;Changing the Document</source>
      <translation>Salli &amp;asiakirjan muokkaus</translation>
    </message>
    <message>
      <source>Allow Cop&amp;ying Text and Graphics</source>
      <translation>Salli tekstin ja kuvien &amp;kopiointi</translation>
    </message>
    <message>
      <source>Allow Adding &amp;Annotations and Fields</source>
      <translation>Salli &amp;kenttien lisääminen</translation>
    </message>
    <message>
      <source>S&amp;ecurity</source>
      <translation>&amp;Turvallisuus</translation>
    </message>
    <message>
      <source>General</source>
      <translation>Yleiset</translation>
    </message>
    <message>
      <source>Output &amp;Intended For:</source>
      <translation>&amp;Tuloste on tarkoitettu:</translation>
    </message>
    <message>
      <source>Screen / Web</source>
      <translation>Näytölle/Verkkoon</translation>
    </message>
    <message>
      <source>Printer</source>
      <translation>Painoon</translation>
    </message>
    <message>
      <source>Grayscale</source>
      <translation>Harmaasävy</translation>
    </message>
    <message>
      <source>&amp;Use Custom Rendering Settings</source>
      <translation>Käytä &amp;omia sovitusasetuksia</translation>
    </message>
    <message>
      <source>Rendering Settings</source>
      <translation>Sovitusasetukset</translation>
    </message>
    <message>
      <source>Fre&amp;quency:</source>
      <translation>&amp;Taajuus:</translation>
    </message>
    <message>
      <source>&amp;Angle:</source>
      <translation>K&amp;ulma:</translation>
    </message>
    <message>
      <source>S&amp;pot Function:</source>
      <translation>&amp;Spot-funktio:</translation>
    </message>
    <message>
      <source>Simple Dot</source>
      <translation>Yksinkertainen piste</translation>
    </message>
    <message>
      <source>Line</source>
      <translation>Viiva</translation>
    </message>
    <message>
      <source>Round</source>
      <translation>Ympyrä</translation>
    </message>
    <message>
      <source>Ellipse</source>
      <translation>Ellipsi</translation>
    </message>
    <message>
      <source>Solid Colors:</source>
      <translation>Tasaiset värit:</translation>
    </message>
    <message>
      <source>Use ICC Profile</source>
      <translation>Käytä ICC-profiilia</translation>
    </message>
    <message>
      <source>Profile:</source>
      <translation>Profiili:</translation>
    </message>
    <message>
      <source>Rendering-Intent:</source>
      <translation>Sovitustapa:</translation>
    </message>
    <message>
      <source>Perceptual</source>
      <translation>Perceptual</translation>
    </message>
    <message>
      <source>Relative Colorimetric</source>
      <translation>Relative Colorimetric</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>Kylläisyys</translation>
    </message>
    <message>
      <source>Absolute Colorimetric</source>
      <translation>Absolute Colorimetric</translation>
    </message>
    <message>
      <source>Images:</source>
      <translation>Kuvat:</translation>
    </message>
    <message>
      <source>Don't use embedded ICC profiles</source>
      <translation>Älä käytä upotettuja ICC-profiileja</translation>
    </message>
    <message>
      <source>C&amp;olor</source>
      <translation>&amp;Väri</translation>
    </message>
    <message>
      <source>PDF/X-3 Output Intent</source>
      <translation>PDF/X-3 tuloste</translation>
    </message>
    <message>
      <source>&amp;Info String:</source>
      <translation>&amp;Infoteksti:</translation>
    </message>
    <message>
      <source>Output &amp;Profile:</source>
      <translation>&amp;Vientiprofiili:</translation>
    </message>
    <message>
      <source>Trim Box</source>
      <translation>Trim Box</translation>
    </message>
    <message>
      <source>PDF/X-&amp;3</source>
      <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
      <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</source>
      <translation>Mahdollistaa esitysefektien käytön käytettäessä Adobe&amp;#174; Readeria&amp;#174; koko näytön tilassa.</translation>
    </message>
    <message>
      <source>Show page previews of each page listed above.</source>
      <translation>Näytä sivujen esikatselu sivuista jotka on listattu yllä.</translation>
    </message>
    <message>
      <source>Length of time the page is shown before the presentation starts on the selected page.</source>
      <translation>Aika, joka odotetaan ennen esityksen alkua valitulla sivulla.</translation>
    </message>
    <message>
      <source>Type of the display effect.</source>
      <translation>Näytettävän efektin tyyppi.</translation>
    </message>
    <message>
      <source>Direction of the effect of moving lines for the split and blind effects.</source>
      <translation>Liikkuvien viivojen suunta jako- ja kaihdinefekteille.</translation>
    </message>
    <message>
      <source>Starting position for the box and split effects.</source>
      <translation>Alkupiste laatikko- ja jakoefekteille.</translation>
    </message>
    <message>
      <source>Direction of the glitter or wipe effects.</source>
      <translation>Suunta kimallus- ja pyyhkimisefekteille.</translation>
    </message>
    <message>
      <source>Apply the selected effect to all pages.</source>
      <translation>Käytä valittua esitysefektiä kaikkiin sivuihin.</translation>
    </message>
    <message>
      <source>Export all pages to PDF</source>
      <translation>Vie kaikki sivut PDF:ään</translation>
    </message>
    <message>
      <source>Export a range of pages to PDF</source>
      <translation>Vie joukko sivuja PDF:ään</translation>
    </message>
    <message>
      <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
      <translation>Tekee PDF-artikkeleita, jotka ovat hyödyllisiä navigoidessa linkitettyjen artikkelien välillä PDF:ssä.</translation>
    </message>
    <message>
      <source>DPI (Dots Per Inch) for image export.</source>
      <translation>DPI (pistettä tuumalle) kuvien vientiä varten.</translation>
    </message>
    <message>
      <source>Choose a password for users to be able to read your PDF.</source>
      <translation>Valitse salasana käyttäjille PDF-dokumentin lukua varten.</translation>
    </message>
    <message>
      <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
      <translation>Salli PDF-dokumentin tulostaminen. Jos kohta ei ole valittuna tulostaminen on estetty.</translation>
    </message>
    <message>
      <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
      <translation>Salli PDF-dokumentin muokkaus. Jos ei ole valittuna, muokkaus on estetty.</translation>
    </message>
    <message>
      <source>Embed a color profile for solid colors</source>
      <translation>Upota väriprofiili tasaisille väreille</translation>
    </message>
    <message>
      <source>Color profile for solid colors</source>
      <translation>Väriprofiili tasaisille väreille</translation>
    </message>
    <message>
      <source>Rendering intent for solid colors</source>
      <translation>Sovitustapa tasaisille väreille</translation>
    </message>
    <message>
      <source>Embed a color profile for images</source>
      <translation>Upota väriprofiili kuville</translation>
    </message>
    <message>
      <source>Do not use color profiles that are embedded in source images</source>
      <translation>Älä käytä väriprofiileja, jotka on upotettu kuviin</translation>
    </message>
    <message>
      <source>Color profile for images</source>
      <translation>Väriprofiili kuville</translation>
    </message>
    <message>
      <source>Rendering intent for images</source>
      <translation>Sovitustapa kuville</translation>
    </message>
    <message>
      <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
      <translation>Profiili tulosteelle. Jos mahdollista pyydä ohjeita painolta profiilin valinnassa.</translation>
    </message>
    <message>
      <source>Distance for bleed from the top of the physical page</source>
      <translation>Etäisyys leikkuuvaralle sivun todellisesta yläreunasta</translation>
    </message>
    <message>
      <source>Distance for bleed from the bottom of the physical page</source>
      <translation>Etäisyys leikkuuvaralle sivun todellista alareunasta</translation>
    </message>
    <message>
      <source>Distance for bleed from the left of the physical page</source>
      <translation>Etäisyys leikkuuvaralle sivun todellisesta vasemmasta reunasta</translation>
    </message>
    <message>
      <source>Distance for bleed from the right of the physical page</source>
      <translation>Etäisyys leikkuuvaralle sivun todellisesta oikeasta reunasta</translation>
    </message>
    <message>
      <source>Mirror Page(s) horizontally</source>
      <translation>Peilaa sivut vaakasuorasti</translation>
    </message>
    <message>
      <source>Mirror Page(s) vertically</source>
      <translation>Peilaa sivut pystysuorasti</translation>
    </message>
    <message>
      <source>Convert Spot Colors to Process Colors</source>
      <translation>Muunna spottivärit prosessiväreiksi</translation>
    </message>
    <message>
      <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
      <translation>Määrittelee PDF-yhteensopivuuden. Oletusarvo on PDF 1.3, joka antaa laajimman yhteensopivuuden. Valitse PDF 1.4, jos tiedostossasi on PDF 1.4:n ominaisuuksia kuten läpinäkyvyyttä tai tarvitset 128 bittistä salausta. PDF/X-3 mahdollistaa RGB-värienhallinnan sisällyttämisen PDF-tiedostoon. Se on valittavana, jos värienhallinta on käytössä. PDF/X-3 muuttaa kaikki CMYK-kuvat ICC-pohjaisiin RGB-väreihin. PDF/X-3:a tulee käyttää ainoastaan, jos paino on sitä pyytänyt tai joissain tapauksissa, kun tulostetaan nelivärilasertulostimilla.</translation>
    </message>
    <message>
      <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level.</source>
      <translation>Pakkaustasot häviöllisille pakkausmenetelmille: Heikoin (25 %), Matala (50 %), Keskitaso (75 %), Korkea (85 %), Paras (95 %). Huomioi, että taso ei suoraan vaikuta tuloksena saatavan kuvan kokoon. Kuvan koko ja laadun heikkeneminen ovat tapauskohtaisia jokaisella pakkaustasolla.</translation>
    </message>
    <message>
      <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
      <translation>Mahdollistaa spottivärien muuntamisen prosessiväreiksi. Jos et aio painaa käyttäen spottivärejä, tämä on paras jättää valituksi.</translation>
    </message>
    <message>
      <source>Compression &amp;Quality:</source>
      <translation>Pakkaus&amp;laatu:</translation>
    </message>
    <message>
      <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
      <translation>Pakkausmenetelmä kuville. Automaattinen antaa Scribuksen valita parhaan mahdollisimman menetelmän. ZIP on häviötön ja hyvä pakkaus kuville, joissa on tasaisia värejä. JPEG on hyvä keino luoda pienempikokoisia PDF-tiedostoja, jos asiakirja sisältää paljon kuvia (kuvien laatu saattaa kärsiä hieman). Jos erityistä tarvetta jollekin pakkaustyypille ei ole, käytä Automaattista.</translation>
    </message>
    <message>
      <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
      <translation>Salli tekstin ja grafiikan kopiointi PDF-asiakirjasta. Jos kohta ei ole valittuna, kopiointi on estetty.</translation>
    </message>
    <message>
      <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
      <translation>Salli kenttien ja huomautusten lisääminen PDF-tiedostoon. Jos kohtaa ei valita, kenttien ja huomautusten muokkaus on estetty.</translation>
    </message>
    <message>
      <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
      <translation>Upota kirjaintyypit PDF-tiedostoon. Kirjaintyyppien upotus säilyttää asiakirjan ulkoasun.</translation>
    </message>
    <message>
      <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
      <translation>Efektin kesto. Lyhyempi aika nopeuttaa efektiä ja pidempi hidastaa sitä.</translation>
    </message>
    <message>
      <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
      <translation>Anna pilkuin eroteltu lista, jossa alkio voi olla * (kaikki sivut), 1-5 (sivuväli) tai yksittäinen sivunumero.</translation>
    </message>
    <message>
      <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
      <translation>Määrittele PDF-asiakirjan sidonta. Jos olet epävarma jätä oletusarvo &quot;Vasen&quot;.</translation>
    </message>
    <message>
      <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
      <translation>Luo esikatselukuvat sivuista PDF-asiakirjaan. Jotkut katseluohjelmat käyttävät kuvia navigointiin.</translation>
    </message>
    <message>
      <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is choosen.</source>
      <translation>Asiakirjan tasot viedään PDF-asiakirjaan. Mahdollista ainoastaan PDF versiossa 1.5.</translation>
    </message>
    <message>
      <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
      <translation>Upota luomasi kirjanmerkit asiakirjaan. Helpottaa navigointia pitkissä PDF-asiakirjoissa</translation>
    </message>
    <message>
      <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
      <translation>Tekstin ja vektorigrafiikan resoluutio. Tämä ei vaikuta bittikarttakuvien resoluutioon.</translation>
    </message>
    <message>
      <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
      <translation>Häviötön pakkaus tekstille ja grafiikalle. Ilman pätevää syytä tämä tulisi jättää valituksi. Pienentää tuotettavan PDF-tiedoston kokoa.</translation>
    </message>
    <message>
      <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. This can increase memory usage and slow down export.</source>
      <translation>Aseta kuvien resoluutio haluttuun DPI-arvoon. Jos kohta ei ole valittuna, kuvien resoluutio säilytetään niiden alkuperäisessä arvossaan. Valittuna tämä kohta hidastaa PDF-vientiä ja kasvattaa muistin käyttöä.</translation>
    </message>
    <message>
      <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
      <translation>Salli PDF:n turvallisuusominaisuudet. PDF 1.3 suojataan 40 bit salauksella, PDF 1.4 128 bit salauksella. Huomautus: PDF-salaus ei ole yhtä luotettava kuin GPG- tai PGP-salaus ja siinä on joitain rajoituksia.</translation>
    </message>
    <message>
      <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
      <translation>Valitse päätason salasana, jonka avulla voidaan poistaa tai asettaa kaikki PDF-tiedoston turvallisuusominaisuudet.</translation>
    </message>
    <message>
      <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer.</source>
      <translation>Värimalli PDF-asiakirjalle. Valitse &quot;Näyttölle/Verkkoon&quot;, jos PDF-tiedostoasi on tarkoitus katsoa monitorilta tai se on tarkoitus tulostaa käyttäen tavallista mustesuihkutulostinta. Valitse &quot;Painoon&quot;, jos asiakirjasi on tarkoitus tulostaa käyttäen oikeaa neliväri-CMYK-tulostinta.</translation>
    </message>
    <message>
      <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
      <translation>Tätä asetusta tulisi käyttää ainoastaan, jos paino vaatii sitä ja he ovat antaneet sinulle tarkat arvot asetuksille. Muuten PDF-tiedosto ei välttämättä tulostu oikein eikä ole yhdenmukainen kaikilla järjestelmillä.</translation>
    </message>
    <message>
      <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
      <translation>Pakollinen merkkijono PDF/X-3 -tiedostolle. Ilman tätä PDF-tiedosto ei ole PDF/X-3 -yhteensopiva. Suosituksemme on käyttää asiakirjan otsikkoa.</translation>
    </message>
    <message>
      <source>Include La&amp;yers</source>
      <translation>Sisällytä tasot</translation>
    </message>
    <message>
      <source>Compression Metho&amp;d:</source>
      <translation>P&amp;akkausmenetelmä:</translation>
    </message>
    <message>
      <source>Resa&amp;mple Images to:</source>
      <translation>Muuta kuvien tarkkuus arvoon:</translation>
    </message>
    <message>
      <source>Display Settings</source>
      <translation>Näytön asetukset</translation>
    </message>
    <message>
      <source>Page Layout</source>
      <translation>Sivun asettelu</translation>
    </message>
    <message>
      <source>Single Page</source>
      <translation>Yksittäinen sivu</translation>
    </message>
    <message>
      <source>Continuous</source>
      <translation>Jatkuva</translation>
    </message>
    <message>
      <source>Double Page Left</source>
      <translation>Aukeama, vasen</translation>
    </message>
    <message>
      <source>Double Page Right</source>
      <translation>Aukeama, oikea</translation>
    </message>
    <message>
      <source>Visual Appearance</source>
      <translation>Ulkonäkö</translation>
    </message>
    <message>
      <source>Use Viewers Defaults</source>
      <translation>Oletusarvot</translation>
    </message>
    <message>
      <source>Use Full Screen Mode</source>
      <translation>Kokonäytön tila</translation>
    </message>
    <message>
      <source>Display Bookmarks Tab</source>
      <translation>Kirjanmerkkipalkki</translation>
    </message>
    <message>
      <source>Display Thumbnails</source>
      <translation>Esikatselukuvat</translation>
    </message>
    <message>
      <source>Display Layers Tab</source>
      <translation>Näytä tasovalikko</translation>
    </message>
    <message>
      <source>Hide Viewers Toolbar</source>
      <translation>Piilota työkalurivi</translation>
    </message>
    <message>
      <source>Hide Viewers Menubar</source>
      <translation>Piilota valikkorivi</translation>
    </message>
    <message>
      <source>Zoom Pages to fit Viewer Window</source>
      <translation>Sovita sivut katseluohjelman ikkunan kokoon</translation>
    </message>
    <message>
      <source>Special Actions</source>
      <translation>Lisätoiminnot</translation>
    </message>
    <message>
      <source>Javascript to be executed
when Document is opened:</source>
      <translation>Asiakirjan avauksen jälkeen 
suoritettava Javascript:</translation>
    </message>
    <message>
      <source>No Script</source>
      <translation>Ei skriptiä</translation>
    </message>
    <message>
      <source>Viewer</source>
      <translation>Katseluohjelma
</translation>
    </message>
  </context>
  <context>
    <name>TabTools</name>
    <message>
      <source>Font:</source>
      <translation>Kirjaintyyppi:</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>Size:</source>
      <translation>Koko:</translation>
    </message>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Fill Color:</source>
      <translation>Täyttöväri:</translation>
    </message>
    <message>
      <source>Stroke Color:</source>
      <translation>Viivan väri:</translation>
    </message>
    <message>
      <source>Tab Fill Character:</source>
      <translation>Sarkainmerkki:</translation>
    </message>
    <message>
      <source>Tab Width:</source>
      <translation>Sarkainleveys:</translation>
    </message>
    <message>
      <source>Colu&amp;mns:</source>
      <translation>&amp;Palstat:</translation>
    </message>
    <message>
      <source>&amp;Gap:</source>
      <translation>&amp;Väli:</translation>
    </message>
    <message>
      <source>Woven silk pyjamas exchanged for blue quartz</source>
      <translation>Törkylempijä vongahdus on suomenkielinen pangram</translation>
    </message>
    <message>
      <source>&amp;Line Color:</source>
      <translation>V&amp;iivan väri:</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>&amp;Shading:</source>
      <translation>&amp;Sävy:</translation>
    </message>
    <message>
      <source>&amp;Fill Color:</source>
      <translation>&amp;Täyttöväri:</translation>
    </message>
    <message>
      <source>S&amp;hading:</source>
      <translation>S&amp;ävy:</translation>
    </message>
    <message>
      <source>Line Style:</source>
      <translation>Viivan tyyli:</translation>
    </message>
    <message>
      <source>Line &amp;Width:</source>
      <translation>Viivan &amp;leveys:</translation>
    </message>
    <message>
      <source>Line S&amp;tyle:</source>
      <translation>&amp;Viivan tyyli:</translation>
    </message>
    <message>
      <source>Arrows:</source>
      <translation>Nuolet:</translation>
    </message>
    <message>
      <source>Start:</source>
      <translation>Alku:</translation>
    </message>
    <message>
      <source>End:</source>
      <translation>Loppu:</translation>
    </message>
    <message>
      <source>&amp;Free Scaling</source>
      <translation>&amp;Ei skaalausta</translation>
    </message>
    <message>
      <source>&amp;Horizontal Scaling:</source>
      <translation>&amp;Vaakasuora skaalaus:</translation>
    </message>
    <message>
      <source>&amp;Vertical Scaling:</source>
      <translation>&amp;Pystysuora skaalaus:</translation>
    </message>
    <message>
      <source>&amp;Scale Picture to Frame Size</source>
      <translation>&amp;Sovita kuva kehyksen kokoon</translation>
    </message>
    <message>
      <source>Keep Aspect &amp;Ratio</source>
      <translation>S&amp;äilytä sivujen suhteet</translation>
    </message>
    <message>
      <source>F&amp;ill Color:</source>
      <translation>&amp;Täyttöväri:</translation>
    </message>
    <message>
      <source>Use embedded Clipping Path</source>
      <translation>Käytä upotettuja syväyspolkuja</translation>
    </message>
    <message>
      <source>On Screen Preview</source>
      <translation>Esikatselu näytöllä</translation>
    </message>
    <message>
      <source>Full Resolution Preview</source>
      <translation>Täysi tarkkuus</translation>
    </message>
    <message>
      <source>Normal Resolution Preview</source>
      <translation>Normaali tarkkuus</translation>
    </message>
    <message>
      <source>Low Resolution Preview</source>
      <translation>Alhainen tarkkuus</translation>
    </message>
    <message>
      <source>Mi&amp;nimum:</source>
      <translation>&amp;Pienin:</translation>
    </message>
    <message>
      <source>Ma&amp;ximum:</source>
      <translation>&amp;Suurin:</translation>
    </message>
    <message>
      <source>&amp;Stepping:</source>
      <translation>&amp;Askellus:</translation>
    </message>
    <message>
      <source>Text Frame Properties</source>
      <translation>Tekstikehyksen asetukset</translation>
    </message>
    <message>
      <source>Picture Frame Properties</source>
      <translation>Kuvakehyksen asetukset</translation>
    </message>
    <message>
      <source>Shape Drawing Properties</source>
      <translation>Kuvioiden asetukset</translation>
    </message>
    <message>
      <source>Magnification Level Defaults</source>
      <translation>Suurennostason Oletusarvot</translation>
    </message>
    <message>
      <source>Line Drawing Properties</source>
      <translation>Viivan ominaisuudet</translation>
    </message>
    <message>
      <source>Polygon Drawing Properties</source>
      <translation>Monikulmion ominaisuudet</translation>
    </message>
    <message>
      <source>Font for new text frames</source>
      <translation>Kirjaintyyppi uuteen tekstikehykseen</translation>
    </message>
    <message>
      <source>Size of font for new text frames</source>
      <translation>Kirjaintyypin koko uuteen tekstikehykseen</translation>
    </message>
    <message>
      <source>Color of font</source>
      <translation>Kirjaintyypin väri</translation>
    </message>
    <message>
      <source>Number of columns in a text frame</source>
      <translation>Tekstikehyksen palstojen lukumäärä</translation>
    </message>
    <message>
      <source>Gap between text frame columns</source>
      <translation>Kehyksen palstojen väli</translation>
    </message>
    <message>
      <source>Sample of your font</source>
      <translation>Esimerkki valitusta kirjaintyypistä</translation>
    </message>
    <message>
      <source>Picture frames allow pictures to scale to any size</source>
      <translation>Kuvakehykset sallivat kuvan skaalauksen mihin tahansa kokoon</translation>
    </message>
    <message>
      <source>Horizontal scaling of images</source>
      <translation>Kuvien vaakasuora skaalaus</translation>
    </message>
    <message>
      <source>Vertical scaling of images</source>
      <translation>Kuvien pystysuora skaalaus</translation>
    </message>
    <message>
      <source>Keep horizontal and vertical scaling the same</source>
      <translation>Säilytä sivujen suhde samana</translation>
    </message>
    <message>
      <source>Pictures in picture frames are scaled to the size of the frame</source>
      <translation>Kuvat kuvakehyksissä skaalataan kehyksen kokoon</translation>
    </message>
    <message>
      <source>Automatically scaled pictures keep their original proportions</source>
      <translation>Automaattisesti skaalatut kuvat säilyttävät sivujen väliset suhteensa</translation>
    </message>
    <message>
      <source>Fill color of picture frames</source>
      <translation>Kuvakehyksen täyttöväri</translation>
    </message>
    <message>
      <source>Saturation of color of fill</source>
      <translation>Täyttövärin kylläisyys</translation>
    </message>
    <message>
      <source>Line color of shapes</source>
      <translation>Kuvion viivan väri</translation>
    </message>
    <message>
      <source>Saturation of color of lines</source>
      <translation>Kuvion viivan värin kylläisyys</translation>
    </message>
    <message>
      <source>Fill color of shapes</source>
      <translation>Kuvion täyttöväri</translation>
    </message>
    <message>
      <source>Line style of shapes</source>
      <translation>Kuvion viivatyyli</translation>
    </message>
    <message>
      <source>Line width of shapes</source>
      <translation>Kuvion viivan paksuus</translation>
    </message>
    <message>
      <source>Minimum magnification allowed</source>
      <translation>Pienin sallittu pienennös</translation>
    </message>
    <message>
      <source>Maximum magnification allowed</source>
      <translation>Suurin sallittu suurennus</translation>
    </message>
    <message>
      <source>Change in magnification for each zoom operation</source>
      <translation>Suurennoksen muutos jokaisella suurennoskerralla</translation>
    </message>
    <message>
      <source>Color of lines</source>
      <translation>Viivan väri</translation>
    </message>
    <message>
      <source>Saturation of color</source>
      <translation>Värin kylläisyys</translation>
    </message>
    <message>
      <source>Style of lines</source>
      <translation>Viivan tyyli</translation>
    </message>
    <message>
      <source>Width of lines</source>
      <translation>Viivan paksuus</translation>
    </message>
    <message>
      <source>Custom:</source>
      <translation>Oma:</translation>
    </message>
    <message>
      <source>Custom: </source>
      <translation>Oma: </translation>
    </message>
    <message>
      <source>Text Color:</source>
      <translation>Tekstin väri:</translation>
    </message>
    <message>
      <source>Shading:</source>
      <translation>Sävy:</translation>
    </message>
    <message>
      <source>Text Stroke:</source>
      <translation>Tekstin reunaviiva:</translation>
    </message>
    <message>
      <source>Dot</source>
      <translation>Piste</translation>
    </message>
    <message>
      <source>Hyphen</source>
      <translation>Yhdysmerkki</translation>
    </message>
    <message>
      <source>Underscore</source>
      <translation>Alaviiva</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>Oma</translation>
    </message>
  </context>
  <context>
    <name>TabTypograpy</name>
    <message>
      <source>Subscript</source>
      <translation>Alaindeksi</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>&amp;Displacement:</source>
      <translation>&amp;Siirtymä:</translation>
    </message>
    <message>
      <source>&amp;Scaling:</source>
      <translation>&amp;Skaalaus:</translation>
    </message>
    <message>
      <source>Superscript</source>
      <translation>Yläindeksi</translation>
    </message>
    <message>
      <source>D&amp;isplacement:</source>
      <translation>S&amp;iirtymä:</translation>
    </message>
    <message>
      <source>S&amp;caling:</source>
      <translation>S&amp;kaalaus:</translation>
    </message>
    <message>
      <source>Underline</source>
      <translation>Alleviivattu</translation>
    </message>
    <message>
      <source>Displacement:</source>
      <translation>Siirtymä:</translation>
    </message>
    <message>
      <source>Auto</source>
      <translation>Auto</translation>
    </message>
    <message>
      <source>Line Width:</source>
      <translation>Viivan leveys:</translation>
    </message>
    <message>
      <source>Strikethru</source>
      <translation>Yliviivattu</translation>
    </message>
    <message>
      <source>Small Caps</source>
      <translation>Kapiteelit</translation>
    </message>
    <message>
      <source>Sc&amp;aling:</source>
      <translation>Skaa&amp;laus:</translation>
    </message>
    <message>
      <source>Automatic &amp;Line Spacing</source>
      <translation>&amp;Automaattinen riviväli</translation>
    </message>
    <message>
      <source>Line Spacing:</source>
      <translation>Riviväli:</translation>
    </message>
    <message>
      <source>Displacement above the baseline of the font on a line</source>
      <translation>Siirtymä kirjaintyypin peruslinjan yläpuolelle</translation>
    </message>
    <message>
      <source>Relative size of the superscript compared to the normal font</source>
      <translation>Yläindeksin kirjaintyypin koko verrattuna normaaliin kirjaintyyppiin</translation>
    </message>
    <message>
      <source>Displacement below the baseline of the normal font on a line</source>
      <translation>Siirtymä kirjaintyypin peruslinjan alapuolelle</translation>
    </message>
    <message>
      <source>Relative size of the subscript compared to the normal font</source>
      <translation>Alaindeksin kirjaintyypin koko verrattuna normaaliin kirjasimeen</translation>
    </message>
    <message>
      <source>Relative size of the small caps font compared to the normal font</source>
      <translation>Kapiteelin kirjainten koko verrattuna normaaliin kirjaintyyppiin</translation>
    </message>
    <message>
      <source>Percentage increase over the font size for the line spacing</source>
      <translation>Rivivälin kasvu prosentteina kirjaintyypin koosta</translation>
    </message>
    <message>
      <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
      <translation>Siirtymä kirjaintyypin peruslinjan alapuolelle ilmaistuna prosentteina.</translation>
    </message>
    <message>
      <source>Line width expressed as a percentage of the font size</source>
      <translation>Riviväli ilmaistuna prosentteina kirjaintyypin koosta</translation>
    </message>
    <message>
      <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
      <translation>Siirtymä kirjaintyypin peruslinjan yläpuolelle ilmaistuna prosentteina.

</translation>
    </message>
  </context>
  <context>
    <name>Tabruler</name>
    <message>
      <source>Left</source>
      <translation>Vasen</translation>
    </message>
    <message>
      <source>Right</source>
      <translation>Oikea</translation>
    </message>
    <message>
      <source>Full Stop</source>
      <translation>Piste</translation>
    </message>
    <message>
      <source>Comma</source>
      <translation>Pilkku</translation>
    </message>
    <message>
      <source>Center</source>
      <translation>Keskitetty</translation>
    </message>
    <message>
      <source>Delete All</source>
      <translation>Poista kaikki</translation>
    </message>
    <message>
      <source>Indentation for first line of the paragraph</source>
      <translation>Kappaleen ensimmäisen rivin sisennys</translation>
    </message>
    <message>
      <source>Indentation from the left for the whole paragraph</source>
      <translation>Koko kappaleen sisennys vasemmalta</translation>
    </message>
    <message>
      <source>Delete all Tabulators</source>
      <translation>Poista kaikki sarkaimet</translation>
    </message>
    <message>
      <source>&amp;Position:</source>
      <translation>&amp;Sijainti:</translation>
    </message>
    <message>
      <source>None</source>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Fill Char:</source>
      <translation>Täyttömerkki:</translation>
    </message>
    <message>
      <source>Custom:</source>
      <translation>Oma:</translation>
    </message>
    <message>
      <source>Custom: </source>
      <translation>Oma: </translation>
    </message>
    <message>
      <source>Dot</source>
      <translation>Piste</translation>
    </message>
    <message>
      <source>Hyphen</source>
      <translation>Yhdysmerkki</translation>
    </message>
    <message>
      <source>Underscore</source>
      <translation>Alaviiva</translation>
    </message>
    <message>
      <source>Custom</source>
      <translation>Oma</translation>
    </message>
  </context>
  <context>
    <name>Tree</name>
    <message>
      <source>Outline</source>
      <translation>Rakenne</translation>
    </message>
    <message>
      <source>Element</source>
      <translation>Osa</translation>
    </message>
    <message>
      <source>Group </source>
      <translation>Ryhmä</translation>
    </message>
    <message>
      <source>Free Objects</source>
      <translation>Vapaat kohteet</translation>
    </message>
    <message>
      <source>Page </source>
      <translation>Sivu</translation>
    </message>
  </context>
  <context>
    <name>UnderlineValues</name>
    <message>
      <source>Auto</source>
      <translation>Auto</translation>
    </message>
    <message>
      <source> %</source>
      <translation> %</translation>
    </message>
    <message>
      <source>Displacement</source>
      <translation>Siirtymä</translation>
    </message>
    <message>
      <source>Linewidth</source>
      <translation>Viivan leveys</translation>
    </message>
  </context>
  <context>
    <name>UndoManager</name>
    <message>
      <source>Add vertical guide</source>
      <translation>Lisää pystysuora apuviiva</translation>
    </message>
    <message>
      <source>Add horizontal guide</source>
      <translation>Lisää vaakasuora apuviiva</translation>
    </message>
    <message>
      <source>Remove vertical guide</source>
      <translation>Poista pystysuora apuviiva</translation>
    </message>
    <message>
      <source>Remove horizontal guide</source>
      <translation>Poista vaakasuora apuviiva</translation>
    </message>
    <message>
      <source>Move vertical guide</source>
      <translation>Siirrä pystysuoraa apuviivaa</translation>
    </message>
    <message>
      <source>Move horizontal guide</source>
      <translation>Siirrä vaakasuoraa apuviivaa</translation>
    </message>
    <message>
      <source>Lock guides</source>
      <translation>Lukitse apuviivat</translation>
    </message>
    <message>
      <source>Unlock guides</source>
      <translation>Vapauta apuviivat</translation>
    </message>
    <message>
      <source>Move</source>
      <translation>Siirrä</translation>
    </message>
    <message>
      <source>Resize</source>
      <translation>Muuta kokoa</translation>
    </message>
    <message>
      <source>Rotate</source>
      <translation>Kierrä</translation>
    </message>
    <message>
      <source>X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</source>
      <translation>X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</translation>
    </message>
    <message>
      <source>W1: %1, H1: %2
W2: %3, H2: %4</source>
      <translation>L1: %1, K1: %2
L2: %3, K2: %4</translation>
    </message>
    <message>
      <source>Selection</source>
      <translation>Valinta</translation>
    </message>
    <message>
      <source>Group</source>
      <translation>Ryhmitä</translation>
    </message>
    <message>
      <source>Selection/Group</source>
      <translation>Valinta/Ryhmä</translation>
    </message>
    <message>
      <source>Create</source>
      <translation>Luo</translation>
    </message>
    <message>
      <source>X: %1, Y: %2
W: %3, H: %4</source>
      <translation>X: %1, Y: %2
L: %3, K: %4</translation>
    </message>
    <message>
      <source>Align/Distribute</source>
      <translation>Jaa/Tasaa</translation>
    </message>
    <message>
      <source>Items involved</source>
      <translation>Kohteet mukana</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation>Peruuta</translation>
    </message>
    <message>
      <source>Set fill color</source>
      <translation>Aseta täyttöväri</translation>
    </message>
    <message>
      <source>Color1: %1, Color2: %2</source>
      <translation>Väri1: %1, väri2: %2</translation>
    </message>
    <message>
      <source>Set fill color shade</source>
      <translation>Aseta täyttövärin sävy</translation>
    </message>
    <message>
      <source>Set line color</source>
      <translation>Aseta viivan väri</translation>
    </message>
    <message>
      <source>Set line color shade</source>
      <translation>Aseta viivan värin sävy</translation>
    </message>
    <message>
      <source>Flip horizontally</source>
      <translation>Peilaa vaakasuunnassa</translation>
    </message>
    <message>
      <source>Flip vertically</source>
      <translation>Peilaa pystysuunnassa</translation>
    </message>
    <message>
      <source>Lock</source>
      <translation>Lukitse</translation>
    </message>
    <message>
      <source>Unlock</source>
      <translation>Vapauta</translation>
    </message>
    <message>
      <source>Lock size</source>
      <translation>Lukitse koko</translation>
    </message>
    <message>
      <source>Unlock size</source>
      <translation>Vapauta koko</translation>
    </message>
    <message>
      <source>Ungroup</source>
      <translation>Pura ryhmitys</translation>
    </message>
    <message>
      <source>Delete</source>
      <translation>Poista</translation>
    </message>
    <message>
      <source>Rename</source>
      <translation>Nimeä uudelleen</translation>
    </message>
    <message>
      <source>From %1
to %2</source>
      <translation>%1 - %2</translation>
    </message>
    <message>
      <source>Apply Master Page</source>
      <translation>Käytä mallisivua</translation>
    </message>
    <message>
      <source>Paste</source>
      <translation>Liitä</translation>
    </message>
    <message>
      <source>Cut</source>
      <translation>Leikkaa</translation>
    </message>
    <message>
      <source>Set fill color transparency</source>
      <translation>Aseta täyttövärin läpinäkyvyys</translation>
    </message>
    <message>
      <source>Set line color transparency</source>
      <translation>Aseta viivan värin läpinäkyvyys</translation>
    </message>
    <message>
      <source>Set line style</source>
      <translation>Aseta viivatyyli</translation>
    </message>
    <message>
      <source>Set the style of line end</source>
      <translation>Aseta viivan pään tyyli</translation>
    </message>
    <message>
      <source>Set the style of line join</source>
      <translation>Aseta viivan liittymän tyyli</translation>
    </message>
    <message>
      <source>Set line width</source>
      <translation>Aseta viivan leveys</translation>
    </message>
    <message>
      <source>No style</source>
      <translation>Ei tyyliä</translation>
    </message>
    <message>
      <source>Set custom line style</source>
      <translation>Aseta oma viivatyyli</translation>
    </message>
    <message>
      <source>Do not use custom line style</source>
      <translation>Älä käytä omaa viivatyyliä</translation>
    </message>
    <message>
      <source>Set start arrow</source>
      <translation>Aseta nuolen alkupää</translation>
    </message>
    <message>
      <source>Set end arrow</source>
      <translation>Aseta nuolen loppupää</translation>
    </message>
    <message>
      <source>Create table</source>
      <translation>Luo taulukko</translation>
    </message>
    <message>
      <source>Rows: %1, Cols: %2</source>
      <translation>Rivit: %1, Sarakkeet: %2</translation>
    </message>
    <message>
      <source>Set font</source>
      <translation>Aseta kirjaintyyppi</translation>
    </message>
    <message>
      <source>Set font size</source>
      <translation>Aseta kirjaintyypin koko</translation>
    </message>
    <message>
      <source>Set font width</source>
      <translation>Aseta kirjaintyypin leveys</translation>
    </message>
    <message>
      <source>Set font height</source>
      <translation>Aseta kirjaintyypin korkeus</translation>
    </message>
    <message>
      <source>Set font fill color</source>
      <translation>Aseta kirjaintyypin täyttöväri</translation>
    </message>
    <message>
      <source>Set font stroke color</source>
      <translation>Aseta kirjaintyypin reunan väri</translation>
    </message>
    <message>
      <source>Set font fill color shade</source>
      <translation>Aseta kirjaintyypin täyttövärin sävy</translation>
    </message>
    <message>
      <source>Set font stroke color shade</source>
      <translation>Aseta kirjaintyypin reunuksen värin sävy</translation>
    </message>
    <message>
      <source>Set kerning</source>
      <translation>Aseta kirjainväli</translation>
    </message>
    <message>
      <source>Set line spacing</source>
      <translation>Aseta riviväli</translation>
    </message>
    <message>
      <source>Set paragraph style</source>
      <translation>Aseta kappaletyyli</translation>
    </message>
    <message>
      <source>Set language</source>
      <translation>Aseta kieli</translation>
    </message>
    <message>
      <source>Align text</source>
      <translation>Tasaa teksti</translation>
    </message>
    <message>
      <source>Set font effect</source>
      <translation>Aseta kirjainefekti</translation>
    </message>
    <message>
      <source>Image frame</source>
      <translation>Kuvakehys</translation>
    </message>
    <message>
      <source>Text frame</source>
      <translation>Tekstikehys</translation>
    </message>
    <message>
      <source>Polygon</source>
      <translation>Monikulmio</translation>
    </message>
    <message>
      <source>Bezier curve</source>
      <translation>Bezier-viiva</translation>
    </message>
    <message>
      <source>Polyline</source>
      <translation>Kulmikas viiva</translation>
    </message>
    <message>
      <source>Convert to</source>
      <translation>Muunna</translation>
    </message>
    <message>
      <source>Import SVG image</source>
      <translation>Tuo SVG-kuva</translation>
    </message>
    <message>
      <source>Import EPS image</source>
      <translation>Tuo EPS-kuva</translation>
    </message>
    <message>
      <source>Import OpenOffice.org Draw image</source>
      <translation>Tuo OpenOffice.org Draw -kuva</translation>
    </message>
    <message>
      <source>Scratch space</source>
      <translation>Työalue</translation>
    </message>
    <message>
      <source>Text flows around the frame</source>
      <translation>Teksti kiertää kehyksen</translation>
    </message>
    <message>
      <source>Text flows around bounding box</source>
      <translation>Teksti kiertää rajausalueen</translation>
    </message>
    <message>
      <source>Text flows around contour line</source>
      <translation>Teksti kiertää reunuksen</translation>
    </message>
    <message>
      <source>No text flow</source>
      <translation>Ei tekstin kiertoa</translation>
    </message>
    <message>
      <source>No bounding box</source>
      <translation>Ei rajausaluetta</translation>
    </message>
    <message>
      <source>No contour line</source>
      <translation>Ei reunusta</translation>
    </message>
    <message>
      <source>Page %1</source>
      <translation>Sivu %1</translation>
    </message>
    <message>
      <source>Set image scaling</source>
      <translation>Aseta kuvan skaalaus</translation>
    </message>
    <message>
      <source>Frame size</source>
      <translation>Kehyksen koko</translation>
    </message>
    <message>
      <source>Free scaling</source>
      <translation>Vapaa skaalaus</translation>
    </message>
    <message>
      <source>Keep aspect ratio</source>
      <translation>Säilytä suhteet</translation>
    </message>
    <message>
      <source>Break aspect ratio</source>
      <translation>Älä säilytä suhteita 
</translation>
    </message>
    <message>
      <source>Edit contour line</source>
      <translation>Muokkaa reunusta</translation>
    </message>
    <message>
      <source>Edit shape</source>
      <translation>Muokkaa muotoa</translation>
    </message>
    <message>
      <source>Reset contour line</source>
      <translation>Palauta reunus</translation>
    </message>
    <message>
      <source>Add page</source>
      <translation>Lisää sivu</translation>
    </message>
    <message>
      <source>Add pages</source>
      <translation>Lisää sivuja</translation>
    </message>
    <message>
      <source>Delete page</source>
      <translation>Poista sivu</translation>
    </message>
    <message>
      <source>Delete pages</source>
      <translation>Poista sivuja</translation>
    </message>
    <message>
      <source>Add layer</source>
      <translation>Lisää taso</translation>
    </message>
    <message>
      <source>Delete layer</source>
      <translation>Poista taso</translation>
    </message>
    <message>
      <source>Rename layer</source>
      <translation>Nimeä taso uudelleen</translation>
    </message>
    <message>
      <source>Raise layer</source>
      <translation>Nosta tasoa</translation>
    </message>
    <message>
      <source>Lower layer</source>
      <translation>Laske tasoa</translation>
    </message>
    <message>
      <source>Send to layer</source>
      <translation>Vie tasoon</translation>
    </message>
    <message>
      <source>Enable printing of layer</source>
      <translation>Tulosta taso</translation>
    </message>
    <message>
      <source>Disable printing of layer</source>
      <translation>Älä tulosta tasoa</translation>
    </message>
    <message>
      <source>Change name of the layer</source>
      <translation>Muuta tason nimi</translation>
    </message>
    <message>
      <source>Get image</source>
      <translation>Hae kuva</translation>
    </message>
    <message>
      <source>Text on a Path</source>
      <translation>ÅTeksti polulla</translation>
    </message>
  </context>
  <context>
    <name>UndoPalette</name>
    <message>
      <source>Initial State</source>
      <translation>Alkutila</translation>
    </message>
    <message>
      <source>Action History</source>
      <translation>Toimintohistoria</translation>
    </message>
    <message>
      <source>Show selected object only</source>
      <translation>Näytä ainoastaan valittu kohde</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>&amp;Kumoa</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>&amp;Tee uudelleen</translation>
    </message>
  </context>
  <context>
    <name>UndoWidget</name>
    <message>
      <source>%1: %2</source>
      <comment>undo target: action (f.e. Text frame: Resize)</comment>
      <translation>%1: %2</translation>
    </message>
  </context>
  <context>
    <name>ValueDialog</name>
    <message>
      <source>Insert value</source>
      <translation>Lisää arvo</translation>
    </message>
    <message>
      <source>Enter a value then press OK.</source>
      <translation>Syötä arvo ja paina OK.</translation>
    </message>
    <message>
      <source>Enter a value then press OK</source>
      <translation>Syötä arvo ja paina OK</translation>
    </message>
    <message>
      <source>Alt+O</source>
      <translation>Alt+O</translation>
    </message>
    <message>
      <source>Send your value to the script</source>
      <translation>Lähetä arvo skriptille</translation>
    </message>
  </context>
  <context>
    <name>WerkToolB</name>
    <message>
      <source>Tools</source>
      <translation>Työkalut</translation>
    </message>
    <message>
      <source>Properties...</source>
      <translation>Ominaisuudet...</translation>
    </message>
  </context>
  <context>
    <name>WerkToolBP</name>
    <message>
      <source>Link</source>
      <translation>Linkki</translation>
    </message>
    <message>
      <source>Text</source>
      <translation>Teksti</translation>
    </message>
    <message>
      <source>List Box</source>
      <translation>Listavalikko</translation>
    </message>
    <message>
      <source>Combo Box</source>
      <translation>Alasvetovalikko</translation>
    </message>
    <message>
      <source>Button</source>
      <translation>Painike</translation>
    </message>
    <message>
      <source>Text Field</source>
      <translation>Tekstikenttä</translation>
    </message>
    <message>
      <source>Check Box</source>
      <translation>Valintaruutu</translation>
    </message>
    <message>
      <source>PDF Tools</source>
      <translation>PDF-työkalut</translation>
    </message>
    <message>
      <source>Insert PDF Fields</source>
      <translation>Lisää PDF-kenttä</translation>
    </message>
    <message>
      <source>Insert PDF Annotations</source>
      <translation>Lisää PDF-huomautus</translation>
    </message>
  </context>
  <context>
    <name>gtFileDialog</name>
    <message>
      <source>Choose the importer to use</source>
      <translation>Valitse käytettävä tuojatyyppi</translation>
    </message>
    <message>
      <source>Automatic</source>
      <translation>Automaattinen</translation>
    </message>
    <message>
      <source>Import text without any formatting</source>
      <translation>Tuo teksti ilman muotoilua</translation>
    </message>
    <message>
      <source>Importer:</source>
      <translation>Tuoja:</translation>
    </message>
    <message>
      <source>Encoding:</source>
      <translation>Koodaus:</translation>
    </message>
    <message>
      <source>Import Text Only</source>
      <translation>Tuo ainoastaan teksti</translation>
    </message>
  </context>
  <context>
    <name>gtImporterDialog</name>
    <message>
      <source>Choose the importer to use</source>
      <translation>Valitse käytettävä tuojatyyppi</translation>
    </message>
    <message>
      <source>Remember association</source>
      <translation>Muista yhteys</translation>
    </message>
    <message>
      <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
      <translation>Muista tiedostopääte - tuoja -yhteys äläkä kysy uudestaan tuotaessa tiedostoja tällä päätteellä.</translation>
    </message>
  </context>
  <context>
    <name>nftdialog</name>
    <message>
      <source>New From Template</source>
      <translation>Uusi mallista</translation>
    </message>
    <message>
      <source>All</source>
      <translation>Kaikki</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Nimi</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>Sivun koko</translation>
    </message>
    <message>
      <source>Colors</source>
      <translation>Värit</translation>
    </message>
    <message>
      <source>Description</source>
      <translation>Kuvaus</translation>
    </message>
    <message>
      <source>Usage</source>
      <translation>Käyttö</translation>
    </message>
    <message>
      <source>Author</source>
      <translation>Tekijä</translation>
    </message>
    <message>
      <source>Created with</source>
      <translation>Tehty</translation>
    </message>
    <message>
      <source>&amp;Remove</source>
      <translation>&amp;Poista</translation>
    </message>
    <message>
      <source>&amp;Open</source>
      <translation>&amp;Avaa</translation>
    </message>
    <message>
      <source>Downloading Templates</source>
      <translation>Asiakirjamallien hakeminen</translation>
    </message>
    <message>
      <source>Installing Templates</source>
      <translation>Asiakirjamallien asennus</translation>
    </message>
    <message>
      <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
      <translation>Pura paketti mallihakemistoon ~/.scribus/templates nykyiselle käyttäjälle tai PREFIX/share/scribus/templates kaikille järjestelmän käyttäjille.</translation>
    </message>
    <message>
      <source>Preparing a template</source>
      <translation>Asiakirjamallin valmistelu</translation>
    </message>
    <message>
      <source>Removing a template</source>
      <translation>Malliasiakirjan poistaminen</translation>
    </message>
    <message>
      <source>Translating template.xml</source>
      <translation>Template.xml-tiedoston kääntäminen</translation>
    </message>
    <message>
      <source>Date</source>
      <translation>Päivämäärä</translation>
    </message>
    <message>
      <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
      <translation>Asiakirjamalleja löytyy osoitteesta http://www.scribus.net/ Downloads-sivulta.</translation>
    </message>
    <message>
      <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
      <translation>Varmista että kuvia voidaan käyttää vapaasti. Jos kirjaintyyppiä ei voi levittää, älä kerää niitä tallentaessasi asiakirjamallin.</translation>
    </message>
    <message>
      <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
      <translation>Asiakirjapohjan tekijän tulisi varmistaa että kohta &quot;Asiakirjamallien asennus&quot; pätee myös hänen malleihinsa. Käyttäjän tulisi saada mallit käyttöönsä purkamalla asiakirjapohjapaketin mallihakemistoonsa.</translation>
    </message>
    <message>
      <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
      <translation>Poistettaessa malli &quot;Uusi mallista&quot;-dialogissa vain malliin liittyvät rivit template.xml-tiedostossa poistetaan; siihen liittyviä tiedostoja ei. Poistaminen on mahdollista ainoastaan, kun käyttäjällä on kirjoitusoikeus template.xml-tiedostoon.</translation>
    </message>
    <message>
      <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
      <translation>Kopioi olemassa oleva template.xml-tiedosto tiedostoon template.kieli_MAA.xml. Käytä samaa kielikoodia kuin käännöstiedostossa on käytetty kielellesi. Esimerkiksi suomenkielinen käännös tulisi nimetä template.fi.xml. Kopion tulee sijaita samassa hakemistossa kuin alkuperäinen template.xml-tiedosto.</translation>
    </message>
  </context>
  <context>
    <name>satdialog</name>
    <message>
      <source>Save as Template</source>
      <translation>Tallenna mallina</translation>
    </message>
    <message>
      <source>Name</source>
      <translation>Nimi</translation>
    </message>
    <message>
      <source>Category</source>
      <translation>Tyyppi</translation>
    </message>
    <message>
      <source>Page Size</source>
      <translation>Sivun koko</translation>
    </message>
    <message>
      <source>Colors</source>
      <translation>Värit</translation>
    </message>
    <message>
      <source>Description</source>
      <translation>Kuvaus</translation>
    </message>
    <message>
      <source>Usage</source>
      <translation>Käyttö</translation>
    </message>
    <message>
      <source>Author</source>
      <translation>Tekijä</translation>
    </message>
    <message>
      <source>Email</source>
      <translation>Sähköpostiosoite</translation>
    </message>
    <message>
      <source>More Details</source>
      <translation>Lisää yksityiskohtia</translation>
    </message>
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Less Details</source>
      <translation>Vähemmän yksityiskohtia</translation>
    </message>
    <message>
      <source>Legal</source>
      <translation>Legal</translation>
    </message>
    <message>
      <source>Letter</source>
      <translation>Letter</translation>
    </message>
    <message>
      <source>Tabloid</source>
      <translation>Tabloid</translation>
    </message>
    <message>
      <source>landscape</source>
      <translation>vaakasuora</translation>
    </message>
    <message>
      <source>portrait</source>
      <translation>pystysuora</translation>
    </message>
    <message>
      <source>custom</source>
      <translation>oma</translation>
    </message>
  </context>
  <context>
    <name>tfDia</name>
    <message>
      <source>Create filter</source>
      <translation>Luo suodin</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>&amp;Tyhjennä</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>P&amp;oista</translation>
    </message>
    <message>
      <source>Choose a previously saved filter</source>
      <translation>Valitse tallennettu suodin</translation>
    </message>
    <message>
      <source>Give a name to this filter for saving</source>
      <translation>Anna nimi tallennusta varten</translation>
    </message>
    <message>
      <source>Give a name for saving</source>
      <translation>Anna nimi tallennusta varten</translation>
    </message>
  </context>
  <context>
    <name>tfFilter</name>
    <message>
      <source>Disable or enable this filter row</source>
      <translation>Poista käytöstä tai ota käyttöön tämä rivi</translation>
    </message>
    <message>
      <source>Remove this filter row</source>
      <translation>Poista tämä rivi</translation>
    </message>
    <message>
      <source>Add a new filter row</source>
      <translation>Lisää uusi rivi suotimeen</translation>
    </message>
    <message>
      <source>to</source>
      <translation>mihin</translation>
    </message>
    <message>
      <source>and</source>
      <translation>ja</translation>
    </message>
    <message>
      <source>remove match</source>
      <translation>poista osuma</translation>
    </message>
    <message>
      <source>do not remove match</source>
      <translation>älä poista osumaa</translation>
    </message>
    <message>
      <source>words</source>
      <translation>sanaa</translation>
    </message>
    <message>
      <source>Remove</source>
      <translation>Poista</translation>
    </message>
    <message>
      <source>Replace</source>
      <translation>Korvaa</translation>
    </message>
    <message>
      <source>Apply</source>
      <translation>Käytä</translation>
    </message>
    <message>
      <source>with</source>
      <translation>millä</translation>
    </message>
    <message>
      <source>paragraph style</source>
      <translation>Kappaletyyliä</translation>
    </message>
    <message>
      <source>all instances of</source>
      <translation>kaikki</translation>
    </message>
    <message>
      <source>all paragraphs</source>
      <translation>kaikkiin kappaleisiin</translation>
    </message>
    <message>
      <source>paragraphs starting with</source>
      <translation>kappaleisiin, jotka alkavat</translation>
    </message>
    <message>
      <source>paragraphs with less than</source>
      <translation>kappaleisiin, joissa on vähemmän kuin</translation>
    </message>
    <message>
      <source>paragraphs with more than</source>
      <translation>kappaleisiin, joissa on enemmän kuin</translation>
    </message>
    <message>
      <source>Value at the left is a regular expression</source>
      <translation>Teksti vasemmalla on regular expression</translation>
    </message>
  </context>
</TS>
