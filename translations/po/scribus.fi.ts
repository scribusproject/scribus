<!DOCTYPE TS>
<TS>
  <context>
    <name>@default</name>
    <message>
      <source>getColorNames() -> list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
      <translation type="unfinished" >getColorNames() -> list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.</translation>
    </message>
    <message>
      <source>newDocDialog() -> bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
      <translation type="unfinished" >getColorNames() -> list

Näyttää dialogin &quot;Uusi asiakirja&quot;. Luo uuden asiakirjan, jos käyttäjä hyväksyy
asetukset. Asiakirjaa ei luoda, jos käyttäjä painaa Peruuta-painiketta.
Palauttaa true, jos uusi asiakirja luotiin.</translation>
    </message>
    <message>
      <source>getFillColor([&quot;name&quot;]) -> string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" >getFillColor([&quot;name&quot;]) -> string

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
      <translation type="unfinished" >moveObject(dx, dy [, &quot;name&quot;])

Siirtää kohdetta &quot;name&quot; arvoilla dx ja dy  suhteessa nykyiseen sijaintiin. Etäisyydet
ilmaistaan asiakirjassa käytössä olevalla yksiköllä (katso UNIT-vakio). Jos &quot;name&quot; ei
ole määritelty, käytetään valittuna olevaa kohdetta. Jos kohde &quot;name&quot; kuuluu 
ryhmään, koko ryhmää siirretään.</translation>
    </message>
    <message>
      <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
      <translation type="unfinished" >setRedraw(bool)

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
      <translation type="unfinished" >createRect(x, y, width, height, [&quot;name&quot;]) -> string

Luo uuden nelikulmion nykyiselle sivulle ja palauttaa luodun kohteen nimen.
Koordinaatit on annettava asiakirjassa käytössä olevalla yksiköllä. Katso (UNIT vakiot).
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
      <translation type="unfinished" >setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Asettaa liukuväritäytön kohteelle &quot;name&quot;. Värin kuvaukset ovat samat kuin
setFillColor():lla ja setFillShade():lla. Katso vakiot FILL_&lt;type>.</translation>
    </message>
    <message>
      <source>getFontSize([&quot;name&quot;]) -> float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" >getFontSize([&quot;name&quot;]) -> float

Palauttaa fontin koon pisteinä tekstikehykselle &quot;name&quot;. Jos tekstikehyksessä
on tekstiä valittuna palautetaan valinnan ensimmäisen fontin koko.
Jos &quot;name&quot; ei ole määritelty, käytetään valittuna olevaa kohdetta.</translation>
    </message>
    <message>
      <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or 'unicode' string(recommended).
</source>
      <translation type="unfinished" >messagebarText(&quot;string&quot;)

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
      <translation type="unfinished" >newPage(where [,&quot;masterpage&quot;])

Creates a new page. If &quot;where&quot; is -1 the new Page is appended to the
document, otherwise the new page is inserted before &quot;where&quot;. Page numbers are
counted from 1 upwards, no matter what the displayed first page number of your
document is. The optional parameter &quot;masterpage&quot; specifies the name of the
master page for the new page.

May raise IndexError if the page number is out of range
</translation>
    </message>
    <message>
      <source>newDocument(size, margins, orientation, firstPageNumber,
                        unit, pagesType, firstPageOrder, numPages) -> bool

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

numPage = Number of pages to be created.

The values for width, height and the margins are expressed in the given unit
for the document. PAPER_* constants are expressed in points. If your document
is not in points, make sure to account for this.

example: newDocument(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 7, UNIT_POINTS,
PAGE_4, 3, 1)

May raise ScribusError if is firstPageOrder bigger than allowed by pagesType.
</source>
      <translation type="unfinished" >newDocument(size, margins, orientation, firstPageNumber,
                        unit, pagesType, firstPageOrder, numPages) -> bool

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

numPage = Number of pages to be created.

The values for width, height and the margins are expressed in the given unit
for the document. PAPER_* constants are expressed in points. If your document
is not in points, make sure to account for this.

example: newDocument(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 7, UNIT_POINTS,
PAGE_4, 3, 1)

May raise ScribusError if is firstPageOrder bigger than allowed by pagesType.
</translation>
    </message>
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
    <message>
      <source>duplicateObject([&quot;name&quot;]) -> string

creates a Duplicate of the selected Object (or Selection Group).
</source>
      <translation>duplicateObject([&quot;name&quot;]) -> string

creates a Duplicate of the selected Object (or Selection Group).
</translation>
    </message>
    <message>
      <source>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -> bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
      <translation>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -> bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</translation>
    </message>
    <message>
      <source>getFillTransparency([&quot;name&quot;]) -> float

Returns the fill transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
      <translation>getFillTransparency([&quot;name&quot;]) -> float

Returns the fill transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</translation>
    </message>
    <message>
      <source>getFillBlendmode([&quot;name&quot;]) -> integer

Returns the fill blendmode of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
      <translation>getFillBlendmode([&quot;name&quot;]) -> integer

Returns the fill blendmode of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</translation>
    </message>
    <message>
      <source>getLineTransparency([&quot;name&quot;]) -> float

Returns the line transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
      <translation>getLineTransparency([&quot;name&quot;]) -> float

Returns the line transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</translation>
    </message>
    <message>
      <source>getLineBlendmode([&quot;name&quot;]) -> integer

Returns the line blendmode of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
      <translation>getLineBlendmode([&quot;name&quot;]) -> integer

Returns the line blendmode of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</translation>
    </message>
    <message>
      <source>setLayerLocked(&quot;layer&quot;, locked)

Sets the layer &quot;layer&quot; to be locked or not. If locked is set to
true the layer will be locked.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>setLayerLocked(&quot;layer&quot;, locked)

