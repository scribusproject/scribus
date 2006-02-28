<!DOCTYPE TS><TS>
<context>
    <name></name>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation>getColorNames() -&gt; list 

Returnerer en liste med navn på alle definerte farger i dokumentet. Hvis ikke et dokument er åpnet, så vil en liste med standard dokumentfarger bli returnert. </translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation>newDocDialog() -&gt; bool 

Viser dialogboksen &quot;New Document&quot;. Lager et nytt dokument hvis brukeren aksepterer 
innstillingene. Lager ikke et nytt dokument hvis brukeren trykker Avbryt. Returnerer 
true hvis et nytt dokument lages.</translation>
    </message>
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillColor([&quot;name&quot;]) -&gt; string 

Returnerer navnet på fyllfargen til objektet med navn &quot;name&quot;. 
Hvis &quot;name&quot;.ikke er oppgitt, så vil det objektet som er valgt bli brukt.</translation>
    </message>
    <message>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation>moveObject(dx, dy [, &quot;name&quot;]) 

Flytter objektet &quot;name&quot; en avstand dy og dx relativt til dets nåværende posisjon. 
Enhetene som brukes er den enhet som er valgt for dokumentet. Hvis &quot;name&quot; ikke er 
oppgitt, så vil det objektet som er valgt bli brukt. Hvis objektet &quot;name&quot; tilhører en 
gruppe, så vil hele gruppen bli flyttet.</translation>
    </message>
    <message>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>createRect(x, y, width, height, [&quot;name&quot;]) -&gt; string

Creates a new rectangle on the current page and returns its name. The
coordinates are given in the current measurement units of the document
(see UNIT constants). &quot;name&quot; should be a unique identifier for the object
because you need this name to reference that object in future. If &quot;name&quot;
is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getFontSize([&quot;name&quot;]) -&gt; float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>newDocument(size, margins, orientation, firstPageNumber,
                        unit, pagesType, firstPageOrder) -&gt; bool

Creates a new document and returns true if successful. The parameters have the
following meaning:

size = A tuple (width, height) describing the size of the document. You can
use predefined constants named PAPER_&lt;paper_type&gt; e.g. PAPER_A4 etc.

margins = A tuple (left, right, top, bottom) describing the document
margins

orientation = the page orientation - constants PORTRAIT, LANDSCAPE

firstPageNumer = is the number of the first page in the document used for
pagenumbering. While you&apos;ll usually want 1, it&apos;s useful to have higher
numbers if you&apos;re creating a document in several parts.

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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>@default</name>
    <message>
        <source>getColor(&quot;name&quot;) -&gt; tuple

Returns a tuple (C, M, Y, K) containing the four color components of the
color &quot;name&quot; from the current document. If no document is open, returns
the value of the named color from the default document colors.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>changeColor(&quot;name&quot;, c, m, y, k)

Changes the color &quot;name&quot; to the specified CMYK value. The color value is
defined via four components c = Cyan, m = Magenta, y = Yellow and k = Black.
Color components should be in the range from 0 to 255.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>deleteColor(&quot;name&quot;, &quot;replace&quot;)

Deletes the color &quot;name&quot;. Every occurence of that color is replaced by the
color &quot;replace&quot;. If not specified, &quot;replace&quot; defaults to the color
&quot;None&quot; - transparent.

deleteColor works on the default document colors if there is no document open.
In that case, &quot;replace&quot;, if specified, has no effect.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>messageBox(&quot;caption&quot;, &quot;message&quot;,
    icon=ICON_NONE, button1=BUTTON_OK|BUTTONOPT_DEFAULT,
    button2=BUTTON_NONE, button3=BUTTON_NONE) -&gt; integer

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
result = messageBox(&apos;Script failed&apos;,
                    &apos;This script only works when you have a text frame selected.&apos;,
                    ICON_ERROR)
result = messageBox(&apos;Monkeys!&apos;, &apos;Something went ook! &lt;i&gt;Was it a monkey?&lt;/i&gt;&apos;,
                    ICON_WARNING, BUTTON_YES|BUTTONOPT_DEFAULT,
                    BUTTON_NO, BUTTON_IGNORE|BUTTONOPT_ESCAPE)

Defined button and icon constants:
BUTTON_NONE, BUTTON_ABORT, BUTTON_CANCEL, BUTTON_IGNORE, BUTTON_NO,
BUTTON_NOALL, BUTTON_OK, BUTTON_RETRY, BUTTON_YES, BUTTON_YESALL,
ICON_NONE, ICON_INFORMATION, ICON_WARNING, ICON_CRITICAL.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>valueDialog(caption, message [,defaultvalue]) -&gt; string

Shows the common &apos;Ask for string&apos; dialog and returns its value as a string
Parameters: window title, text in the window and optional &apos;default&apos; value.

Example: valueDialog(&apos;title&apos;, &apos;text in the window&apos;, &apos;optional&apos;)
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>saveDoc()

Saves the current document with its current name, returns true if successful.
If the document has not already been saved, this may bring up an interactive
save file dialog.

If the save fails, there is currently no way to tell.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type&gt;.

May raise ValueError if an invalid unit is passed.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getUnit() -&gt; integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setDocType(facingPages, firstPageLeft)

Sets the document type. To get facing pages set the first parameter to
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for
a right page use FIRSTPAGERIGHT.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getSize([&quot;name&quot;]) -&gt; (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type&gt;
for reference.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getRotation([&quot;name&quot;]) -&gt; integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>moveObjectAbs(x, y [, &quot;name&quot;])

Moves the object &quot;name&quot; to a new location. The coordinates are expressed in
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot;
is not given the currently selected item is used.  If the object &quot;name&quot;
belongs to a group, the whole group is moved.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>rotateObject(rot [, &quot;name&quot;])

Rotates the object &quot;name&quot; by &quot;rot&quot; degrees relatively. The object is
rotated by the vertex that is currently selected as the rotation point - by
default, the top left vertext at zero rotation. Positive values mean counter
clockwise rotation when the default rotation point is used. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>selectionCount() -&gt; integer

Returns the number of selected objects.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>deselectAll()

Deselects all objects in the whole document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is
not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>scaleImage(x, y [, &quot;name&quot;])

Sets the scaling factors of the picture in the image frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. A number of 1
means 100 %.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>lockObject([&quot;name&quot;]) -&gt; bool

Locks the object &quot;name&quot; if it&apos;s unlocked or unlock it if it&apos;s locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>isLocked([&quot;name&quot;]) -&gt; bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getXFontNames() -&gt; list of tuples

Returns a larger font info. It&apos;s a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getLayers() -&gt; list

Returns a list with the names of all defined layers.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getActiveLayer() -&gt; string

Returns the name of the current active layer.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn&apos;t
exists or if it&apos;s the only layer in the document.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>createEllipse(x, y, width, height, [&quot;name&quot;]) -&gt; string

Creates a new ellipse on the current page and returns its name.
The coordinates are given in the current measurement units of the document
(see UNIT constants). &quot;name&quot; should be a unique identifier for the object
because you need this name for further referencing of that object. If &quot;name&quot;
is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>createImage(x, y, width, height, [&quot;name&quot;]) -&gt; string

Creates a new picture frame on the current page and returns its name. The
coordinates are given in the current measurement units of the document.
&quot;name&quot; should be a unique identifier for the object because you need this
name for further access to that object. If &quot;name&quot; is not given Scribus will
create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>createText(x, y, width, height, [&quot;name&quot;]) -&gt; string

Creates a new text frame on the actual page and returns its name.
The coordinates are given in the actual measurement unit of the document (see
UNIT constants). &quot;name&quot; should be a unique identifier for the object because
you need this name for further referencing of that object. If &quot;name&quot; is not
given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>createLine(x1, y1, x2, y2, [&quot;name&quot;]) -&gt; string

Creates a new line from the point(x1, y1) to the point(x2, y2) and returns
its name. The coordinates are given in the current measurement unit of the
document (see UNIT constants). &quot;name&quot; should be a unique identifier for the
object because you need this name for further access to that object. If
&quot;name&quot; is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>createPolyLine(list, [&quot;name&quot;]) -&gt; string

Creates a new polyline and returns its name. The points for the polyline are
stored in the list &quot;list&quot; in the following order: [x1, y1, x2, y2...xn. yn].
The coordinates are given in the current measurement units of the document (see
UNIT constants). &quot;name&quot; should be a unique identifier for the object because
you need this name for further access to that object. If &quot;name&quot; is not given
Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
May raise ValueError if an insufficient number of points is passed or if
the number of values passed don&apos;t group into points without leftovers.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>createPolygon(list, [&quot;name&quot;]) -&gt; string

Creates a new polygon and returns its name. The points for the polygon are
stored in the list &quot;list&quot; in the following order: [x1, y1, x2, y2...xn. yn].
At least three points are required. There is no need to repeat the first point
to close the polygon. The polygon is automatically closed by connecting the
first and the last point.  The coordinates are given in the current measurement
units of the document (see UNIT constants).  &quot;name&quot; should be a unique
identifier for the object because you need this name for further access to that
object. If &quot;name&quot; is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
May raise ValueError if an insufficient number of points is passed or if
the number of values passed don&apos;t group into points without leftovers.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>createBezierLine(list, [&quot;name&quot;]) -&gt; string

Creates a new bezier curve and returns its name. The points for the bezier
curve are stored in the list &quot;list&quot; in the following order:
[x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn. yn, kxn. kyn]
In the points list, x and y mean the x and y coordinates of the point and kx
and ky meaning the control point for the curve.  The coordinates are given in
the current measurement units of the document (see UNIT constants). &quot;name&quot;
should be a unique identifier for the object because you need this name for
further access to that object. If &quot;name&quot; is not given Scribus will create one
for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
May raise ValueError if an insufficient number of points is passed or if
the number of values passed don&apos;t group into points without leftovers.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>createPathText(x, y, &quot;textbox&quot;, &quot;beziercurve&quot;, [&quot;name&quot;]) -&gt; string

Creates a new pathText by merging the two objects &quot;textbox&quot; and
&quot;beziercurve&quot; and returns its name. The coordinates are given in the current
measurement unit of the document (see UNIT constants). &quot;name&quot; should be a
unique identifier for the object because you need this name for further access
to that object. If &quot;name&quot; is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
May raise NotFoundError if one or both of the named base object don&apos;t exist.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>textFlowsAroundFrame(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot;
is not passed, text flow is toggled.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>objectExists([&quot;name&quot;]) -&gt; bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it&apos;s applied on the selected object.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>redrawAll()

Redraws all pages.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>deletePage(nr)

Deletes the given page. Does nothing if the document contains only one page.
Page numbers are counted from 1 upwards, no matter what the displayed first
page number is.

May raise IndexError if the page number is out of range
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn&apos;t (curently) change the page the user&apos;s view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>pageCount() -&gt; integer

Returns the number of pages in the document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type&gt; constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getVGuides()

See getHGuides.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setVGuides()

See setHGuides.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getPageItems() -&gt; list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)]
means that object named &apos;Text1&apos; is a text frame (type 4) and is the first at
the page...
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLineWidth(width, [&quot;name&quot;])

Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not
given the currently selected item is used.

May raise ValueError if the line width is out of bounds.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLineShade(shade, [&quot;name&quot;])

Sets the shading of the line color of the object &quot;name&quot; to &quot;shade&quot;.
&quot;shade&quot; must be an integer value in the range from 0 (lightest) to 100
(full color intensity). If &quot;name&quot; is not given the currently selected item
is used.

May raise ValueError if the line shade is out of bounds.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type&gt;.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type&gt;.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style&gt;.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setFillShade(shade, [&quot;name&quot;])

Sets the shading of the fill color of the object &quot;name&quot; to &quot;shade&quot;.
&quot;shade&quot; must be an integer value in the range from 0 (lightest) to 100
(full Color intensity). If &quot;name&quot; is not given the currently selected
Item is used.

May raise ValueError if the fill shade is out of bounds.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn&apos;t exist.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getFont([&quot;name&quot;]) -&gt; string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getTextLength([&quot;name&quot;]) -&gt; integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getAllText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot; and of all text frames which are
linked with this frame. If this textframe has some text selected, the selected
text is returned. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getLineSpacing([&quot;name&quot;]) -&gt; float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getColumns([&quot;name&quot;]) -&gt; integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, &apos;iso-8859-2&apos;). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setFont(&quot;font&quot;, [&quot;name&quot;])

Sets the font of the text frame &quot;name&quot; to &quot;font&quot;. If there is some text
selected only the selected text is changed.  If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError if the font cannot be found.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setFontSize(size, [&quot;name&quot;])

Sets the font size of the text frame &quot;name&quot; to &quot;size&quot;. &quot;size&quot; is treated
as a value in points. If there is some text selected only the selected text is
changed. &quot;size&quot; must be in the range 1 to 512. If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError for a font size that&apos;s out of bounds.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setColumnGap(size, [&quot;name&quot;])

Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If
&quot;name&quot; is not given the currently selected item is used.

May throw ValueError if the column gap is out of bounds (must be positive).
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setColumns(nr, [&quot;name&quot;])

Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May throw ValueError if number of columns is not at least one.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setTextAlignment(align, [&quot;name&quot;])

Sets the text alignment of the text frame &quot;name&quot; to the specified alignment.
If &quot;name&quot; is not given the currently selected item is used. &quot;align&quot; should
be one of the ALIGN_ constants defined in this module - see dir(scribus).

May throw ValueError for an invalid alignment constant.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>deleteText([&quot;name&quot;])

Deletes any text in the text frame &quot;name&quot;. If there is some text selected,
only the selected text will be deleted. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there
is some text selected only the selected text is changed. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setTextShade(shade, [&quot;name&quot;])

Sets the shading of the text color of the object &quot;name&quot; to &quot;shade&quot;. If
there is some text selected only the selected text is changed. &quot;shade&quot; must
be an integer value in the range from 0 (lightest) to 100 (full color
intensity). If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Link two text frames. The frame named &quot;fromname&quot; is linked to the
frame named &quot;toname&quot;. The target frame must be an empty text frame
and must not link to or be linked from any other frames already.

May throw ScribusException if linking rules are violated.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>unlinkTextFrames(&quot;name&quot;)

Remove the specified (named) object from the text frame flow/linkage. If the
frame was in the middle of a chain, the previous and next frames will be
connected, eg &apos;a-&gt;b-&gt;c&apos; becomes &apos;a-&gt;c&apos; when you unlinkTextFrames(b)&apos;

May throw ScribusException if linking rules are violated.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>progressTotal(max)

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>progressSet(nr)

Set the progress bar position to &quot;nr&quot;, a value relative to the previously set
progressTotal. The progress bar uses the concept of steps; you give it the
total number of steps and the number of steps completed so far and it will
display the percentage of steps that have been completed. You can specify the
total number of steps with progressTotal(). The current number of steps is set
with progressSet(). The progress bar can be rewound to the beginning with
progressReset(). [based on info taken from Trolltech&apos;s Qt docs]
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection&gt;)

Sets the scale to frame on the selected or specified image frame to `scaletoframe&apos;.
If `proportional&apos; is specified, set fixed aspect ratio scaling to `proportional&apos;.
Both `scaletoframe&apos; and `proportional&apos; are boolean.

May raise WrongFrameTypeError.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getColorAsRGB(&quot;name&quot;) -&gt; tuple

Returns a tuple (R,G,B) containing the three color components of the
color &quot;name&quot; from the current document, converted to the RGB color
space. If no document is open, returns the value of the named color
from the default document colors.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yello and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>fileDialog(&quot;caption&quot;, [&quot;filter&quot;, &quot;defaultname&quot;, haspreview, issave, isdir]) -&gt; string with filename

Shows a File Open dialog box with the caption &quot;caption&quot;. Files are filtered
with the filter string &quot;filter&quot;. A default filename or file path can also
supplied, leave this string empty when you don&apos;t want to use it.  A value of
True for haspreview enables a small preview widget in the FileSelect box.  When
the issave parameter is set to True the dialog acts like a &quot;Save As&quot; dialog
otherwise it acts like a &quot;File Open Dialog&quot;. When the isdir parameter is True
the dialog shows and returns only directories. The default for all of the
opional parameters is False.

The filter, if specified, takes the form &apos;comment (*.type *.type2 ...)&apos;.
For example &apos;Images (*.png *.xpm *.jpg)&apos;.

Refer to the Qt-Documentation for QFileDialog for details on filters.

Example: fileDialog(&apos;Open input&apos;, &apos;CSV files (*.csv)&apos;)
Example: fileDialog(&apos;Save report&apos;, defaultname=&apos;report.txt&apos;, issave=True)
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius isexpressed in points. If &quot;name&quot; is not given the currentlyselected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property&apos; of `object&apos;. See getProperty()
for details of arguments.

If `includesuper&apos; is true, search inherited properties too.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object&apos;.
If `includesuper&apos; is true, return properties supported
by parent classes as well.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getProperty(object, property)

Return the value of the property `property&apos; of the passed `object&apos;.

The `object&apos; argument may be a string, in which case the named PageItem
is searched for. It may also be a PyCObject, which may point to any
C++ QObject instance.

The `property&apos; argument must be a string, and is the name of the property
to look up on `object&apos;.

The return value varies depending on the type of the property.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setProperty(object, property, value)

Set `property&apos; of `object&apos; to `value&apos;. If `value&apos; cannot be converted to a type
compatible with the type of `property&apos;, an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object&apos;, possibly restricted to children
of class named `ofclass&apos; or children named `ofname&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getChild(object, childname, ofclass=None, recursive=True)

