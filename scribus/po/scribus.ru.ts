<!DOCTYPE TS><TS>
<context>
    <name></name>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation>getColorNames() -&gt; list

Возвращает список имён цветов, определённых для этого документа.
Если ни один документ не открыт, возвращается список стандартных цветов документа.
</translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation>newDocDialog() -&gt; bool

Показывает диалог «Новый документ». Создаёт новый документ, если пользователь
примет установки. Не создаёт документ, если пользователь нажимает «Отменить».
Возвращает значение true, если документ создан.
</translation>
    </message>
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillColor([&quot;name&quot;]) -&gt; string

Возвращает имя цвета заливки объекта &quot;name&quot;. Если &quot;name&quot; 
не задан, используется текущий выбранный объект.
</translation>
    </message>
    <message>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation>moveObject(dx, dy [, &quot;name&quot;])

Смещает обект &quot;name&quot; на dx и dy по отношению к текущей позиции.
Расстояния выражаются в текущей единице измерения документа 
(см. константы UNIT). Если &quot;name&quot; не задано, используется выбранный
в данный момент объект. Если объект &quot;name&quot; принадлежит группе, 
перемещается вся группа.
</translation>
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
        <source>Contributions from:</source>
        <translation>Свой вклад в разработку внесли:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>О &amp;программе</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>&amp;Авторы</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>П&amp;ереводы</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>В &amp;Интернете</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Закрыть</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Команда разработчиков:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Официальная документация:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Остальная документация:</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Веб-сайт</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Электронный справочник</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Система отслеживания ошибок</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Список рассылки</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Официальные переводы и переводчики:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Предыдущие авторы переводов:</translation>
    </message>
    <message>
        <source>About Scribus %1</source>
        <translation>О Scribus %1</translation>
    </message>
    <message>
        <source>Windows Port:</source>
        <translation>Версия для Windows:</translation>
    </message>
    <message>
        <source>Mac OSX Aqua Port:</source>
        <translation>Портирование на Mac OS X:</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Вики</translation>
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
        <translation>Используется GhostScript версии %1</translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation>Ни одна версия GS не  доступна</translation>
    </message>
    <message>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation>&lt;b&gt;Scribus версии %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Build ID:</source>
        <translation>ID сборки:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation>Здесь показывается версия, дата сборки и статус поддержки тех или иных возможностей в этой сборке Scribus. Сокращение C-C-T-F означает: C=littlecms C=CUPS T=TIFF F=Fontconfig. Последняя буква - это либо C=cairo, либо A=libart. Если библиотека не использована, вместо буквы подставляется астериск - *. Здесь также отображается номер используемой версии Ghostscript.</translation>
    </message>
    <message>
        <source>January</source>
        <translation>Январь</translation>
    </message>
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Нет</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus: О модулях</translation>
    </message>
    <message>
        <source>File Name:</source>
        <translation>Имя файла:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>Версия:</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation>Включено:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation>Дата выпуска:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Копирайт:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation>Автор(ы):</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Описание:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation>Лицензия:</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Закрыть</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+З</translation>
    </message>
</context>
<context>
    <name>ActionManager</name>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новый</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Открыть...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Закрыть</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Сохранить</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Сохранить &amp;как...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>&amp;Вернутьcя к сохранённому</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Со&amp;брать для вывода...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Получить текст...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>&amp;Добавить текст...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation>Вставить изображение...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Сохранить &amp;текст...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Сохран&amp;ить страницу как EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Сохр&amp;анить как PDF...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>Параметры до&amp;кумента...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>На&amp;печатать...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>В&amp;ыйти</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Отменить</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Пов&amp;торить</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation>&amp;Действия только с текущим объектом</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Вырезать</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>С&amp;копировать</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Вст&amp;авить</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>&amp;Очистить содержимое</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Выделить в&amp;сё</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>Снять в&amp;ыделение</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Найти/Заменить...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>Изменить изображение...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>Ц&amp;вета...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>&amp;Абзацные стили...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>Стили &amp;линий...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation>&amp;Мастер-страницы...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>&amp;Настроить Scribus...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>Друг&amp;ое...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>В&amp;лево</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>По &amp;центру</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>&amp;Вправо</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>По &amp;ширине</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Принудительно</translation>
    </message>
    <message>
        <source>&amp;%1 %</source>
        <translation>&amp;%1 %</translation>
    </message>
    <message>
        <source>&amp;Normal</source>
        <translation>&amp;Обычный</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Подчёркнутый</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation>Подчеркнуть &amp;слова</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>В&amp;ычеркнутый</translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation>Все ка&amp;пителью</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation>&amp;Капитель</translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>&amp;Верхний индекс</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>&amp;Нижний индекс</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation>&amp;Схема документа</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation>&amp;Тень</translation>
    </message>
    <message>
        <source>&amp;Image Effects</source>
        <translation>&amp;Эффекты изображения</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>Т&amp;абуляторы...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Проду&amp;блировать</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>Продублировать &amp;многократно</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Удалить</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>С&amp;группировать</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>&amp;Разгруппировать</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>За&amp;блокирован</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>&amp;Размер заблокирован</translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation>В самый &amp;низ</translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation>На самый &amp;верх</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>Сделать уровнем н&amp;иже</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>Сделать уровнем в&amp;ыше</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Передать в запа&amp;сник</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>&amp;Атрибуты...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>Изображение &amp;видимо</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation>&amp;Обновить изображение</translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation>Блок до размеров изображения</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation>Расширенные свойства изображения</translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation>&amp;Низкое разрешение</translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation>&amp;Высокое разрешение</translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation>&amp;Полное разрешение</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Является PDF-&amp;закладкой</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Является PDF-&amp;аннотацией</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>Свойства а&amp;ннотации</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>Свойства по&amp;ля</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>Изменить &amp;очертания...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>Направить &amp;текст по контуру</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>Сн&amp;ять текст с контура</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>Объединить &amp;многоугольники</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Разде&amp;лить многоугольники</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>Кривая &amp;Безье</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>&amp;Изображение</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation>&amp;Контуры</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>&amp;Многоугольник</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Текстовый блок</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation>&amp;Символ...</translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>Шаблонный текст</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Вставить...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation>&amp;Импортировать...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Удалить...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>С&amp;копировать...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Переместить...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation>Применить &amp;мастер-страницу...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>&amp;Настроить направляющие...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation>&amp;Уместить в окне</translation>
    </message>
    <message>
        <source>&amp;50%</source>
        <translation>&amp;50&amp;</translation>
    </message>
    <message>
        <source>&amp;75%</source>
        <translation>&amp;75&amp;</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation>&amp;100&amp;</translation>
    </message>
    <message>
        <source>&amp;200%</source>
        <translation>&amp;200&amp;</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation>&amp;Миниатюры</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Показывать &amp;поля</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Показывать &amp;обрамление</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Показывать &amp;изображения</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Показывать &amp;страничную сетку</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Показывать &amp;направляющие</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Показывать &amp;опорную сетку</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>Показывать связь между &amp;блоками</translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation>Показывать спецсимволы</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Привязать к &amp;страничной сетке</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Привязать к н&amp;аправляющим</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Свойства</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>&amp;Запасник</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>С&amp;лои</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Закладки</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>&amp;Измерения</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>История &amp;действий</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation>&amp;Валидатор</translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation>В&amp;ыравнивание и распределение</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>С&amp;ервис</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>PDF-&amp;инструменты</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation>Выбрать объект</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>&amp;Таблица</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Фигура</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Линия</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation>Линия &amp;от руки</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Повернуть объект</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Изменить масштаб</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>Приблизить</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>Отдалить</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Изменить содержимое блока</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Изменить текст...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Связаны текстовые блоки</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Разъединены тестовые блоки</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation>&amp;Пипетка</translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation>Скопированы свойства объекта</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Текст изменён в Story Editor</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Добавить тестовый блок</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>Добавить изображение</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Добавить таблицу</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>Добавить фигуру</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>Добавить многоугольник</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>Добавить линию</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>Добавить кривую Безье</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Добавить линию от руки</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>Настроить &amp;изображения</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>Вставить &amp;переносы</translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>До&amp;бавить указатель оглавления</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>О &amp;Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>О &amp;Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>&amp;Подсказки</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>Ру&amp;ководство по Scribus...</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation>&amp;Умные переносы</translation>
    </message>
    <message>
        <source>Non Breaking Dash</source>
        <translation>Неразрывный дефис</translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation>&amp;Неразрывный пробел</translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>&amp;Номер страницы</translation>
    </message>
    <message>
        <source>New Line</source>
        <translation>Новая линия</translation>
    </message>
    <message>
        <source>Frame Break</source>
        <translation>Разрыв блока</translation>
    </message>
    <message>
        <source>Column Break</source>
        <translation>Разрыв столбца</translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>Копирайт</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation>Зарегистрированная торговая марка</translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation>Торговая марка</translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation>Маркер списка</translation>
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
        <translation>Апостроф</translation>
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
        <translation>Переключить отображение палитр</translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation>Переключить отображение направляющих</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation>&amp;Палитра страниц</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation>Убрать переносы из текста</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation>Настроить страницу...</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Линейки относительно страницы</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>Просмотреть пе&amp;чать</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation>&amp;JavaScripts...</translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation>Преобразовать в мастер-страницу...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Каскадом</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Мозаикой</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation>О &amp;модулях</translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation>Подробнее...</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation>Выравнивание и распределение</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Выравнивание</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation>&amp;Относительно:</translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation>Первого выделенного</translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation>Последнего выделенного</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Страницы</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Полей</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Выделенного</translation>
    </message>
    <message>
        <source>Align right sides of objects to left side of anchor</source>
        <translation>Выровнять правые края объектов по левой стороне опорной точки</translation>
    </message>
    <message>
        <source>Align left sides of objects to right side of anchor</source>
        <translation>Выровнять левые края объектов по правой стороне опорной точки</translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation>Выровнять по нижним краям</translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation>Выровнять по правым краям</translation>
    </message>
    <message>
        <source>Align tops of objects to bottom of anchor</source>
        <translation>Выровнять верхние края объектов по нижнему краю опорной точки</translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation>Центрировать по вертикальной оси</translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation>Выровнять по левым краям</translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation>Центрировать по горизонтальной оси</translation>
    </message>
    <message>
        <source>Align bottoms of objects to top of anchor</source>
        <translation>Выровнять нижние края объектов по верхнему краю опорной точки</translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation>Выровнять по верхним краям</translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation>Распределение</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal</source>
        <translation>Сделать горизонтальные интервалы между объектами одинаковыми</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation>Сделать горизонтальные интервалы между объектами равными указанному значению</translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation>Равноудалённо расставить правые края объектов</translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation>Равноудалённо расставить нижние края объектов</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation>Равноудалённо расставить центры объектов по горизонтали</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal</source>
        <translation>Сделать вертикальные интервалы между объектами одинаковыми</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation>Сделать вертикальные интервалы между объектами равными указанному значению</translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation>Равноудалённо расставить левые края объектов</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation>Равноудалённо расставить центры объектов по вертикали</translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation>Равноудалённо расставить верхние края объектов</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>&amp;Расстояние:</translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation>Распределить по указанному расстоянию</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Некоторые выделенные объекты заблокированы.</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>&amp;Разблокировать все</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Направляющей</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation>&amp;Направляющая:</translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation>Ничего не выбрано</translation>
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
        <translation>Выключка по левой стороне</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Выключка по правой стороне</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Выключка по центру</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Выключка по ширине</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Принудительная выключка по ширине</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Свойства поля</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Тип:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Кнопка</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Текстовое поле</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Флажок</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Раскрывающийся список</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Список</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Свойства</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Имя:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Подсказка:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Граница</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Цвет:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Не задано</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Ширина:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Тонкая</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Нормальная</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Широкая</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Стиль:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Сплошная</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Пунктирная</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Подчёркнутая</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>С фаской</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Втяжка</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Другое</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Только для чтения</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Необходимо</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Не экспортировать значение</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Видимость:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Видимо</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Скрыто</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Не  печатается</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Не видно на экране</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Внешний вид</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Текст нажатой кнопки</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Текст для Roll Over</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Пиктограммы</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Использовать пиктограммы</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Нажато</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Roll Over</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Расположение...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>В фокусе</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Инвертировано</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Обведено</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Нажато</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Многострочное</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Пароль</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Символов не более</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Символы</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Без прокрутки</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Без проверки орфографии</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Стиль флажка:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Галочка</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Крестик</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Ромб</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Круг</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Звезда</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Квадрат</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>По умолчанию включён</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Редактируем</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Параметры</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Перейти</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Отправить форму</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Сбросить введённые данные</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Импортировать данные</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Событие:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Mouse Up</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Mouse Down</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Mouse Enter</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Mouse Exit</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>On Focus</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>On Blur</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Сценарий:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Изменить...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Передать на URL</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Переслать данные как HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Откуда взять данные:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Цель</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>В файл:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Изменить...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Страница:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-Поз:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-Поз:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Действие</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Формат поля:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Общий</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Числовой</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Проценты</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Дата</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Время</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Свой</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Числовой формат</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Цифр в дробной части:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Использовать символ валюты</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Знак валюты перед числом</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Формат</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Процентный формат</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Формат даты</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Формат времени</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Другие сценарии</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Формат:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Комбинация клавиш:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Формат</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Значение не проверено</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Значение должно быть больше или равно:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>и меньше или равно:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Свой сценарий проверки:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Проверка</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Значение не вычислено</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Значение является</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>суммой</translation>
    </message>
    <message>
        <source>product</source>
        <translation>произведением</translation>
    </message>
    <message>
        <source>average</source>
        <translation>средним арифметическим</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>минимальным из</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>максимальным из</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>следующих полей:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Выбрать</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Свой сценарий для вычисления:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Вычисления</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отменить</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Введите разделённый запятыми список полей</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Для использования пиктограмм в кнопках необходима, 