Sets the layer &quot;layer&quot; to be locked or not. If locked is set to
true the layer will be locked.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>isLayerLocked(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is locked or not, a value of True means
that the layer &quot;layer&quot; is editable, a value of False means that the layer
&quot;layer&quot; is locked.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>isLayerLocked(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is locked or not, a value of True means
that the layer &quot;layer&quot; is editable, a value of False means that the layer
&quot;layer&quot; is locked.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>isLayerOutlined(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is outlined or not, a value of True means
that the layer &quot;layer&quot; is outlined, a value of False means that the layer
&quot;layer&quot; is normal.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>isLayerOutlined(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is outlined or not, a value of True means
that the layer &quot;layer&quot; is outlined, a value of False means that the layer
&quot;layer&quot; is normal.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>isLayerFlow(&quot;layer&quot;) -> bool

Returns whether text flows around objects on layer &quot;layer&quot;, a value of True means
that text flows around, a value of False means that the text does not flow around.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>isLayerFlow(&quot;layer&quot;) -> bool

Returns whether text flows around objects on layer &quot;layer&quot;, a value of True means
that text flows around, a value of False means that the text does not flow around.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>getLayerBlendmode(&quot;layer&quot;) -> int

Returns the &quot;layer&quot; layer blendmode,

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>getLayerBlendmode(&quot;layer&quot;) -> int

Returns the &quot;layer&quot; layer blendmode,

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>getLayerTransparency(&quot;layer&quot;) -> float

Returns the &quot;layer&quot; layer transparency,

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation>getLayerTransparency(&quot;layer&quot;) -> float

Returns the &quot;layer&quot; layer transparency,

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</translation>
    </message>
    <message>
      <source>textFlowMode(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional int &quot;state&quot; (0 &lt;= state &lt;= 3).
Setting &quot;state&quot; to 0 will disable text flow.
Setting &quot;state&quot; to 1 will make text flow around object frame.
Setting &quot;state&quot; to 2 will make text flow around bounding box.
Setting &quot;state&quot; to 3 will make text flow around contour line.
If &quot;state&quot; is not passed, text flow is toggled.
</source>
      <translation>textFlowMode(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional int &quot;state&quot; (0 &lt;= state &lt;= 3).
Setting &quot;state&quot; to 0 will disable text flow.
Setting &quot;state&quot; to 1 will make text flow around object frame.
Setting &quot;state&quot; to 2 will make text flow around bounding box.
Setting &quot;state&quot; to 3 will make text flow around contour line.
If &quot;state&quot; is not passed, text flow is toggled.
</translation>
    </message>
    <message>
      <source>getPageType() -> integer

Returns the type of the Page, 0 means left Page, 1 is a middle Page and 2 is a right Page
</source>
      <translation>getPageType() -> integer

Returns the type of the Page, 0 means left Page, 1 is a middle Page and 2 is a right Page
</translation>
    </message>
    <message>
      <source>getTextLines([&quot;name&quot;]) -> integer

Returns the number of lines of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation>getTextLines([&quot;name&quot;]) -> integer

Returns the number of lines of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
      <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yellow and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getCornerRadius([&quot;name&quot;]) -> integer

Returns the corner radius of the object &quot;name&quot;. The radius is
expressed in points. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getPosition([&quot;name&quot;]) -> (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type> for reference.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positive values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the
printable set to false the layer won't be printed.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>isLayerVisible(&quot;layer&quot;) -> bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference) The first character has an
index of 0. Inserting text at position -1 appends it to the frame. If &quot;name&quot; is
not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Qt::DockLeft(lr), Qt::DockRight(rr), Qt::DockTop(tr) and Qt::DockBottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type>
constants.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLayerOutlined(&quot;layer&quot;, outline)

Sets the layer &quot;layer&quot; to be locked or not. If outline is set to
true the layer will be displayed outlined.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLayerFlow(&quot;layer&quot;, flow)

Sets the layers &quot;layer&quot;  flowcontrol to flow. If flow is set to
true text in layers above this one will flow around objects on this layer.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLayerBlendmode(&quot;layer&quot;, blend)

Sets the layers &quot;layer&quot;  blendmode to blend.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLayerTransparency(&quot;layer&quot;, trans)

Sets the layers &quot;layer&quot;  transparency to trans.

May raise NotFoundError if the layer can't be found.
May raise ValueError if the layer name isn't acceptable.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setFillTransparency(transparency, [&quot;name&quot;])

Sets the fill transparency of the object &quot;name&quot; to transparency
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setFillBlendmode(blendmode, [&quot;name&quot;])

Sets the fill blendmode of the object &quot;name&quot; to blendmode
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLineTransparency(transparency, [&quot;name&quot;])

Sets the line transparency of the object &quot;name&quot; to transparency
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setLineBlendmode(blendmode, [&quot;name&quot;])

Sets the line blendmode of the object &quot;name&quot; to blendmode
If &quot;name&quot; is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>placeEPS(&quot;filename&quot;, x, y)

Places the EPS &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the EPS placed on the page

If loading was successful, the selection contains the imported EPS
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>placeSXD(&quot;filename&quot;, x, y)

Places the SXD &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the SXD placed on the page

If loading was successful, the selection contains the imported SXD
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>placeODG(&quot;filename&quot;, x, y)

Places the ODG &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the ODG placed on the page

If loading was successful, the selection contains the imported ODG
</source>
      <translation type="unfinished" />
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
optional parameters is False.

The filter, if specified, takes the form 'comment (*.type *.type2 ...)'.
For example 'Images (*.png *.xpm *.jpg)'.

Refer to the Qt-Documentation for QFileDialog for details on filters.

Example: fileDialog('Open input', 'CSV files (*.csv)')
Example: fileDialog('Save report', defaultname='report.txt', issave=True)
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getLineJoin([&quot;name&quot;]) -> integer (see constants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>rotateObject(rot [, &quot;name&quot;])

Rotates the object &quot;name&quot; by &quot;rot&quot; degrees relatively. The object is
rotated by the vertex that is currently selected as the rotation point - by
default, the top left vertex at zero rotation. Positive values mean counter
clockwise rotation when the default rotation point is used. If &quot;name&quot; is not
given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn't (currently) change the page the user's view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets whether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>moveSelectionToFront()

Moves current selection to front.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>moveSelectionToFront()

Moves current selection to back.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>importPage(&quot;fromDoc&quot;, (pageList), [create, imortwhere, importwherePage])

Imports a set of pages (given as a tuple) from an existing document (the file name must be given). This functions maps the &quot;Page->Import&quot; dropdown menu function.
fromDoc: string; the filename of the document to import pages from
pageList: tuple with page numbers of pages to import
create: number; 0 to replace existing pages, 1 (default) to insert new pages
importWhere: number; the page number (of the current document) at which import the pages
importWherePage: number; used if create==1; 0 to create pages before selected page; 1 to create pages after selected page; 2 (default) to create pages at the end of the document
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>createCharStyle(...)

Creates a character style. This function takes the following keyword parameters:

&quot;name&quot; [required] -> name of the char style to create

&quot;font&quot; [optional] -> name of the font to use

fontsize [optional] -> font size to set (double)

&quot;features&quot; [optional] -> nearer typographic details can be defined by a string that might contain the following phrases comma-seperated (without spaces!):

-> inherit

-> bold

-> italic

-> underline

-> underlinewords

-> strike

-> superscript

-> subscript

-> outline

-> shadowed

-> allcaps

-> smallcaps

&quot;fillcolor&quot; [optional], &quot;fillshade&quot; [optional] -> specify fill options

&quot;strokecolor&quot; [optional], &quot;strokeshade&quot; [optional] -> specify stroke options

baselineoffset [optional] -> offset of the baseline

shadowxoffset [optional], shadowyoffset [optional] -> offset of the shadow if used

outlinewidth [optional] -> width of the outline if used

underlineoffset [optional], underlinewidth [optional] -> underline options if used

strikethruoffset [optional], strikethruwidth [optional] -> strikethru options if used

scaleh [optional], scalev [optional] -> scale of the chars

tracking [optional] -> tracking of the text

&quot;language&quot; [optional] -> language code

</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>hyphenateText([&quot;name&quot;]) -> bool

Does hyphenation on text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>dehyphenateText([&quot;name&quot;]) -> bool

Does dehyphenation on text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>scrollDocument(x,y)

Scroll the document in main GUI window by x and y.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>createParagraphStyle(...)

Creates a paragraph style. This function takes the following keyword parameters:

&quot;name&quot; [required] -> specifies the name of the paragraphstyle to create

linespacingmode [optional] -> specifies the linespacing mode; possible modes are:

fixed linespacing:          0

automatic linespacing:      1

baseline grid linespacing:  2

linespacing [optional] -> specifies the linespacing if using fixed linespacing

alignment [optional] -> specifies the alignment of the paragraph

-> left:     0

-> center:   1

-> right:    2

-> justify:  3

-> extend:   4

leftmargin [optional], rightmargin [optional] -> specify the margin

gapbefore [optional], gapafter [optional] -> specify the gaps to the heading and following paragraphs

firstindent [optional] -> the indent of the first line

hasdropcap [optional] -> specifies if there are caps (1 = yes, 0 = no)

dropcaplines [optional] -> height (in lines) of the caps if used

dropcapoffset [optional] -> offset of the caps if used

&quot;charstyle&quot; [optional] -> char style to use

</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>placeSVG(&quot;filename&quot;, x, y)

Places the SVG &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the SVG placed on the page

If loading was successful, the selection contains the imported SVG
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>getTextDistances([&quot;name&quot;]) -> tuple

Returns the text distances of the text frame &quot;name&quot; expressed in points. The
distances are returned as a tuple like (left, right, top, bottom). If &quot;name&quot;
is not given the currently selected item is used.
</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>setTextDistances(left, right, top, bottom, [&quot;name&quot;])

Sets the text distances of the text frame &quot;name&quot; to the values &quot;left&quot;
&quot;right&quot;, &quot;top&quot; and &quot;bottom&quot;. If &quot;name&quot; is not given the currently
selected item is used.

May throw ValueError if any of the distances are out of bounds (must be positive).
</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>AIPlug</name>
    <message>
      <source>Importing: %1</source>
      <translation>Tuodaan: %1</translation>
    </message>
    <message>
      <source>Analyzing File:</source>
      <translation>Tutkitaan tiedostoa:</translation>
    </message>
    <message>
      <source>Group%1</source>
      <translation>Ryhmä%1</translation>
    </message>
    <message>
      <source>Generating Items</source>
      <translation>Luodaan kohteita</translation>
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
      <source>Build ID:</source>
      <translation>Käännöstunnus</translation>
    </message>
    <message>
      <source>Mac OS&amp;#174; X Aqua Port:</source>
      <translation>Mac OS&amp;#174; X Aqua -versio:</translation>
    </message>
    <message>
      <source>Windows&amp;#174; Port:</source>
      <translation>Windows&amp;#174; -versio:</translation>
    </message>
    <message>
      <source>Tango Project Icons:</source>
      <translation>Tango-projektin kuvakkeet:</translation>
    </message>
    <message>
      <source>&amp;Updates</source>
      <translation>&amp;Päivitykset</translation>
    </message>
    <message>
      <source>Check for updates to Scribus. No data from your machine will be transferred off it.</source>
      <translation>Tarkista Scribus-päivitykset. Tietoa koneeltasi ei lähetetä eteenpäin.</translation>
    </message>
    <message>
      <source>OS/2&amp;#174;/eComStation&amp;#8482; Port:</source>
      <translation>OS/2&amp;#174;/eComStation&amp;#8482;-versio:</translation>
    </message>
    <message>
      <source>Splash Screen:</source>
      <translation>Latausikkuna:</translation>
    </message>
    <message>
      <source>This panel shows the version, build date and compiled in library support in Scribus.</source>
      <translation>Tämä ikkuna näyttää version, käännöspäivämäärän sekä Scribuksen käyttämät kirjastot.</translation>
    </message>
    <message>
      <source>The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support.Last Letter is the renderer C=cairo or Q=Qt</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The Windows version does not use fontconfig or CUPS libraries.</source>
      <translation>Windows-versio ei käytä fontconfig- eikä CUPS-kirjastoja.</translation>
    </message>
    <message>
      <source>&lt;p align=&quot;center&quot;>&lt;b>%1 %2&lt;/b>&lt;/p>&lt;p align=&quot;center&quot;>%3&lt;br>%4 %5&lt;br>%6&lt;/p></source>
      <translation>&lt;p align=&quot;center&quot;>&lt;b>%1 %2&lt;/b>&lt;/p>&lt;p align=&quot;center&quot;>%3&lt;br>%4 %5&lt;br>%6&lt;/p></translation>
    </message>
    <message>
      <source>Scribus Version</source>
      <translation>Scribus-versio</translation>
    </message>
    <message>
      <source>Check for Updates</source>
      <translation>Tarkista päivitykset</translation>
    </message>
    <message>
      <source>Abort Update Check</source>
      <translation>Peruuta päivitysten tarkistus</translation>
    </message>
    <message>
      <source>Developer Blog</source>
      <translation>Kehittäjien blogi</translation>
    </message>
    <message>
      <source>&amp;Licence</source>
      <translation>&amp;Lisenssi</translation>
    </message>
    <message>
      <source>Unable to open licence file. Please check your install directory or the Scribus website for licencing information.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Doc Translators:</source>
      <translation>Ohjeiden kääntäjät:</translation>
    </message>
    <message>
      <source>Webmasters:</source>
      <translation>Sivustojen ylläpitäjät</translation>
    </message>
    <message>
      <source>Unable to open %1 file. Please check your install directory or the Scribus website for %1 information.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>AboutPlugins</name>
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
    <message>
      <source>Scribus: About Plug-ins</source>
      <translation type="unfinished" >Scribus: Tietoja liitännäisistä</translation>
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
      <translation>&amp;Objektikohtainen toimintohistoria</translation>
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
      <source>&amp;Polygon</source>
      <translation>&amp;Monikulmio</translation>
    </message>
    <message>
      <source>&amp;Text Frame</source>
      <translation>&amp;Tekstikehys</translation>
    </message>
    <message>
      <source>&amp;Glyph...</source>
      <translation>&amp;Merkki...</translation>
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
      <translation>&amp;Tuo...</translation>
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
      <translation>&amp;Ohje...</translation>
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
      <source>&amp;Cascade</source>
      <translation>&amp;Lomittain</translation>
    </message>
    <message>
      <source>&amp;Tile</source>
      <translation>&amp;Vierekkäin</translation>
    </message>
    <message>
      <source>&amp;Outline</source>
      <comment>Document Outline Palette</comment>
      <translation>&amp;Rakenne</translation>
    </message>
    <message>
      <source>ff</source>
      <translation>ff</translation>
    </message>
    <message>
      <source>fi</source>
      <translation>fi</translation>
    </message>
    <message>
      <source>fl</source>
      <translation>fl</translation>
    </message>
    <message>
      <source>ffi</source>
      <translation>ffi</translation>
    </message>
    <message>
      <source>ffl</source>
      <translation>ffl</translation>
    </message>
    <message>
      <source>ft</source>
      <translation>ft</translation>
    </message>
    <message>
      <source>st</source>
      <translation>st</translation>
    </message>
    <message>
      <source>&amp;Printing Enabled</source>
      <translation>&amp;Tulostetaan</translation>
    </message>
    <message>
      <source>&amp;Flip Horizontally</source>
      <translation>&amp;Peilaa vaakasuunnassa</translation>
    </message>
    <message>
      <source>&amp;Flip Vertically</source>
      <translation>&amp;Peilaa pystysuunnassa</translation>
    </message>
    <message>
      <source>Show Rulers</source>
      <translation>Näytä viivaimet</translation>
    </message>
    <message>
      <source>Solidus</source>
      <translation>Solidus</translation>
    </message>
    <message>
      <source>Middle Dot</source>
      <translation>Rivinkeskinen piste</translation>
    </message>
    <message>
      <source>En Space</source>
      <translation>Puolikas</translation>
    </message>
    <message>
      <source>Em Space</source>
      <translation>Neliö</translation>
    </message>
    <message>
      <source>Thin Space</source>
      <translation>Ohuke</translation>
    </message>
    <message>
      <source>Thick Space</source>
      <translation>1/3 Neliö</translation>
    </message>
    <message>
      <source>Mid Space</source>
      <translation>1/4 Neliö</translation>
    </message>
    <message>
      <source>Hair Space</source>
      <translation>Hieno väli</translation>
    </message>
    <message>
      <source>S&amp;tyles...</source>
      <translation>&amp;Tyylit...</translation>
    </message>
    <message>
      <source>&amp;Outline</source>
      <comment>type effect</comment>
      <translation>&amp;Rakenne</translation>
    </message>
    <message>
      <source>&amp;Outlines</source>
      <comment>Convert to oulines</comment>
      <translation>&amp;Ääriviivat</translation>
    </message>
    <message>
      <source>Paste (&amp;Absolute)</source>
      <translation>Liitä (&amp;absoluuttinen)</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation>T&amp;yhjennä</translation>
    </message>
    <message>
      <source>Show Text Frame Columns</source>
      <translation>Näytä tekstikehyksen palstat</translation>
    </message>
    <message>
      <source>Save as &amp;EPS...</source>
      <translation>Tallenna &amp;EPS-tiedostoksi...</translation>
    </message>
    <message>
      <source>Patterns...</source>
      <translation>Kuvioinnit...</translation>
    </message>
    <message>
      <source>Send to Patterns</source>
      <translation>Vie kuvioinniksi</translation>
    </message>
    <message>
      <source>&amp;Frame...</source>
      <translation>&amp;Kehys...</translation>
    </message>
    <message>
      <source>Preview Mode</source>
      <translation>Esikatselutila</translation>
    </message>
    <message>
      <source>Show Layer Indicators</source>
      <translation type="unfinished" >Näytä tasojen merkit</translation>
    </message>
    <message>
      <source>Insert PDF Push Button</source>
      <translation>Lisää PDF-painike</translation>
    </message>
    <message>
      <source>Insert PDF Text Field</source>
      <translation>Lisää PDF-tekstikenttä</translation>
    </message>
    <message>
      <source>Insert PDF Check Box</source>
      <translation>Lisää PDF-valintaruutu</translation>
    </message>
    <message>
      <source>Insert PDF Combo Box</source>
      <translation>Lisää PDF-alasvetovalikko</translation>
    </message>
    <message>
      <source>Insert PDF List Box</source>
      <translation>Lisää PDF-listavalikko</translation>
    </message>
    <message>
      <source>Insert Text Annotation</source>
      <translation>Lisää Tekstikenttä</translation>
    </message>
    <message>
      <source>Insert Link Annotation</source>
      <translation>Lisää linkki</translation>
    </message>
    <message>
      <source>Sticky Tools</source>
      <translation>Työkalut pysyvät päällä</translation>
    </message>
    <message>
      <source>&amp;Fit to Height</source>
      <translation>&amp;Sovita korkeuteen</translation>
    </message>
    <message>
      <source>Fit to Width</source>
      <translation>Sovita leveyteen</translation>
    </message>
    <message>
      <source>Show Bleeds</source>
      <translation>Näytä leikkuuvarat</translation>
    </message>
    <message>
      <source>&amp;Zero Width Space</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Zero Width NB Space</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Apostrophe</source>
      <comment>
Unicode 0x0027</comment>
      <translation type="unfinished" >Ascii-heittomerkki</translation>
    </message>
    <message>
      <source>Straight Double</source>
      <comment>
Unicode 0x0022</comment>
      <translation type="unfinished" >Ascii-lainausmerkki</translation>
    </message>
    <message>
      <source>Single Left</source>
      <comment>
Unicode 0x2018</comment>
      <translation type="unfinished" >Ylösalainen puolilainausmerkki</translation>
    </message>
    <message>
      <source>Single Right</source>
      <comment>
Unicode 0x2019</comment>
      <translation type="unfinished" >Puolilainausmerkki</translation>
    </message>
    <message>
      <source>Double Left</source>
      <comment>
Unicode 0x201C</comment>
      <translation type="unfinished" >Ylösalainen kokolainausmerkki</translation>
    </message>
    <message>
      <source>Double Right</source>
      <comment>
Unicode 0x201D</comment>
      <translation type="unfinished" >Kokolainausmerkki</translation>
    </message>
    <message>
      <source>Single Reversed</source>
      <comment>
Unicode 0x201B</comment>
      <translation type="unfinished" >Käännetty puolilainausmerkki</translation>
    </message>
    <message>
      <source>Double Reversed</source>
      <comment>
Unicode 0x201F</comment>
      <translation type="unfinished" >Käännetty kokolainausmerkki</translation>
    </message>
    <message>
      <source>Single Left Guillemet</source>
      <comment>
Unicode 0x2039</comment>
      <translation type="unfinished" >Vasen kulmapuolilainausmerkki</translation>
    </message>
    <message>
      <source>Single Right Guillemet</source>
      <comment>
Unicode 0x203A</comment>
      <translation type="unfinished" >Oikea kulmapuolilainausmerkki</translation>
    </message>
    <message>
      <source>Double Left Guillemet</source>
      <comment>
Unicode 0x00AB</comment>
      <translation type="unfinished" >Vasen kulmalainausmerkki</translation>
    </message>
    <message>
      <source>Double Right Guillemet</source>
      <comment>
Unicode 0x00BB</comment>
      <translation type="unfinished" >Oikea kulmalainausmerkki</translation>
    </message>
    <message>
      <source>Low Single Comma</source>
      <comment>
Unicode 0x201A</comment>
      <translation type="unfinished" >Rivinalinen puolilainausmerkki</translation>
    </message>
    <message>
      <source>Low Double Comma</source>
      <comment>
Unicode 0x201E</comment>
      <translation type="unfinished" >Rivinalinen kokolainausmerkki</translation>
    </message>
    <message>
      <source>CJK Single Left</source>
      <comment>
Unicode 0x300C</comment>
      <translation type="unfinished" >CJK vasen puolilainausmerkki</translation>
    </message>
    <message>
      <source>CJK Single Right</source>
      <comment>
Unicode 0x300D</comment>
      <translation type="unfinished" >CJK oikea puolilainausmerkki</translation>
    </message>
    <message>
      <source>CJK Double Left</source>
      <comment>
Unicode 0x300E</comment>
      <translation type="unfinished" >CJK vasen kokolainausmerkki</translation>
    </message>
    <message>
      <source>CJK Double Right</source>
      <comment>
Unicode 0x300F</comment>
      <translation type="unfinished" >CJK oikea kokolainausmerkki</translation>
    </message>
    <message>
      <source>&amp;400%</source>
      <translation type="unfinished" >&amp;200 % {400%?}</translation>
    </message>
    <message>
      <source>Insert &amp;Text Frame</source>
      <translation>Lisää &amp;tekstikehys</translation>
    </message>
    <message>
      <source>Insert &amp;Image Frame</source>
      <translation>Lisää &amp;kuvakehys</translation>
    </message>
    <message>
      <source>Insert T&amp;able</source>
      <translation>Lisää t&amp;aulukko</translation>
    </message>
    <message>
      <source>Insert &amp;Shape</source>
      <translation>Lisää k&amp;uvio</translation>
    </message>
    <message>
      <source>Insert &amp;Polygon</source>
      <translation>Lisää &amp;monikulmio</translation>
    </message>
    <message>
      <source>Insert &amp;Line</source>
      <translation>Lisää &amp;viiva</translation>
    </message>
    <message>
      <source>Insert &amp;Bezier Curve</source>
      <translation>Lisää &amp;bezier-viiva</translation>
    </message>
    <message>
      <source>Insert &amp;Freehand Line</source>
      <translation>Lisää va&amp;paa viiva</translation>
    </message>
    <message>
      <source>Scribus Homepage</source>
      <translation>Scribuksen kotisivu</translation>
    </message>
    <message>
      <source>Scribus Online Documentation</source>
      <translation>Scribuksen dokumentaatio verkossa</translation>
    </message>
    <message>
      <source>Scribus Wiki</source>
      <translation>Scribus-wiki</translation>
    </message>
    <message>
      <source>Getting Started with Scribus</source>
      <translation>Alkuun Scribuksen kanssa</translation>
    </message>
    <message>
      <source>Show Context Menu</source>
      <translation>Näytä kontekstivalikko</translation>
    </message>
    <message>
      <source>&amp;Manage Images</source>
      <translation>&amp;Kuvien hallinta</translation>
    </message>
    <message>
      <source>&amp;About Plugins</source>
      <translation>&amp;Tietoja liitännäisistä</translation>
    </message>
    <message>
      <source>Insert Unicode Character Begin Sequence</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Get Vector File...</source>
      <translation>Hae vektoritiedosto...</translation>
    </message>
    <message>
      <source>Advanced Select All...</source>
      <translation type="unfinished" >Valitse kaikki....</translation>
    </message>
    <message>
      <source>Edit Source...</source>
      <translation>Muokkaa lähdettä...</translation>
    </message>
    <message>
      <source>Replace Colors...</source>
      <translation>Korvaa värit...</translation>
    </message>
    <message>
      <source>Rulers Relative to Page</source>
      <translation type="unfinished" >Viivaimet suhteessa sivuun</translation>
    </message>
    <message>
      <source>Insert &amp;Render Frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Check for Updates</source>
      <translation>Tarkista päivitykset</translation>
    </message>
    <message>
      <source>Number of Pages</source>
      <translation>Sivumäärä</translation>
    </message>
    <message>
      <source>Adjust Image to Frame</source>
      <translation>Sovita kuva kehykseen</translation>
    </message>
  </context>
  <context>
    <name>AdjustCmsDialog</name>
    <message>
      <source>CMS Settings</source>
      <translation>CMS-asetukset</translation>
    </message>
  </context>
  <context>
    <name>AlignDistribute</name>
    <message>
      <source>Align</source>
      <translation>Tasaa</translation>
    </message>
    <message>
      <source>&amp;Selected Guide:</source>
      <translation>&amp;Valittu apuviiva:</translation>
    </message>
    <message>
      <source>&amp;Relative To:</source>
      <translation>&amp;Suhteessa:</translation>
    </message>
    <message>
      <source>...</source>
      <translation>...</translation>
    </message>
    <message>
      <source>Distribute</source>
      <translation>Jaa</translation>
    </message>
    <message>
      <source>&amp;Distance:</source>
      <translation>&amp;Etäisyys:</translation>
    </message>
  </context>
  <context>
    <name>AlignDistributePalette</name>
    <message>
      <source>Align and Distribute</source>
      <translation>Jaa ja tasaa</translation>
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
      <source>Align bottoms</source>
      <translation>Tasaa alareunat</translation>
    </message>
    <message>
      <source>Align right sides</source>
      <translation>Tasaa oikeat reunat</translation>
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
      <source>Align tops</source>
      <translation>Tasaa yläreunat</translation>
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
    <message>
      <source>Align right sides of items to left side of anchor</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Align left sides of items to right side of anchor</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Align tops of items to bottom of anchor</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Align bottoms of items to top of anchor</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Make horizontal gaps between items equal</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Make horizontal gaps between items equal to the value specified</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Make vertical gaps between items equal</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Make vertical gaps between items equal to the value specified</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Make horizontal gaps between items and sides of page equal</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Make vertical gaps between items and the top and bottom of page margins equal</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Make horizontal gaps between items and sides of page margins equal</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Make vertical gaps between items and the top and bottom of page equal</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Align</source>
      <translation>Tasaa</translation>
    </message>
    <message>
      <source>Distribute</source>
      <translation>Jaa</translation>
    </message>
    <message>
      <source>&lt;qt>Align relative to the:&lt;ul>&lt;li>First selected item&lt;/li>&lt;li>Second Selected Item&lt;/li>&lt;li>The current page&lt;/li>&lt;li>The margins of the current page&lt;/li>&lt;li>A Guide&lt;/li>&lt;li>The selection&lt;/ul>&lt;/qt></source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The location of the selected guide to align to</source>
      <translation type="unfinished" />
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
      <source>On Blur</source>
      <translation>On Blur</translation>
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
      <translation>PDF 1.3:n kanssa käytettävä fontti</translation>
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
      <source>None</source>
      <comment>highlight</comment>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>None</source>
      <comment>action</comment>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Tooltip:</source>
      <translation>Työkaluvihje:</translation>
    </message>
    <message>
      <source>Do Not Export Value</source>
      <translation>Älä vie arvoa</translation>
    </message>
    <message>
      <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps *.epsi);;All Files (*)</source>
      <translation>Kuvat (*.tif *.png *.jpg *.xpm);;PostScript (*.eps *.epsi);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Images (*.tif *.png *.jpg *.xpm);;%1;;All Files (*)</source>
      <translation>Kuvat (*.tif *.png *.jpg *.xpm);;%1;;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Export absolute Filename</source>
      <translation type="unfinished" />
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
      <translation>&amp;X</translation>
    </message>
    <message>
      <source>&amp;Y-Pos:</source>
      <translation>&amp;Y:</translation>
    </message>
    <message>
      <source>%1;;All Files (*)</source>
      <translation>%1;;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Export absolute Filename</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>ApplyMasterPageDialog</name>
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
      <source>Apply the selected master page to even, odd or all pages within the following range</source>
      <translation>Käytä valittua mallisivua parillisilla, parittomilla tai kaikilla sivuilla</translation>
    </message>
    <message>
      <source>Possible Hyphenation</source>
      <translation>Mahdollinen tavutus</translation>
    </message>
  </context>
  <context>
    <name>AspellPlugin</name>
    <message>
      <source>Spell-checking support</source>
      <translation>Oikolukutuki</translation>
    </message>
    <message>
      <source>Adds support for spell-checking via aspell. Languages can be chosen from among the installed aspell dictionaries, and spell-checking can be done on the fly, or on selected text.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>0.1</source>
      <translation>0.1</translation>
    </message>
    <message>
      <source>Spell Checker</source>
      <translation>Oikoluku</translation>
    </message>
  </context>
  <context>
    <name>AspellPluginBase</name>
    <message>
      <source>Spell Check</source>
      <translation>Oikoluku</translation>
    </message>
    <message>
      <source>Replacement:</source>
      <translation>Korvaava:</translation>
    </message>
    <message>
      <source>&amp;Add Word</source>
      <translation>Lisää s&amp;ana</translation>
    </message>
    <message>
      <source>&amp;Change</source>
      <translation>&amp;Muuta</translation>
    </message>
    <message>
      <source>Not in Dictionary:</source>
      <translation>Ei ole sanastossa:</translation>
    </message>
    <message>
      <source>Word that was not found in the active dictionary</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Replacement text for the current word that was not found in the dictionary</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Active Dictionary: </source>
      <translation>Sanasto käytössä:</translation>
    </message>
    <message>
      <source>The currently active dictionary. Scribus uses aspell for dictionary support.&lt;br />If you require updated or more dictionaries, you should install them via your system's installation system or package manager.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Ignore the current text not found in the active dictionary</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Ignore</source>
      <translation>&amp;Ohita</translation>
    </message>
    <message>
      <source>Ignore all occurrences of the current text not found in the active dictionary</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>I&amp;gnore All</source>
      <translation>O&amp;hita kaikki</translation>
    </message>
    <message>
      <source>Add the current word to the your personal spelling dictionary for future use</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Change the current word that was not found to that shown in the replacement entry field</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Change all occurrences of the current word in the text that was not found to that shown in the replacement entry field</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Change A&amp;ll</source>
      <translation>Muu&amp;ta kaikki</translation>
    </message>
    <message>
      <source>Close</source>
      <translation>Sulje</translation>
    </message>
  </context>
  <context>
    <name>AspellPluginImpl</name>
    <message>
      <source>Loaded </source>
      <translation>Ladattu </translation>
    </message>
    <message>
      <source>default </source>
      <translation>oletus </translation>
    </message>
    <message>
      <source> aspell dictionary.</source>
      <translation> aspell-sanasto</translation>
    </message>
    <message>
      <source>aspellplugin (AspellPluginImpl::AspellPluginImpl): Error in aspell speller configuration.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>aspellplugin (AspellPluginImpl::AspellPluginImpl): Error in creating aspell speller.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>AspellPluginImpl::on_fskipAllBtn_clicked(): Unable to skip all instances of &quot;</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source> by adding it to the session list.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>AspellPluginImpl::on_faddWordBtn_clicked(): Unable to add word to personal list.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Spelling check complete</source>
      <translation>Oikoluku suoritettu</translation>
    </message>
    <message>
      <source>Spell Checker</source>
      <translation>Oikoluku</translation>
    </message>
  </context>
  <context>
    <name>AutoformButtonGroup</name>
    <message>
      <source>Arrows</source>
      <translation>Nuolet</translation>
    </message>
    <message>
      <source>Flow Chart</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Jigsaw</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Specials</source>
      <translation type="unfinished" >Erikoismerkit</translation>
    </message>
    <message>
      <source>Default Shapes</source>
      <translation>Oletusmuodot</translation>
    </message>
  </context>
  <context>
    <name>Barcode</name>
    <message>
      <source>Scribus frontend for Pure Postscript Barcode Writer</source>
      <translation>Scribus-liitäntä Pure Postscript Barcode -ohjelmaan</translation>
    </message>
    <message>
      <source>&amp;Barcode...</source>
      <translation>&amp;Viivakoodi...</translation>
    </message>
  </context>
  <context>
    <name>BarcodeGenerator</name>
    <message>
      <source>Error opening file: %1</source>
      <translation>Virhe avattaessa tiedostoa: %1</translation>
    </message>
    <message>
      <source>Barcode incomplete</source>
      <translation>Viivakoodi ei ole täydellinen</translation>
    </message>
    <message>
      <source>12 or 13 digits</source>
      <translation>12 tai 13 numeroa</translation>
    </message>
    <message>
      <source>8 digits</source>
      <translation>8 numeroa</translation>
    </message>
    <message>
      <source>11 or 12 digits</source>
      <translation>11 tai 12 numeroa</translation>
    </message>
    <message>
      <source>7 or 8 digits</source>
      <translation>7 tai 8 numeroa</translation>
    </message>
    <message>
      <source>5 digits</source>
      <translation>5 numeroa</translation>
    </message>
    <message>
      <source>2 digits</source>
      <translation>2 numeroa</translation>
    </message>
    <message>
      <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
      <translation>Kirjainmerkkejä, numeroita ja symboleja: -, *, $, /, +, %</translation>
    </message>
    <message>
      <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
      <translation>Valinnainen määrä ASCII- ja funktio-merkkejä, aloitus oikealla aloitusmerkillä alkuperäiselle merkkiryhmälle. UCC/EAN-128:lla täytyy olla FNC 1 -merkki heti aloitusmerkin jälkeen</translation>
    </message>
    <message>
      <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
      <translation>Numeroita ja symboleja: -, $, :, /, +, A, B, C, D</translation>
    </message>
    <message>
      <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
      <translation>Numeroita. ITF-14 käsittää 14 merkkiä eikä sisällä tarkastusmerkkiä</translation>
    </message>
    <message>
      <source>Variable number of digits</source>
      <translation>Numeroita</translation>
    </message>
    <message>
      <source>Variable number of digits and capital letters</source>
      <translation>Numeroita ja versaalikirjaimia</translation>
    </message>
    <message>
      <source>Variable number of hexadecimal characters</source>
      <translation>Hex-merkkejä</translation>
    </message>
    <message>
      <source>12 or 13 digits with dashes. The legacy ISBN-10 format accepts 9 or 10 digits with dashes, but this standard was depreciated for public use after 1st January 2007. (Note: To convert an old ISBN-10 to a new ISBN-13, prefix 978- to the first 9 digits, e.g. 1-56592-479-7 -> 978-1-56592-479. The final check-digit will be calculated automatically.)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select Type</source>
      <translation>Valitse tyyppi</translation>
    </message>
    <message>
      <source>Select Barcode Type</source>
      <translation>Valitse viivakoodin tyyppi</translation>
    </message>
  </context>
  <context>
    <name>BarcodeGeneratorBase</name>
    <message>
      <source>&amp;Type:</source>
      <translation>&amp;Tyyppi:</translation>
    </message>
    <message>
      <source>Select one of the available barcode type here</source>
      <translation>Valitse viivakoodityyppi</translation>
    </message>
    <message>
      <source>The numeric representation of the code itself. See the help message below</source>
      <translation>Koodin numeerinen esitys. Lisää tietoa ohjeessa alapuolella.</translation>
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
    <message>
      <source>Co&amp;de:</source>
      <translation>&amp;Koodi:</translation>
    </message>
    <message>
      <source>I&amp;nclude checksum</source>
      <translation>&amp;Näytä tarkastussumma</translation>
    </message>
    <message>
      <source>Alt+N</source>
      <translation>Alt+N</translation>
    </message>
    <message>
      <source>Generate and include a checksum in barcode</source>
      <translation>Luo ja lisää tarkastussumma viivakoodiin</translation>
    </message>
    <message>
      <source>Incl&amp;ude checksum digit</source>
      <translation>&amp;Lisää tarkastussumman numero</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation>Alt+U</translation>
    </message>
    <message>
      <source>Include the checksum digit in the barcode text</source>
      <translation>Sisällytä tarkastussumman numero viivakoodin tekstiin</translation>
    </message>
    <message>
      <source>Insert Barcode</source>
      <translation>Lisää viivakoodi</translation>
    </message>
    <message>
      <source>Format</source>
      <translation type="unfinished" >Muotoilu</translation>
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
      <source>&amp;Name:</source>
      <translation>&amp;Nimi:</translation>
    </message>
    <message>
      <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
      <translation>Nimi &quot;%1&quot; on jo olemassa. 
Valitse toinen nimi.</translation>
    </message>
    <message>
      <source>Choose a Scrapbook Directory</source>
      <translation>Valitse leikekirjahakemisto</translation>
    </message>
    <message>
      <source>Choose a Directory</source>
      <translation>Valitse hakemisto</translation>
    </message>
    <message>
      <source>Choose a scrapbook file to import</source>
      <translation>Valitse leikekirjatiedosto</translation>
    </message>
    <message>
      <source>Main</source>
      <translation>Oletus</translation>
    </message>
    <message>
      <source>Copied Items</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Copy To:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Move To:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Create a new scrapbook page</source>
      <translation>Luo uusi leikekirjan sivu</translation>
    </message>
    <message>
      <source>Load an existing scrapbook</source>
      <translation>Lataa leikekirja</translation>
    </message>
    <message>
      <source>Save the selected scrapbook</source>
      <translation>Tallenna valittu leikekirja</translation>
    </message>
    <message>
      <source>Import an scrapbook file from Scribus &lt;=1.3.2</source>
      <translation>Tuo Scribus &lt;= 1.3.2 -leikekirjatiedosto</translation>
    </message>
    <message>
      <source>Close the selected scrapbook</source>
      <translation>Sulje valittu leikekirja</translation>
    </message>
    <message>
      <source>New Name</source>
      <translation>Uusi nimi</translation>
    </message>
    <message>
      <source>Scrapbook (*.scs *.SCS)</source>
      <translation>Leikekirja (*.scs *.SCS)</translation>
    </message>
    <message>
      <source>Paste to Page</source>
      <translation>Liitä sivulle</translation>
    </message>
    <message>
      <source>Save as...</source>
      <translation>Tallenna nimellä...</translation>
    </message>
    <message>
      <source>Close</source>
      <translation>Sulje</translation>
    </message>
    <message>
      <source>Delete Contents</source>
      <translation>Poista sisältö</translation>
    </message>
    <message>
      <source>Do you really want to delete all entries?</source>
      <translation>Haluatko poistaa kaikki?</translation>
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
  </context>
  <context>
    <name>CMSPrefsBase</name>
    <message>
      <source>Form</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Activate Color Management</source>
      <translation>Käytä &amp;värien hallintaa</translation>
    </message>
    <message>
      <source>System Profiles</source>
      <translation>Järjestelmän profiilit</translation>
    </message>
    <message>
      <source>&amp;RGB Images:</source>
      <translation>&amp;RGB-kuvat:</translation>
    </message>
    <message>
      <source>Default color profile for imported RGB images</source>
      <translation>Oletusväriprofiili tuoduille RGB-kuville</translation>
    </message>
    <message>
      <source>&amp;CMYK Images:</source>
      <translation>&amp;CMYK-kuvat:</translation>
    </message>
    <message>
      <source>Default color profile for imported CMYK images</source>
      <translation>Oletusväriprofiili tuoduille CMYK-kuville</translation>
    </message>
    <message>
      <source>&amp;RGB Solid Colors:</source>
      <translation>&amp;RGB-värit:</translation>
    </message>
    <message>
      <source>Default color profile for solid RGB colors on the page</source>
      <translation>Oletusprofiili sivun RGB-väreille</translation>
    </message>
    <message>
      <source>&amp;CMYK Solid Colors:</source>
      <translation>&amp;CMYK-värit</translation>
    </message>
    <message>
      <source>Default color profile for solid CMYK colors on the page</source>
      <translation>Oletusprofiili sivun CMYK-väreille</translation>
    </message>
    <message>
      <source>&amp;Monitor:</source>
      <translation>&amp;Näyttö:</translation>
    </message>
    <message>
      <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
      <translation type="unfinished" >Itse tekemäsi tai näyttösi valmistajan toimittama väriprofiili. 
Profiilin tulee olla näytöllesi tarkoitettu ei yleinen väriprofiili kuten sRGB.</translation>
    </message>
    <message>
      <source>P&amp;rinter:</source>
      <translation>Tu&amp;lostin:</translation>
    </message>
    <message>
      <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
      <translation type="unfinished" >Tulostimesi valmistajan toimittama väriprofiili tulostimellesi. 
Profiilin tulee olla tulostimellesi tarkoitettu ei yleinen väriprofiili kuten sRGB.</translation>
    </message>
    <message>
      <source>Rendering Intents</source>
      <translation>Sovitustavat</translation>
    </message>
    <message>
      <source>Images:</source>
      <translation>Kuvat:</translation>
    </message>
    <message>
      <source>Default rendering intent for images. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
      <translation type="unfinished" >Oletuksena käytetty sovitustapa kuville. Relative Colorimetric tai Perceptual tulisi valita, ellet tiedä haluavasi jotain muuta.</translation>
    </message>
    <message>
      <source>Sol&amp;id Colors:</source>
      <translation>V&amp;ärit:</translation>
    </message>
    <message>
      <source>Default rendering intent for solid colors. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
      <translation type="unfinished" >Oletuksena käytetty sovitustapa tasaisille väreille. Relative Colorimetric tai Perceptual tulisi valita, ellet tiedä haluavasi jotain toista.</translation>
    </message>
    <message>
      <source>Enable 'soft proofing' of how your document colors will print,
based on the chosen printer profile.</source>
      <translation type="unfinished" >Mahdollistaa tulosteen värien jäljittelyn näytöllä 
perustuen käytettävään väriprofiiliin.</translation>
    </message>
    <message>
      <source>Sim&amp;ulate Printer on the Screen</source>
      <translation>&amp;Jäljittele tulostinta näytöllä</translation>
    </message>
    <message>
      <source>Simulate a full color managed environment :
all colors, rgb or cmyk, are converted to printer color space.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Convert all colors to printer space</source>
      <translation>Muunna kaikki värit tulostimen väriavaruuteen</translation>
    </message>
    <message>
      <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
      <translation type="unfinished" >Toiminto näyttää värit, jotka saattavat tulostua väärin. 
Toiminnon käyttö vaatii tarkat profiilit ja se tulee huomioida ainoastaan varoituksena.</translation>
    </message>
    <message>
      <source>Mark Colors out of &amp;Gamut</source>
      <translation>&amp;Merkkaa asteikon ulkopuoliset värit</translation>
    </message>
    <message>
      <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
      <translation type="unfinished" >Tumman pään tasaus on keino parantaa kuvien kontrastia. 
On suositeltavaa että valitsen tämän, jos asiakirjasi sisältää kuvia.</translation>
    </message>
    <message>
      <source>Use &amp;Blackpoint Compensation</source>
      <translation>Käytä &amp;Tumman pään tasausta (BPC)</translation>
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
      <source>&amp;Name:</source>
      <translation>&amp;Nimi:</translation>
    </message>
    <message>
      <source>Color &amp;Model</source>
      <translation>&amp;Värimalli</translation>
    </message>
    <message>
      <source>Is Spot Color</source>
      <translation>On lisäväri</translation>
    </message>
    <message>
      <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
      <translation>Nimi &quot;%1&quot; on varattu läpinäkyvyydelle. 
Sitä ei voi käyttää.</translation>
    </message>
    <message>
      <source>Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks.</source>
      <translation>Valitsemalla tämä väristä tehdään lisäväri (spot color), jolloin painoprosessiin lisätään uusi laatta tai erottelu kutakin lisäväriä varten. Lisävärejä käytetään useimmiten, kun esimerkiksi logon väri halutaan saada juuri oikeaksi eikä sitä voida esittää CMYK-musteilla. Metalli- ja hohtovärit ovat esimerkiksi tällaisia värejä.</translation>
    </message>
    <message>
      <source>The name of the color already exists,
please choose another one.</source>
      <translation>Valitsemasi nimi värille on jo käytössä. 
Valitse toinen nimi.</translation>
    </message>
    <message>
      <source>If color management is enabled, a triangle warning indicator is a warning that the color maybe outside of the color gamut of the current printer profile selected. What this means is the color may not print exactly as indicated on screen. More hints about gamut warnings are in the online help under Color Management.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>You cannot create a color without a name
Please give it a name</source>
      <translation>Väriä ei voi luoda ilman nimeä. Anna sille nimi.</translation>
    </message>
    <message>
      <source>HSV Color Map</source>
      <translation>HSV-värikartta</translation>
    </message>
  </context>
  <context>
    <name>CWDialog</name>
    <message>
      <source>Merging colors</source>
      <translation type="unfinished" >Lisätään värejä</translation>
    </message>
    <message>
      <source>Error: </source>
      <translation>Virhe: </translation>
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
      <source>Now opening the color manager.</source>
      <translation>Avataan värien hallinta.</translation>
    </message>
    <message>
      <source>Color Merging</source>
      <translation type="unfinished" >Värien lisäys</translation>
    </message>
    <message>
      <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
      <translation type="unfinished" >Haluttua väriä ei löydy. Olet luultavasti valinnut mustan, harmaan tai valkoisen. Tätä väriä ei voida prosessoida.</translation>
    </message>
    <message>
      <source>Color Wheel</source>
      <translation>Väripyörä</translation>
    </message>
    <message>
      <source>Click the wheel to get the base color. Its color model depends on the chosen tab.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Result Colors</source>
      <translation type="unfinished" />
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
      <source>Colors of your chosen color scheme.</source>
      <translation>Valitsemasi värimallin värit</translation>
    </message>
    <message>
      <source>Color Scheme Method</source>
      <translation type="unfinished" >Värimallin </translation>
    </message>
    <message>
      <source>Angle:</source>
      <translation type="unfinished" >Kulma:</translation>
    </message>
    <message>
      <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
      <translation type="unfinished" >Erotus valitun arvon ja laskettujen välillä. Lisää tietoa löytyy dokumentaatiosta.</translation>
    </message>
    <message>
      <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
      <translation type="unfinished" >Valitse jokin tyyli luoda väriteema. Lisää tietoa löytyy dokumentaatiosta.</translation>
    </message>
    <message>
      <source>Merge created colors into the document colors</source>
      <translation type="unfinished" >Lisää luodut värit asiakirjan väreihin</translation>
    </message>
    <message>
      <source>&amp;Merge</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Alt+M</source>
      <translation type="unfinished" >Alt+M</translation>
    </message>
    <message>
      <source>Replace created colors in the document colors</source>
      <translation type="unfinished" >Korvaa luodut värit asiakirjan väreissä</translation>
    </message>
    <message>
      <source>&amp;Replace</source>
      <translation type="unfinished" >&amp;Korvaa</translation>
    </message>
    <message>
      <source>Alt+R</source>
      <translation type="unfinished" >Alt+R</translation>
    </message>
    <message>
      <source>Leave colors untouched</source>
      <translation type="unfinished" >Älä tee muutoksia väreihin</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation type="unfinished" >&amp;Peruuta</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation type="unfinished" >Alt+C</translation>
    </message>
    <message>
      <source>Preview:</source>
      <translation type="unfinished" >Esikatselu:</translation>
    </message>
    <message>
      <source>Sample color scheme.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Simulate common vision defects here. Select type of the defect.</source>
      <translation type="unfinished" >Mallinna yleisiä näön rajoituksia. Valitse rajoituksen tyyppi.</translation>
    </message>
    <message>
      <source>Vision Defect Type:</source>
      <translation type="unfinished" >Näkörajoitteisuuden tyyppi</translation>
    </message>
    <message>
      <source>C:</source>
      <translation type="unfinished" >C:</translation>
    </message>
    <message>
      <source> %</source>
      <translation type="unfinished" > %</translation>
    </message>
    <message>
      <source>M:</source>
      <translation type="unfinished" >M:</translation>
    </message>
    <message>
      <source>Y:</source>
      <translation type="unfinished" >Y:</translation>
    </message>
    <message>
      <source>K:</source>
      <translation type="unfinished" >K:</translation>
    </message>
    <message>
      <source>RGB:</source>
      <translation type="unfinished" >RGB:</translation>
    </message>
    <message>
      <source>HSV:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>R:</source>
      <translation type="unfinished" >R:</translation>
    </message>
    <message>
      <source>G:</source>
      <translation type="unfinished" >G:</translation>
    </message>
    <message>
      <source>B:</source>
      <translation type="unfinished" >B:</translation>
    </message>
    <message>
      <source>CMYK:</source>
      <translation type="unfinished" >CMYK:</translation>
    </message>
    <message>
      <source>H:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>S:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>V:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Document</source>
      <translation type="unfinished" >Asiakirja</translation>
    </message>
  </context>
  <context>
    <name>CharSelect</name>
    <message>
      <source>Font:</source>
      <translation>Fontti:</translation>
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
      <source>Insert the characters at the cursor in the text</source>
      <translation>Lisää merkit kursorin kohdalle tekstiin</translation>
    </message>
    <message>
      <source>Delete the current selection(s).</source>
      <translation>Poista valinta.</translation>
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
    <message>
      <source>Scribus Char Palette (*.ucp);;All Files (*)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Enhanced Palette</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Quick Palette</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Choose a filename to open</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Error</source>
      <translation type="unfinished" >Virhe</translation>
    </message>
    <message>
      <source>Error reading file %1 - file is corrupted propably.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Cannot write file %1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Character Palette</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Empty the Palette?</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>You will remove all characters from this palette. Are you sure?</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Hide/Show Enhanced Palette</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Save Quick Character Palette</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>You can see a thumbnail if you press and hold down the right mouse button.
The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>CharStyleComboBox</name>
    <message>
      <source>No Style</source>
      <translation type="unfinished" >Ei tyyliä</translation>
    </message>
  </context>
  <context>
    <name>CharTableView</name>
    <message>
      <source>Delete</source>
      <translation type="unfinished" >Poista</translation>
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
    <message>
      <source>OK</source>
      <translation>OK</translation>
    </message>
    <message>
      <source>Transparency used</source>
      <translation>Läpinäkyvyyttä on käytetty</translation>
    </message>
    <message>
      <source>Blendmode used</source>
      <translation>Sekoitustiloja on käytetty</translation>
    </message>
    <message>
      <source>Layer &quot;%1&quot;</source>
      <translation>Taso &quot;1&quot;</translation>
    </message>
    <message>
      <source>Check again</source>
      <translation>Tarkasta uudelleen</translation>
    </message>
    <message>
      <source>Image resolution below %1 DPI, currently %2 x %3 DPI</source>
      <translation>Kuvan resoluutio on alle %1 DPI, tällä hetkellä %2x%3 DPI</translation>
    </message>
    <message>
      <source>Image resolution above %1 DPI, currently %2 x %3 DPI</source>
      <translation>Kuvan resoluutio on yli %1 DPI, tällä hetkellä %2x%3 DPI</translation>
    </message>
    <message>
      <source>Image is GIF</source>
      <translation>Kuva on GIF</translation>
    </message>
    <message>
      <source>Annotation uses a non TrueType font</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Preflight profile to base the report generation on. Options can be set in Document Setup or Preferences</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Ignore found errors and continue to export or print. Be sure to understand the errors you are ignoring before continuing.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Rerun the document scan to check corrections you may have made</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Empty Image Frame</source>
      <translation type="unfinished" />
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
      <source>Copy of %1</source>
      <translation>Kopio-%1</translation>
    </message>
    <message>
      <source>New Color</source>
      <translation>Uusi väri</translation>
    </message>
    <message>
      <source>Information</source>
      <translation type="unfinished" >Tiedot</translation>
    </message>
    <message>
      <source>The file %1 does not contain colors which can be imported.
If the file was a PostScript-based, try to import it with File -&amp;gt; Import. 
Not all files have DSC conformant comments where the color descriptions are located.
 This prevents importing colors from some files.
See the Edit Colors section of the documentation for more details.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>If color management is enabled, a triangle warning indicator is a warning that the color maybe outside of the color gamut of the current printer profile selected. What this means is the color may not print exactly as indicated on screen. Spot colors are indicated by a red circle. More hints about gamut warnings are in the online help under Color Management. Registration colors will have a registration mark next to the color. Use Registration only for printers marks and crop marks.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>All Supported Formats (%1);;Documents (%2);;Other Files (%3);;All Files (*)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Import Colors</source>
      <translation type="unfinished" />
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
    <message>
      <source>Custom</source>
      <comment>CommonStrings, custom page size</comment>
      <translation>Oma</translation>
    </message>
    <message>
      <source>Single Page</source>
      <translation>Yksittäinen sivu</translation>
    </message>
    <message>
      <source>Double Sided</source>
      <translation>Aukeama</translation>
    </message>
    <message>
      <source>3-Fold</source>
      <translation>Kolme sivua</translation>
    </message>
    <message>
      <source>4-Fold</source>
      <translation>Neljä sivua</translation>
    </message>
    <message>
      <source>None</source>
      <comment>color name</comment>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Monday</source>
      <translation>Maanantai</translation>
    </message>
    <message>
      <source>Tuesday</source>
      <translation>Tiistai</translation>
    </message>
    <message>
      <source>Wednesday</source>
      <translation>Keskiviikko</translation>
    </message>
    <message>
      <source>Thursday</source>
      <translation>Torstai</translation>
    </message>
    <message>
      <source>Friday</source>
      <translation>Perjantai</translation>
    </message>
    <message>
      <source>Saturday</source>
      <translation>Lauantai</translation>
    </message>
    <message>
      <source>Sunday</source>
      <translation>Sunnuntai</translation>
    </message>
    <message>
      <source>January</source>
      <translation>Tammikuu</translation>
    </message>
    <message>
      <source>February</source>
      <translation>Helmikuu</translation>
    </message>
    <message>
      <source>March</source>
      <translation>Maaliskuu</translation>
    </message>
    <message>
      <source>April</source>
      <translation>Huhtikuu</translation>
    </message>
    <message>
      <source>May</source>
      <translation>Toukokuu</translation>
    </message>
    <message>
      <source>June</source>
      <translation>Kesäkuu</translation>
    </message>
    <message>
      <source>July</source>
      <translation>Heinäkuu</translation>
    </message>
    <message>
      <source>August</source>
      <translation>Elokuu</translation>
    </message>
    <message>
      <source>September</source>
      <translation>Syyskuu</translation>
    </message>
    <message>
      <source>October</source>
      <translation>Lokakuu</translation>
    </message>
    <message>
      <source>November</source>
      <translation>Marraskuu</translation>
    </message>
    <message>
      <source>December</source>
      <translation>Joulukuu</translation>
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
      <source>&amp;Yes</source>
      <translation>&amp;Yes</translation>
    </message>
    <message>
      <source>&amp;No</source>
      <translation>&amp;Ei</translation>
    </message>
    <message>
      <source>Left Page</source>
      <comment>
Left page location</comment>
      <translation type="unfinished" >Vasen sivu</translation>
    </message>
    <message>
      <source>Middle</source>
      <comment>
Middle page location</comment>
      <translation type="unfinished" >Keskimmäinen</translation>
    </message>
    <message>
      <source>Middle Left</source>
      <comment>
Middle Left page location</comment>
      <translation type="unfinished" >Keskimmäinen vasen</translation>
    </message>
    <message>
      <source>Middle Right</source>
      <comment>
Middle Right page location</comment>
      <translation type="unfinished" >Keskimmäinen oikea</translation>
    </message>
    <message>
      <source>Right Page</source>
      <comment>
Right page location</comment>
      <translation type="unfinished" >Oikea sivu</translation>
    </message>
    <message>
      <source>Normal</source>
      <comment>Default single master page</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Normal Left</source>
      <comment>
Default left master page</comment>
      <translation type="unfinished" >Normaali vasen</translation>
    </message>
    <message>
      <source>Normal Middle</source>
      <comment>
Default middle master page</comment>
      <translation type="unfinished" >Normaali keski</translation>
    </message>
    <message>
      <source>Normal Right</source>
      <comment>
Default right master page</comment>
      <translation type="unfinished" >Normaali oikea</translation>
    </message>
    <message>
      <source>Normal Vision</source>
      <comment>
Color Blindness - Normal Vision</comment>
      <translation type="unfinished" >Normaali näkö</translation>
    </message>
    <message>
      <source>Protanopia (Red)</source>
      <comment>
Color Blindness - Red Color Blind</comment>
      <translation type="unfinished" >Punasokeus</translation>
    </message>
    <message>
      <source>Deuteranopia (Green)</source>
      <comment>
Color Blindness - Greed Color Blind</comment>
      <translation type="unfinished" >Vihersokeus</translation>
    </message>
    <message>
      <source>Tritanopia (Blue)</source>
      <comment>
Color Blindness - Blue Color Blind</comment>
      <translation type="unfinished" >Sinisokeus</translation>
    </message>
    <message>
      <source>Full Color Blindness</source>
      <comment>
Color Blindness - Full Color Blindness</comment>
      <translation type="unfinished" >Täysi värisokeus</translation>
    </message>
    <message>
      <source>Custom: </source>
      <comment>
Custom Tab Fill Option</comment>
      <translation type="unfinished" >Oma: </translation>
    </message>
    <message>
      <source>None</source>
      <translation type="unfinished" >Ei mitään</translation>
    </message>
    <message>
      <source>Solid Line</source>
      <translation type="unfinished" >Yhtenäinen viiva</translation>
    </message>
    <message>
      <source>Dashed Line</source>
      <translation type="unfinished" >Katkoviiva</translation>
    </message>
    <message>
      <source>Dotted Line</source>
      <translation type="unfinished" >Pisteviiva</translation>
    </message>
    <message>
      <source>Dash Dot Line</source>
      <translation type="unfinished" >Viiva-piste-viiva</translation>
    </message>
    <message>
      <source>Dash Dot Dot Line</source>
      <translation type="unfinished" >Viiva-piste-piste-viiva</translation>
    </message>
    <message>
      <source>None</source>
      <comment>
Optical Margin Setting</comment>
      <translation type="unfinished" >Ei mitään</translation>
    </message>
    <message>
      <source>Left Protruding</source>
      <comment>Optical Margin Setting</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Right Protruding</source>
      <comment>Optical Margin Setting</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Left Hanging Punctuation</source>
      <comment>Optical Margin Setting</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Right Hanging Punctuation</source>
      <comment>Optical Margin Setting</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Default</source>
      <comment>Optical Margin Setting</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Min. Word Tracking</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Max. Word Tracking</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Min. Glyph Extension</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Max. Glyph Extension</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>RGB</source>
      <comment>
Colorspace</comment>
      <translation type="unfinished" >RGB</translation>
    </message>
    <message>
      <source>CMYK</source>
      <comment>
Colorspace</comment>
      <translation type="unfinished" >CMYK</translation>
    </message>
    <message>
      <source>Grayscale</source>
      <comment>
Colorspace</comment>
      <translation type="unfinished" >Harmaasävy</translation>
    </message>
    <message>
      <source>Duotone</source>
      <comment>
Colorspace</comment>
      <translation type="unfinished" >Kaksiväri</translation>
    </message>
    <message>
      <source>Unknown</source>
      <comment>
Colorspace (Unknown)</comment>
      <translation type="unfinished" >Tuntematon</translation>
    </message>
    <message>
      <source>PostScript</source>
      <translation type="unfinished" >PostScript</translation>
    </message>
    <message>
      <source>Text Frame</source>
      <translation type="unfinished" >Tekstikehys</translation>
    </message>
    <message>
      <source>Image Frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Line</source>
      <translation type="unfinished" >Viiva</translation>
    </message>
    <message>
      <source>Polygon</source>
      <translation type="unfinished" >Monikulmio</translation>
    </message>
    <message>
      <source>Polyline</source>
      <translation type="unfinished" >Kulmikas viiva</translation>
    </message>
    <message>
      <source>Text on a Path</source>
      <translation type="unfinished" >Teksti polulla</translation>
    </message>
    <message>
      <source>Multiple</source>
      <comment>Multiple frame types</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PDF Push Button</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PDF Text Field</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PDF Check Box</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PDF Combo Box</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PDF List Box</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PDF Text Annotation</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PDF Link Annotation</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PostScript Level 1</source>
      <translation type="unfinished" >PostScript taso 1</translation>
    </message>
    <message>
      <source>PostScript Level 2</source>
      <translation type="unfinished" >PostScript taso 2</translation>
    </message>
    <message>
      <source>PostScript Level 3</source>
      <translation type="unfinished" >PostScript taso 3</translation>
    </message>
    <message>
      <source>Windows GDI</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Render Frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Default Paragraph Style</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Default Character Style</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Default Line Style</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>ContextMenu</name>
    <message>
      <source>Preview Settings</source>
      <translation type="unfinished" >Esikatseluasetukset</translation>
    </message>
    <message>
      <source>Paste File...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Delete Page</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>CopyPageToMasterPageBase</name>
    <message>
      <source>Convert Page to Master Page</source>
      <translation type="unfinished" >Muunna sivu mallisivuksi</translation>
    </message>
    <message>
      <source>Name:</source>
      <translation type="unfinished" >Nimi:</translation>
    </message>
    <message>
      <source>Copy Applied Master Page Items</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>CopyPageToMasterPageDialog</name>
    <message>
      <source>New Master Page %1</source>
      <translation type="unfinished" >Uusi mallisivu %1</translation>
    </message>
  </context>
  <context>
    <name>Cpalette</name>
    <message>
      <source> %</source>
      <translation> %</translation>
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
      <translation>Viisto väriliuku 2</translation>
    </message>
    <message>
      <source>Diagonal Gradient</source>
      <translation>Viisto väriliuku 1</translation>
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
      <source> pt</source>
      <translation> pt</translation>
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
    <message>
      <source>Transparency Settings</source>
      <translation>Läpinäkyvyyden asetukset</translation>
    </message>
    <message>
      <source>Offsets</source>
      <translation type="unfinished" >Siirtymät</translation>
    </message>
    <message>
      <source>X:</source>
      <translation>X:</translation>
    </message>
    <message>
      <source>Y:</source>
      <translation>Y:</translation>
    </message>
    <message>
      <source>Scaling</source>
      <translation>Koon muutos</translation>
    </message>
    <message>
      <source>X-Scale:</source>
      <translation>Koon muutos X</translation>
    </message>
    <message>
      <source>Y-Scale:</source>
      <translation>Koon muutos Y</translation>
    </message>
    <message>
      <source>Rotation</source>
      <translation>Kierto</translation>
    </message>
    <message>
      <source>Angle</source>
      <translation>Kulma</translation>
    </message>
    <message>
      <source>Pattern</source>
      <translation>Kuviointi</translation>
    </message>
    <message>
      <source>Blend Mode:</source>
      <translation>Sekoitustila:</translation>
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
      <source>Multiply</source>
      <translation>Kerroin</translation>
    </message>
    <message>
      <source>Screen</source>
      <translation>Rasterointi</translation>
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
      <source>Hue</source>
      <translation>Sävy</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>Saturaatio</translation>
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
      <source>Display only used Colors</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Display all colors from the document color list, or only the already used colors</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>CreateRange</name>
    <message>
      <source>Create Range</source>
      <translation type="unfinished" >Luo sivuväli</translation>
    </message>
    <message>
      <source>Number of Pages in Document:</source>
      <translation type="unfinished" >Asiakirjan sivujen lukumäärä</translation>
    </message>
    <message>
      <source>Doc Page Range</source>
      <translation type="unfinished" >Sivuväli asiakirjassa</translation>
    </message>
    <message>
      <source>Basic Range Selection</source>
      <translation type="unfinished" >Perusasetukset välille</translation>
    </message>
    <message>
      <source>Range of Pages</source>
      <translation type="unfinished" >Sivuväli</translation>
    </message>
    <message>
      <source>De&amp;lete</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Alt+L</source>
      <translation type="unfinished" >Alt+L</translation>
    </message>
    <message>
      <source>Move &amp;Down</source>
      <translation type="unfinished" >Siirrä &amp;alas</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation type="unfinished" >Alt+D</translation>
    </message>
    <message>
      <source>Move &amp;Up</source>
      <translation type="unfinished" >Siirrä &amp;ylös</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation type="unfinished" >Alt+U</translation>
    </message>
    <message>
      <source>Add a Range of Pages</source>
      <translation type="unfinished" >Lisää sivuväli</translation>
    </message>
    <message>
      <source>Consecutive Pages</source>
      <translation type="unfinished" >Peräkkäisiä sivuja</translation>
    </message>
    <message>
      <source>Even Pages</source>
      <translation type="unfinished" >Parilliset sivut</translation>
    </message>
    <message>
      <source>From:</source>
      <translation type="unfinished" >Mistä:</translation>
    </message>
    <message>
      <source>To:</source>
      <translation type="unfinished" >Minne:</translation>
    </message>
    <message>
      <source>&amp;Add To Range</source>
      <translation type="unfinished" >&amp;Lisää joukkoon</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation type="unfinished" >Alt+A</translation>
    </message>
    <message>
      <source>Odd Pages</source>
      <translation type="unfinished" >Parittomat sivut</translation>
    </message>
    <message>
      <source>Comma Separated List</source>
      <translation type="unfinished" >Pilkuin eroteltu lista</translation>
    </message>
    <message>
      <source>Advanced Reordering</source>
      <translation type="unfinished" >Muut asettelut</translation>
    </message>
    <message>
      <source>Page Order</source>
      <translation type="unfinished" >Sivujärjestys</translation>
    </message>
    <message>
      <source>Sample Page Order:</source>
      <translation type="unfinished" >Esimerkkijärjestys:</translation>
    </message>
    <message>
      <source>Page Group Size:</source>
      <translation type="unfinished" >Sivuryhmän koko:</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation type="unfinished" >&amp;OK</translation>
    </message>
    <message>
      <source>Alt+O</source>
      <translation type="unfinished" >Alt+O</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation type="unfinished" >&amp;Peruuta</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation type="unfinished" >Alt+C</translation>
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
      <comment>delimiter</comment>
      <translation>Ei mitään</translation>
    </message>
  </context>
  <context>
    <name>CupsOptions</name>
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
    <name>CurveWidget</name>
    <message>
      <source>Open</source>
      <translation>Avaa</translation>
    </message>
    <message>
      <source>Curve Files (*.scu);;All Files (*)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Save as</source>
      <translation>Tallenna nimellä</translation>
    </message>
    <message>
      <source>Cannot write the file: 
%1</source>
      <translation>Tiedostoon ei voi kirjoittaa:
%1</translation>
    </message>
    <message>
      <source>Inverts the curve</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Resets the curve</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Switches between linear and cubic interpolation of the curve</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Loads a curve</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Saves this curve</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>CustomFDialog</name>
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
      <translation>&amp;Sisällytä fontit</translation>
    </message>
    <message>
      <source>Show Preview</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Include Color Profiles</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Show a preview and information for the selected file</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Compress the Scribus document on save</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Include fonts when collecting files for the document. Be sure to know and understand licensing information for any fonts you collect and possibly redistribute.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Include color profiles when collecting files for the document</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>DashEditor</name>
    <message>
      <source>Value:</source>
      <translation type="unfinished" >Arvo:</translation>
    </message>
    <message>
      <source>Offset:</source>
      <translation type="unfinished" >Siirtymä:</translation>
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
    <name>DocIm</name>
    <message>
      <source>Importing failed</source>
      <translation>Tuonti epäonnistui</translation>
    </message>
    <message>
      <source>Importing Word document failed 
%1</source>
      <translation>Word-asiakirjan tuonti epäonnistui \n
%1</translation>
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
      <source>Page Number Out Of Bounds</source>
      <translation>Sivunumero sallitun alueen ulkopuolella</translation>
    </message>
    <message>
      <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
      <translation>Antamasi sivunumero on asiakirjan sallittujen arvojen ulkopuolella (%1-%2).</translation>
    </message>
    <message>
      <source>&lt;b>Name:&lt;/b> Optional name for section eg. Index&lt;br/>&lt;b>Shown:&lt;/b> Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/>&lt;b>From:&lt;/b> The page index for this section to start at.&lt;br/>&lt;b>To:&lt;/b> The page index for this section to stop at.&lt;br/>&lt;b>Style:&lt;/b> Select the page number style to be used.&lt;br/>&lt;b>Start:&lt;/b> The index within the Style's range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</source>
      <translation>b>Nimi:&lt;/b> Nimi alueelle&lt;br/> 
&lt;b>Shown:&lt;/b> Näytä sivunumero alueella, jos yksi tai useampi tekstikehys sisältää sivunumerot&lt;br/> 
&lt;b>Alku:&lt;/b> Alueen ensimmäinen sivu&lt;br/> 
&lt;b>Loppu:&lt;/b> Alueen viimeinen sivu&lt;br/> 
&lt;b>Tyyli:&lt;/b> Käytettävä sivunumeron tyyli.&lt;br/> 
&lt;b>Aloitus:&lt;/b> Alueen ensimmäisen sivun numero. Jos Aloitus on &quot;2&quot; ja valittu numerotyyli on &quot;a,b,c,...&quot;, alkaa sivunumerot merkistä &quot;b&quot;.</translation>
    </message>
    <message>
      <source>Document Sections</source>
      <translation type="unfinished" >Asiakirjan osat</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation type="unfinished" >&amp;Lisää</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation type="unfinished" >Alt+A</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Alt+D</source>
      <translation type="unfinished" >Alt+D</translation>
    </message>
    <message>
      <source>Name</source>
      <translation type="unfinished" >Nimi</translation>
    </message>
    <message>
      <source>Shown</source>
      <translation type="unfinished" >Näytä</translation>
    </message>
    <message>
      <source>From</source>
      <translation type="unfinished" >Mistä</translation>
    </message>
    <message>
      <source>To</source>
      <translation type="unfinished" >Minne</translation>
    </message>
    <message>
      <source>Style</source>
      <translation type="unfinished" >Tyyli</translation>
    </message>
    <message>
      <source>Start</source>
      <translation type="unfinished" >Aloitus</translation>
    </message>
  </context>
  <context>
    <name>DocumentItemAttributes</name>
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
    <message>
      <source>None</source>
      <comment>relationship</comment>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>None</source>
      <comment>auto add</comment>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>None</source>
      <comment>types</comment>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Document Item Attributes</source>
      <translation type="unfinished" >Kohteen muuttujat</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation type="unfinished" >&amp;Lisää</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation type="unfinished" >Alt+A</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation type="unfinished" >&amp;Kopioi</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation type="unfinished" >Alt+C</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Alt+D</source>
      <translation type="unfinished" >Alt+D</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Alt+L</source>
      <translation type="unfinished" >Alt+L</translation>
    </message>
    <message>
      <source>Name</source>
      <translation type="unfinished" >Nimi</translation>
    </message>
    <message>
      <source>Type</source>
      <translation type="unfinished" >Tyyppi</translation>
    </message>
    <message>
      <source>Value</source>
      <translation type="unfinished" >Arvo</translation>
    </message>
    <message>
      <source>Parameter</source>
      <translation type="unfinished" >Parametri</translation>
    </message>
    <message>
      <source>Relationship</source>
      <translation type="unfinished" >Suhde</translation>
    </message>
    <message>
      <source>Relationship To</source>
      <translation type="unfinished" >Suhteessa</translation>
    </message>
    <message>
      <source>Auto Add To</source>
      <translation type="unfinished" >Lisää automaattisesti</translation>
    </message>
  </context>
  <context>
    <name>EPSPlug</name>
    <message>
      <source>Importing File:
%1
failed!</source>
      <translation>Tiedoston %1 tuonti epäonnistui</translation>
    </message>
    <message>
      <source>Fatal Error</source>
      <translation>Vakava virhe</translation>
    </message>
    <message>
      <source>Error</source>
      <translation>Virhe</translation>
    </message>
    <message>
      <source>Generating Items</source>
      <translation>Luodaan kohteita</translation>
    </message>
    <message>
      <source>Analyzing PostScript:</source>
      <translation>Analysoidaan Postscript-tiedostoa</translation>
    </message>
    <message>
      <source>Converting of %1 images failed!</source>
      <translation>Kuvan %1 tuominen epäonnistui</translation>
    </message>
    <message>
      <source>Group%1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Importing: %1</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>EditToolBar</name>
    <message>
      <source>Edit</source>
      <translation type="unfinished" >Muokkaa</translation>
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
    <message>
      <source>Ctrl+N</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Ctrl+Z</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Ctrl+X</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Ctrl+C</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Ctrl-V</source>
      <translation type="unfinished" />
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
      <translation>Porrasta:</translation>
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
    <message>
      <source>Color 1:</source>
      <translation>Väri 1:</translation>
    </message>
    <message>
      <source>Color 2:</source>
      <translation>Väri 2:</translation>
    </message>
    <message>
      <source>Color 3:</source>
      <translation>Väri 3:</translation>
    </message>
    <message>
      <source>Color 4:</source>
      <translation>Väri 4:</translation>
    </message>
    <message>
      <source>Duotone</source>
      <translation>Kaksiväri</translation>
    </message>
    <message>
      <source>Tritone</source>
      <translation>Kolmeväri</translation>
    </message>
    <message>
      <source>Quadtone</source>
      <translation>Neliväri</translation>
    </message>
    <message>
      <source>Curves</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>ExportBitmap</name>
    <message>
      <source>File exists. Overwrite?</source>
      <translation>Tiedosto on jo olemassa. Ylikirjoitetaanko?</translation>
    </message>
    <message>
      <source>exists already. Overwrite?</source>
      <translation>on jo olemassa. Ylikirjoitetaanko?</translation>
    </message>
    <message>
      <source>Save as Image</source>
      <translation type="unfinished" >Tallenna kuvana</translation>
    </message>
    <message>
      <source>Insufficient memory for this image size.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Error writing the output file(s).</source>
      <translation type="unfinished" >Virhe kirjoitettaessa kohdetiedostoa.</translation>
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
    <message>
      <source>Image size in Pixels</source>
      <translation>Kuvan koko pikseleinä</translation>
    </message>
    <message>
      <source>TextLabel</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The compression ratio of your images - 100% is no compression, 0% highest compression. If in doubt, use 'Automatic'</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Automatic</source>
      <translation type="unfinished" >Automaattinen</translation>
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
    <message>
      <source>Live Preview</source>
      <translation type="unfinished" />
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
      <source>Colorspace:</source>
      <translation>Väriavaruus:</translation>
    </message>
    <message>
      <source>File Format:</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>FileLoader</name>
    <message>
      <source>Some fonts used by this document have been substituted:</source>
      <translation>Jotkin asiakirjan fontit vaihdettiin:</translation>
    </message>
    <message>
      <source> was replaced by: </source>
      <translation>korvattiin tyypillä: </translation>
    </message>
  </context>
  <context>
    <name>FileToolBar</name>
    <message>
      <source>File</source>
      <translation type="unfinished" >Tiedosto</translation>
    </message>
  </context>
  <context>
    <name>FontComboH</name>
    <message>
      <source>Face:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Style:</source>
      <translation type="unfinished" >Tyyli:</translation>
    </message>
  </context>
  <context>
    <name>FontListModel</name>
    <message>
      <source>Font Name</source>
      <translation type="unfinished" >Fontin nimi</translation>
    </message>
    <message>
      <source>Use Font</source>
      <translation type="unfinished" >Käytä fonttia</translation>
    </message>
    <message>
      <source>Family</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Style</source>
      <translation type="unfinished" >Tyyli</translation>
    </message>
    <message>
      <source>Variant</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Type</source>
      <translation type="unfinished" >Tyyppi</translation>
    </message>
    <message>
      <source>Format</source>
      <translation type="unfinished" >Muotoilu</translation>
    </message>
    <message>
      <source>Embed in PostScript</source>
      <translation type="unfinished" >Sijoita PostScript-tiedostoon</translation>
    </message>
    <message>
      <source>Subset</source>
      <translation type="unfinished" >Osajoukko</translation>
    </message>
    <message>
      <source>Access</source>
      <translation type="unfinished" >Löytyi</translation>
    </message>
    <message>
      <source>Used in Doc</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Path to Font File</source>
      <translation type="unfinished" >Polku fonttitiedostoon</translation>
    </message>
    <message>
      <source>Unknown</source>
      <comment>
font type</comment>
      <translation type="unfinished" >Tuntematon</translation>
    </message>
    <message>
      <source>Unknown</source>
      <comment>
font format</comment>
      <translation type="unfinished" >Tuntematon</translation>
    </message>
    <message>
      <source>User</source>
      <comment>
font preview</comment>
      <translation type="unfinished" >Käyttäjä</translation>
    </message>
    <message>
      <source>System</source>
      <comment>
font preview</comment>
      <translation type="unfinished" >Järjestelmä</translation>
    </message>
    <message>
      <source>Click to change the value</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>FontPrefs</name>
    <message>
      <source>Font Name</source>
      <translation>Fontin nimi</translation>
    </message>
    <message>
      <source>Font Substitutions</source>
      <translation>Korvaavat fontit</translation>
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
      <translation>Mahdolliset fontit</translation>
    </message>
    <message>
      <source>&amp;Available Fonts</source>
      <translation>L&amp;öydetyt fontit</translation>
    </message>
    <message>
      <source>Font &amp;Substitutions</source>
      <translation>&amp;Korvaavat fontit</translation>
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
      <source>Font search paths can only be set in File > Preferences, and only when there is no document currently open. Close any open documents, then use File > Preferences > Fonts to change the font search path.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>FontPreview</name>
    <message>
      <source>Size of the selected font</source>
      <translation>Valitun fontin koko</translation>
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
    <message>
      <source>Fonts Preview</source>
      <translation type="unfinished" >Fonttien esikatselu</translation>
    </message>
    <message>
      <source>&amp;Quick Search:</source>
      <translation type="unfinished" >&amp;Pikahaku</translation>
    </message>
    <message>
      <source>&amp;Font Size:</source>
      <translation type="unfinished" >&amp;Fontin koko:</translation>
    </message>
    <message>
      <source>Text</source>
      <translation type="unfinished" >Teksti</translation>
    </message>
    <message>
      <source>Sample text to display</source>
      <translation type="unfinished" >Näytettävä esimerkkiteksti</translation>
    </message>
    <message>
      <source>Se&amp;t</source>
      <translation type="unfinished" >&amp;Käytä</translation>
    </message>
    <message>
      <source>Alt+T</source>
      <translation type="unfinished" >Alt+T</translation>
    </message>
    <message>
      <source>Reset the text</source>
      <translation type="unfinished" >Palauta teksti</translation>
    </message>
    <message>
      <source>&amp;Close</source>
      <translation type="unfinished" >&amp;Sulje</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation type="unfinished" >Alt+C</translation>
    </message>
    <message>
      <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. The given text is taken as substring.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Default</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Show Extended Font Information</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Set the preview text</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>FontPreviewPlugin</name>
    <message>
      <source>&amp;Font Preview...</source>
      <translation>&amp;Fonttien esikatselu...</translation>
    </message>
    <message>
      <source>Font Preview dialog</source>
      <translation>Fonttien esikatselun dialogi</translation>
    </message>
    <message>
      <source>Sorting, searching and browsing available fonts.</source>
      <translation>Järjestä, etsi ja selaa saatavilla olevia fontteja</translation>
    </message>
  </context>
  <context>
    <name>FontReplaceDialog</name>
    <message>
      <source>Font Substitution</source>
      <translation>Korvaa fontti</translation>
    </message>
    <message>
      <source>Original Font</source>
      <translation>Alkuperäinen fontti</translation>
    </message>
    <message>
      <source>Substitution Font</source>
      <translation>Korvaava fontti</translation>
    </message>
    <message>
      <source>Make these substitutions permanent</source>
      <translation>Tee korvauksista pysyviä</translation>
    </message>
    <message>
      <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
      <translation>Asiakirja sisältää fontteja, joita ei löydy järjestelmästäsi. Valitse ne korvaavat fontit. Peruuttaminen lopettaa asiakirjan latauksen.</translation>
    </message>
    <message>
      <source>Cancels these font substitutions and stops loading the document.</source>
      <translation>Peruuta fonttien korvaus ja keskeytä asiakirjan lataaminen.</translation>
    </message>
    <message>
      <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit > Preferences > Fonts.</source>
      <translation>Jos käytät tätä Scribus korvaa kaikissa asiakirjoissa nämä fontit valinnoillasi. Fonttien korvaavuuksia voi myöhemmin säätää valikon kohdassa Muokkaa->Asetukset->Fontit.</translation>
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
      <source>Add, change or remove color stops here</source>
      <translation>Lisää, muuta tai poista värirajoja tässä</translation>
    </message>
  </context>
  <context>
    <name>GradientVectorDialog</name>
    <message>
      <source>Gradient Vector</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>GuideManager</name>
    <message>
      <source>&amp;Single</source>
      <translation type="unfinished" >&amp;Yksi</translation>
    </message>
    <message>
      <source>Horizontals</source>
      <translation type="unfinished" >Vaakasuorat</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation type="unfinished" >&amp;Lisää</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation type="unfinished" >Alt+A</translation>
    </message>
    <message>
      <source>D&amp;elete</source>
      <translation type="unfinished" >D&amp;elete</translation>
    </message>
    <message>
      <source>Alt+E</source>
      <translation type="unfinished" >Alt+E</translation>
    </message>
    <message>
      <source>Verticals</source>
      <translation type="unfinished" >Pystysuorat</translation>
    </message>
    <message>
      <source>A&amp;dd</source>
      <translation type="unfinished" >A&amp;dd</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation type="unfinished" >Alt+D</translation>
    </message>
    <message>
      <source>De&amp;lete</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Alt+L</source>
      <translation type="unfinished" >Alt+L</translation>
    </message>
    <message>
      <source>&amp;Lock Guides</source>
      <translation type="unfinished" >&amp;Lock Guides</translation>
    </message>
    <message>
      <source>Appl&amp;y to All Pages</source>
      <translation type="unfinished" >Kä&amp;ytä kaikkiin sivuihin</translation>
    </message>
    <message>
      <source>Alt+Y</source>
      <translation type="unfinished" >Alt+Y</translation>
    </message>
    <message>
      <source>&amp;Column/Row</source>
      <translation type="unfinished" >&amp;Rivi tai sarake</translation>
    </message>
    <message>
      <source>&amp;Number:</source>
      <translation type="unfinished" >&amp;Numero:</translation>
    </message>
    <message>
      <source>U&amp;se Gap:</source>
      <translation type="unfinished" >&amp;Väli:</translation>
    </message>
    <message>
      <source>Alt+S</source>
      <translation type="unfinished" >Alt+S</translation>
    </message>
    <message>
      <source>Refer To</source>
      <translation type="unfinished" >Suhteessa</translation>
    </message>
    <message>
      <source>&amp;Page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Alt+P</source>
      <translation type="unfinished" >Alt+P</translation>
    </message>
    <message>
      <source>M&amp;argins</source>
      <translation type="unfinished" >&amp;Reunuksiin</translation>
    </message>
    <message>
      <source>S&amp;election</source>
      <translation type="unfinished" >&amp;Valintaan</translation>
    </message>
    <message>
      <source>Nu&amp;mber:</source>
      <translation type="unfinished" >Nu&amp;mero:</translation>
    </message>
    <message>
      <source>Use &amp;Gap:</source>
      <translation type="unfinished" >V&amp;äli:</translation>
    </message>
    <message>
      <source>Alt+G</source>
      <translation type="unfinished" >Alt+G</translation>
    </message>
    <message>
      <source>&amp;Misc</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Delete all guides from the current page</source>
      <translation type="unfinished" >Poista kaikki apuviivat nykyiseltä sivulta</translation>
    </message>
    <message>
      <source>Delete Guides from Current &amp;Page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Delete all guides from the current document</source>
      <translation type="unfinished" >Poista kaikki asiakirjan apuviivat</translation>
    </message>
    <message>
      <source>Delete Guides from &amp;All Pages</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Guide Manager</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Add a new horizontal guide</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Delete the selected horizontal guide</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Add a new vertical guide</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Delete the selected vertical guide</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Lock the guides</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Apply to all pages</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Number of horizontal guides to create</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Number of vertical guides to create</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Create rows with guides, with an additional gap between the rows</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Create columns with guides, with an additional gap between the columns</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Create the selected number of horizontal guides relative to the current page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Create the selected number of horizontal guides relative to the current page's margins</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Create the selected number of horizontal guides relative to the current selection of items</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Create the selected number of vertical guides relative to the current page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Create the selected number of vertical guides relative to the current page's margins</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Create the selected number of vertical guides relative to the current selection of items</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Apply the shown guides to all pages in the document</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Delete all guides shown on the current page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Delete all guides from all pages</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>HelpBrowser</name>
    <message>
      <source>Scribus Online Help</source>
      <translation>Scribus ohje verkossa</translation>
    </message>
    <message>
      <source>&amp;Contents</source>
      <translation>&amp;Sisältö</translation>
    </message>
    <message>
      <source>&amp;Search</source>
      <translation>&amp;Etsi</translation>
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
      <source>Book&amp;marks</source>
      <translation>&amp;Kirjanmerkit</translation>
    </message>
    <message>
      <source>&amp;Print...</source>
      <translation>T&amp;ulosta...</translation>
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
    <message>
      <source>Scribus Help</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Searching is case insensitive</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Exit</source>
      <translation type="unfinished" >&amp;Poistu</translation>
    </message>
    <message>
      <source>Find &amp;Next...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Find &amp;Previous...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Add</source>
      <translation type="unfinished" >&amp;Lisää</translation>
    </message>
    <message>
      <source>&amp;Quit</source>
      <translation type="unfinished" >&amp;Lopeta</translation>
    </message>
    <message>
      <source>&lt;h2>&lt;p>Sorry, no manual is installed!&lt;/p>&lt;p>Please see:&lt;/p>&lt;ul>&lt;li>http://docs.scribus.net for updated documentation&lt;/li>&lt;li>http://www.scribus.net for downloads&lt;/li>&lt;/ul>&lt;/h2></source>
      <comment>HTML message for no documentation available to show</comment>
      <translation type="unfinished" />
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
    <message>
      <source>Add to the
Exception List</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Add to the
Ignore List</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>HySettings</name>
    <message>
      <source>Ignore List</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Add a new Entry</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Edit Entry</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Exception List</source>
      <translation type="unfinished" />
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
    <message>
      <source>Exposure time</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Aperture:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>ISO equiv.:</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>ImportAIPlugin</name>
    <message>
      <source>Import AI...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Imports Illustrator Files</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Imports most Illustrator files into the current document,
converting their vector data into Scribus objects.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The file could not be imported</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>ImportPSPlugin</name>
    <message>
      <source>Import PostScript...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Imports PostScript Files</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Imports most PostScript files into the current document,
converting their vector data into Scribus objects.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>ImportXfigPlugin</name>
    <message>
      <source>Import Xfig...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Imports Xfig Files</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Imports most Xfig files into the current document,
converting their vector data into Scribus objects.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>All Supported Formats</source>
      <translation type="unfinished" >Kaikki tuetut tiedostotyypit</translation>
    </message>
  </context>
  <context>
    <name>Imposition</name>
    <message>
      <source>Portrait</source>
      <translation type="unfinished" >Pystysuora</translation>
    </message>
    <message>
      <source>Landscape</source>
      <translation type="unfinished" >Vaakasuora</translation>
    </message>
  </context>
  <context>
    <name>ImpositionBase</name>
    <message>
      <source>Imposition</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Gri&amp;d</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Copies</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Do&amp;uble sided</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Alt+U</source>
      <translation type="unfinished" >Alt+U</translation>
    </message>
    <message>
      <source>Front side</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Back side</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Booklet</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Pages per sheet</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>4</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>8</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>16</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Pages</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&lt;html>&lt;head>&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; />&lt;style type=&quot;text/css&quot;>
p, li { white-space: pre-wrap; }
&lt;/style>&lt;/head>&lt;body style=&quot; font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;&quot;>
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;>Separate pages with a comma, ranges with a hyphen, e.g. 1,4,9-11 to get pages 1,4,9,10,11.&lt;/p>&lt;/body>&lt;/html></source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Fold</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Front page from</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Double sided</source>
      <translation type="unfinished" >Kaksipuolinen</translation>
    </message>
    <message>
      <source>Back page from</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Destination page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Size</source>
      <translation type="unfinished" >Koko</translation>
    </message>
    <message>
      <source>Orientation</source>
      <translation type="unfinished" >Suunta</translation>
    </message>
    <message>
      <source>Width</source>
      <translation type="unfinished" >Leveys</translation>
    </message>
    <message>
      <source>Height</source>
      <translation type="unfinished" >Korkeus</translation>
    </message>
    <message>
      <source>Preview</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Refresh preview</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;OK</source>
      <translation type="unfinished" >&amp;OK</translation>
    </message>
    <message>
      <source>Alt+G</source>
      <translation type="unfinished" >Alt+G</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation type="unfinished" >&amp;Peruuta</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation type="unfinished" >Alt+C</translation>
    </message>
  </context>
  <context>
    <name>ImpositionPlugin</name>
    <message>
      <source>&amp;Imposition...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Imposition dialog</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Imposition on grids, booklets and folds</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>InsPage</name>
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
    <name>InsertAFrame</name>
    <message>
      <source>Open</source>
      <translation type="unfinished" >Avaa</translation>
    </message>
    <message>
      <source>Insert A Frame</source>
      <translation type="unfinished" >Lisää kehys</translation>
    </message>
    <message>
      <source>T&amp;ype</source>
      <translation type="unfinished" >T&amp;yyppi</translation>
    </message>
    <message>
      <source>&amp;Text Frame</source>
      <translation type="unfinished" >&amp;Tekstikehys</translation>
    </message>
    <message>
      <source>&amp;Image Frame</source>
      <translation type="unfinished" >&amp;Kuvakehys</translation>
    </message>
    <message>
      <source>&amp;Location</source>
      <translation type="unfinished" >&amp;Sijainti</translation>
    </message>
    <message>
      <source>Page Placement</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Current Page</source>
      <translation type="unfinished" >Nykyinen sivu</translation>
    </message>
    <message>
      <source>All Pages</source>
      <translation type="unfinished" >Kaikki sivut</translation>
    </message>
    <message>
      <source>...</source>
      <translation type="unfinished" >...</translation>
    </message>
    <message>
      <source>Position of Frame</source>
      <translation type="unfinished" >Kehyksen sijainti</translation>
    </message>
    <message>
      <source>Top Left of Margins</source>
      <translation type="unfinished" >Vasen yläkulma reunuksista</translation>
    </message>
    <message>
      <source>Top Left of Page</source>
      <translation type="unfinished" >Sivun vasen yläkulma</translation>
    </message>
    <message>
      <source>Top Left of Bleed</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>X:</source>
      <translation type="unfinished" >X:</translation>
    </message>
    <message>
      <source>Y:</source>
      <translation type="unfinished" >Y:</translation>
    </message>
    <message>
      <source>&amp;Size</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Same as the Page Margins</source>
      <translation type="unfinished" >Sama kuin sivun reunusten</translation>
    </message>
    <message>
      <source>Same as the Page</source>
      <translation type="unfinished" >Sivun koko</translation>
    </message>
    <message>
      <source>Same as the Bleed</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Same as the Imported Image</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Height:</source>
      <translation type="unfinished" >Korkeus:</translation>
    </message>
    <message>
      <source>Width:</source>
      <translation type="unfinished" >Leveys:</translation>
    </message>
    <message>
      <source>&amp;Options</source>
      <translation type="unfinished" >&amp;Asetukset</translation>
    </message>
    <message>
      <source>Source Image:</source>
      <translation type="unfinished" >Kuva:</translation>
    </message>
    <message>
      <source>There are no options for this type of frame</source>
      <translation type="unfinished" >Tälle kehystyypille ei ole asetuksia</translation>
    </message>
    <message>
      <source>Columns:</source>
      <translation type="unfinished" >Sarakkeet:</translation>
    </message>
    <message>
      <source>Gap:</source>
      <translation type="unfinished" >Väli:</translation>
    </message>
    <message>
      <source>Source Document:</source>
      <translation type="unfinished" >Asiakirja:</translation>
    </message>
    <message>
      <source>Range of Pages</source>
      <translation type="unfinished" >Sivuväli</translation>
    </message>
    <message>
      <source>Custom Position</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Custom Size</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&lt;b>Insert a text frame&lt;/b>&lt;br/>A text frame allows you to enter any text in a defined position with the formatting you choose. You may select a text file on the Options tab if you want to immediately import a document into the frame. Scribus supports a wide variety of importable formats from plain text to OpenOffice.org.&lt;br/>Your text may be edited and formatted on the page directly or in the Story Editor.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&lt;b>Insert an image frame&lt;/b>&lt;br/>An image frame allows you to place an image onto your page. Various image effects may be applied or combined including transparencies, brightness, and posterisation that allow retouching or the creation of interesting visual results. Image scaling and shaping is performed with the Properties Palette.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insert one or more text frames</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insert one or more image frames</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Place the new frames on the current page, on all pages or on a selected range</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insert the frame on the current page only</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insert one frame for each existing page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insert frames on a range of pages</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Range of pages to insert frames on</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Position the new frame in relation to the page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insert the frame at the top left of the page margins</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insert the frame at the top left of the page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insert the frame at the top left of the page bleed</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insert the frame at a custom position on the page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Top position of the inserted frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Left position of the inserted frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insert the new frame with the same dimensions as the page margins</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insert the new frame with the same dimensions as the page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insert the new frame with the same dimensions as the bleed area outside the boundary of the page itself</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insert the new frame with the same dimensions as the image that will be imported</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insert the new frame with a custom size</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Width of the inserted frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Height of the inserted frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Number of columns for the inserted text frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Distance between the columns in the text frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Link the inserted text frames together to form a chain of frames</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Link Inserted Frames</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Link the first inserted frame to a preexisting text frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Link to Existing Frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Name of existing text frame to link to</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Source document to load into the text frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Source image to load into the inserted image frame</source>
      <translation type="unfinished" />
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
      <source>Do you really want to delete this script?</source>
      <translation>Haluatko poistaa tämän skriptin?</translation>
    </message>
    <message>
      <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
      <translation>Lisää uuden skriptin, määrittelee funktion samalla nimellä. Jos haluat käyttää tätä skriptiä avaustoimintaskriptinä älä muuta funktion nimeä.</translation>
    </message>
  </context>
  <context>
    <name>LatexEditor</name>
    <message>
      <source>Enter Code:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Update</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Revert</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Program Messages:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Status: Unknown</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Kill Program</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Options</source>
      <translation type="unfinished" >Asetukset</translation>
    </message>
    <message>
      <source>Resolution:</source>
      <translation type="unfinished" >Tarkkuus:</translation>
    </message>
    <message>
      <source>Automatic</source>
      <translation type="unfinished" >Automaattinen</translation>
    </message>
    <message>
      <source> DPI</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Program:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Use Preamble</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Status: </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Error</source>
      <translation type="unfinished" >Virhe</translation>
    </message>
    <message>
      <source>Finished</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Running</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>No item selected!</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insert symbol</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Editor</source>
      <translation type="unfinished" >Editori</translation>
    </message>
    <message>
      <source>Information</source>
      <translation type="unfinished" >Tiedot</translation>
    </message>
    <message>
      <source>An editor for this frame is already running!</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Editor running!</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Please specify an editor in the preferences!</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Could not create a temporary file to run the external editor!</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Run External Editor...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Running the editor failed with exitcode %d!</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Running the editor &quot;%1&quot; failed!</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Run external editor...</source>
      <translation type="unfinished" />
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
    <message>
      <source>Opacity:</source>
      <translation>Peittävyys:</translation>
    </message>
    <message>
      <source> %</source>
      <translation type="unfinished" > %</translation>
    </message>
    <message>
      <source>Blend Mode:</source>
      <translation type="unfinished" >Sekoitustila:</translation>
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
      <source>Multiply</source>
      <translation type="unfinished" >Kerroin</translation>
    </message>
    <message>
      <source>Screen</source>
      <translation>Rasterointi</translation>
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
      <source>Hue</source>
      <translation>Sävy</translation>
    </message>
    <message>
      <source>Saturation</source>
      <translation>Saturaatio</translation>
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
      <source>Duplicates the current layer</source>
      <translation>Monistaa nykyisen tason</translation>
    </message>
    <message>
      <source>Make Layer Visible - Uncheck to hide the layer from the display </source>
      <translation type="unfinished" >Näytä taso. Valinnan poistaminen piilottaa tason näytöltä</translation>
    </message>
    <message>
      <source>Print Layer - Uncheck to disable printing. </source>
      <translation type="unfinished" >Tulosta taso. Valinnan poistaminen estää tason tulostamisen.</translation>
    </message>
    <message>
      <source>Lock or Unlock Layer - Unchecked is unlocked </source>
      <translation type="unfinished" >Lukitse taso. Valinnan poistaminen vapauttaa tason.</translation>
    </message>
    <message>
      <source>Color of the Layer Indicator - Each layer has a color assigned to display on the canvas when layer indicators are enabled. You can double click to edit the color. </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Text flows around objects in lower Layers - Enabling this forces text frames to flow around other objects, even in layers below</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Outline Mode - Toggles the 'wireframe' display of objects to speed the display of very complex objects.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Name of the Layer - Double clicking on the name of a layer enabled editing</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>LegacyMode</name>
    <message>
      <source>Open</source>
      <translation type="unfinished" >Avaa</translation>
    </message>
    <message>
      <source>All Supported Formats</source>
      <translation type="unfinished" >Kaikki tuetut tiedostotyypit</translation>
    </message>
  </context>
  <context>
    <name>LensDialogBase</name>
    <message>
      <source>Optical Lens</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>+</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>-</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Lens Parameters</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Add a new lens</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Add Lens</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Remove selected lens</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Remove Lens</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Zoom In</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Zoom Out</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;X Pos:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Horizontal position of the lens</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Y Pos:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Vertical position of the lens</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The selected lens acts like a magnification lens</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Magnification Lens</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The selected lens acts like a fish eye lens</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Fish Eye Lens</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Ra&amp;dius:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Radius of the lens</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Strength:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Strength of the lens</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>LensEffectsPlugin</name>
    <message>
      <source>Lens Effects...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Path Tools</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Lens Effects</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Apply fancy lens effects</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>LoadSavePlugin</name>
    <message>
      <source>All Files (*)</source>
      <translation>Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>No File Loader Plugins Found</source>
      <translation type="unfinished" />
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
    <message>
      <source>Standard Lorem Ipsum</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Random Paragraphs</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Number of paragraphs of selected sample text to insert</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>List of languages available to insert sample text in</source>
      <translation type="unfinished" />
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
    <message>
      <source>Other Settings</source>
      <translation>Muut asetukset</translation>
    </message>
    <message>
      <source>Master Page:</source>
      <translation>Mallisivu:</translation>
    </message>
    <message>
      <source>Size of the inserted pages, either a standard or custom size.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Orientation of the page(s) to be inserted</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Width of the page(s) to be inserted</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Height of the page(s) to be inserted</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>When inserting a new page between others, move objects with their current pages. This is the default action.</source>
      <translation type="unfinished" />
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
      <source>Apply the margin changes to all existing pages in the document</source>
      <translation>Käytä reunusten muutoksia asiakirjan kaikkiin sivuihin.</translation>
    </message>
    <message>
      <source>Printer Margins...</source>
      <translation>Tulostimen reunukset...</translation>
    </message>
    <message>
      <source>Import the margins for the selected page size from the available printers.</source>
      <translation>Tuo reunukset valitulle sivukoolle asennetuista tulostimista</translation>
    </message>
    <message>
      <source>Apply settings to:</source>
      <translation>Käytä asetuksia:</translation>
    </message>
    <message>
      <source>All Document Pages</source>
      <translation>Kaikkiin asiakirjan sivuihin</translation>
    </message>
    <message>
      <source>All Master Pages</source>
      <translation>Kaikille mallisivuille</translation>
    </message>
    <message>
      <source>Apply the margin changes to all existing master pages in the document</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Margin Guides</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Top:</source>
      <translation type="unfinished" >Yläreuna:</translation>
    </message>
    <message>
      <source>Bottom:</source>
      <translation type="unfinished" >Alareuna:</translation>
    </message>
    <message>
      <source>Distance for bleed from the top of the physical page</source>
      <translation type="unfinished" >Etäisyys leikkausvaralle sivun todellisesta yläreunasta</translation>
    </message>
    <message>
      <source>Distance for bleed from the bottom of the physical page</source>
      <translation type="unfinished" >Etäisyys leikkausvaralle sivun todellista alareunasta</translation>
    </message>
    <message>
      <source>Distance for bleed from the left of the physical page</source>
      <translation type="unfinished" >Etäisyys leikkausvaralle sivun todellisesta vasemmasta reunasta</translation>
    </message>
    <message>
      <source>Distance for bleed from the right of the physical page</source>
      <translation type="unfinished" >Etäisyys leikkausvaralle sivun todellisesta oikeasta reunasta</translation>
    </message>
    <message>
      <source>Bleeds</source>
      <translation type="unfinished" >Leikkausvarat</translation>
    </message>
    <message>
      <source>Inside:</source>
      <translation type="unfinished" >Sisäreuna:</translation>
    </message>
    <message>
      <source>Outside:</source>
      <translation type="unfinished" >Ulkoreuna:</translation>
    </message>
    <message>
      <source>Left:</source>
      <translation type="unfinished" >Vasen reuna:</translation>
    </message>
    <message>
      <source>Right:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Distance between the left margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Distance between the right margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>MasterPagesPalette</name>
    <message>
      <source>Edit Master Pages</source>
      <translation>Muokkaa mallisivuja</translation>
    </message>
    <message>
      <source>Do you really want to delete this master page?</source>
      <translation>Haluatko poistaa tämän mallisivun?</translation>
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
    <message>
      <source>Unable to Rename Master Page</source>
      <translation>Mallisivua ei voitu nimetä uudelleen</translation>
    </message>
    <message>
      <source>The Normal page is not allowed to be renamed.</source>
      <translation>Normaali oletusmallisivua ei voi nimetä uudelleen</translation>
    </message>
    <message>
      <source>Rename Master Page</source>
      <translation>Nimeä mallisivu uudelleen</translation>
    </message>
    <message>
      <source>New Name:</source>
      <translation>Uusi nimi:</translation>
    </message>
    <message>
      <source>Copy #%1 of %2</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>Measurements</name>
    <message>
      <source> °</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>MeasurementsBase</name>
    <message>
      <source>Distances</source>
      <translation type="unfinished" >Etäisyydet</translation>
    </message>
    <message>
      <source>X1:</source>
      <translation type="unfinished" >X1:</translation>
    </message>
    <message>
      <source>10000.0000</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Y1:</source>
      <translation type="unfinished" >Y1:</translation>
    </message>
    <message>
      <source>X2:</source>
      <translation type="unfinished" >X2:</translation>
    </message>
    <message>
      <source>Y2:</source>
      <translation type="unfinished" >Y2:</translation>
    </message>
    <message>
      <source>Length:</source>
      <translation type="unfinished" >Pituus:</translation>
    </message>
    <message>
      <source>DX:</source>
      <translation type="unfinished" >DX:</translation>
    </message>
    <message>
      <source>DY:</source>
      <translation type="unfinished" >DY:</translation>
    </message>
    <message>
      <source>Angle:</source>
      <translation type="unfinished" >Kulma:</translation>
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
    <name>MeshDistortionDialog</name>
    <message>
      <source>+</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>-</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Mesh Distortion</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Drag the red handles with the mouse to distort the mesh</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Zoom In</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Zoom Out</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Resets the selected handles to their initial position.
If no handle is selected all handles will be reset.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Reset</source>
      <translation type="unfinished" >&amp;Palauta</translation>
    </message>
  </context>
  <context>
    <name>MeshDistortionPlugin</name>
    <message>
      <source>Mesh Distortion...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Path Tools</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Mesh Distortion of Polygons</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>MissingFont</name>
    <message>
      <source>Missing Font</source>
      <translation>Puuttuva fontti</translation>
    </message>
    <message>
      <source>The Font %1 is not installed.</source>
      <translation>Fonttia %1 ei ole asennettu.</translation>
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
    <name>ModeToolBar</name>
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
    <name>MultiProgressDialog</name>
    <message>
      <source>Progress</source>
      <translation type="unfinished" >Edistyminen</translation>
    </message>
    <message>
      <source>Overall Progress:</source>
      <translation type="unfinished" >Kokonaisedistyminen:</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation type="unfinished" >&amp;Peruuta</translation>
    </message>
    <message>
      <source>%v of %m</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>MultipleDuplicate</name>
    <message>
      <source>&amp;Horizontal Shift:</source>
      <translation>&amp;Vaakasuora siirtymä:</translation>
    </message>
    <message>
      <source>&amp;Vertical Shift:</source>
      <translation>P&amp;ystysuora siirtymä:</translation>
    </message>
    <message>
      <source>&amp;Horizontal Gap:</source>
      <translation>&amp;Vaakaväli:</translation>
    </message>
    <message>
      <source>&amp;Vertical Gap:</source>
      <translation>Pysty&amp;väli:</translation>
    </message>
    <message>
      <source>Multiple Duplicate</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;By Number of Copies</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Number of Copies:</source>
      <translation type="unfinished" >&amp;Kopioiden lukumäärä:</translation>
    </message>
    <message>
      <source>&amp;Shift Created Items By</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Alt+S</source>
      <translation type="unfinished" >Alt+S</translation>
    </message>
    <message>
      <source>Create &amp;Gap Between Items Of</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Alt+G</source>
      <translation type="unfinished" >Alt+G</translation>
    </message>
    <message>
      <source>Rotation:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>By &amp;Rows &amp;&amp; Columns</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Vertical Gap:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Horizontal Gap:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Number of Rows:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Number of Columns:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;OK</source>
      <translation type="unfinished" >&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation type="unfinished" >&amp;Peruuta</translation>
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
    <message>
      <source>First Page is:</source>
      <translation type="unfinished" >Ensimmäinen sivu on:</translation>
    </message>
    <message>
      <source>Show Document Settings After Creation</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Document Layout</source>
      <translation type="unfinished" />
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
      <source>Enlarge the Size of the Path by shown %</source>
      <translation>Suurenna polun kokoa</translation>
    </message>
    <message>
      <source>Angle of Rotation</source>
      <translation>Kierron kulma</translation>
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
    <message>
      <source>Shrink the Size of the Path by shown %</source>
      <translation>Pienennä koko annetulla prosenttimäärällä</translation>
    </message>
    <message>
      <source>Reduce the Size of the Path by the shown value</source>
      <translation>Pienennä kokoa annetulla arvolla</translation>
    </message>
    <message>
      <source>Enlarge the Size of the Path by the shown value</source>
      <translation>Suurenna kokoa annetulla arvolla</translation>
    </message>
    <message>
      <source>% to Enlarge or Shrink By</source>
      <translation>Koon muutoksen prosenttimäärä</translation>
    </message>
    <message>
      <source>Value to Enlarge or Shrink By</source>
      <translation>Koon muutoksen arvo</translation>
    </message>
    <message>
      <source>Set Contour to Image Clip</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Reset the Contour Line to the Clipping Path of the Image</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>to Canvas</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>to Page</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>OODPlug</name>
    <message>
      <source>This document does not seem to be an OpenOffice Draw file.</source>
      <translation>Valittu asiakirja ei ole OpenOffice Draw -tiedosto.</translation>
    </message>
    <message>
      <source>Group%1</source>
      <translation type="unfinished" />
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
    <message>
      <source>This file contains some unsupported features</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The file could not be imported</source>
      <translation type="unfinished" />
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
    <name>OneClick</name>
    <message>
      <source>Origin</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Size</source>
      <translation type="unfinished" >Koko</translation>
    </message>
    <message>
      <source>Width:</source>
      <translation type="unfinished" >Leveys:</translation>
    </message>
    <message>
      <source>Length:</source>
      <translation type="unfinished" >Pituus:</translation>
    </message>
    <message>
      <source>Height:</source>
      <translation type="unfinished" >Korkeus:</translation>
    </message>
    <message>
      <source>Angle:</source>
      <translation type="unfinished" >Kulma:</translation>
    </message>
    <message>
      <source>Remember Values</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>OutlinePalette</name>
    <message>
      <source>Element</source>
      <translation type="unfinished" >Osa</translation>
    </message>
    <message>
      <source>Name &quot;%1&quot; isn't unique.&lt;br/>Please choose another.</source>
      <translation type="unfinished" >Nimi %1 on jo käytössä.&lt;br/>Ole hyvä ja valitse toinen.</translation>
    </message>
    <message>
      <source>Group </source>
      <translation type="unfinished" >Ryhmä</translation>
    </message>
    <message>
      <source>Page </source>
      <translation type="unfinished" >Sivu</translation>
    </message>
    <message>
      <source>Free Objects</source>
      <translation type="unfinished" >Vapaat kohteet</translation>
    </message>
    <message>
      <source>Outline</source>
      <translation type="unfinished" >Rakenne</translation>
    </message>
    <message>
      <source>Enter a keyword or regular expression to filter the outline.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Ctrl+f</source>
      <comment>Filter the Outline using a keyword</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Filter:</source>
      <translation type="unfinished" />
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
    <message>
      <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
      <translation>Tallenna-painike ei ole käytössä, jos yrität tallentaa PDF/X-3 -muodossa ja infotekstiä ei ole annettu PDF/X-3 -välilehdellä.</translation>
    </message>
    <message>
      <source>%1 does not exists and will be created, continue?</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Cannot create directory: 
%1</source>
      <translation>Hakemistoa ei voi luoda:\n
%1</translation>
    </message>
  </context>
  <context>
    <name>PDFLibCore</name>
    <message>
      <source>Saving PDF</source>
      <translation type="unfinished" >PDF-tallennus</translation>
    </message>
    <message>
      <source>Exporting Master Page:</source>
      <translation type="unfinished" >Viedään mallisivu:</translation>
    </message>
    <message>
      <source>Exporting Page:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Exporting Items on Current Page:</source>
      <translation type="unfinished" >Viedään kohteita sivulta:</translation>
    </message>
    <message>
      <source>Page:</source>
      <translation type="unfinished" >Sivu:</translation>
    </message>
    <message>
      <source>Date:</source>
      <translation type="unfinished" >Päivämäärä:</translation>
    </message>
    <message>
      <source>Failed to load an image : %1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Failed to write an image : %1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Failed to load an image mask : %1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insufficient memory for processing an image</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>A write error occured, please check available disk space</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PDFToolBar</name>
    <message>
      <source>PDF Tools</source>
      <translation type="unfinished" >PDF-työkalut</translation>
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
    <message>
      <source>Enable &amp;Antialiasing</source>
      <translation>Käytä reun&amp;anpehmennystä</translation>
    </message>
    <message>
      <source>Display Settings</source>
      <translation>Näytön asetukset</translation>
    </message>
    <message>
      <source>Print Settings</source>
      <translation>Tulostuksen asetukset</translation>
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
      <source>Clip to Page Margins</source>
      <translation>Leikkaa sivun reunuksiin</translation>
    </message>
    <message>
      <source>Print in Grayscale</source>
      <translation>Harmaasävytulostus</translation>
    </message>
    <message>
      <source>Convert Spot Colors</source>
      <translation>Muunna lisävärit</translation>
    </message>
    <message>
      <source>Fit to Width</source>
      <translation>Sovita leveyteen</translation>
    </message>
    <message>
      <source>Fit to Height</source>
      <translation>Sovita korkeuteen</translation>
    </message>
    <message>
      <source>Fit to Page</source>
      <translation>Sovita sivulle</translation>
    </message>
    <message>
      <source>Provides a more pleasant view of Type 1 fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
      <translation type="unfinished" >Mahdollistaa lisävärien muuntamisen prosessiväreiksi. Jos et aio painaa käyttäen lisävärejä, tämä on paras jättää valituksi.</translation>
    </message>
    <message>
      <source>Apply Color Profiles</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Allows you to embed color profiles in the print stream when color management is enabled</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PSLib</name>
    <message>
      <source>Processing Master Page:</source>
      <translation>Käsitellään mallisivua:</translation>
    </message>
    <message>
      <source>Exporting Page:</source>
      <translation>Viedään sivua:</translation>
    </message>
    <message>
      <source>Failed to write data for an image</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Failed to load an image : %1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Failed to load an image mask : %1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Insufficient memory for processing an image</source>
      <translation type="unfinished" />
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
      <source>None</source>
      <comment>relationship</comment>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>&amp;Add</source>
      <translation type="unfinished" >&amp;Lisää</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation type="unfinished" >Alt+A</translation>
    </message>
    <message>
      <source>&amp;Copy</source>
      <translation type="unfinished" >&amp;Kopioi</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation type="unfinished" >Alt+C</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Alt+D</source>
      <translation type="unfinished" >Alt+D</translation>
    </message>
    <message>
      <source>C&amp;lear</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Alt+L</source>
      <translation type="unfinished" >Alt+L</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation type="unfinished" >&amp;OK</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation type="unfinished" >&amp;Peruuta</translation>
    </message>
    <message>
      <source>Name</source>
      <translation type="unfinished" >Nimi</translation>
    </message>
    <message>
      <source>Type</source>
      <translation type="unfinished" >Tyyppi</translation>
    </message>
    <message>
      <source>Value</source>
      <translation type="unfinished" >Arvo</translation>
    </message>
    <message>
      <source>Parameter</source>
      <translation type="unfinished" >Parametri</translation>
    </message>
    <message>
      <source>Relationship</source>
      <translation type="unfinished" >Suhde</translation>
    </message>
    <message>
      <source>Relationship To</source>
      <translation type="unfinished" >Suhteessa</translation>
    </message>
    <message>
      <source>Attributes</source>
      <translation type="unfinished" >Muuttujat</translation>
    </message>
  </context>
  <context>
    <name>PageItem_ImageFrame</name>
    <message>
      <source>Preview Settings</source>
      <translation type="unfinished" >Esikatseluasetukset</translation>
    </message>
    <message>
      <source>Image</source>
      <translation type="unfinished" >Kuva</translation>
    </message>
  </context>
  <context>
    <name>PageItem_LatexFrame</name>
    <message>
      <source>Error</source>
      <translation type="unfinished" >Virhe</translation>
    </message>
    <message>
      <source>Running the external application failed!</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Could not create a temporary file to run the application!</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Information</source>
      <translation type="unfinished" >Tiedot</translation>
    </message>
    <message>
      <source>Running the application &quot;%1&quot; failed!</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Running</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The application &quot;%1&quot; failed to start!</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The application &quot;%1&quot; crashed!</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Application</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>DPI</source>
      <translation type="unfinished" >DPI</translation>
    </message>
    <message>
      <source>State</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Finished</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Render</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The config file didn't specify a executable path!</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Render Frame</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PageItem_PathText</name>
    <message>
      <source>Paragraphs: </source>
      <translation type="unfinished" >Kappaleet: </translation>
    </message>
    <message>
      <source>Lines: </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Words: </source>
      <translation type="unfinished" >Sanat: </translation>
    </message>
    <message>
      <source>Chars: </source>
      <translation type="unfinished" >Merkit: </translation>
    </message>
  </context>
  <context>
    <name>PageItem_TextFrame</name>
    <message>
      <source>Linked Text</source>
      <translation type="unfinished" >Linkitetty teksti</translation>
    </message>
    <message>
      <source>Text Frame</source>
      <translation type="unfinished" >Tekstikehys</translation>
    </message>
    <message>
      <source>Paragraphs: </source>
      <translation type="unfinished" >Kappaleet: </translation>
    </message>
    <message>
      <source>Lines: </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Words: </source>
      <translation type="unfinished" >Sanat: </translation>
    </message>
    <message>
      <source>Chars: </source>
      <translation type="unfinished" >Merkit: </translation>
    </message>
  </context>
  <context>
    <name>PageLayouts</name>
    <message>
      <source>First Page is:</source>
      <translation>Ensimmäinen sivu on:</translation>
    </message>
    <message>
      <source>Document Layout</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Number of pages to show side-by-side on the canvas
Often used for allowing items to be placed across page spreads</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Location on the canvas where the first page of the document is placed</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PagePalette</name>
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
    <message>
      <source>List of normal pages in the document, shown with the document layout. Pages may be dragged to rearrange or delete them.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Drag pages or master pages onto the trash to delete them</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>List of master pages in the document. Master page names may be dragged onto the page view below to apply master pages, or onto the empty space between pages to create new pages.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PageSelector</name>
    <message>
      <source>%1 of %2</source>
      <translation>%1/%2</translation>
    </message>
  </context>
  <context>
    <name>ParaStyleComboBox</name>
    <message>
      <source>No Style</source>
      <translation type="unfinished" >Ei tyyliä</translation>
    </message>
  </context>
  <context>
    <name>PathAlongPathPlugin</name>
    <message>
      <source>Path Along Path...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Path Tools</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Bends a Polygon along a Polyline</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>This plugin bends a Polygon with the help of a Polyline.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PathCutPlugin</name>
    <message>
      <source>Cut Polygon</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Path Tools</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Cuts a Polygon by a Polyline</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Qt Version too old</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>This plugin requires at least version 4.3.3 of the Qt library</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Error</source>
      <translation type="unfinished" >Virhe</translation>
    </message>
    <message>
      <source>The cutting line must cross the polygon and
both end points must lie outside of the polygon</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PathDialogBase</name>
    <message>
      <source>Path along Path</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Effect Type</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Single</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Single, stretched</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Repeated</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Repeated, stretched</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Horizontal Offset</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Vertical Offset</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Gap between Objects</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Preview on Canvas</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Rotate Objects by:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>0°</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>90°</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>180°</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>270°</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PathFinderBase</name>
    <message>
      <source>Boolean Path Operations</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Keep a copy of the original Item after applying the operation</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>keep</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>+</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>=</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Operation</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>...</source>
      <translation type="unfinished" >...</translation>
    </message>
    <message>
      <source>Swap Shapes</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Options</source>
      <translation type="unfinished" >Asetukset</translation>
    </message>
    <message>
      <source>Result gets Color of:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>first Shape</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>second Shape</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PathFinderDialog</name>
    <message>
      <source>Result gets Color of:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Intersection gets Color of:</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PathFinderPlugin</name>
    <message>
      <source>Path Operations...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Path Tools</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Path Operations</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Apply fancy boolean operations to paths.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Qt Version too old</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>This plugin requires at least version 4.3.3 of the Qt library</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PathStrokerPlugin</name>
    <message>
      <source>Create Path from Stroke</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Path Tools</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Converts the stroke of a Path to a filled Path.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PatternDialog</name>
    <message>
      <source>Choose a Directory</source>
      <translation>Valitse hakemisto</translation>
    </message>
    <message>
      <source>Loading Patterns</source>
      <translation>Ladataan kuviointeja</translation>
    </message>
    <message>
      <source>All Files (*)</source>
      <translation>Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Open</source>
      <translation>Avaa</translation>
    </message>
    <message>
      <source>Patterns</source>
      <translation type="unfinished" >Kuvioinnit</translation>
    </message>
    <message>
      <source>Load</source>
      <translation type="unfinished" >Lataa</translation>
    </message>
    <message>
      <source>Load Set</source>
      <translation type="unfinished" >Lataa kokoelma</translation>
    </message>
    <message>
      <source>Remove</source>
      <translation type="unfinished" >Poista</translation>
    </message>
    <message>
      <source>Remove All</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>OK</source>
      <translation type="unfinished" >OK</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation type="unfinished" >Peruuta</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation type="unfinished" >&amp;Nimi:</translation>
    </message>
    <message>
      <source>Rename Entry</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>All Supported Formats</source>
      <translation type="unfinished" >Kaikki tuetut tiedostotyypit</translation>
    </message>
    <message>
      <source>Rename</source>
      <translation type="unfinished" >Nimeä uudelleen</translation>
    </message>
  </context>
  <context>
    <name>PicSearch</name>
    <message>
      <source>Size:</source>
      <translation type="unfinished" >Koko:</translation>
    </message>
    <message>
      <source>Resolution:</source>
      <translation type="unfinished" >Tarkkuus:</translation>
    </message>
    <message>
      <source>DPI</source>
      <translation type="unfinished" >DPI</translation>
    </message>
    <message>
      <source>Unknown</source>
      <translation type="unfinished" >Tuntematon</translation>
    </message>
    <message>
      <source>Colorspace:</source>
      <translation type="unfinished" >Väriavaruus:</translation>
    </message>
    <message>
      <source>Result</source>
      <translation type="unfinished" >Tulos</translation>
    </message>
    <message>
      <source>Search Results for: </source>
      <translation type="unfinished" >Etsi tuloksia: </translation>
    </message>
    <message>
      <source>&amp;Preview</source>
      <translation type="unfinished" >&amp;Esikatselu</translation>
    </message>
    <message>
      <source>Alt+P</source>
      <translation type="unfinished" >Alt+P</translation>
    </message>
    <message>
      <source>&amp;Select</source>
      <translation type="unfinished" >Valit&amp;se</translation>
    </message>
    <message>
      <source>Alt+S</source>
      <translation type="unfinished" >Alt+S</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation type="unfinished" >Peruuta</translation>
    </message>
  </context>
  <context>
    <name>PicSearchOptions</name>
    <message>
      <source>The filesystem will be searched for case insensitive file names when you check this on. Remember it is not default on most operating systems except MS Windows</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Cancel Search</source>
      <translation type="unfinished" >Peruuta etsintä</translation>
    </message>
    <message>
      <source>Start Search</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select a base directory for search</source>
      <translation type="unfinished" >Valitse juurihakemisto etsinnälle</translation>
    </message>
    <message>
      <source>Scribus - Image Search</source>
      <translation type="unfinished" >Scribus - Kuvien etsintä</translation>
    </message>
    <message>
      <source>The search failed: %1</source>
      <translation type="unfinished" >Etsintä epäonnistui: %1</translation>
    </message>
    <message>
      <source>Search Images</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Search for:</source>
      <translation type="unfinished" >Etsi:</translation>
    </message>
    <message>
      <source>Start at:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Change...</source>
      <translation type="unfinished" >Muuta...</translation>
    </message>
    <message>
      <source>Searching</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Case insensitive search</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Search recursively</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Base directory for search does not exist.
Please choose another one.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PicStatus</name>
    <message>
      <source>Scribus - Image Search</source>
      <translation>Scribus - Kuvien etsintä</translation>
    </message>
    <message>
      <source>No images named &quot;%1&quot; were found.</source>
      <translation>Kuvia nimellä &quot;%1&quot; ei löytynyt.</translation>
    </message>
    <message>
      <source>Close</source>
      <translation type="unfinished" >Sulje</translation>
    </message>
    <message>
      <source>Not on a Page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>JPG</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>TIFF</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PSD</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>EPS/PS</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PDF</source>
      <translation type="unfinished" >PDF</translation>
    </message>
    <message>
      <source>JPG2000</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>emb. PSD</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Unknown</source>
      <translation type="unfinished" >Tuntematon</translation>
    </message>
    <message>
      <source>n/a</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Information</source>
      <translation type="unfinished" >Tiedot</translation>
    </message>
    <message>
      <source>Path:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Search...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Name:</source>
      <translation type="unfinished" >Nimi:</translation>
    </message>
    <message>
      <source>Image</source>
      <translation type="unfinished" >Kuva</translation>
    </message>
    <message>
      <source>DPI:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Format:</source>
      <translation type="unfinished" >Muotoilu:</translation>
    </message>
    <message>
      <source>Colorspace:</source>
      <translation type="unfinished" >Väriavaruus:</translation>
    </message>
    <message>
      <source>Size</source>
      <translation type="unfinished" >Koko</translation>
    </message>
    <message>
      <source>Pixels:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Scale:</source>
      <translation type="unfinished" >Skaalaa:</translation>
    </message>
    <message>
      <source>Printed:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Layout</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>On Page:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Image Tools</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Image Visible</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Image Effects...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Edit Image...</source>
      <translation type="unfinished" >Muokkaa kuvaa...</translation>
    </message>
    <message>
      <source>Extended Image Properties...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Sort by Name</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Sort by Page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Manage Images</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Go to</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Name of the image file</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Location where the image file is stored</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Search for a missing image</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Type of the image</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The colorspace of the image</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Colorspace used within the image, eg RGB or CMYK</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Native resolution of the image, in dots per inch</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Height and width of the image</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Horizontal and vertical scaling applied to the image</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Size of the image when printed</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Page that the image is displayed on</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Page Item:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Name of the page item that contains the image</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Effective DPI:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Effective resolution of the image after scaling</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Move to the page that the image is on</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Move to the page that the item is on and select it</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Enable or disable exporting of the item</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Export/Print Image</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Set format specfic properties of certain image types, like clipping paths</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Edit the image in the default editor</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Make the image visible or invisible</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Apply non destructive effects to the image in its frame</source>
      <translation type="unfinished" />
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
    <message>
      <source>Export successful</source>
      <translation type="unfinished" />
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
      <translation>tuntematon liitännäistyyppi</translation>
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
    <message>
      <source>There is a problem loading %1 of %2 plugins. %3 This is probably caused by some kind of dependency issue or old plugins existing in your install directory. If you clean out your install directory and reinstall and this still occurs, please report it on bugs.scribus.net.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Plugin: %1 initialized ok </source>
      <comment>plugin manager</comment>
      <translation>Liitännäinen %1 alustettu</translation>
    </message>
    <message>
      <source>Plugin: %1 failed post initialization</source>
      <comment>plugin manager</comment>
      <translation>Liitännäisen %1 alustus epäonnistui</translation>
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
      <translation>Liitännäisen ID</translation>
    </message>
    <message>
      <source>File</source>
      <translation>Tiedosto</translation>
    </message>
    <message>
      <source>You need to restart the application to apply the changes.</source>
      <translation>Ohjelma täytyy käynnistää uudestaan jotta muutokset tulevat voimaan.</translation>
    </message>
    <message>
      <source>Form</source>
      <translation type="unfinished" />
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
      <source>Tools</source>
      <translation>Työkalut</translation>
    </message>
    <message>
      <source>Guides</source>
      <translation>Apuviivat</translation>
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
      <source>Display</source>
      <translation>Näyttö</translation>
    </message>
    <message>
      <source>Preferences</source>
      <translation>Asetukset</translation>
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
      <source>Hyphenator</source>
      <translation>Tavutus</translation>
    </message>
    <message>
      <source>Fonts</source>
      <translation>Fontit</translation>
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
      <source>Miscellaneous</source>
      <translation>Muut</translation>
    </message>
    <message>
      <source>Plugins</source>
      <translation>Liitännäiset</translation>
    </message>
    <message>
      <source>Preflight Verifier</source>
      <translation>Tarkastustyökalu</translation>
    </message>
    <message>
      <source>Scrapbook</source>
      <translation>Leikekirja</translation>
    </message>
    <message>
      <source>Printer</source>
      <translation type="unfinished" >Painoon</translation>
    </message>
  </context>
  <context>
    <name>PrefsDialogBase</name>
    <message>
      <source>&amp;Defaults</source>
      <translation>&amp;Oletusarvot</translation>
    </message>
    <message>
      <source>Save Preferences</source>
      <translation>Tallenna asetukset</translation>
    </message>
    <message>
      <source>Export...</source>
      <translation>Vie...</translation>
    </message>
    <message>
      <source>&amp;Apply</source>
      <translation>&amp;Käytä</translation>
    </message>
    <message>
      <source>All preferences can be reset here</source>
      <translation>Palauta kaikki asetukset oletusarvoihin</translation>
    </message>
    <message>
      <source>Apply all changes without closing the dialog</source>
      <translation>Ota muutokset käyttöön sulkematta ikkunaa</translation>
    </message>
    <message>
      <source>Export current preferences into file</source>
      <translation>Tallenna nykyiset asetukset tiedostoon</translation>
    </message>
  </context>
  <context>
    <name>PrefsManager</name>
    <message>
      <source>Postscript</source>
      <translation>Postscript</translation>
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
      <source>None</source>
      <comment>layout type</comment>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>When you have selected a Document Layout other than Single Page, you can select a predefined page layout here. 'None' leaves margins as is, Gutenberg sets margins classically. 'Magazine' sets all margins to the same value. Leading is Left/Inside value.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PrintDialog</name>
    <message>
      <source>All</source>
      <translation>Kaikki</translation>
    </message>
    <message>
      <source>File</source>
      <translation>Tiedosto</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>Tallenna nimellä</translation>
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
      <source>Print Current Pa&amp;ge</source>
      <translation>Tulosta n&amp;ykyinen sivu</translation>
    </message>
    <message>
      <source>Print Normal</source>
      <translation>Normaali tulostus</translation>
    </message>
    <message>
      <source>PostScript Files (*.ps);;All Files (*)</source>
      <translation>PostScript-tiedostot (*.ps);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Failed to retrieve printer settings</source>
      <translation>Tulostimen asetusten hakeminen epäonnistui</translation>
    </message>
    <message>
      <source>Inside:</source>
      <translation>Sisäreuna:</translation>
    </message>
    <message>
      <source>Outside:</source>
      <translation>Ulkoreuna:</translation>
    </message>
  </context>
  <context>
    <name>PrintDialogBase</name>
    <message>
      <source>Setup Printer</source>
      <translation type="unfinished" >Aseta tulostin</translation>
    </message>
    <message>
      <source>Print Destination</source>
      <translation type="unfinished" >Tulosteen kohde</translation>
    </message>
    <message>
      <source>&amp;Options...</source>
      <translation type="unfinished" >&amp;Asetukset...</translation>
    </message>
    <message>
      <source>&amp;File:</source>
      <translation type="unfinished" >&amp;Tiedosto:</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation type="unfinished" >&amp;Muuta...</translation>
    </message>
    <message>
      <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
      <translation type="unfinished" >Käytä vaihtoehtoista tulostuksen hallintaa kuten kprinter tai gtklp</translation>
    </message>
    <message>
      <source>A&amp;lternative Printer Command</source>
      <translation type="unfinished" >Oma tulo&amp;stuskomento</translation>
    </message>
    <message>
      <source>Co&amp;mmand:</source>
      <translation type="unfinished" >&amp;Komento:</translation>
    </message>
    <message>
      <source>Range</source>
      <translation type="unfinished" >Väli</translation>
    </message>
    <message>
      <source>Print &amp;All</source>
      <translation type="unfinished" >Tulosta &amp;kaikki</translation>
    </message>
    <message>
      <source>N&amp;umber of Copies:</source>
      <translation type="unfinished" >K&amp;opioiden määrä:</translation>
    </message>
    <message>
      <source>Print Current Pa&amp;ge</source>
      <translation type="unfinished" >Tulosta n&amp;ykyinen sivu</translation>
    </message>
    <message>
      <source>Print &amp;Range</source>
      <translation type="unfinished" >Tulosta &amp;väli</translation>
    </message>
    <message>
      <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Options</source>
      <translation type="unfinished" >Asetukset</translation>
    </message>
    <message>
      <source>Print Normal</source>
      <translation type="unfinished" >Normaali tulostus</translation>
    </message>
    <message>
      <source>Print Separations</source>
      <translation type="unfinished" >Tulosta erottelu</translation>
    </message>
    <message>
      <source>Print in Color if Available</source>
      <translation type="unfinished" >Väritulostus, jos mahdollista</translation>
    </message>
    <message>
      <source>Print in Grayscale</source>
      <translation type="unfinished" >Harmaasävytulostus</translation>
    </message>
    <message>
      <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
      <translation type="unfinished" >Asettaa PostScript-tason. 
Tasot 1 ja 2 saattavat luoda suuria tiedostoja</translation>
    </message>
    <message>
      <source>Advanced Options</source>
      <translation type="unfinished" >Lisäasetukset</translation>
    </message>
    <message>
      <source>Page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Mirror Page(s) Horizontal</source>
      <translation type="unfinished" >Peilaa sivut vaakasuunnassa</translation>
    </message>
    <message>
      <source>Mirror Page(s) Vertical</source>
      <translation type="unfinished" >Peilaa sivut pystysuunnassa</translation>
    </message>
    <message>
      <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
      <translation type="unfinished" >Aseta PostScript-tiedoston median koko eksplisiittisesti. Käyttö on suositeltavaa ainoastaan jos paino pyysi sitä.</translation>
    </message>
    <message>
      <source>Set Media Size</source>
      <translation type="unfinished" >Aseta kohteen koko</translation>
    </message>
    <message>
      <source>Clip to Page Margins</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Color</source>
      <translation type="unfinished" >Väri</translation>
    </message>
    <message>
      <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
      <translation type="unfinished" >Tapa vaihtaa C, Y ja M värien tekemä harmaa mustalla tehdyksi. 
UCR vaikuttaa kuvien kohdissa, jotka ovat neutraaleja tai lähellä harmaata 
olevia tummia värejä. UCR:n käyttäminen saattaa parantaa joidenkin kuvien
tulostamista. UCR parantaa kykyä välttää värien liikakylläisyyttä CMY-musteilla. 
Parhaan tuloksen saavuttamiseksi tarvitaan tapauskohtaista testausta. UCR 
vähentää ylikylläisyyden vaaraa CMY-musteilla.</translation>
    </message>
    <message>
      <source>Apply Under Color Removal</source>
      <translation type="unfinished" >Käytä alivärinpoistoa (UCR)</translation>
    </message>
    <message>
      <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
      <translation type="unfinished" >Mahdollistaa lisävärien muuntamisen prosessiväreiksi. Jos et aio painaa käyttäen lisävärejä, tämä on paras jättää valituksi.</translation>
    </message>
    <message>
      <source>Convert Spot Colors to Process Colors</source>
      <translation type="unfinished" >Muunna lisävärit prosessiväreiksi</translation>
    </message>
    <message>
      <source>Allows you to embed color profiles in the print stream when color management is enabled</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Apply Color Profiles</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Marks</source>
      <translation type="unfinished" >Merkit</translation>
    </message>
    <message>
      <source>This creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Crop Marks</source>
      <translation type="unfinished" >Leikkausmerkit</translation>
    </message>
    <message>
      <source>Add registration marks which are added to each separation</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Registration Marks</source>
      <translation type="unfinished" >Kohdistusmerkit</translation>
    </message>
    <message>
      <source>This creates bleed marks which are indicated by  _ . _ and show the bleed limit</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Bleed Marks</source>
      <translation type="unfinished" >Leikkausvaran merkit</translation>
    </message>
    <message>
      <source>Add color calibration bars</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Color Bars</source>
      <translation type="unfinished" >Väriskaalat</translation>
    </message>
    <message>
      <source>Offset:</source>
      <translation type="unfinished" >Siirtymä:</translation>
    </message>
    <message>
      <source>Indicate the distance offset for the registration marks</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Bleeds</source>
      <translation type="unfinished" >Leikkausvarat</translation>
    </message>
    <message>
      <source>Top:</source>
      <translation type="unfinished" >Yläreuna:</translation>
    </message>
    <message>
      <source>Distance for bleed from the top of the physical page</source>
      <translation type="unfinished" >Etäisyys leikkausvaralle sivun todellisesta yläreunasta</translation>
    </message>
    <message>
      <source>Left:</source>
      <translation type="unfinished" >Vasen reuna:</translation>
    </message>
    <message>
      <source>Distance for bleed from the right of the physical page</source>
      <translation type="unfinished" >Etäisyys leikkausvaralle sivun todellisesta oikeasta reunasta</translation>
    </message>
    <message>
      <source>Bottom:</source>
      <translation type="unfinished" >Alareuna:</translation>
    </message>
    <message>
      <source>Distance for bleed from the bottom of the physical page</source>
      <translation type="unfinished" >Etäisyys leikkausvaralle sivun todellista alareunasta</translation>
    </message>
    <message>
      <source>Right:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Distance for bleed from the left of the physical page</source>
      <translation type="unfinished" >Etäisyys leikkausvaralle sivun todellisesta vasemmasta reunasta</translation>
    </message>
    <message>
      <source>Use the existing bleed settings from the document preferences</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Use Document Bleeds</source>
      <translation type="unfinished" >Käytä asiakirjan leikkausvaroja</translation>
    </message>
    <message>
      <source>Preview...</source>
      <translation type="unfinished" >Esikatselu...</translation>
    </message>
    <message>
      <source>&amp;Print</source>
      <translation type="unfinished" >T&amp;ulosta</translation>
    </message>
    <message>
      <source>Cancel</source>
      <translation type="unfinished" >Peruuta</translation>
    </message>
    <message>
      <source>Include PDF Annotations and Links into the output.
Note: PDF Forms will not be exported.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Include PDF Annotations and Links</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>PropertiesPalette</name>
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
      <translation>Fontin koko</translation>
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
      <source>Font of selected text or object</source>
      <translation>Valitun kohteen fontti</translation>
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
      <source>Use &amp;Bounding Box</source>
      <translation>Käytä &amp;rajausaluetta</translation>
    </message>
    <message>
      <source>&amp;Use Contour Line</source>
      <translation>&amp;Käytä reunusta</translation>
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
      <source>&amp;Free Scaling</source>
      <translation>&amp;Ei sovitusta</translation>
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
      <translation>Sovita &amp;kehyksen kokoon</translation>
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
      <source>Fill Rule</source>
      <translation>Täyttösääntö</translation>
    </message>
    <message>
      <source>Even-Odd</source>
      <translation>parillinen-pariton</translation>
    </message>
    <message>
      <source>Non Zero</source>
      <translation>nollasta poikkeavat</translation>
    </message>
    <message>
      <source>Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.</source>
      <translation>Tekstin ääriviivojen tai varjon väri. Jos molemmat ovat valittuna, ne jakavat saman värin.</translation>
    </message>
    <message>
      <source>Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.</source>
      <translation>Valitun tekstin väri. Jos tekstille on valittu ääriviivat tämä väri on täyttöväri. Jos käytössä on teksti varjolla, tämä on tekstin väri.</translation>
    </message>
    <message>
      <source>Use the bounding box, which is always rectangular, instead of the frame's shape for text flow of text frames below the object. </source>
      <translation>Käytä nelikulmion muotoista rajausaluetta kehyksen todellisen muodon sijaan tekstin kierron rajana.</translation>
    </message>
    <message>
      <source>Auto</source>
      <translation>Auto</translation>
    </message>
    <message>
      <source>Transparency Settings</source>
      <translation>Läpinäkyvyyden asetukset</translation>
    </message>
    <message>
      <source>&amp;Group</source>
      <translation>&amp;Ryhmitä</translation>
    </message>
    <message>
      <source>Opacity:</source>
      <translation>Peittävyys:</translation>
    </message>
    <message>
      <source>Blend Mode:</source>
      <translation>Sekoitustila:</translation>
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
      <source>Multiply</source>
      <translation>Kerroin</translation>
    </message>
    <message>
      <source>Screen</source>
      <translation>Rasterointi</translation>
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
      <source>Hue</source>
      <translation>Sävy</translation>
    </message>
    <message>
      <source>Color</source>
      <translation>Väri</translation>
    </message>
    <message>
      <source>Gap:</source>
      <translation>Väli:</translation>
    </message>
    <message>
      <source>Width:</source>
      <translation>Leveys:</translation>
    </message>
    <message>
      <source>Text &amp;Flow Around Frame</source>
      <translation>&amp;Teksti kiertää kehyksen</translation>
    </message>
    <message>
      <source>Disabled</source>
      <translation type="unfinished" >Ei</translation>
    </message>
    <message>
      <source>Use Frame &amp;Shape</source>
      <translation>Käytä kehyk&amp;sen muotoa</translation>
    </message>
    <message>
      <source>Image Effects</source>
      <translation>Kuvaefektit</translation>
    </message>
    <message>
      <source>Extended Image Properties</source>
      <translation>Kuvan ominaisuudet</translation>
    </message>
    <message>
      <source>Overprinting</source>
      <translation>Päällepainatus</translation>
    </message>
    <message>
      <source>Knockout</source>
      <translation>Poista alivärit</translation>
    </message>
    <message>
      <source>Overprint</source>
      <translation>Päällepainatus</translation>
    </message>
    <message>
      <source>Group the selected objects</source>
      <translation>Ryhmitä valitut kohteet</translation>
    </message>
    <message>
      <source>Disable text flow from lower frames around object</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Use the frame shape for text flow of text frames below the object.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>When chosen, the contour line can be edited with the Edit Shape Tool on the palette further above. When edited via the shape palette, this becomes a second separate line originally based on the frame's shape for text flow of text frames below the object. T</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Default</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Stair Step</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Skew</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Flip Text</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Type:</source>
      <translation type="unfinished" >Tyyppi:</translation>
    </message>
    <message>
      <source>Use Image Clip Path</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Paragraph St&amp;yle:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Character St&amp;yle:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Optical Margins:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Word Tracking</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Min:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Norm:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Glyph Extension</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Max:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Use the clipping path of the image</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Paragraph style of currently selected text or paragraph</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Character style of currently selected text or paragraph</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Remove Direct Paragraph Formatting</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Remove Direct Character Formatting</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Minimal width of spaces between words</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Normal width of spaces between words</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Minimal shrinkage of glyphs for justification</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Maximal extension of glyphs for justification</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Uses hanging punctuation and margin kerning to achieve nicer looking columns</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Enable or disable exporting of the object</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Custom</source>
      <translation type="unfinished" >Oma</translation>
    </message>
    <message>
      <source>&amp;Edit...</source>
      <translation type="unfinished" >&amp;Muokkaa...</translation>
    </message>
    <message>
      <source>First Line Offset</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Maximum Ascent</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Font Ascent</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Color &amp; Effects</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Advanced Settings</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Style Settings</source>
      <translation type="unfinished" >Tyylin asetukset</translation>
    </message>
    <message>
      <source>Baseline</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Ungroup the selected group</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select the line spacing mode.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Set the height of the first line of the text frame to use the tallest height of the included characters</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Set the height of the first line of text frame to use the full ascent of the font(s) in use</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Set the height of the first line of the text frame to the specified line height</source>
      <translation type="unfinished" />
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
      <source>Save the Python Commands in File</source>
      <translation>Tallenna Python-komennot tiedostoon</translation>
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
      <source>Open Python Script File</source>
      <translation>Avaa Python-skriptitiedosto</translation>
    </message>
    <message>
      <source>Python Scripts (*.py *.PY)</source>
      <translation>Python-skriptit (*.py *.PY)</translation>
    </message>
    <message>
      <source>Col: %1 Row: %2/%3</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Ctrl+O</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Ctrl+S</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Quit</source>
      <translation type="unfinished" >&amp;Lopeta</translation>
    </message>
    <message>
      <source>This is a standard Python console with some 
known limitations. Please consult the Scribus 
Scripter documentation for futher information. </source>
      <translation type="unfinished" />
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
      <translation>&amp;Fontti</translation>
    </message>
    <message>
      <source>Font st&amp;yle</source>
      <translation>Fontin &amp;tyyli</translation>
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
      <translation>Valitse fontti</translation>
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
      <source>Open</source>
      <translation>Avaa</translation>
    </message>
    <message>
      <source>Save as</source>
      <translation>Tallenna nimellä</translation>
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
      <translation>Fontti %1 on viallinen ja sitä ei käytetä</translation>
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
      <translation>fontin kokoa ei voi saada muusta kuin tekstikehyksestä.</translation>
    </message>
    <message>
      <source>Cannot get font of non-text frame.</source>
      <comment>python error</comment>
      <translation>Fonttia ei voi saada muusta kuin tekstikehyksestä.</translation>
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
      <translation>Kokoa ei voi skaalata 0 %:lla</translation>
    </message>
    <message>
      <source>Specified item not an image frame.</source>
      <comment>python error</comment>
      <translation>Valittu kohde ei ole kuvakehys.</translation>
    </message>
    <message>
      <source>Font not found.</source>
      <comment>python error</comment>
      <translation>Fonttia ei löydy.</translation>
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
      <translation>Fontin koko virheellinen. Täytyy olla 1 &lt;= koko &lt;= 512.</translation>
    </message>
    <message>
      <source>Cannot set font size on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Fontin koon voi asettaa vain tekstikehyksessä.</translation>
    </message>
    <message>
      <source>Cannot set font on a non-text frame.</source>
      <comment>python error</comment>
      <translation>Fontin voi asettaa vain tekstikehyksessä.</translation>
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
      <source>Font %1 is broken (read stream), no embedding</source>
      <translation>Fontti %1 on viallinen, ei upoteta</translation>
    </message>
    <message>
      <source>Font %1 is broken (no Face), discarding it</source>
      <translation>Fontti %1 on viallinen, ohitetaan se</translation>
    </message>
    <message>
      <source>Font %1 has broken glyph %2 (charcode %3)</source>
      <translation>Fontissa %1 on viallinen merkki %2 (merkkikoodi %3)</translation>
    </message>
    <message>
      <source>Font %1 cannot be read, no embedding</source>
      <translation>Fonttia %1 ei voida lukea, ei upotusta</translation>
    </message>
    <message>
      <source>Failed to load font %1 - font type unknown</source>
      <translation>Fontin %1 lataus epäonnistui, tuntematon tiedostotyyppi</translation>
    </message>
    <message>
      <source>Font %1 loaded from %2(%3)</source>
      <translation>Fontti %1 ladattiin %2(%3)</translation>
    </message>
    <message>
      <source>Font %1(%2) is duplicate of %3</source>
      <translation>Fontti %1(%2) on kopio tyypistä %3</translation>
    </message>
    <message>
      <source>Loading font %1 (found using fontconfig)</source>
      <translation>Ladataan fonttia %1 (löydettiin fontconfigilla)</translation>
    </message>
    <message>
      <source>Failed to load a font - freetype2 couldn't find the font file</source>
      <translation>Fontin lataus epäonnistui. Freetype2 ei löytänyt tiedostoa</translation>
    </message>
    <message>
      <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
      <translation>haetaan perhettä %1 fontista %1 (siirtymä=%3, nTaulu=%4)</translation>
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
      <translation>Luodaan välimuistia fonteille</translation>
    </message>
    <message>
      <source>New Font found, checking...</source>
      <translation>Uusi fontti löydetty, tarkastetaan...</translation>
    </message>
    <message>
      <source>Modified Font found, checking...</source>
      <translation>Muokattu fontti löytyi, tarkastetaan...</translation>
    </message>
    <message>
      <source>Reading Font Cache</source>
      <translation>Luetaan fonttivälimuistia</translation>
    </message>
    <message>
      <source>Writing updated Font Cache</source>
      <translation>Päivitetään fonttivälimuistia</translation>
    </message>
    <message>
      <source>Searching for Fonts</source>
      <translation>Etsitään fontteja</translation>
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
      <translation>Fonttien esikatselu</translation>
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
    <message>
      <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
      <translation>OpenOffice.org Draw (*.sxd *.odg);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Word Documents</source>
      <translation>Word-asiakirjat</translation>
    </message>
    <message>
      <source>Palm PDB Documents</source>
      <comment>PDB Importer</comment>
      <translation>Palm PDB -asiakirjat</translation>
    </message>
    <message>
      <source>PDB_data</source>
      <comment>PDB Importer</comment>
      <translation>PDB_data</translation>
    </message>
    <message>
      <source>PDB Import</source>
      <comment>PDB Importer</comment>
      <translation>PDB-tuonti</translation>
    </message>
    <message>
      <source>Could not open file %1</source>
      <comment>PDB Importer</comment>
      <translation>Tiedostoa %1 ei voitu avata</translation>
    </message>
    <message>
      <source>Luxembourgish</source>
      <translation>Luxemburg</translation>
    </message>
    <message>
      <source>Japanese</source>
      <translation>Japani</translation>
    </message>
    <message>
      <source>Given master page name does not match any existing.</source>
      <comment>python error</comment>
      <translation>Annetulla mallisivun nimellä ei löydy mallisivua.</translation>
    </message>
    <message>
      <source>Arabic</source>
      <translation>arabia</translation>
    </message>
    <message>
      <source>Estonian</source>
      <translation>viro</translation>
    </message>
    <message>
      <source>Icelandic</source>
      <translation>islanti</translation>
    </message>
    <message>
      <source>%1 may be corrupted : missing resolution tags</source>
      <translation>%1 saattaa olla virheellinen. Tarkkuustiedot puuttuvat.</translation>
    </message>
    <message>
      <source>This file is not recognized as a PDB document. Please, report this as a bug if you are sure it is one.</source>
      <comment>PDB Importer</comment>
      <translation>Tiedostoa ei tunnistettu PDB-asiakirjaksi. Jos tiedät sen olevan sellainen, kirjoita virheraportti.</translation>
    </message>
    <message>
      <source>Breton</source>
      <translation>bretoni</translation>
    </message>
    <message>
      <source>English (American)</source>
      <translation>englanti (Amerikka)</translation>
    </message>
    <message>
      <source>English (Australian)</source>
      <translation>englanti (Australia)</translation>
    </message>
    <message>
      <source>%1 may be corrupted : missing or wrong resolution tags</source>
      <translation>%1 saattaa olla virheellinen: puuttuva tai väärä resoluutio</translation>
    </message>
    <message>
      <source>Dzongkha</source>
      <translation>dzongkha</translation>
    </message>
    <message>
      <source>Latin</source>
      <translation>latina</translation>
    </message>
    <message>
      <source>Romanian</source>
      <translation>romania</translation>
    </message>
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
      <source>Govt. Letter</source>
      <translation>Govt. Letter</translation>
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
      <translation>Medium</translation>
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
    <message>
      <source>The Font(s):
%1 are not available.
They have been replaced by &quot;Courier&quot;
Therefore the image may be not correct</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.x.  Saving the current file under 1.3.x renders it unable to be edited in Scribus 1.2.x versions. To preserve the ability to edit in 1.2.x, save this file under a different name and further edit the newly named file and the original will be untouched. Are you sure you wish to proceed with this operation?</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Font %1(%2) is broken</source>
      <translation>Fontti %1 (%2) on viallinen</translation>
    </message>
    <message>
      <source>Font %1 has broken glyph %2</source>
      <translation>Fontissa %1 on viallinen merkki %2</translation>
    </message>
    <message>
      <source>font %1 </source>
      <translation>fontti %1 </translation>
    </message>
    <message>
      <source>size %1 </source>
      <translation>koko %1 </translation>
    </message>
    <message>
      <source>+style </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>+color </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>+underline </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>-underline </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>+strikeout </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>-strikeout </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>+shadow </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>-shadow </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>+outline </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>-outline </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>+tracking %1 </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>-tracking </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>+baseline %1 </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>+stretch </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>parent= %1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Transparency out of bounds, must be 0 &lt;= transparency &lt;= 1.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Blendmode out of bounds, must be 0 &lt;= blendmode &lt;= 15.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Cannot get number of lines of non-text frame.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Scribus 1.2.x Support</source>
      <translation>Scribus 1.2.x -yhteensopivuus</translation>
    </message>
    <message>
      <source>Scribus 1.3.4 Support</source>
      <translation>Scribus 1.3.4 -yhteensopivuus</translation>
    </message>
    <message>
      <source>German (Swiss)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Hebrew</source>
      <translation type="unfinished" >Heprealaiset merkit</translation>
    </message>
    <message>
      <source>Font %1 has broken metrics in file %2, ignoring metrics</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Chinese (Trad.)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Scribus 1.3.0->1.3.3.x Support</source>
      <translation type="unfinished" >Scribus 1.2.x -yhteensopivuus {1.3.0->1.3.3.?}</translation>
    </message>
    <message>
      <source>Copy of %1 (%2)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>%</source>
      <translation type="unfinished" >%</translation>
    </message>
    <message>
      <source>Khmer</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Lao</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Vietnamese</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>An error occurred while initializing icc transforms</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Output profile is not supported</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>WMF Import</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>New Layer</source>
      <translation type="unfinished" >Uusi taso</translation>
    </message>
    <message>
      <source>Adobe Illustrator Importer</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Imposition</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PostScript Importer</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>%1;;All Files (*)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Do you really want to overwrite the file:
%1 ?</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Encapsulated PostScript</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>GIF</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>JPEG</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Pattern Files</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PDF Document</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PNG</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PostScript</source>
      <translation type="unfinished" >PostScript</translation>
    </message>
    <message>
      <source>Adobe Photoshop</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>TIFF</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>XPM</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Windows Meta File</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Scalable Vector Graphics</source>
      <translation type="unfinished" >Scalable Vector Graphics</translation>
    </message>
    <message>
      <source>Adobe Illustrator</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Valid metrics were found for font %1, using metrics in file %2</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>No metrics found for font %1, ignoring font</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Error</source>
      <translation type="unfinished" >Virhe</translation>
    </message>
    <message>
      <source>Configfile %1 not found or the file is not readable</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>second argument is not tuple: must be tuple of int values.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>second argument contains non-numeric values: must be list of int values.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Cannot have an empty paragraph style name.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>hasdropcap has to be 0 or 1.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Cannot have an empty char style name.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Can only hyphenate text frame</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Can only dehyphenate text frame</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Lens Effects</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PathCutter</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PathFinder</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PathStroker</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Spell check (aspell)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Subdivide</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Xfig Importer</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Xfig File</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Norwegian (Bokmål)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Parsing the configfile %1 failed! Depending on the type of the error render frames might not work correctly!
%2</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>MeshDistortion</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PathAlongPath</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Transform Effect</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>°</source>
      <comment>degrees, unicode 0xB0</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Cannot get text distances of non-text frame.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Text distances out of bounds, must be positive.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Cannot set text distances on a non-text frame.</source>
      <comment>python error</comment>
      <translation type="unfinished" />
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
      <translation>Fontit</translation>
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
      <source>Preflight Verifier</source>
      <translation>Tarkastustyökalu</translation>
    </message>
    <message>
      <source>Sections</source>
      <translation>Alueet</translation>
    </message>
    <message>
      <source>Adjusting Colors</source>
      <translation>Värien säätäminen</translation>
    </message>
  </context>
  <context>
    <name>RunScriptDialog</name>
    <message>
      <source>Python Scripts (*.py *.PY);; All Files (*)</source>
      <translation>Python-skriptit (*.py *.PY);; Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Dialog</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Run as Extension Script</source>
      <translation type="unfinished" >Aja laajennusskriptinä</translation>
    </message>
  </context>
  <context>
    <name>SMAlignSelect</name>
    <message>
      <source>P</source>
      <comment>P as in Parent</comment>
      <translation>V</translation>
    </message>
    <message>
      <source>Use parent style's alignment instead of overriding it</source>
      <translation>Käytä vanhemman tasausasetuksia niiden ohittamisen sijaan</translation>
    </message>
  </context>
  <context>
    <name>SMCStyleWidget</name>
    <message>
      <source>Tracking</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Language</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Based On:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Language:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Shade</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Basic Formatting</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source> %</source>
      <translation type="unfinished" > %</translation>
    </message>
    <message>
      <source>Advanced Formatting</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>TextLabel</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Colors</source>
      <translation type="unfinished" >Värit</translation>
    </message>
    <message>
      <source>Parent Style</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Font Face</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Font Size</source>
      <translation type="unfinished" >Fontin koko</translation>
    </message>
    <message>
      <source>Baseline Offset</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Horizontal Scaling</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Vertical Scaling</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Fill Color</source>
      <translation type="unfinished" >Täyttöväri</translation>
    </message>
    <message>
      <source>Fill Shade</source>
      <translation type="unfinished" >Täytön sävy</translation>
    </message>
    <message>
      <source>Stroke Color</source>
      <translation type="unfinished" >Viivan väri</translation>
    </message>
    <message>
      <source>Stroke Shade</source>
      <translation type="unfinished" >Reunan sävy</translation>
    </message>
  </context>
  <context>
    <name>SMCharacterStyle</name>
    <message>
      <source>Properties</source>
      <translation>Ominaisuudet</translation>
    </message>
    <message>
      <source>Character Styles</source>
      <translation>Merkkityylit</translation>
    </message>
    <message>
      <source>Character Style</source>
      <translation>Merkkityyli</translation>
    </message>
    <message>
      <source>New Style</source>
      <translation>Uusi tyyli</translation>
    </message>
    <message>
      <source>Clone of %1</source>
      <translation>Kopio-%1</translation>
    </message>
    <message>
      <source>%1 (%2)</source>
      <comment>This for unique name when creating a new character style. %1 will be the name of the style and %2 will be a number forming a style name like: New Style (2)</comment>
      <translation>%1 (%2)</translation>
    </message>
  </context>
  <context>
    <name>SMColorCombo</name>
    <message>
      <source>Use Parent Value</source>
      <translation>Peri vanhemmalta</translation>
    </message>
  </context>
  <context>
    <name>SMFontComboH</name>
    <message>
      <source>Use Parent Font</source>
      <translation>Käytä vanhemman fonttia</translation>
    </message>
  </context>
  <context>
    <name>SMLineStyle</name>
    <message>
      <source>Properties</source>
      <translation>Ominaisuudet</translation>
    </message>
    <message>
      <source>Line Styles</source>
      <translation>Viivatyylit</translation>
    </message>
    <message>
      <source>Line Style</source>
      <translation>Viivatyyli</translation>
    </message>
    <message>
      <source>New Style</source>
      <translation>Uusi tyyli</translation>
    </message>
    <message>
      <source>Clone of %1</source>
      <translation>Kopio-%1</translation>
    </message>
    <message>
      <source>%1 (%2)</source>
      <comment>This for unique name when creating a new character style. %1 will be the name of the style and %2 will be a number forming a style name like: New Style (2)</comment>
      <translation>%1 (%2)</translation>
    </message>
    <message>
      <source> pt</source>
      <translation> pt</translation>
    </message>
    <message>
      <source>Solid Line</source>
      <translation>Yhtenäinen viiva</translation>
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
      <source>Dash Dot Line</source>
      <translation>Viiva-piste-viiva</translation>
    </message>
    <message>
      <source>Dash Dot Dot Line</source>
      <translation>Viiva-piste-piste-viiva</translation>
    </message>
    <message>
      <source> pt </source>
      <translation> pt </translation>
    </message>
  </context>
  <context>
    <name>SMLineStyleWidget</name>
    <message>
      <source>Flat Cap</source>
      <translation type="unfinished" >Litteä</translation>
    </message>
    <message>
      <source>Square Cap</source>
      <translation type="unfinished" >Neliö</translation>
    </message>
    <message>
      <source>Round Cap</source>
      <translation type="unfinished" >Pyöreä</translation>
    </message>
    <message>
      <source>Miter Join</source>
      <translation type="unfinished" >Viisto</translation>
    </message>
    <message>
      <source>Bevel Join</source>
      <translation type="unfinished" >Tasainen</translation>
    </message>
    <message>
      <source>Round Join</source>
      <translation type="unfinished" >Pyöreä</translation>
    </message>
    <message>
      <source>Add a new line</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Remove a line</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Line style</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Line width</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>End style</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Join style</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Line color</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Line shade</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source> pt</source>
      <translation type="unfinished" > pt</translation>
    </message>
    <message>
      <source>%</source>
      <translation type="unfinished" >%</translation>
    </message>
    <message>
      <source>Line Width:</source>
      <translation type="unfinished" >Viivan leveys:</translation>
    </message>
  </context>
  <context>
    <name>SMPStyleWidget</name>
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
      <source>Distances and Alignment</source>
      <translation>Etäisyydet ja tasaus</translation>
    </message>
    <message>
      <source>Drop Caps</source>
      <translation>Anfangit</translation>
    </message>
    <message>
      <source>Tabulators and Indentation</source>
      <translation>Sarkaimet ja sisennykset</translation>
    </message>
    <message>
      <source>Properties</source>
      <translation>Ominaisuudet</translation>
    </message>
    <message>
      <source>Character Style</source>
      <translation>Merkkityyli</translation>
    </message>
    <message>
      <source>&amp;Lines:</source>
      <translation>R&amp;ivejä:</translation>
    </message>
    <message>
      <source>Distance from Text:</source>
      <translation type="unfinished" >Etäisyys tekstistä</translation>
    </message>
    <message>
      <source>Alignment</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Parent Style</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Line Spacing Mode</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Line Spacing</source>
      <translation type="unfinished" >Riviväli</translation>
    </message>
    <message>
      <source>Space Above</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Space Below</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Drop Cap Lines</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Drop Cap Offset</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>First Line Indent</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Left Indent</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Right Indent</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Based On:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>TextLabel</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Parent's Drop Cap Status</source>
      <translation type="unfinished" >Vanhemman anfangi</translation>
    </message>
    <message>
      <source>Ch&amp;aracter Style</source>
      <translation type="unfinished" >&amp;Merkkityyli</translation>
    </message>
  </context>
  <context>
    <name>SMParagraphStyle</name>
    <message>
      <source>Paragraph Styles</source>
      <translation>Kappaletyylit</translation>
    </message>
    <message>
      <source>Paragraph Style</source>
      <translation>Kappaletyyli</translation>
    </message>
    <message>
      <source>New Style</source>
      <translation type="unfinished" >Uusi tyyli</translation>
    </message>
    <message>
      <source>Clone of %1</source>
      <translation>Kopio-%1</translation>
    </message>
    <message>
      <source>%1 (%2)</source>
      <comment>This for unique name when creating a new character style. %1 will be the name of the style and %2 will be a number forming a style name like: New Style (2)</comment>
      <translation>%1 (%2)</translation>
    </message>
  </context>
  <context>
    <name>SMReplaceDia</name>
    <message>
      <source>Remove</source>
      <translation>Poista</translation>
    </message>
    <message>
      <source>Replace with</source>
      <translation>Korvaava</translation>
    </message>
    <message>
      <source>Delete Styles</source>
      <translation type="unfinished" >Poista tyylit</translation>
    </message>
    <message>
      <source>&amp;OK</source>
      <translation type="unfinished" >&amp;OK</translation>
    </message>
    <message>
      <source>Ca&amp;ncel</source>
      <translation type="unfinished" >Pe&amp;ruuta</translation>
    </message>
    <message>
      <source>Alt+N</source>
      <translation type="unfinished" >Alt+N</translation>
    </message>
  </context>
  <context>
    <name>SMRowWidget</name>
    <message>
      <source>No Style</source>
      <translation>Ei tyyliä</translation>
    </message>
  </context>
  <context>
    <name>SMScComboBox</name>
    <message>
      <source>Use Parent Value</source>
      <translation>Peri vanhemmalta</translation>
    </message>
  </context>
  <context>
    <name>SMShadeButton</name>
    <message>
      <source>Use Parent Value</source>
      <translation>Peri vanhemmalta</translation>
    </message>
  </context>
  <context>
    <name>SMStyleImport</name>
    <message>
      <source>Character Styles</source>
      <translation type="unfinished" >Merkkityylit</translation>
    </message>
    <message>
      <source>Paragraph Styles</source>
      <translation type="unfinished" >Kappaletyylit</translation>
    </message>
    <message>
      <source>Line Styles</source>
      <translation type="unfinished" >Viivatyylit</translation>
    </message>
    <message>
      <source>Choose Styles</source>
      <translation type="unfinished" >Valitse tyylit</translation>
    </message>
    <message>
      <source>Available Styles</source>
      <translation type="unfinished" >Käytettävissä olevat tyylit</translation>
    </message>
    <message>
      <source>&amp;Rename Imported Style</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>R&amp;eplace Existing Style</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select or Unselect All</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>In case of a name clash</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>SMStyleSelect</name>
    <message>
      <source>P</source>
      <comment>P as in Parent</comment>
      <translation>V</translation>
    </message>
    <message>
      <source>Use parent style's effects instead of overriding them</source>
      <translation type="unfinished" >Käytä vanhemman efektejä</translation>
    </message>
  </context>
  <context>
    <name>SMTabruler</name>
    <message>
      <source> Parent Tabs </source>
      <translation> Vanhemman sarkaimet</translation>
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
      <source>Font of selected text</source>
      <translation>Valitun tekstin fontti</translation>
    </message>
    <message>
      <source>Font Size</source>
      <translation>Fontin koko</translation>
    </message>
    <message>
      <source>Scaling width of characters</source>
      <translation>Merkkien leveyssuuntainen skaalaus</translation>
    </message>
    <message>
      <source>Font Settings</source>
      <translation>Fontin asetukset</translation>
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
      <translation>Fontin asetukset</translation>
    </message>
    <message>
      <source>Manual Tracking</source>
      <translation>Oma merkkiväli</translation>
    </message>
  </context>
  <context>
    <name>SVGExportPlugin</name>
    <message>
      <source>Exports SVG Files</source>
      <translation>Vie SVG-tiedostoja</translation>
    </message>
    <message>
      <source>Exports the current page into an SVG file.</source>
      <translation>Vie nykyinen sivu SVG-tiedostoon.</translation>
    </message>
    <message>
      <source>Save as &amp;SVG...</source>
      <translation>Tallenna &amp;SVG-tiedostoksi</translation>
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
      <source>SVG file contains some unsupported features</source>
      <translation>SVG-tiedosto sisältää joitain toimintoja, joita Scribus ei tue</translation>
    </message>
    <message>
      <source>The file could not be imported</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>SVGPlug</name>
    <message>
      <source>Group%1</source>
      <translation type="unfinished" />
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
    <message>
      <source>User configuration exists already. Do you really want to overwrite it?</source>
      <translation type="unfinished" />
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
    <name>ScGTFileDialog</name>
    <message>
      <source>Select a file to import</source>
      <translation>Valitse tuotava tiedosto</translation>
    </message>
    <message>
      <source>Append</source>
      <translation>Jatka</translation>
    </message>
    <message>
      <source>Show options</source>
      <translation>Näytä asetukset</translation>
    </message>
  </context>
  <context>
    <name>ScInputDialog</name>
    <message>
      <source>Input Dialog</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>InputDialog</source>
      <translation type="unfinished" />
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
    <name>ScPrintEngine_GDI</name>
    <message>
      <source>Printing...</source>
      <translation type="unfinished" >Tulostaa...</translation>
    </message>
  </context>
  <context>
    <name>ScProgressBar</name>
    <message>
      <source>%1 of %2</source>
      <translation>%1/%2</translation>
    </message>
  </context>
  <context>
    <name>Scribus12Format</name>
    <message>
      <source>Scribus 1.2.x Document</source>
      <translation>Scribus 1.2.x -asiakirja</translation>
    </message>
    <message>
      <source>Copy #%1 of </source>
      <translation type="unfinished" >Kopio #%1/</translation>
    </message>
  </context>
  <context>
    <name>Scribus134Format</name>
    <message>
      <source>Scribus 1.3.4 Document</source>
      <translation>Scribus 1.3.4 -asiakirja</translation>
    </message>
    <message>
      <source>Copy #%1 of </source>
      <translation>Kopio #%1/</translation>
    </message>
  </context>
  <context>
    <name>Scribus13Format</name>
    <message>
      <source>Copy #%1 of </source>
      <translation>Kopio #%1/</translation>
    </message>
    <message>
      <source>Scribus 1.3.0->1.3.3.7 Document</source>
      <translation type="unfinished" >Scribus 1.3.0->1.3.3.7 -asiakirja</translation>
    </message>
  </context>
  <context>
    <name>ScribusCore</name>
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
      <source>Searching for Fonts</source>
      <translation>Etsitään fontteja</translation>
    </message>
    <message>
      <source>There are no fonts found on your system.</source>
      <translation>Järjestelmästä ei löytynyt fontteja</translation>
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
      <translation>Fontit alustettu</translation>
    </message>
    <message>
      <source>Reading Color Profiles</source>
      <translation type="unfinished" />
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
      <source>Do you really want to clear all your text?</source>
      <translation>Haluatko poistaa kaiken tekstin?</translation>
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
      <source>An error occurred while opening ICC profiles, color management is not enabled.</source>
      <translation>ICC-profiilien avaamisessa tapahtui virhe. Värienhallintaa ei otettu käyttöön.</translation>
    </message>
    <message>
      <source>Adjusting Colors</source>
      <translation>Säädetään värejä</translation>
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
      <source>&amp;Skip locked objects</source>
      <translation>&amp;Ohita lukitut kohteet</translation>
    </message>
    <message>
      <source>Number of copies: %1
Horizontal gap: %2
Vertical gap: %3</source>
      <translation>kopioiden lukumäärä: %1\n
vaakaväli: %2\n
pystyväli: %3</translation>
    </message>
    <message>
      <source>remove direct paragraph formatting</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>remove direct char formatting</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3
Rotation: %4</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Group%1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Copy_of_</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Imported </source>
      <comment>Prefix of imported default style</comment>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>ScribusMainWindow</name>
    <message>
      <source>Initializing Story Editor</source>
      <translation>Alustetaan tekstieditoria</translation>
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
      <source>Fatal Error</source>
      <translation>Vakava virhe</translation>
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
      <source> was replaced by: </source>
      <translation>korvattiin tyypillä: </translation>
    </message>
    <message>
      <source>(converted)</source>
      <translation>(muutettu)</translation>
    </message>
    <message>
      <source>Cannot write the file: 
%1</source>
      <translation>Tiedostoon ei voi kirjoittaa:
%1</translation>
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
    <message>
      <source>Do you really want to replace your existing image?</source>
      <translation>Haluatko korvata nykyisen kuvan?</translation>
    </message>
    <message>
      <source>Contents</source>
      <translation>Sisältö</translation>
    </message>
    <message>
      <source>Liga&amp;ture</source>
      <translation>Liga&amp;tuurit</translation>
    </message>
    <message>
      <source>Paste Recent</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Character</source>
      <translation>&amp;Merkki</translation>
    </message>
    <message>
      <source>&amp;Quote</source>
      <translation>&amp;Lainausmerkki</translation>
    </message>
    <message>
      <source>S&amp;paces &amp;&amp; Breaks</source>
      <translation type="unfinished" >&amp;Välit ja katkot</translation>
    </message>
    <message>
      <source>Documents (*.sla *.sla.gz);;All Files (*)</source>
      <translation>Asiakirjat (*.sla *.sla.gz);;Kaikki tiedostot (*)</translation>
    </message>
    <message>
      <source>Do you really want to clear all your text?</source>
      <translation type="unfinished" >Haluatko poistaa kaiken tekstin?</translation>
    </message>
    <message>
      <source>Scribus </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Online &amp;Tutorials</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Some color profiles used by this document are not installed:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>%1;;All Files (*)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Applying User Shortcuts</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Updating Images</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Your document was saved to a temporary file and could not be moved: 
%1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Print engine initialization failed</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>All Supported Formats</source>
      <translation type="unfinished" >Kaikki tuetut tiedostotyypit</translation>
    </message>
    <message>
      <source>&amp;Name:</source>
      <translation type="unfinished" >&amp;Nimi:</translation>
    </message>
    <message>
      <source>New Entry</source>
      <translation type="unfinished" >Uusi leike</translation>
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
      <translation>Näytä tietoa fonteista niitä ladattaessa</translation>
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
    <message>
      <source>Display a console window</source>
      <translation>Näytä konsoli-ikkuna</translation>
    </message>
    <message>
      <source>Stop the showing of the splashscreen on startup. Writes an empty file called .neversplash in ~/.scribus.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Download a file from the Scribus website and show the latest available version.</source>
      <translation>Hae tiedosto Scribuksen sivuilta ja näytä viimeisin saatavilla oleva versio</translation>
    </message>
    <message>
      <source>Show location ICC profile information on console while starting</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Invalid argument: %1</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>ScribusView</name>
    <message>
      <source> %</source>
      <translation> %</translation>
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
      <source>Colorspace: </source>
      <translation>Väriavaruus: </translation>
    </message>
    <message>
      <source>Unknown</source>
      <translation>Tuntematon</translation>
    </message>
    <message>
      <source>Linking Text Frames</source>
      <translation>Tekstikehysten linkitys</translation>
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
    <message>
      <source>Contents</source>
      <translation>Sisältö</translation>
    </message>
    <message>
      <source>Preview Mode</source>
      <translation>Esikatselutila</translation>
    </message>
    <message>
      <source>Paste Recent</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Group%1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>CMS is active. Therefore the color display may not match the perception by visually impaired</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Enter Object Size</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>No Image Loaded</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>You are trying to link a frame to itself.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>You are trying to link a frame which is already linked.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Page: </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Pages: </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Enable/disable Color Management</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Enable/disable the Preview Mode</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select the visual appearance of the display. You can choose between normal and several color blindness forms</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Configure CMS...</source>
      <translation type="unfinished" />
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
      <translation>Python-liitännäisen alustus epäonnistui. Yksityiskohdat tulostettiin stderr:iin.</translation>
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
    <message>
      <source>Python Scripts (*.py *.PY);;All Files (*)</source>
      <translation>Python-skriptit (*.py *.PY);;Kaikki tiedostot (*)</translation>
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
      <source>Change...</source>
      <translation>Muuta...</translation>
    </message>
    <message>
      <source>Locate Startup Script</source>
      <translation>Etsi aloitusskripti</translation>
    </message>
    <message>
      <source>Form</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Comments:</source>
      <translation type="unfinished" >Kommentit:</translation>
    </message>
    <message>
      <source>Keywords:</source>
      <translation type="unfinished" >Avainsanat:</translation>
    </message>
    <message>
      <source>Signs:</source>
      <translation type="unfinished" >Merkit:</translation>
    </message>
    <message>
      <source>Strings:</source>
      <translation type="unfinished" >Merkkijonot:</translation>
    </message>
    <message>
      <source>Numbers:</source>
      <translation type="unfinished" >Numerot:</translation>
    </message>
    <message>
      <source>Errors:</source>
      <translation type="unfinished" >Virheet:</translation>
    </message>
    <message>
      <source>Base Texts:</source>
      <translation type="unfinished" >Perustekstit:</translation>
    </message>
  </context>
  <context>
    <name>SeList</name>
    <message>
      <source>Show Page Previews</source>
      <translation>Näytä sivun esikatselu</translation>
    </message>
    <message>
      <source>Delete Master Page?</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Are you sure you want to delete this master page?</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>SeView</name>
    <message>
      <source>Delete Page?</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Are you sure you want to delete this page?</source>
      <translation type="unfinished" />
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
      <source>Font</source>
      <translation>Fontti</translation>
    </message>
    <message>
      <source>Font Size</source>
      <translation>Fontin koko</translation>
    </message>
    <message>
      <source>Font Effects</source>
      <translation>Fonttien tehosteet</translation>
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
    <message>
      <source>Alignment</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Search for text or formatting in the current text</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Replace the searched for formatting with the replacement values</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Replace all found instances</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Clear all search and replace options</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Close search and replace</source>
      <translation type="unfinished" />
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
    <message>
      <source>Special plug-in for adding non-breaking spaces before or after so called short words. Available in the following languages: </source>
      <translation>Liitännäinen sitovien välilyöntien lisäämiseksi ennen ja jälkeen listattuja sanoja. Saatavilla seuraaville kielille:</translation>
    </message>
  </context>
  <context>
    <name>ShortcutWidget</name>
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
      <source>Alt</source>
      <translation>Alt</translation>
    </message>
    <message>
      <source>Ctrl</source>
      <translation>Ctrl</translation>
    </message>
    <message>
      <source>Shift</source>
      <translation>Shift</translation>
    </message>
    <message>
      <source>Meta</source>
      <translation>Meta</translation>
    </message>
    <message>
      <source>Meta+</source>
      <translation>Meta+</translation>
    </message>
    <message>
      <source>Shift+</source>
      <translation>Shift+</translation>
    </message>
    <message>
      <source>Alt+</source>
      <translation>Alt+</translation>
    </message>
    <message>
      <source>Ctrl+</source>
      <translation>Ctrl+</translation>
    </message>
    <message>
      <source>No shortcut for the style</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Style has user defined shortcut</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Assign a shortcut for the style</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>SideBar</name>
    <message>
      <source>No Style</source>
      <translation type="unfinished" >Ei tyyliä</translation>
    </message>
    <message>
      <source>Edit Styles...</source>
      <translation type="unfinished" >Muokkaa tyylejä...</translation>
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
      <translation>&amp;Käytettävä fontti...</translation>
    </message>
    <message>
      <source>&amp;Settings</source>
      <translation>As&amp;etukset</translation>
    </message>
    <message>
      <source>&amp;Fonts Preview...</source>
      <translation>&amp;Fonttien esikatselu...</translation>
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
    <message>
      <source>&amp;Insert</source>
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
      <source>Ligature</source>
      <translation>Ligatuuri</translation>
    </message>
    <message>
      <source>Spaces &amp;&amp; Breaks</source>
      <translation>Välit ja katkot</translation>
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
    <name>StyleManager</name>
    <message>
      <source>Name</source>
      <translation>Nimi</translation>
    </message>
    <message>
      <source>Shortcut</source>
      <translation>Näppäinyhdistelmä</translation>
    </message>
    <message>
      <source>Name:</source>
      <translation>Nimi:</translation>
    </message>
    <message>
      <source>&amp;Reset</source>
      <translation>&amp;Palauta</translation>
    </message>
    <message>
      <source>&amp;Apply</source>
      <translation>&amp;Käytä</translation>
    </message>
    <message>
      <source>&lt;&lt; &amp;Done</source>
      <translation type="unfinished" >&lt;&lt; &amp;Valmis</translation>
    </message>
    <message>
      <source>&amp;New</source>
      <translation>&amp;Uusi</translation>
    </message>
    <message>
      <source>&amp;Import</source>
      <translation>&amp;Tuo</translation>
    </message>
    <message>
      <source>&amp;Clone</source>
      <translation>&amp;Kopioi</translation>
    </message>
    <message>
      <source>&amp;Delete</source>
      <translation>&amp;Poista</translation>
    </message>
    <message>
      <source>Reset all changes</source>
      <translation>Palauta kaikki muutokset</translation>
    </message>
    <message>
      <source>Apply all changes</source>
      <translation>Ota muutokset käyttöön</translation>
    </message>
    <message>
      <source>Apply all changes and exit edit mode</source>
      <translation>Ota muutokset käyttöön ja poistu muokkaustilasta</translation>
    </message>
    <message>
      <source>Create a new style</source>
      <translation>Luo uusi tyyli</translation>
    </message>
    <message>
      <source>Import styles from another document</source>
      <translation>Tuo tyylejä toisesta asiakirjasta</translation>
    </message>
    <message>
      <source>Clone selected style</source>
      <translation>Kopioi valittu tyyli</translation>
    </message>
    <message>
      <source>Delete selected styles</source>
      <translation>Poista valittu tyyli</translation>
    </message>
    <message>
      <source>New</source>
      <translation>Uusi</translation>
    </message>
    <message>
      <source>Import</source>
      <translation>Tuo</translation>
    </message>
    <message>
      <source>Edit</source>
      <translation>Muokkaa</translation>
    </message>
    <message>
      <source>Clone</source>
      <translation>Kopioi</translation>
    </message>
    <message>
      <source>Send to Scrapbook</source>
      <translation>Vie leikekirjaan</translation>
    </message>
    <message>
      <source>Delete</source>
      <translation>Poista</translation>
    </message>
    <message>
      <source>New %1</source>
      <translation>Uusi %1</translation>
    </message>
    <message>
      <source>&amp;Edit</source>
      <translation>&amp;Muokkaa</translation>
    </message>
    <message>
      <source>&amp;Done</source>
      <translation>&amp;Valmis</translation>
    </message>
    <message>
      <source>This key sequence is already in use</source>
      <translation>Näppäinyhdistelmä on jo käytössä</translation>
    </message>
    <message>
      <source>Edit styles</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Name of the selected style</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>More than one style selected</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Open</source>
      <translation type="unfinished" >Avaa</translation>
    </message>
    <message>
      <source>documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Alt+N</source>
      <translation type="unfinished" >Alt+N</translation>
    </message>
    <message>
      <source>Clone copies the style to make similar styles easily.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Alt+C</source>
      <translation type="unfinished" >Alt+C</translation>
    </message>
    <message>
      <source>Alt+I</source>
      <translation type="unfinished" >Alt+I</translation>
    </message>
    <message>
      <source>Alt+D</source>
      <translation type="unfinished" >Alt+D</translation>
    </message>
    <message>
      <source>Please select a unique name for the style</source>
      <translation type="unfinished" >Valitse tyylille nimi, joka ei ole käytössä</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation type="unfinished" >Alt+A</translation>
    </message>
    <message>
      <source>Alt+R</source>
      <translation type="unfinished" >Alt+R</translation>
    </message>
    <message>
      <source>Style Manager</source>
      <translation type="unfinished" >Tyylit</translation>
    </message>
  </context>
  <context>
    <name>StyleSelect</name>
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
      <source>All Caps</source>
      <translation>Kaikki versaalina</translation>
    </message>
    <message>
      <source>Underline Text. Hold down the button momentarily to set line width and displacement options.</source>
      <translation>Alleviivaa tekstiä. Pidä näppäin alhaalla asettaaksesi viivan leveyden ja sijainnin.</translation>
    </message>
    <message>
      <source>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</source>
      <translation>Alleviivaa ainoastaan sanoja. Pidä näppäin alhaalla asettaaksesi viivan leveyden ja sijainnin.</translation>
    </message>
    <message>
      <source>Strike Out. Hold down the button momentarily to set line width and displacement options.</source>
      <translation>Yliviivaus. Pidä näppäin alhaalla asettaaksesi viivan leveyden ja sijainnin.</translation>
    </message>
    <message>
      <source>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</source>
      <translation>Teksti varjolla. Pidä näppäin alhaalla asettaaksesi varjon sijainnin.

</translation>
    </message>
    <message>
      <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
      <comment>Text Style Selector</comment>
      <translation>Ääriviivat. Pidä näppäin alhaalla asettaaksesi viivan leveyden</translation>
    </message>
  </context>
  <context>
    <name>SubdividePlugin</name>
    <message>
      <source>Subdivide Path</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Path Tools</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Subdivide</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Subdivide selected Path</source>
      <translation type="unfinished" />
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
      <source>Table of Contents %1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Page Number Placement:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Beginning</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>End</source>
      <translation type="unfinished" />
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
      <source>Check for overflow in text frames</source>
      <translation>Tarkasta tekstikehysten ylivuoto</translation>
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
    <message>
      <source>Highest allowed resolution</source>
      <translation>Suurin sallittu resoluutio</translation>
    </message>
    <message>
      <source>Check for GIF images</source>
      <translation>Tarkasta GIF-kuvat</translation>
    </message>
    <message>
      <source>Ignore non-printable Layers</source>
      <translation>Ohita tulostumattomat tasot</translation>
    </message>
    <message>
      <source>Check for items not on a page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Check for used transparencies</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>TabDisplay</name>
    <message>
      <source>Color for paper</source>
      <translation>Paperin väri</translation>
    </message>
    <message>
      <source>Mask the area outside the margins in the margin color</source>
      <translation>Merkkaa reunusten ulkopuolinen alue reunuksen värillä</translation>
    </message>
    <message>
      <source>Enable or disable  the display of linked frames.</source>
      <translation type="unfinished" >Näytä tai piilota linkitetyt kehykset.</translation>
    </message>
    <message>
      <source>Display non-printing characters such as paragraph markers in text frames</source>
      <translation>Näytä tulostumattomat merkit kuten kappalemerkki tekstikehyksissä</translation>
    </message>
    <message>
      <source>Turns the display of frames on or off</source>
      <translation type="unfinished" >Näytä tai piilota kehykset</translation>
    </message>
    <message>
      <source>Turns the display of layer indicators on or off</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
      <translation type="unfinished" >Määrittelee tyhjän tilan määrän asiakirjan vasemmalla puolella, jota voi käyttää objektien säilytykseen ja työskentelyalueena sivua tehtäessä.</translation>
    </message>
    <message>
      <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
      <translation type="unfinished" >Määrittelee tyhjän tilan määrän asiakirjan oikealla puolella, jota voi käyttää objektien säilytykseen ja työskentelyalueena sivua tehtäessä.</translation>
    </message>
    <message>
      <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
      <translation type="unfinished" >Määrittelee tyhjän tilan määrän asiakirjan yläpuolella, jota voi käyttää objektien säilytykseen ja työskentelyalueena sivua tehtäessä.</translation>
    </message>
    <message>
      <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
      <translation type="unfinished" >Määrittelee tyhjän tilan määrän asiakirjan alapuolella, jota voi käyttää objektien säilytykseen ja työskentelyalueena sivua tehtäessä.</translation>
    </message>
    <message>
      <source>Set the default zoom level</source>
      <translation>Aseta sivun näytettävä koko</translation>
    </message>
    <message>
      <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
      <translation type="unfinished" >Aseta viivain näytölle ja säädä pituus oikeaksi. Tämä mahdollistaa sivujen ja objektien näytön oikean kokoisina.</translation>
    </message>
    <message>
      <source>TabDisplayBase</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>General</source>
      <translation type="unfinished" >Yleiset</translation>
    </message>
    <message>
      <source>Adjust Display Size</source>
      <translation type="unfinished" >Aseta näytön koko</translation>
    </message>
    <message>
      <source>Scale%</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>To adjust the display drag the ruler below with the slider.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Gaps Between Pages</source>
      <translation type="unfinished" >Sivujen etäisyys toisistaan</translation>
    </message>
    <message>
      <source>Vertical:</source>
      <translation type="unfinished" >Pystysuora:</translation>
    </message>
    <message>
      <source>Horizontal:</source>
      <translation type="unfinished" >Vaakasuora:</translation>
    </message>
    <message>
      <source>Scratch Space</source>
      <translation type="unfinished" >Sivujen ulkopuolinen alue</translation>
    </message>
    <message>
      <source>&amp;Bottom:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Top:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Right:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Left:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Page Display</source>
      <translation type="unfinished" >Sivun näyttö</translation>
    </message>
    <message>
      <source>Show Bleed Area</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Display &amp;Unprintable Area in Margin Color</source>
      <translation type="unfinished" >N&amp;äytä tulostumaton alue reunuksen värillä</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation type="unfinished" >Alt+U</translation>
    </message>
    <message>
      <source>Show Layer Indicators</source>
      <translation type="unfinished" >Näytä tasojen merkit</translation>
    </message>
    <message>
      <source>Show Frames</source>
      <translation type="unfinished" >Näytä kehykset</translation>
    </message>
    <message>
      <source>Show Text Chains</source>
      <translation type="unfinished" >Näytä tekstiketjut</translation>
    </message>
    <message>
      <source>Rulers Relative to Page</source>
      <translation type="unfinished" >Viivaimet suhteessa sivuun</translation>
    </message>
    <message>
      <source>Show Text Control Characters</source>
      <translation type="unfinished" >Näytä tulostumattomat merkit</translation>
    </message>
    <message>
      <source>Colors</source>
      <translation type="unfinished" >Värit</translation>
    </message>
    <message>
      <source>Pages:</source>
      <translation type="unfinished" >Sivut:</translation>
    </message>
    <message>
      <source>Selected Page Border:</source>
      <translation type="unfinished" >Valitun sivun korostus:</translation>
    </message>
    <message>
      <source>Fill Color:</source>
      <translation type="unfinished" >Täyttöväri:</translation>
    </message>
    <message>
      <source>Frames</source>
      <translation type="unfinished" >Kehykset</translation>
    </message>
    <message>
      <source>Grouped:</source>
      <translation type="unfinished" >Ryhmitetty:</translation>
    </message>
    <message>
      <source>Annotation:</source>
      <translation type="unfinished" >Huomautus:</translation>
    </message>
    <message>
      <source>Selected:</source>
      <translation type="unfinished" >Valittu:</translation>
    </message>
    <message>
      <source>Linked:</source>
      <translation type="unfinished" >Linkitetty:</translation>
    </message>
    <message>
      <source>Locked:</source>
      <translation type="unfinished" >Lukittu:</translation>
    </message>
    <message>
      <source>Normal:</source>
      <translation type="unfinished" >Normaali:</translation>
    </message>
    <message>
      <source>Text:</source>
      <translation type="unfinished" >Teksti:</translation>
    </message>
    <message>
      <source>Control Characters:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Turns the display of images on or off</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Show Images</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source> dpi</source>
      <translation type="unfinished" > dpi</translation>
    </message>
    <message>
      <source>Resets the scale to the default dpi of your display</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Turns the page shadow on or off</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Show Page Shadow</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>TabDocument</name>
    <message>
      <source>Page Size</source>
      <translation>Sivun koko</translation>
    </message>
    <message>
      <source>&amp;Size:</source>
      <translation>&amp;Koko:</translation>
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
      <source>Orie&amp;ntation:</source>
      <translation>Suu&amp;nta:</translation>
    </message>
    <message>
      <source>Units:</source>
      <translation>Yksiköt:</translation>
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
      <source>Apply settings to:</source>
      <translation>Käytä asetuksia:</translation>
    </message>
    <message>
      <source>All Document Pages</source>
      <translation>Kaikille asiakirjan sivuille</translation>
    </message>
    <message>
      <source>All Master Pages</source>
      <translation>Kaikille mallisivuille</translation>
    </message>
    <message>
      <source>Margin Guides</source>
      <translation>Reunukset</translation>
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
      <translation>A&amp;ikaväli:</translation>
    </message>
    <message>
      <source>Undo/Redo</source>
      <translation>Kumoa ja tee uudelleen</translation>
    </message>
    <message>
      <source>Action history length</source>
      <translation>Toimintohistorian pituus</translation>
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
      <source>Default page size, either a standard size or a custom size</source>
      <translation>Oletuskoko sivulle, joko standardi tai itse määritelty koko</translation>
    </message>
    <message>
      <source>Default orientation of document pages</source>
      <translation>Asiakirjan sivujen suunta</translation>
    </message>
    <message>
      <source>Default unit of measurement for document editing</source>
      <translation>Asiakirjan muokkauksessa käytettävä mittayksikkö</translation>
    </message>
    <message>
      <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
      <translation>Scribus tallentaa kopion asiakirjastasi bak-päätteellä aina kun valittu aikaväli edellisestä tallennuksesta täyttyy.</translation>
    </message>
    <message>
      <source>Time period between saving automatically</source>
      <translation>Automaattisen tallennuksen aikaväli</translation>
    </message>
    <message>
      <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
      <translation>Toimintohistorian pituus. 0 ei rajoita pituutta.</translation>
    </message>
    <message>
      <source>Apply the page size changes to all existing pages in the document</source>
      <translation>Käytä sivun koon asetuksia asiakirjan jokaiseen sivuun</translation>
    </message>
    <message>
      <source>Apply the page size changes to all existing master pages in the document</source>
      <translation>Käytä sivun koon muutosta kaikkiin asiakirjan mallisivuihin</translation>
    </message>
  </context>
  <context>
    <name>TabExternalToolsWidget</name>
    <message>
      <source>Locate Ghostscript</source>
      <translation>Ghostscriptin sijainti</translation>
    </message>
    <message>
      <source>Locate your image editor</source>
      <translation>Kuvanmuokkausohjelman sijainti</translation>
    </message>
    <message>
      <source>Locate your web browser</source>
      <translation>Web-selaimen sijainti</translation>
    </message>
    <message>
      <source>External Tools</source>
      <translation type="unfinished" >Ulkoiset työkalut</translation>
    </message>
    <message>
      <source>Web Browser to launch with links from the Help system</source>
      <translation type="unfinished" >Web-selain, johon ohjeitten linkit avataan</translation>
    </message>
    <message>
      <source>Web Browser</source>
      <translation type="unfinished" >Web-selain</translation>
    </message>
    <message>
      <source>&amp;Change...</source>
      <translation type="unfinished" >&amp;Muuta...</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation type="unfinished" >Alt+C</translation>
    </message>
    <message>
      <source>&lt;qt>File system location for your web browser. This is used for external links from the Help system.&lt;/qt></source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Name of &amp;Executable:</source>
      <translation type="unfinished" >&amp;Ajettavan tiedoston nimi:</translation>
    </message>
    <message>
      <source>Image Processing Tool</source>
      <translation type="unfinished" >Kuvanmuokkaustyökalu</translation>
    </message>
    <message>
      <source>&lt;qt>File system location for graphics editor. If you use gimp and your distribution includes it, we recommend 'gimp-remote', as it allows you to edit the image in an already running instance of gimp.&lt;/qt></source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PostScript Interpreter</source>
      <translation type="unfinished" >PostScript-tulkki</translation>
    </message>
    <message>
      <source>Antialias text for EPS and PDF onscreen rendering</source>
      <translation type="unfinished" >Käytä reunanpehmennystä EPS- ja PDF-tekstiin näytöllä</translation>
    </message>
    <message>
      <source>Antialias &amp;Text</source>
      <translation type="unfinished" >Reunanpehmennys &amp;tekstiin</translation>
    </message>
    <message>
      <source>Alt+T</source>
      <translation type="unfinished" >Alt+T</translation>
    </message>
    <message>
      <source>Antialias graphics for EPS and PDF onscreen rendering</source>
      <translation type="unfinished" >Käytä reunanpehmennystä EPS- ja PDF-grafiikkaan näytöllä</translation>
    </message>
    <message>
      <source>Antialias &amp;Graphics</source>
      <translation type="unfinished" >Reunanpehmennys &amp;grafiikkaan</translation>
    </message>
    <message>
      <source>Alt+G</source>
      <translation type="unfinished" >Alt+G</translation>
    </message>
    <message>
      <source>Resolution:</source>
      <translation type="unfinished" >Tarkkuus:</translation>
    </message>
    <message>
      <source> dpi</source>
      <translation type="unfinished" > dpi</translation>
    </message>
    <message>
      <source>&amp;Name of Executable:</source>
      <translation type="unfinished" >&amp;Ajettavan tiedoston nimi:</translation>
    </message>
    <message>
      <source>&lt;qt>Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.&lt;/qt></source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Rescan for the external tools if they do not exist in the already specified location</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Rescan</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Alt+R</source>
      <translation type="unfinished" >Alt+R</translation>
    </message>
    <message>
      <source>Start with empty frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Always use the configured DPI setting for calculating the size, even if the image file reports something different.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Force DPI</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&lt;qt>Path to the editor executable.&lt;/qt></source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Locate a Configuration file</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Configuration files</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Configurations:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Up</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Down</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Delete</source>
      <translation type="unfinished" >Poista</translation>
    </message>
    <message>
      <source>Locate your editor</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Render Frames</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>External Editor:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Add...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Change Path...</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>TabGeneral</name>
    <message>
      <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
      <translation type="unfinished" >Valitse Scribuksen käyttöliittymän kieli. Jos jätetään tyhjäksi, kieli valitaan ympäristömuuttujien perusteella. Valinta on mahdollista ohittaa käyttämällä komentoriviparametriä Scribusta käynnistettäessä.</translation>
    </message>
    <message>
      <source>Number of recently edited documents to show in the File menu</source>
      <translation>Tiedosto-valikossa näytettävien viimeksi käsiteltyjen asiakirjojen lukumäärä</translation>
    </message>
    <message>
      <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
      <translation>Askellettavien rivien lukumäärä hiiren rullaa käytettäessä</translation>
    </message>
    <message>
      <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
      <translation>Valitse ulkonäkö. Scribus mahdollistaa KDE:n ja Qt:n teemojen käytön, jos Qt on asetettu etsimään KDE-liitännäisiä.</translation>
    </message>
    <message>
      <source>Default font size for the menus and windows</source>
      <translation>Valikoissa ja ikkunoissa käytettävän fontin koko</translation>
    </message>
    <message>
      <source>Default font size for the tool windows</source>
      <translation>Oletuskoko työkaluikkunoissa käytettäville fonteille</translation>
    </message>
    <message>
      <source>Default documents directory</source>
      <translation>Oletushakemisto asiakirjoille</translation>
    </message>
    <message>
      <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
      <translation>Oletushakemisto ICC-profiileille. Tätä ei voi muuttaa asiakirjan ollessa avoinna. Oletuksena Scribus hakee profiileja järjestelmähakemistoista Windowsissa ja Mac OSX:ssä. Linuxissa ja Unixeissa profiileja etsitään hakemistoista $home/.color/icc,/usr/share/color/icc ja /usr/local/share/color/icc </translation>
    </message>
    <message>
      <source>Default Scripter scripts directory</source>
      <translation>Oletushakemisto skripteille</translation>
    </message>
    <message>
      <source>Additional directory for document templates</source>
      <translation>Hakemisto asiakirjamalleille</translation>
    </message>
    <message>
      <source>Choose a Directory</source>
      <translation>Valitse hakemisto</translation>
    </message>
    <message>
      <source>TabGeneralBase</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>User Interface</source>
      <translation type="unfinished" >Käyttöliittymä</translation>
    </message>
    <message>
      <source>&amp;Recent Documents:</source>
      <translation type="unfinished" >&amp;Viimeaikaiset asiakirjat:</translation>
    </message>
    <message>
      <source>&amp;Wheel Jump:</source>
      <translation type="unfinished" >&amp;Rullan askellus:</translation>
    </message>
    <message>
      <source>Show Splashscreen on Startup</source>
      <translation type="unfinished" >Näytä latausikkuna, kun Scribus käynnistyy</translation>
    </message>
    <message>
      <source>Show Startup Dialog</source>
      <translation type="unfinished" >Näytä latausikkuna</translation>
    </message>
    <message>
      <source> pt</source>
      <translation type="unfinished" > pt</translation>
    </message>
    <message>
      <source>&amp;Font Size (Menus):</source>
      <translation type="unfinished" >&amp;Fonttikoko (valikot):</translation>
    </message>
    <message>
      <source>Font Size (&amp;Palettes):</source>
      <translation type="unfinished" >F&amp;onttikoko (työkaluikkunat):</translation>
    </message>
    <message>
      <source>Time before a Move or Resize starts:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source> ms</source>
      <translation type="unfinished" > ms</translation>
    </message>
    <message>
      <source>&amp;Theme:</source>
      <translation type="unfinished" >&amp;Teema:</translation>
    </message>
    <message>
      <source>&amp;Language:</source>
      <translation type="unfinished" >&amp;Kieli:</translation>
    </message>
    <message>
      <source>Paths</source>
      <translation type="unfinished" >Hakemistot</translation>
    </message>
    <message>
      <source>&amp;Change...</source>
      <translation type="unfinished" >&amp;Muuta...</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation type="unfinished" >Alt+C</translation>
    </message>
    <message>
      <source>C&amp;hange...</source>
      <translation type="unfinished" >&amp;Muuta...</translation>
    </message>
    <message>
      <source>Alt+H</source>
      <translation type="unfinished" >Alt+H</translation>
    </message>
    <message>
      <source>&amp;Scripts:</source>
      <translation type="unfinished" >&amp;Skriptit:</translation>
    </message>
    <message>
      <source>Cha&amp;nge...</source>
      <translation type="unfinished" >&amp;Muuta...</translation>
    </message>
    <message>
      <source>Alt+N</source>
      <translation type="unfinished" >Alt+N</translation>
    </message>
    <message>
      <source>&amp;ICC Profiles:</source>
      <translation type="unfinished" >&amp;ICC-profiilit:</translation>
    </message>
    <message>
      <source>&amp;Documents:</source>
      <translation type="unfinished" >&amp;Asiakirjat:</translation>
    </message>
    <message>
      <source>Document &amp;Templates:</source>
      <translation type="unfinished" >Malliasiakirja&amp;t:</translation>
    </message>
    <message>
      <source>Ch&amp;ange...</source>
      <translation type="unfinished" >&amp;Muuta...</translation>
    </message>
    <message>
      <source>Alt+A</source>
      <translation type="unfinished" >Alt+A</translation>
    </message>
    <message>
      <source>Palette windows will use smaller (space savy) widgets. Requires application restart</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Use Small Widgets in Palettes</source>
      <translation type="unfinished" />
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
    <message>
      <source>px</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Distance within which an object will snap to your placed guides. After setting this you will need to restart Scribus to set this setting.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Radius of the area where Scribus will allow you to grab an objects handles.After setting this you will need to restart Scribus to set this setting.</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>TabKeyboardShortcutsWidget</name>
    <message>
      <source>Alt</source>
      <translation>Alt</translation>
    </message>
    <message>
      <source>Ctrl</source>
      <translation>Ctrl</translation>
    </message>
    <message>
      <source>Shift</source>
      <translation>Shift</translation>
    </message>
    <message>
      <source>Meta</source>
      <translation>Meta</translation>
    </message>
    <message>
      <source>Meta+</source>
      <translation>Meta+</translation>
    </message>
    <message>
      <source>Shift+</source>
      <translation>Shift+</translation>
    </message>
    <message>
      <source>Alt+</source>
      <translation>Alt+</translation>
    </message>
    <message>
      <source>Ctrl+</source>
      <translation>Ctrl+</translation>
    </message>
    <message>
      <source>This key sequence is already in use</source>
      <translation>Näppäinyhdistelmä on jo käytössä</translation>
    </message>
    <message>
      <source>Keyboard Shortcuts</source>
      <translation type="unfinished" >Näppäinoikopolut</translation>
    </message>
    <message>
      <source>Action</source>
      <translation type="unfinished" >Toiminta</translation>
    </message>
    <message>
      <source>Shortcut</source>
      <translation type="unfinished" >Näppäinyhdistelmä</translation>
    </message>
    <message>
      <source>Search:</source>
      <translation type="unfinished" >Etsi:</translation>
    </message>
    <message>
      <source>Shortcut for Selected Action</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>CTRL+ALT+SHIFT+W</source>
      <translation type="unfinished" >CTRL+ALT+SHIFT+W</translation>
    </message>
    <message>
      <source>Set &amp;Key</source>
      <translation type="unfinished" >&amp;Aseta näppäin</translation>
    </message>
    <message>
      <source>Alt+K</source>
      <translation type="unfinished" >Alt+K</translation>
    </message>
    <message>
      <source>&amp;User Defined Key</source>
      <translation type="unfinished" >O&amp;ma näppäin</translation>
    </message>
    <message>
      <source>Alt+U</source>
      <translation type="unfinished" >Alt+U</translation>
    </message>
    <message>
      <source>&amp;No Key</source>
      <translation type="unfinished" >&amp;Ei näppäintä</translation>
    </message>
    <message>
      <source>Alt+N</source>
      <translation type="unfinished" >Alt+N</translation>
    </message>
    <message>
      <source>Loadable Shortcut Sets</source>
      <translation type="unfinished" >Ladattavat näppäinoikopolut</translation>
    </message>
    <message>
      <source>Reload the default Scribus shortcuts</source>
      <translation type="unfinished" >Lataa oletusnäppäinoikopolut</translation>
    </message>
    <message>
      <source>&amp;Reset</source>
      <translation type="unfinished" >&amp;Palauta</translation>
    </message>
    <message>
      <source>Alt+R</source>
      <translation type="unfinished" >Alt+R</translation>
    </message>
    <message>
      <source>Export the current shortcuts into an importable file</source>
      <translation type="unfinished" >Vie nykyiset näppäinoikopolut tiedostoon</translation>
    </message>
    <message>
      <source>&amp;Export...</source>
      <translation type="unfinished" >&amp;Vie...</translation>
    </message>
    <message>
      <source>Alt+E</source>
      <translation type="unfinished" >Alt+E</translation>
    </message>
    <message>
      <source>Import a shortcut set into the current configuration</source>
      <translation type="unfinished" >Tuo näppäinoikopolut nykyisiin asetuksiin</translation>
    </message>
    <message>
      <source>&amp;Import...</source>
      <translation type="unfinished" >&amp;Tuo...</translation>
    </message>
    <message>
      <source>Alt+I</source>
      <translation type="unfinished" >Alt+I</translation>
    </message>
    <message>
      <source>Load the selected shortcut set</source>
      <translation type="unfinished" >Lataa valittu näppäinoikopolku</translation>
    </message>
    <message>
      <source>&amp;Load</source>
      <translation type="unfinished" >&amp;Lataa</translation>
    </message>
    <message>
      <source>Alt+L</source>
      <translation type="unfinished" >Alt+L</translation>
    </message>
    <message>
      <source>Keyboard shortcut sets available to load</source>
      <translation type="unfinished" >Ladattavissa olevat näppäinoikopolut</translation>
    </message>
    <message>
      <source>Export Keyboard Shortcuts to File</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Enter the name of the shortcut set:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select a Key set file to read</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Key Set XML Files (*.xml)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select a Key set file to save to</source>
      <translation type="unfinished" />
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
    <name>TabMiscellaneous</name>
    <message>
      <source>Lorem Ipsum</source>
      <translation type="unfinished" >Lorem Ipsum</translation>
    </message>
    <message>
      <source>Count of the Paragraphs:</source>
      <translation type="unfinished" >Kappaleiden lukumäärä:</translation>
    </message>
    <message>
      <source>Always use standard Lorem Ipsum</source>
      <translation type="unfinished" >Käytä aina perus Lorem ipsumia</translation>
    </message>
    <message>
      <source>Preview of current Paragraph Style visible when editing Styles</source>
      <translation type="unfinished" >Kappaletyylin esikatselu näkyvissä muokattaessa kappaletyyliä</translation>
    </message>
    <message>
      <source>Always ask before fonts are replaced when loading a document</source>
      <translation type="unfinished" >Kysy aina ennen fonttien korvaamista asiakirjan latauksen yhteydessä</translation>
    </message>
    <message>
      <source>Default number of paragraphs for sample text insertion</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Always use the typical Latin-based Lorem Ipsum text for sample text</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Show a preview by default when editing styles</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Allow Scribus to automatically replace fonts when they are missing when opening a document</source>
      <translation type="unfinished" />
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
      <translation>EPS-grafiikan tarkkuus:</translation>
    </message>
    <message>
      <source>Com&amp;press Text and Vector Graphics</source>
      <translation>Pakkaa teksti ja &amp;vektorigrafiikka</translation>
    </message>
    <message>
      <source>Automatic</source>
      <translation>Automaattinen</translation>
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
      <source>Embedding</source>
      <translation>Upottaminen</translation>
    </message>
    <message>
      <source>Available Fonts:</source>
      <translation>Käytetyt fontit</translation>
    </message>
    <message>
      <source>Fonts to embed:</source>
      <translation>Upotettavat fontit</translation>
    </message>
    <message>
      <source>&amp;Fonts</source>
      <translation>&amp;Fontit</translation>
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
      <source>Profile:</source>
      <translation>Profiili:</translation>
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
      <source>Show page previews of each page listed above.</source>
      <translation>Näytä sivujen esikatselu sivuista jotka on listattu yllä.</translation>
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
      <translation>Etäisyys leikkausvaralle sivun todellisesta yläreunasta</translation>
    </message>
    <message>
      <source>Distance for bleed from the bottom of the physical page</source>
      <translation>Etäisyys leikkausvaralle sivun todellista alareunasta</translation>
    </message>
    <message>
      <source>Distance for bleed from the left of the physical page</source>
      <translation>Etäisyys leikkausvaralle sivun todellisesta vasemmasta reunasta</translation>
    </message>
    <message>
      <source>Distance for bleed from the right of the physical page</source>
      <translation>Etäisyys leikkausvaralle sivun todellisesta oikeasta reunasta</translation>
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
      <translation>Muunna lisävärit prosessiväreiksi</translation>
    </message>
    <message>
      <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
      <translation>Mahdollistaa lisävärien muuntamisen prosessiväreiksi. Jos et aio painaa käyttäen lisävärejä, tämä on paras jättää valituksi.</translation>
    </message>
    <message>
      <source>Compression &amp;Quality:</source>
      <translation>Pakkaus&amp;laatu:</translation>
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
      <translation>Upota fontit PDF-tiedostoon. Fonttien upotus säilyttää asiakirjan ulkoasun.</translation>
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
      <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
      <translation>Salli PDF:n turvallisuusominaisuudet. PDF 1.3 suojataan 40 bit salauksella, PDF 1.4 128 bit salauksella. Huomautus: PDF-salaus ei ole yhtä luotettava kuin GPG- tai PGP-salaus ja siinä on joitain rajoituksia.</translation>
    </message>
    <message>
      <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
      <translation>Valitse päätason salasana, jonka avulla voidaan poistaa tai asettaa kaikki PDF-tiedoston turvallisuusominaisuudet.</translation>
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
      <source>Display Settings</source>
      <translation>Näytön asetukset</translation>
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
      <source>No Script</source>
      <translation>Ei skriptiä</translation>
    </message>
    <message>
      <source>Viewer</source>
      <translation>Katseluohjelma
</translation>
    </message>
    <message>
      <source>Clip to Page Margins</source>
      <translation>Leikkaa sivun reunuksiin</translation>
    </message>
    <message>
      <source>Lossy - JPEG</source>
      <translation>Häviöllinen - JPEG</translation>
    </message>
    <message>
      <source>Lossless - Zip</source>
      <translation>Häviötön - Zip</translation>
    </message>
    <message>
      <source>Javascript to be executed
when PDF document is opened:</source>
      <translation>PDF-dokumentin avauksen 
yhteydessä suoritettava JavaScript:</translation>
    </message>
    <message>
      <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
      <translation>Asiakirjasi tasot viedään PDF-dokumenttiin (ainoastaan PDF-versiolle 1.5).</translation>
    </message>
    <message>
      <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
      <translation>Värimalli tallennettavalle PDF-tiedostolle. Näytölle/Verkkoon sopii näytöltä katsottaville PDF-tiedostoille sekä tulostettavaksi tavallisilla mustesuihkutulostimilla. Valitse Painoon, jos tulostat neliväritulostimelle (CMYK). Jos haluat harmaasävy-PDF:n valitse Harmaasävy.</translation>
    </message>
    <message>
      <source>Image Compression Method</source>
      <translation>Kuvien pakkausmenetelmä</translation>
    </message>
    <message>
      <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
      <translation>Mahdollistaa esitysefektien käytön PDF-lukijaohjelmissa, jotka tukevat niitä kokonäytön tilassa.</translation>
    </message>
    <message>
      <source>Do not show objects outside the margins in the exported file</source>
      <translation>Älä näytä kohteita reunusten ulkopuolella viedyssä tiedostossa.</translation>
    </message>
    <message>
      <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
      <translation>Sivun näytön aika, joka odotetaan ennen esityksen alkua. 0 ottaa automaattisen sivunvaihdoin pois käytöstä</translation>
    </message>
    <message>
      <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
      <translation>Käytettävä pakkausmenetelmä kuville. Automaattinen antaa Scribuksen tehdä valinnan parhaasta pakkauksesta. ZIP on häviötön ja hyvä tasavärisille kuville. JPEG on tehokkaampi ja auttaa tuottamaan pienempiä PDF-tiedostoja, jos asiakirja sisältää paljon kuvia. JPEG-pakkauksen kanssa saattaa kuvien laatu kärsiä. Automaattinen on varma valinta, ellet erityisesti halua käyttää jotain muuta vaihtoehtoa.</translation>
    </message>
    <message>
      <source>Printer Marks</source>
      <translation>Painomerkit</translation>
    </message>
    <message>
      <source>Crop Marks</source>
      <translation>Leikkausmerkit</translation>
    </message>
    <message>
      <source>Bleed Marks</source>
      <translation>Leikkausvaran merkit</translation>
    </message>
    <message>
      <source>Registration Marks</source>
      <translation>Kohdistusmerkit</translation>
    </message>
    <message>
      <source>Color Bars</source>
      <translation>Väriskaalat</translation>
    </message>
    <message>
      <source>Page Information</source>
      <translation>Sivun tiedot</translation>
    </message>
    <message>
      <source>Offset:</source>
      <translation>Siirtymä:</translation>
    </message>
    <message>
      <source>Bleed Settings</source>
      <translation>Leikkausvaran asetukset</translation>
    </message>
    <message>
      <source>Top:</source>
      <translation>Yläreuna:</translation>
    </message>
    <message>
      <source>Bottom:</source>
      <translation>Alareuna:</translation>
    </message>
    <message>
      <source>Left:</source>
      <translation>Vasen reuna:</translation>
    </message>
    <message>
      <source>Right:</source>
      <translation>Oikea reuna:</translation>
    </message>
    <message>
      <source>Use Document Bleeds</source>
      <translation>Käytä asiakirjan leikkausvaroja</translation>
    </message>
    <message>
      <source>Pre-Press</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Determines the PDF compatibility.&lt;br/>The default is &lt;b>PDF 1.3&lt;/b> which gives the widest compatibility.&lt;br/>Choose &lt;b>PDF 1.4&lt;/b> if your file uses features such as transparency or you require 128 bit encryption.&lt;br/>&lt;b>PDF 1.5&lt;/b> is necessary when you wish to preserve objects in separate layers within the PDF.&lt;br/>&lt;b>PDF/X-3&lt;/b> is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Inside:</source>
      <translation>Sisäreuna:</translation>
    </message>
    <message>
      <source>Outside:</source>
      <translation>Ulkoreuna:</translation>
    </message>
    <message>
      <source>&amp;Embed all</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Fonts to outline:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Outline all</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Convert all glyphs in the document to outlines.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Document Layout</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Maximum Image Resolution:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Show the document in single page mode</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Show the document in single page mode with the pages displayed continuously end to end like a scroll</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Show the document with facing pages, starting with the first page displayed on the left</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Show the document with facing pages, starting with the first page displayed on the right</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Use the viewer's defaults or the user's preferences if set differently from the viewer defaults</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Enables viewing the document in full screen</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Display the bookmarks upon opening</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Display the page thumbnails upon opening</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Forces the displaying of layers. Useful only for PDF 1.5+.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Hides the Tool Bar which has selection and other editing capabilities</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Hides the Menu Bar for the viewer, the PDF will display in a plain window. </source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Fit the document page or pages to the available space in the viewer window.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Limits the resolution of your bitmap images to the selected DPI. Images with a lower resolution will be left untouched. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>This creates bleed marks which are indicated by  _ . _ and show the bleed limit</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Add registration marks to each separation</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Add color calibration bars</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Add document information which includes the document title and page numbers</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Indicate the distance offset for the registration marks</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Use the existing bleed settings from the document preferences</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Use Color Profile</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Do not use embedded color profiles</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Apply Effect to all Pages</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Automatically rotate the exported pages</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Export PDFs in image frames as embedded PDFs. This does *not* yet take care of colorspaces, so you should know what you are doing before setting this to 'true'.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Push</source>
      <translation type="unfinished" >Paina</translation>
    </message>
    <message>
      <source>Cover</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Uncover</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Fade</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Embed PDF &amp;&amp; EPS files (EXPERIMENTAL)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Rendering Intent:</source>
      <translation type="unfinished" >Sovitustapa:</translation>
    </message>
  </context>
  <context>
    <name>TabPrinter</name>
    <message>
      <source>Distance for bleed from the top of the physical page</source>
      <translation type="unfinished" >Etäisyys leikkausvaralle sivun todellisesta yläreunasta</translation>
    </message>
    <message>
      <source>Distance for bleed from the bottom of the physical page</source>
      <translation type="unfinished" >Etäisyys leikkausvaralle sivun todellista alareunasta</translation>
    </message>
    <message>
      <source>Distance for bleed from the left of the physical page</source>
      <translation type="unfinished" >Etäisyys leikkausvaralle sivun todellisesta vasemmasta reunasta</translation>
    </message>
    <message>
      <source>Distance for bleed from the right of the physical page</source>
      <translation type="unfinished" >Etäisyys leikkausvaralle sivun todellisesta oikeasta reunasta</translation>
    </message>
    <message>
      <source>Do not show objects outside the margins on the printed page</source>
      <translation type="unfinished" >Älä näytä reunuksien ulkopuolella olevia kohteita tulostetussa sivussa</translation>
    </message>
    <message>
      <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
      <translation type="unfinished" >Käytä vaihtoehtoista tulostuksen hallintaa kuten kprinter tai gtklp</translation>
    </message>
    <message>
      <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
      <translation type="unfinished" >Asettaa PostScript-tason. 
Tasot 1 ja 2 saattavat luoda suuria tiedostoja</translation>
    </message>
    <message>
      <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
      <translation type="unfinished" >Tapa vaihtaa C, Y ja M värien tekemä harmaa mustalla tehdyksi. 
UCR vaikuttaa kuvien kohdissa, jotka ovat neutraaleja tai lähellä harmaata 
olevia tummia värejä. UCR:n käyttäminen saattaa parantaa joidenkin kuvien
tulostamista. UCR parantaa kykyä välttää värien liikakylläisyyttä CMY-musteilla. 
Parhaan tuloksen saavuttamiseksi tarvitaan tapauskohtaista testausta. UCR 
vähentää ylikylläisyyden vaaraa CMY-musteilla.</translation>
    </message>
    <message>
      <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
      <translation type="unfinished" >Mahdollistaa lisävärien muuntamisen prosessiväreiksi. Jos et aio painaa käyttäen lisävärejä, tämä on paras jättää valituksi.</translation>
    </message>
    <message>
      <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
      <translation type="unfinished" >Aseta PostScript-tiedoston median koko eksplisiittisesti. Käyttö on suositeltavaa ainoastaan jos paino pyysi sitä.</translation>
    </message>
    <message>
      <source>File</source>
      <translation type="unfinished" >Tiedosto</translation>
    </message>
    <message>
      <source>All</source>
      <translation type="unfinished" >Kaikki</translation>
    </message>
    <message>
      <source>TabPrinterBase</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Options</source>
      <translation type="unfinished" >Asetukset</translation>
    </message>
    <message>
      <source>Page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Mirror Page(s) Horizontal</source>
      <translation type="unfinished" >Peilaa sivut vaakasuunnassa</translation>
    </message>
    <message>
      <source>Mirror Page(s) Vertical</source>
      <translation type="unfinished" >Peilaa sivut pystysuunnassa</translation>
    </message>
    <message>
      <source>Set Media Size</source>
      <translation type="unfinished" >Aseta kohteen koko</translation>
    </message>
    <message>
      <source>Clip to Page Margins</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Print in Grayscale</source>
      <translation type="unfinished" >Harmaasävytulostus</translation>
    </message>
    <message>
      <source>Print in Color if Available</source>
      <translation type="unfinished" >Väritulostus, jos mahdollista</translation>
    </message>
    <message>
      <source>Level 1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Level 2</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Level 3</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>General</source>
      <translation type="unfinished" >Yleiset</translation>
    </message>
    <message>
      <source>Print Separations</source>
      <translation type="unfinished" >Tulosta erottelu</translation>
    </message>
    <message>
      <source>Print Normal</source>
      <translation type="unfinished" >Normaali tulostus</translation>
    </message>
    <message>
      <source>Color</source>
      <translation type="unfinished" >Väri</translation>
    </message>
    <message>
      <source>Apply Under Color Removal</source>
      <translation type="unfinished" >Käytä alivärinpoistoa (UCR)</translation>
    </message>
    <message>
      <source>Convert Spot Colors to Process Colors</source>
      <translation type="unfinished" >Muunna lisävärit prosessiväreiksi</translation>
    </message>
    <message>
      <source>Apply ICC Profiles</source>
      <translation type="unfinished" >Käytä ICC-profiileja</translation>
    </message>
    <message>
      <source>Marks &amp;&amp; Bleeds</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Bleed Settings</source>
      <translation type="unfinished" >Leikkausvaran asetukset</translation>
    </message>
    <message>
      <source>Top:</source>
      <translation type="unfinished" >Yläreuna:</translation>
    </message>
    <message>
      <source>Bottom:</source>
      <translation type="unfinished" >Alareuna:</translation>
    </message>
    <message>
      <source>Left:</source>
      <translation type="unfinished" >Vasen reuna:</translation>
    </message>
    <message>
      <source>Right:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Printer Marks</source>
      <translation type="unfinished" >Painomerkit</translation>
    </message>
    <message>
      <source>Add color calibration bars</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Color Bars</source>
      <translation type="unfinished" >Väriskaalat</translation>
    </message>
    <message>
      <source>Offset:</source>
      <translation type="unfinished" >Siirtymä:</translation>
    </message>
    <message>
      <source>Add registration marks which are added to each separation</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Registration Marks</source>
      <translation type="unfinished" >Kohdistusmerkit</translation>
    </message>
    <message>
      <source>This creates bleed marks which are indicated by  _ . _ and show the bleed limit</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Bleed Marks</source>
      <translation type="unfinished" >Leikkausvaran merkit</translation>
    </message>
    <message>
      <source>This creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Crop Marks</source>
      <translation type="unfinished" >Leikkausmerkit</translation>
    </message>
    <message>
      <source>Print Destination</source>
      <translation type="unfinished" >Tulosteen kohde</translation>
    </message>
    <message>
      <source>Alternative Printer Command</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Command:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Allows you to embed color profiles in the print stream when color management is enabled</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>PostScript Options</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Include PDF Annotations and Links</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>TabScrapbook</name>
    <message>
      <source>This enables the scrapbook to be used an extension to the copy/paste buffers. Simply copying an object or grouped object will send this to the Scrapbook automatically</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Send Copied Items Automatically to Scrapbook</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>This enables copied items to be kept permanently in the scrapbook.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Keep Copied Items Permanently Across Sessions</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The minimum number is 1; the maximum us 100.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Number of Copied Items to Keep in Scrapbook:</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>TabTools</name>
    <message>
      <source>Font:</source>
      <translation>Fontti:</translation>
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
      <translation>Fontti uuteen tekstikehykseen</translation>
    </message>
    <message>
      <source>Size of font for new text frames</source>
      <translation>Fontin koko uuteen tekstikehykseen</translation>
    </message>
    <message>
      <source>Color of font</source>
      <translation>Fontin väri</translation>
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
      <translation>Esimerkki valitusta fontista</translation>
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
    <message>
      <source>Text</source>
      <translation>Teksti</translation>
    </message>
    <message>
      <source>Shapes</source>
      <translation>Kuviot</translation>
    </message>
    <message>
      <source>Lines</source>
      <translation>Viivat</translation>
    </message>
    <message>
      <source>Images</source>
      <translation>Kuvat</translation>
    </message>
    <message>
      <source>Regular Polygons</source>
      <translation>Tavalliset monikulmiot</translation>
    </message>
    <message>
      <source>Zoom</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Rotation Tool</source>
      <translation>Kiertotyökalu</translation>
    </message>
    <message>
      <source>Constrain to:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Other Properties</source>
      <translation>Muut ominaisuudet</translation>
    </message>
    <message>
      <source>None</source>
      <comment>tab fill</comment>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Miscellaneous Settings</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Item Duplicate</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>X Displacement</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Y Displacement</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Horizontal displacement of page items</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Vertical displacement of page items</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Constrain value for the rotation tool when the Control key is pressed</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Degrees</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Use the embedded clipping paths in images when importing them. JPEG, PSD and TIFF are the image formats which can embedded clipping paths.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Scale Image to Frame Size</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Image Frame Properties</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Image frames allow images to scale to any size</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Images in image frames are scaled to the size of the frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Automatically scaled images keep their original proportions</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Fill color of image frames</source>
      <translation type="unfinished" />
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
      <translation>&amp;Koon muutos:</translation>
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
      <translation>K&amp;oon muutos:</translation>
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
      <translation>Koo&amp;n muutos:</translation>
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
      <translation>Siirtymä fontin peruslinjan yläpuolelle</translation>
    </message>
    <message>
      <source>Relative size of the superscript compared to the normal font</source>
      <translation>Yläindeksin fontin koko verrattuna normaaliin fonttiin</translation>
    </message>
    <message>
      <source>Displacement below the baseline of the normal font on a line</source>
      <translation>Siirtymä fontin peruslinjan alapuolelle</translation>
    </message>
    <message>
      <source>Relative size of the subscript compared to the normal font</source>
      <translation>Alaindeksin fontin koko verrattuna normaaliin fonttiin</translation>
    </message>
    <message>
      <source>Relative size of the small caps font compared to the normal font</source>
      <translation>Kapiteelin fontin koko verrattuna normaaliin fonttiin</translation>
    </message>
    <message>
      <source>Percentage increase over the font size for the line spacing</source>
      <translation>Rivivälin kasvu prosentteina fontin koosta</translation>
    </message>
    <message>
      <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
      <translation>Siirtymä fontin peruslinjan alapuolelle ilmaistuna prosentteina.</translation>
    </message>
    <message>
      <source>Line width expressed as a percentage of the font size</source>
      <translation>Riviväli ilmaistuna prosentteina fontin koosta</translation>
    </message>
    <message>
      <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
      <translation>Siirtymä fontin peruslinjan yläpuolelle ilmaistuna prosentteina.

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
      <source>Fill Char:</source>
      <translation>Täyttömerkki:</translation>
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
    <message>
      <source>None</source>
      <comment>tab fill</comment>
      <translation>Ei mitään</translation>
    </message>
    <message>
      <source>Indentation from the right for the whole paragraph</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Fill Character of Tab</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Type/Orientation of Tab</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Position of Tab</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Period</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>TransformDialog</name>
    <message>
      <source>Scaling</source>
      <translation type="unfinished" >Koon muutos</translation>
    </message>
    <message>
      <source>Translation</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Rotation</source>
      <translation type="unfinished" >Kierto</translation>
    </message>
    <message>
      <source>Skewing</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Scale</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Scale H = %1 % V = %2 %</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Translate</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Translate H = %1%2 V = %3%4</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Rotate</source>
      <translation type="unfinished" >Kierrä</translation>
    </message>
    <message>
      <source>Rotate Angle = %1%2</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Skew</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Skew H = %1%2 V = %3%4</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>TransformDialogBase</name>
    <message>
      <source>Transform</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Add</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Remove</source>
      <translation type="unfinished" >Poista</translation>
    </message>
    <message>
      <source>u</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>d</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Scaling</source>
      <translation type="unfinished" >Koon muutos</translation>
    </message>
    <message>
      <source>Horizontal</source>
      <translation type="unfinished" >Vaakasuora</translation>
    </message>
    <message>
      <source> %</source>
      <translation type="unfinished" > %</translation>
    </message>
    <message>
      <source>Vertical</source>
      <translation type="unfinished" >Pystysuora</translation>
    </message>
    <message>
      <source>Translation</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Rotate</source>
      <translation type="unfinished" >Kierrä</translation>
    </message>
    <message>
      <source>Angle</source>
      <translation type="unfinished" >Kulma</translation>
    </message>
    <message>
      <source>Skew</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Origin</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Copies</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>TransformEffectPlugin</name>
    <message>
      <source>Transform...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Transform Effect</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Apply multiple transformations at once</source>
      <translation type="unfinished" />
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
      <translation>Aseta fontti</translation>
    </message>
    <message>
      <source>Set font size</source>
      <translation>Aseta fontin koko</translation>
    </message>
    <message>
      <source>Set font width</source>
      <translation>Aseta fontin leveys</translation>
    </message>
    <message>
      <source>Set font height</source>
      <translation>Aseta fontin korkeus</translation>
    </message>
    <message>
      <source>Set font fill color</source>
      <translation>Aseta fontin täyttöväri</translation>
    </message>
    <message>
      <source>Set font stroke color</source>
      <translation>Aseta fontin reunan väri</translation>
    </message>
    <message>
      <source>Set font fill color shade</source>
      <translation>Aseta fontin täyttövärin sävy</translation>
    </message>
    <message>
      <source>Set font stroke color shade</source>
      <translation>Aseta fontin reunuksen värin sävy</translation>
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
      <translation>Aseta fonttiefekti</translation>
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
      <translation>Teksti polulla</translation>
    </message>
    <message>
      <source>Enable Item Printing</source>
      <translation>Tulosta kohde</translation>
    </message>
    <message>
      <source>Disable Item Printing</source>
      <translation>Älä tulosta kohdetta</translation>
    </message>
    <message>
      <source>Multiple duplicate</source>
      <translation>Useita kopioita</translation>
    </message>
    <message>
      <source>Change Image Offset</source>
      <translation>Muuta kuvan sijainti</translation>
    </message>
    <message>
      <source>Change Image Scale</source>
      <translation>Muuta kuvan kokoa</translation>
    </message>
    <message>
      <source>X1: %1, Y1: %2
X2: %4, Y2: %5</source>
      <translation>X1: %1, Y1: %2
X2: %4, Y2: %5</translation>
    </message>
    <message>
      <source>X: %1, Y: %2
X: %4, Y: %5</source>
      <translation>X: %1, Y: %2
X: %4, Y: %5</translation>
    </message>
    <message>
      <source>Reset control point</source>
      <translation>Palauta hallintapiste</translation>
    </message>
    <message>
      <source>Reset control points</source>
      <translation>Palauta hallintapisteet</translation>
    </message>
    <message>
      <source>No object frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Apply text style</source>
      <translation>Käytä tekstityyliä</translation>
    </message>
    <message>
      <source>&amp;Undo: %1</source>
      <comment>f.e. Undo: Move</comment>
      <translation>&amp;Kumoa: %1</translation>
    </message>
    <message>
      <source>&amp;Undo</source>
      <translation>&amp;Kumoa</translation>
    </message>
    <message>
      <source>&amp;Redo: %1</source>
      <comment>f.e. Redo: Move</comment>
      <translation>&amp;Tee uudelleen: %1</translation>
    </message>
    <message>
      <source>&amp;Redo</source>
      <translation>&amp;Tee uudelleen</translation>
    </message>
    <message>
      <source>Apply image effects</source>
      <translation>Käytä kuvaefektejä</translation>
    </message>
    <message>
      <source>Insert frame</source>
      <translation>Lisää kehys</translation>
    </message>
    <message>
      <source>Adjust frame to the image size</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Set start and end arrows</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Remove vertical auto guide</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Remove horizontal auto guide</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Text flows around image clipping path</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Remove all guides</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Remove page guides</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Copy</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Copy page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Convert to outlines</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Change formula</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Import AI drawing</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Import XFig drawing</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Render frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Import Barcode</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Duplicate layer %1</source>
      <translation type="unfinished" />
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
    <message>
      <source>Show the action history for the selected item only. This changes the effect of the undo/redo buttons to act on the object or document.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Undo the last action for either the current object or the document</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Redo the last action for either the current object or the document</source>
      <translation type="unfinished" />
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
    <name>UnicodeSearch</name>
    <message>
      <source>Unicode Search</source>
      <translation type="unfinished" >Unicode-etsintä</translation>
    </message>
    <message>
      <source>&amp;Search:</source>
      <translation type="unfinished" >Et&amp;si:</translation>
    </message>
  </context>
  <context>
    <name>UpgradeChecker</name>
    <message>
      <source>Attempting to get the Scribus version update file</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>(No data on your computer will be sent to an external location)</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Timed out when attempting to get update file.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>File not found on server</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Could not open version file: %1
Error:%2 at line: %3, row: %4</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>An error occurred while looking for updates for Scribus, please check your internet connection.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>No updates are available for your version of Scribus %1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>One or more updates for your version of Scribus (%1) are available:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Please visit www.scribus.net for details.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Finished</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Operation canceled</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>This list may contain development/unstable versions.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Error: %1</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>UrlLauncher</name>
    <message>
      <source>Locate your web browser</source>
      <translation type="unfinished" >Web-selaimen sijainti</translation>
    </message>
    <message>
      <source>External Web Browser Failed to Start</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Scribus was not able to start the external web browser application %1. Please check the setting in Preferences.
Would you like to start the system's default browser instead?</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>UsePrinterMarginsDialog</name>
    <message>
      <source>Minimum Margins for Page Size %1</source>
      <translation>Pienin reunus sivukoolle %1</translation>
    </message>
    <message>
      <source>Use Printer Margins</source>
      <translation type="unfinished" >Käytä tulostimen reunuksia</translation>
    </message>
    <message>
      <source>Select &amp;Printer:</source>
      <translation type="unfinished" >Valitse &amp;tulostin:</translation>
    </message>
    <message>
      <source>Margins</source>
      <translation type="unfinished" >Reunukseen</translation>
    </message>
    <message>
      <source>Right:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Top:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Bottom:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Left:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;OK</source>
      <translation type="unfinished" >&amp;OK</translation>
    </message>
    <message>
      <source>Alt+O</source>
      <translation type="unfinished" >Alt+O</translation>
    </message>
    <message>
      <source>&amp;Cancel</source>
      <translation type="unfinished" >&amp;Peruuta</translation>
    </message>
    <message>
      <source>Alt+C</source>
      <translation type="unfinished" >Alt+C</translation>
    </message>
  </context>
  <context>
    <name>WMFImport</name>
    <message>
      <source>Group%1</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>WMFImportPlugin</name>
    <message>
      <source>Import &amp;WMF...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Imports WMF Files</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Imports most WMF files into the current document,
converting their vector data into Scribus objects.</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>The file could not be imported</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>WMF file contains some unsupported features</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>XfigPlug</name>
    <message>
      <source>Importing: %1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Analyzing File:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Group%1</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Generating Items</source>
      <translation type="unfinished" >Luodaan kohteita</translation>
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
      <source>Open</source>
      <translation type="unfinished" >Avaa</translation>
    </message>
    <message>
      <source>&amp;Importer:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Import &amp;Text Only</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Encoding:</source>
      <translation type="unfinished" />
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
    <name>hysettingsBase</name>
    <message>
      <source>Form</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>General Options</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
      <translation type="unfinished" >Dialogi-ikkuna, joka näyttää kaikki mahdolliset tavutettavat sanat tavutusvaihtoehtoineen, 
ilmestyy, kun käytät Työkalut, Tavuta teksti.</translation>
    </message>
    <message>
      <source>&amp;Hyphenation Suggestions</source>
      <translation type="unfinished" >&amp;Tavutusehdotukset</translation>
    </message>
    <message>
      <source>Enables automatic hyphenation of your text while typing.</source>
      <translation type="unfinished" >Mahdollistaa automaattiset tavutuksen kirjoittaessasi.</translation>
    </message>
    <message>
      <source>Hyphenate Text Automatically &amp;During Typing</source>
      <translation type="unfinished" >Tavuta teksti automaattisesti &amp;kirjoitettaessa</translation>
    </message>
    <message>
      <source>Behaviour</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>&amp;Language:</source>
      <translation type="unfinished" >&amp;Kieli:</translation>
    </message>
    <message>
      <source>&amp;Smallest Word:</source>
      <translation type="unfinished" >&amp;Lyhin sana:</translation>
    </message>
    <message>
      <source>Length of the smallest word to be hyphenated.</source>
      <translation type="unfinished" >Lyhimmän tavutettavan sanan pituus.</translation>
    </message>
    <message>
      <source>Chars</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Consecutive Hyphenations &amp;Allowed:</source>
      <translation type="unfinished" >&amp;Sallitut perättäiset tavutukset:</translation>
    </message>
    <message>
      <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
      <translation type="unfinished" >Suurin määrä tavutuksia peräkkäin.
0 tarkoittaa rajoittamatonta määrää tavutuksia.</translation>
    </message>
    <message>
      <source>Exceptions</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Edit</source>
      <translation type="unfinished" >Muokkaa</translation>
    </message>
    <message>
      <source>Ignore List</source>
      <translation type="unfinished" />
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
      <translation>Varmista että kuvia voidaan käyttää vapaasti. Jos fontteja ei voi levittää, älä kerää niitä tallentaessasi asiakirjamallin.</translation>
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
    <message>
      <source>&amp;About</source>
      <translation type="unfinished" >T&amp;ietoja</translation>
    </message>
    <message>
      <source>&amp;Help</source>
      <translation type="unfinished" >&amp;Ohje</translation>
    </message>
    <message>
      <source>&amp;Preview</source>
      <translation type="unfinished" >&amp;Esikatselu</translation>
    </message>
  </context>
  <context>
    <name>replaceColorDialog</name>
    <message>
      <source>Replace Color</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Replace:</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>with:</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>replaceColorsDialog</name>
    <message>
      <source>Original</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Replacement</source>
      <translation type="unfinished" >Korvaava</translation>
    </message>
    <message>
      <source>Replace Colors</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Add ...</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Remove</source>
      <translation type="unfinished" >Poista</translation>
    </message>
    <message>
      <source>Edit...</source>
      <translation type="unfinished" >Muokkaa...</translation>
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
    <message>
      <source>&amp;More Details</source>
      <translation type="unfinished" />
    </message>
  </context>
  <context>
    <name>selectDialog</name>
    <message>
      <source>Text Frame</source>
      <translation type="unfinished" >Tekstikehys</translation>
    </message>
    <message>
      <source>Image Frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Shape</source>
      <translation type="unfinished" >Kuvio</translation>
    </message>
    <message>
      <source>Polyline</source>
      <translation type="unfinished" >Kulmikas viiva</translation>
    </message>
    <message>
      <source>Line</source>
      <translation type="unfinished" >Viiva</translation>
    </message>
    <message>
      <source>Render Frame</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Fill Color</source>
      <translation type="unfinished" >Täyttöväri</translation>
    </message>
    <message>
      <source>Line Color</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Line Width</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Printable</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Yes</source>
      <translation type="unfinished" >Kyllä</translation>
    </message>
    <message>
      <source>No</source>
      <translation type="unfinished" >Ei</translation>
    </message>
    <message>
      <source>Locked</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Resizeable</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select All Objects</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select all items on the current page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>On Current Page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select all items on the current layer on all pages</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>On Current Layer</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select all items not on a page</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>On the Scratch Space</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Narrow the selection of items based on various item properties</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>With the Following Attributes</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select based on item type</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Item Type</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select based on the color that the item is filled with</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select based on the color of the line or outline</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select based on the width of the line of the item</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select items based on whether they will be printed or not</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select items based on their locked status</source>
      <translation type="unfinished" />
    </message>
    <message>
      <source>Select items based on whether they have their size locked or not</source>
      <translation type="unfinished" />
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