Return the first child of `object&apos; named `childname&apos;, possibly restricting
the search to children of type name `ofclass&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positve values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>renderFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size, format=&quot;PPM&quot;) -&gt; bool

Creates an image preview of font &quot;name&quot; with given text &quot;sample&quot; and size.
If &quot;filename&quot; is not &quot;&quot;, image is saved into &quot;filename&quot;. Otherwise
image data is returned as a string. The optional &quot;format&quot; argument
specifies the image format to generate, and supports any format allowed
by QPixmap.save(). Common formats are PPM, JPEG, PNG and XPM.

May raise NotFoundError if the specified font can&apos;t be found.
May raise ValueError if an empty sample or filename is passed.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is printable or not, a value of True means
that the layer &quot;layer&quot; can be printed, a value of False means that printing
the layer &quot;layer&quot; is disabled.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference) The first character has an
index of 0. Inserting at position -1 appends text to the frame. If &quot;name&quot; is
not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>selectText(start, count, [&quot;name&quot;])

Selects &quot;count&quot; characters of text in the text frame &quot;name&quot; starting from the
character &quot;start&quot;. Character counting starts at 0. If &quot;count&quot; is zero, any
text selection will be cleared.  If &quot;name&quot; is not given the currently
selected item is used.

May throw IndexError if the selection is outside the bounds of the text.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>textOverflows([&quot;name&quot;, nolinks]) -&gt; integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn&apos;t
use text frame linking. Without this parameter it search all linking chain.

May raise WrongFrameTypeError if the target frame is not an text frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets wether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>isPDFBookmark([&quot;name&quot;]) -&gt; bool

Returns true if the text frame &quot;name&quot; is a PDF bookmark.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>newStyleDialog() -&gt; string

Shows &apos;Create new paragraph style&apos; dialog. Function returns real
style name or None when user cancels the dialog.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>newDoc(size, margins, orientation, firstPageNumber,
                   unit, facingPages, firstSideLeft) -&gt; bool

WARNING: Obsolete procedure! Use newDocument instead.

Creates a new document and returns true if successful. The parameters have the
following meaning:

    size = A tuple (width, height) describing the size of the document. You can
    use predefined constants named PAPER_&lt;paper_type&gt; e.g. PAPER_A4 etc.

    margins = A tuple (left, right, top, bottom) describing the document
    margins

    orientation = the page orientation - constants PORTRAIT, LANDSCAPE

    firstPageNumer = is the number of the first page in the document used for
    pagenumbering. While you&apos;ll usually want 1, it&apos;s useful to have higher
    numbers if you&apos;re creating a document in several parts.

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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>masterPageNames()

Returns a list of the names of all master pages in the document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>createMasterPage(pageName)

Creates a new master page named pageName and opens it for
editing.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>deleteMasterPage(pageName)

Delete the named master page.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>About</name>
    <message>
        <source>%1. %2 %3 </source>
        <translation type="obsolete">%1.%2 %3</translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation type="obsolete">Scribus versjon %1
%2 %3</translation>
    </message>
    <message>
        <source>Build-ID:</source>
        <translation type="obsolete">Build-ID:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Bidrag fra:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation type="obsolete">Windows versjon:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation type="obsolete">Tysk:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation type="obsolete">Fransk:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation type="obsolete">Italiensk:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation type="obsolete">Ungarsk:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation type="obsolete">Ukrainsk:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation type="obsolete">Bulgarsk:</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation type="obsolete">Galisk:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation type="obsolete">Tyrkisk:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation type="obsolete">Litauisk:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation type="obsolete">Polsk:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation type="obsolete">Tsjekkisk:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation type="obsolete">Slovakisk:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation type="obsolete">Dansk:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation type="obsolete">Norsk:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation type="obsolete">Welsk:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation type="obsolete">Russisk:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation type="obsolete">Brasiliansk:</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation type="obsolete">Finsk:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation type="obsolete">Slovensk:</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation type="obsolete">Baskisk:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;Om</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>&amp;Forfattere</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;Oversettelser</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;Nett</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Utviklingslag:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Offisiell dokumentasjon:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Annen dokumentasjon:</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Offisielle oversettelser og oversettere:</translation>
    </message>
    <message>
        <source>English (British):</source>
        <translation type="obsolete">Engelsk (Britisk):</translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation type="obsolete">Esperanto:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation type="obsolete">Koreansk:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation type="obsolete">Serbisk:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation type="obsolete">Spansk:</translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation type="obsolete">Svensk:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Tidligere oversettere:</translation>
    </message>
    <message>
        <source>Catalan:</source>
        <translation type="obsolete">Katalansk:</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Hjemmeside</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Online referanse</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Feil og ønsker</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>E-postliste</translation>
    </message>
    <message>
        <source>About Scribus %1</source>
        <translation>Om Scribus %1</translation>
    </message>
    <message>
        <source>%1 %2 %3 </source>
        <translation type="obsolete">%1 %2 %3 </translation>
    </message>
    <message>
        <source>Using GhostScript version %1</source>
        <translation type="obsolete">Bruker GhostScript versjon %1</translation>
    </message>
    <message>
        <source>No GS version available</source>
        <translation type="obsolete">Ingen GS versjon tilgjengelig</translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3 (%4)</source>
        <translation type="obsolete">Scribus versjon %1
%2 %3 (%4)</translation>
    </message>
    <message>
        <source>Mac OSX Aqua Port:</source>
        <translation type="obsolete">Mac OSX Aqua Port:</translation>
    </message>
    <message>
        <source>Windows Port:</source>
        <translation type="obsolete">Windows Port:</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Missing library support is indicated by a *</source>
        <translation type="obsolete">Dette viser hvilken Scribus versjon du bruker, når den ble bygd og hvilke biblioteker den inkludereeer. C-C-T-F står for C=littlecms C=CUPS T=TIFF F=Fontconfig  støtte. Manglende støtte for noe av dette er indikert med en *</translation>
    </message>
    <message>
        <source>November</source>
        <translation type="obsolete">November</translation>
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
        <translation>Bruker Ghostscript  versjon %1</translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation>Ingen Ghostscript  versjon tilgjengelig</translation>
    </message>
    <message>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation>&lt;b&gt;Scribus Versjon %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Build ID:</source>
        <translation>Bygg ID:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation>Dette panelet viser hvilken versjon, byggdato og hvilke biblioteker som er inkludert i Scribus. Koden  C-C-T-F henviser  C=littlecms C=CUPS T=TIFF støtte F=Fontconfig. Den siste bokstaven viser til C=cairo eller A=libart. Manglene bibliotek er merket med en *. Dette panelet viser også hvilken Ghostscript  versjon som Scribus har funnet på systemet og bruker.</translation>
    </message>
    <message>
        <source>January</source>
        <translation type="obsolete">Januar</translation>
    </message>
    <message>
        <source>February</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mac OS&amp;#174; X Aqua Port:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Windows&amp;#174; Port:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nei</translation>
    </message>
    <message>
        <source>Filename:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Version:</source>
        <translation type="unfinished">Versjon:</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation type="unfinished">Skrudd på:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation type="unfinished">Utgivelses dato:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation type="unfinished">Beskrivelse:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation type="unfinished">Forfatter(e):</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation type="unfinished">Copyright:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation type="unfinished">Lisens:</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus: om Plug-ins</translation>
    </message>
    <message>
        <source>File Name:</source>
        <translation type="obsolete">Filnavn:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation type="obsolete">Versjon:</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation type="obsolete">Skrudd på:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation type="obsolete">Utgivelses dato:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation type="obsolete">Copyright:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation type="obsolete">Forfatter(e):</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation type="obsolete">Beskrivelse:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation type="obsolete">Lisens:</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
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
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Åpne ...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Lagre</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Lagre &amp;som ...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>Gå &amp;tilbake til lagret versjon</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Samle for U&amp;tskrift ...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Hent tekst ...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Legg til &amp;tekst ...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation>Sett inn bilde ...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Lagre &amp;tekst ...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Lagre som &amp;EPS ...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Lagre som P&amp;DF ...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>Dokument inn&amp;stillinger ...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>Skriv &amp;ut ...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>A&amp;vslutt</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Angre</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Kli&amp;pp</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Lim inn</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>F&amp;jern innhold</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Velg &amp;alt</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>&amp;Fravelg alt</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Søk/Erstatt ...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>Rediger bilde ...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>Far&amp;ger ...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>A&amp;vsnitts stiler ...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>&amp;Linjestiler ...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;Javaskripts ...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>&amp;Innstillinger ...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Andre ...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Venstre</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Midten</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>Høy&amp;re</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Blokk</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Tvunget</translation>
    </message>
    <message>
        <source>&amp;%1 %</source>
        <translation>&amp;%1 %</translation>
    </message>
    <message>
        <source>&amp;Normal</source>
        <translation>&amp;Normal</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Understrek</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation>Understek &amp;ord</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>&amp;Gjennomstrek</translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>Hevet &amp;skrift</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>Senket s&amp;krift</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation>&amp;Oversikt</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation>S&amp;kygge</translation>
    </message>
    <message>
        <source>&amp;Image Effects</source>
        <translation>&amp;Bildeeffekter</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabulatorer ...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>K&amp;opier</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>&amp;Flere kopier</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Grupèr</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>&amp;Oppløs gruppen</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>Lås/&amp;lås opp for redigering</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>Størr&amp;elsen er fastlåst</translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation>Legg &amp;underst</translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation>Legg &amp;øverst</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Senk</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;Hev</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Send til k&amp;laddebok</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>A&amp;ttributter ...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>V&amp;is bilde</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation>&amp;Oppdater bilde</translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation>Juster ramme til bilde</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation>&amp;Lav oppløsning</translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation>&amp;Normal oppløsning</translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation>&amp;Full oppløsning</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Er et PDF &amp;bokmerke</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Er PDF komme&amp;ntar</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>Kommenta&amp;r egenskaper</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>Felt egenskape&amp;r</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Rediger form ...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>&amp;Legg tekst på sti</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>&amp;Fjern tekst fra sti</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>&amp;Kombiner polygoner</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Splitt &amp;polygoner</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>&amp;Bezierkurve</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>&amp;Bilderamme</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation>&amp;Oversikt</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>&amp;Polygon</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Tekstramme</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>Eksempeltekst</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>Sett &amp;inn ...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation>Im&amp;porter ...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Slett ...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>&amp;Kopier ...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Flytt ...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>Håndter &amp;hjelpelinjer ...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation>&amp;Tilpass vinduet</translation>
    </message>
    <message>
        <source>&amp;50%</source>
        <translation>&amp;50%</translation>
    </message>
    <message>
        <source>&amp;75%</source>
        <translation>&amp;75%</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation>&amp;100%</translation>
    </message>
    <message>
        <source>&amp;200%</source>
        <translation>&amp;200%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation>&amp;Thumbnails</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Vis &amp;marger</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Vis &amp;rammer</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Vis b&amp;ilder</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Vis &amp;gitter</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Vis h&amp;jelpelinjer</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Vis &amp;Baseline gitter</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>Vis &amp;tekstlenker</translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>F&amp;est til gitter</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Fest til hjel&amp;pelinjer</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Egenskaper</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>&amp;Kladdebok</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>&amp;Lag</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation>&amp;Arranger sidene</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Bokmerker</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>&amp;Målinger</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>Aktiver &amp;Historie</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation>&amp;Juster og fordel</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>Verk&amp;tøy</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>P&amp;DF-Verktøy</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation>Velg objekt</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>T&amp;abell</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Form</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Linje</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation>&amp;Frihåndslinje</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Roter objekt</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Zoom inn eller ut</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>Zoom inn</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>Zoom ut</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Rediger innholdet i denne rammen</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Rediger tekst ...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Lenk sammen tekstrammer</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Fjern lenken mellom tekstrammene</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Rediger teksten med tekstbehandler</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Sett inn tekstramme</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>Sett inn bilderamme</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Sett inn tabell</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>Sett inn en form</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>Sett inn polygon</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>Sett inn linje</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>Sett inn Bezierkurve</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Sett inn frihåndtegnet linje</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>Hå&amp;ndter bilder</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;Orddel tekst</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>&amp;Lag innholdfortegnelse</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>&amp;Om Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>Om &amp;Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>Hjel&amp;petekster</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>Scribus h&amp;åndbok ...</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Non Breaking Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>Side&amp;nummer</translation>
    </message>
    <message>
        <source>New Line</source>
        <translation>Ny linje</translation>
    </message>
    <message>
        <source>Frame Break</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Column Break</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>Copyright</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation>Kule</translation>
    </message>
    <message>
        <source>Em Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>En Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Figure Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Quotation Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apostrophe</source>
        <translation>Apostroff</translation>
    </message>
    <message>
        <source>Straight Double</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Single Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Single Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Single Reversed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double Reversed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Single Left Guillemet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Single Right Guillemet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double Left Guillemet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double Right Guillemet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Low Single Comma</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Low Double Comma</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>CJK Single Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>CJK Single Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>CJK Double Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>CJK Double Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Toggle Palettes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation>Håndter egenskaper for siden ...</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="unfinished">Linjaler relativ i forhold til siden</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>Forhåndsvisning &amp;av utskrift</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Legg over hverandre</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>L&amp;egg ved siden av hverandre</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation>&amp;Om Plug-ins</translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation>Mer informasjon ...</translation>
    </message>
    <message>
        <source>Copy Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paste Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Printing Enabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Flip Horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Flip Vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Rulers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation type="unfinished">&amp;Oversikt</translation>
    </message>
    <message>
        <source>Solidus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Middle Dot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>En Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Em Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Thin Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Thick Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mid Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hair Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Smart Hyphen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Non Breaking Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Non Breaking Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation type="unfinished">Sett inn sidenummer</translation>
    </message>
    <message>
        <source>ff</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>fi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>fl</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ffi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ffl</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ft</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>st</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation type="obsolete">Flere valg</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation type="obsolete">Lager PostScript Nivå 3</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation type="obsolete">Lager kun PostScript Nivå 2,
OBS! Dette kan føre til store filer</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation type="obsolete">Lager kun PostScript Nivå 1,
OBS! Dette kan føre til store filer</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation type="obsolete">Speil siden(e) &amp;horisontalt</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation type="obsolete">Speil siden(e) &amp;vertikalt</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation type="obsolete">Bruk &amp;ICC profiler</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation type="obsolete">PostScript Nivå &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation type="obsolete">PostScript Nivå &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation type="obsolete">PostScript Nivå &amp;3</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation type="obsolete">Bruk under fje&amp;rning av farge</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">En måte å skru av noen av de gråskygger som er laget 
av cyan, gult og magenta rød ved å  bruke svart istedenfor. 
UCR virker best på deler av bilder som er nøytrale og/eller mørke toner 
som ligger nært opptil grått. Bruk dette for å få utsftening enkelteoen bilder til å se bedre; 
og noe eksperimentering og testing er nødvendig neste gang.
UCR reduserer muligheten for overmettning med CMY farger.</translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Distribute/Align</source>
        <translation type="obsolete">Fordel/Still opp på linje</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="obsolete">Still opp</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Vannrett</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation type="obsolete">Venstre sider</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation type="obsolete">Midten</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation type="obsolete">Høyre sider</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Loddrett</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation type="obsolete">Øverste sider</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation type="obsolete">Nederste sider</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> i</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="obsolete">&amp;Bruk</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation type="obsolete">&amp;Mellom:</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation type="obsolete">St&amp;ill opp</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation type="obsolete">For&amp;skyvning</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation type="obsolete">Fordel &amp;jevnt</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation type="obsolete">&amp;Mellom:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation type="obsolete">Gjør i&amp;ngen endringer</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation type="obsolete">S&amp;till opp</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation type="obsolete">For&amp;skyvning</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation type="obsolete">Fordel &amp;jevnt</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation type="obsolete">&amp;Gjør ingen endringer</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation>Juster og fordel</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Juster</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation>&amp;I forhold til:</translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation>Første valgte</translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation>Siste valgte</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Marger</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Utvalg</translation>
    </message>
    <message>
        <source>Align right sides of objects to left side of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align left sides of objects to right side of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation>Juster alle bunnene</translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation>Juster alle høyresider</translation>
    </message>
    <message>
        <source>Align tops of objects to bottom of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation>Juster alle venstresider</translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align bottoms of objects to top of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation>Juster alle topper</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation>Fordel</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation type="unfinished">Fordel </translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>&amp;Avstand:</translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation>Ingen valgt</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation type="unfinished"></translation>
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
        <translation>Juster tekst mot venstre</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Juster tekst mot høyre</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Midtstill tekst</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Juster tekstfyll</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Juster tekst tvungen fyll</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Felt egenskaper</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Type:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Knapp</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Tekstfelt</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Avkryssingsboks</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Kombiboks</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Listeboks</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Egenskaper</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Navn:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Hjelpetekst:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Grense</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farge:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Bredde:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Tynn</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Bred</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Stil:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Solid</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Punktet</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Understreket</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Med skrå kanter</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Sett inn</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Andre</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Skrivebeskyttet</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Nødvendig</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Ikke eksporter verdi</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Synlighet:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Synlig</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Skjult</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Ingen utskrift</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Ingen visning</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Utseende</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Tekst for knapp ned</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Tekst for rull rundt</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Ikoner</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Bruk ikoner</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Fjern</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Trykket ned</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Rull rundt</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Ikon plassering ...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Markert</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Inverter</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Utlinjet</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Dytt</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Multi-Linje</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Passord</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Maksimalt</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Tegn</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Ingen rulling</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Ingen stavekontroll</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Sjekk stil:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Sjekk</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Kryss</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Diamant</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Sirkel</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Stjerne</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Firkant</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Standard er aktiv</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Redigerbar</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Innstillinger</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation type="obsolete">Java Script</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Gå til</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Send inn skjema</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Nullstill skjema</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Importer data</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Hendelse:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Mus opp</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Mus ned</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Mus Enter</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Mus avslutt</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>På fokus</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>På Blur</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Skript:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Rediger ...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Send til URL:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Send data som HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Importer data fra:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Bestemmelsessted</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>Til fil:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Endre ...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Side:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-Pos:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-Pos:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Handling</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Felt er formatert som:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Enkel</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Nummer</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Prosentdel</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Dato</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Tid</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Brukerdefinert</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Nummerformat</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Desimaler:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Bruk valutasymbol</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Sett valutasymbol foran</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Formatering</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Prosentformat</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Datoformat</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Tidsformat</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Brukerdefinerte skript</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Format:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Tasteanslag:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Format</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Verdi er ikke kontrollert</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Verdi skal være lik med eller større enn:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>og lik med eller mindre enn:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Brukerdefinert valideringsscript:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Valider</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Verdi er ikke utregnet</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Verdien er</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>sum</translation>
    </message>
    <message>
        <source>product</source>
        <translation>produkt</translation>
    </message>
    <message>
        <source>average</source>
        <translation>gjennomsnitt</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>minimum</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>maksimum</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>av de følgende felter:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Velg ...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Brukerdefinert utregningsskript:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Regn ut</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Tast inn en kommaseparert liste med felter her</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Du behøver i det minste Normal-Ikonet for å bruke ikoner på knapper</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation type="obsolete">Bilder (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Eksempel:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Endring av valg</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Skrifttyper som skal brukes med PDF 1.3:</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Dette valget blir ikke brukt i PDF 1.3</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF Filer (*.pdf);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation>JavaScript</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation>Bilder (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;Alle filer (*)</translation>
    </message>
    <message>
        <source>None</source>
        <comment>highlight</comment>
        <translation type="unfinished">Ingen</translation>
    </message>
    <message>
        <source>None</source>
        <comment>action</comment>
        <translation type="unfinished">Ingen</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Kommentar egenskaper</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Lenke</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Ekstern lenke</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Ekstern Web-Lenke</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Bestemmelsessted</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF-Dokumenter (*.pdf);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Type:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Fo&amp;randre ...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Side:</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X-Pos</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Pos:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>Bruk på</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>Denne &amp;siden</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>&amp;Partallsider</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>&amp;Oddetallsider</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Alle sider</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation>Alt+W</translation>
    </message>
    <message>
        <source>to</source>
        <translation>til</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Apply the selected master page to even, odd or all pages within the following range</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Apply Template</source>
        <translation type="obsolete">Bruk mal</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="obsolete">&amp;Mal:</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation type="obsolete">Bruk på &amp;gjeldende side</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation type="obsolete">Bruk fra &amp;side:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation type="obsolete">Til:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation type="obsolete">Bruk på alle sider med &amp;partall</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation type="obsolete">Bruk på alle sider med &amp;oddetall</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
</context>
<context>
    <name>Barcode</name>
    <message>
        <source>&amp;Barcode Generator...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <source>Error opening file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error creating preview</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BarcodeGeneratorBase</name>
    <message>
        <source>Barcode Creator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Barcode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation type="unfinished">&amp;Type:</translation>
    </message>
    <message>
        <source>C&amp;ode:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select one of the available barcode type here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reset the barcode samples</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Include text in barcode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="unfinished">Alt+I</translation>
    </message>
    <message>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Guard whitespace</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Draw arrows to be sure of space next the code</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colors</source>
        <translation type="unfinished">Farger</translation>
    </message>
    <message>
        <source>&amp;Background</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Background color - under the code lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+L</translation>
    </message>
    <message>
        <source>Color of the lines in barcode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation type="unfinished">&amp;Tekst</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color of the text and numbers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hints and help is shown here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preview of the result. 72dpi sample.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>Kladdebok</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation type="obsolete">Kladdebøker (*.scs);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Slett</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Nytt innslag</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Endre navn</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Navnet &quot;%1&quot; er ikke entydig.
Velg vennligst et annet navn.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Greit</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Hent ...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Lagre</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Lagre &amp;som ...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation type="obsolete">&amp;Liten</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation type="obsolete">&amp;Medium</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation type="obsolete">&amp;Stor</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Forhåndsvisning</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation>Navnet &quot;%1&quot; er ikke unikt.
Velg et annet.</translation>
    </message>
    <message>
        <source>Choose a Scrapbook Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Velg en katalog</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>Bokmerker</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>Flytt bokmerke</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Sett inn bokmerke</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Bokmerker</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Ikonplassering </translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Utseende:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Kun tekst</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Kun ikon</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Tekst under ikon</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Tekst over ikon</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Tekst til høyre for ikon</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Tekst til venstre for ikon</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Tekst ovenpå ikon</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Skala:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Alltid</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Når ikonet er for lite</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Når ikonet er for stort</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Aldri</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Skaler hvordan:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proporsjonal</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Ikke-proporsjonal</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Ikon</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Tilbakestill</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Color Management Settings</source>
        <translation type="obsolete">Innstilling av fargestyring</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>System profiler</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Rendering Intenter</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Oppfattet</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relative Colorimetric</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Mettning</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolutt Colorimetric</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation type="obsolete">Standard farge profil for importerte bilder</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Standard fargeprofil for massive farger på siden</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Fargeprofil du har laget eller mottatt fra produsenten.
Denne profil burde være spesielt laget til din skjerm og ikke en generell profil (f.eks. sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Fargeprofil for din skrivermodel fra produsenten.
Denne profil burde være spesielt laget til din skriver og ikke en generell profil (f.eks. sRGB).</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standard rendering intent for din skjerm. Hvis ikke du vet hvorfor du burde
forandre det, bør du velge Relative Colorimetric eller Perceptual. </translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standard rendering intent for din skriver. Hvis ikke du vet hvorfor du burde
forandre det, bør du velge Relative Colorimetric eller Perceptual. </translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Aktiver &apos;soft proofing&apos; av hvordan dine dokument farger vil 
skrives ut basert på den valgte skriver profil.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Metode for å vise farger på skjermen som kanskje ikke skrives ut korrekt.
Dette krever veldig nøyaktige profiler og fungerer kun som advarsel.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Black Point Compensation er en metode for å øke kontrasten i fotografier.
Det er smart å aktivere dette hvis du har fotografier i ditt dokumentet.</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Aktiver fargekontroll</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation type="obsolete">&amp;Bilder:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>&amp;Solide farger:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Monitor:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>S&amp;kriver:</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>M&amp;onitor:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>S&amp;kriver:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Sim&amp;uler skriver på skjermen</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Marker farge ut av &amp;Gamut</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Bruk &amp;Blackpoint Compensation</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;RGB Pictures:</source>
        <translation>&amp;RGB bilder:</translation>
    </message>
    <message>
        <source>&amp;CMYK Pictures:</source>
        <translation>&amp;CMYK  bilder:</translation>
    </message>
    <message>
        <source>Default color profile for imported CMYK images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Rediger farger</translation>
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
        <source>Web Safe RGB</source>
        <translation>Websikkert RGB</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Ny</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Gammel</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Greit</translation>
    </message>
    <message>
        <source>C:</source>
        <translation>C:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
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
        <source>Dynamic Color Bars</source>
        <translation>Dynamiske fargesøyler</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Statiske fargesøyler</translation>
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
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation type="obsolete">Navnet på denne fargen er brukt allerede</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>HSV-Colormap</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>Farge&amp;model</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation type="obsolete">Du kan ikke lage en farge med navn &quot;%1&quot;. 
Dette navnet er reservert for transparente farger</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name of the color is not unique</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation>Velg Tegn:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Skrifttype:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>Sett &amp;inn</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>T&amp;øm</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Sett inn tegn ved markørens posisjon i teksten</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Fjern det valgte.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="obsolete">Lukk denne dialog og gå tilbake til tekstredigering.</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>Piler</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Specials</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Gresk</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Arabisk</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>Hebraisk</translation>
    </message>
    <message>
        <source>&amp;Insert Code:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type in a four digit unicode value directly here</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Glyphs missing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>Manglende bilde</translation>
    </message>
    <message>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>Ingen problemer funnet</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Side </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Frie objekter</translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation>Problemer funnet</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Items</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Problems</source>
        <translation>Problemer</translation>
    </message>
    <message>
        <source>Current Profile:</source>
        <translation>Gjeldende profil:</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation type="unfinished">&amp;fIgnorer feil</translation>
    </message>
</context>
<context>
    <name>CheckerPrefsList</name>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>PDF 1.3</source>
        <translation type="obsolete">PDF 1.3</translation>
    </message>
    <message>
        <source>PDF 1.4</source>
        <translation type="obsolete">PDF 1.4</translation>
    </message>
    <message>
        <source>PDF/X-3</source>
        <translation type="obsolete">PDF/X-3</translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation>Velg stil</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Tilgjengelige stiler</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Greit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Avbryt</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation>Velg en katalog</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <source>Colors</source>
        <translation type="unfinished">Farger</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importer</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Rediger</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation type="unfinished">&amp;Fjern ubrukte</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation type="unfinished">Fargesett</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation type="unfinished">Nåværende fargesett:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation type="unfinished">&amp;Lagre fargevalg</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation type="unfinished">Velg et fargesett å laste inn</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation type="unfinished">Lagre nåværende fargesett</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation type="unfinished">Slett ikke-brukte farger fra dette dokuments fargesett</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation type="unfinished">Lag en ny farge innenfor det nåværende sett</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation type="unfinished">Rediger den nåværende aktive farge</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation type="unfinished">Lag en kopi av den nåværende aktive farge</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation type="unfinished">Slett den nåværende aktive farge</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation type="unfinished">Sett det nåværende fargesett til standard fargesett</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished">&amp;Navn:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation type="unfinished">Velg et navn</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Åpne</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="unfinished">Kopi av %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation type="unfinished">Ny farge</translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <source>Monochromatic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Analogous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Complementary</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Split Complementary</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Triadic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tetradic (Double Complementary)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Base Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Monochromatic Light</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Monochromatic Dark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>1st. Analogous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>2nd. Analogous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>1st. Split</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>2nd. Split</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>3rd. Split</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>4th. Split</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>1st. Triadic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>2nd. Triadic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>1st. Tetradic (base opposite)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>2nd. Tetradic (angle)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>3rd. Tetradic (angle opposite)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorWheelDialog</name>
    <message>
        <source>Color Wheel</source>
        <translation>Fargehjul</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farge</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
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
        <translation>Velg metode:</translation>
    </message>
    <message>
        <source>Angle (0 - 90 degrees):</source>
        <translation>Vinkel (0 - 90 grader):</translation>
    </message>
    <message>
        <source>&amp;Merge Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Replace Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Merge created colors into the document colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error: </source>
        <translation>Feil:</translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal Vision</source>
        <translation>Noralt syn</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Vision Defect:</source>
        <translation>Synsdefekt:</translation>
    </message>
    <message>
        <source>Cr&amp;eate color...</source>
        <translation>&amp;Lag farge ...</translation>
    </message>
    <message>
        <source>&amp;Import existing color...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Merge colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Replace colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>A&amp;vslutt</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>F&amp;arge</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation type="obsolete">Verdi:</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sample color scheme</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colors of your chosen color scheme</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Ny farge</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;olor Components...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation>Protanopia (Rød)</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation>Deuteranopia (Grønn)</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation>Tritanopia (Blå)</translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation>Farge %1 finnes allerede!</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation>Farge %1 er lagt til.</translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation>&amp;Fargehjul ...</translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color selector with color theory included.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Bruk</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Lagre</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>None</source>
        <comment>color name</comment>
        <translation type="unfinished">Ingen</translation>
    </message>
    <message>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation type="unfinished">Brukerdefinert</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation type="unfinished">Enkel side</translation>
    </message>
    <message>
        <source>Double Sided</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation type="unfinished">Venstre side</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation type="unfinished">I midten</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation type="unfinished">I midten til venstre</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation type="unfinished">Høyre mot høyre</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="unfinished">Høyre side</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Shade:</source>
        <translation>Skygge:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Gjennomsiktighet:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Horisontal skravering</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Vertikal skravering</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Diagonal skravering</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Kryssdiagonal gradering</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Radial skravering</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Rediger egenskapene til linjefargen</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Rediger egenskapene til fyllfargen</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Metning av farge</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Normal eller skravering fyllmetode</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Sett gjennomsiktigheten av den valgte fargen</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation type="obsolete">Det valgte objektets farge</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Fri lineær skravering</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Fri radial skravering</translation>
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
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>Move Vector</source>
        <translation>Flytt vektor</translation>
    </message>
    <message>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Opsjoner for CVS import</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Skillefelt:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Skilleverdi:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Første rad er overskrift</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>None</source>
        <comment>delimiter</comment>
        <translation type="unfinished">Ingen</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Skriver innstillinger</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Sideoppsett</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Alle sider</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Kun partall sider</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Kun oddetall sider</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Speil</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nei</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Retning</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Stående</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Landskap</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>N-Up Utskriving</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Side per ark</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Sider per ark</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Innstilling</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Verdi</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Denne panel viser diverse CUPS innstillinger mens skriving pågår.
Hvilke parametre er tilgjengelige avhenger av din skriver driver.
Du kan sjekke for CUPS støtte ved å gå på Hjelp &gt; Om.
Se etter: C-C-T Disse står for C=CUPS C=littlecms T=Tiff støtte.
Manglende librarz støtte er indikert med et *</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>Encoding:</source>
        <translation>Kodering:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Denne bringer deg til din standard katalog for 
dokumenter. Denne kan du selv velge i Preferanser.</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Komprimer filen</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Inkluder skrifttypene</translation>
    </message>
</context>
<context>
    <name>CwSetColor</name>
    <message>
        <source>Set Color Components</source>
        <translation type="unfinished"></translation>
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
        <translation>Bruk &amp;RGB</translation>
    </message>
    <message>
        <source>Set C&amp;MYK</source>
        <translation>Bruk C&amp;MYK</translation>
    </message>
    <message>
        <source>Set &amp;HSV</source>
        <translation>Bruk &amp;HSV</translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <source>Cancelled by user</source>
        <translation>Avbrutt av bruker </translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Fjern farge</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation type="obsolete">Fjern farge:</translation>
    </message>
    <message>
        <source>?</source>
        <translation type="obsolete">?</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation type="obsolete">Erstatt den med:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation>Slett farge:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Erstatt med:</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Slett sider</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation type="obsolete">Slett fra:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>til:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation>Slett fra:</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation>Slett stilen</translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation>Slett stilen:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Erstatt med:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Ingen stil</translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>Missing Font</source>
        <translation type="obsolete">Manglende skrifttype</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation type="obsolete">Skrifttypen %1 er ikke installert.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="obsolete">Bruk</translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="obsolete">i stedet</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Greit</translation>
    </message>
</context>
<context>
    <name>DocIm</name>
    <message>
        <source>Importing failed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Importing Word document failed 
%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Dokument informasjon</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Tittel:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Forfatter:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>Nø&amp;kkelord:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>Beskri&amp;velse:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Utgiver:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;Bidragsytere:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>Da&amp;to:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>T&amp;ype:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>F&amp;ormat:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>Identi&amp;fiser:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Kilde:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Språk:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;Relasjon:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>Dek&amp;ning:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>Retti&amp;gheter:</translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation type="obsolete">&amp;Dokument</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Mer &amp;informasjon</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">Personen eller organisasjonen som primært er ansvarlig for å lage dokumentets innhold.
Dette felt kan bli inkludert i Scribus dokumentet for referanse, såvel som i metadataene til en PDF</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">Et navn som er gitt til dokumentet.
Dette felt kan bli inkludert i Scribus dokumentet for referanse, såvel som i metadataene til en PDF</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation type="obsolete">En beskrivelse av dokumentets innhold.
Dette felt er for en innholdsbeskrivelse eller sammenfatning av dokumentet. Det blir inkludert i PDFen ved eksport</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation type="obsolete">Temaet for dokumentets innhold.
Dette felt er for dokument nøkkelord som du vil inkludere i en PDF, til å assistere søkninger og indeksering av PDF filer</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>En person eller organisasjon som er ansvarlig for å gjøre dokumentet tilgjengelig</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>En person eller organisasjon som er ansvarlig for å bidra til dokumentets innhold</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>En dato assosiert med en hendelse i dokumentets livssyklus, i YYYY-MM-DD format, som per ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Formen eller genren for dokumentets innhold, f.eks kategorier, funksjoner, genrer, etc</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation type="obsolete">Den fysiske eller digitale manifestasjon av dokumentet. Media type og dimensjoner ville være nyttige å merke seg.
RFC2045,RFC2046 for MIME typer er også brukbare her</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>En entydig referanse til dokumentet innenfor en gitt kontekst som ISBN eller URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>En referanse til et dokument hvor det nåværende dokument er derivert, f.eks ISBN eller URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation type="obsolete">Språket som dokumentet er skrevet i, vanligvis en ISO-638 språk kode 
eventuelt fulgt av en dash og en ISO-3166 lande kode, f.eks nn-NO, nb-NO, fr-CH, en-GB</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>En referanse til et relatert dokument, kanskje ved bruk av en formell henvisning som ISBN eller URI </translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Avgrensningen for dokumentets innhold, kanskje inkluderende sted, tid og juridisk virkningsområde </translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Informasjoner omkring rettigheter holdet i og over dokumentet, f.eks copyright, patent eller trademark</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation>Dokumen&amp;t</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation type="unfinished"></translation>
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
        <source>&lt;b&gt;Name:&lt;/b&gt; Optional name for section eg. Index&lt;br/&gt;&lt;b&gt;Shown:&lt;/b&gt; Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;&lt;b&gt;From:&lt;/b&gt; The page index for this section to start at.&lt;br/&gt;&lt;b&gt;To:&lt;/b&gt; The page index for this section to stop at.&lt;br/&gt;&lt;b&gt;Style:&lt;/b&gt; Select the page number style to be used.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Number Out Of Bounds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Active</source>
        <translation type="obsolete">Aktiv</translation>
    </message>
    <message>
        <source>From</source>
        <translation>Fra</translation>
    </message>
    <message>
        <source>To</source>
        <translation>Til</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Stil</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Start</translation>
    </message>
    <message>
        <source>End</source>
        <translation type="obsolete">Slutt</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>Shown</source>
        <translation>Vist</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation>Tekstramme</translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation>Boolsk</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>Heltall</translation>
    </message>
    <message>
        <source>String</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Navn</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Type</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="obsolete">Verdi</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation type="obsolete">Parameter</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Legg til</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopier</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Delete</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">T&amp;øm</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+L</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation type="unfinished">Ingen</translation>
    </message>
    <message>
        <source>None</source>
        <comment>auto add</comment>
        <translation type="unfinished">Ingen</translation>
    </message>
    <message>
        <source>None</source>
        <comment>types</comment>
        <translation type="unfinished">Ingen</translation>
    </message>
    <message>
        <source>Real Number</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Verdi</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>T&amp;øm</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation>Skriverinnstillinger</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Innstillinger</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cyan</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Gult</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Svart</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Lagre som</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation type="obsolete">Postscript-filer (*.ps);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Skriv inn en kommaseparert liste med tegn hvor
tegnet * representerer alle sider, og 1-5 
representerer en rekke sider, eller en enkelt side.</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Valg av skriver</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Valg ...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Fil:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Fo&amp;randre ...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>A&amp;lternativ utskriftskommando</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>Ko&amp;mmando:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Område</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Skriv ut &amp;alt</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Skriv ut gjeldende &amp;side</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Utsk&amp;riftsområde</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>A&amp;ntall kopier:</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation type="obsolete">Skriv ut &amp;Normal</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation type="obsolete">Utskrift &amp;Seperasjoner</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation type="obsolete">Skri&amp;v ut i farger hvis tilgjengelig</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation type="obsolete">Skriv ut i grå&amp;farger</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation type="obsolete">F&amp;lere valg ...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Skriv ut</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation type="obsolete">Bruk en alternativ utskriftsbehandler, som kprinter eller gtklp,
for å bruke flere utskriftsopsjoner</translation>
    </message>
    <message>
        <source>Postscript Files (*.ps);;All Files (*)</source>
        <translation type="obsolete">Postscript filer (*.ps);;Alle filer (*)</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript Level 1</source>
        <translation>PostScript nivå 1</translation>
    </message>
    <message>
        <source>PostScript Level 2</source>
        <translation>PostScript nivå 2</translation>
    </message>
    <message>
        <source>PostScript Level 3</source>
        <translation>PostScript nivå 3</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Speilvend siden(e) horisontalt</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>Speilvend siden(e) vertikalt</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farge</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Flere valg</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation>Forhåndsvis ...</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">En måte å skru av noen av de gråskygger som er laget 
av cyan, gult og magenta rød ved å  bruke svart istedenfor. 
UCR virker best på deler av bilder som er nøytrale og/eller mørke toner 
som ligger nært opptil grått. Bruk dette for å få utsftening enkelteoen bilder til å se bedre; 
og noe eksperimentering og testing er nødvendig neste gang.
UCR reduserer muligheten for overmettning med CMY farger.</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>PostScript filer (*.ps);;Alle filer (*)</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation></translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation type="unfinished">Ikke vis objekter utenfor margene på utskrift eller eksportert fil</translation>
    </message>
    <message>
        <source>Failed to retrieve printer settings</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Import av fil:
%1
feilet!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Alvorlig feil</translation>
    </message>
    <message>
        <source>Converting Image:
%1
failed!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error</source>
        <translation type="unfinished">Feil</translation>
    </message>
</context>
<context>
    <name>EditMacroDialog</name>
    <message>
        <source>Editing Macro: &lt;b&gt;</source>
        <translation type="obsolete">Redigerer makro: &lt;b&gt;</translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; already exists.
Are you sure you want to overwrite it?
</source>
        <translation type="obsolete">Filen &apos;%1&apos; finnes allerede. 
Er du sikker på at du vil overskrive den?</translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation type="obsolete">Scribus - Ny makro</translation>
    </message>
    <message>
        <source>Editing Macro:</source>
        <translation type="obsolete">Redigerer makro:</translation>
    </message>
    <message>
        <source>The name of the macro being edited.</source>
        <translation type="obsolete">Navnet på makroen som blir redigert.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Discard all changes and exit.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Forkast alle endringer og avslutt.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes and exit.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Lagre endringene og avslutt.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+L</translation>
    </message>
    <message>
        <source>&amp;Save Source As...</source>
        <translation type="obsolete">&amp;Lagre kilden som ...</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="obsolete">Alt+S</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Rediger stil</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Tegn</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation type="obsolete">Effekter:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation type="obsolete">Vertikal avstand</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Linjeavstand</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabulatorer og innrykk</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Greit</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Navn på din avsnittsstil</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Skrifttypen til den markerte tekst eller objekt</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Skriftstørrelse</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Farge på tekstfyll</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Farge på tekststrek</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation type="obsolete">Gir et ekstrastort førstebokstav til et avsnitt. Brukt som stileffekt</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Bestemmer høyden, i linjeantall, av Drop Caps</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation type="obsolete">Juster tekst til baseline nettet</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Plass over avsnittet</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Plass under avsnittet</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation type="obsolete">Navnet på denne stilen er ikke entydig</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation type="obsolete">&amp;Skrifttype:</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation type="obsolete">Større&amp;lse:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation type="obsolete">&amp;Justering:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation type="obsolete">&amp;Fjern store bokstaver</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Linjer:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">&amp;Fyllfarge:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation type="obsolete">S&amp;trekfarge:</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation type="obsolete">Juster til baseline &amp;nettet</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation type="obsolete">Linjeav&amp;stand:</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation type="obsolete">Ov&amp;er:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation type="obsolete">&amp;Under:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation>Avstander</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>Avstand til tekst:</translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Bakgrunn</translation>
    </message>
    <message>
        <source>Select for easier reading of light coloured text styles</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Tekstbehandler</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation type="obsolete">Javascripts (*.js);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Åpne ...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Lagre &amp;som ...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Lagre og avslutt</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Avslutt uten å lagre</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Angre</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Gjør omigjen</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Klipp ut</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>K&amp;opier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Lim inn</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>T&amp;øm</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Hent navn på felt</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Rediger</translation>
    </message>
    <message>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>JavaScript (*.js);;Alle filer (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation>Bildeeffekter</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Innstillinger:</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farge:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Skygge:</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Contrast:</source>
        <translation>Kontrast:</translation>
    </message>
    <message>
        <source>Radius:</source>
        <translation>Radius:</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation>Verdi:</translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation>Tilgjengelige effekter</translation>
    </message>
    <message>
        <source>Blur</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Brightness</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colorize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Contrast</source>
        <translation>Kontrast</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Gråskala</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Inverter</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&gt;&gt;</source>
        <translation>&gt;&gt;</translation>
    </message>
    <message>
        <source>&lt;&lt;</source>
        <translation>&lt;&lt;</translation>
    </message>
    <message>
        <source>Effects in use</source>
        <translation>Effekter i bruk</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Velg en katalog for eksport</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Alle sider</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Forandre utskriftskatalog</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Utskriftskatalogen - stedet for å lagre bildene dine.
Navnet for den eksporterte fil vil være &apos;dokumentnavn-sidenummer.filtype&apos;</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Eksporter kun gjeldene side</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Tilgjengelige eksport formater</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Endre ...</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Eksporter til katalog:</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>Bilde&amp;format:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Kvalitet:</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Eksporter som bilde(r)</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Innstillinger</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Oppløsning:</translation>
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
        <translation>Område</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>&amp;Gjeldende side</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Område</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Eksporter et sett av sider</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Skriv inn en kommaseparert liste med tegn hvor
tegnet * representerer alle sider, og 1-5 
representerer en rekke sider, eller en enkelt side. </translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Eksporter alle sidene</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Oppløsning av bildene 
Bruk 72 dpi for bilder som er ment for skjermen</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Bildekvalitet - 100% er best, 1% er lavest kvalitet</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Størrelse på bildene. 100% for ingen endring, 200% for dobbelt så stort etc.</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hue</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farge</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Skjerm</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Oppløse</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Forskjell</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Ugjennomsiktighet:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Bakgrunn</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Lag</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Stier</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Størrelse:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Tittel:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Ingen tittel</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Forfatter:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Ukjent</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus dokument</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Oppløsning:</translation>
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
        <translation>Gråskala</translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Colors</source>
        <translation type="obsolete">Farger</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation type="obsolete">Fargesett</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation type="obsolete">Nåværende fargesett:</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation type="obsolete">Velg et fargesett å laste inn</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation type="obsolete">Lagre nåværende fargesett</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation type="obsolete">Slett ikke-brukte farger fra dette dokuments fargesett</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation type="obsolete">Legg til farger til nåværende sett fra et eksisterende dokument</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation type="obsolete">Lag en ny farge innenfor det nåværende sett</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation type="obsolete">Rediger den nåværende aktive farge</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation type="obsolete">Lag en kopi av den nåværende aktive farge</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation type="obsolete">Slett den nåværende aktive farge</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation type="obsolete">Sett det nåværende fargesett til standard fargesett</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation type="obsolete">Velg et navn</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">Åpne</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumenter (*.sla *.sla.gz *.scd *scd.gz);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokumenter (*.sla *.scd);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Kopi av %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation type="obsolete">Ny farge</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Legg til</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">&amp;Rediger</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">D&amp;upliker</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Slett</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation type="obsolete">&amp;Fjern ubrukte</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation type="obsolete">&amp;Lagre fargevalg</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Navn:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Importer</translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation>Noen skrifttyper i dette dokumentet ar blitt erstattet:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation>  ble erstatet av: </translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Global Font Settings</source>
        <translation type="obsolete">Globale skrifttypeinnstillinger</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Tilgjengelige skrifttyper</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Erstatningsskrifttyper</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Andre stier</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ja</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Navn på skrifttype</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation type="obsolete">Bruk skrifttype</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation type="obsolete">Inkluder i:</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation type="obsolete">Senket skrift</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Type</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation type="obsolete">Sti til skrifttypefil</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>&amp;Tilgjengelige skrifttyper</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Erstatning</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Erstatningsskrifttyper</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>Andre &amp;stier</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Velg en katalog</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Endre ...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>&amp;Legg til ...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Fjern</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Navn på skrifttype</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Bruk skrifttype</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation>Inkluder i:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Senket skrift</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>Sti til skrifttypefil</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">Vevet silke pyjamas byttet for blå quartz</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <translation type="obsolete">Forhåndsvisning av skrifttyper</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation type="obsolete">Forlatt forhåndsvisning</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation type="obsolete">Navn på skrifttype</translation>
    </message>
    <message>
        <source>Type</source>
        <comment>font preview</comment>
        <translation type="obsolete">Type</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation type="obsolete">Senket skrift</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <comment>font preview</comment>
        <translation type="obsolete">Forhåndsvisning av skrifttyper</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>font preview</comment>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <comment>font preview</comment>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>Quick Search: </source>
        <translation type="obsolete">Raskt søk: </translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="obsolete">&amp;Søk</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <comment>font preview</comment>
        <translation type="obsolete">&amp;Lukk</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <comment>font preview</comment>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Forlatt forhåndsvisning</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. E.g. &apos;bold&apos; shows all fonts with Bold in name. Searching is case insensitive.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Bruker</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>System</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Legg til</translation>
    </message>
    <message>
        <source>Sample will be shown after key release</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <source>Fonts Preview</source>
        <translation type="unfinished">Forhåndsvisning av skrifttyper</translation>
    </message>
    <message>
        <source>&amp;Quick Search:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="unfinished">&amp;Søk</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="unfinished">Alt+S</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation type="unfinished">Navn på skrifttype</translation>
    </message>
    <message>
        <source>Doc</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Type</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation type="unfinished">Senket skrift</translation>
    </message>
    <message>
        <source>Access</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="unfinished">&amp;Skriftstørrelse:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="unfinished">Tekst</translation>
    </message>
    <message>
        <source>Sample text to display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Se&amp;t</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reset the text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="unfinished">&amp;Legg til</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Lukk</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
</context>
<context>
    <name>FontPreviewPlugin</name>
    <message>
        <source>&amp;Font Preview...</source>
        <translation>&amp;Forhåndsvisning av skrifttype ...</translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Posisjon:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Her kan du legge til, forandre eller fjerne Color-Stops.</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Håndtering av hjelpelinjer</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Horisontale hjelpelinjer</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Vertikale hjelpelinjer</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Pos:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Pos:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>Lås hje&amp;lpelinjer</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Rows and Columns - Automatic Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Rows:</source>
        <translation>&amp;Rader:</translation>
    </message>
    <message>
        <source>C&amp;olumns:</source>
        <translation>&amp;Kolonner:</translation>
    </message>
    <message>
        <source>Row &amp;Gap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colum&amp;n Gap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Refer to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>Si&amp;de</translation>
    </message>
    <message>
        <source>&amp;Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Selection</source>
        <translation>&amp;Utvalg</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>&amp;Update</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Apply to All Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="unfinished">Forhåndsvis</translation>
    </message>
    <message>
        <source>There is empty (0.0) guide already</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Beklager, ingen håndbok er tilgjengelig. Ta en titt på http://docs.scribus.net for oppdaterte håndbøker, og www.scribus.net for nye versjoner av Scribus.</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Innhold</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Lenke</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Scribus Online hjelp</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;Innhold</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Søk</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation>Sø&amp;k</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nytt</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation>Slett a&amp;lle</translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation>Bok&amp;merker</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>Skriv &amp;ut ...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>&amp;Avslutt</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>unknown</source>
        <translation type="obsolete">ukjent</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Finn</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Søk etter:</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>Nytt bokmerke</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Navn på bokmerke:</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation>&amp;Finn ...</translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation>Finn &amp;neste</translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation>Finn f&amp;orrige</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Rediger</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation>&amp;Legg til </translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>S&amp;lett alle</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Bokmerker</translation>
    </message>
    <message>
        <source>Relevance</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Mulig orddeling</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Aksepter</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Hopp over</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Hyphenator Settings</source>
        <translation type="obsolete">Orddelings innstillinger</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Lengden av det minste ord som deles.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Maksimum antall orddelinger som følger etter hverandre.
0 betyr uendelige orddelinger.</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Språk:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;Minste ord:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Bruk orddeling automatisk mens du skriver</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Skrur på automatisk orddeling mens du skriver.</translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <source>Image Info</source>
        <translation>Bilde info</translation>
    </message>
    <message>
        <source>General Info</source>
        <translation>Generell info</translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation>Dato / Tid:</translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nei</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation>Profilnavn:</translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation>Inneholder lag:</translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation>EXIF Info</translation>
    </message>
    <message>
        <source>Artist:</source>
        <translation>Artist:</translation>
    </message>
    <message>
        <source>Comment:</source>
        <translation>Kommentar:</translation>
    </message>
    <message>
        <source>User Comment:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation>Kamera modell:</translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Beskrivelse:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Copyright:</translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation>Skanner modell:</translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Importer &amp;EPS/PS ...</translation>
    </message>
    <message>
        <source>Imports EPS Files</source>
        <translation>Importer EPS filer</translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>PDF</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Sett inn side</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation type="obsolete">Setter inn</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>før side</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>etter side</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>på slutten</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation type="obsolete">Mal (Høyre side):</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation type="obsolete">&amp;Setter inn</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>side(r)</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation type="obsolete">&amp;Mal (venstre side):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="obsolete">&amp;Mal:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>Sett &amp;inn</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Brukerdefinert</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Ret&amp;ning:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Stående</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Landskap</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Høyde:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Sett inn tabell</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation type="obsolete">Antall rader:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation type="obsolete">Antall kolonner:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Greit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Avbryt</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation>Antall rader:</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>Antall kolonner:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Rediger JavaScript</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation>Nytt skript</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Rediger ...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Legg til ...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Nytt skript:</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;nei</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation type="obsolete">Vil du virkelig slette dette skriptet?</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation>Ønsker du virkelig å slette dette skriptet?
</translation>
    </message>
    <message>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation type="obsolete">Håndtering av hurtigtaster</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Handling</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Nåværende hurtigtast</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Velg en hurtigtast for denne handling</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT + SHIFT + T</translation>
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
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Ingen hurtigtast</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>Br&amp;ukerdefinert hurtigtast</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Sett hurti&amp;gtast</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation type="obsolete">Denne hurtigtasten er allerede i bruk</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation>&amp;Hent</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation>&amp;Importer ...</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation>&amp;Eksporter ...</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Meta</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Meta+</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation>Lag</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation type="obsolete">Tilføy et ny lag</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Slett lag</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation type="obsolete">Hev lag</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation type="obsolete">Senk lag</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation type="obsolete">Nytt lag</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation type="obsolete">Vil du også slette alle objekter på dette laget?</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation>Legg et nytt lag</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Slett lag</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Hev lag</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Senk lag</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Rediger linjestiler</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopi av %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Ny stil</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *scd.gz);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.scd);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Legg til</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Rediger</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;upliker</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Lagre</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nei</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="obsolete">Vil du virkelig slette denne stilen?</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation>Ønsker du virkelig å slette denne stilen?</translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <source>All Files (*)</source>
        <translation>Alle filtyper (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation>Velg Lorem Ipsum</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Forfatter:</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation>Hent mer:</translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation>XML fil:</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation></translation>
    </message>
    <message>
        <source>Paragraphs:</source>
        <translation>Avsnitt:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
</context>
<context>
    <name>MSpinBox</name>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
</context>
<context>
    <name>MacroManager</name>
    <message>
        <source>Manage Macros</source>
        <translation type="obsolete">Behandle makroer</translation>
    </message>
    <message>
        <source>Brings up a graphical window for creating, deleting, editing, saving and loading macros.</source>
        <translation type="obsolete">Starter et grafisk vindu for opprettelse, sletting, redigering, lagring og lasting av makroer.</translation>
    </message>
    <message>
        <source>Create, edit and delete macros</source>
        <translation type="obsolete">Lag, rediger eller slett makroer</translation>
    </message>
    <message>
        <source>&amp;Macro</source>
        <translation type="obsolete">&amp;Makro</translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="obsolete">Scribus - Makro behandler</translation>
    </message>
    <message>
        <source>Unable to open the requested file: %1</source>
        <translation type="obsolete">Klarte ikke å åpne fila: %1</translation>
    </message>
    <message>
        <source>Scribus - Edit Macro</source>
        <translation type="obsolete">Scribus - Rediger makro</translation>
    </message>
    <message>
        <source>&lt;qt&gt;The macro name you requested is already taken  by another macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Navnet du valgte for din makro er allerede gitt til en annen makro.&lt;qt&gt;</translation>
    </message>
</context>
<context>
    <name>ManageMacrosDialog</name>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="obsolete">Scribus - Makro behandler</translation>
    </message>
    <message>
        <source>Scribus - Manage Macros</source>
        <translation type="obsolete">Scribus - Behandle makroer</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Ny</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation type="obsolete">Alt+N</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Lag en ny makro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>Close this dialog.</source>
        <translation type="obsolete">Avslutt denne dialogen</translation>
    </message>
    <message>
        <source>Return to Scribus.</source>
        <translation type="obsolete">Gå tilbake til Scribus</translation>
    </message>
    <message>
        <source>Macro</source>
        <translation type="obsolete">Makro</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Rediger</translation>
    </message>
    <message>
        <source>Description</source>
        <translation type="obsolete">Beskrivelse</translation>
    </message>
    <message>
        <source>Rena&amp;me</source>
        <translation type="obsolete">Gi &amp;nytt navn</translation>
    </message>
    <message>
        <source>Alt+M</source>
        <translation type="obsolete">Alt+M</translation>
    </message>
    <message>
        <source>Rename the selected macro.</source>
        <translation type="obsolete">Gi nytt navn til den valgte makroen.</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation type="obsolete">&amp;Rediger ...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation type="obsolete">Alt+E</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Delete</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+D</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="obsolete">Alt+S</translation>
    </message>
    <message>
        <source>E&amp;xport</source>
        <translation type="obsolete">E&amp;ksport</translation>
    </message>
    <message>
        <source>Alt+X</source>
        <translation type="obsolete">Alt+X</translation>
    </message>
    <message>
        <source>Export macros to a file.</source>
        <translation type="obsolete">Eksporter makroer til en fil.</translation>
    </message>
    <message>
        <source>Delete &amp;All</source>
        <translation type="obsolete">Slett &amp;alle</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <source>Delete all macros.</source>
        <translation type="obsolete">Slett alle makroer.</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Importer</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="obsolete">Alt+I</translation>
    </message>
</context>
<context>
    <name>MarginDialog</name>
    <message>
        <source>Margin Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Manage Page Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Brukerdefinert</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Ret&amp;ning:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Stående</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Landskap</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Høyde:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Type:</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Bunn:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Topp:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Høyre:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Venstre:</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preset Layouts:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Printer Margins...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import the margins for the selected page size from the available printers.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nei</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopi av %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Navn:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopi #%1 av </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Dupliker flere ganger</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Antall kopier:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Horisontale forskyvning:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Vertikal forskyvning:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Avstander</translation>
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
        <translation>Vinkel:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Lengde:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>pt</source>
        <translation>pt</translation>
    </message>
</context>
<context>
    <name>MenuTest</name>
    <message>
        <source>Script error</source>
        <translation type="obsolete">Script feil</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation type="obsolete">Hvis du kjører et offisielt skript reporter det vennligst til 
&lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">Vis &amp;konsollen</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">Skjul &amp;konsollen</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation type="obsolete">Denne meldingen er overført til ditt klippebord, bruk Ctrl+V for å lime den inn i feilmeldingssøkeren.</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Change...</source>
        <translation type="obsolete">Endre ...</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="obsolete">Importer</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Avbryt</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *scd.gz);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.scd);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Import Template</source>
        <translation type="obsolete">Importer en mal</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importer side(r)</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation type="obsolete">Fra dokument:</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation type="obsolete">Importer side(r):</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Skriv inn en kommaseparert liste med tegn hvor
tegnet * representerer alle sider, og 1-5 
representerer en rekke sider, eller en enkelt side.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> fra 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Lag side(r)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">før side</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">etter side</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">på slutten</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> fra %1</translation>
    </message>
    <message>
        <source>Import Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation>&amp;Fra dokument:</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>E&amp;ndre ...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>&amp;Importer side(r):</translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Før side</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Etter side</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>På slutten</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>Manglende skrifttype</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Skrifttypen %1 er ikke installert.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Bruk</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>i stedet</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Flytt sider</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Kopier side</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Flytt side(r):</translation>
    </message>
    <message>
        <source>to:</source>
        <translation type="obsolete">til:</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">før side</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">etter side</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">på slutten</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation>Flytt side(r)</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Før side</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Etter side</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>På slutten</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>Til:</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation>Antall kopier:</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>Egenskaper</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometri</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Basepoint:</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Nivå</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Form:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Avstand til tekst</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Tekststi egenskaper</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Vis kurve</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Start forskyvning:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Avstand fra kurve:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="obsolete">Skygge:</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation type="obsolete">Brukerdefinert avstand</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Input profil:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Rendering Intent:</translation>
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
        <translation>Metning</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolutt Colorimetric</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Venstre punkt</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Endepunkter</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Mitter sammenføyning</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Bevel sammenføyning</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Rund sammenføyning</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Flat Cap</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Firkant Cap</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Rund Cap</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Ingen stil</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Cellelinjer</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Linje ved topp</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Linje til venstre</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Linje til høyre </translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Linje ved bunnen</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Det valgte objektets navn</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Horisontal posisjon av nåværende basepoint</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Vertikal posisjon av nåværende basepoint</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Bredde</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Høyde</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Rotasjon av objekt ved nåværende basepoint</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Punkt hvorfra målinger eller rotasjonsvinkler er regnet ut</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Velg venstre på toppen som basepoint</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Velg høyre på toppen som basepoint</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Velg venstre på bunnen som basepoint</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Velg høyre på bunnen som basepoint</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Velg senter som basepoint</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Speil horisontalt</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Speil vertikalt</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Flytt et nivå opp</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Flytt et nivå ned</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Plasser fremst</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Plasser bakerst</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Indikerer nivået objektet er på, 0 betyr at objektet er på bunnen</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Lås objektet eller fjern låsing</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Lås eller fjern låsing av objektets størrelse</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Aktiver eller deaktiver utskriving av objektet</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Få tekst i lavere rammer til å flyte rundt objektets form</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Skrifttype til den valgte tekst eller objekt</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Skriftstørrelse</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Skallering bredde av tegn</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation type="obsolete">Tekststrekfarge</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation type="obsolete">Tekstfyllfarge</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Metning av tekststrekens farge</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Metning av tekstfyllets farge</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="obsolete">Manuell Kerning</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Linjeavstand</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Stil til gjeldende avsnitt</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Endre innstillinger for venstre eller endepunkter</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Linjemønster</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Linjetykkelse</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Form for linjesammenføyninger</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Form for linjeavslutninger</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>linjestil for gjeldende objekt</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Velg rammens form ...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Endre rammens form ...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Sett radius på hjørneavrunding</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Antall kolonner i tekstrammen</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Skifter mellom mellomrom og kolonnebredde</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Avstand mellom koloner</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Avstand av tekst til rammens topp</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Avstand av tekst til rammens bunn</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Avstand av tekst til rammens venstreside</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Avstand av teksten til rammens høyreside</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Rediger tab innstillinger av tekst rammen ...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Tillat bildet å være en annen størrelse i forhold til rammen</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Horisontal forskyving av bildet innenfor rammen</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Vertikal forskyving av bildet innenfor rammen</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Skaler bildet horisontalt</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Skaler bilder vertikalt</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Behold X og Y skaleringen</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Behold høyde/bredde forhold</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Få bildet til å passe innenfor rammens størrelse</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Bruk bilde proporsjoner istedenfor rammens</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Kildeprofil til bildet</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Rendering intent for bildet</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Kolonnebredde</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Navnet &quot;%1&quot; er ikke entydig.
Vennligst velg et annet navn.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Greit</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Form</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Tekst</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Bilde</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Linje</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Farger</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Pos:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Pos:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Høyde:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotasjon:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Rediger form ...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>R&amp;unde
hjørner:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Kolo&amp;nner:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Mellomrom:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>To&amp;pp:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Bunn:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Venstre:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Høyre:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>&amp;Tabulatorer ...</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>Tekst &amp;flyter rundt ramme</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>&amp;Bruk Bounding boks</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>Br&amp;uk kontur linjer</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="obsolete">&amp;Skriftstørrelse:</translation>
    </message>
    <message>
        <source>&amp;Kerning:</source>
        <translation type="obsolete">&amp;Kerning:</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation type="obsolete">Linjeav&amp;stand:</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>St&amp;il:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>Sp&amp;råk:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Fri Skalering</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>X-sk&amp;ala:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>Y-ska&amp;la:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Skaler &amp;til rammestørrelse</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>P&amp;roporsjonal</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Basepoint:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>Li&amp;njetype:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Linje &amp;bredde:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>Ka&amp;nter:</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Endelser:</translation>
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
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation>Bruk en omkringliggende boks fremfor rammens form for tekstflyt</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Bruk en annen linje opprinnelig basert på rammens form for tekstflyten</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Orddelingsspråk for rammen</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Høyre mot venstre skriving</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation>Faktisk X-DPI:</translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation>Faktisk Y-DPI:</translation>
    </message>
    <message>
        <source>Start Arrow:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation type="unfinished">Navnet &quot;%1&quot; er ikke unikt. &lt;br/&gt;Velg et annet.</translation>
    </message>
    <message>
        <source>Color of text stroke. Only available with &quot;outline&quot; text decoration.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color of text fill. Only available with &quot;outline&quot; text decoration.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fill Rule</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Even-Odd</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Non Zero</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation>Rediger stil</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Flat Cap</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Firkant Cap</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Rund Cap</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Midt sammenføyning</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Bevel sammenføyning</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Rund sammenføyning</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Linjetykkelse:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source> pt </source>
        <translation> pt </translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Heltrukken linje</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Prikket linje</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Prikket linje</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Bindestrek punktum linje</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Bindestrek punktum punktum linje</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Navnet &quot;%1&quot; er ikke entydig.
Vennligst velg et annet navn.</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Navnet &quot;%1&quot; er ikke unikt. &lt;br/&gt;Velg et annet.</translation>
    </message>
</context>
<context>
    <name>MultiProgressDialogBase</name>
    <message>
        <source>Progress</source>
        <translation>Fremgang</translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Edit Templates</source>
        <translation type="obsolete">Rediger maler</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Navn:</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation type="obsolete">Ny mal</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Kopi av %1</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">Kopi #%1 av </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Legg til</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Ny</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">D&amp;upliker</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Slett</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Lukk</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;No</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Navn:</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation type="obsolete">Vil du virkelig slette denne malen?</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <source>My &amp;Plugin</source>
        <translation>Min &amp;Plugin</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation>Scribus - Min Plugin</translation>
    </message>
    <message>
        <source>The plugin worked!</source>
        <translation>Plugin virket!</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>New Document</source>
        <translation>Nytt Dokument</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Brukerdefinert</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Stående</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Landskap</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Marg hjelpelinjer</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Innstillinger</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation type="obsolete">Punkter (pts)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Millimeter (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Tommer (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Picas (p)</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation type="obsolete">Kolonne hjelpelinjer</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Dokument side størrelse, enten en standard størrelse eller en brukerdefinert størrelse</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Retning for dokumentets sider</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Bredde av dokumentets sider, kun redigerbar hvis du har valgt en brukerdefinert sidestørrelse</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Høyde av dokumentets sider, kun redigerbar hvis du har valgt en brukerdefinert sidestørrelse</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Aktiver singel eller spredt basert layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">Lag første side venstre side av dokumentet</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Avstanden mellom topp hjelpemarg og sidens kant</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Avstanden mellom bunn hjelpemarg og sidens kant</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Hvis Avstand mellom venstre hjelpemarg og sidens kant.
Hvis Motstående Sider er markert kan denne brukes til å få til den korrekte marg for binding</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Avstand mellom høyre hjelpemarg og sidens kant.
Hvis Motstående Sider er markert kan denne brukes til å få til den korrekte marg for binding</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation type="obsolete">Første sidenummer av dokumentet</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Standard enhet for måling ved dokument redigering</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Lag tekst rammer automatisk når nye sider er tilføyet</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Antall kolonner som blir laget i automatisk lagde tekstrammer</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Avstand mellom automatisk lagde kolonner</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Letter</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Tabloid</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Ret&amp;ning:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Høyde:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Motstående sider</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Venstre &amp;side først</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">&amp;Venstre:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">&amp;Høyre:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">&amp;Topp:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">&amp;Bunn:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="obsolete">Fø&amp;rste sidenummer:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Stan&amp;dard enhet:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>Automatiske t&amp;ekstrammer</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Mellomrom:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Kolo&amp;nner:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Innenfor:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="obsolete">U&amp;tenfor:</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="obsolete">Executive</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation type="obsolete">Folio</translation>
    </message>
    <message>
        <source>Open Document</source>
        <translation type="obsolete">Åpne dokument</translation>
    </message>
    <message>
        <source>Recent Documents</source>
        <translation type="obsolete">Nylig åpnede dokumenter</translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation>Ikke vis denne dialogboksen igjen</translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation>Antall sider i dette dokumentet</translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation>Antall s&amp;ider:</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="obsolete">Dokumenter (*.sla *.sla.gz *.scd *.scd.gz);;</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="obsolete">Dokumenter (*.sla *.scd);;</translation>
    </message>
    <message>
        <source>Postscript Files (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="obsolete">Postscript filer (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation type="obsolete">SVG bilder (*.svg *.svgz);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg);;</source>
        <translation type="obsolete">SVG bilder (*.svg);;</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;</source>
        <translation type="obsolete">OpenOffice.org Draw (*.sxd);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation>&amp;Nytt dokument</translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation>Åpne &amp;eksisterende dokument</translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation>Nylig åpnede &amp;dokumenter</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Ny &amp;fra mal ...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NewTm</name>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Venstre side</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Høyre side</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Noder</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Flytt noder</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Flytt kontrollpunkter</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Legg til noder</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Slett noder</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Nullstill Kontrollpunktene</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Nullstill dette kontrollpunkt</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation type="obsolete">Hvis markert bruker Koordinater relativt til Siden,
ellers er Koordinater relative til Objektet.</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Absolutte Koordinater</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Posisjon:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Posisjon:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>Rediger &amp;konturlinjer</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>Nullstill kontu&amp;rlinjer</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>Avslutt r&amp;edigeringen</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Beveg kontrollpunkter uavhengig</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Beveg kontrollpunkter symmetrisk</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Åpen en Polygon eller skjærer en bezierkurve</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Lukk denne bezierkurve</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Speil stien horisontalt</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Speil stien vertikalt</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Shear stien horisontalt til venstre</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>SHera stien horisontalt opp</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Shear stien vertikalt ned</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Roter stien mot klokka</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Roter stien med klokka</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation type="obsolete">Reduser stiens størrelse med viste %</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Forstørr stiens størrelse med viste %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Rotasjonsvinkel</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation type="obsolete">% å forstørre eller forminske med</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Aktiver konturlinjeediteringsmodus</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Tilbakestill konturlinjen til objektets originale form</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>ShearShear stien horisontalt til høyre</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shrink the Size of the Path by shown %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>% to Enlarge or Shrink By</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Value to Enlarge or Shrink By</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Importer &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation>Importerer OpenOffice.org Draw filer</translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation>Importerer de fleste OpenOffice.org Draw filene inn i det gjeldene dokumentet, ved å konvertere deres vektor data til Scribus objekter.</translation>
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
        <source>OpenDocument Importer Options</source>
        <translation>Opsjoner for import av OpenDocument </translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Bruk filnavnet som begynnelse på navnet på stilen til avsnittet</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Ikke spør igjen</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Avbryt</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus dokument</translation>
    </message>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation>Scribus 1.2.x dokument</translation>
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
        <translation>Linjebredde</translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation type="obsolete">Skript konsoll</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
        <translation>Lagre som PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>&amp;Send til fil:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>E&amp;ndre ...</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation>Lag en fil for &amp;hver side</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Lagre</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Lagre som</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF Filer (*.pdf);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Export Range</source>
        <translation type="obsolete">Eksporter område</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="obsolete">Fil innstillinger</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="obsolete">Venstre marg</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="obsolete">Høyre marg</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="obsolete"> dpi</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="obsolete">Bilde innstillinger</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="obsolete">Automatisk</translation>
    </message>
    <message>
        <source>JPEG</source>
        <translation type="obsolete">JPEG</translation>
    </message>
    <message>
        <source>Zip</source>
        <translation type="obsolete">Zip</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="obsolete">Maksimum</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="obsolete">Høyde</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Medium</translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="obsolete">Lav</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="obsolete">Minimum</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="obsolete">&amp;Generelt</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation type="obsolete">Inkluder</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="obsolete">Tilgjengelige skrifttyper:</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation type="obsolete">Skrifttyper som skal inkluderes:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="obsolete">&amp;Skrifttyper</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Side</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="obsolete">Effekter</translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="obsolete"> sec</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="obsolete">Ingen effekt</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation type="obsolete">Persienner</translation>
    </message>
    <message>
        <source>Box</source>
        <translation type="obsolete">Boks</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="obsolete">Oppløse</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation type="obsolete">Glitter</translation>
    </message>
    <message>
        <source>Split</source>
        <translation type="obsolete">Dele</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation type="obsolete">Tørk av</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Horisontalt</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Vertikalt</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="obsolete">Innenfor</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="obsolete">Ytterside</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="obsolete">Venstre mot høyre</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="obsolete">Topp til bunn</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="obsolete">Bunn til topp</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="obsolete">Høyre mot venstre</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="obsolete">Topp-venstre til Bunn-Høyre</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="obsolete">Passord</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="obsolete">Innstillinger</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="obsolete">Generelt</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation type="obsolete">Skjerm / Web</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="obsolete">Skriver</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="obsolete">Massive farger:</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="obsolete">Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation type="obsolete">Rendering-Intent:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="obsolete">Perceptual</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="obsolete">Relative Colorimetric</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="obsolete">Metning</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="obsolete">Absolutt Colorimetric</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation type="obsolete">Bilder:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="obsolete">Ikke bruk inkluderte ICC profiler</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="obsolete">PDF/X-3 Output Intent</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="obsolete">Trim boks</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="obsolete">Eksporter alle sider til PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="obsolete">Eksporter et område av sider til PDF</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation type="obsolete">Bestemmer PDf kompatibiliteten. Standard er Acrobat 4.0 som gir den største kompatibilitet.
Velg Acrobat 5.0 hvis din fil har PDF 1.4 egenskaper som gjennomsiktighet eller hvis du trenger 128 bit kryptering.
PDF/X-3 er for å eksportere PDf til kommersiell printing og kan kan velges dersom du har aktivert farge management.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="obsolete">Bestemmer bindingen av sider i PDFen. Hvis ikke du er sikker
på at du må forandre det, bruk Venstre.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="obsolete">Lager thumbnails av hver side i PDFen.
Noen visere kan bruke thumbnails for navigering.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="obsolete">Lag PDF artikler, noe som er nyttig for å navigere lenkede artikler i en PDF.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="obsolete">Legg inn bokmerkene du lagde i ditt dokument.
Disse er nyttige for å navigere i lange PDF filer.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="obsolete">Eksporter resolutionen til tekst og vektor grafikk.
Dette har ingen innflytelse på oppløsningen til bitmap bilder som fotografier.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Komprimering av tekst og grafikk.
Hvis ikke du har noen grunn, bruk dette. Det reduserer PDF størrelsen.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Versjon av komprimering for bilder.
Automatisk tillater Scribus å velge den beste metoden.
ZIP er bra for bilder med solide farger.
JPEG er bedre for å lage små PDF filer som har mane fotografier (mens det kan forekomme tap i bildekvaliteten).
La det stå på automatisk, hvis ikke du har en spesiell grunn til å endre det.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="obsolete">Komprimeringsnivåer: Minimum (25%), Lavt (50%), Medium (75%), Høyt (85%), Maksimum (95%)</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation type="obsolete">Nedgrader dine bitmap bilder til den valgte DPI.
Hvis du ikke velger dette vil de bruke sin egen oppløsning.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="obsolete">DPI (Prikker per Tomme) for bilde eksportering.</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="obsolete">Inkluder skrifttyper i PDFen. Dette vil bety at layouten 
og utseendet av ditt dokument vil bli beholdt.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation type="obsolete">Aktiverer presentasjonseffekter når du bruker Acrobat Reader i fullskjerm modus.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="obsolete">Vis sideforhåndsvisning av alle de ovenfor valgte sider.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="obsolete">Tidsrom siden er vist før presentasjonen starter på den valgte siden.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="obsolete">Tidsrom effekten varer.
Et mindre tidsrom betyr at effekten vil gå fortere, et lengre vil gjøre at den går langsommere</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="obsolete">Type av visningseffekt.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="obsolete">Retning på bevegende linjer for split og blining effektene.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="obsolete">Startposisjonen av boks og splitt effektene.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="obsolete">Retningen på glitter eller wipe effektene.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation type="obsolete">Bruk effekt på alle sider.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="obsolete">Aktiver sikkerhetsfunksjoner i din eksporterte PDF.
Hvis du valgte Acrobat 4.0, vil PDFen bli beskyttet av 40 bit kryptering.
Hvis du valgte Acrobat 5.0, vil PDFen bli beskyttet av 128 bit kryptering.
Disclaimer: PDF kryptering er ikke like sikker som GPG eller PGP kryptering og har en del begrensninger.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="obsolete">Velg et hovedpassord som aktiverer og deaktiverer alle
sikkerhetsfunksjonen i din eksporterte PDF</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="obsolete">Velg et passord så brukere kan lese din PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="obsolete">Tillat utskriving av PDFen.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="obsolete">Tillat forandring av PDfen.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="obsolete">Tillat kopiering av tekst eller grafikk fra PDFen.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="obsolete">Tillat tilføyning av merknader og felter til PDFen.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="obsolete">Fargemodell for utgivelse av din PDF.
Velg Skjerm/Web for PDFer som skal vises på skjerm eller for å skrive på vanlige inkjets.
Velg Skriver når du skal skrive på en riktig 4 farge CMYK skriver.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation type="obsolete">Legg inn en fargeprofil for mettede farger</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="obsolete">Fargeprofil for mettede farger</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation type="obsolete">Rendering intent for mettede farger</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation type="obsolete">Legg inn en farge profil for bilder</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="obsolete">Ikke bruk farge profiler som er lagt inn i bildene</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="obsolete">Farge profil for bilder</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="obsolete">Rendering intent for bilder</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="obsolete">Utgivelse profil for utskrift. hvis mulig, få noe rådgiving fra din skriver når du velger.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="obsolete">Nødvendig tekstbit for PDF/X-3. Ellers vil PDFen ikke
følge PDF/X3-conformance. Vi foreslår at du bruker dokumentets tittel.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="obsolete">Avstand for bleed fra toppen av den fysiske siden</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="obsolete">Avstand for bleed fra bunnen av den fysiske siden</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="obsolete">Avstand for bleed fra venstresiden av den fysiske siden</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="obsolete">Avstand for bleed fra høyresiden av den fysiske siden</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="obsolete">Lagre som</translation>
    </message>
    <message>
        <source>Create PDF File</source>
        <translation type="obsolete">Lag PDF-fil</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation type="obsolete">&amp;Send til fil:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="obsolete">E&amp;ndre ...</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="obsolete">&amp;Alle sider</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="obsolete">&amp;Velg sider</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="obsolete">Kompatibil&amp;itet:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="obsolete">Inn&amp;binding:</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="obsolete">Lag &amp;Thumbnailer</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="obsolete">Lagre &amp;lenkede tekstrammer som PDF artikler</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="obsolete">&amp;Inkluder bokmerker</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="obsolete">&amp;Oppløsning:</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="obsolete">&amp;Metode:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="obsolete">&amp;Kvalitet:</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation type="obsolete">&amp;Forminsk bildene til:</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="obsolete">&amp;Inkluder alle skrifttyper</translation>
    </message>
    <message>
        <source>&amp;&gt;&gt;</source>
        <translation type="obsolete">&amp;&gt;&gt;</translation>
    </message>
    <message>
        <source>&amp;&lt;&lt;</source>
        <translation type="obsolete">&amp;&lt;&lt;</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation type="obsolete">&amp;Vis forhåndsvisning av sidene</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="obsolete">&amp;Vis varighet:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="obsolete">&amp;Varighet på effekten:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="obsolete">Effektt&amp;ype:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="obsolete">Beve&amp;gelige linjer:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="obsolete">F&amp;ra:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="obsolete">Retn&amp;ing:</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="obsolete">&amp;Bruk effekten på alle sidene</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="obsolete">&amp;Bruk kryptering</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="obsolete">&amp;Bruker:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="obsolete">&amp;Eier:</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="obsolete">&amp;Godta utskrift av dokumentet</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="obsolete">&amp;Godta endringer i dokumentet</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="obsolete">&amp;La andre kopiere tekst og bilder</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="obsolete">La andre legge til nye &amp;kommentarer og felt</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="obsolete">S&amp;ikkerhet</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="obsolete">Utskrift Be&amp;regnet for:</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="obsolete">&amp;Bruk eget framvisningsoppsett</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="obsolete">Framvisningsoppsett</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="obsolete">Hy&amp;ppighet:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="obsolete">&amp;Vinkel:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="obsolete">S&amp;pot funksjon:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="obsolete">Enkel prikk</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Linje</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="obsolete">Rund</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="obsolete">Ellipse</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="obsolete">Bruk ICC profil</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="obsolete">F&amp;arge</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="obsolete">&amp;Informasjons streng:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="obsolete">Output &amp;Profil:</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="obsolete">PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Lagre</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Skriv inn en kommaseparert liste med tegn hvor
tegnet * representerer alle sider, og 1-5 
representerer en rekke sider, eller en enkelt side. </translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="obsolete">PDF Filer (*.pdf);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="obsolete">Dette er en avansert innstilling som ikke er tilgjengelig som standard. Du bør bare bruke dette hvis skriveren ber spesielt om det og du vet nøyaktig hvilke innstillinger du trenger. Hvis du gjør det feil kan det hende pdf-fila ikke kan skrives ut på rett vis, og er absolutt ikke overførbart mellom ulike plattformer.</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation type="obsolete">Komprimer tekst og &amp;vektorgrafikk</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation type="obsolete">Br&amp;uk presentasjons effekter</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation type="obsolete">&amp;Presentasjon</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rotasjon:</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation type="obsolete">Speilvend siden(e) horisontalt</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="obsolete">Speilvend siden(e) vertikalt</translation>
    </message>
    <message>
        <source>Save as PDF</source>
        <translation type="obsolete">Lagre som PDF</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation type="obsolete">Lag en fil for &amp;hver side</translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Forhåndsvisning av utskrift</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="obsolete">Gir en penere visning av tekst i viseren, men forhåndsvisning blir
litt langsommere. Har kun effekt på Type 1 skrifttyper</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Viser gjennomsiktighet og gjennomsiktige objekter i dokumentet ditt. Trenger Ghostscript 7.07 eller senere</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Gir en utskrifsforhåndsvisning ved bruk av simulasjoner for generelle CMYK blekk, istedenfor RGB farger</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Aktiver/deaktiver C (Cyan) blekkplate</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Aktiver/deaktiver M (Magenta) blekkplate</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Aktiver/deaktiver Y (Yellow) blekkplate</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Aktiver/deaktiver K (Black) blekkplate</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>Anti-alias &amp;tekst</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Anti-alias &amp;grafikk</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Vis gjennoms&amp;iktighet</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;Vis CMYK</translation>
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
        <translation>&amp;k</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>&amp;Under fargefjernelse</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">En måte å skru av noen av de gråskygger som er laget 
av cyan, gult og magenta rød ved å  bruke svart istedenfor. 
UCR virker best på deler av bilder som er nøytrale og/eller mørke toner 
som ligger nært opptil grått. Bruk dette for å få utsftening enkelteoen bilder til å se bedre; 
og noe eksperimentering og testing er nødvendig neste gang.
UCR reduserer muligheten for overmettning med CMY farger.</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>Abort</source>
        <translation type="obsolete">Avbryt</translation>
    </message>
    <message>
        <source>Ignore</source>
        <translation type="obsolete">Ignoreer</translation>
    </message>
    <message>
        <source>Separation Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cyan</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Gult</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Svart</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Skriv ut ...</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Lukk</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">Fil</translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Page</name>
    <message>
        <source>Copy Here</source>
        <translation type="obsolete">Kopier hit</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation type="obsolete">Flytt hit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Avbryt</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation type="obsolete">Bilde</translation>
    </message>
    <message>
        <source>File: </source>
        <translation type="obsolete">Fil: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation type="obsolete">Lenket tekst</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation type="obsolete">Tekstramme</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="obsolete">Legg tekst til sti</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="obsolete">Avsnitt: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="obsolete">Ord: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="obsolete">Tegn: </translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="obsolete">Skriv ut: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="obsolete">I bruk</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="obsolete">Ikke i bruk</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="obsolete">Rediger tekst ...</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation type="obsolete">Programmet</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">mangler!</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">Kopi av</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">&amp;Lim inn</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Vis &amp;marger</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Vis &amp;rammer</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">Vis b&amp;ilder</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Vis &amp;gitter</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Vis h&amp;jelpelinjer</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">Vis &amp;Baseline gitter</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="obsolete">F&amp;est til gitter</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="obsolete">Fest til hjel&amp;pelinjer</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="obsolete">Original PPI: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="obsolete">Faktisk PPI: </translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="obsolete">In&amp;fo</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation type="obsolete">&amp;Hent bilde ...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="obsolete">V&amp;is bilde</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation type="obsolete">&amp;Oppdater bilde</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation type="obsolete">&amp;Rediger bilde</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation type="obsolete">Juster r&amp;amme til bilde</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">&amp;Hent tekst ...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation type="obsolete">&amp;Legg til tekst ...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation type="obsolete">R&amp;ediger tekst ...</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation type="obsolete">Sett &amp;inn eksempeltekst</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="obsolete">Er et PDF &amp;bokmerke</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="obsolete">Er PDF komme&amp;ntar</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="obsolete">Kommenta&amp;r egenskaper</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="obsolete">Felt egenskape&amp;r</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="obsolete">&amp;PDF opsjoner</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">&amp;Lås</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">&amp;Lås opp</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation type="obsolete">Lås objekt&amp;størrelse</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation type="obsolete">Lås opp objekt&amp;størrelse</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="obsolete">Send til k&amp;laddebok</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">Send til &amp;lag</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">&amp;Grupèr</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation type="obsolete">&amp;Oppløs gruppen</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="obsolete">Ni&amp;vå</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">Bring til &amp;bakgrunnen</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Bring til &amp;forgrunnen</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">&amp;Senk</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">He&amp;v</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation type="obsolete">&amp;Bilderamme</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation type="obsolete">Pol&amp;ygon</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="obsolete">&amp;Oversikt</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="obsolete">&amp;Tekstramme</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="obsolete">&amp;Bezierkurve</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="obsolete">Konve&amp;rter til</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">K&amp;lipp</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopier</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Slett</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="obsolete">F&amp;jern innhold</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation type="obsolete">Vis e&amp;genskaper ....</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation type="obsolete">Skjul e&amp;genskaper ...</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Ønsker du virkelig å fjerne all din Tekst?</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Bilde</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linje</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polygon</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polylinje</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Tekststi</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Kopi av</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Navn</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Type</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="obsolete">Verdi</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation type="obsolete">Parameter</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Legg til</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopier</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Delete</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">T&amp;øm</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+L</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation type="unfinished">Ingen</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <source>Page Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Verdi</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>K&amp;opier</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>T&amp;øm</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>Page Layout</source>
        <translation>Side utseende</translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation>Første side er:</translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation type="unfinished">Høyre mot høyre</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Arranger sider</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Dokument sider:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation type="obsolete">Side </translation>
    </message>
    <message>
        <source> of %1</source>
        <translation type="obsolete"> av %1</translation>
    </message>
    <message>
        <source>%1 of %2</source>
        <translation>%1 av %2</translation>
    </message>
    <message>
        <source>%1 of %1</source>
        <translation>%1 av %1</translation>
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
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Result</source>
        <translation>Resultat</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Søk i Resultat etter:</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Forhåndsvis</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Velg</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Pictures</source>
        <translation type="obsolete">Bilder</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Sti</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Skriv ut</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Tilstand</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Gå til</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Mangler</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Søk</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation></translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Håndter bilder</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation>Scribus - bildesøk</translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation>Søket feilet: %1</translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>Ingen bilder med navn &quot;%1&quot; ble funnet.</translation>
    </message>
    <message>
        <source>Select a base directory for search</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Lagre som b&amp;ilde ...</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Eksporter som bilde</translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation>Kan ikke laste plugin</translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>ukjent feil</translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation>Kan ikke finne symbol (%1)</translation>
    </message>
    <message>
        <source>Import</source>
        <comment>plugin manager</comment>
        <translation type="obsolete">Importer</translation>
    </message>
    <message>
        <source>Standard</source>
        <comment>plugin manager</comment>
        <translation type="obsolete">Standard</translation>
    </message>
    <message>
        <source>Unknown</source>
        <comment>plugin manager</comment>
        <translation type="obsolete">Ukjent</translation>
    </message>
    <message>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation>Plugin: laster %1</translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation>Start feilet</translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>Ukjent plugin type</translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>Plugin: %1 lastet</translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation>Plugin: %1 mislyktes å starte: %2</translation>
    </message>
    <message>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation type="obsolete">SVG bilder (*.svg *.svgz);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg);;</source>
        <translation type="obsolete">SVG bilder (*.svg);;</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;</source>
        <translation type="obsolete">OpenOffice.org Draw (*.sxd);;</translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <source>Plugin Manager</source>
        <translation>Pluginbehandler</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation>Plugin</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation>Hvordan skal den kjøres</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation>Start den?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation>Plugin ID</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nei</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation>Du må restarte programet for at endringene skal tre i kraft.</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Polygon egenskaper</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete"> %</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">Hjørn&amp;er:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rotasjon:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">&amp;Faktor:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Antall hjørner for polygoner</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Antall grader rotasjon for polygoner</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Eksempel polygon</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Bruk &amp;faktor</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Bruk konveks/konkav faktor for å forandre formen på polygoner</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">En negativ verdi vil gjøre polygonet konkavt (stjerneformet), mens en 
positiv verdi vil gjøre det konveks</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Hjørn&amp;er:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotasjon:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Bruk &amp;faktor</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>&amp;Faktor:</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Eksempel polygon</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">En negativ verdi vil gjøre polygonet konkavt (stjerneformet), mens en 
positiv verdi vil gjøre det konveks</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Innstillinger</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Generelt</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Guide</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Typografier</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Verktøy</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="obsolete">Kladdebok</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Vis</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Eksterne verktøy</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation type="obsolete">Diverse</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>GUI</translation>
    </message>
    <message>
        <source>Units</source>
        <translation type="obsolete">Enheter</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation type="obsolete">Punkter (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Millimeter (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Tommer (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Picas (p)</translation>
    </message>
    <message>
        <source> px</source>
        <translation type="obsolete"> px</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation type="obsolete">Menyer</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Stier</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Brukerdefinert</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Stående</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Liggende</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Hjelpemarger</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Autolagre</translation>
    </message>
    <message>
        <source>min</source>
        <translation>minimum</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation type="obsolete">Gitter layout</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation type="obsolete">Farge på gitter</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation type="obsolete">Plassering</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation type="obsolete">Baseline gitter</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Senket skrift</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete"> %</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Hevet skrift</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Små bokstaver</translation>
    </message>
    <message>
        <source>Other</source>
        <translation type="obsolete">Andre</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">Woven silk pyjamas exchanged for blue quartz</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation type="obsolete">Andre innstillinger</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Forhåndsvis</translation>
    </message>
    <message>
        <source>Small</source>
        <translation type="obsolete">Liten</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Medium</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation type="obsolete">For å justere visningen trekk linjalen nedenfor med glideren.</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation type="obsolete">Postscript behandler</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Bildebehandlingsverktøy</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation type="obsolete">Skriver ut</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation type="obsolete">Velg standard vindudekorasjon.
Scribus kan bruke alle KDE og Qt temaer</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Standard skriftstørrelse for menyer og vinduer</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Standard enhet for måling ved dokument redigering</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Antall linjer Scribus vil scrolle for hver bevegelse med musehjulet</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="obsolete">Radius for område der Scribus vil la deg fange handles for et objekt</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Antall nylig brukte filer som vises i filmenyen</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Standard dokument katalog</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation type="obsolete">Standard ICC profil katalog</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Standard Scripter skript katalog</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Standard dokument sidestørrelse, enten en standard størrelse eller en brukerdefinert størrelse</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Standard retning for dokumentets sider</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Bredde av dokumentets sider, kun redigerbar hvis du har valgt en brukerdefinert sidestørrelse</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Høyde av dokumentets sider, kun redigerbar hvis du har valgt en brukerdefinert sidestørrelse</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Aktiver singel eller spredt basert layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation type="obsolete">Gjør første side til venstre side av dokumentet</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Avstanden mellom topp hjelpemargen og sidens kant</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Avstand mellom bunn hjelpemargen og sidens kant</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Avstand mellom venstre hjelpemarg og sidens kant.
Hvis Motstående Sider er markert kan denne brukes til å få til den korrekte marg for binding</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Avstand mellom høyre hjelpemarg og sidens kant.
Hvis Motstående Sider er markert kan denne brukes til å få til den korrekte marg for binding</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation type="obsolete">Hvis slått på, så lager Scribus en kopi av din fil med navnet *.bak hver
gang tidsperioden utløper</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Tidsperiode mellom automatisk lagring</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation type="obsolete">Avstand mellom gitter linjer</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation type="obsolete">Avstand mellom hoved-gitterlinjer</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="obsolete">Maksimum avstand for når et objekt vil legge seg på plasseringsguidene</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation type="obsolete">Farge for gitterlinjer</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation type="obsolete">Farge for hoved-gitterlinjer</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation type="obsolete">Farge på hjelpelinjer du innsetter</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation type="obsolete">Legg gitter bak dine sideobjekter</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation type="obsolete">Legg gitter foran dine sideobjekter</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation type="obsolete">Vis Baseline gitter</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation type="obsolete">Skjul Baseline gitter</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation type="obsolete">Forskyvning over bunnlinjen for en skrifttype på en linje</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="obsolete">Relativ størrelse av hevet skrift i forhold til normal skrifttype</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="obsolete">Forskyvning under bunnlinjen for en skrifttype på en linje</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="obsolete">Relativ størrelse av senket skrift i forhold til normal skrifttype</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="obsolete">Relativ størrelse av små bokstaver i forhold til normal skrifttype</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="obsolete">Relativ størrelse i prosent av linjeavstand i forhold til normal skrifttype</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation type="obsolete">Tekstfelt egenskaper</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="obsolete">Billedramme egenskaper</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation type="obsolete">Form tegningegenskaper</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation type="obsolete">Standard forstørrelsesnivå</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation type="obsolete">Linje tegningegenskaper</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation type="obsolete">Polygon tegneegenskaper</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation type="obsolete">Skrifttype som skal brukes i nye tekstrammer</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation type="obsolete">Størrelse for skrifttype for nye tekstrammer</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation type="obsolete">Skriftfarge</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="obsolete">Antall kolonner i en tekstramme</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="obsolete">Mellomrom mellom kolonner i tekstramme</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation type="obsolete">Eksempel på skrifttypen din</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="obsolete">Bilderamme tillater bilder å skalere til enhver størrelse</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation type="obsolete">Horisontal skalering av bilder</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation type="obsolete">Vertikal skalering av bilder</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="obsolete">Samme horisontal og vertikal skalering </translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="obsolete">Bilder i bilderammer er skalert opp til rammens størrelse</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="obsolete">Automatisk skalerte bilder beholder deres originale proporsjoner</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="obsolete">Fyll farge på bilderammer</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation type="obsolete">Fyllfargens styrke </translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation type="obsolete">Formers linjefarge </translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation type="obsolete">Linjefarges styrke</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation type="obsolete">Formers fyllfarge</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation type="obsolete">Formenes linjestil</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation type="obsolete">Formenes linjebredde</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation type="obsolete">Minimum tillat forstørrelse</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation type="obsolete">Maksimum tillat forstørrelse</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation type="obsolete">Forandring i forstørrelse for hvert enkelt zoom</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation type="obsolete">Linjefarge</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="obsolete">Fargestyrke</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation type="obsolete">Linjestil</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation type="obsolete">Linjetykkelse</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Antall hjørner for polygoner </translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Antall grader for rotasjon av polygoner</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Eksempel polygon</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation type="obsolete">Velg størrelse på forhåndsvisning i kladdebok paletten</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation type="obsolete">Lagre kladdebokinnhold etter hver forandring</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation type="obsolete">Ved bruk av motstående sider, vis sidene ved siden av hverandre</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Papirfarge</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="obsolete">Marginlinjefarge</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Legg området utenfor margene i margenfargen</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation type="obsolete">Slå på gjennomsiktighetsfunksjoner innenfor PDF 1.4 eksportering</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Sett standard zoom nivå</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation>Ghostscript behandlerens posisjon i filsystemet</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Antialias tekst for EPS og PDF skjermrendering</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Antialias bilder for EPS og PDF skjermrendering</translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation type="obsolete">Bildebehandlerens posisjon i filsystemet</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation type="obsolete">Ikke vis objekter utenfor margene på utskrift eller eksportert fil</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Velg en katalog</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Tema:</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="obsolete">&amp;Skriftstørrelse:</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation type="obsolete">Mus innstillinger</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Hjulhopp:</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation type="obsolete">&amp;Hent radius:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>&amp;Nylig åpnede dokumenter:</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Dokumenter:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Forandre ...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC profiler:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Forandre ...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Skript:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>Fo&amp;randre ...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Ret&amp;ning:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Høyde:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Motstående sider</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Venstre &amp;side først</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>B&amp;unn:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Topp:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Høyre:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Venstre:</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation type="obsolete">&amp;Aktivert</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Intervall:</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation type="obsolete">M&amp;indre gitter avstand:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation type="obsolete">St&amp;ørre gitter avstand:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation type="obsolete">Gitter &amp;snap avstand:</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation type="obsolete">M&amp;indre gitterfarge:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation type="obsolete">St&amp;ørre gitterfarge:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation type="obsolete">Br&amp;uk gitterfarge:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation type="obsolete">Bruk Baes&amp;line gitterfarge:</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation type="obsolete">I &amp;bakgrunnen</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation type="obsolete">I &amp;forgrunnen</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation type="obsolete">&amp;På</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation type="obsolete">&amp;Av</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="obsolete">&amp;Forskyvning:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="obsolete">&amp;Skalering:</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="obsolete">F&amp;orskyvning:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="obsolete">S&amp;kalering:</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="obsolete">Sk&amp;alering:</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="obsolete">Grunnlinje &amp;Nett:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation type="obsolete">Automatisk linjeav&amp;stand:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation type="obsolete">Standard &amp;skrifttype:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation type="obsolete">Standard &amp;størrelse:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation type="obsolete">&amp;Tekstfarge:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="obsolete">Kolo&amp;nner:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="obsolete">&amp;Mellomrom:</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="obsolete">&amp;Linjefarge:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="obsolete">&amp;Skygge:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="obsolete">&amp;Fyllfarge:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="obsolete">&amp;Skygge:</translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation type="obsolete">Li&amp;njetype:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="obsolete">Linje&amp;bredde:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation type="obsolete">Mi&amp;nimum:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation type="obsolete">Ma&amp;ksimum:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation type="obsolete">&amp;Skritt:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="obsolete">&amp;Fri skalering</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="obsolete">&amp;Horisontal skalering:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="obsolete">&amp;Vertikal skalering:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="obsolete">&amp;Skaler bilde til rammestørrelse</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="obsolete">Behold høyde/bredde fo&amp;rhold</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">&amp;Fyllfarge:</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">Hjørn&amp;er:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rotasjon:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">&amp;Faktor:</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation type="obsolete">La&amp;gre innhold når noe forandres</translation>
    </message>
    <message>
        <source>Large</source>
        <translation type="obsolete">Stor</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation type="obsolete">Vis &amp;sider ved siden av hverandre</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation type="obsolete">Sidefarger</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation type="obsolete">&amp;Bakgrunn:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation type="obsolete">&amp;Marger:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Vis &amp;ikke utskrivbare områder i samme farge som margene</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation type="obsolete">Bruk funksjonen for gjennomsiktighet som finnes i pdf 1.4</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>&amp;Juster visningsstørrelsen</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Navn på kjørbar fil:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Anti-alias &amp;tekst</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Anti-alias &amp;grafikk</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>&amp;Navn på kjørbar fil:</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation type="obsolete">&amp;Fest til sidemargene</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Innside:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="obsolete">&amp;Utside:</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation type="obsolete">Bruk under farge fje&amp;rnelse</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation type="obsolete">M&amp;aler:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>E&amp;ndre ...</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="obsolete">Grunnlinje &amp;forskyvning:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Bruk &amp;faktor</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation type="obsolete">Flere kataloger for dokumentmaler</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Bruk konveks/konkav faktoren for å endre formen på polygonet</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">En negativ verdi vil gjøre polygonet konkavt (stjerneformet), mens en 
positiv verdi vil gjøre det konveks</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">En måte å skru av noen av de gråskygger som er laget 
av cyan, gult og magenta rød ved å  bruke svart istedenfor. 
UCR virker best på deler av bilder som er nøytrale og/eller mørke toner 
som ligger nært opptil grått. Bruk dette for å få utsftening enkelteoen bilder til å se bedre; 
og noe eksperimentering og testing er nødvendig neste gang.
UCR reduserer muligheten for overmettning med CMY farger.</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="obsolete">Executive</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation type="obsolete">Folio</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Letter</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Tabloid</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Språk:</translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Units:</source>
        <translation>Enhetr:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Skrifttyper</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF Eksport</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Tastatur snarveier</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farge:</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="unfinished">Vis bilder</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="unfinished">Vis tekstlenker</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="unfinished">Vis rammer</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Oppløsning:</translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation>Diverse</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Type</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="obsolete">Fil</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Nei</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation>Plugins</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Linjaler relativ i forhold til siden</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>Horisontal:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>Vertikal:</translation>
    </message>
    <message>
        <source>Locate Ghostscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Defaults</source>
        <translation>&amp;Standard</translation>
    </message>
    <message>
        <source>Save...</source>
        <translation>Lagre ...</translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation>Lagre innstillinger</translation>
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
        <translation>Overfør instillinger fra tidligere Scribus versjoner?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation type="obsolete">Enkel side</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Venstre side</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Høyre side</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation type="obsolete">I midten</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation type="obsolete">I midten til venstre</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation type="obsolete">Høyre mot høyre</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error Writing Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Magazine</source>
        <translation>Magasine</translation>
    </message>
    <message>
        <source>Guttenberg</source>
        <translation type="obsolete">Guttenberg</translation>
    </message>
    <message>
        <source>Fibonacci</source>
        <translation>Fibonacci</translation>
    </message>
    <message>
        <source>Golden Mean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Nine Parts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Gutenberg</source>
        <translation>Gutenberg</translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None</source>
        <comment>layout type</comment>
        <translation type="unfinished">Ingen</translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Åpne ...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Lagre</translation>
    </message>
    <message>
        <source>&amp;Save As...</source>
        <translation type="obsolete">La&amp;gre som ...</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation>&amp;Avslutt</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation>&amp;Kjør</translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>S&amp;kript</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation>Skript konsoll</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation>Python script (*.py)</translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation>Tekstfiler (*.txt)</translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Lagre &amp;som ...</translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>Hu&amp;e:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Sat:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>&amp;Val:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Rød:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Grønn:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>Bl&amp;å:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>A&amp;lfa kanal:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>&amp;Basis farge</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>B&amp;rukerdefinerte farger</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Definer egne farger &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Legg til egne farger</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Velg farge</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Kopier eller flytt en fil</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Les: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Skriv: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>Fil&amp;navn:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>Fil&amp;type:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>En katalog opp</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Størrelse</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Dato</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Attributter</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Søk &amp;i:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Tilbake</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Lag ny katalog</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Listevisning</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Detaljert visning</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Forhåndsvis info i fil</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Forhåndsvis innhold i fil</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Lese og skrive</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Skrivebeskyttet</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Bare skrive</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Ikke tilgjengelig</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Symlink til fil</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Symlink til katalog</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Symlink til spesiell</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Katalog</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Spesiell</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Lagre som</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Åpne</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Lagre</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Endre navn</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;lett</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>&amp;Oppdater</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Sorter etter &amp;navn</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Sorter etter &amp;størrelse</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Sorter etter &amp;dato</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Ikke sortert</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Sorter</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Vis s&amp;kjulte filer</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>filen</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>katalogen</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>symlinken</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Slett %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Er du sikker på at du vil slette %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nei</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Ny katalog 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Ny katalog</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Ny katalog %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Finn katalogen</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Kataloger</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Lagre</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Feil</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Filen ble ikke funnet.
Sjekk sti og filnavn.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Alle filtyper (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Velg en katalog</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Katalog:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Skrifttype</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>Skriftst&amp;il</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Størrelse</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effekter</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>Gjennomstre&amp;k</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Understrek</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Farge</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Prøve</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>Skr&amp;ipt</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Bruk</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Lukk</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Velg skrifttype</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Tøm</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Marker alt</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Angre</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Pånytt</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation> &amp;Klipp ut</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>K&amp;opier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Lim inn</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Still opp</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Brukerdefinert ...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Om Qt&lt;/h3&gt;&lt;p&gt;Dette programet bruker Qt versjon %1.&lt;/p&gt;&lt;p&gt;Qt er et C++ toolkit for multiplattform GUI &amp;amp; program utvikling&lt;/p&gt;&lt;p&gt;Qt gir single-source portabilitet mellom MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, og alle større kommersielle Unix varianter.&lt;br&gt;Qt er også tilgjengelig for embedded deviced.&lt;/p&gt;&lt;p&gt;Qt er et Trolltech produkt. Se &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for mere informasjon.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Initializing...</source>
        <translation>Starter opp ...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="obsolete">Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Bakgrunn</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Ønsker du virkelig å overskrive filen:
%1 ?</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">Forhåndsvisning av &amp;skrifttyper</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">Sett &amp;inn spesielt</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation type="obsolete">Ny &amp;fra mal ...</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Nyhetsbrev</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Brosjyrer</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Kataloger</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Løpesedler</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Tegn</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Kort</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Brevhoder</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Konvolutter</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Visittkort</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Kalendere</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Annonser</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Merkelapper</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menyer</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programmer</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>PDF skjemaer</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF presentasjoner</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Magasiner</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Plakater</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Beskjeder</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Tekst dokumenter</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Folder</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Egne maler</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation type="obsolete">Lagre som b&amp;ilde ...</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Lagre som bilde</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation type="obsolete">Feil ved skriving av output filen(e).</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Feil ved skriving av output filen(e).</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Eksportering gjennomført uten problemer.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Filen eksisterer. Erstatt?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>finnes allerede. Erstatt?</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nei</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Ja til alle</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation type="obsolete">Forhåndsvisning av &amp;utskrift</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation type="obsolete">Importer &amp;EPS/PS ...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Alle støttede formater (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation type="obsolete">Lagre som &amp;mal ...</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation type="obsolete">S&amp;kripting håndbok ...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scribus Skripts</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Utfør Script ...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>Nylig b&amp;rukte skript</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Vis &amp;konsoll</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation type="obsolete">S&amp;kript</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation type="obsolete">Online referanse</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Python skript (*.py);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="obsolete">Lagre som &amp;SVG ...</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Lagre som</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG-Bilder (*.svg ·svgz);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG-Bilder (*.svg);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation type="obsolete">Importer &amp;SVG ...</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation type="obsolete">Forhåndsvisning av utskrift</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Importerer tekst</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Alle støttede formater</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>HTML filer</translation>
    </message>
    <message>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Tekst filer</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>Skrifttype %1 er ødelagt, forkaster den</translation>
    </message>
    <message>
        <source>Template: </source>
        <translation type="obsolete">Mal: </translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Komma separert fil</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>CSV_data</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>CSV_header</translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation>
Eksterne lenker</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Tekstfiltre</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Albansk</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Baskisk</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Bulgarsk</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation>Brasilsk</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Katalansk</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Kinesisk</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tsjekkisk</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Dansk</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Nederlandsk</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Engelsk</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Engelsk (Britisk)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Tysk</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finsk</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Fransk</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Gresk</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Ungarsk</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Indonesisk</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italiensk</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Koreansk</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Litauisk</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>Norsk (Bokmål)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Norsk (Nynorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Norsk</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polsk</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Russisk</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Svensk</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Spansk</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Spansk (Latin)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slovakisk</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Slovensk</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Serbisk</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>&amp;Om skript ...</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>Om skript</translation>
    </message>
    <message>
        <source>Failed to open document</source>
        <comment>python error</comment>
        <translation type="obsolete">Klarte ikke å åpne dokument</translation>
    </message>
    <message>
        <source>Failed to save document</source>
        <comment>python error</comment>
        <translation type="obsolete">Klarte ikke å lagre dokumentet</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Fant ikke skrifttypen</translation>
    </message>
    <message>
        <source>Can&apos;t have an empty layer name</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan ikke ha et lag uten navn</translation>
    </message>
    <message>
        <source>Layer not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Fant ikke laget</translation>
    </message>
    <message>
        <source>Can&apos;t remove the last layer</source>
        <comment>python error</comment>
        <translation type="obsolete">Klarte ikke å fjerne det siste laget</translation>
    </message>
    <message>
        <source>Can&apos;t create layer without a name</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan ikke lage et lag uten navn</translation>
    </message>
    <message>
        <source>Object not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Objektet ble ikke funnet</translation>
    </message>
    <message>
        <source>Style not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Stilen ble ikke funnet</translation>
    </message>
    <message>
        <source>Failed to save EPS</source>
        <comment>python error</comment>
        <translation type="obsolete">Klarte ikke å lagre til EPS</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Hjørneradius må være et positivt tall.</translation>
    </message>
    <message>
        <source>Line style not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Fant ikke linjestilen</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke sette inn tekst i en ikke-tekstramme</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set font size on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan ikke sette størrelsen på skrifttypen på en ikke-tekstramme</translation>
    </message>
    <message>
        <source>Can&apos;t set font on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan ikke sette skrifttypenavnet på en ikke-tekstramme</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1</source>
        <comment>python error</comment>
        <translation type="obsolete">Spaltenummerering ute av system, må være &gt; 1</translation>
    </message>
    <message>
        <source>Can&apos;t number of columns on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan kun nummerer spalter i tekstrammer</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t select text in a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan ikke velge tekst i en ikke-tekstramme</translation>
    </message>
    <message>
        <source>Can&apos;t delete text from a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan ikke slette tekst fra en ikke-tekstramme</translation>
    </message>
    <message>
        <source>Can&apos;t set text fill on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan ikke bruke tekstfyll på en ikke-tekstramme</translation>
    </message>
    <message>
        <source>Can&apos;t set text stroke on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan ikke bruke tekststrek på en ikke-tekstramme</translation>
    </message>
    <message>
        <source>Can&apos;t set text shade on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan ikke bruke tekstskygge på en ikke-tekstramme</translation>
    </message>
    <message>
        <source>Can only link text frames</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan kun blinke sammen tekstrammer</translation>
    </message>
    <message>
        <source>Target frame must be empty</source>
        <comment>python error</comment>
        <translation type="obsolete">Målrammen må være tom</translation>
    </message>
    <message>
        <source>Target frame links to another frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Målrammen er allerede lenket sammen med en annen ramme</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Målrammen er allerede lenket sammen med en annen ramme</translation>
    </message>
    <message>
        <source>Source and target are the same object</source>
        <comment>python error</comment>
        <translation type="obsolete">Kilden og målet er det samme objektet</translation>
    </message>
    <message>
        <source>Can&apos;t unlink a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan ikke fjerne lenken til en ikke-tekstramme</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>Objektet er ikke en lenket tekstramme, kan derfor ikke fjerne noen lenke.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>Objektet er det siste i en serie, kan ikke fjerne lenken. Fjern lenken til den forrige rammen istedenfor.</translation>
    </message>
    <message>
        <source>Can&apos;t convert a non-text frame to outlines</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan ikke konvertere en ikke-tekst ramme til outlines</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke hente en farge uten navn.</translation>
    </message>
    <message>
        <source>Color not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Fargen ble ikke funnet</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke forandre en farge uten navn.</translation>
    </message>
    <message>
        <source>Color not found in document</source>
        <comment>python error</comment>
        <translation type="obsolete">Fargen ble ikke funnet i filen</translation>
    </message>
    <message>
        <source>Color not found in default colors</source>
        <comment>python error</comment>
        <translation type="obsolete">Fargen ble ikke funnet blant standard fargene</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke lage en farge med tomt navn.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke slette en farge med tomt navn.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke bytte ut en farge med tomt navn.</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation type="obsolete">OpenOffice.org Draw (*.sxd);;Alle filer (*)</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>OpenOffice.org Writer dokumenter</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikaans</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>Tyrkisk</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>Ukrainsk</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scripter &amp;Settings</source>
        <comment>script menu</comment>
        <translation type="obsolete">Skripter &amp;innstillinger</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Kroatisk</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugisisk</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Portugisisk (BR)</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Scribus krasj</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus krasjer pga Signal #%1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Greit</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Brukerdefinert</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Verification of settings failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not open input file %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unable to read settings XML:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation>%1 (linje %2 kolonne %3)</translation>
    </message>
    <message>
        <source>Unable to read settings XML: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>unexpected null &lt;%2&gt; node</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>node &lt;%1&gt; not an element</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>element &lt;%1&gt; lacks `value&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>element &lt;%1&gt; value must be `true&apos; or `false&apos;</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Freetype2 library not available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 is broken, no embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation>Skrifttype %1 er ødelagt (FreeType2), forkaster den</translation>
    </message>
    <message>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 is broken and will be discarded</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 cannot be read, no embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to load font %1 - font type unknown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 loaded from %2(%3)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 (found using fontconfig) is broken, discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType), discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>memcpy header: %1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>table &apos;%1&apos;</source>
        <translation>tabell &apos;%1&apos;</translation>
    </message>
    <message>
        <source>memcpy table: %1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>memcpy offset: %1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Proceed</source>
        <translation type="obsolete">&amp;Fortsett</translation>
    </message>
    <message>
        <source>Invalid argument: </source>
        <translation type="obsolete">Ugyldig argument: </translation>
    </message>
    <message>
        <source>Options:</source>
        <translation type="obsolete">Innstillinger:</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation type="obsolete">Ikke vis oppstartsskjerm ved oppstart</translation>
    </message>
    <message>
        <source>Scribus Version </source>
        <translation type="obsolete">Scribus Versjon </translation>
    </message>
    <message>
        <source>---------------------------------------</source>
        <translation type="obsolete">---------------------------------------</translation>
    </message>
    <message>
        <source>Homepage:       http://www.scribus.net </source>
        <translation type="obsolete">Hjemmeside:       http://www.scribus.net </translation>
    </message>
    <message>
        <source>Documentation:  http://docs.scribus.net</source>
        <translation type="obsolete">Dokumentasjon:  http://docs.scribus.net</translation>
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
        <translation>cm</translation>
    </message>
    <message>
        <source> c</source>
        <translation>c</translation>
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
        <translation>Punkter (pt)</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation>Millimeter (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Tommer (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picas (p)</translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation>Centimeter (cm)</translation>
    </message>
    <message>
        <source>Cicero (c)</source>
        <translation>Cicero (c)</translation>
    </message>
    <message>
        <source>File exists</source>
        <translation>Filen finnes allerede</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Bytt ut</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation type="obsolete">&amp;Forhåndsvisning av skrifttype ...</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Types matched, but setting property failed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>Klarte ikke å lagre som EPS.</translation>
    </message>
    <message>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>S&amp;kript</translation>
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
raise a NoValidObjectError if it can&apos;t find anything to operate
on.
- Many functions will raise WrongFrameTypeError if you try to use them
on a frame type that they do not make sense with. For example, setting
the text color on a graphics frame doesn&apos;t make sense, and will result
in this exception being raised.
- Errors resulting from calls to the underlying Python API will be
passed through unaltered. As such, the list of exceptions thrown by
any function as provided here and in its docstring is incomplete.

Details of what exceptions each function may throw are provided on the
function&apos;s documentation, though as with most Python code this list
is not exhaustive due to exceptions from called functions.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation>OpenDocument Tekst Dokumenter</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation>Side </translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopi #%1 av </translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Svart</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cyan</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Gult</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>Fargehjul</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>My Plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation>Ny fra mal</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Eksporter som bilde</translation>
    </message>
    <message>
        <source>PS/EPS Importer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save As Template</source>
        <translation>Lagre som mal</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished">Short Words</translation>
    </message>
    <message>
        <source>SVG Export</source>
        <translation>SVG Eksport</translation>
    </message>
    <message>
        <source>SVG Import</source>
        <translation>SVG Import</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw Importer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Creating Font Cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Font found, checking...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Modified Font found, checking...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reading Font Cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Writing updated Font Cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Henter skrifttyper</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Old .sla format support</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>German (Trad.)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting PostScript File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Printing File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;reate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importer</translation>
    </message>
    <message>
        <source>Thai</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Barcode Generator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Word Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This file is not recognized as a PDB document propably. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Tøm</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Marker alt</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Angre</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Gjør om</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Klipp ut</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>K&amp;opier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Lim inn</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Systemmeny</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Skygge</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Fjern skygge</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Normaliser</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimer</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maksimer</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Lukk</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Gjenopprett</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Flytt</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Størrelse</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Mi&amp;nimer</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Mak&amp;simer</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>&amp;Bli på topp</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimer</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Gjenopprett Ned</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Lukk</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>Sk&amp;ygge</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>&amp;Fjern skygge</translation>
    </message>
</context>
<context>
    <name>Query</name>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>Document Setup</source>
        <translation>Dokument innstillinger</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Marg hjelpelinjer</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Aktiverer singel eller oppdelt basert layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">Gjør første side venstre side av dokumentet</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Avstand mellom toppmargen guiden og sidens kant</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Avstand mellom bunnmargen guiden og sidens kant</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Avstand mellom venstremarg guiden og sidens kant.
Hvis Motstående Sider er markert kan denne brukes til å få til den korrekte marg for binding</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Avstand mellom høyremarg guiden og sidens kant.
Hvis Motstående Sider er markert kan denne brukes til å få til den korrekte marg for binding</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Topp:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Venstre:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Bunn:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Høyre:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Motstående sider</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Venstre &amp;side først</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Innenfor:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation type="obsolete">&amp;Utenfor:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Størrelse:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Brukerdefinert</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="obsolete">Retning:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Stående</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Liggende</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="obsolete">Bredde:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="obsolete">Høyde:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="obsolete">Fø&amp;rste sidenummer:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Ret&amp;ning:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Høyde:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation>&amp;Enhet3:</translation>
    </message>
    <message>
        <source>Layout</source>
        <translation type="obsolete">Utseende</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Autolagre</translation>
    </message>
    <message>
        <source>min</source>
        <translation>minimum</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Intervall:</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation>Dokument informasjon</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Guide</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farge:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Vis &amp;ikke utskrivbare områder i samme farge som margene</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation>Vis bilder</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Vis tekstlenker</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Vis rammer</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Vis</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Typografier</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Verktøy</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation type="unfinished">Skrifttyper</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF Eksport</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Papirfarge</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Legg området utenfor margene i margenfargen</translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="obsolete">Innstillinger</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="unfinished">Linjaler relativ i forhold til siden</translation>
    </message>
    <message>
        <source>Minimum Scratch Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>Horisontal:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>Vertikal:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Tilpasser farger</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply size settings to all pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sections</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Python skript (*.py);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style of current paragraph</source>
        <translation>Stilen til gjeldende avsnitt</translation>
    </message>
    <message>
        <source>Style Settings</source>
        <translation>Stil innstillinger</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Farge på tekstfyll</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Metning av tekstfyllens farge</translation>
    </message>
    <message>
        <source>Fill Color Settings</source>
        <translation>Fyllfarge opsjoner</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Farge til tekststrek</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Metning av tekststrekens farge</translation>
    </message>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Fyllfarge opsjoner</translation>
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
        <translation>Skrifttype på den markerte teksten</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Skriftstørrelse</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Skalering av tegnbredde</translation>
    </message>
    <message>
        <source>Font Settings</source>
        <translation>Skrifttype innstillinger</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Kerning:</source>
        <translation type="obsolete">Kerning:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="obsolete">Manuell Kerning</translation>
    </message>
    <message>
        <source>Character Settings</source>
        <translation>Tegninnstilling</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Lagre som &amp;SVG ...</translation>
    </message>
    <message>
        <source>Exports SVG Files</source>
        <translation>Eksporterer SVG-filer</translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Importer &amp;SVG ...</translation>
    </message>
    <message>
        <source>Imports SVG Files</source>
        <translation>Importerer SVG-filer</translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scalable Vector Graphics</source>
        <translation>Scalable Vector Graphics</translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>&amp;Valgte rammer</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation>Aktiv &amp;side</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>&amp;Alle objekter</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Kun den valgte rammen er behandlet.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Kun den faktiske siden er behandlet.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Alle objekter i dokumentet er behandlet.</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Lagre</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished">Short Words</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation>Kan ikke åpne fil %1</translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Lagre som &amp;mal ...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import</source>
        <comment>plugin manager plugin type</comment>
        <translation type="obsolete">Importer</translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>Handling</translation>
    </message>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Ukjent</translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <source>Top</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Right</source>
        <translation type="unfinished">Høyre</translation>
    </message>
    <message>
        <source>Bottom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left</source>
        <translation type="unfinished">Venstre</translation>
    </message>
    <message>
        <source>Allow Docking To...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Floating Orientation...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <source>Printing...</source>
        <translation type="unfinished">Skriver ut ...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopi #%1 av </translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Bakgrunn</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>File</source>
        <translation type="obsolete">Fil</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation type="obsolete">Lag et nytt dokument</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation type="obsolete">Åpne dokument</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation type="obsolete">Lagre dokument</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation type="obsolete">Lukk dokument</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation type="obsolete">Skriver ut dokument</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation type="obsolete">Lagre dokument som PDF</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="obsolete">Henter skrifttyper</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation type="obsolete">Det er ingen Postscript-skrifttyper installert</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation type="obsolete">Avslutter nå</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation type="obsolete">Alvorlig feil</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation type="obsolete">Smart orddeling</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation type="obsolete">Juster mot venstre</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation type="obsolete">Juster mot høyre</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation type="obsolete">Juster sentrert</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation type="obsolete">Sett inn sidenummer</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation type="obsolete">Legg tekst på sti</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation type="obsolete">Vis Lag</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation type="obsolete">JavaSkript ...</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation type="obsolete">Angre</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation type="obsolete">Vis sidepalet</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation type="obsolete">Lås/Lås opp</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation type="obsolete">Ikke-delende mellomrom</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation type="obsolete">Henter innstillinger</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation type="obsolete">Starter orddeler</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation type="obsolete">Setter opp snarveier</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation type="obsolete">Setter opp kladdebok</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation type="obsolete">Starter tilleggsprogram</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Ny</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation type="obsolete">Åpne ...</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Lukk</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="obsolete">Lagre</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation type="obsolete">Lagre som ...</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation type="obsolete">Hent tekst/bilde ...</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation type="obsolete">Dokument info ...</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation type="obsolete">Dokument innstillinger ...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation type="obsolete">Skriv ut ...</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation type="obsolete">Avslutt</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="obsolete">Klipp</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation type="obsolete">Kopier</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="obsolete">Lim inn</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation type="obsolete">Tøm</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation type="obsolete">Marker alt</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation type="obsolete">Farger ...</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation type="obsolete">Stiler ...</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation type="obsolete">Maler ...</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation type="obsolete">Skrifttyper ...</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation type="obsolete">Velg ny skrifttype</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation type="obsolete">Dupliker</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation type="obsolete">Dupliker flere ganger</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Slett</translation>
    </message>
    <message>
        <source>Group</source>
        <translation type="obsolete">Grupèr</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation type="obsolete">Oppløs gruppen</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation type="obsolete">Lås</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation type="obsolete">Plasser bakerst</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation type="obsolete">Bring til forgrunnen</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation type="obsolete">Senk</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation type="obsolete">Hev</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation type="obsolete">Fordel/Still opp på linje ...</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation type="obsolete">Sett inn ...</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation type="obsolete">Slett ...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation type="obsolete">Flytt ...</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation type="obsolete">Bruk mal ...</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation type="obsolete">Håndter hjelpelinjer ...</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation type="obsolete">Tilpass til vinduet</translation>
    </message>
    <message>
        <source>50%</source>
        <translation type="obsolete">50%</translation>
    </message>
    <message>
        <source>75%</source>
        <translation type="obsolete">75%</translation>
    </message>
    <message>
        <source>200%</source>
        <translation type="obsolete">200%</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation type="obsolete">Thumbnails</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation type="obsolete">Skjul marg</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation type="obsolete">Skjul rammer</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation type="obsolete">Skjul bilder</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation type="obsolete">Vis gitter</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation type="obsolete">Fest til gitter</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="obsolete">Vis Baseline gitter</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="obsolete">Verktøy</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="obsolete">Egenskaper</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="obsolete">Oversikt</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="obsolete">Kladdebok</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="obsolete">Håndter bilder</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation type="obsolete">Orddel tekst</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation type="obsolete">Om Scribus</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation type="obsolete">Om Qt</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation type="obsolete">Online hjelp ...</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="obsolete">&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">&amp;Rediger</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation type="obsolete">&amp;Objekt</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation type="obsolete">S&amp;ide</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation type="obsolete">&amp;Vis</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation type="obsolete">Verk&amp;tøy</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="obsolete">E&amp;kstra</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation type="obsolete">Vi&amp;nduer</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="obsolete">&amp;Hjelp</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="obsolete">Understreket</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation type="obsolete">Gjennomstreking</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Små bokstaver</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Hevet skrift</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Senket skrift</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation type="obsolete">Utlinjet</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation type="obsolete">X-Pos:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation type="obsolete">Y-Pos:</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation type="obsolete">Klar</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation type="obsolete">Hent bilde ...</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Farge</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation type="obsolete">Inverter</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="obsolete">Hent tekst ...</translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="obsolete">Skrifttype</translation>
    </message>
    <message>
        <source>Size</source>
        <translation type="obsolete">Størrelse</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation type="obsolete">Skygge</translation>
    </message>
    <message>
        <source>Style</source>
        <translation type="obsolete">Stil</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="obsolete">Lås opp</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">Åpne</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumenter (*.sla *.sla.gz *.scd *scd.gz);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokumenter (*.sla *.scd);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation type="obsolete">Leser inn ...</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="obsolete">Alle støttede formater</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="obsolete">Tekstfiler (*.txt);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation type="obsolete">Kunne ikke skrive filen: 
%1</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Greit</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="obsolete">Lagre som</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumenter (*.sla *.sla.gz *.scd *scd.gz);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation type="obsolete">Lagrer ...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation type="obsolete">Skriver ut ...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="obsolete">Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation type="obsolete">Utskrift feilet</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation type="obsolete">Scribus håndbok</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation type="obsolete">Skjul Baseline gitter</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation type="obsolete">Disse programmer mangler:</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="obsolete">Alle</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation type="obsolete">EPS-Filer (*.eps);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation type="obsolete">Noen objekter er låst.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Avbryt</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation type="obsolete">Lås alle</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation type="obsolete">Lås opp alle</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation type="obsolete">Laster:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="obsolete">Tilpasser farger</translation>
    </message>
    <message>
        <source>English</source>
        <translation type="obsolete">Engelsk</translation>
    </message>
    <message>
        <source>German</source>
        <translation type="obsolete">Tysk</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation type="obsolete">Spansk</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation type="obsolete">Italiensk</translation>
    </message>
    <message>
        <source>French</source>
        <translation type="obsolete">Fransk</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation type="obsolete">Russisk</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation type="obsolete">Dansk</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation type="obsolete">Slovakisk</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation type="obsolete">Ungarsk</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation type="obsolete">Tsjekkisk</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation type="obsolete">Nederlandsk</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation type="obsolete">Portugisisk</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation type="obsolete">Ukrainsk</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation type="obsolete">Polsk</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="obsolete">Gresk</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation type="obsolete">Katalansk</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation type="obsolete">Finsk</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation type="obsolete">Irsk</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="obsolete">Litauisk</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="obsolete">Svensk</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="obsolete">Slovensk</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="obsolete">Velg en katalog</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation type="obsolete">Scribus krasj</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation type="obsolete">Scribus krasjer pga Signal #%1</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation type="obsolete">&amp;Farge håndtering ...</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="obsolete">&amp;Åpne ...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation type="obsolete">Åpne nylig b&amp;rukte</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Lukk</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Lagre</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="obsolete">L&amp;agre som ...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="obsolete">Gå &amp;tilbake til lagret versjon</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="obsolete">Samle for U&amp;tskrift ...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation type="obsolete">&amp;Hent tekst/bilde ...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="obsolete">Legg til &amp;tekst ...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Importer</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="obsolete">Lagre &amp;tekst ...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="obsolete">Lagre side som &amp;EPS ...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="obsolete">Lagre som P&amp;DF ...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation type="obsolete">&amp;Eksporter</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="obsolete">Dokument inn&amp;stillinger ...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="obsolete">Skriv &amp;ut ...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="obsolete">Avs&amp;lutt</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="obsolete">&amp;Angre</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">Kli&amp;pp</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">S&amp;ett inn</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">T&amp;øm</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="obsolete">Marker &amp;alt</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="obsolete">&amp;Søk/Erstatt ...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="obsolete">Far&amp;ger ...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="obsolete">A&amp;vsnitts stiler ...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="obsolete">&amp;Linjestiler ...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation type="obsolete">&amp;Maler ...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;Javaskripts ...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">D&amp;upliker</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="obsolete">&amp;Flere kopier</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">F&amp;jern</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">&amp;Grupèr</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation type="obsolete">&amp;Oppløs gruppen</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">&amp;Lås</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">Send til &amp;Bakgrunn</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Bring til &amp;forgrunn</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">&amp;Lavere</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">Høye&amp;re</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation type="obsolete">Fordel/&amp;Still opp på linje ...</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation type="obsolete">&amp;Form</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="obsolete">&amp;Legg tekst på sti</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="obsolete">&amp;Fjern tekst fra sti</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="obsolete">&amp;Kombiner polygoner</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="obsolete">Splitt &amp;polygoner</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation type="obsolete">K&amp;onverter til Outlines</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation type="obsolete">Sett &amp;inn ...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="obsolete">&amp;Slett ...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="obsolete">&amp;Flytt ...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation type="obsolete">&amp;Bruk mal ...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation type="obsolete">Til&amp;pass vinduet</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation type="obsolete">&amp;100%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation type="obsolete">&amp;Thumbnails</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Vis &amp;gitter</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="obsolete">Fest til hjel&amp;pelinjer</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">Vis &amp;Baseline gitter</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation type="obsolete">&amp;Egenskaper</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="obsolete">&amp;Oversikt</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="obsolete">&amp;Kladdebok</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="obsolete">&amp;Nivåer</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation type="obsolete">Sidep&amp;alet</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="obsolete">&amp;Bokmerker</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="obsolete">Hå&amp;ndter bilder</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="obsolete">&amp;Orddel tekst</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="obsolete">Hjel&amp;petekster</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="obsolete">P&amp;DF-Verktøy</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation type="obsolete">Hjelpetekster</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation type="obsolete">&amp;Skrifttyper ...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation type="obsolete">&amp;Orddeler ...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation type="obsolete">&amp;Håndtering av hurtigtaster ...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation type="obsolete">&amp;Om Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="obsolete">Om &amp;Qt</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="obsolete">Scribus h&amp;åndbok ...</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation type="obsolete">&amp;Stil</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="obsolete">Inn&amp;stillinger</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="obsolete">&amp;Venstre</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="obsolete">&amp;Midten</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="obsolete">Høy&amp;re</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="obsolete">&amp;Blokk</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation type="obsolete">&amp;Tvunget</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="obsolete">&amp;Andre ...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation type="obsolete">&amp;Legg over hverandre</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation type="obsolete">L&amp;egg ved siden av hverandre</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation type="obsolete">&amp;Farge</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation type="obsolete">&amp;Inverter</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">&amp;Hent tekst ...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation type="obsolete">&amp;Skrifttype</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="obsolete">&amp;Størrelse</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation type="obsolete">&amp;Effekter</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation type="obsolete">&amp;Justering</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation type="obsolete">&amp;Skygge</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation type="obsolete">&amp;Tabulatorer ...</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">&amp;Lås/Lås opp</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">Vis b&amp;ilder</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Vis &amp;marger</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Vis &amp;rammer</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Vis h&amp;jelpelinjer</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation type="obsolete">Ghostscript: Du kan ikke bruke EPS Bilder</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation type="obsolete">Importer &amp;side(r) ...</translation>
    </message>
    <message>
        <source>100%</source>
        <translation type="obsolete">100%</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="obsolete">F&amp;est til gitter</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="obsolete">&amp;Innstillinger ...</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation type="obsolete">Importer sider ...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation type="obsolete">Importer side(r)</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation type="obsolete">&lt;p&gt;Du prøver å importere flere sider end hva som er tilgjengelig i det nåværende dokument hvis man teller fra den aktive side.&lt;/p&gt;Velg et av de følgende:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Lag&lt;/b&gt; manglende sider&lt;/li&gt;&lt;li&gt;&lt;b&gt;Importer&lt;/b&gt; sider inntil den siste siden&lt;/li&gt;&lt;li&gt;&lt;b&gt;Avbryt&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation type="obsolete">Lag</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="obsolete">Importer</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation type="obsolete">Import fullført</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation type="obsolete">Fant ingenting å importere</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation type="obsolete">Henter ICC profiler</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="obsolete">Håndter &amp;hjelpelinjer ...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="obsolete">&amp;Størrelse:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation type="obsolete">&amp;Skygge:</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation type="obsolete">Dokument &amp;informasjon ...</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation type="obsolete">&amp;Angre sletting av objektet</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation type="obsolete">&amp;Angre objektflytt</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation type="obsolete">&amp;Angre objektforandring</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation type="obsolete">&amp;Rediger form</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation type="obsolete">Filen %1 er ikke i Scribus format</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation type="obsolete">Afrikaans</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation type="obsolete">Skrifttype systemet starter</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation type="obsolete">Portugisisk (BR)</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation type="obsolete">Starter tekstredigerer</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation type="obsolete">Leser ICC Profiler</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Rediger</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation type="obsolete">Avslutter nå.</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="obsolete">&amp;PDF opsjoner</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation type="obsolete">K&amp;onverter til</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation type="obsolete">S&amp;ett inn</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="obsolete">Tegn</translation>
    </message>
    <message>
        <source>Space</source>
        <translation type="obsolete">Mellomrom</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="obsolete">Dokumenter (*.sla *.sla.gz *.scd *.scd.gz);;</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="obsolete">Dokumenter (*.sla *.scd);;</translation>
    </message>
    <message>
        <source>Postscript Files (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="obsolete">Postscript filer (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation type="obsolete">SVG bilder (*.svg *.svgz);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg);;</source>
        <translation type="obsolete">SVG bilder (*.svg);;</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;</source>
        <translation type="obsolete">OpenOffice.org Draw (*.sxd);;</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>PDF 1.3</source>
        <translation type="obsolete">PDF 1.3</translation>
    </message>
    <message>
        <source>PDF 1.4</source>
        <translation type="obsolete">PDF 1.4</translation>
    </message>
    <message>
        <source>PDF/X-3</source>
        <translation type="obsolete">PDF/X-3</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation type="obsolete"> ble erstatet av: </translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation type="obsolete">(Konvertert)</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation type="obsolete">Lagre som</translation>
    </message>
    <message>
        <source>Abort</source>
        <translation type="obsolete">Avbryt</translation>
    </message>
    <message>
        <source>Ignore</source>
        <translation type="obsolete">Ignoreer</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation type="obsolete">Følgende program mangler:</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation type="obsolete">EPS filer (*.eps);;Alle files (*)</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation type="obsolete">&amp;Lås alle</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation type="obsolete">Kroatisk</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation type="obsolete">Bulgarsk</translation>
    </message>
    <message>
        <source>The program</source>
        <translation type="obsolete">Programmet</translation>
    </message>
    <message>
        <source>is already running!</source>
        <translation type="obsolete">kjører allerede!</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="obsolete">Informasjon</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">mangler!</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation type="obsolete">&amp;Nivå</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation type="obsolete">Send til lag</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation type="obsolete">Forhåns&amp;visning innstillinger</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation type="obsolete">Forhånsvisning innstillinger</translation>
    </message>
    <message>
        <source>Level</source>
        <translation type="obsolete">Nivå</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">Send til &amp;lag</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="obsolete">PostScript</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation type="obsolete">&amp;Ignorer</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Navn:</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation type="obsolete">-Side%1</translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Sample</source>
        <translation type="obsolete">Prøve</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Farge</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Document Colors</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <source>New Layer</source>
        <translation>Nytt lag</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Bakgrunn</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Starter tilleggsprogram</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Henter innstillinger</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation>Starter tekstredigerer</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation>Leser ICC Profiler</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Setter opp kladdebok</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Setter opp snarveier</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Rediger</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Henter skrifttyper</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation>Avslutter nå.</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Alvorlig feil</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Skrifttype systemet starter</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Åpne nylig b&amp;rukte</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Eksporter</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Rediger</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>&amp;Stil</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Farge</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Størrelse</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Skygge</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Skrifttype</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Effekter</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Objekt</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Forhånsvisning innstillinger</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Nivå</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Send til &amp;lag</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF opsjoner</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Form</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation>K&amp;onverter til</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation>S&amp;ett inn</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Tegn</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Mellomrom</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>Si&amp;de</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Vis</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;kstra</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>Vi&amp;nduer</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Hjelp</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Justering</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Klar</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Importer sider ...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importer side(r)</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Import fullført</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Fant ingenting å importere</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Leser inn ...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation>(Konvertert)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Alle støttede formater</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *scd.gz);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Lagre som</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Lagrer ...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>&amp;Ignorer</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Skriver ut ...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Utskrift feilet!</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Om Qt</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Scribus håndbok</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Lagre som</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Tekstfiler (*.txt);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Navn:</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Størrelse</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Skygge:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Skygge</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Ingen stil</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation>Følgende program mangler:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>EPS filer (*.eps);;Alle files (*)</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation>-Side%1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation>&amp;Lås alle</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Informasjon</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation>&amp;Nivå</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation>Send til lag</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation>Forhåns&amp;visning innstillinger</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>Verk&amp;tøy</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-Pos:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-Pos:</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do you really want to replace your existing image?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <source>Invalid argument: </source>
        <translation>Ugyldig argument: </translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Innstillinger:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation>Ikke vis oppstartsskjerm ved oppstart</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>filename</source>
        <translation>filnavn</translation>
    </message>
    <message>
        <source>Use filename as path for user given preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus Version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Hjemmeside</translation>
    </message>
    <message>
        <source>Documentation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>Issues</source>
        <translation type="unfinished"></translation>
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
        <translation>Lag</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="obsolete">Alle</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>Kopier hit</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Flytt hit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Lim inn</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Bilde</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Fil: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Original PPI: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Faktisk PPI: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Lenket tekst</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Tekstramme</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Legg tekst til sti</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Avsnitt: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Ord: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Tegn: </translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Skriv ut: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>I bruk</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Ikke i bruk</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>In&amp;fo</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation type="unfinished">Forhånsvisning innstillinger</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF opsjoner</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Send til &amp;lag</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>Ni&amp;vå</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>Konve&amp;rter til</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation>Side %1 til %2</translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Ukjent</translation>
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
        <translation>Gråskala</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Dokument:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>er endret siden sist gang det ble lagret.</translation>
    </message>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation type="obsolete">&amp;Avslutt uten å lagre</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation type="obsolete">&amp;Lukk uten å lagre</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation type="obsolete">&amp;Lagre nå</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Discard</source>
        <translation>&amp;Forkast</translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <source>Embedded Python scripting support.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>Script error</source>
        <translation>Script feil</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Hvis du kjører et offisielt skript reporter det vennligst til 
&lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Denne meldingen er overført til ditt klippebord, bruk Ctrl+V for å lime den inn i feilmeldingssøkeren.</translation>
    </message>
    <message>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation type="obsolete">Python script (*.py)</translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documentation for:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> doesn&apos;t contain any docstring!</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScripterPreferences</name>
    <message>
        <source>Use a Startup Script</source>
        <translation type="obsolete">Bruk et oppstartsskript</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation type="obsolete">Let ...</translation>
    </message>
    <message>
        <source>Browse for a new script file</source>
        <translation type="obsolete">Let etter en ny skriptfil</translation>
    </message>
    <message>
        <source>Script File:</source>
        <translation type="obsolete">Skriptfil:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Close without saving changes</source>
        <translation type="obsolete">Lukk uten å lagre endringer</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>Save changes and close</source>
        <translation type="obsolete">Lagre endringer og lukk</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation type="obsolete">Flere valg</translation>
    </message>
    <message>
        <source>Use Fake Stdin</source>
        <translation type="obsolete">Bruk falsk Stdin</translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Scripter Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Extensions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation>Feil:</translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation>Kommentarer:</translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation>Nøkkelord:</translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation>Tegn:</translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation>Tall:</translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation>Velg farge</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="unfinished">Endre ...</translation>
    </message>
    <message>
        <source>Locate Startup Script</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Vis forhåndsvisning av side</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation type="obsolete">Vis navn på maler</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Søk/Erstatt</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Søk etter:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Avsnittsstil</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Skrifttype</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Skriftstørrelse</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Skrifteffekter</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Fyllfarge</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Fyllskygge</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Strekfarge</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Strekskygge</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Venstre</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Midten</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Høyre</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Blokk</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Tvunget</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Erstatt med:</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Søking avsluttet</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Greit</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Hele ordet</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Ignorer små/store bokstaver</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Søk</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Bytt ut</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Bytt ut &amp;alle</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>T&amp;øm</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>Search finished, found %1 matches</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Arrange Pages</source>
        <translation type="obsolete">Arranger sider</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation type="obsolete">Tilgjengelige maler:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation type="obsolete">Dokument sider:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation type="obsolete">Motstående sider</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation type="obsolete">Venstre side først</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation type="obsolete">Trekk sider eller maler til Søppelbøtten for å slette dem.</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation type="obsolete">Her kan du se forhåndsvisning av alle sidene i ditt dokument.</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation type="obsolete">Her er alle dine maler. Trekk en mal 
til vinduet nedenunder, for å lage en ny side.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Left Page First</source>
        <translation type="obsolete">Venstre side først</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Velg felt</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Tilgjengelige felt</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>Valgte felt</translation>
    </message>
    <message>
        <source>&amp;&gt;&gt;</source>
        <translation>&amp;&gt;&gt;</translation>
    </message>
    <message>
        <source>&amp;&lt;&lt;</source>
        <translation>&amp;&lt;&lt;</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Andre ...</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Skygge</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Skygge:</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y-Offset</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished">Short Words</translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Ingen stil</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>Rediger stiler ...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Ingen stil</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Rediger stiler</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopi av %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Ny stil</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Nei</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ja</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *scd.gz);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.scd);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Legg til</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Rediger</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;upliker</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Lagre</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="obsolete">Vil du virkelig slette denne stilen?</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Tekstbehandler</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation type="obsolete">Oppdater tekstramme</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Gjeldende avsnitt:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Ord: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Tegn: </translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Totalt:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Avsnitt: </translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Vil du lagre dine forandringer?</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation type="obsolete">Ønsker du virkelig å miste alle dine forandringer?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Ønsker du virkelig å fjerne all din tekst?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Tekstfiler (*.txt);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Lagre som</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation type="obsolete">Oppdater tekstrammen og avslutt</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation type="obsolete">Avslutt uten å oppdatere tekstrammen</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Hent pånytt tekst fra ramme</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Lagre til fil ...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Hent fil ...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Lagre &amp;dokument</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Oppdater tekstrammen og avslutt</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>&amp;avslutt uten oppdatering av tekstramme</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Kli&amp;pp</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Lim inn</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>T&amp;øm</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation type="obsolete">Sett &amp;inn spesiell ...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Oppdater tekstramme</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Rediger</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Marker &amp;Alt</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>R&amp;ediger stiler ...</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation type="obsolete">Hent tekst fra fil</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation type="obsolete">Lagre tekst til fil</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation type="obsolete">Hent pånytt tekst fra ramme</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Søk/Erstatt ...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>&amp;Forhåndsvisning av skrifttype ...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Bakgrunn ...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Vis skrifttype ...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>Inn&amp;stillinger</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation type="obsolete">Søk/Erstatt</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation type="obsolete">Slett all tekst</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Smart tekstvalg</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation>Fjern all tekst</translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation>Tekstbehandler - %1</translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="unfinished">Sett &amp;inn</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="unfinished">Tegn</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Space</source>
        <translation type="unfinished">Mellomrom</translation>
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
        <translation>Forskyvning</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Linjebredde</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation>Understreket</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Små bokstaver</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Senket skrift</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Hevet skrift</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Gjennomstrekking</translation>
    </message>
    <message>
        <source>Outline Text</source>
        <translation type="obsolete">Fremhev tekst</translation>
    </message>
    <message>
        <source>Underline Words Only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="obsolete">Oversikt</translation>
    </message>
    <message>
        <source>Shadow</source>
        <translation>Skygge</translation>
    </message>
    <message>
        <source>Outline</source>
        <comment>Text Style Selector</comment>
        <translation type="unfinished">Oversikt</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>Update paragraph styles</source>
        <translation type="obsolete">Oppdater stilen på avsnittet</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Bruk filnavnet som begynnelse på navnet på stilen til avsnittet</translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation type="obsolete">Skal importverktøyet legge til navnet på dokumentet i avsnittsstilen?</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Ikke spør igjen</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation type="obsolete">Hvis det allerede finnes en avsnittsstil med det samme navnet som avsnittet fra OpenOffice dokumentet, skal da stilen i Scribus endres slik at det passer avsnittet som importeres, eller skal være i fred?</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>OpenOffice.org Writer importopsjoner</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Avbryt</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation>På slutten</translation>
    </message>
    <message>
        <source>At the end</source>
        <translation>På begynnelsen</translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation>Ikke vist</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Innholdsfortegnelse</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation>Innhol&amp;d</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Innholdsfortegnelse</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation>Innhol&amp;d</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation>Ignorer alle feil</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation>Laveste tillatte oppløsning</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation>Legg til profil</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation>Fjern profil</translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Snapping</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Snap Distance:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Grab Radius:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> px</source>
        <translation> px</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation>Vis hjelpelinjer</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farge:</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation>Vis marglinjer</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Vis Baseline gitter</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>Grunnlinje &amp;Nett:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>Grunnlinje &amp;forskyvning:</translation>
    </message>
    <message>
        <source>Guides are not visible through objects on the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Guides are visible above all objects on the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Avstand mellom gitter linjer</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Avstand mellom hoved-gitterlinjer</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Maksimum avstand for når et objekt vil legge seg på plasseringsguidene</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Radius for område der Scribus vil la deg fange handles for et objekt</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Farge for gitterlinjer</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Farge for hoved-gitterlinjer</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Farge på hjelpelinjer du innsetter</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Marginlinjefarge</translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the lines of the baseline grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance from the top of the page for the first baseline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Håndter tabulatorer</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation>Eksporter område</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>&amp;Alle sider</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>&amp;Velg sider</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotasjon:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Fil innstillinger</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>Kompatibil&amp;itet:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>Inn&amp;binding:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Venstre marg</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Høyre marg</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Lag &amp;Thumbnailer</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Lagre &amp;lenkede tekstrammer som PDF artikler</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>&amp;Inkluder bokmerker</translation>
    </message>
    <message>
        <source>Include Layers</source>
        <translation type="obsolete">Inkluder lag</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>&amp;Oppløsning for EPS bilder:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>Kom&amp;primer tekst og vektorgrafikk</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation>Bilde innstillinger</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatisk</translation>
    </message>
    <message>
        <source>JPEG</source>
        <translation type="obsolete">JPEG</translation>
    </message>
    <message>
        <source>Zip</source>
        <translation type="obsolete">Zip</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="obsolete">&amp;Metode:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="obsolete">&amp;Kvalitet:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Maksimum</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Høy</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Medium</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Lav</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Minimum</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;Generelt</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>&amp;Inkluder alle skrifttyper</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Inkluder</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Tilgjengelige skrifttyper:</translation>
    </message>
    <message>
        <source>&amp;&gt;&gt;</source>
        <translation>&amp;&gt;&gt;</translation>
    </message>
    <message>
        <source>&amp;&lt;&lt;</source>
        <translation>&amp;&lt;&lt;</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation>Skrifttyper som skal inkluderes:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Skrifttyper</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>&amp;Vis forhåndsvisning av sidene</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effekter</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>&amp;Vis varighet:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>&amp;Varighet på effekten:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>Effektt&amp;ype:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>Beve&amp;gelige linjer:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>F&amp;ra:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>Retn&amp;ing:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> sekund</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Ingen effekt</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Persienner</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Boks</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Oppløse</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Glitter</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Dele</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Tørk vekk</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horisontal</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertikal</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Innenfor</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Ytterside</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Venstre mot høyre</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Topp til bunn</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Bunn til topp</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Høyre mot venstre</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Topp-venstre til Bunn-Høyre</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>&amp;Bruk effekten på alle sidene</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;kstra</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>&amp;Bruk kryptering</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Passord</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Bruker:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>&amp;Eier:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Innstillinger</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>&amp;Godta utskrift av dokumentet</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>G&amp;odta endringer i dokumentet</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>&amp;La andre kopiere tekst og bilder</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>L&amp;a andre legge til nye kommentarer og felt</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>S&amp;ikkerhet</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Generelt</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Utskrift Be&amp;regnet for:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Skjerm / Web</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Skriver</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Gråskala</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Bruk eget framvisningsoppsett</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Framvisningsoppsett</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>Hy&amp;ppighet:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Vinkel:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>S&amp;pot funksjon:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Enkel prikk</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linje</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Rund</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Ellipse</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Massive farger:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Bruk ICC profil</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Rendering-Intent:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relative Colorimetric</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolutt Colorimetric</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Bilder:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Ikke bruk inkluderte ICC profiler</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>F&amp;arge</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>PDF/X-3 Output Intent</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Informasjons streng:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Output &amp;Profil:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Trimboks</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="obsolete">Inkluder skrifttyper i PDFen. Dette vil bety at layouten 
og utseendet av ditt dokument vil bli beholdt.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Vis sideforhåndsvisning av alle de ovenfor valgte sider.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>Tidsrom siden er vist før presentasjonen starter på den valgte siden.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="obsolete">Tidsrom effekten varer.
Et mindre tidsrom betyr at effekten vil gå fortere, et lengre vil gjøre at den går langsommere.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Type av visningseffekt.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Retning på bevegende linjer for split og blining effektene.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Startposisjonen av boks og splitt effektene.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Retningen på glitter eller wipe effektene.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Bruk effekt på alle sider.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Eksporter alle sider til PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Eksporter et utvalg av sider til PDF</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="obsolete">Bestemmer bindingen av sider i PDFen. Hvis ikke du er sikker
på at du må forandre det, bruk Venstre.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="obsolete">Lager thumbnails av hver side i PDFen.
Noen visere kan bruke thumbnails for navigering.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Lag PDF artikler, noe som er nyttig for å navigere lenkede artikler i en PDF.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="obsolete">Legg inn bokmerkene du lagde i ditt dokument.
Disse er nyttige for å navigere i lange PDF filer.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="obsolete">Eksporter resolutionen til tekst og vektor grafikk.
Dette har ingen innflytelse på oppløsningen til bitmap bilder som fotografier.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Komprimering av tekst og grafikk.
Hvis ikke du har noen grunn, bruk dette. Det reduserer PDF størrelsen.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Versjon av komprimering for bilder.
Automatisk tillater Scribus å velge den beste metoden.
ZIP er bra for bilder med solide farger.
JPEG er bedre for å lage små PDF filer som har mane fotografier (mens det kan forekomme tap i bildekvaliteten).
La det stå på automatisk, hvis ikke du har en spesiell grunn til å endre det.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="obsolete">Komprimeringsnivåer: Minimum (25%), Lavt (50%), Medium (75%), Høyt (85%), Maksimum (95%)</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (Prikker per Tomme) for bilde eksportering.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="obsolete">Velg et hovedpassord som aktiverer og deaktiverer alle
sikkerhetsfunksjonen i din eksporterte PDF</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Velg et passord så brukere kan lese din PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Tillat utskriving av PDFen.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Tillat forandring av PDfen.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="obsolete">Tillat kopiering av tekst eller grafikk fra PDFen.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="obsolete">Tillat tilføyning av merknader og felter til PDFen.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="obsolete">Fargemodell for utgivelse av din PDF.
Velg Skjerm/Web for PDFer som skal vises på skjerm eller for å skrive på vanlige inkjets.
Velg Skriver når du skal skrive på en riktig 4 farge CMYK skriver.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="obsolete">Dette er en avansert innstilling som ikke er tilgjengelig som standard. Du bør bare bruke dette hvis skriveren ber spesielt om det og du vet nøyaktig hvilke innstillinger du trenger. Hvis du gjør det feil kan det hende pdf-fila ikke kan skrives ut på rett vis, og er absolutt ikke overførbart mellom ulike plattformer.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Legg inn en fargeprofil for mettede farger</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Fargeprofil for mettede farger</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Rendering intent for mettede farger</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Legg inn en farge profil for bilder</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Ikke bruk farge profiler som er lagt inn i bildene</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Farge profil for bilder</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Rendering intent for bilder</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Utgivelse profil for utskrift. hvis mulig, få noe rådgiving fra din skriver når du velger.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="obsolete">Nødvendig tekstbit for PDF/X-3. Ellers vil PDFen ikke
følge PDF/X3-conformance. Vi foreslår at du bruker dokumentets tittel.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Avstand for bleed fra toppen av den fysiske siden</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Avstand for bleed fra bunnen av den fysiske siden</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Avstand for bleed fra venstresiden av den fysiske siden</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Avstand for bleed fra høyresiden av den fysiske siden</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Speilvend siden(e) horisontalt</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Speilvend siden(e) vertikalt</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation>Komprimerings &amp;kvalitet:</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Include La&amp;yers</source>
        <translation>Inkluder la&amp;g</translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation>Komprimeringsmeto&amp;de:</translation>
    </message>
    <message>
        <source>Resa&amp;mple Images to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is choosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. This can increase memory usage and slow down export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Layout</source>
        <translation type="unfinished">Side utseende</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation type="unfinished">Enkel side</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double Page Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double Page Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Visual Appearance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use Viewers Defaults</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use Full Screen Mode</source>
        <translation>Bruk fullskjermmodus</translation>
    </message>
    <message>
        <source>Display Bookmarks Tab</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display Thumbnails</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display Layers Tab</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hide Viewers Toolbar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hide Viewers Menubar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Special Actions</source>
        <translation>Spesielle handlinger</translation>
    </message>
    <message>
        <source>Javascript to be executed
when Document is opened:</source>
        <translation>Javaskript som skal brukes
når dokumentet åpnes:</translation>
    </message>
    <message>
        <source>No Script</source>
        <translation>Ingen skript</translation>
    </message>
    <message>
        <source>Viewer</source>
        <translation>Fremviser</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lossy - JPEG</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lossless - Zip</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation>Skrifttype:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Størrelse:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Kolo&amp;nner:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Mellomrom:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>&amp;Linjefarge:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Skygge:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>&amp;Fyllfarge:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>&amp;Skygge:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation>Linjestil:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Linje &amp;bredde:</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation>&amp;Linjestil:</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>Piler:</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>Start:</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>Slutt:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>&amp;Horisontal skalering:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>&amp;Vertikal skalering:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>&amp;Skaler bilde til rammestørrelse</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Behold høyde/bredde fo&amp;rhold</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>&amp;Fyllfarge:</translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>Mi&amp;nimum:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>Ma&amp;ksimum:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>&amp;Skritt:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Tekstfelt egenskaper</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Billedramme egenskaper</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Form tegningegenskaper</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Standard forstørrelsesnivå</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Linje tegningegenskaper</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Polygon tegneegenskaper</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Skrifttype som skal brukes i nye tekstrammer</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Størrelse for skrifttype for nye tekstrammer</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Skriftfarge</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Antall kolonner i en tekstramme</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Mellomrom mellom kolonner i tekstramme</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Eksempel på skrifttypen din</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Bilderamme tillater bilder å skalere til enhver størrelse</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Horisontal skalering av bilder</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Vertikal skalering av bilder</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Samme horisontal og vertikal skalering </translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Bilder i bilderammer er skalert opp til rammens størrelse</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Automatisk skalerte bilder beholder deres originale proporsjoner</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Fyll farge på bilderammer</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Fyllfargens styrke </translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Formers linjefarge </translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Linjefarges styrke</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Formenes fyllfarge</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Formenes linjestil</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Formenes linjebredde</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Minimum tillat forstørrelse</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Maksimal tillat forstørrelse</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Forandring i forstørrelse for hvert enkelt zoom</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Linjefarge</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Linjestil</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Linjetykkelse</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Brukerdefinert:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Brukerdefinert: </translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Brukerdefinert</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation>Senket skrift</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Forskyvning:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;Skalering:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Hevet skrift</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>F&amp;orskyvning:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>S&amp;kalering:</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Understreket</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation>Forskyvning:</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Linjebredde:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Gjennomstreking</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Små bokstaver</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Sk&amp;alering:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Forskyvning over bunnlinjen for en skrifttype på en linje</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Relativ størrelse av hevet skrift i forhold til normal skrifttype</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Forskyvning under bunnlinjen for en skrifttype på en linje</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Relativ størrelse av senket skrift i forhold til normal skrifttype</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Relativ størrelse av små bokstaver i forhold til normal skrifttype</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Relativ størrelse i prosent av linjeavstand i forhold til normal skrifttype</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Venstre</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Høyre</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Full stopp</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Komma</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Midten</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Slett alle</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Innrykk for første linje av avsnittet</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Innrykk fra venstre for hele avsnittet</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Slett alle tabulatorer</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Posisjon:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation type="obsolete">Første &amp;linje:</translation>
    </message>
    <message>
        <source>Left Ind&amp;ent:</source>
        <translation type="obsolete">Innrykk fra v&amp;enstre:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Brukerdefinert:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Brukerdefinert: </translation>
    </message>
    <message>
        <source>Dot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Brukerdefinert</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>Oversikt</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Element</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Type</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="obsolete">Informasjon</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Advarsel</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Navnet &quot;%1&quot; er ikke entydig.
Vennligst velg et annet.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Greit</translation>
    </message>
    <message>
        <source>X:</source>
        <translation type="obsolete">X:</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="obsolete">Y:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation type="obsolete">Skrifttype:</translation>
    </message>
    <message>
        <source>Image</source>
        <translation type="obsolete">Bilde</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="obsolete">Tekst</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Linje</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="obsolete">Polygon</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="obsolete">Polylinje</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation type="obsolete">Tekststi</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Side</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Gruppe </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Frie objekter</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Side </translation>
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
        <translation>Forskyvning</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Linjebredde</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>Legg til vertikal hjelpelinje</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>Legg til horisontal hjelpelinje</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>Fjern vertikal hjelpelinje</translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation>Fjern horisontal hjelpelinje</translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation>Flytt vertikal hjelpelinje</translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation>Flytt horisontal hjelpelinje</translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation>Lås fast hjelpelinjene</translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation>Lås opp hjelpelinjene</translation>
    </message>
    <message>
        <source>Move</source>
        <translation>Flytt</translation>
    </message>
    <message>
        <source>Resize</source>
        <translation>Endre størrelse</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>Roter</translation>
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
        <translation>W1: %1, H1: %2
W2: %3, H2: %4</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Utvalg</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Grupèr</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Lag</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation>X: %1, Y: %2
W: %3, H: %4</translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation>Juster/Fordel</translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation>Farge1: %1, Farge2: %2</translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Flip horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Flip vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Lås</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Lås opp</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation>Lås størrelse</translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation>Lås opp størrelse</translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Slett</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Endre navn</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation>Fra %1
til %2</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Lim inn</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Klipp ut</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set the style of line end</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set the style of line join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set line width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set custom line style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do not use custom line style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set start arrow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set end arrow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create table</source>
        <translation>Lag tabell</translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation>Rader: %1, Kolonner: %2</translation>
    </message>
    <message>
        <source>Set font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font stroke color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font fill color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font stroke color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set kerning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set line spacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set language</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align text</source>
        <translation>Juster tekst</translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation>Bilderamme</translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation>Tekstramme</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polygon</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation>Bezierkurve</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polylinje</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation>Konverter til</translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation>Importer SVG bilde</translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation>Importer EPS bilde</translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation>Importer OpenOffice.org Draw bilde</translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text flows around bounding box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text flows around contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No text flow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No bounding box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation>Side %1</translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation>Størrelse på ramme</translation>
    </message>
    <message>
        <source>Free scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Keep aspect ratio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add page</source>
        <translation>Legg til side</translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation>Legg til sider</translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation>Slett side</translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation>Slett sider</translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation>Legg til lag</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Slett lag</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation>Gi nytt navn på lag</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Hev lag</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation type="unfinished">Senk lag</translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation>Flytt til lag</translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation>Bytt navn på lag</translation>
    </message>
    <message>
        <source>Get image</source>
        <translation>Sett inn bilde</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Legg tekst til sti</translation>
    </message>
    <message>
        <source>Enable Item Printing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Disable Item Printing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Multiple duplicate</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Initial State</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Action History</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Angre</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished"></translation>
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
    <name>UsePrinterMarginsDialog</name>
    <message>
        <source>Minimum Margins for Page Size %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <source>Use Printer Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select &amp;Printer:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Margins</source>
        <translation type="unfinished">Marger</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished">&amp;Topp:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished">&amp;Venstre:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation>Skriv inn en verdi</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Skriv inn en verdi, og trykk på Greit.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Skriv inn en verdi, og trykk på Greit</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Send din verdi til skriptet</translation>
    </message>
</context>
<context>
    <name>VlnaDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Short Words</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Valgte rammer</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Aktiv &amp;side</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Alle objekter</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Info and
Languages</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Informasjon og språk</translation>
    </message>
    <message>
        <source>Replace defaults by user config</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Bytt ut standard innstillinger med brukerens egne</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Kun den valgte rammen er behandlet.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Kun den faktiske siden er behandlet.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Alle objekter i dokumentet er behandlet.</translation>
    </message>
    <message>
        <source>Short Words for Scribus</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Short Words for Scribus</translation>
    </message>
    <message>
        <source>Available in the following languages</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Tilgjengelig på disse språkene</translation>
    </message>
    <message>
        <source>About Short Words</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Om Short Words</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">&amp;Rediger</translation>
    </message>
    <message>
        <source>&amp;Info and Languages...</source>
        <translation type="obsolete">&amp;Informasjon og språk ...</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="obsolete">&amp;Hjelp</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="obsolete">Short Words</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Verktøy</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation type="obsolete">Valgte objekter</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation type="obsolete">Sett inn tekstramme</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation type="obsolete">Sett inn bilde</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation type="obsolete">Sett inn tabell</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Egenskaper ...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation type="obsolete">Sett inn polygoner</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation type="obsolete">Sett inn linjer</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation type="obsolete">Sett inn Bezierkurve</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation type="obsolete">Sett inn frihånd tegnet linje</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation type="obsolete">Roter objekt</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation type="obsolete">Zoom inn eller ut</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation type="obsolete">Rediger innhold av rammen</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation type="obsolete">Rediger teksten med tekstbehandler</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation type="obsolete">Lenk sammen tekstrammer</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation type="obsolete">Fjern lenken mellom tekstrammene</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation type="obsolete">Tegn forskjellige former</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation type="obsolete">Gjennomfør målinger</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>Button</source>
        <translation>Knapp</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Tekstfelt</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Avkryssingsboks</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Kombi boks</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Liste Boks</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Lenke</translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation>PDF Verktøy</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Sett inn PDF felt</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Sett inn PDF annotasjoner</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation type="obsolete">Velg Tegn:</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="obsolete">Sett inn tegn ved markørens posisjon i teksten</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="obsolete">Fjern det valgte.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="obsolete">Lukk denne dialog og gå tilbake til tekstredigering.</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="obsolete">Sett &amp;inn</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">T&amp;øm</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Lukk</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Velg filter for bruk ved import</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatisk</translation>
    </message>
    <message>
        <source>Get text only</source>
        <translation type="obsolete">Hent kun tekst</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Importer tekst uten å formatere den</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Importer:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kodering:</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>Importer kun tekst</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Velg filter for bruk ved import</translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Husk filendelsen</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation type="obsolete">Husk hvilken type importfilter som skal brukes for filer med 
denne filendelsen, og ikke spør i fremtiden om dette.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Greit</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation>Husk hvilken type importfilter som skal brukes for filer med denne filendelsen, og ikke spør i fremtiden om dette.</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Ny fra mal</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farger</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Beskrivelse</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Bruk</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Laget med</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Forfatter</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Fjern</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Åpne</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Last ned maler</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Installer maler</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Pakk ut til den midlertidige katalogen ~/.scribus/templates tilhørende gjeldende bruker, eller bruk PREFIX/share/scribus/templates for alle brukere på systemet.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Gjør klar en mal</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Fjern en mal</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Oversetter template.xml</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Dokumentmaler finnes på http://www.scribus.net, se under Downloads.</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Sørg for at bilder og skrifttyper du bruker er fritt tilgjengelige. Hvis skrifttypen ikke kan bli delt med andre, ikke sammel dem når dokumentet lagres som mal.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Maloppretteren sørger også for at Installasjonsmal seksjon overfor også passer til deres templates. Dette betyr at brukeren skulle kunne være i stand til å laste ned en samling med maler, og pakke dem ut i mal katalogen og starte å bruke dem. </translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Det å fjerne en mal fra Ny Template dialogen vil kun fjerne denne fra template.xml, det vil ikke slette dokument filene. En meny angående fjerning vises bare dersom du har skrivetilgang til template.xml filen.</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Kopier en eksisterende template.xml til en fil med navn template.språk_LAND.xml (bruk samme språk kode som er brukt i qm filen for ditt språk), for eksempel template.nb_NO.xml for norsk bokmål template.xml. Kopien må ligge i samme katalog som den originale template.xml for at Scribus skal kunne laste den opp.</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Dato</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Lagre som mal</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Kategori</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farger</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Beskrivelse</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Bruk</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Forfatter</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>Epost</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Flere detaljer</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Færre detaljer</translation>
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
        <translation>liggende</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>stående</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>brukerdefinert</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Lag et filter</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>T&amp;øm</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Velg et tidligere lagret filter</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Oppgi et navn på dette filteret for lagring</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Oppgi et navn for lagring</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Fjern eller aktiver denne filerraden</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Fjern denne filterraden</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Legg til et nytt filter</translation>
    </message>
    <message>
        <source>to</source>
        <translation>til</translation>
    </message>
    <message>
        <source>and</source>
        <translation>og</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>fjern passende</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>ikke fjern passende</translation>
    </message>
    <message>
        <source>words</source>
        <translation>ord</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Fjern</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Bytt ut</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Bruk</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>Verdien til venstre er et regulært uttrykk</translation>
    </message>
    <message>
        <source>with</source>
        <translation>med</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>avsnittsstil</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>alle tilfeller av</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>alle avsnitt</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>avsnitt som begynner med</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>avsnitt med mindre enn</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>avsnitt med mer enn</translation>
    </message>
</context>
</TS>