как минимум, пиктограмма для Normal</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Открыть</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Пример:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Изменение выделения</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Шрифт для использования с PDF 1.3:</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Флажок игнорируется для PDF 1.3
</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF-документы (*.pdf);;Все файлы (*)</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation>JavaScript</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation>Изображения (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;Все файлы (*)</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Свойства аннотации</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Ссылка</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Внешняя ссылка</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Внешняя веб-ссылка</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Цель</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Открыть</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF-документы (*.pdf);;Все файлы (*)</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Тип:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Из&amp;менить...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>С&amp;траница:</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X-Поз.</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Поз:</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <source>Normal</source>
        <translation>Обычная</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Применение мастер-страницы</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Мастер-страница:</translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>К чему применить</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>&amp;Текущая страница</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+т</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>&amp;Чётные страницы</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+ч</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>&amp;Нечётные страницы</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+н</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Все страницы</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+в</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation>Страницы &amp;диапазона</translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation>Alt+д</translation>
    </message>
    <message>
        <source>to</source>
        <translation>до</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+О</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+т</translation>
    </message>
    <message>
        <source>Apply the selected template to even, odd or all pages within the following range</source>
        <translation>Применить выбранный шаблон к чётным, нечётным или всем страницам следующего диапазона</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>Запасник</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>Запасники (*.scs);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Объект</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Новая запись</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Переименовать</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новый</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Открыть...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Сохранить</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Сохранить &amp;как...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Закрыть</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>
&amp;Небольшой</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>&amp;Средний</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>&amp;Большой</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Файл</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Предпросмотр</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Имя:</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation>Имя &quot;%1&quot; уже используется.
Выберите другое.</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>Закладки</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>Переместить закладку</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Вставить закладку</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отменить</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Закладки</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Расположение</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Способ:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Только название</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Только пиктограмма</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Название под пиктограммой</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Название над пиктограммой</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Название справа от пиктограммы</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Название слева от пиктограммы</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Название поверх пиктограммы</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Масштабировать:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Всегда</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Когда пиктограмма слишком мала</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Когда пиктограмма слишком велика</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Никогда</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Как масштабировать:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Пропорционально</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Непропорционально</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Пикт.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отменить</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Сбросить</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>System Profiles</source>
        <translation>Системные профили</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Типы вывода</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Перцепционный</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Относительно колориметрический</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Насыщенность</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Абсолютно колориметрический</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Стандартный цветовой профиль 
для сплошных тонов на странице</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Цветовой профиль, созданный вами или полученный 
от производителя устройства. Этот должен быть 
профиль для конкретного монитора, а не общий 
профиль  (например, sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Цветовой профиль для вашего принтера, созданный 
его производителем. Этот профиль дожен быть 
уникальным для вашего принтера, а не общим 
профилем  (например, sRGB).</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Компенсация чёрной точки -- это метод улучшения 
контрастности фотографий. Его рекомендуется 
использовать при наличии фотографий в документе.</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Стандартный тип рендеринга для вашего монитора. 
Если вы не знаете, что это такое, выберите 
относительно колориметрический или перцептуальный.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Стандартный тип рендеринга для вашего принтера. 
Если вы не знаете, что это такое, выберите 
относительно колориметрический или перцептуальный.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Включить мягкий режим цветопробы вашего 
документа на основе выбранного профиля для принтера.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Способ отображения на экране тех цветов, которые 
могут быть некорректно напечатаны. Он требует очень 
точных профилей и предназначен лишь для предупреждения.</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Включить управление цветом</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>&amp;Сплошные тона:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Монитор:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>&amp;Принтер:</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>М&amp;онитор:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>Пр&amp;интер:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>&amp;Эмулировать принтер на экране</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Пометить цвета вне &amp;гаммы</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Использовать компенсацию &amp;чёрной точки</translation>
    </message>
    <message>
        <source>&amp;RGB Pictures:</source>
        <translation>&amp;RGB изображения:</translation>
    </message>
    <message>
        <source>&amp;CMYK Pictures:</source>
        <translation>&amp;CMYK изображения:</translation>
    </message>
    <message>
        <source>Default color profile for imported CMYK images</source>
        <translation>Используемый по умолчанию профиль для CMYK-изображений</translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation>Используемый по умолчанию профиль для RGB-изображений</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Изменить цвет</translation>
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
        <translation>RGB для web</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Новый</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Старый</translation>
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
        <source>Dynamic Color Bars</source>
        <translation>Динамические цветовые полосы</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Статические цветовые полосы</translation>
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
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>Карта цветов HSV</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Имя:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>Цветовая &amp;модель</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation>Это плашечный цвет</translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation>Это цвет для приводки</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation>Нельзя создать цвет с именем &quot;%1&quot;.
Он уже зарезервирован для прозрачного цвета</translation>
    </message>
    <message>
        <source>Name of the color is not unique</source>
        <translation>Это имя цвета уже используется</translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation>Нажмите и удерживайте нажатой правую клавишу мыши для увеличенного просмотра символа. «Вставить» добавляет выбранный символ в строку ниже, а «Удалить» убирает последний добавленный 
туда символ</translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation>Выбор символа:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Шрифтовый файл:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation>Класс символов:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>В&amp;ставить</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>О&amp;чистить</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Закрыть</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Вставить символы под курсор в тексте</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Удалить текущее выделение(ия).</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation>Закрыть это диалог и вернуться к правке текста.</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation>Полный набор символов</translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation>Базовая латиница</translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation>Дополнение к Latin-1</translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation>Латиница расширенная A</translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation>Латиница расширенная B</translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation>Основные знаки пунктуации</translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation>Верхний и нижний индексы</translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation>Символы валют</translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation>Буквоподобные символы</translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation>Дроби</translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>Стрелки</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation>Математические операторы</translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation>Псевдографика</translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation>Блочные элементы</translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation>Геометрические фигуры</translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation>Разные символы</translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation>Звёздочки</translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation>Лигатуры</translation>
    </message>
    <message>
        <source>Specials</source>
        <translation>Спецсимволы</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Греческий алфавит</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation>Греческий расширенный</translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation>Кириллица</translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation>Дополнение к кириллице</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Арабский</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation>Арабский расширенный А</translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation>Арабский расширенный B</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>Иврит</translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Current Profile:</source>
        <translation>Текущий профиль:</translation>
    </message>
    <message>
        <source>Items</source>
        <translation>Объекты</translation>
    </message>
    <message>
        <source>Problems</source>
        <translation>Проблемы</translation>
    </message>
    <message>
        <source>Glyphs missing</source>
        <translation>Отсутствующие символы</translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation>Переполнение текст. блока</translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation>Объект вне страницы</translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>Отсутствует изображение</translation>
    </message>
    <message>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation>В изображении точек на дюйм меньше %1</translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation>Объект полупрозрачен</translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation>Объект является встроенным PDF</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Документ</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>Проблем не выявлено</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Страница</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Свободные объекты</translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation>Найдена проблема</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Валидатор</translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation>Объект является PDF-аннотацией или полем</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation>&amp;Игнорировать ошибки</translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation>Выберите стиль</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Доступные стили</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation>Выберите каталог</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation>Идёт сборка...</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation>Невозможно собрать все файлы для вывода файла:
%1</translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation>Невозможно собрать для вывода следующий файл: 
%1</translation>
    </message>
</context>
<context>
    <name>ColorListBox</name>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <source>Colors</source>
        <translation>Цвета</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Импортировать</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новый</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Изменить</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Проду&amp;блировать</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Удалить</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>Удалить &amp;неиспользуемые</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Наборы цветов</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Текущий набор цветов:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>Со&amp;хранить набор цветов</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Выберите загружаемый набор цветов </translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Сохранить текущий набор цветов</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Удалить неиспользуемые цвета из 
текущего набора цветов в документе</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation>Импортировать цвета в набор для текущего документа</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Создать новый цвет для текущего набора</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Изменить выбранный цвет</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Сделать копию выбранных цветов</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Удалить выбранные цвета</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Сделать текущий набор цветов стандартным</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Имя:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Выберите имя</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Открыть</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Документы (*.sla *.sla.gz *.scd *.scd.gz);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Документы (*.sla *.scd);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Копия %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Новый цвет</translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <source>Monochromatic</source>
        <translation>Монохроматический</translation>
    </message>
    <message>
        <source>Analogous</source>
        <translation>Близкие цвета</translation>
    </message>
    <message>
        <source>Complementary</source>
        <translation>Противоположный цвет</translation>
    </message>
    <message>
        <source>Split Complementary</source>
        <translation>Разделённые противоположные цвета</translation>
    </message>
    <message>
        <source>Triadic</source>
        <translation>Триады</translation>
    </message>
    <message>
        <source>Tetradic (Double Complementary)</source>
        <translation>Тетрады (двойные противоположные цвета)</translation>
    </message>
    <message>
        <source>Base Color</source>
        <translation>Базовый цвет</translation>
    </message>
    <message>
        <source>Monochromatic Light</source>
        <translation>Монохромный светлый</translation>
    </message>
    <message>
        <source>Monochromatic Dark</source>
        <translation>Монохромный тёмный</translation>
    </message>
    <message>
        <source>1st. Analogous</source>
        <translation>Первый близкий</translation>
    </message>
    <message>
        <source>2nd. Analogous</source>
        <translation>Второй близкий</translation>
    </message>
    <message>
        <source>1st. Split</source>
        <translation>Первое деление</translation>
    </message>
    <message>
        <source>2nd. Split</source>
        <translation>Второе деление</translation>
    </message>
    <message>
        <source>3rd. Split</source>
        <translation>Третье деление</translation>
    </message>
    <message>
        <source>4th. Split</source>
        <translation>Четвёртое деление</translation>
    </message>
    <message>
        <source>1st. Triadic</source>
        <translation>Первый триадный</translation>
    </message>
    <message>
        <source>2nd. Triadic</source>
        <translation>Второй триадный</translation>
    </message>
    <message>
        <source>1st. Tetradic (base opposite)</source>
        <translation>Первая тетрада (базовый противоположный)</translation>
    </message>
    <message>
        <source>2nd. Tetradic (angle)</source>
        <translation>Вторая тетрада (угол)</translation>
    </message>
    <message>
        <source>3rd. Tetradic (angle opposite)</source>
        <translation>Третья тетрада (противоположный угол)</translation>
    </message>
</context>
<context>
    <name>ColorWheelDialog</name>
    <message>
        <source>Color Wheel</source>
        <translation>Цветовой круг</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Цвет</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Имя</translation>
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
        <translation>Выберите метод:</translation>
    </message>
    <message>
        <source>Angle (0 - 90 degrees):</source>
        <translation>Угол (0 - 90 градусов):</translation>
    </message>
    <message>
        <source>&amp;Merge Colors</source>
        <translation>&amp;Объединить цвета</translation>
    </message>
    <message>
        <source>&amp;Replace Colors</source>
        <translation>&amp;Заменить цвета</translation>
    </message>
    <message>
        <source>Merge created colors into the document colors</source>
        <translation>Внести созданные цвета в список доступных для документа</translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation>Заменить созданными цветами все цвета, доступные для документа</translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation>Оставить цвета нетронутыми</translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation>Объединяются цвета</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation>Ошибка: </translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation>Сейчас откроется диалог управления цветами.</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation>Объединение цветов</translation>
    </message>
    <message>
        <source>Normal Vision</source>
        <translation>Нормальное зрение</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation>Полное неразличение цветов</translation>
    </message>
    <message>
        <source>Vision Defect:</source>
        <translation>Дефект зрения:</translation>
    </message>
    <message>
        <source>Cr&amp;eate color...</source>
        <translation>&amp;Создать цвет...</translation>
    </message>
    <message>
        <source>&amp;Import existing color...</source>
        <translation>&amp;Импортировать готовый цвет...</translation>
    </message>
    <message>
        <source>&amp;Merge colors</source>
        <translation>&amp;Объединить цвета</translation>
    </message>
    <message>
        <source>&amp;Replace colors</source>
        <translation>&amp;Заменить цвета</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>В&amp;ыйти</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>Ц&amp;вет</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation>Разница между выбранным значением и расчитанными значениями. Подробнее см. в документации.</translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation>Щёлкните по колесу для получения основного цвета, каковым будет тон в режиме HSV.</translation>
    </message>
    <message>
        <source>Sample color scheme</source>
        <translation>Получаемая цветовая схема</translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation>Выберите один из способов создания цветовой схемы. Подробнее см. документацию.</translation>
    </message>
    <message>
        <source>Colors of your chosen color scheme</source>
        <translation>Цвета выбранной схемы</translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation>Можно эмулировать различные дефекты зрения. Выберите подходящий из списка.</translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation>Цвет %1 уже существует!</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation>Цвет %1 добавлен.</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Новый цвет</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation>Невозможно найти запрошенный цвет. Вероятно, был запрошен  чёрный, серый илибелый цвет. Ни один из них невозможно обработать.</translation>
    </message>
    <message>
        <source>C&amp;olor Components...</source>
        <translation>&amp;Компоненты цвета...</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation>Протанопия (красный)</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation>Дейтеронопия (зелёный)</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation>Тританопия (синий)</translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation>&amp;Цветовой круг...</translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation>Помощник в установке цвета</translation>
    </message>
    <message>
        <source>Color selector with color theory included.</source>
        <translation>Выбор цвета с учётом дефектов зрения</translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Применить</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Сохранить</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Горизонтальный градиент</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Вертикальный градиент</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Диагональный градиент</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Перекрёстно-диагональный градиент</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Радиальный градиент</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Непрозрачность:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Тень:</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Изменить свойства цвета линии</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Изменить свойства заливки линии</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Насыщение цвета</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Обычная или градиентная заливка</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Установить прозрачность для выбранного цвета</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Линейный градиент</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Радиальный градиент</translation>
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
        <translation>pt</translation>
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
        <source>Move Vector</source>
        <translation>Сместь вектор направления</translation>
    </message>
    <message>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation>Изменить вектор направления градиента: начало - левой кнопкой мыши, конец - правой</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Параметры импорта CSV</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Разделитель полей:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Разделитель значений:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Первая строка как заголовок</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отменить</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Параметры печати</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Параметр</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Значение</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Набор страниц</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Все страницы</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Только чётные</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Только нечётные</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Зеркалирование</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Нет</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Ориентация</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Портрет</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Альбом</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Страниц на лист</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>страница на лист</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>страниц на лист</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation>Здесь отображаются различные параметры CUPS для печати. Состав параметров зависит от используемой модели принтера. наличие поддержки можно проверить в диалоге &quot;О Scribus&quot;, вызываемом через меню &quot;Справка&quot;. Найдите сокращение вида &quot;C-C-T&quot;, которое расшифровывается так: C=CUPS C=littlecms T=TIFF. Отсутствующая библиотека помечается символом *</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>Encoding:</source>
        <translation>Кодировка:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Перемещается в каталог с документами.
Это можно изменить через диалог общих настроек.</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Сжать файл</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Включить шрифты</translation>
    </message>
</context>
<context>
    <name>CwSetColor</name>
    <message>
        <source>Set Color Components</source>
        <translation>Установка компонентов цвета</translation>
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
        <translation>Установить &amp;RGB</translation>
    </message>
    <message>
        <source>Set C&amp;MYK</source>
        <translation>Установить C&amp;MYK</translation>
    </message>
    <message>
        <source>Set &amp;HSV</source>
        <translation>Установить &amp;HSV</translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <source>Cancelled by user</source>
        <translation>Отменено пользователем</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Удаление цвета</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation>Удалить цвет:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Заменить на:</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Удаление страниц</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>до:</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation>Удалить от:</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation>Удаление стиля</translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation>Удалить стиль:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Заменить на:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Без стиля</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Информация о документе</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Заголовок:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Автор:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Ключевые слова:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>Опи&amp;сание:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>Изда&amp;тель:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>Соа&amp;вторы:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>Д&amp;ата:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>&amp;Тип:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>Фор&amp;мат:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>Идентиф&amp;икатор:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Источник:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>Я&amp;зык:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>О&amp;тношение:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>&amp;Cфера действия:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>Пра&amp;ва:</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>&amp;Подробнее</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Человек или организация, ответственные за выпуск этого документа</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Человек или организация, внесшие вклад в содержимое этого документа</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Дата, связанная с событием в жизненном цикле этого документа, в формате ГГГГ-ММ-ДД согласно ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Природа жанра содержимого этого документа, например, категории, функции и т.д.</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Уникальная ссылка на этот документ посредством ISBN или URI</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Ссылка на документ с родственной тематикой, возможно, с формальным идентификатором, таким как ISBN или URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Область охвата данного документа, возможно, включая место, время и область юрисдикции</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Информация о правах на документ и его содержимое, например, авторские и патентные права, торговая марка</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Ссылка на документ, производным от которого является данный документ, например, в виде ISBN или URI</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation>&amp;Документ</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Название организации, в первую очередь ответственной за создание этого документа. Это поле может быть встроено как в документ Scribus для ссылок на него, так и в метаданные PDF</translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Имя, данное документу. Это поле может быть встроено как в документ Scribus для ссылок на него, так и в метаданные PDF</translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Краткое описание содержимого документа. Это поле для краткого описания или аннотации к документу, встраиваемое в экспортируемый файл PDF</translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Тема этого документа. Это поле содержит ключевые слова документа для встраивания в файл PDF для упрощения поиска и индексирования таких файлов</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Физическое или цифровое представление этого документа. Стоит указать тип носителя и его размеры. Также полезны RFC2045 и RFC2046 для типов MIME</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Язык, на котором написан документ, обычно обозначенный кодом ISO-639, опционально с суффиксом в виде дефиса и кодом страны по ISO-3166, например: en-GB, fr-CH</translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation>Добавить раздел нумерации страниц в документ. Новый раздел будет начат после текущего выделения.</translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation>Удалить выделенный раздел</translation>
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
        <source>Name: Optional name for section eg. Index&lt;br/&gt;Shown: Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;From: The page index for this section to start at.&lt;br/&gt;To: The page index for this section to stop at.&lt;br/&gt;Style: Select the page number style to be used.&lt;br/&gt;Start: The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation>Разделы документа</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <source>From</source>
        <translation>От</translation>
    </message>
    <message>
        <source>To</source>
        <translation>До</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Стиль</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Начало</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Добавить</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+Д</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Удалить</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+У</translation>
    </message>
    <message>
        <source>Shown</source>
        <translation>Показано</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>Соотносится с</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Выше по иерерахии</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Ниже по иерархии</translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation>Текстовые блоки</translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation>Блоки изображений</translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation>Логическая</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>Переменная</translation>
    </message>
    <message>
        <source>String</source>
        <translation>Строковая</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation>Атрибуты объекта документа</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Значение</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Параметр</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Соотношение</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Соотношение с</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation>Автомат. добавить в</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Добавить</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+Д</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>С&amp;копировать</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+К</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Удалить</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+У</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>О&amp;чистить</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+Ч</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation>Настройка принтера</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Параметры</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Все</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Сохранить как</translation>
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
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Black</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Вставить сюда разделённый запятыми список 
маркёров, где каждый маркёр может быть * для 
всех страниц, 1-5 для диапазона страниц или 
номером одиночной страницы.</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Путь к принтеру</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Параметры...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Файл:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Изменить...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>&amp;Альтернативная команда для принтера</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>&amp;Команда:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Диапазон</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Напечатать &amp;все</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Напечатать те&amp;кущую страницу</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Напечатать ди&amp;апазон</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>Ко&amp;личество копий:</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Напечатать</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation>Обычная печать</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation>Печать цветоделений</translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation>Печать в цвете, если возможно</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>Печать в оттенках серого</translation>
    </message>
    <message>
        <source>PostScript Level 1</source>
        <translation>PostScript Level 1</translation>
    </message>
    <message>
        <source>PostScript Level 2</source>
        <translation>PostScript Level 2</translation>
    </message>
    <message>
        <source>PostScript Level 3</source>
        <translation>PostScript Level 3</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Страница</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Зеркально отразить по горизонтали</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>Зеркально отразить по вертикали</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Указать размер носителя</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Цвет</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation>Применить вычитание из-под чёрного</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Преобразовать сплошные тона в печатные</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>Применить ICC-профили</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Дополнительные параметры</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation>Предпросмотр...</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>Указывает уровень PostScript.
Выбор Level 1 или 2 может привести к 
созданию огромных файлов</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>Файлы PostScript (*.ps);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation>Использовать альтернативный менеджер печати, например kprinter или gtklp, для расширенной настройки печати</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Способ удаления некоторых серых (ахроматических) тонов, составленных из голубой, желтой и пурпурной красок, и замены их на чёрный. UCR главным образом воздействует на нейтральные по цвету части изображения. Использование этого метода способно улучшить печать некоторых изображений, но каждый отдельный случай требует особого рассмотрения. При использовании этого способа также снижается вероятность избыточной яркости изображения.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Разрешить преобразование плашечных цветов в композитные. Если вы не собираетесь печатать плашечные цвета в типографии, то лучше оставьте опцию включённой.</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation>Встраивать ICC-профили в печатный поток при включённом управлении цветом</translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation>Напрямую указать размер носителя в PostScript-файле. Не рекомендуется использовать эту функция, пока этого не потребует принтер.</translation>
    </message>
    <message>
        <source>Failed to retrieve printer settings</source>
        <translation>Не удалось получить настройки принтера</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Импорт файла:
%1
не удался!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Критическая ошибка</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Правка стиля</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Символ</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Между строк</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Не задано</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Имя стиля абзаца</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Шрифт выделенного текста или объекта</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Кегль шрифта</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Цвет заливки текста</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Цвет контура текста</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Задать общую высоту буквицы в строках</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Отбивка перед абзацем</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Отбивка под абзацем</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Табуляторы и отступы</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Имя:</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Линии:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation>Расстояния</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Фиксированный интерлиньяж</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Автомат. между строк</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Выровнять по сетке базовой линии</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation>Капитель</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>Расстояние до текста:</translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation>Показать вид абзацного стиля</translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation>Определяет интервал между буквицей и текстом</translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation>Переключает показ текста с этим абзацным стилем</translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation>Это имя стиля уже используется</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Фон</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Редактор</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новый</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Открыть...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Сохранить &amp;как...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>Сохр&amp;анить и выйти</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>Вы&amp;йти без сохранения</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Отменить</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Пов&amp;торить</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Вырезать</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>С&amp;копировать</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Вст&amp;авить</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>О&amp;чистить</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>Полу&amp;чить имена полей</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Файл</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Правка</translation>
    </message>
    <message>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>JavaScripts (*.js);;Все файлы (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation>Эффекты для изображения</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Параметры:</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Цвет:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Затенение:</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation>Яркость:</translation>
    </message>
    <message>
        <source>Contrast:</source>
        <translation>Контраст:</translation>
    </message>
    <message>
        <source>Radius:</source>
        <translation>Радиус:</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation>Значение:</translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation>Постеризация:</translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation>Доступные эффекты</translation>
    </message>
    <message>
        <source>Blur</source>
        <translation>Размытие</translation>
    </message>
    <message>
        <source>Brightness</source>
        <translation>Яркость</translation>
    </message>
    <message>
        <source>Colorize</source>
        <translation>Колоризация</translation>
    </message>
    <message>
        <source>Contrast</source>
        <translation>Контраст</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>В оттенках серого</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Инвертация</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation>Постеризация</translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation>Усиление резкости</translation>
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
        <translation>Используемые эффекты</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отменить</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Все страницы</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Сменить каталог вывода</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Каталог вывода – каталог, в который помещаются изображения.
Файлы будут иметь имена вида «имядокумента-номерстраницы.расширение»</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Только текущую страницу</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Возможные форматы для экспорта</translation>
    </message>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Выберите каталог для экспорта</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Экспортировать как изображение</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Экспортировать в каталог:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Изменить...</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Параметры</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>&amp;Формат:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Качество:</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Разрешение:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Диапазон</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>&amp;Текущая страница</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>Д&amp;иапазон</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Экспортировать диапазон страниц</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Вставить сюда разделённый запятыми список 
маркёров, где каждый маркёр может быть * для 
всех страниц, 1-5 для диапазона страниц или 
номером одиночной страницы.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Экспортировать все страницы</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Разрешение изображений
Используйте 72 dpi для изображений, 
ориентированных на просмотр с экрана</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Качество изображений - 100% является 
наилучшим, 1% - наихудшим</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>Раз&amp;мер:</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Размер изображений. 100% - без изменений, 200% - в 2  раза больше и т.д.</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation>Расширенные свойства изображения</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Обычный</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Затемнение</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Осветление</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Тон</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Насыщенность</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Цвет</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation>Яркость</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Умножение</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Экран</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Растворение</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Перекрытие</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Жёсткий свет</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Мягкий свет</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Разница</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>Вычитание</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>Затемнение штрихов</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>Осветление штрихов</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation>Исключение</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Режим совмещения:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Непрозрачность:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Фон</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Слои</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation>Не использовать контуры</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Контуры</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Размер:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Название:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Без заголовка</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Автор:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Неизвестно</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Документ Scribus</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Разрешение:</translation>
    </message>
    <message>
        <source>DPI</source>
        <translation>DPI</translation>
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
        <source>Colorspace:</source>
        <translation>Цветовое пространство:</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>В оттенках серого</translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation>Некоторые шрифты из этого документа заменены на другие:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> был заменён на: </translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Available Fonts</source>
        <translation>Доступные шрифты</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Замена шрифтов</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Дополнительные пути</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Имя шрифта</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Замена</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Выберите каталог</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>&amp;Доступные шрифты</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>Замена &amp;шрифтов</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>Дополнительные &amp;пути</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>У&amp;далить</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Изменить...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>До&amp;бавить...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Удалить</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Имя шрифта</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Использовать шрифт</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation>Встраивать в:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Подмножество</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>Путь к файлу шрифта</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.</source>
        <translation>Пути к шрифтам могут быть установлены только через диалог настройки программы и только при отсутствии открытых документов. Закройте все документы, и в диалоге, открывающимся через меню Правка -&gt; Настроить..., укажите путь к шрифтам.</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Имя шрифтового файла</translation>
    </message>
    <message>
        <source>Doc</source>
        <comment>font preview</comment>
        <translation>В этом документе</translation>
    </message>
    <message>
        <source>Type</source>
        <comment>font preview</comment>
        <translation>Тип</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Подмножество</translation>
    </message>
    <message>
        <source>Access</source>
        <comment>font preview</comment>
        <translation>Доступ</translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Пользователь</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>Система</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <comment>font preview</comment>
        <translation>Просмотр шрифтов</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <comment>font preview</comment>
        <translation>Alt+О</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <comment>font preview</comment>
        <translation>Alt+з</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation>Добавить выбранный шрифт в Стиль (меню «Шрифт»)</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Закрыть этот диалог</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation>А ещё неплохо бы на зависть другим уметь красиво читать и писать</translation>
    </message>
    <message>
        <source>Font Size:</source>
        <translation>Кегль:</translation>
    </message>
    <message>
        <source>Quick Search: </source>
        <translation>Быстрый поиск: </translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Искать</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <comment>font preview</comment>
        <translation>&amp;Закрыть</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. E.g. &apos;bold&apos; shows all fonts with Bold in name. Searching is case insensitive.</source>
        <translation>По вводу сюда автоматически запускается поиск шрифта по имени. Например, по запросу &apos;bold&apos; будут осталвены все шрифтыс Bold в имени. Поиск нечувствителен к регистру.</translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation>Начать поиск</translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation>Кегль выбранного шрифта</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Добавить</translation>
    </message>
</context>
<context>
    <name>FontPreviewPlugin</name>
    <message>
        <source>&amp;Font Preview...</source>
        <translation>&amp;Просмотр шрифтов...</translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation>Диалог просмотра шрифтов</translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>Сортировка, поиск и просмотр доступных шрифтов.</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation>Замена шрифта</translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation>Исходный шрифт</translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation>Шрифт замены</translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation>Использовать эту замену всегда</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>Некоторые шрифты, используемые в этом документе, отсутствуют в вашей системе. Выберите замену для них. Нажав кнопку «Отмена», вы прекратите загрузку документа.</translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>Отмену замену шрифтов и прекратить загрузку документа.</translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>Постоянно производить такую замену шрифтов в будущем. При желании этоможено отключить в настройке шрифтов диалога настройки Scribus.</translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>Если нажать ОК, а затем сохранить, то эта замена станет постоянной для сохранённого документа.</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Положение:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Здесь можно, добавить, изменить или удалить опорные точки.</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Настройка направляющих</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Горизонтальные</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Вертикальные</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Поз:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Добавить</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>У&amp;далить</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Поз:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>До&amp;бавить...</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>Уда&amp;лить</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Блокировать направляющие</translation>
    </message>
    <message>
        <source>Rows and Columns - Automatic Guides</source>
        <translation>Строки и столбцы - Автоматические направляющие</translation>
    </message>
    <message>
        <source>&amp;Rows:</source>
        <translation>&amp;Строк:</translation>
    </message>
    <message>
        <source>C&amp;olumns:</source>
        <translation>&amp;Столбцов:</translation>
    </message>
    <message>
        <source>Row &amp;Gap</source>
        <translation>Интервал &amp;между строк</translation>
    </message>
    <message>
        <source>Colum&amp;n Gap</source>
        <translation>Интервал м&amp;ежду столбцов</translation>
    </message>
    <message>
        <source>Refer to:</source>
        <translation>По отношение к:</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>Стр&amp;аница</translation>
    </message>
    <message>
        <source>&amp;Margins</source>
        <translation>&amp;Полям</translation>
    </message>
    <message>
        <source>&amp;Selection</source>
        <translation>&amp;Выделенному</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Закрыть</translation>
    </message>
    <message>
        <source>&amp;Update</source>
        <translation>&amp;Обновить</translation>
    </message>
    <message>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation>Установить направляющие в документе. 
Диалог их настройки по-прежнему открыт, 
но изменения применяются сразу</translation>
    </message>
    <message>
        <source>&amp;Apply to All Pages</source>
        <translation>П&amp;рименить ко всем страницам</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Направляющая</translation>
    </message>
    <message>
        <source>Unit</source>
        <translation>Единица</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Просмотр</translation>
    </message>
    <message>
        <source>There is empty (0.0) guide already</source>
        <translation>Нулевая (0.0) направляющая уже существует</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Извините, но руководство недоступно! 
Посетите сайт http://docs.scribus.net 
для получения актуальной документации.</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Содержание</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Ссылка</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Электронная справка по Scribus</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;Содержание</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Искать</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation>Ис&amp;кать</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новый</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Удалить</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation>Удалить &amp;всё</translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation>&amp;Закладки</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>На&amp;печатать...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>В&amp;ыйти</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation>Строка поиска регистронезависима</translation>
    </message>
    <message>
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Искать</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Искомый термин:</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>Новая закладка</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Название новой закладки:</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Файл</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation>&amp;Найти...</translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation>Найти &amp;следующее совпадение</translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation>Найти &amp;предыдущее совпадение</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Правка</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation>&amp;Добавить закладку</translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>&amp;Удалить все</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Закладки</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Возможен перенос</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Принять</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Пропустить</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Вставка страницы</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Размер наименьшего переносимого слова.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Максимально допустимое количество переносов подряд.
Значение &quot;0&quot; аналогично бесконечному количеству переносов.</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>Я&amp;зык:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>На&amp;именьшее слово:</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Предлагать &amp;варианты переноса</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Автоматически вставлять по мере &amp;набора текста</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Диалоговое окно с вариантами каждого переноса будет 
появляться после вызова функции вставки переносов</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Вставлять переносы по мере набора текста</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>Переносов &amp;подряд не более:</translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <source>Image Info</source>
        <translation>Об изображении</translation>
    </message>
    <message>
        <source>General Info</source>
        <translation>Основная информация</translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation>Дата / Время:</translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation>Встроенный профиль:</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Нет</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation>Имя профиля:</translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation>Встроенные пути:</translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation>Слои:</translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation>Данные EXIF</translation>
    </message>
    <message>
        <source>Artist:</source>
        <translation>Исполнитель:</translation>
    </message>
    <message>
        <source>Comment:</source>
        <translation>Комментарий:</translation>
    </message>
    <message>
        <source>User Comment:</source>
        <translation>Пользовательский комментарий:</translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation>Модель камеры:</translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation>Производитель камеры:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Описание:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Копирайт:</translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation>Модель сканера:</translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation>Производитель сканера:</translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Импортировать &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>Imports EPS Files</source>
        <translation>Импортирует файлы EPS</translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Импортирует файлы EPS в текущий документ,
преобразовывая их данные в векторные объекты Scribus.</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Вставка страницы</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>перед страницей</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>после страницы</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>в конец</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Страниц(а)</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>В&amp;ставить</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Мастер-страница:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Формат страницы</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Формат:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Свой</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Ор&amp;иентация:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Портрет</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Альбом</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Ширина:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Высота:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Переместить объекты с их страницей</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation>Мастер-страницы</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Вставить таблицу</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation>Строк:</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>Столбцов:</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>New Script</source>
        <translation>Новый сценарий</translation>
    </message>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Изменить JavaScripts</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Изменить...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Добавить</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>У&amp;далить</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Закрыть</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Новый сценарий</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Нет</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Да</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation>Вы действительно хотите удалить этот сценарий?</translation>
    </message>
    <message>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation>Добавляет новый сценарий, предопределяет функцию с таким же именем. Если вы хотите исопльзовать этот сценарий как сценарий &quot;Открыть действие&quot;, убедитесь, что не поменяли имя функции.</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Action</source>
        <translation>Действие</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Комбинация</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Выберите комбинацию клавиш для действия</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
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
        <source>&amp;No Key</source>
        <translation>&amp;Без горячих клавиш</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>&amp;Собственная комбинация</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>&amp;Установить</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation>Загружаемые клавиатурные настройки</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation>&amp;Загрузить</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation>&amp;Импортировать...</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation>&amp;Экспортировать...</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>С&amp;бросить</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation>Доступные схемы горячих клавиш</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation>Применить выбранную схему горячих клавиш</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation>Импортировать схему горячих клавиш в текущую конфигурацию</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation>Экспортировать текущую схему горячих клавиш в файл</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation>Загрузить схему по умолчанию</translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation>Файлы с настройками клавиатуры (*.ksxml)</translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation>Эта комбинация уже использована</translation>
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
        <source>Layers</source>
        <translation>Слои</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Удалить слой</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>Вы хотите заодно удалить все объекты этого слоя?</translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation>Добавить новый слой</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Удалить слой</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Поднять слой</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Опустить слой</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Правка стилей линий</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Копия %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Новый стиль</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Открыть</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Документы (*.sla *.sla.gz *.scd *.scd.gz);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Документы (*.sla *.scd);;Все файлы (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новый</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Правка</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Проду&amp;блировать</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>У&amp;далить</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Сохранить</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Нет</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Да</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Импортировать</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation>Вы действительно хотите удалить этот стиль?</translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <source>All Files (*)</source>
        <translation>Все файлы (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation>Выберите шаблонный текст</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Автор:</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation>Больше:</translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation>XML-файл:</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Шаблонный текст</translation>
    </message>
    <message>
        <source>Paragraphs:</source>
        <translation>Абзацев:</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>ALT+О</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+Т</translation>
    </message>
</context>
<context>
    <name>MarginDialog</name>
    <message>
        <source>Manage Page Properties</source>
        <translation>Настройка свойств страницы</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Формат страницы</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Формат:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Свой</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Ор&amp;иентация:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Портрет</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Альбом</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Ширина:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Высота:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Переместить объекты с их страницей</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Направляющие полей</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Тип:</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>&amp;Bottom:</source>
        <translation>С&amp;низу:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>С&amp;верху:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>С&amp;права:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>С&amp;лева:</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Расстояние между направляющей верхнего поля и краем страницы</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Расстояние между направляющей нижнего поля и краем страницы</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Изнутри:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>Снару&amp;жи:</translation>
    </message>
    <message>
        <source>Preset Layouts:</source>
        <translation>Предустановки:</translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation>Использовать эти поля во всех страницах</translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation>Использовать новые настройки полей во всех страницах документа</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Расстояние между направляющей левого поля и краем страницы. Если страницы указаны как парные, пространство поля используется для получения правильных полей при брошюровке</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Расстояние между направляющей правого поля и краем страницы. Если страницы указаны как парные, пространство поля используется для получения правильных полей при брошюровке</translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation>Правка мастер-страниц</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation>Вы действительно хотите удалить эту мастер-страницу?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Нет</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Да</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Имя:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>Новая мастер-страница</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Копия %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Имя:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation>Новая мастер-страница</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Копия #%1 из </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation>Продублировать выбранную мастер-страницу</translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation>Удалить выбранную мастер-страницу</translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation>Добавить новую мастер-страницу</translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation>Импортировать мастер-страницу из другого документа</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Новая мастер-страница %1</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Многократное дублирование</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>Ко&amp;личество копий:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>Смещение по &amp;горизонтали:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>Смещение по &amp;вертикали:</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Расстояния</translation>
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
        <translation>ДX:</translation>
    </message>
    <message>
        <source>DY:</source>
        <translation>ДY:</translation>
    </message>
    <message>
        <source>Angle:</source>
        <translation>Угол:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Длина:</translation>
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
        <translation>Открыть</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Документы (*.sla *.sla.gz *.scd *.scd.gz);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Документы (*.sla *.scd);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Импортировать страницы</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation>из 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Создать страницы</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation>из %1</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation>Из &amp;документа:</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>Из&amp;менить...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>Импортировать &amp;страниц:</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Перед страницей</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>После страницы</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>В конец</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Импортировать</translation>
    </message>
    <message>
        <source>Import Master Page</source>
        <translation>Импортировать мастер-страницу</translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation>Импортировать &amp;мастер-страницу</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Вставить сюда разделённый запятыми список токенов, где каждый токен может быть * для всех страниц, 1-5 для диапазона страниц или номером одиночной страницы.</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>Шрифт отсутствует</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Шрифт %1 не установлен.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Использовать</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>вместо</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Перемещение страниц</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Скопировать страницу</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Переместить страницу(ы):</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation>Переместить страниц(у)</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Перед страницей</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>После страницы</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>В конец</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>До:</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation>Количество копий:</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>Свойства</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Геометрия</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Уровень</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Показать кривую</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Смещение от начала кривой:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Расстояние от кривой:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Расстояние до текста</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Входной профиль:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Тип рендеринга:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Перцепционный</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Относительно колориметрический</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Насыщенность</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Абсолютно колориметрический</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Левая точка</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Конечные точки</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Фацетное соединение</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Фасочное соединение</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Скруглённое соединение</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Плоская шляпка</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Квадратная шляпка</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Округлая шляпка</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Без стиля</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Кегль шрифта</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Межстрочное расстояние</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Очертание:</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Опорная точка:</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Линии ячейки</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Линия вверху</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Линия слева</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Линия справа</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Линия внизу</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Имя выбранного объекта</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Горизонтальное положение опорной точки</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Вертикальное положение опорной точки</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Ширина</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Высота</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Вращение объекта относительно опорной точки</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Точка, относительно которой вычисляются углы поворота</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Выберите верхний левый угол в качестве опорной точки</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Выберите верхний правый угол в качестве опорной точки</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Выберите нижний левый угол в качестве опорной точки</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Выберите нижний правый угол в качестве опорной точки</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Выберите центр в качестве опорной точки</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Перевернуть по горизонтали</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Перевернуть по вертикали</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>На уровень выше</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>На уровень ниже</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>На самый высокий уровень</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>На самый низкий уровень</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Заблокировать или разблокировать объект</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Заблокировать или разблокировать размер объекта</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Разрешить или запретить печать объекта</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Шрифт выделенного текста или объекта</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Изменяемая ширина символов</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Насыщенность цвета контура текста</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Насыщенность цвета заливки текста</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Стиль текущего абзаца</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Изменить настройки левой или конечной точек</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Шаблон линии</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Толщина линии</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Тип соединения линий</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Тип окончания линий</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Стиль линии текущего объекта</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Выбрать очертание рамки...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Изменить очертание рамки...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Указать радиус скругления углов</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Количество столбцов в текстовой рамке</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Расстояние между столбцами</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Расстояние между текстом и верхом рамки</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Расстояние между текстом и низом рамки</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Расстояние между текстом и левой стороной рамки</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Расстояние между текстом и правой стороной рамки</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Изменить табуляторы текстовой рамки...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Разрешить изображению иметь отличный от рамки размер</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Горизонтальное смещение изображения относительно рамки</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Вертикальное смещение изображения относительно рамки</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Изменить размер изображения по горизонтали</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Изменить размер изображения по вертикали</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Сохранить пропорции при масштабировании</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Уместить изображение в рамке</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Использовать пропорции изображения вместо пропорций рамки</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Сохранять соотношение сторон</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Исходный профиль для изображения</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Тип рендеринга изображения</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Свойства текста на контуре</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Отображает уровень объекта  относительно других.
Ноль обозначает, что объект находится в самом низу.</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Включить обтекание текста вокруг 
объектов более высоких уровней</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Переключиться между интервалом и шириной столбцов</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Ширина столбцов</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>О&amp;чертания</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Текст</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Изображение</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Линия</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Цвета</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Поз:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Поз:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Ширина:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Высота:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Поворот:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>Изменить &amp;очертания...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>За&amp;круглённые
углы:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>С&amp;толбцов:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Интервал:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>С&amp;верху:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>С&amp;низу:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>С&amp;лева:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>С&amp;права:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>Т&amp;абуляторы...</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>Текст о&amp;бтекает рамку</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Использовать &amp;площадку</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>&amp;Контурная линия</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>С&amp;тиль:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>Я&amp;зык:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Свободное масштабирование</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>X-М&amp;асшт.:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>Y-Мас&amp;шт.:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>&amp;Масштабировать до размера рамки</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>Пропорц&amp;ионально</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Базовая точка:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>&amp;Тип линии:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>То&amp;лщина линии:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>&amp;Края:</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Окончания:</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation>Использовать для расчёта обтекания 
текста площадку вместо контура рамки</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Использовать для расчёта обтекания текста вторую 
линию, исходно основанную на контуре рамки</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Язык для расстановки 
переносов в рамке</translation>
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
        <translation>Написание справа налево</translation>
    </message>
    <message>
        <source>Start Arrow:</source>
        <translation>Начальная стрелка:</translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation>Конечная стрелка:</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Фиксированный интерлиньяж</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Автомат. между строк</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Выровнять по сетке базовой линии</translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation>Текущий X-DPI: </translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation>Текущий Y-DPI: </translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Смещение символов от базовой линии</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Изменяемая высота символов</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Ручной трекинг</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Имя &quot;%1&quot; уже использовано.&lt;br/&gt;Выберите другое.</translation>
    </message>
    <message>
        <source>Color of text stroke. Only available with &quot;outline&quot; text decoration.</source>
        <translation>Цвет штриха текста. Возможно только при включённом эффекте «Контур».</translation>
    </message>
    <message>
        <source>Color of text fill. Only available with &quot;outline&quot; text decoration.</source>
        <translation>Цвет текста. Возможно только при включённом эффекте «Контур».</translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation>Изменить стиль</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Плоская шляпка</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Квадратная шляпка</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Округлая шляпка</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Фацетное соединение</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Фасочное соединение</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Скруглённое соединение</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Толщина линии:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source> pt </source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Целая линия</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Линейный пунктир</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Пунктир</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Тире-точка</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Тире-точка-точка</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Имя &quot;%1&quot; уже использовано.&lt;br/&gt;Выберите другое.</translation>
    </message>
</context>
<context>
    <name>MultiProgressDialogBase</name>
    <message>
        <source>Progress</source>
        <translation>Прогресс</translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation>Общий прогресс:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <source>My &amp;Plugin</source>
        <translation>&amp;Моё расширение</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation>Scribus - Моё расширение</translation>
    </message>
    <message>
        <source>The plugin worked!</source>
        <translation>Расширение работает!</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>New Document</source>
        <translation>Новый документ</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Формат страницы</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Портрет</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Альбом</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Направляющие полей</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Параметры</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Размер страницы документа, стандартный 
либо заданный пользователем</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Ориентация страниц документа</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Ширина страниц документа, настраиваемая в том случае, если 
выбраны пользовательские настройки размера страницы</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Настраиваемая высота страниц документа</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>Номер первой страницы в документе</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Единица измерения по умолчанию</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Автоматически создавать текстовые рамки 
при добавлении новых страниц</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Количество столбцов в автоматически 
создаваемых текстовых рамках</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Расстояние между автоматически 
создаваемыми столбцами</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>Раз&amp;мер:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Ор&amp;иентация:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Ширина:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Высота:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>&amp;Номер первой страницы:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>&amp;Единица измерения:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>&amp;Автосоздание текстовых рамок</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Интервал:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>С&amp;толбцов:</translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation>Больше не показывать этот диалог</translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation>Исходное кол-во страниц в документе</translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation>&amp;Количество страниц:</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Открыть</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation>&amp;Новый документ</translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation>Открыть &amp;существующий документ</translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation>Открыть &amp;недавний документ</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Созд&amp;ать из шаблона...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation>Загрузка документов с готовым макетом</translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation>Создать новый документ из готового шаблона (к примеру, для документов со строгим стилевым оформлением)</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Узлы</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Переместить узлы</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Переместить контрольные точки</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Добавить узлы</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Удалить узлы</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Восстановить контрольные точки</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Восстановить эту контрольную точку</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Абсолютные координаты</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Поз:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Поз:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>&amp;Изменить контурную линию</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>&amp;Восстановить 
контурную линию</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Завершить</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Переместить контрольные точки независимо</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Переместить контрольные точки симметрично</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Разомкнуть многоугольник или кривую Безье</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Замкнуть эту кривую Безье</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Зеркально отразить этот 
контур по горизонтали</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Зеркально отразить этот 
контур по вертикали</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Разрезать контур по 
горизонтали направо</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Разрезать контур по 
горизонтали направо</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Разрезать контур 
по вертикали вверх</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Разрезать контур 
по вертикали вниз</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Вращать контур против 
часовой стрелки</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Вращать контур по 
часовой стрелке</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation>Уменьшить контур на показываемые %</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Увеличить контур на показываемые %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Угол вращения</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation>% увеличения или уменьшения</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Включить режим правки контурной линии</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Восстановить исходную контурную линию</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation>Если включено, координаты отсчитываются относительно страницы, в противном случае - относительно объекта.</translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Импортировать  &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation>Импортирует файлы OpenOffice.org Draw</translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation>Импортирует большинство файлов OpenOffice.org Draw в текущий документ, преобразовывая их данные в векторные объекты Scribus.</translation>
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
        <source>Overwrite Paragraph Styles</source>
        <translation>Перезаписать абзацные стили</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Во включённом состоянии перезаписывает существующие стили текущего документа Scribus</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Объединить абзацные стили</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Объединить абзацные стили по атрибутам. В результате получается меньше стилей, но с теми же атрибутами, даже если исходные стили документа именуются иначе.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Добавить название документа в начало абзацного стиля</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Добавить имя документа в начало имени стиля в Scribus.</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Больше не спрашивать</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Сделать эти настройки исходными или больше не задавать вопросов при импорте OASIS OpenDocument.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation>Параметры импорта OpenDocument</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отменить</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <source>Scribus Document</source>
        <translation>Документ Scribus</translation>
    </message>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation>Документ Scribus 1.2.x</translation>
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
        <translation>Толщина линии</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
        <translation>Экспорт в PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>Вы&amp;вод в файл:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Изменить...</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation>&amp;Каждую страницу сохранить в отдельный файл</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Сохранить</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Сохранить как</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF-документы (*.pdf);;Все файлы (*)</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation>Экспортировать каждую страницу документа в отдельный PDF. Номера страниц добавляются автоматически. Это полезнее всего для коммерческой печати.</translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation>Сохранение в PDF</translation>
    </message>
    <message>
        <source>Exporting Master Pages:</source>
        <translation>Экспорт мастер-страниц:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation>Экспорт страниц:</translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation>Экспорт объектов текущей страницы:</translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Просмотр печати</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Все</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Включить отображение полупрозрачных объектов. 
Требует Ghostscript как минимум версии 7.07.</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Включить режим эмуляции красок CMYK вместо палитры RGB</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Включить/отключить красочную плиту C (Cyan)</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Включить/отключить красочную плиту M (Magenta)</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Включить/отключить красочную плиту Y (Yellow)</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Включить/отключить красочную плиту K (Black)</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>Сгладить &amp;текст</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Сгладить &amp;графику</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Показать &amp;полупрозрачность</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>Вычитание из-под &amp;чёрного</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>По&amp;казать CMYK</translation>
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
        <source>Separation Name</source>
        <translation>Имя сепарации</translation>
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
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Black</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation>Масштаб:</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Напечатать...</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation>Включить более качественное отображение текста при просмотре ценой замедления прорисовки. Работает только со шрифтами Type1</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Включить более качественное отображение шрифтов TrueType, Open Type Fonts, EPS, PDF и векторной графики при просмотре ценой замедления прорисовки</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Способ удаления некоторых серых (ахроматических) тонов, составленных из голубой, желтой и пурпурной красок, и замены их на черный. UCR главным образом воздействует на нейтральные по цвету части изображения. Использование этого метода способно улучшить печать некоторых изображений, но каждый отдельный случай требует особого рассмотрения. При использовании этого способа также снижается вероятность избыточной яркости изображения.</translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation>Меняет масштаб отображения страницы.</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Файл</translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Pages:</source>
        <translation>Обработка мастер-страницы:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation>Экспорт страниц:</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Изображение</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Линия</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Многоугольник</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Ломаная линия</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Контурный текст</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Копия</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>Соотносится с</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Выше по иерерахии</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Ниже по иерархии</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <source>Page Item Attributes</source>
        <translation>Атрибуты объекта страницы</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Значение</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Параметр</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Соотношение</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Соотношение с</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Добавить</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+Д</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>С&amp;копировать</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+К</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Удалить</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+У</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>О&amp;чистить</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+Ч</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>Page Layout</source>
        <translation>Макет страницы</translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation>Первая страница:</translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation>Разворот</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Средняя правая</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation>Перетащите страницы или мастер-страницы на корзину для их удаления</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation>Здесь ваши мастер-страницы. Для создания новой страницы перетащите мастер-страницу вниз на список страниц</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Палитра страниц</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation>Доступные мастер-страницы:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Страницы документа:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>%1 of %2</source>
        <translation>%1 из %2</translation>
    </message>
    <message>
        <source>%1 of %1</source>
        <translation>%1 из %1</translation>
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
        <translation>Результат</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Результаты поиска для:</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Просмотр</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Выбрать</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отменить</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Путь</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Стр</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Печать</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Статус</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Перейти</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Отсутствует</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Искать</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation>Отменить поиск</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Настройка изображений</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation>Scribus - Поиск изображений</translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation>Поиск не удался: %1</translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>Не найдено ни одного изображения с именем &quot;%1&quot;.</translation>
    </message>
    <message>
        <source>Select a base directory for search</source>
        <translation>Выберите базовый каталог для поиска</translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Сохранить как &amp;изображение...</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Экспорт в растровое изображение</translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation>Экспортирует выделенные страницы в растровые изображения.</translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation>Невозможно найти расширение</translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>неизвестная ошибка</translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation>Невозможно найти символ (%1)</translation>
    </message>
    <message>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation>Расширение: загружается %1</translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation>инициализация не удалась</translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>неизвестный тип модуля</translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>Модуль: %1 загружен</translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation>Модуль: %1 не удалось загрузить: %2</translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <source>Plugin Manager</source>
        <translation>Управление модулями</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation>Модуль</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation>Как запустить</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation>Загрузить его?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation>ID модуля</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Нет</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation>Для учёта этих изменений программу необходимо перезапустить.</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Свойства многоугольника</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>&amp;Углы:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Поворот:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Использовать &amp;коэффициент</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>Коэ&amp;ффициент:</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Количество углов в многоугольнике</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Градусы вращения многоугольников</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Использовать коэффициент для создания многоугольника 
с выпуклыми или вогнутыми сторонами</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Пример многоугольника</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation>Отрицательное значение сделает многоугольник вогнутым (в форме звезды), а положительное - выпуклым</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Общие</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Документ</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Направляющие</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Типографика</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Инструменты</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Запасник</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Отображение</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>GUI</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Пути</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Формат страницы</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Свой</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Портретная</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Альбомная</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Направляющие полей</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Автосохранение</translation>
    </message>
    <message>
        <source>min</source>
        <translation>мин</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Другие параметры</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Просмотр</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>Небольшой</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Средний</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Выберите каталог</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Внешние инструменты</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Инструмент обработки графики</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Печать</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Стандартный размер шрифта для меню и окон</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Стандартная единица измерения в работе</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Количество строк, прокручиваемых за один раз при помощи колеса мыши</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Количество недавно открытых документов, отображаемых в меню «Файл»</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Стандартный каталог для документов</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Стандартный каталог для сценариев</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Стандартный, либо настраиваемый размер страницы</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Стандартная ориентация страниц документа</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Ширина страниц документа, настраиваемая в том случае, если 
выбраны пользовательские настройки размера страницы</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Высота страниц документа, настраиваемая в том случае, если 
выбраны пользовательские настройки размера страницы</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Период времени между автоматическим сохранением</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Изменить размер миниатюры в запаснике</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Цвет бумаги</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Пометить пространство за полями цветом полей</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Указать стандартный масштаб вида страницы</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation>Путь к интерпретатору Ghostscript в системе</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Сглаживать текст для EPS и PDF при отображении на экране</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Сглаживать графику для EPS и PDF при отображении на экране</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Не показывать объекты вне полей на распечатанной 
странице или в экспортированном файле</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Сохранять содержимое запасника при каждом его изменении</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Тема:</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Смещение по колесу мыши:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>Сколько &amp;недавних документов помнить:</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>Доку&amp;менты:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Изменить...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>ICC-&amp;профили:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Из&amp;менить...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>С&amp;ценарии:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>Изм&amp;енить...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>Раз&amp;мер:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Ор&amp;иентация:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Ширина:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Высота:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>С&amp;низу:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>С&amp;верху:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>С&amp;права:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>С&amp;лева:</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Интервал:</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>Сохранять содержимое
при &amp;изменениях</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>Большой</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Показывать &amp;непечатаемую область цветом полей</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>Подстройка отображения страницы на &amp;экране</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Имя исполняемого файла:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Сгладить &amp;текст</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Сгладить &amp;графику</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>&amp;Имя исполняемого файла:</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation>Обрезать печатаемую область до &amp;полей</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Применить вычитание из-под &amp;чёрного</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Изменить...</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>Я&amp;зык:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation>Единицы измерения:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation>Отмена и повтор действий</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation>Сколько действий
запоминать</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Перенос слов</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Шрифты</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Управление цветом</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>Экспорт в PDF</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Горячие клавиши</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Вид страницы</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Цвет:</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+Ч</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation>Показывать изображения</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Показывать связь между текстовыми блоками</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Показывать обрамление блоков</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation>Монтажный стол</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation>Расширения</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Включить или отключить показ рамок блоков</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation>Включить или отключить показ изображений</translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation>&amp;Шаблоны документов:</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Атрибуты объекта документа</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Указатель оглавления и индексы</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation>Показывать спецсимволы</translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation>Всегда спрашивать перед заменой шрифтов при загрузке документа</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation>Просмотр изменений абзацных стилей «на лету»</translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation>Разное</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation>Дополнительный каталог с шаблонами</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation>Для подстройки отображения листа перетащите ползунок внизу в нужную позицию.</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Показывать в текстовых блоках непечатаемые символы вроде меток окончания абзаца</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation>Приложите к экрану линейку и перетащите ползунок до позиции, при которой деления на экрани и динейке полностью совпадают. После этого 100% масштаб документа будет соответствовать действительности</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Сколько пространства слева от листа использовать в качестве монтажного стола</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Сколько пространства справа от листа использовать в качестве монтажного стола</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Сколько пространства над листом использовать в качестве монтажного стола</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Сколько пространства под листом использовать в качестве монтажного стола</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Валидатор</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Линейки относительно страницы</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation>Пробел между страницами</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>По горизонтали:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>По вертикали:</translation>
    </message>
    <message>
        <source>dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Разрешение:</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation>Показывать заставку при старте</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Шаблонный текст</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation>Всегда использовать латинский шаблонный текст</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation>Количество абзацев:</translation>
    </message>
    <message>
        <source>Locate Ghostscript</source>
        <translation>Найти Ghostscript</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation>Найти графический редактор</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation>Интерпретатор PostScript</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation>Показывать ли связь текстовых блоков.</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation>Здесь определяется язык, используемый Scribus по умолчанию. Оставьте поле пустым, чтобы настройки брались из системного языкового окружения. Системные настройки можно также «обойти» специальным аргументом командной строки (см. вывод команды scribus --help)</translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation>Кегль шрифта в &amp;меню:</translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation>Кегль шрифта в &amp;палитрах:</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation>Выберите стиль отображения графических элементов интерфейса программы. Scribus использует темы KDE и Qt, если таковые доступны.</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation>Кегль шрифта для диалогов программы</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation>Стандартный каталог для поиска ICC-профилей. Пока открыт хотя бы документ, изменить эту настройку невозможно. По умолчанию Scribus ищет профили в соответствующих системных каталогах Mac OS X и Windows. В Linux и Unix такими каталогами являются $home/.color/icc,/usr/share/color/icc и /usr/local/share/color/icc </translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation>Если включить, через указанный промежуток времени Scribus будет сохранять копию документа в файл с расширением .bak</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation>Здесь указывается количество изменений, который помнит Scribus. Если указать 0, будут запоминаться абсолютно все действия.</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation>Местоположение исполняемого файла графического редактора. Если вы используете GIMP, рекомендуется использовать &apos;gimp-remote&apos; для открытия файлов в той же сессии GIMP.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Способ удаления некоторых серых (ахроматических) тонов, составленных из голубой, желтой и пурпурной красок, и замены их на черный. UCR главным образом воздействует на нейтральные по цвету части изображения. Использование этого метода способно улучшить печать некоторых изображений, но каждый отдельный случай требует особого рассмотрения. При использовании этого способа также снижается вероятность избыточной яркости изображения.</translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;Defaults</source>
        <translation>По у&amp;молчанию</translation>
    </message>
    <message>
        <source>Save...</source>
        <translation>Сохранить...</translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation>Сохранить настройки</translation>
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
        <translation>Перенести прежние настройки Scribus?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation>Вы хотите использовать настройки 
от предыдущей версии Scribus?</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Одиночная</translation>
    </message>
    <message>
        <source>Double sided</source>
        <translation>Разворот</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation>Левая страница</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Правая страница</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation>3-частная «гармошка»</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation>Средняя</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation>4-частная «гармошка»</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation>Средняя левая</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Средняя правая</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation>Не удалось открыть файл настроек &quot;%1&quot; для записи: %2</translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation>Запись в файл настроек &quot;%1&quot; не удалась: QIODevice status code %2</translation>
    </message>
    <message>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation>Не удалось открыть файл с настройками &quot;%1&quot;: %2</translation>
    </message>
    <message>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation>Не удалось прочитать XML-файл с настройками от &quot;%1&quot;: %2 в строке %3, столбце %4</translation>
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
        <source>Error Writing Preferences</source>
        <translation>Произошла ошибка при записи настроек</translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation>Не удалось записать настройки Scribus:&lt;br&gt;%1&lt;br&gt;Проверьте права на запись каталога и файла настроек и, а также доступное свободное место на диске.</translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation>Произошла ошибка при загрузке настроек</translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation>Не удалось загрузить настройки Scribus:&lt;br&gt;%1&lt;br&gt;Будут использованы настройки по умолчанию.</translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>Magazine</source>
        <translation>Журнал</translation>
    </message>
    <message>
        <source>Fibonacci</source>
        <translation>Фибоначчи</translation>
    </message>
    <message>
        <source>Golden Mean</source>
        <translation>Золотая середина</translation>
    </message>
    <message>
        <source>Nine Parts</source>
        <translation>Девять долей</translation>
    </message>
    <message>
        <source>Gutenberg</source>
        <translation>Гутенберг</translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation>Здесь можно выбрать готовые настройки макета. «Ничего» - оставить поля как есть. «Гутенберг» устанавливает обычные книжные поля. «Журнал» устанавливает одинаковое значение для всех полей. Ведущим является значение Слева/Изнутри.</translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Открыть...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Сохранить</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation>В&amp;ыйти</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Файл</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation>&amp;Запустить</translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation>Сохранить &amp;вывод...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Сценарий</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation>Консоль для сценариев</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation>Напишите здесь свою команду. Выделенное обрабатывается как сценарий</translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation>Вывод вашего сценария</translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation>Сценарии на Python (*.py)</translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation>Сохранить файл с командами на Python</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation>Текстовые файлы (*.txt)</translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation>Сохранить текущий консольный вывод</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Сохранить &amp;как...</translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation>Запустить как к&amp;онсоль</translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation>Python-консоль Scribus</translation>
    </message>
    <message>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation>Это функционально урезанная в части пробелов версия Python-консоли. Подробности изложены в документации к Scribus.</translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation>Открыть файл сценария на Python</translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>&amp;Тон:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Насыщ.:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>&amp;Ярк.:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Красный:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Зелёный:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>&amp;Синий:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>&amp;Альфа-канал:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>&amp;Основные цвета</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Пользовательские цвета</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Добавить цвета &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отменить</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>До&amp;бавить в пользовательские цвета</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Выберите цвет</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Скопировать или переместить файл</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Прочитать: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Прочитать: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>&amp;Имя файла:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>&amp;Тип файла:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>На один каталог вверх</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отменить</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Все файлы (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Размер</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Дата</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Аттрибуты</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Посмотреть &amp;в:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Вернуться</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Создать новый каталог</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Список</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Таблица</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Информация о просматриваемом файле</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Содержимое просматриваемого файла</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Чтение-запись</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Только чтение</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Только запись</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Недоступен</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Символьная ссылка на файл</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Символьная ссылка на каталог</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Кат</translation>
    </message>
    <message>
        <source>Special</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Открыть</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Сохранить как</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Открыть</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Сохранить</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Переименовать</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>У&amp;далить</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>Пере&amp;загрузить</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Сортировать по &amp;имени</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Сортировать по &amp;размеру</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Сортировать по &amp;дате</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Без сортировки</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Сортировать</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Показать &amp;скрытые файлы</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>файл</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>каталог</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>символьная ссылка</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Удалить %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Вы действительно хотите удалить %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Да</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Нет</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Новый каталог 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Новый каталог</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Новый каталог %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Найти каталог</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Каталоги</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Сохранить</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Файл не найден.
Проверьте путь и имя файла.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Все файлы (*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Выберите каталог</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Каталог:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Шрифт</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>С&amp;тиль шрифта</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>Раз&amp;мер</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Эффекты</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>&amp;Вычеркнутый</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Подчёркнутый</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Цвет</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Пример</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>С&amp;ценарий</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Применить</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отменить</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Выберите новый шрифт</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Очистить</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Выделить всё</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Отменить</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Пов&amp;торить</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Вырезать</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>С&amp;копировать</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Вст&amp;авить</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Выстроить</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Настроить...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;О Qt&lt;/h3&gt;&lt;p&gt;Эта программа использует Qt версии %1.&lt;/p&gt;&lt;p&gt;Qt является инструментом разработки многоплатформенных приложений на C++ с графическим интерфейсом.&lt;/p&gt;&lt;p&gt;Qt обеспечивает собираемость приложений без несовместимого изменения исходного кода в MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux и основных вариантах коммерческих Unix-систем.&lt;br&gt;Также существует версия Qt лоя встраиваемых устройств.&lt;/p&gt;&lt;p&gt;Qt является продуктом компании Trolltech. Подробности вы можете узнать на сайте &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt;.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Вы действительно хотите перезаписать файл:
%1 ?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Открыть</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Сохранить как</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG-изображения (*.svg *.svgz);;Все файлы (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG-изображения (*.svg);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Нет</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Инициализация...</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Фон</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Ошибка при записи файла (-ов).</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Файл существует. Перезаписать?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>уже существует. Перезаписать его?</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Да, все</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Сохранить как изображение</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Экспорт прошёл успешно</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Все файлы (*)</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Бюллетени</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Буклеты</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Каталоги</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Флайеры</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Знаки</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Карточки</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Фирменные бланки</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Конверты</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Визитные карточки</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Календари</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Рекламные проспекты</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Этикетки</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Меню</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Программы</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>PDF-формы</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF-презентации</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Журналы</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Плакаты</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Объявления</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Текстовые документы</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Буклеты-гармошки</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Свои шаблоны</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Сценарии Scribus</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Выполнить сненарий...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Недавние сценарии</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Показать &amp;консоль</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Все поддерживаемые форматы (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Импортируется текст</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Все поддерживаемые форматы</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>Документы HTML</translation>
    </message>
    <message>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Текстовые документы</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Файлы со значениями, разделёнными запятой</translation>
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
        <source>Font %1 is broken, discarding it</source>
        <translation>Шрифт %1 повреждён и не будет использован</translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation>
Внешние ссылки
</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Текстовые фильтры</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Албанский</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Баскский</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Болгарский</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation>Португальский (Бразилия)</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Каталонский</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Китайский</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Чешский</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Датский</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Голландский</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Английский</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Английский (Англия)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Эсперанто</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Немецкий</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Финский</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Французский</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Галицийский</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Греческий</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Венгерский</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Индонезийский</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Итальянский</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Корейский</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Литовский</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>Норвежский (бокмол)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Норвежский (новонорвежский)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Норвежский</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Польский</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Русский</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Шведский</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Испанский</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Испанский (Латинская Америка)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Словацкий</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Словенский</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Сербский</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>О сц&amp;енарии...</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>О сценарии</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Невозможно получить цвет с пустым именем.</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Невозможно изменить цвет с пустым именем.</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Невозможно создать цвет с пустым именем.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Невозможно удалить цвет с пустым именем.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Невозможно заменить цвет с пустым именем.</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>Цель не является рамкой изображения.</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Невозможно получить кегль шрифта из нетекстовой рамки.</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Невозможно шрифт из нетекстовой рамки.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Невозможно получить текстовый кегль из нетекстовой рамки.</translation>
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
        <translation>Невозможно получить текст из нетекстовой рамки.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Невозможно вставить текст в нетекстовую рамку.</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation>OpenOffice.org Draw (*.sxd);;Все файлы (*)</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>Документы OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Свои настройки:</translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Обычные настройки:</translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Идёт обработка типографики для коротких слов.
Подождите, пожалуйста...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Обработка типографики для коротких слов завершена.</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Африкаанс</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>Турецкий</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>Украинский</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation>Уэльский</translation>
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
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Шрифт не найден.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
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
        <translation>Слой не найден.</translation>
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
        <source>Page</source>
        <translation>Страница</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Свой</translation>
    </message>
    <message>
        <source>Font %1 (found using fontconfig) is broken, discarding it</source>
        <translation>Шрифт %1 (найден при помощи fontconfig) поломан и не будет использован</translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation>Нестабильная версия Scribus</translation>
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
        <source>pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>mm</source>
        <translation> mm</translation>
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
        <source>Points (pt)</source>
        <translation>Точки (pt)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Дюймы (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Пики (p)</translation>
    </message>
    <message>
        <source>File exists</source>
        <translation>Файл с таким именем уже существует</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>За&amp;менить</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Все</translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation>Не удалось открыть документ.</translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation>Не удалось сохранить документ.</translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation>Свойство не найдено</translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation>Потомок не найден</translation>
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
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation>Невозможно сохранить растровое изображение</translation>
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
        <translation>Объект не найден.</translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation>Стиль не найден.</translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>Не удалось сохранить EPS.</translation>
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
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>С&amp;ценарии</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Хорватский</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Португальский</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Португальский (Бразилия)</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>«Обрушивание» Scribus</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus обрушился по причине #%1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation>Мастер-страница </translation>
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
        <translation>Не удалось открыть файл вывода %1</translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation>Поток вывода не открыт на запись</translation>
    </message>
    <message>
        <source>Verification of settings failed: %1</source>
        <translation>Верификация настроек не удалась: %1</translation>
    </message>
    <message>
        <source>Could not open input file %1</source>
        <translation>Не удалось открыть файл ввода %1</translation>
    </message>
    <message>
        <source>Unable to read settings XML:</source>
        <translation>Не удалось прочитать XML-файл с настройками:</translation>
    </message>
    <message>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation>%1 (строка %2 столбец %3)</translation>
    </message>
    <message>
        <source>Unable to read settings XML: %1</source>
        <translation>Не удалось прочитать XML-файл с настройками: %1</translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation>Некорректное значение в тэге &lt;pdfVersion&gt;</translation>
    </message>
    <message>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation>Найдено %1 &lt;%2&gt; узлов, а нужен только один.</translation>
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
        <translation>Библиотека Freetype2 недоступна</translation>
    </message>
    <message>
        <source>Font %1 is broken, no embedding</source>
        <translation>Файл шрифта %1 испорчен, встраивание невозможно</translation>
    </message>
    <message>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation>Файл шрифта %1 испорчен (чтение потока), встраивание невозможно</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation>Файл шрифта %1 испорчен (FreeType2), отказ от него</translation>
    </message>
    <message>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation>Файл шрифта %1 испорчен (нет гарнитуры), отказ от него</translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation>Файл шрифта %1 содержит испорченный наборный знак (код символа %3)</translation>
    </message>
    <message>
        <source>Font %1 is broken and will be discarded</source>
        <translation>Файл шрифта %1 испорчен, отказ от него</translation>
    </message>
    <message>
        <source>Font %1 cannot be read, no embedding</source>
        <translation>Файл шрифта %1 не может быть прочитан, отказ от встраивания</translation>
    </message>
    <message>
        <source>Failed to load font %1 - font type unknown</source>
        <translation>Не удалось загрузить файл шрифта %1 - неизвестный тип шрифта</translation>
    </message>
    <message>
        <source>Font %1 loaded from %2(%3)</source>
        <translation>Файл шрифта %1 загружен из %2(%3)</translation>
    </message>
    <message>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation>Файл шрифта %1 (%2) является копией %3</translation>
    </message>
    <message>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation>Загружается файл шрифта %1 (найден при помощи fontconfig)</translation>
    </message>
    <message>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation>Не удалось загрузить шрифт - freetype2 не смог найти файл шрифта</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType), discarding it</source>
        <translation>Файл шрифта %1 испорчен (FreeType), отказ от него</translation>
    </message>
    <message>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation>Файл шрифта %1  содержит некорректный наборный знак %2 (код символа %3), отказ от него</translation>
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
        <translation>таблица &apos;%1&apos;</translation>
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
        <source> cm</source>
        <translation> cm</translation>
    </message>
    <message>
        <source>cm</source>
        <translation>cm</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation>Миллиметры (mm)</translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation>Сантиметры (cm)</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation>Шаблон документа: </translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Цвет не найден.</translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>Цвет не найден в документе.</translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>Цвет не найден среди обычного набора цветов.</translation>
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
        <translation>Текстовые документы OpenDocument</translation>
    </message>
    <message>
        <source> c</source>
        <translation> c</translation>
    </message>
    <message>
        <source>c</source>
        <translation>c</translation>
    </message>
    <message>
        <source>Cicero (c)</source>
        <translation>Цицеро (с)</translation>
    </message>
    <message>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation>Имя файла не должно быть пустой строкой</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation>страницв</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Black</translation>
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
        <translation>Yellow</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Копия #%1 из </translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>Цветовой круг</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation>Просмотр шрифтов</translation>
    </message>
    <message>
        <source>My Plugin</source>
        <translation>Мой модуль</translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation>Новый документ из шаблона</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Экспортировать как изображение</translation>
    </message>
    <message>
        <source>PS/EPS Importer</source>
        <translation>Импортировать PS/EPS</translation>
    </message>
    <message>
        <source>Save As Template</source>
        <translation>Сохранить как шаблон</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Сценарист</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Короткие слова</translation>
    </message>
    <message>
        <source>SVG Export</source>
        <translation>Экспорт в SVG</translation>
    </message>
    <message>
        <source>SVG Import</source>
        <translation>Импорт SVG</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw Importer</source>
        <translation>Импорт OpenOffice.org Draw</translation>
    </message>
    <message>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation>Scribus упал в связи с ошибкой %1</translation>
    </message>
    <message>
        <source>Creating Font Cache</source>
        <translation>Создаётся шрифтовый кэш</translation>
    </message>
    <message>
        <source>New Font found, checking...</source>
        <translation>Найден новый шрифт, идёт проверка...</translation>
    </message>
    <message>
        <source>Modified Font found, checking...</source>
        <translation>Найден изменённый шрифт, идёт проверка...</translation>
    </message>
    <message>
        <source>Reading Font Cache</source>
        <translation>Читается шрифтовый кэш</translation>
    </message>
    <message>
        <source>Writing updated Font Cache</source>
        <translation>Пишется обновлённый шрифтовый кэш</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Идёт поиск шрифтов</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation>Вы работаете с нестабильной версией  Scribus 1.3.x. Текущий документ был создан в Scribus версии 1.2.3 или ниже. Если вы сейчас сохраните этот документ, работать с ним в Scribus 1.2.3 будет уже нельзя, если вы не сохраните его под другим именем. Вы точно хотите просто сохранить этот файл?</translation>
    </message>
    <message>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation>Вы не сохранили внесённые изменения и хотите отменить их все. Продолжить?</translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation>Файл с именем &apos;%1&apos; уже существует.&lt;br/&gt;Вы хотите заменить его тем, который сохраняете сейчас?</translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Old .sla format support</source>
        <translation>Поддержка старого формата .sla</translation>
    </message>
    <message>
        <source>German (Trad.)</source>
        <translation>Немецкий (Традиц.)</translation>
    </message>
    <message>
        <source>Exporting PostScript File</source>
        <translation>Идёт экспорт в файл PostScript</translation>
    </message>
    <message>
        <source>Printing File</source>
        <translation>Идёт печать в файл</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;p&gt;Вы пытаетесь импортировать больше страниц, чем есть в текущем документе, считая с текущей страницы.&lt;/p&gt;Выберите одно из следующих действий:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Создать&lt;/b&gt; недостающие страницы&lt;/li&gt;&lt;li&gt;&lt;b&gt;Импортировать&lt;/b&gt; страницы до последней&lt;/li&gt;&lt;li&gt;&lt;b&gt;Отменить&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>C&amp;reate</source>
        <translation>&amp;Создать</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Импортировать</translation>
    </message>
    <message>
        <source>Thai</source>
        <translation>Тайский</translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Очистить</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Выделить всё</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Отменить</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Пов&amp;торить</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Вырезать</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>С&amp;копировать</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Вст&amp;авить</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Системное меню</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Затенить</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Убрать тень</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Обычный вид</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Свернуть</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Развернуть</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Восстановить</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Переместить</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>Раз&amp;мер</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>&amp;Свернуть</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>&amp;Развернуть</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Закрыть</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Оставаться на&amp;верху</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Свернуть</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Восстановить вниз</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>За&amp;тенить</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>Убр&amp;ать тень</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Document Setup</source>
        <translation>Параметры документа</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Направляющие полей</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>С&amp;верху:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>С&amp;лева:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>С&amp;низу:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>С&amp;права:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Формат страницы</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Свой</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Портрет</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Альбом</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>Раз&amp;мер:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Ор&amp;иентация:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Ширина:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Высота:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation>&amp;Единица измерения:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Автосохранение</translation>
    </message>
    <message>
        <source>min</source>
        <translation>мин</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Интервал:</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Документ</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Направляющие</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Вид страницы</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Цвет:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Показывать &amp;непечатаемую область цветом полей</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+Н</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation>Показывать изображения</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Показывать связь между текстовыми блоками</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Показывать обрамление блоков</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Отображение</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Типографика</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Инструменты</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Перенос слов</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Шрифты</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>Экспорт в PDF</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Управление цветом</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Включить или отключить показ рамок блоков</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation>Включить или отключить показ изображений</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Цвет бумаги</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Пометить пространство за полями цветом полей</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation>Информация о документе</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation>Показывать спецсимволы</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Атрибуты объекта документа</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Указатель оглавления и индексы</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Показывать в текстовых блоках непечатаемые символы вроде меток окончания абзаца</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Линейки относительно страницы</translation>
    </message>
    <message>
        <source>Minimum Scratch Space</source>
        <translation>Минимальный рабочий стол</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation>Пробел между страницами</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>По горизонтали:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>По вертикали:</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Валидатор</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Идёт подстройка цветов</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation>Показывать ли связь текстовых блоков.</translation>
    </message>
    <message>
        <source>Apply size settings to all pages</source>
        <translation>Использовать эти настройки во всех страницах</translation>
    </message>
    <message>
        <source>Sections</source>
        <translation>Разделы</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation>Использовать новые настройки во всех страницах текущего документа</translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Сценарии на Python (*.py);; Все файлы (*)</translation>
    </message>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation>Запустить как сценарий расширения</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style Settings</source>
        <translation>Настройки стиля</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Стиль текущего абзаца</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Fill Color Settings</source>
        <translation>Настройки заливки цветом</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Цвет заливки текста</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Цветонасыщенность заливки текста</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Настройки цвета контура</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Цвет контура текста</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Цветонасыщенность контура текста</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source>Font Settings</source>
        <translation>Настройки шрифта</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Font of selected text</source>
        <translation>Шрифт выделенного текста</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Кегль шрифта</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Изменяемая ширина символов</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Изменяемая ширина высота символов</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Настройки символа</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Ручной трекинг</translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Сохран&amp;ить страницу как SVG...</translation>
    </message>
    <message>
        <source>Exports SVG Files</source>
        <translation>Экспортирует страницы в файлы SVG</translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation>Экспортирует текущую страницу в файл SVG.</translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Импортировать &amp;SVG...</translation>
    </message>
    <message>
        <source>Imports SVG Files</source>
        <translation>Импортирует файлы SVG</translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Импортирует большинство файлов SVG в текущий документ,
преобразовывая их векторные данные в объекты Scribus.</translation>
    </message>
    <message>
        <source>Scalable Vector Graphics</source>
        <translation>Масштабируемая векторная графика (SVG)</translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation>Файл в формате SVG содержит неподдерживаемые элементы разметки</translation>
    </message>
</context>
<context>
    <name>SWDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation>Короткие слова</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation>Вставить неразрывные пробелы:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>В &amp;выбранные рамки</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation>По всей &amp;странице</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>По всему &amp;документу</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Обработаны только выбранные рамки</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Обработана только текущая страница</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Обработан весь документ</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation>Пользовательские настройки</translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation>Общесистемные настройки</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Сохранить</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>С&amp;бросить</translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation>Сохранить пользовательские настройки</translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation>Снова загрузить общесистемные настройки и удалить пользовательские</translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation>Изменить пользовательские настройки. Если их сохранить, они будут использоваться вместо общесистемных</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Короткие слова</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation>Пользовательские настройки уже существуют. Вы точно хотите их перезаписать?</translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation>Невозможно записать в файл %1</translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation>Пользовательские настройки сохранены</translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation>Общесистемные настройки перезагружены</translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation>Невозможно открыть файл %1</translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Сохранить как &amp;шаблон...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation>Сохранение как шаблона</translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation>Сохраняет документ как шаблон. Это удобный способ придавать последующим документам единообразный вид</translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation>Постоянный</translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>Действие</translation>
    </message>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation>Загрузка/Сохранение/Импорт/Экспорт</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Неизвестно</translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <source>Printing...</source>
        <translation>Идёт вывод на печать...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Background</source>
        <translation>Фон</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Копия #%1 из </translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Document Colors</source>
        <translation>Цвета документа</translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <source>New Layer</source>
        <translation>Новый слой</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Документ</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Фон</translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Инициализируются модули</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation>Инициализируются горячие клавиши</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Читаются настройки</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation>Инициализируется Story Editor</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation>Читаются ICC-профили</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation>Инициализируется модуль переносов</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Читается запасник</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Устанавливаются горячие клавиши</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Правка</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Идёт поиск шрифтов</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation>В вашей системе нет шрифтов.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation>Завершается работа приложения.</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Критическая ошибка</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Шрифтовая подсистема инициализирована</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Файл</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>&amp;Недавние</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Импортировать</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Экспортировать</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Правка</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>Ст&amp;иль</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Цвет</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>Раз&amp;мер</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Оттенение</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Шрифт</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Эффекты</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Объект</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Настройки предпросмотра</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Уровень</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Перенести в сло&amp;й</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>Пара&amp;метры PDF</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>О&amp;чертание</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation>&amp;Преобразовать в</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation>Вст&amp;авка</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Символ</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Кавычки</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Пробел</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>Стр&amp;аница</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Вид</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>&amp;Расш.</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>О&amp;кна</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Справка</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Выключка</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Готово</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Открыть</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Идёт импорт страниц...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Импорт страниц(ы)</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Импорт завершён</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Данные для импорта не найдены</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation>Файл %1 в неподдерживаемом формате</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Файл загружается...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation>У вас нет некоторых ICC-профилей из этого документа:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> был заменён на: </translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation>(преобразовано)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Все поддерживаемые форматы</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Все файлы (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation>Невозможно записать в файл: 
%1</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Документы (*.sla *.sla.gz *.scd *.scd.gz);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Документы (*.sla *.scd);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Сохранить как</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Файл сохраняется...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus обнаружил ошибки. Вам стоит запустить валидатор, чтобы исправить их</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>Про&amp;игнорировать</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation>&amp;Прервать</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Идёт вывод на печать...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Документ</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Напечатать не удалась!</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation>Невозможно вырезать используемый объект</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation>Объект %1 сейчас открыт в Story Editor. Вырезание отменяется</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>О Qt</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Руководство по Scribus</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Сохранить как</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Текстовые файлы (*.txt);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Имя:</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation>Преобразовать страницу в мастер-страницу</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>Новая мастер-страница</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>Раз&amp;мер:</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Размер</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Без стиля</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation>Следующие программы отсутствуют:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation>Ghostscript : невозможно использовать изображения формата EPS или просмотривать печатную версию</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Все</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation>Обнаружены ошибки.
Запустите валидатор  для их устранения.</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>Файлы в EPS (*.eps);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation>Обнаружены ошибки.
Запустите валидатор  для их устранения</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation>-Страница%1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Некоторые объекты заблокированы.</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation>За&amp;блокировать все</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>&amp;Разблокировать все</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Информация</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation>Программа %1 уже запущена!</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation>Программа %1 отсутствует!</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation>Выбранный цвет отсутствует в цветовой палитре документа. Введите имя для этого нового цвета.</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation>Цвет не найден</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation>Выбранное вами имя уже существует. Введите другое имя для этого цвета.</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation>&amp;Уровень</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation>Перенести в слой</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation>Настройки предпрос&amp;мотра</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>С&amp;ервис</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-Поз:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-Поз:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation>Ghostscript : Вы не можете исопльзовать изображения в формате EPS или просматривать печать PostScript</translation>
    </message>
    <message>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation>Ghostscript отсутствует : просмотр печати Postscript недоступен</translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <source>Invalid argument: </source>
        <translation>Некорректный аргумент: </translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation>Файл %1 не существует, прерывание операции.</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation>Использование: scribus [ключ ... ] [файл]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Параметры:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation>Напечатать справку (это сообщение) и выйти</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation>Использует сокращение вида xx для обозначения языка, например, &quot;en&quot; или &quot;de&quot;</translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation>Вывести список установленных переводов интерфейса</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation>Показать информацию в консоли при загрузке шрифтов</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation>Не показывать заставку при старте</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation>Показать информацию о версии и выйти</translation>
    </message>
    <message>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation>Использовать обратный порядок кнопок в диалоге (например, Отменить/Нет/Да вместо Да/Нет/Отменить)</translation>
    </message>
    <message>
        <source>filename</source>
        <translation>имя_файла</translation>
    </message>
    <message>
        <source>Use filename as path for user given preferences</source>
        <translation>Использовать имя файла как путь к указанным пользователем настройкам</translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation>Установлены следующие переводы интерфейсы Scribus:</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation>Для «преодоления» системных настроек языкового окружения выберите:</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation>scribus -l xx или scribus --lang xx, где xx - избранный язык.</translation>
    </message>
    <message>
        <source>Scribus Version</source>
        <translation>Версия Scribus</translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation>Scribus, Предпечатная подготовка свободными средствами</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Веб-сайт</translation>
    </message>
    <message>
        <source>Documentation</source>
        <translation>Документация</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Вики</translation>
    </message>
    <message>
        <source>Issues</source>
        <translation>Проблемы</translation>
    </message>
</context>
<context>
    <name>ScribusView</name>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Layer</source>
        <translation>Слой</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>Скопировать сюда</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Переместить сюда</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отменить</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Вст&amp;авить</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Изображение</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Файл:</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Исходный PPI: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Текущий PPI: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Связанный текст</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Текстовая рамка</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Текст по контуру</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Абзацев: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Слов: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Символов: </translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Печать: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Включено</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Выключено</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>&amp;Инфо</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>Пара&amp;метры PDF</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Перенести в сло&amp;й</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>Уро&amp;вень</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>&amp;Преобразовать в</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Удалить</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Настройки предпросмотра</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation>Невозможно удалить используемый объект</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>Объект %1 сейчас изменяется в редакторе текста. Его невозможно удалить</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation>Связанные текстовые блоки</translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation>Вы пытаетесь перенести текст в уже заполненный блок или исходный блок.</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Вы действительно хотите удалить весь текст?</translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation>Невозможно преобразовать используемый объект</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>Объект %1 сейчас изменяется в редакторе текста. Его невозможно преобразовать в контуры</translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation>Страница %1 до %2</translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation>Цветовое пространство: </translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Неизвестно</translation>
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
        <translation>В оттенках серого</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>Document:</source>
        <translation>Документ:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>изменился с момента последнего сохранения.</translation>
    </message>
    <message>
        <source>&amp;Discard</source>
        <translation>От&amp;казаться</translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <source>Embedded Python scripting support.</source>
        <translation>Встроена поддержка сценариев на Python.</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Сценарист</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>Script error</source>
        <translation>Ошибка сценария</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Если вы пытались выполнить сценарий из стандартной поставки Scribus, 
сообщите об ошибке на &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;, пожалуйста.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Это сообщение скопировано в буфер обмена.
Нажмите Ctrl+V, чтобы вставить его в жалобную книгу.</translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation>Исследовать сценарий</translation>
    </message>
    <message>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation>Произошла внутренняя ошибка при выполнении заданной команды. Подробности переданы на stderr. </translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation>Установка модуля Python провалилась. Подробности об ошибке переданы на stderr. </translation>
    </message>
    <message>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation>Сценарии на Python (*.py);; Все файлы (*)</translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Scripter Preferences</source>
        <translation>Настройки сценариста</translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation>Включить сценарии расширения</translation>
    </message>
    <message>
        <source>Extensions</source>
        <translation>Расширения</translation>
    </message>
    <message>
        <source>Console</source>
        <translation>Консоль</translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation>Сценарий запуска:</translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation>Ошибки:</translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation>Комментарии:</translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation>Ключевые слова:</translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation>Знаки:</translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation>Числа:</translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation>Строки:</translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation>Простой текст:</translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation>Выберите цвет</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Просмотр  страниц</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Найти/Заменить</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Найти:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Стиль абзаца</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Шрифт</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Кегль шрифта</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Эффекты шрифта</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Цвет заливки</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Тень заливки</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Цвет контура</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Тень контура</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Влево</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>По центру</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Вправо</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>По ширине</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Принудительно</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Не задано</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Заменить на:</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Поиск окончен</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>Слово це&amp;ликом</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>Не у&amp;читывать регистр</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Искать</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>За&amp;менить</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Заменить &amp;все</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Закрыть</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>О&amp;чистить</translation>
    </message>
    <message>
        <source>Search finished, found %1 matches</source>
        <translation>Поиск завершён, найденных совпадений: %1</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Выбор полей</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Доступные поля</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>Выбранные поля</translation>
    </message>
    <message>
        <source>&amp;&gt;&gt;</source>
        <translation>&amp;&gt;&gt;</translation>
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
        <translation>Другое...</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Тень</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Тень:</translation>
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
        <translation>X-смещение</translation>
    </message>
    <message>
        <source>Y-Offset</source>
        <translation>Y-смещение</translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation>&amp;Короткие слова...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Короткие слова</translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Без стиля</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>Изменить стили...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Без стиля</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Правка стилей</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Копия %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Новый стиль</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Открыть</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Документы (*.sla *.sla.gz *.scd *.scd.gz);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Документы (*.sla *.scd);;Все файлы (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новый</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Изменить</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Проду&amp;блировать</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>У&amp;далить</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Импортировать</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Story Editor</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Текущий абзац:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Слов:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Символов:</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Итого:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Абзацев:</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation>Обновить текстовую рамку</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Открыть</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Текстовые файлы (*.txt);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Сохранить как</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Вы хотите сохранить изменения?</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation>Обновить текстовую рамку и выйти</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>Выйти без обновления текстовой рамки</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новый</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Восстановить текст из рамки</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Сохранить в файл...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>За&amp;грузить из файла...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Сохранить до&amp;кумент</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Обновить текст в рамке и выйти</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>Выйти &amp;без обновления текста в рамке</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Выделить в&amp;сё</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Вырезать</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>С&amp;копировать</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Вст&amp;авить</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>О&amp;чистить</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Найти/Заменить...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>&amp;Изменить стили...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>&amp;Просмотреть шрифты...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Обновить текстовую рамку</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Фон...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Шрифт текста...</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Файл</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Правка</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Настроить...</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation>Загрузить текст из файла</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>Сохранить текст в файл</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>Восстановить текст из рамки</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Найти/Заменить</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Умное выделение текста</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation>В&amp;ставить символ...</translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation>Удалить весь текст</translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation>Story Editor - %1</translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation>Вы действительно хотите потерять все изменения?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Вы действительно хотите удалить весь текст?</translation>
    </message>
</context>
<context>
    <name>StrikeValues</name>
    <message>
        <source>Auto</source>
        <translation>Авто</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>Смещение</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Толщина линии</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation>Подчёркивание</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Капитель</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Нижний индекс</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Верхний индекс</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Вычеркнутый текст</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Контур</translation>
    </message>
    <message>
        <source>Underline Words Only</source>
        <translation>Подчеркнуть только слова</translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation>Все капителью</translation>
    </message>
    <message>
        <source>Shadow</source>
        <translation>Тень</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Добавить название документа в начало абзацного стиля</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Больше не спрашивать</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Параметры импорта из OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Перезаписать абзацные стили</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Если включить это, существующие стили документа Scribus будут перезаписаны</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Объединить абзацные стили</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Объединить абзацные стили по атрибутам. В результате получается меньше стилей, но с теми же атрибутами, даже если исходные стили документа именуются иначе.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Добавить имя документа в начало имени стиля в Scribus.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Сделать эти настройки исходными или больше не задавать вопросов при импорте документа OpenOffice.org 1.x.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отменить</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation>В начале</translation>
    </message>
    <message>
        <source>At the end</source>
        <translation>В конце</translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation>Не показывать</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Указатель оглавления и индексы</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Указатель оглавления</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Добавить</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+д</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Удалить</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+у</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Блок, в который будет помещено оглавление</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Номера страниц:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Имя атрибута объекта:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Атрибут объекта, который будет установлен для блоков в качестве основы для создания записей</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Поместить номера страниц записей в в начало или конец строки, либо не помещать вовсе</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Перечислить непечатаемые записи</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Также включить непечататыемые блоки</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Абзацный стиль для строк элементов оглавления</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Абзацный стиль:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Блок назначения:</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation>&amp;Индекс</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Указатель оглавления и индексы</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Указатель оглавления</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Добавить</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+Д</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Удалить</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+У</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Блок, в который будет помещено оглавление</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Номера страниц:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Имя атрибута объекта:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Атрибут объекта, который будет установлен для блоков в качестве основы для создания записей</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Поместить номера страниц записей в в начало или конец строки, либо не помещать вовсе</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Перечислить непечатаемые записи</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Также включить непечататыемые блоки</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Абзацный стиль для строк элементов оглавления</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Абзацный стиль:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Блок назначения:</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation>&amp;Индекс</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation>Игнорировать все ошибки</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation>Автоматически проверять перед печатью или экспортом</translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation>Искать отсутствующие символы</translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation>Искать объекты вне страницы</translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation>Искать переполнения текстовых блоков</translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation>Искать полупрозрачные объекты</translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation>Искать отсутствующие изображения</translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation>Проверять разрешение изображений</translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation>Минимально возможное разрешение</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation>Искать PDF-аннотации и поля</translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation>Добавить профиль</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation>Удалить профиль</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation>Проверять наличие встроенных файлов PDF</translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation>Основные настройки</translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation>Расположение в документах</translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation>На заднем плане</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation>На переднем плане</translation>
    </message>
    <message>
        <source>Snapping</source>
        <translation>Привязка</translation>
    </message>
    <message>
        <source>Snap Distance:</source>
        <translation>Расстояние привязки:</translation>
    </message>
    <message>
        <source>Grab Radius:</source>
        <translation>Радиус захвата:</translation>
    </message>
    <message>
        <source> px</source>
        <translation> px</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation>Показывать направляющие</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation>Показывать поля</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation>Показывать страничную сетку</translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation>Основная сетка</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Цвет:</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation>Интервал:</translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation>Вспомогательная сетка</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Показывать опорную сетку</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>&amp;Сетка линии шрифта:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>С&amp;мещение сетки линии шрифта:</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Расстояние между малыми ячейками сетки</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Расстояние между большими ячейками сетки</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Расстояние, начиная с которого объект начнёт «прилипать» к направляющим линиям</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Радиус области, внутри которой можно захватывать точки контроля над объектом</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Цвет малых ячеек сетки</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Цвет больших ячеек сетки</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Цвет направляющих линий</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Цвет линий полей</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation>Включить или отключить показ основной сетки</translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation>Включить или отключить показ линий сетки</translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation>Включить или отключить показ направляющих</translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation>Включить или отключить показ полей</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation>Настройки опорной сетки</translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation>Цвет сетки основной линии</translation>
    </message>
    <message>
        <source>Guides are not visible through objects on the page</source>
        <translation>Направляющие не видны за объектами на страницы</translation>
    </message>
    <message>
        <source>Guides are visible above all objects on the page</source>
        <translation>Направляющие всегда выше объектов страницы</translation>
    </message>
    <message>
        <source>Distance between the lines of the baseline grid</source>
        <translation>Расстояние между линиями сетки основной линии</translation>
    </message>
    <message>
        <source>Distance from the top of the page for the first baseline</source>
        <translation>Расстояние от верха страницы до первой базовой линии</translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Настройка табуляторов</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation>Диапазон экспорта</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>&amp;Все страницы</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>Вы&amp;брать страницы</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Поворот:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Параметры</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>Сов&amp;местимость:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>П&amp;ереплёт:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>По левому полю</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>По правому полю</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Создать &amp;миниатюры</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Сохранить св&amp;язанные текст. рамки как PDF-Articles</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>Добавить &amp;закладки</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>С&amp;жать текст и векторную графику</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation>Характеристики изображения</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Автоматически</translation>
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
        <translation>Ничего</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Максимальное</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Высокое</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Среднее</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Низкое</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Наихудшее</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;Общие</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>Встроить &amp;все шрифтовые файлы</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation>&amp;Вычесть неиспользуемые знаки из всех шрифтовых файлов</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Внедрение</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Доступные шрифты:</translation>
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
        <translation>Внедряемые шрифты:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation>Из каких шрифтовых файлов вычесть знаки:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Шрифты</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation>&amp;Включить презентационные эффекты</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Страница</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>Показывать вид &amp;страниц</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Эффекты</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>Продолжительность
&amp;отображения:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Продолжительность
&amp;эффекта:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>&amp;Тип эффекта:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>Дви&amp;жущиеся линии:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>От&amp;куда:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>&amp;Направление:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation>сек</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Без эффекта</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Blinds</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Box</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Растворение</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Мерцание</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Разделение</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Расчистка</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>По горизонтали</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>По вертикали</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Изнутри</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Снаружи</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Слева направо</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Сверху вниз</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Снизу вверх</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Справа налево</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Из верхнего левого в правый нижний</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>&amp;Применить эффект ко всем страницам</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>&amp;Эффекты</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>Использовать &amp;шифрование</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Пароли</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Пользователь:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>&amp;Владелец:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>Разрешить &amp;печать документа</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Разрешить &amp;изменения в документе</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Разрешить &amp;копирование текста и графики</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Разрешить добавление &amp;аннотаций и полей</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>&amp;Безопасность</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Общие</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Назначение в&amp;ывода:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Просмотра с монитора / Web</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Вывод на печать</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>В оттенках серого</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Свои настройки рендеринга</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Настройки рендеринга</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>&amp;Частота:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Угол:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>Сп&amp;от-функция:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Простая точка</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Линия</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Окружность</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Эллипс</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Сплошные тона:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Использовать ICC-профиль</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Профиль:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Тип рендеринга:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Перцепционный</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Относительно колориметрический</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Насыщенность</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Абсолютно колориметрический</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Изображения:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Не использовать встроенные ICC-профили</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>Ц&amp;вет</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>Тип вывода в PDF/X-3</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Инфострока:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Профи&amp;ль вывода:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Рамка под обрез</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Показывать миниатюры каждой 
указанной выше страницы.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>Как долго отображается страница</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Тип отображаемого эффекта</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Направление движения линий в эффектах split и blind.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Стартовая позиция эффектов box и split.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Направление эффектов glitter и wipe.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Применить выбранный эффект ко всем страницам.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Экспортировать все страницы в PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Экспортировать часть страниц в PDF</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Если параметр включён, Scribus создаст PDF-Articles, 
что полезно для навигации по связанным статьям в PDF.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>dpi (кол-во точек на дюйм) при экспорте изображений</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Выберите пароль для доступа на чтение PDF-документа.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Разрешить печать документа.
При невыставленном флажке печатать  
файл PDF будет невозможно.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Разрешить изменения в документе. 
При невыставленном флажке изменять  
файл PDF будет невозможно.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Встроить цветовой профиль для сплошных тонов</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Цветовой профиль для сплошных тонов</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Тип рендеринга сплошных тонов</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Встроить цветовой профиль для изображений</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Не использовать цветовые профили, 
встроенные в исходные изображения</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Цветовой профиль для изображений</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Тип рендеринга изображений</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Профиль вывода на печать. При возможности 
получите информацию в вашей типографии 
о необходимом профиле.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Расстояние от линии обреза до верха физической страницы</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Расстояние от линии обреза до низа физической страницы</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Расстояние от линии обреза до левого края физической страницы</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Расстояние от линии обреза до правого края физической страницы</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Зеркально отразить по горизонтали</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Зеркально отразить по вертикали</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>&amp;Разрешение для EPS-графики:</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</source>
        <translation>Включает презентационные эффекты в полноэкранном режиме Adobe&amp;#174; Reader&amp;#174;.</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Преобразовать сплошные тона в печатные</translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation>Степень сжат&amp;ия:</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation>Здесь определяется совместимость PDF. По умолчанию используется PDF 1.3, обеспечивающий максимально возможную совместимость. Выбирайте PDF 1.4, если в документе используется полупрозрачность или необходимо 128-битное шифрование. PDF/X-3 предназначен для экспорта в PDF, когда необходим файл с управлением цветом в RGB для  коммерческой печати, и когда управление цветом включено. Используйте PDF/X-3 только если точно знаете, что он поддерживается, либо, в некоторых случаях, когда ожидается печать на 4-цветный лазерный принтер.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation>Способ сжатия изображений. По умолчанию Scribus сам выбирает подходящий. ZIP сжимает без потерь в качестве изображения. JPEG больше подходит для создания небольших файлов PDF с большим количеством изображений внутри, но удаляет полезную информацию. Предоставьте Scribus самостоятельно выбрать подходящий способ, если не думаете, что лучше программы знаете, что к чему.</translation>
    </message>
    <message>
        <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level.</source>
        <translation>Качество после сжатия: Наихудшее (25%), Низкое (50%), Среднее (75%), Высокое (85%), Максимальное (95%). Помните, что степень сжатия не напрямую определяет размер конечного файла - размер и качество всегда в первую очередь зависят от характеристик исходных изображений.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation>Разрешить копирование текста и/или графики. Если отключено, текст и графику копировать нельзя.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation>Разрешить добавление аннотация и полей в файл PDF. Если отключено, изменять аннотации и поля нельзя.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Разрешить преобразование плашечных цветов в композитные. Если вы не собираетесь печатать плашечные цвета в типографии, то лучше оставьте опцию включённой.</translation>
    </message>
    <message>
        <source>Include La&amp;yers</source>
        <translation>Включить с&amp;лои</translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation>Способ с&amp;жатия:</translation>
    </message>
    <message>
        <source>Resa&amp;mple Images to:</source>
        <translation>Изменить разрешение всего растра &amp;до:</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
        <translation>Встроить шрифты в файл PDF, чтобы документ везде выглядел так, как он выглядит на вашем компьютере.</translation>
    </message>
    <message>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Длительность действия эффекта. Чем меньше значение, тем быстрее воспроизводится эффект, и наоборот.</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Вставить сюда разделённый запятыми список маркёров, где каждый маркёр может быть * для всех страниц, 1-5 для диапазона страниц или номером одиночной страницы.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation>Определяет переплёт страниц документа в PDF. Если точно не знаете, что нужно изменить, оставьте значение по умолчанию - по левому полю.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation>Создать миниатюрыне изображения каждой страницы в файле PDF. В некоторых просмотрщиках это облегчает навигацию по документу.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is choosen.</source>
        <translation>Слои в вашем документе экспортируются в PDF только если выбран формат PDF 1.5.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation>Встроить в документ созданные вами закладки, упрощающие навигацию по большим документам.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation>Изменить разрешение для текста и векторной графики. Это изменение не затрагивает растровые изображения.</translation>
    </message>
    <message>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation>Включить сжатие текста и графики без потерь в качестве, чтобы снизить размер файла PDF. Оставьте включённым, если нет особых причин отключить.</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. This can increase memory usage and slow down export.</source>
        <translation>Пересэмплировать все растровые изображения до указанного значения DPI. Отключите, если хотите, чтобы изображения сохранили исходное разрешение. При пересэмплировании может увеличиться потребление памяти и замедлиться экспорт.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Включить функции безопасности в экспортируемом файле PDF. Если выбран PDF 1.3, файл будет зашифрован 40-битным ключом. Если выбран PDF 1.4, файл будет зашифрован 128-битным ключом. Помните, что шифрование в PDF менее мощное и более ограниченное в сравнении с GPG и PGP.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation>Выберите общий пароль для включения и отключения функций безопасности в экспортируемом файле PDF</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation>Цветовая модель для экспортируемого PDF. Выберите «Просмотр с монитора/Веб» для PDF, которые предполагается читать с экрана или печатать на обычном струйном принтере. Выберите «Вывод на печать», если собираетесь печатать на 4-цветный CMYK принтер.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Это отключенные по умолчанию расширенные настройки. Включить их нужно лишь в том случае, если этого требует принтер или если они точно обеспечивают желаемый уровень детализации. В противном случае полученный файл PDF может некорректно напечататься и не будет полностью переносимым между системами.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Обязательная строка для PDF/X-3 или PDF нарушит соответствие PDF/X-3. Рекомендуется использовать заголовок документа.</translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation>Настройки отображения документа</translation>
    </message>
    <message>
        <source>Page Layout</source>
        <translation>Способ показа страниц</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>По одной за раз</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation>Бесконечной лентой</translation>
    </message>
    <message>
        <source>Double Page Left</source>
        <translation>По две слева</translation>
    </message>
    <message>
        <source>Double Page Right</source>
        <translation>По две справа</translation>
    </message>
    <message>
        <source>Visual Appearance</source>
        <translation>Прочие настройки</translation>
    </message>
    <message>
        <source>Use Viewers Defaults</source>
        <translation>Использовать настройки просмотрщика</translation>
    </message>
    <message>
        <source>Use Full Screen Mode</source>
        <translation>Включить полноэкранный режим</translation>
    </message>
    <message>
        <source>Display Bookmarks Tab</source>
        <translation>Показывать вкладку Bookmarks</translation>
    </message>
    <message>
        <source>Display Thumbnails</source>
        <translation>Показывать вкладку Thumbnails</translation>
    </message>
    <message>
        <source>Display Layers Tab</source>
        <translation>Показывать вкладку Layers</translation>
    </message>
    <message>
        <source>Hide Viewers Toolbar</source>
        <translation>Прятатать панель Viewers</translation>
    </message>
    <message>
        <source>Hide Viewers Menubar</source>
        <translation>Прятатать меню Viewers</translation>
    </message>
    <message>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation>Отмасштабировать страницы до размера окна</translation>
    </message>
    <message>
        <source>Special Actions</source>
        <translation>Специальные действия</translation>
    </message>
    <message>
        <source>Javascript to be executed
when Document is opened:</source>
        <translation>Javascript для исполнения
при открытии документа:</translation>
    </message>
    <message>
        <source>No Script</source>
        <translation>Без сценария</translation>
    </message>
    <message>
        <source>Viewer</source>
        <translation>Просмотрщик</translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation>Шрифт:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Размер:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation>Цвет заливки:</translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation>Цвет контура:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>С&amp;толбцов:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Интервал:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>А ещё неплохо бы на зависть другим уметь красиво читать и писать</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>&amp;Цвет линии:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Затенение:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>Цвет за&amp;ливки:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>За&amp;тенение:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation>Стиль линии:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>То&amp;лщина линии:</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation>Стиль &amp;линии:</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>Стрелки:</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>Начало:</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>Конец:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Свободное масштабирование</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>Масштабирование 
по &amp;горизонтали:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>Масштабирование 
по &amp;вертикали:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>Масштабировать &amp;изображение до блока</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Сохранять &amp;соотношение сторон</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Цвет &amp;заливки:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>&amp;Минимум:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>Ма&amp;ксимум:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>Ш&amp;аг:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Свойства текстовой рамки</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Свойства рамки изображения</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Свойства очертаний</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Степень увеличения по умолчанию</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Свойства линии</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Свойства многоугольника</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Шрифт для новых текстовых рамок</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Кегль шрифта для новых текстовых рамок</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Цвет шрифта</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Количество столбцов в текстовой рамке</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Расстояние между столбцами в текстовой рамке</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Так выглядит указанный шрифт</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Рамки позволяют произвольно масштабировать изображения</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Масштабирование изображений по горизонтали</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Масштабирование изображений по вертикали</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Сохранять соотношение сторон при масштабировании</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Изображения в рамке масштабируются под её размер</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Автоматически масштабируемые изображения 
сохраняют исходное соотношение сторон</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Цвет заливки рамок изображений</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Насыщение цвета заливки</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Цвет линий фигур</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Насыщение цвета линий</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Цвет заливки фигур</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Стиль линий фигур</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Толщина линий фигур</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Включено минимально возможное «прилипание»</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Включено максимально возможное «прилипание»</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Изменить «прилипание» для каждого шага масштабирования</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Цвет линий</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Насыщенность цвета</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Стиль линий</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Толщина линий</translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation>Заполняющий символ табуляции:</translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation>Ширина табуляции:</translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation>Использовать встроенный кадрирующий контур</translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation>Просмотр на экране</translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation>С полным разрешением</translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation>С обычным разрешением</translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation>С низким разрешением</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Свой:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Свой: </translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation>Цвет текста:</translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation>Оттенение:</translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation>Штрих текста:</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Точка</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Перенос</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Подчёркивание</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Свой</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation>Нижний индекс</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Смещение:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;Масштабирование:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Верхний индекс</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>Смеще&amp;ние:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>&amp;Масштабирование:</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Капитель</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Ма&amp;сштабирование:</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Смещение выше линии опорной сетки</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Относительный размер верхнего индекса по отношению к обычному шрифту</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Смещение ниже линии опорной сетки обычного шрифта</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Относительный размер нижнего индекса по отношению к обычному шрифту</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Относительный размер капители по отношению к обычному шрифту</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Подчёркивание</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation>Смещение:</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Авто</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Толщина линии:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Зачёркнутый</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation>Автоматическое &amp;межстрочное расстояние</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation>Между строк:</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Увеличение междустрочного интервала в процентах размера шрифта</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation>Смещение ниже линии опорной сетки обычного шрифта, выраженное в процентном соотношении нисхождения шрифтов</translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation>Толщина линии, выраженная процентным соотношением от кегля шрифта</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation>Смещение выше линии опорной сетки обычного шрифта, выраженное в процентном соотношении восхождения шрифтов</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Влево</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Вправо</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>По точке</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>По запятой</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>По центру</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Удалить всё</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Отступ первой строки абзаца</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Отступ слева для всего абзаца</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Удалить все табуляторы</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Положение:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation>Заполняющий символ:</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Свой:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Свой:</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Точка</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Перенос</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Подчёркивание</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Свой</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>Структура</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Элемент</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Группа</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Свободные объекты</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Страница </translation>
    </message>
</context>
<context>
    <name>UnderlineValues</name>
    <message>
        <source>Auto</source>
        <translation>Авто</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>Смещение</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Толщина линии</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>Добавлена верт. направляющая</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>Добавлена горизонт. направляющая</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>Убрана верт. направляющая</translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation>Убрана горизонт. направляющая</translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation>Смещена вертикальная направляющая</translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation>Смещена горизонтальная направляющая</translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation>Заблокированы направляющие</translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation>Разблокированы направляющие</translation>
    </message>
    <message>
        <source>Move</source>
        <translation>Перемещение</translation>
    </message>
    <message>
        <source>Resize</source>
        <translation>Смена размера</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>Вращение</translation>
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
        <translation>Выделение</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Группировка</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation>Выделение/группировка</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Создание</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation>X: %1, Y: %2
W: %3, H: %4</translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation>Выравнивание/Распределение</translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation>Вовлечённые объекты</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отменить</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation>Изменена заливка</translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation>Цвет1: %1, Цвет: %2</translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation>Изменён цвет заполнения тени</translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation>Изменён цвет линии</translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation>Изменён цвет тени линии</translation>
    </message>
    <message>
        <source>Flip horizontally</source>
        <translation>Переворот по горизонтали</translation>
    </message>
    <message>
        <source>Flip vertically</source>
        <translation>Переворот по вертикали</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Блокирование</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Разблокирование</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation>Размер заблокирован</translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation>Размер разблокирован</translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation>Разгруппировка</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Удаление</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Переименование</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation>От %1
до %2</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Вставка</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Вырезание</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation>Изменена прозрачность заливки</translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation>Изменена прозрачность цвета линии</translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation>Изменён стиль линии</translation>
    </message>
    <message>
        <source>Set the style of line end</source>
        <translation>Изменён стиль начала линии</translation>
    </message>
    <message>
        <source>Set the style of line join</source>
        <translation>Изменён стиль стыковки линий</translation>
    </message>
    <message>
        <source>Set line width</source>
        <translation>Изменена толщина линии</translation>
    </message>
    <message>
        <source>No style</source>
        <translation>Снятие стиля</translation>
    </message>
    <message>
        <source>Set custom line style</source>
        <translation>Применён свой стиль линии</translation>
    </message>
    <message>
        <source>Do not use custom line style</source>
        <translation>Не использовать свой стиль линии</translation>
    </message>
    <message>
        <source>Set start arrow</source>
        <translation>Изменена начальная стрелка</translation>
    </message>
    <message>
        <source>Set end arrow</source>
        <translation>Изменена конечная стрелка</translation>
    </message>
    <message>
        <source>Create table</source>
        <translation>Создана таблица</translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation>Строк: %1, столбцов: %2</translation>
    </message>
    <message>
        <source>Set font</source>
        <translation>Изменён шрифт</translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation>Изменён кегль шрифта</translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation>Изменена ширина шрифта</translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation>Изменён цвет заполнения шрифта</translation>
    </message>
    <message>
        <source>Set font stroke color</source>
        <translation>Изменён цвет заполнения штриха шрифта</translation>
    </message>
    <message>
        <source>Set font fill color shade</source>
        <translation>Изменена тень цвета заполнения шрифта</translation>
    </message>
    <message>
        <source>Set font stroke color shade</source>
        <translation>Изменена тень цвета заполнения штриха шрифта</translation>
    </message>
    <message>
        <source>Set kerning</source>
        <translation>Изменён кернинг</translation>
    </message>
    <message>
        <source>Set line spacing</source>
        <translation>Изменён интерлиньяж</translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation>Изменён стиль абзаца</translation>
    </message>
    <message>
        <source>Set language</source>
        <translation>Изменён язык</translation>
    </message>
    <message>
        <source>Align text</source>
        <translation>Изменена выключка</translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation>Применён эффект шрифта</translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation>Блок изображения</translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation>Текстовый блок</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Многоугольник</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation>Кривая Безье</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Ломаная линия</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation>Преобразование в</translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation>Импорт SVG-изображения</translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation>Импорт EPS-изображения</translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation>Рабочий стол</translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation>Обтекание текстом вокруг блока</translation>
    </message>
    <message>
        <source>Text flows around bounding box</source>
        <translation>Обтекание текстом вокруг площадки</translation>
    </message>
    <message>
        <source>Text flows around contour line</source>
        <translation>Обтекание текстом вокруг контурной линии</translation>
    </message>
    <message>
        <source>No text flow</source>
        <translation>Снятие обтекания текстом</translation>
    </message>
    <message>
        <source>No bounding box</source>
        <translation>Без площадки</translation>
    </message>
    <message>
        <source>No contour line</source>
        <translation>Без контурной линии</translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation>Страница №</translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation>Изменить масштаб изображения</translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation>Размер блока</translation>
    </message>
    <message>
        <source>Free scaling</source>
        <translation>Свободное масштабирование</translation>
    </message>
    <message>
        <source>Keep aspect ratio</source>
        <translation>Сохранять пропорции</translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation>Не сохранять пропорции</translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation>Изменить контурную линию</translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation>Правка очертаний</translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation>Восстановить контурную линию</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Применение мастер-страницы</translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation>Смена высоты шрифта</translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation>Импорт изображения OpenOffice.org Draw</translation>
    </message>
    <message>
        <source>Add page</source>
        <translation>Добавлена страница</translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation>Добавлены страницы</translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation>Удалена страница</translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation>Удалены страницы</translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation>Добавлен слой</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Удалён слой</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation>Переименован слой</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Поднят слой</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Опущен слой</translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation>Переложен в слой</translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation>Включена печать слоя</translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation>Отключена печать слоя</translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation>Переименован слой</translation>
    </message>
    <message>
        <source>Get image</source>
        <translation>Вставлено изображение</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Текст по контуру</translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Action History</source>
        <translation>История действий</translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation>Только выделенный объект</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>О&amp;тменить</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Повторить</translation>
    </message>
    <message>
        <source>Initial State</source>
        <translation>Исходное состояние</translation>
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
        <translation>Вставить значение</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Введите значение и нажмите ОК</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Введите значение и нажмите ОК</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Передать ваше значение сценарию</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Инструменты</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Свойства...</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>Button</source>
        <translation>Кнопка</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Текстовое поле</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Флажок</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Раскрывающийся список</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Список</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>ССылка</translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation>PDF-инструменты</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Вставить PDF-поля</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Вставить PDF-аннотации</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Выберите фильтр импорта</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Автоматически</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Импортировать текст без форматирования</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Фильтр импорта:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Кодировка:</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>Импортировать только текст</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Выберите фильтр импорта</translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Запомнить ассоциацию</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation>Запомнить расширение файла - ассоциацию импортировщика, и больше не спрашивать о выборе средства импорта для этого файла.</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Новый документ из шаблона</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Все</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Размер страницы</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Цвета</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Описание</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Использование</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Создано при помощи:</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Автор</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Удалить</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Открыть</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Скачивание шаблонов</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Устанавка шаблонов</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Распакуйте архив в каталог шаблонов ~/.scribus/templates 
для текущего пользователя или в PREFIX/share/scribus/templates 
для всех пользователей системы.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Подготовка шаблона</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Удаление шаблона</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Перевод template.xml</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Дата</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Шаблоны документов можно найти на сайте http://www.scribus.net/ в разделе Downloads.</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Убедитесь в том, что используемые изображения доступны с любого другого компьютера. Если шрифты не доступны отовсюду, не собирайте их при сохранении шаблона.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Создатель шаблона должен убедиться в том, что раздел «Установка шаблонов» действителен и для его шаблонов. Это означает, что у пользователя не должно возникнуть проблем при скачивании пакета, извлечении файлов шаблонов из него и использовании этих шаблонов.</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Удаление шаблона из данного диалога означает лишь удаление записи из template.xml, а сами файлы с документами удалены не будут. Всплывающее меню с пунктом «Удалить» появится только в том случае, если у вас есть права на запись в файл template.xml.</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Скопируйте существующий template.xml в файл с именем вида template.lang_COUNTRY.xml (используйте тот же код страны, что и в qm-файле для вашего языка), например template.fi.xml для template.xml на финском языке. Копия должна находиться в том же каталоге, что и исходный template.xml, так чтобы Scribus мог загрузить этот файл.</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Сохранить как шаблон</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Категория</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Размер страницы</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Цвета</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Описание</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Использование</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Автор</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>Эл. адрес</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Подробнее</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Менее подробно</translation>
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
        <translation>Таблоид</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>альбом</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>портрет</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>свой</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Создать фильтр</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>О&amp;чистить</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>У&amp;далить</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Выбрать предыдущий сохранённый фильтр</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Дать имя этому фильтру для запоминания</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Дать имя для запоминания</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Отключить или включить эту строку фильтра</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Удалить эту строку фильтра</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Добавить новую строку фильтра</translation>
    </message>
    <message>
        <source>to</source>
        <translation>до</translation>
    </message>
    <message>
        <source>and</source>
        <translation>и</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>удалить совпадение</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>не удалять совпадение</translation>
    </message>
    <message>
        <source>words</source>
        <translation>слов</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Заменить</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Применить</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>Значение слева является регулярным выражением</translation>
    </message>
    <message>
        <source>with</source>
        <translation>на</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>абзацный стиль</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>все вхождения</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>все абзацы</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>абзацы, начинающиеся с</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>абзац меньше</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>абзацы больше</translation>
    </message>
</context>
</TS>
