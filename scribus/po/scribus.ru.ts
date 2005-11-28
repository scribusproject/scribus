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

Показавает диалог &quot;Новый документ&quot;. Создаёт новый документ, если пользователь
примет установки. Не создаёт документ, если пользователь нажимает &quot;Отменить&quot;.
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
        <source>newPage(where [,&quot;template&quot;])

Creates a new page. If &quot;where&quot; is -1 the new Page is appended to the
document, otherwise the new page is inserted before &quot;where&quot;. Page numbers are
counted from 1 upwards, no matter what the displayed first page number of your
document is. The optional parameter &quot;template&quot; specifies the name of the
template page for the new page.

May raise IndexError if the page number is out of range
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
        <source>register_macro_callable(name, callable, accel=&apos;&apos;)

Create a macro called &quot;name&quot; with the existing callable object &quot;callable&quot;.
The passed callable must not require any arguments when called (it may take
optional arguments, but will not be given any).
If provided, the string &quot;accel&quot; will be used to set a keyboard shortcut
for the macro.
If the passed callable is a class, it will be rejected. Functions and bound
methods are quite acceptable, as are instances of classes that provide a
__call__ method with no arguments. There is no problem with registering
a callable more than once, nor with registering multiple bound methods
of a single instance.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>newDocument(size, margins, orientation, firstPageNumber,
unit, pagesType, firstPageOrder) -&gt; bool

WARNING: This is backported function from 1.3.x series. You are using constants
larger PAGE_3 and bigger on your own risk. So do you with firstPageOrder biger
than 1.

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
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yellow and k = Black. Color components should be in
the range from 0 to 255.

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
        <source>fileDialog(&quot;caption&quot;, [&quot;filter&quot;, &quot;defaultname&quot; ,haspreview, issave]) -&gt; string with filename

Shows a File Open dialog box with the caption &quot;caption&quot;. Files are filtered
with the filter string &quot;filter&quot;. A default filename or file path can also
supplied, leave this string empty when you don&apos;t want to use it.  A value of
True for haspreview enables a small preview widget in the FileSelect box.  When
the issave parameter is set to True the dialog acts like a &quot;Save As&quot; dialog
otherwise it acts like a &quot;File Open Dialog&quot;. The default for both of the
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
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius is
expressed in points. If &quot;name&quot; is not given the currently
selected item is used.
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
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
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
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positive values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
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
        <source>selectText(start, count, [&quot;name&quot;])

Selects &quot;count&quot; characters of text in the text frame &quot;name&quot; starting from the
character &quot;start&quot;. Character counting starts at 0. If &quot;count&quot; is zero, any
text selection will be cleared. If &quot;count&quot; is -1, all text in the frame will
be selected. If &quot;name&quot; is not given the currently selected item is used.

May throw IndexError if the selection is outside the bounds of the text.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>register_macro_code(name, sourcetext, accel=&apos;&apos;)

Create a macro named &quot;name&quot; by evaluating the the source code &quot;sourcetext&quot;.
&quot;sourcetext&quot; must follow the same rules as macros created in the GUI.
If provided, the string &quot;accel&quot; will be used to set a keyboard shortcut
for the macro.
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
color &quot;name&quot; from the current document, converted to the RGB colour
space. If no document is open, returns the value of the named color
from the default document colors.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
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
        <source>isLayerVisible(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
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
        <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference). The first character has
an index of 0. Inserting at position -1 appends text to the frame. If &quot;name&quot;
is not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>newDoc(size, margins, orientation, firstPageNumber,
                   unit, facingPages, firstSideLeft) -&gt; bool

Creates a new document and returns true if successful. The parameters have the
following meaning:

    size = A tuple (width, height) describing the size of the document. You can
    use predefined constants named PAPER_&lt;paper_type&gt; e.g. PAPER_A4 etc.

    margins = A tuple (left, right, top, bottom) describing the document
    margins

    orientation = the page orientation - constants PORTRAIT, LANDSCAPE

    firstPageNumber = is the number of the first page in the document used for
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
        <source>textOverflows([&quot;name&quot;, nolinks]) -&gt; integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn&apos;t
use text frame linking. Without this parameter it search all linking chain.

May raise WrongFrameTypeError if the target frame is not an text frame
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
        <source>Scribus Version %1
%2 %3</source>
        <translation>Scribus v%1
%2 %3</translation>
    </message>
    <message>
        <source>Build-ID:</source>
        <translation>ID сборки:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Свой вклад в разработку внесли:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation>На немецкий:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation>На французский:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation>На итальянский:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation>На венгерский:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation>На украинский:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation>На болгарский:</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation>На галльский:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation>На турецкий:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation>На литовский:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation>На польский:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation>На чешский:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation>На словацкий:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation>На датский:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation>На норвежский:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation>На уэльский:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation>На русский:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation>Портирование под Windows:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation>На бразильский:</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation>На финский:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation>На словенский:</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation>На баскский:</translation>
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
        <source>English (British):</source>
        <translation>На британский английский:</translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation>На шведский:</translation>
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
        <source>Catalan:</source>
        <translation>На каталонский:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation>На корейский:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation>На испанский:</translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation>На эсперанто:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation>На сербский:</translation>
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
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=littlecms C=CUPS T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Здесь отображается версия, дата сборки и
статус поддержки разных библиотек.
Сокращение C-C-T обозначает: C=littlecms,
C=CUPS, T=TIFF.
Отсутствие поддержки обозначается 
символом &quot;*&quot;.</translation>
    </message>
    <message>
        <source>%1 %2 %3 </source>
        <translation>%1 %2 %3 </translation>
    </message>
    <message>
        <source>Afrikaans:</source>
        <translation>Африкаанс:</translation>
    </message>
    <message>
        <source>Portuguese (Brazilian):</source>
        <translation>Португальский (Бразилия):</translation>
    </message>
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation>Дополнительные параметры</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation>Создать PostScript Level 3</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation>Создать только PostScript Level 2
Осторожно! Файл может получиться огромным.</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation>Создать только PostScript Level 1
Осторожно! Файл может получиться огромным.</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation>Зеркально отразить по &amp;горизонтали</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation>Зеркально отразить по &amp;вертикали</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation>Применить вычитание из-под &amp;чёрного</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation>Применить ICC-&amp;профили</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation>PostScript Level &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation>PostScript Level &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation>PostScript Level &amp;3</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Способ удаления некоторых серых (ахроматических) тонов, 
составленных из голубой, желтой и пурпурной красок, 
и замены их на черный. UCR главным образом воздействует на 
нейтральные по цвету части изображения. Использование этого 
метода способно улучшить печать некоторых изображений, но 
каждый отдельный случай требует особого рассмотрения. 
При использовании этого способа также снижается 
вероятность избыточной яркости изображения. </translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Указать размер бумаги</translation>
    </message>
    <message>
        <source>This enables you to explicitely set,
the media size of the postscript file.
Not recommended unless
 requested by your printer.</source>
        <translation type="obsolete">Самостоятельно задать формат 
листа в файле Postscript.
Не рекомендуется включать, 
если того не требует принтер.</translation>
    </message>
    <message>
        <source>This enables you to explicitely set,
the media size of the PostScript file.
Not recommended unless
 requested by your printer.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Distribute/Align</source>
        <translation>Распределение / Выравнивание</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Выравнивание</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>По горизонтали</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation>левыми сторонами</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation>серединой</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation>правыми сторонами</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>По вертикали</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation>верхними сторонами</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation>нижними сторонами</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation>&amp;Между:</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation>&amp;Не изменять</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation>В&amp;ыровнять</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation>&amp;Смещение</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation>Распределить ра&amp;вномерно</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation>&amp;Между:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation>Не &amp;изменять</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation>Выр&amp;овнять</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation>Смеще&amp;ние</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation>Распределить р&amp;авномерно</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Применить</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
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
        <source>Java Script</source>
        <translation type="obsolete">Java Script</translation>
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
        <translation>Другой</translation>
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
        <translation>Другой сценарий проверки:</translation>
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
        <translation>Другой сценарий для вычисления:</translation>
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
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation type="obsolete">Изображения (*.tif *.png *.jpg *.xpm);;Файлы Postscript (*.eps);;Все файлы (*)</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation type="unfinished"></translation>
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
    <name>ApplyT</name>
    <message>
        <source>Apply Template</source>
        <translation>Применить шаблон</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Шаблон:</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation>Применить к &amp;текущей странице</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation>Применить ко всем &amp;чётным страницам</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation>Применить ко всем &amp;нечётным страницам</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation>На&amp;чиная со страницы:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>До:</translation>
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
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Имя &quot;%1&quot; уже использовано.
Выберите другое.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
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
        <source>Color Management Settings</source>
        <translation>Настройки управления цветом</translation>
    </message>
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
        <source>Default color profile for imported images</source>
        <translation>Стандартный цветовой профиль для 
импортированных изображений</translation>
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
        <source>&amp;Pictures:</source>
        <translation>&amp;Изображения:</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
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
        <source>OK</source>
        <translation>ОК</translation>
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
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation>Цвет с таким именем уже существует</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
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
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation>Вы не можете создать цвет с именем &quot;%1&quot;.
Это имя зарезервировано для прозрачного цвета</translation>
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
        <source>Color of selected object</source>
        <translation>Цвет выбранного объекта</translation>
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
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
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
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Здесь отображаются различные параметры CUPS для печати.
Состав параметров зависит от используемой модели принтера.
наличие поддержки можно проверить в диалоге &quot;О Scribus&quot;, 
вызываемом через меню &quot;Справка&quot;. Найдите сокращение вида
&quot;C-C-T&quot;, которое расшифровывается так: 
C=CUPS C=littlecms T=TIFF
Отсутствующая библиотека помечается символом *</translation>
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
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Удаление цвета</translation>
    </message>
    <message>
        <source>?</source>
        <translation>?</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation>Заменить на:</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation>Удалить цвет:</translation>
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
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Удаление страниц</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation>Удалить от:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>до:</translation>
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
    <name>DmF</name>
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
    <message>
        <source>OK</source>
        <translation>ОК</translation>
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
        <source>&amp;Document</source>
        <translation>До&amp;кумент</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>&amp;Подробнее</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Человек или организация, в первую очередь 
ответственные за создание содержимого 
этого документа. Это поле может быть встроено 
в документ Scribus для справки, равно как и 
в метаданные для PDF</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Имя, данное этому документу.
Это поле может быть встроено в 
документ Scribus для справки, 
равно как и в метаданные для PDF</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Характеристика этого документа.
Это поле предназначено для краткого описания документа.
Оно встраивается в PDF при экспорте</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Тема этого документа. Это поле для 
ключевых слов, которые будут встроены 
в PDF для упрощения последующего 
поиска и индексирования PDF-файлов</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Человек или организация, ответственные за выпуск этого документа</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Человек или организация, внесшие вклад 
в содержимое этого документа</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Дата, связанная с событием в жизненном цикле этого 
документа, в формате ГГГГ-ММ-ДД согласно ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Природа жанра содержимого этого документа, например, категории, функции и т.д.</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Физическая или цифровая форма этого документа. 
Рекомендуется  указать тип носителя и количество.
Указание RFC2045, RFC2046 для MIME-типов также не помешает.</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Уникальная ссылка на этот документ посредством ISBN или URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Код языка, на котором написано содержимое этого 
документа -- обычно, в коде ISO-639, обычно 
имеющее суффикс с дефисом и кодом страны 
по ISO-3166, например, en-GB, fr-CH.</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Ссылка на документ с родственной тематикой, 
возможно, с формальным идентификатором, 
таким как ISBN или URI.</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Область охвата данного документа, возможно, 
включая место, время и область юрисдикции.</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Информация о правах на документ и его содержимое, 
например, авторские и патентные права, торговая марка.</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Ссылка на документ, производным от которого является 
данный документ, например, в виде ISBN или URI.</translation>
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
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation type="obsolete">Файлы Postscript (*.ps);;Все файлы (*)</translation>
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
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation>Использовать альтернативное средство 
настройки печати вроде kprinter или gtklp для 
задания дополнительных параметров печати</translation>
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
        <source>Print &amp;Normal</source>
        <translation>Напечатать &amp;обычным образом</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation>Напечатать с ц&amp;ветоделением</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation>Напечатать в цв&amp;ете (если возможно)</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation>Напечатать в оттенках &amp;серого</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation>До&amp;полнительные параметры...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Напечатать</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation type="unfinished"></translation>
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
    <name>EditMacroDialog</name>
    <message>
        <source>Editing Macro: &lt;b&gt;</source>
        <translation>Изменяется макрос: &lt;b&gt;</translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation>Scribus - Управление макросами</translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; already exists.
Are you sure you want to overwrite it?
</source>
        <translation>Файл &apos;%1&apos; уже существует.
Вы хотите заменить его?
</translation>
    </message>
    <message>
        <source>You have already edited this macro.
Are you sure you want to discard all your changes?
</source>
        <translation>Вы уже изменяете этот макрос.
Вы действительно хотите отказаться ото всех изменений?
</translation>
    </message>
    <message>
        <source>A full traceback follows:

%1
</source>
        <translation>Полный вывод следует далее:

%1
</translation>
    </message>
    <message>
        <source>Compilation of the macro failed, so it can not 
be saved in its current form. The error was:
%1
</source>
        <translation>Сборка макроса не удалась, так что он не может быть
сохранён в текущем виде. Вот ошибка:
%1
</translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation>Scribus - Новый макрос</translation>
    </message>
    <message>
        <source>&lt;qt&gt;This is the Edit Macro / New Macro dialog box. Here you can change the source code to macros. Edit the source code to the macro in the text editing area below the &quot;Source Code&quot; label and click OK to save your changes to the macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Это диалог редактирования и создания макросов. Здесь можно изменить исходный код макросов. Изменив исходный код, нажмите &quot;ОК&quot; для сохранения внесённых вами изменений.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Source Code:</source>
        <translation>Исходный код:</translation>
    </message>
    <message>
        <source>Editing Macro:</source>
        <translation>Редактируется макрос:</translation>
    </message>
    <message>
        <source>The name of the macro being edited.</source>
        <translation>Имя изменяемого сейчас макроса.</translation>
    </message>
    <message>
        <source>&lt;/qt&gt;This item displays the name of the macro you are currently editing.&lt;qt&gt;</source>
        <translation>&lt;/qt&gt;Здесь отображается имя макроса, который вы сейчас изменяете.&lt;qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+т</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Discard all changes and exit.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Отказаться от изменений и выйти.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Exit the editing dialog, discarding all changes you have made. If you want to exit without saving the macro but don&apos;t want to lose your changes, save your changes with &quot;Save Source As...&quot;.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Закрыть диалог, отказавшись ото всех внесённых изменений. Если необходимо закрыть диалог, не сохраняя макрос, но и не теряя изменения, воспользуйтесь функцией сохранения исходного кда во внешнйи файл (кнопка &quot;Сохранить исходный код...&quot;).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+о</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes and exit.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Сохранить изменения и выйти.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes to the macro and exit. If there is a problem with the macro, a message will be displayed and the editing dialog will not close.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Сохранить изменения в макросе и закрыть диалог. Если в макросе есть ошибка, вас об этом уведомят и откажут в выходе из диалога.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;This text area contains the source code of the macro. If you&apos;re creating a new macro there won&apos;t be anything in it, and if you&apos;re editing an existing macro the source code the macro was defined with will be shown here.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Здесь содержится исходный код макроса. Если вы создаёте новый макрос, в текстовой области изначально будет пусто.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Load Source ...</source>
        <translation>&amp;Загрузить исходный код...</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+з</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace the current source code with code from a file.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Заменить текущий исходный код кодом из имеющегося файла.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Load new source code into the editing area from &quot;file&quot;. Any source code in the editing area is replaced. The loaded source must be a Scribus macro function. If you load any other script, you&apos;ll need to edit it so that it&apos;ll work as a scripter macro before saving it.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Загрузить новый исходный код в область редактирования из внешнего файла. Новый исходный код заменит собой любой уже находящийся в области редактирования. Загружаемый код обязательно должен быть макрофункцией Scribus, иначе этот код придётся переписать.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Save Source As...</source>
        <translation>&amp;Сохранить исходный код...</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+С</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save the source code being edited to a file.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Сохранить редактируемый исходный код в файл.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Save the source code - the text - of the macro to a file. You can edit the saved source and load it again with &quot;Load Source...&quot;.</source>
        <translation>Сохранить исходный код - текст макроса во внешний файл. Сохранённый файл можно изменить извне и повторно загрузить.</translation>
    </message>
    <message>
        <source>Save File Dialog</source>
        <translation>Диалог сохранения файлов</translation>
    </message>
    <message>
        <source>Save macro source</source>
        <translation>Сохранить исходный код макроса</translation>
    </message>
    <message>
        <source>Open File Dialog</source>
        <translation>Диалог открытия файлов</translation>
    </message>
    <message>
        <source>Select the source file to load</source>
        <translation>Выберите загружаемый файл с исходным кодом</translation>
    </message>
    <message>
        <source>Python source files (*.py)</source>
        <translation>&lt;qt&gt;&lt;p&gt;Через этот диалог управляются макросы Scribus. Здесь вы можете создать макрос, изменить его и т.д. Все изменения производятся кнопками в правой части диалога.&lt;/p&gt;
&lt;p&gt;Все восимые изменения необратимы - изменив что-то однажды, вы не сможете нажать кнопку &quot;Отменить&quot; и вернуть всё в исходное состояние.
В таблице содержится список макросов, статус их загрузки и сводка по каждому из них. Подробности можно получить при помощи кнопки &quot;Что это&quot; в заголовке окна.&lt;/p&gt;&lt;/qt&gt;</translation>
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
        <source>Vertical Spaces</source>
        <translation>Междустрочные интервалы</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Между строк</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation>Стиль с таким именем уже существует</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation>Эффект:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Не задано</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>pp</translation>
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
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation>Создать (буквицу) увеличенную первую букву абзаца.</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Задать общую высоту буквицы в строках</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation>Выровнять по опорной сетке</translation>
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
        <source>&amp;Font:</source>
        <translation>&amp;Шрифт:</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation>&amp;Размер:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation>&amp;Выключка:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation>&amp;Буквица</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Линии:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Цвет &amp;заливки:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation>Цвет &amp;контура:</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation>Подстроить до опорной &amp;сетки</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation>Между с&amp;трок:</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation>&amp;Над:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation>&amp;Под:</translation>
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
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Редактор</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation type="obsolete">Javascripts (*.js);;Все файлы (*)</translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Все страницы</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Сменить каталог вывода</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Каталог вывода -- каталог, в который помещаются изображения.
Файлы будут иметь имена вида &apos;&apos;имядокумента-№страницы.расширение&quot;</translation>
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
        <translation>Не известно</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Документ Scribus</translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Colors</source>
        <translation>Цвета</translation>
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
        <source>Color Sets</source>
        <translation>Наборы цветов</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Текущий набор цветов:</translation>
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
        <source>Append colors to the current set from an existing document</source>
        <translation>Добавить цвета к текущему набору из другого документа</translation>
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
        <source>Choose a Name</source>
        <translation>Выберите имя</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Новый цвет</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Из файла</translation>
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
        <source>&amp;Remove Unused</source>
        <translation>Удалить &amp;неиспользуемые</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>Со&amp;хранить набор цветов</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Имя:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Global Font Settings</source>
        <translation>Глобальные настройки шрифтов</translation>
    </message>
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
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Имя шрифта</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation>Использовать</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation>Встраивать в:</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Подмножество</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation>Путь к шрифту</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>А ещё неплохо бы на зависть другим уметь красиво читать и писать</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <translation>Просмотр шрифтов</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <translation>Добавить выбранный шрифт в Стиль (меню &quot;Шрифт&quot;)</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation>Закрыть этот диалог</translation>
    </message>
    <message>
        <source>Size of the Font</source>
        <translation>Кегль</translation>
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
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
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
        <translation>У&amp;далить</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Блокировать направляющие</translation>
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
        <source>S&amp;earch</source>
        <translation>Ис&amp;кать</translation>
    </message>
    <message>
        <source>Ctrl+F</source>
        <translation>Ctrl+F</translation>
    </message>
    <message>
        <source>F3</source>
        <translation>F3</translation>
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
        <translation>Искать слово:</translation>
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
        <source>Hyphenator Settings</source>
        <translation>Настройки модуля переносов</translation>
    </message>
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
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
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
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Вставка страницы</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation>Вставляется</translation>
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
        <source>Template (Right Page):</source>
        <translation>Шаблон (Правая страница):</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation>&amp;Вставка</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Страниц(а)</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation>Шаблон (&amp;Левая страница):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Шаблон:</translation>
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
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Вставить таблицу</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation>Строк:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation>Столбцов:</translation>
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
        <source>Warning</source>
        <translation>Предупреждение</translation>
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
        <source>Do you really want to delete this Script?</source>
        <translation>Вы действительно хотите удалить этот сценарий?</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation>Настройка горячих клавиш</translation>
    </message>
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
        <source>Warning</source>
        <translation>Предупреждение</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation>Эта комбинация уже использована</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation>Слои</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation>Добавить новый слой</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Удалить слой</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation>Поднять слой</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation>Опустить слой</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation>Новый слой</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation>Вы хотите заодно удалить все объекты этого слоя?</translation>
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
        <source>Warning</source>
        <translation>Предупреждение</translation>
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
        <source>&amp;Append</source>
        <translation>&amp;Из файла</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
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
        <source>Do you really want to delete this Style?</source>
        <translation>Вы действительно хотите удалить этот стиль?</translation>
    </message>
</context>
<context>
    <name>MSpinBox</name>
    <message>
        <source> pt</source>
        <translation>pt</translation>
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
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
</context>
<context>
    <name>Macro</name>
    <message>
        <source>Passed object is not callable</source>
        <comment>python error</comment>
        <translation>Переданный объект невызываем</translation>
    </message>
</context>
<context>
    <name>MacroManager</name>
    <message>
        <source>Manage Macros</source>
        <translation>Управление макросами</translation>
    </message>
    <message>
        <source>Brings up a graphical window for creating, deleting, editing, saving and loading macros.</source>
        <translation>Открывает диалог создания, удаления, изменения, сохранения и запуска макросов.</translation>
    </message>
    <message>
        <source>Create, edit and delete macros</source>
        <translation>Создать, изменить и удалить макрос</translation>
    </message>
    <message>
        <source>&amp;Macro</source>
        <translation>&amp;Макросы</translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation>Scribus - Управление макросами</translation>
    </message>
    <message>
        <source>Unable to open the requested file: %1</source>
        <translation>Невозможно открыть запрошенный файл: %1</translation>
    </message>
    <message>
        <source>Scribus - Edit Macro</source>
        <translation>Scribus - Изменить макрос</translation>
    </message>
    <message>
        <source>&lt;qt&gt;The macro name you requested is already taken  by another macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Макрос с таким именем уже существует.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Macro creation failed. The macro manager was unable to set up the macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Не удалось создать макрос.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; has reported a minor error.
The error is: %2
A full traceback follows:

%3
</source>
        <translation>Макрос &apos;%1&apos; сообщил о незначительной ошибке:
%2
Вот полный вывод:

%3
</translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; failed to execute correctly.
The error is: %2
A full traceback follows:

%3
</source>
        <translation>Не удалось корректно выполнить макрос &apos;%1&apos;.
Ошибка: %2
Вот полный вывод:

%3
</translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation>Scribus - Новый макрос</translation>
    </message>
    <message>
        <source>Enter name for new macro: </source>
        <translation>Введите имя нового макроса:</translation>
    </message>
</context>
<context>
    <name>ManageMacrosDialog</name>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation>Scribus - Управление макросами</translation>
    </message>
    <message>
        <source>Renaming the macro failed because the name is already in use.</source>
        <translation>Макрос с таким именем уже существует.</translation>
    </message>
    <message>
        <source>Scribus - Manage Macros</source>
        <translation>Scribus - Управление макросами</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;This window is the Scribus Macro Manager. Here you can create macros, edit macros, etc. All changes are made using the buttons on the right hand side of the window.&lt;/p&gt;
&lt;p&gt;All changes made in this dialog take effect instantly - you cannot cancel the actions you make here.
The table in the center of the dialog lists what macros are currently loaded and some information about them. Use &quot;What&apos;s this&quot; on the table for more information.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;Через этот диалог управляются макросы Scribus. Здесь вы можете создать макрос, изменить его и т.д. Все изменения производятся кнопками в правой части диалога.&lt;/p&gt;
&lt;p&gt;Все восимые изменения необратимы - изменив что-то однажды, вы не сможете нажать кнопку &quot;Отменить&quot; и вернуть всё в исходное состояние.
В таблице содержится список макросов, статус их загрузки и сводка по каждому из них. Подробности о каждом макросе можно получить при помощи кнопки &quot;Что это&quot; on the table for more information.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новый</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+н</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Создать новый макрос.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro by prompting for the macro name then bringing up the edit macro dialog box.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Создать новый макрос, задав его имя и внеся его код в открываемый затем диалог с редактором кода.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+о</translation>
    </message>
    <message>
        <source>Macro</source>
        <translation>Макрос</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Правка</translation>
    </message>
    <message>
        <source>Accel</source>
        <translation>Сочетание</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Описание</translation>
    </message>
    <message>
        <source>&lt;p&gt;This table lists the macros that are currently defined.&lt;/p&gt;

&lt;p&gt;&lt;b&gt;Name:&lt;/b&gt; The name of the macro, as shown in the menu bar and in other places around Scribus.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Edit:&lt;/b&gt; If the macro can be edited, &quot;Yes&quot; appears in this column. Usually if a macro cannot be edited it was created using the register_macro command in a script.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Accel:&lt;/b&gt; The menu shortcut key sequence, if any, associated with the macro. For example, CTRL-F8 means that you can press Control-F8 when in Scribus to run the macro.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Description:&lt;/b&gt; If the macro contains a &quot;docstring&quot;, a special string at the start of its definition that describes it, that is shown here. If the docstring is long, only the beginning is shown - use &quot;What&apos;s This&quot; on the macro&apos;s entry in the Macro menu to see the full description.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;В этой таблице перечисляются доступные в данный момент макросы.&lt;/p&gt;

&lt;p&gt;&lt;b&gt;Имя:&lt;/b&gt; Имя макроса, каким оно представлено в меню и прочих частях интерфейса Scribus.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Правка:&lt;/b&gt; Если макрос можно изменить, в этом столбце указывается &quot;Да&quot;. Обычно если макрос изменить нельзя, то скорее всего он был создан командой register_macro из сценария.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Сочетание:&lt;/b&gt; Горячие клавишы для исполнения макроса, если таковые назначены. К примеру,нажатие, скажем, CTRL-F8 в Scribus приведёт к исполнению того или иного макроса.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Описание:&lt;/b&gt; Если в макросе содержится &quot;docstring&quot;, специальная строка в начале описания, то она отображается в этом столбце. Слишком длинное описание при отображении укорачивается - щёлкните знаком вопроса с кнопки &quot;Что это&quot; по строке для просмотра полного описания.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Rena&amp;me</source>
        <translation>&amp;Переименовать</translation>
    </message>
    <message>
        <source>Alt+M</source>
        <translation>Alt+п</translation>
    </message>
    <message>
        <source>Rename the selected macro.</source>
        <translation>Переименовать выбранный макрос.</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Rename the selected macro. You will be prompted for the new name.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Переименовать выбранный макрос. Вас попросят указать новое имя макроса.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Изменить...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+и</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Edit the source of the selected macro, if the source is availible.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Изменить исходный код выбранного макроса, если код доступен.&lt;/qt&gt;</translation>
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
        <source>&lt;qt&gt;Delete the currently selected macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Удалить выбранный макрос.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete the selected macro. This is instant, and there is no way to recover the macro once deleted. If the macro is created by a start-up script, it will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Удалить выбранный макрос. Эта операция необратима. Если же макрос был создан сценарием, запускающимся при загрузке, то он появится при следующем запуске Scribus.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&amp;Set Accel</source>
        <translation>У&amp;казать акс.</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+к</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Set the keyboard shortcut for the selected macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Указать горячие клавиши для выбранного макроса.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Set the keyboard shortcut (accelerator) key of the selected macro. You will be prompted for the new shortcut in a dialog box.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Указать горячие клавиши для выбранного макроса. Вам нужно будет ввести сочетание клавиш в диалоговом окне.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>E&amp;xport</source>
        <translation>&amp;Экспортировать</translation>
    </message>
    <message>
        <source>Alt+X</source>
        <translation>Alt+э</translation>
    </message>
    <message>
        <source>Export macros to a file.</source>
        <translation>Экспортировать макрос в файл.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Export macros to an external file. The file will be a Python script containing the scripter commands to re-create the macros. It can be run using &lt;tt&gt;Load extension script&lt;/tt&gt; from the &lt;tt&gt;Script&lt;/tt&gt; menu, or the import button in the macro manager.&lt;/p&gt;
&lt;p&gt;If you want a nice, human readable version of your macros, select the macro you want, press the &lt;tt&gt;Edit&lt;/tt&gt;  button, and use the &lt;tt&gt;Save source&lt;/tt&gt; button in the &lt;tt&gt;Edit Macro&lt;/tt&gt; dialog. You won&apos;t be able to load that version with &lt;tt&gt;Load extension script&lt;/tt&gt; - instead, create a new macro with the&lt;tt&gt; New&lt;/tt&gt; button and use &lt;tt&gt;Load source&lt;/tt&gt;.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Экспортировать макрос во внешний файл. Этот файл будет сценарием на Python, содержащим команды сценариста для воссоздания  макроса. Он может быть в дальнейшем запущен командой  &lt;tt&gt;Загрузить сценарий расширения&lt;/tt&gt; из меню &lt;tt&gt;Сценарии&lt;/tt&gt;, либо нажатием кнопки &quot;Импортировать&quot; в диалоге управления макросами.&lt;/p&gt;
&lt;p&gt;Если вам нужна красивая, читабельная версия макроса, вызовите диалог правки макроса и сохраните его код во внешний файл. Помните, что вы не сможете затем загрузить этот код через функцию &lt;tt&gt;Загрузить сценарий расширения&lt;/tt&gt;. Вместо этого вам придётся создать новый макрос и загрузить в него этот исходный код.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Delete &amp;All</source>
        <translation>Удалить &amp;все</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+в</translation>
    </message>
    <message>
        <source>Delete all macros.</source>
        <translation>Удалить все макросы.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete all registered macros. This is instant, and there is no way to recover the deleted macros. Any macros created by your start-up script will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Удалить все зарегистрированные макросы. Это действие необратимо и способа восстановить удалённые макросы нет. Все макросы, созданные стартовым сценарием, появятся при следующей загрузке Scribus.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>И&amp;мпортировать</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+м</translation>
    </message>
    <message>
        <source>Import macros from a file.</source>
        <translation>Импортировать макрос из файла.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Loads macros from an external file.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Загрузить макрос из внешнего файла.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Close this dialog</source>
        <translation>Закрыть этот диалог</translation>
    </message>
    <message>
        <source>Return to Scribus</source>
        <translation>Вернуться в Scribus</translation>
    </message>
    <message>
        <source>&lt;p&gt;Edit the selected macro. &lt;/p&gt;
&lt;p&gt;If this button is greyed out, either there is no selected macro or the macro manager does not have the source code for the macro you have selected (in which case &lt;tt&gt;No&lt;/tt&gt; will be shown in the &lt;tt&gt;Edit &lt;/tt&gt;column of the macro).&lt;/p&gt;
&lt;p&gt;If Scribus doesn&apos;t have the source, the macro was probably created by a script.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Изменить выбранный макрос. &lt;/p&gt;
&lt;p&gt;Если кнопка недоступна и надпись на ней серого цвета, значит либо не выбран макрос, либо для макроса нет исходного кода, в случае чего в столбце &lt;tt&gt;Правка&lt;/tt&gt; будет стоять значение &lt;tt&gt;Нет&lt;/tt&gt;.&lt;/p&gt;
&lt;p&gt;Если для макроса нет исходного кода, то, вероятно, этот макрос был создан сценарием.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Scribus Macro Files (*.pymacro)</source>
        <translation>Файлы макросов Scribus (*.pymacro)</translation>
    </message>
    <message>
        <source>Open File Dialog</source>
        <translation>Диалог открытия файлов</translation>
    </message>
    <message>
        <source>Select the macro file to load.</source>
        <translation>Выберите исполняемый сценарий.</translation>
    </message>
    <message>
        <source>Save File Dialog</source>
        <translation>Диалог сохранения файлов</translation>
    </message>
    <message>
        <source>Save all macros</source>
        <translation>Сохранить все макросы</translation>
    </message>
    <message>
        <source>Scribus - Rename Macro</source>
        <translation>Scribus - Смена имени макроса</translation>
    </message>
    <message>
        <source>Enter new name: </source>
        <translation>Введите новое имя:</translation>
    </message>
    <message>
        <source>Scribus - Set Macro Shortcut</source>
        <translation>Scribus - Правка клавиш вызова макроса</translation>
    </message>
    <message>
        <source>Enter new shortcut: </source>
        <translation>Введите новое сочетание:</translation>
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
        <source>&lt;p&gt;This table lists the macros that are currently defined.&lt;/p&gt;

&lt;p&gt;&lt;b&gt;Macro:&lt;/b&gt; The name of the macro, as shown in the menu bar and in other places around Scribus.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Edit:&lt;/b&gt; If the macro can be edited, &quot;Yes&quot; appears in this column. Usually if a macro cannot be edited it was created using the register_macro command in a script.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Accel:&lt;/b&gt; The menu shortcut key sequence, if any, associated with the macro. For example, CTRL-F8 means that you can press Control-F8 when in Scribus to run the macro.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Description:&lt;/b&gt; If the macro contains a &quot;docstring&quot;, a special string at the start of its definition that describes it, that is shown here. If the docstring is long, only the beginning is shown - use &quot;What&apos;s This&quot; on the macro&apos;s entry in the Macro menu to see the full description.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Многократное дублирование</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
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
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Change...</source>
        <translation>Изменить...</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Импортировать</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отменить</translation>
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
        <source>Import Template</source>
        <translation>Импортировать шаблон</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Импортировать страницы</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation>Из документа:</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation>Импортировать страницы:</translation>
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
        <source> from 0</source>
        <translation>из 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Создать страницы</translation>
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
        <source> from %1</source>
        <translation>из %1</translation>
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
        <source>to:</source>
        <translation>до:</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
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
        <source>Custom Spacing</source>
        <translation>Настраиваемые расстояния</translation>
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
        <translation>Стилей нет</translation>
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
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Имя &quot;%1&quot; уже использовано.
Выберите другое.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
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
        <source>Shade:</source>
        <translation>Тень:</translation>
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
        <source>Color of text stroke</source>
        <translation>Цвет контура текста</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Цвет заливки текста</translation>
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
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
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
        <translation>Использовать &amp;площадку (BB)</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>&amp;Контурная линия</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>&amp;Кегль шрифта:</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation>Между с&amp;трок:</translation>
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
        <source>Trac&amp;king:</source>
        <translation>Тре&amp;кинг:</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Ручной трекинг</translation>
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
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Имя &quot;%1&quot; уже использовано.
Выберите другое.</translation>
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
    <name>MusterSeiten</name>
    <message>
        <source>Edit Templates</source>
        <translation>Правка шаблонов</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Копия %1</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation>Новый шаблон</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Копия #%1 из </translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Имя:</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Из файла</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новый</translation>
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
        <source>&amp;Close</source>
        <translation>&amp;Закрыть</translation>
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
        <source>Do you really want to delete this Template?</source>
        <translation>Вы действительно хотите удалить этот шаблон?</translation>
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
        <source>Custom</source>
        <translation>Собственный</translation>
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
        <source>Points (pts)</source>
        <translation>Пункты (pt)</translation>
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
        <source>Column Guides</source>
        <translation>Направляющие столбцов</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Миллиметры (mm)</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
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
        <source>Enable single or spread based layout</source>
        <translation>Включить одиночный или парный режим страниц</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Сделать первую страницу левой страницей документа</translation>
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
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Расстояние между направляющей нижнего поля и краем страницы.
Если включён режим парных страниц, пространство полей может 
быть использовано для просчёта правильных полей для переплёта</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Расстояние между направляющей правого поля и краем страницы.
Если страницы парные, то пространство полей можно использовать 
для расчёта брошюровки</translation>
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
        <source>&amp;Facing Pages</source>
        <translation>Парные &amp;страницы</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>&amp;Левая страница первой</translation>
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
        <source>&amp;Top:</source>
        <translation>С&amp;верху:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>С&amp;низу:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>&amp;Номер первой страницы:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Стандартная &amp;единица:</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
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
        <source>Executive</source>
        <translation>Executive</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation>Folio</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation>Ledger</translation>
    </message>
</context>
<context>
    <name>NewTm</name>
    <message>
        <source>Left Page</source>
        <translation>Левая страница</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Правая страница</translation>
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
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation>Если флажок выставлен, используются 
координаты относительно страницы, в 
противном случае -- относительно объекта.</translation>
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
</context>
<context>
    <name>OdtDialog</name>
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
        <source>OpenDocument Importer Options</source>
        <translation>Параметры импорта OpenDocument</translation>
    </message>
    <message>
        <source>Update paragraph Styles</source>
        <translation>Обновить абзацные стили</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Существующие стили в документе Scribus будут перезаписаны</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Объединить абзацные стили</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Объединить абзацные стили по атрибутам. В результате схожих абзацных стилей станет меньше, атрибуты будут унаследованы, даже если в оригинальном документе стили называются иначе.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Добавить название документа в начало абзацного стиля документа Scribus.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Сделать эти настройки стандартными и больше никогда про них не спрашивать.</translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation>Консоль для сценариев</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Export Range</source>
        <translation>Диапазон экспорта</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Параметры</translation>
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
        <source> dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Общее</translation>
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
        <source>Fonts to embed:</source>
        <translation>Внедряемые шрифты:</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Страница</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Эффекты</translation>
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
        <translation>Dissolve</translation>
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
        <translation>Wipe</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Горизонтально</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Вертикально</translation>
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
        <source>Passwords</source>
        <translation>Пароли</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Вывод на печать</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Сплошные тона:</translation>
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
        <source>PDF/X-3 Output Intent</source>
        <translation>Тип вывода в PDF/X-3</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Рамка под обрез</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Сохранить как</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Просмотра с монитора / Web</translation>
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
        <translation>Не задано</translation>
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
        <source>Top-left to Bottom-Right</source>
        <translation>Из верхнего левого в правый нижний</translation>
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
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>Как долго отображается страница</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Продолжительность действия эффекта.
Меньшее число ускорит его, большее - замедлит.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Применить выбранный эффект ко всем страницам.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation>Основной пароль, которым защищены все параметры 
безопасности экспортируемого PDF-файла</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Встроить цветовой профиль для сплошных тонов</translation>
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
        <source>&amp;General</source>
        <translation>&amp;Общие</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Шрифты</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation>Здесь определяется брошюровка страниц в PDF. Если вы не знаете, 
что это такое, то лучше оставить стандартную настройку: &quot;Слева&quot;.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation>Если параметр включён, Scribus создаст миниатюры для 
каждой страницы в PDF, которые могут использоваться 
для навигации по документу в некоторых программах.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Если параметр включён, Scribus создаст PDF-Articles, 
что полезно для навигации по связанным статьям в PDF.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation>Встроить закладки, созданные в документе. 
Это позволит удобно перемещаться по PDF-документу.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation>Экспортировать разрешение текста и векторной 
графики. На растровые изображения, подобные 
фотографиям, это не распространяется.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>dpi (кол-во точек на дюйм) при экспорте изображений</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation>Встраивать шрифтовые файлы в документы PDF. 
Встраивание этих файлов позволит сохранить вид 
документа при чтении с любого компьютера.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation>Разрешить презентационные эффекты
при использовании Acrobat Reader 
в полноэкранном режиме.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Показывать миниатюры каждой 
указанной выше страницы.</translation>
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
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Здесь определяются настройки безопасности в экспортируемом PDF.
Выберите Acrobat 4.0, если вам нужно 40-битное шифрование.
Выберите Acrobat 5.0, если вам нужно 128-битное шифрование.
Помните, что шифрование в PDF не настолько надёжно, как системы 
шифрования GPG или PGP.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation>Цветовая модель для вывода в PDF.
&quot;Экран/Веб&quot; подходит для просмотра документов с 
экрана и печати на обычных струйных принтерах.
&quot;Принтер&quot; необходимо выбрать при печати на 
настоящий 4-цветный CMYK принтер.</translation>
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
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Обязательная строка для PDF/X-3 или PDF не пройдёт 
тест на соответствие требованиям PDF/X-3. 
Рекомендуется использовать название документа.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation>Уровни сжатия: Минимальный (25%), Низкий (50%), 
Средний (75%), Высокий (85%), Максимальный (95%)</translation>
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
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation>Разрешить копирование текста и графики из PDF.
При невыставленном флажке копировать  
текст и графику в PDF будет невозможно.</translation>
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
        <source>O&amp;utput to File:</source>
        <translation>Вы&amp;вод в файл:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Изменить...</translation>
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
        <source>Compatibilit&amp;y:</source>
        <translation>Сов&amp;местимость:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>П&amp;ереплёт:</translation>
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
        <source>&amp;Resolution:</source>
        <translation>&amp;Разрешение:</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation>&amp;Метод:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Качество:</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation>Понизить р&amp;азрешение 
изображений до:</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>Встроить &amp;все шрифтовые файлы</translation>
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
        <source>Show Page Pre&amp;views</source>
        <translation>Показывать вид &amp;страниц</translation>
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
        <source>&amp;Apply Effect on all Pages</source>
        <translation>&amp;Применить эффект 
ко всем страницам</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>Использовать &amp;шифрование</translation>
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
        <source>Output &amp;Intended For:</source>
        <translation>Назначение в&amp;ывода:</translation>
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
        <source>Use ICC Profile</source>
        <translation>Использовать ICC-профиль</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>Ц&amp;вет</translation>
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
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Сохранить</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Это дополнительный параметр, отключенный по умолчанию. 
Он включается в особых случаях, когда этого требует принтер, 
причём вам сообщаются все детали. В противном случае 
экспортированный вами PDF-файл может не распечататься 
корректно и не отображаться одинаково во всех 
операционных системах.</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF-документы (*.pdf);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation>Сжать текст и ве&amp;кторную графику</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation>Включить п&amp;резентационные эффекты</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation>&amp;Презентация</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Поворот:</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation>&amp;Вычесть неиспользуемые знаки из всех шрифтовых файлов</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation>Из каких шрифтовых файлов вычесть знаки:</translation>
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
        <source>Save as PDF</source>
        <translation>Сохранить как PDF</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management must be enabled to use PDF/X-3. You can enable color management from the Settings menu.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Для использования PDF/X-3 должно быть включено управление цветом. Оно включается через меню &quot;Настройки&quot;.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF/X-3 is supported and enabled, but can not be used for the selected PDF version. If you want to use PDF/X-3, you need to set PDF/X-3 as your PDF version (compatibility level).&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Стандарт PDF/X-3 поддерживается и может быть использован, но не с указанной версией PDF. Для использования PDF/X-3 выберите сам PDF/X-3 в качестве версии PDF (параметр &quot;Совместимость&quot; на вкладке &quot;Общие&quot;).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF/X-3 is not supported by this Scribus build (CMS support not present).&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;PDF/X-3 не поддерживается в этой сборке Scribus (нет поддержки управления цветом).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fields is prevented.</source>
        <translation>Разрешить добавление аннотаций и полей в PDF.
Если параметр отключён, редактирование 
аннотаций и полей также запрещён.</translation>
    </message>
    <message>
        <source>PDF security settings</source>
        <translation>Настройки безопасности PDF</translation>
    </message>
    <message>
        <source>PDF/X-3 settings</source>
        <translation>Настройки PDF/X-3</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF security can not be used with PDF/X-3. If you want to turn on security, change your PDF version (compatibility level) to something other than PDF/X-3.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Функции безопасности в PDF не могут использоваться с PDF/X-3. Для их использования смените версию PDF на любую другую (параметр &quot;Совместимость&quot; на вкладке &quot;Общие&quot;).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source> Lossless compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation>Сжатие текста и графики без потерь.
При включённой опции размер PDF снижает.
не стоит отключать её без особых причин.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic, unless you have a need for special compression options.</source>
        <translation>Способ сжатия изображений. Автоматический - Scribus сам выбирает подходящий способ. ZIP - подходит для изображений со сплошными тонами. JPEG - больше подходит для создания PDF-файлов меньшего размера с большим количеством фотографий (и по возможности небольшой потерей в качестве). Для типичных задач рекомендуется оставить автоматический выбор типа сжатия.</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.
This can increase memory usage and slow down export.</source>
        <translation>Изменить разрешение растровых изображений до указанного.
Если флажок не выставлен, будут использованы исходные 
значения количества точек на дюйм, из-за чего может вырасти 
потребление памяти и замедлиться скорость экспорта.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility. Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption. PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Note: PDF/X-3 will convert all CMYK images to ICC based RGB colors. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation>Здесь определяется совместимость с PDF разных версий. Выберите Acrobat 4.0, если вам нужна максимальная совместимость. Выберите Acrobat 5.0, если в документе используются особенности PDF 1.4 (например, полупрозрачность) или 128-битное шифрование. Выберите PDF/X-3 для серьёзных коммерческих заказов, требующих управления цветом (этот вариант доступен при включённой 
системе управления цветом).</translation>
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
        <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation>Включить более качественное отображение текста при просмотре 
ценой замедления прорисовки. Работает только со шрифтами Type1.</translation>
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
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Способ удаления некоторых серых (ахроматических) тонов, 
составленных из голубой, желтой и пурпурной красок, 
и замены их на черный. UCR главным образом воздействует на 
нейтральные по цвету части изображения. Использование этого 
метода способно улучшить печать некоторых изображений, но 
каждый отдельный случай требует особого рассмотрения. 
При использовании этого способа также снижается 
вероятность избыточной яркости изображения.</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Включить более качественное отображение шрифтов TrueType, Open Type Fonts,
EPS, PDF и векторной графики при просмотре ценой замедления прорисовки</translation>
    </message>
</context>
<context>
    <name>Page</name>
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
        <source>Edit Text...</source>
        <translation>Изменить текст...</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Текстовая рамка</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
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
        <source>Text on a Path</source>
        <translation>Текст по контуру</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Абзацев:</translation>
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
        <source>Linked Text</source>
        <translation>Связанный текст</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Печать:</translation>
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
        <source>The Program</source>
        <translation>Программа</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation>отсутствует!</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Вст&amp;авить</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Показать &amp;поля</translation>
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
        <translation>Показать &amp;сетку</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Показать &amp;направляющие</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Показать опорную &amp;сетку</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Привязать к &amp;сетке</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Привязать к н&amp;аправляющим</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Исходный PPI:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Текущий PPI:</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>&amp;Инфо</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation>Получить &amp;изображение...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>Изображение &amp;видимо</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation>&amp;Обновить изображение</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation>&amp;Изменить изображение</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation>&amp;Подстроить рамку до изображения</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>Получить &amp;текст...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation>Добавить &amp;текст из файла...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation>Изменить те&amp;кст...</translation>
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
        <source>&amp;PDF Options</source>
        <translation>Пара&amp;метры PDF</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>Заб&amp;локировать</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>&amp;Разблокировать</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation>За&amp;блокировать размер объекта</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation>Разблокиро&amp;вать размер объекта</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Передать в запа&amp;сник</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Переложить в сло&amp;й</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation>Встав&amp;ить lorem ipsum</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>С&amp;группировать</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation>Разгру&amp;ппировать</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>Уро&amp;вень</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>Отправить на самый &amp;низкий уровень</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation>Отправить на самый &amp;высокий уровень</translation>
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
        <source>&amp;Picture Frame</source>
        <translation>Рамку изобра&amp;жения</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation>Многоу&amp;гольник</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation>&amp;Контур</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Текстовую рамку</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>Кривая &amp;Безье</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>&amp;Преобразовать в</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Вырезать</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Скопировать</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>У&amp;далить</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>&amp;Очистить содержимое</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation>Показать &amp;свойства...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation>Скрыть &amp;свойства...</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Вы действительно хотите удалить весь текст?</translation>
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
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation>Страница</translation>
    </message>
    <message>
        <source> of %1</source>
        <translation>из %1</translation>
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
        <source>Pictures</source>
        <translation>Изображения</translation>
    </message>
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
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Свойства многоугольника</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>&amp;Углы:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Поворот:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>&amp;Коэфф.:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
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
        <source>Sample Polygon</source>
        <translation>Пример многоугольника</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Использовать к&amp;эфф.</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Использовать коэффициент для создания многоугольника 
с выпуклыми или вогнутыми сторонами</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation>Отрицательное значение сделает многоугольник вогнутым 
(в форме звезды), а положительное - выпуклым</translation>
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
        <source>Units</source>
        <translation>Единицы измерения</translation>
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
        <source>Menus</source>
        <translation>Меню</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Пути</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Размер страницы</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Собственный</translation>
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
        <source>Grid Layout</source>
        <translation>Внешний вид сетки</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation>Цвета сетки</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation>Расположение</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Нижний индекс</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Верхний индекс</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Капитель</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Другое</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>А ещё неплохо бы на зависть другим уметь красиво читать и писать</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
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
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation>Для подстройки отображения листа перетащите ползунок внизу в нужную позицию.</translation>
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
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Миллиметры (mm)</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Внешние инструменты</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation>Разное</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation type="obsolete">Интерпретатор Postscript</translation>
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
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation>Выберите одну из доступных тем интерфейса, 
наследуемую Scribus у KDE и Qt</translation>
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
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Радиус области, внутри которой можно захватывать точки контроля над объектом</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Количество недавно открытых документов, отображаемых в меню &quot;Файл&quot;</translation>
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
        <source>Enable single or spread based layout</source>
        <translation>Включить одиночный или парный режим страниц</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation>Сделать первую страницу левой страницей документа</translation>
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
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Расстояние между направляющей нижнего поля и краем страницы.
Если включён режим парных страниц, пространство полей может 
быть использовано для просчёта правильных полей для переплёта</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation>Если параметр включён, Scribus сохранит резервную копию в файле 
с расширением .bak по истечении указанного периода времени</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Период времени между автоматическим сохранением</translation>
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
        <translation>Расстояние, начиная с которого объект начнёт &quot;прилипать&quot; к направляющим линиям</translation>
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
        <source>Place the grid behind your page objects</source>
        <translation>Расположите сетку подо всеми объектами страницы</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation>Расположите сетку надо всеми объектами страницы</translation>
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
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Увеличение междустрочного интервала в процентах размера шрифта</translation>
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
        <translation>Свойства очертания</translation>
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
        <translation>Цвет линий очертаний</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Насыщение цвета линий</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Цвет заливки очертаний</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Стиль линий очертаний</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Толщина линий очертаний</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Включено минимально возможное &quot;прилипание&quot;</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Включено максимально возможное &quot;прилипание&quot;</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Изменить &quot;прилипание&quot; для каждого шага масштабирования</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Цвет линий</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Насыщение линий</translation>
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
        <source>Number of corners for polygons</source>
        <translation>Количество углов в многоугольнике</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Градусы вращения многоугольников</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Пример многоугольника</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Изменить размер миниатюры в запаснике</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation>Если страницы указаны как парные, разбрасывать 
их отображение по соответствующим сторонам</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Цвет бумаги</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Цвет линий полей</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Пометить пространство за полями цветом полей</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation>Включить полупрозрачность при экспорте в PDF 1.4</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Указать стандартный масштаб вида страницы</translation>
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
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Расстояние между направляющей правого поля и краем страницы.
Если страницы парные, то пространство полей можно использовать 
для расчёта брошюровки</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Сохранять содержимое запасника при каждом его изменении</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation>Сетка линии шрифта</translation>
    </message>
    <message>
        <source> px</source>
        <translation>px</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Тема:</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Размер &amp;шрифта:</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation>Настройки мыши</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Смещение по 
колесу мыши:</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation>Радиус &amp;захвата:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>Сколько &amp;недавних 
документов помнить:</translation>
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
        <source>&amp;Facing Pages</source>
        <translation>Парные &amp;страницы</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>&amp;Левая страница первой</translation>
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
        <source>&amp;Enabled</source>
        <translation>&amp;Включено</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Интервал:</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation>Ширина &amp;маленьких ячеек:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation>Ширина &amp;больших ячеек:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation>Расстояние для привязки:</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation>Цвет маленьких ячеек:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation>Цвет больших ячеек:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation>Цвет &amp;собственных 
направляющих:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation>Цвет &amp;опорной сетки:</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation>На &amp;заднем плане</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation>На &amp;переднем плане</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation>В&amp;кл</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation>В&amp;ыкл</translation>
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
        <source>D&amp;isplacement:</source>
        <translation>Смеще&amp;ние:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>&amp;Масштабирование:</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Ма&amp;сштабирование:</translation>
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
        <source>Automatic &amp;Line Spacing:</source>
        <translation>Автомат. межстрочное
расстоя&amp;ние:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation>Стандартный &amp;шрифт:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation>Стандартный &amp;кегль:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation>Цвет &amp;текста:</translation>
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
        <source>&amp;Line Color:</source>
        <translation>&amp;Цвет линии:</translation>
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
        <source>&amp;Type of Line:</source>
        <translation>Тип ли&amp;нии:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>То&amp;лщина линии:</translation>
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
        <translation>&amp;Шаг:</translation>
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
        <translation>Масштабировать &amp;изображение до рамки</translation>
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
        <source>Corn&amp;ers:</source>
        <translation>&amp;Углы:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Поворот:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>К&amp;оэфф:</translation>
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
        <source>Display Pages &amp;Side by Side</source>
        <translation>Показывать страницы р&amp;ядом друг с другом</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation>Цвета на странице</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation>Задний &amp;план:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation>По&amp;ля:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Показывать &amp;непечатаемую область цветом полей</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation>Использовать возможности &amp;полупрозрачности PDF 1.4</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
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
        <source>T&amp;emplates:</source>
        <translation>&amp;Шаблоны:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Изменить...</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation>Дополнительный каталог для шаблонов макетов</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Использовать &amp;коэфф.</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Использовать коэффициент для создания многоугольника 
с выпуклыми или вогнутыми сторонами</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation>Отрицательное значение сделает многоугольник вогнутым 
(в форме звезды), а положительное - выпуклым</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Способ удаления некоторых серых (ахроматических) тонов, 
составленных из голубой, желтой и пурпурной красок, 
и замены их на черный. UCR главным образом воздействует на 
нейтральные по цвету части изображения. Использование этого 
метода способно улучшить печать некоторых изображений, но 
каждый отдельный случай требует особого рассмотрения. 
При использовании этого способа также снижается 
вероятность избыточной насыщенности изображения. </translation>
    </message>
    <message>
        <source>Executive</source>
        <translation>Executive</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation>Folio</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation>Ledger</translation>
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
        <source>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend &apos;gimp-remote&apos;,
as it allows you to edit the image in an already running
instance of gimp.</source>
        <translation>Здесь указывается команда запуска графического редактора. 
Если вы используете GIMP, мы рекомендуем указать команду 
&apos;gimp-remote&apos;, поскольку тогда изображение откроется в 
существующей сессии GIMP, и запускать редактор по новой 
не придётся.</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot
be changed with documents open.</source>
        <translation>Стандартный каталог с ICC-профилями.
Если открыт хотя бы один документ, 
его нельзя изменить.</translation>
    </message>
    <message>
        <source>Turns on the base grid</source>
        <translation>Включить опорную сетку</translation>
    </message>
    <message>
        <source>Turns off the base grid</source>
        <translation>Выключить опорную сетку</translation>
    </message>
    <message>
        <source>File system location for the Ghostscript interpreter</source>
        <translation type="obsolete">Местоположение интерпретатора Ghostscript в системе</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File system location for the GhostScript interpreter</source>
        <translation type="unfinished"></translation>
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
        <source>Online Reference</source>
        <translation>Электронный справочник</translation>
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
        <source>Document</source>
        <translation>Документ</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Фон</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation>Просмотреть вид печати</translation>
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
        <source>Error writting the output file(s).</source>
        <translation>Произошла ошибка при записи результируемых файлов.</translation>
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
        <source>New &amp;from Template...</source>
        <translation>Созд&amp;ать из шаблона...</translation>
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
        <source>Save as &amp;Template...</source>
        <translation>Сохранить как &amp;шаблон...</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation>&amp;Вставить символ...</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Сохранить как &amp;изображение...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation>Просмотреть &amp;шрифты</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>Просмотреть пе&amp;чать</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation>&amp;Руководство по сценариям</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Сохран&amp;ить страницу как &amp;SVG...</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Импортировать &amp;SVG...</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Импортировать &amp;EPS/PS...</translation>
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
        <source>Template: </source>
        <translation>Шаблон:</translation>
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
        <source>Tried to set progress &gt; maximum progress</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Невозможно получить цвет с пустым именем.</translation>
    </message>
    <message>
        <source>Color not found</source>
        <comment>python error</comment>
        <translation>Цвет не найден</translation>
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
        <source>Failed to open document</source>
        <comment>python error</comment>
        <translation>Не удалось открыть документ</translation>
    </message>
    <message>
        <source>Failed to save document</source>
        <comment>python error</comment>
        <translation>Не удалось сохранить документ</translation>
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
        <source>An object with the requested name already exists</source>
        <comment>python error</comment>
        <translation>Объект с запрошенным именем уже существует</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values)</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must contain an even number of values</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values)</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values)</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object not found</source>
        <comment>python error</comment>
        <translation>Объект не найден</translation>
    </message>
    <message>
        <source>Style not found</source>
        <comment>python error</comment>
        <translation>Стиль не найден</translation>
    </message>
    <message>
        <source>Can&apos;t set style on a non-text frame</source>
        <comment>python error</comment>
        <translation>Невозможно назначить стиль нетекстовой рамке</translation>
    </message>
    <message>
        <source>Failed to save EPS</source>
        <comment>python error</comment>
        <translation>Не удалось сохранить EPS-файл</translation>
    </message>
    <message>
        <source>Page number out of range</source>
        <comment>python error</comment>
        <translation>Количество страниц вне допустимого диапазона</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line style not found</source>
        <comment>python error</comment>
        <translation>Стиль линии не найден</translation>
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
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Импортировать  &amp;OpenOffice.org Draw...</translation>
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
        <translation>Цвет не найден - ошибка интерпретатора Python</translation>
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
- Many functions will raise a NoDocOpenError if you try to use themwithout a document to operate on.
- If you do not pass a frame name to a function that requires one,the function will use the currently selected frame, if any, orraise a NoValidObjectError if it can&apos;t find anything to operateon.
- Many functions will raise WrongFrameTypeError if you try to use them
on a frame type that they do not make sense with. For example, setting
the text colour on a graphics frame doesn&apos;t make sense, and will result
in this exception being raised.
- Errors resulting from calls to the underlying Python API will be
passed through unaltered. As such, the list of exceptions thrown by
any function as provided here and in its docstring is incomplete.

Details of what exceptions each function may throw are provided on the
function&apos;s documentation.</source>
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
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation>&amp;Короткие слова...</translation>
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
        <translation>Файл шрифта не найден.</translation>
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
        <translation>Невозможно удалить последний слой.</translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>Невозможно создать слой без имени.</translation>
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
        <source>Object is not a linked text frame, cannot unlink.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object the last frame in a series, cannot unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Portuguese (Brazilian)</source>
        <translation>На бразильский португальский</translation>
    </message>
    <message>
        <source>Cannot get a colour with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unable to save pixmap.</source>
        <comment>scripter error</comment>
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
        <source>Croatian</source>
        <translation>Хорватский</translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation>Текстовые документы OpenDocument</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation>страница</translation>
    </message>
    <message>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
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
    <name>Query</name>
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
        <source>Enable single or spread based layout</source>
        <translation>Включить одиночный или парный режим страниц</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Сделать первую страницу левой страницей документа</translation>
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
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Расстояние между направляющей нижнего поля и краем страницы.
Если включён режим парных страниц, пространство полей может 
быть использовано для расчёта правильных полей для переплёта.</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Расстояние между направляющей правого поля и краем страницы.
Если включён режим парных страниц, пространство полей может 
быть использовано для расчёта правильных полей для переплёта.</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
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
        <source>&amp;Facing Pages</source>
        <translation>Парные &amp;страницы</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>&amp;Левая страница первой</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Изнутри:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation>Снару&amp;жи:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Размер страницы</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Размер:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Собственный</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation>Ориентация:</translation>
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
        <source>Width:</source>
        <translation>Ширина:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation>Высота:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>&amp;Номер первой страницы:</translation>
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
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Настройки символа</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Tracking:</source>
        <translation>Трекинг:</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Ручной трекинг</translation>
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
    <name>ScribusApp</name>
    <message>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Идёт поиск шрифтов</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation>Выход</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Критическая ошибка</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation>Разумные переносы</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation>Выключка по левой стороне</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation>Выключка по правой стороне</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation>Выключка по центру</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Вставить номер страницы</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation>Направить текст по контуру</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation>Показать Слои</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation type="obsolete">Javascripts...</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation>Откатить</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation>Показать Палитру страниц</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation>Блокировать/Разблокировать</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation>Неразрывный пробел</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Чтение настроек</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation>Запуск модуля переносов</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Установка горячих клавиш</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Чтение запасника</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Запуск модулей</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Новый</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation>Открыть...</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Сохранить</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation>Сохранить как...</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation>Получить Текст/Изображение...</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation>Документ...</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation>Параметры документа...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Напечатать...</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Выйти</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Вырезать</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Скопировать</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Вставить</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Очистить</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation>Выделить всё</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation>Цвета...</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation>Стили...</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation>Шаблоны...</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation>Шрифты...</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation>Выбрать новый шрифт</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation>Продублировать</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Продублировать многократно</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Группировать</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation>Разгруппировать</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Блокировать</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation>Послать на самый низкий уровень</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation>Послать на самый высокий уровень</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation>Сделать уровнем ниже</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation>Сделать уровнем выше</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation>Распространить / Выровнять...</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation>Вставить...</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation>Удалить...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation>Переместить...</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation>Применить шаблон...</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation>Настроить направляющие...</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation>Уместить в окне</translation>
    </message>
    <message>
        <source>50%</source>
        <translation>50%</translation>
    </message>
    <message>
        <source>75%</source>
        <translation>75%</translation>
    </message>
    <message>
        <source>200%</source>
        <translation>200%</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation>Миниатюры</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation>Скрыть поля</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation>Скрыть обрамление</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation>Скрыть изображения</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation>Показать сетку</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation>Привязать к сетке</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Настроить изображения</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation>Вставить переносы</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation>О Scribus</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>О Qt</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation>Электронная помощь...</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Стиль</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Инструменты</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Подчёркнутый</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Зачёркнутый</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Капитель</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Верхний индекс</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Нижний индекс</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Контурный</translation>
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
        <source>Ready</source>
        <translation>Готово</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ничего</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation>Получить изображение...</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Цвет</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Инвертировать</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Получить текст...</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Шрифт</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Размер</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Тень</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Разблокировать</translation>
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
        <source>Loading...</source>
        <translation>Файл загружается...</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Текстовый файлы (*.txt);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation>Невозможно записать файл:
%1</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Сохранить как</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Документы (*.sla *.sla.gz *.scd *.scd.gz);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Файл сохраняется...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Документ выводится на печать...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Документ</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Печать не удалась!</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Руководство по Scribus</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation>Следующие программы отсутствуют:</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Все</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation type="obsolete">EPS-изображения (*.eps);;Все файлы (*)</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation>Загружается:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Идёт подстройка цветов</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Английский</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Немецкий</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Испанский</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Итальянский</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Французский</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Русский</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Датский</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Словацкий</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Венгерский</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Чешский</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Датский</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Португальский</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation>Украинский</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Польский</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Греческий</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Каталонский</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Выберите каталог</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Свойства</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Схема</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Запасник</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>&quot;Обрушивание&quot; Scribus</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus обрушился по причине #%1</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation>Создать новый документ</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation>Открыть документ</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation>Сохранить текущий документ</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation>Закрыть текущий документ</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation>Напечатать текущий документ</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation>Сохранить текущий документ в PDF</translation>
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
        <source>Finnish</source>
        <translation>Финский</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation>Ирландский</translation>
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
        <source>&amp;Item</source>
        <translation>&amp;Объект</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>С&amp;траница</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Вид</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>С&amp;ервис</translation>
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
        <source>Show Baseline Grid</source>
        <translation>Показать опорную сетку</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation>Скрыть опорную сетку</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation>Некоторые объекты заблокированы</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отменить</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation>Заблокировать все</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation>Разблокировать все</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
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
        <source>Lithuanian</source>
        <translation>Литовский</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Шведский</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Словенский</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Настройки</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation>&amp;Управление цветом...</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>Созд&amp;ать</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Открыть...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>&amp;Недавние</translation>
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
        <translation>Сохранить &amp;как</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>&amp;Вернуть</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Со&amp;брать для вывода...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation>По&amp;лучить Текст/Изображение...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>&amp;Добавить текст...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Импортировать</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Сохранить &amp;текст...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Сохран&amp;ить как EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Сохр&amp;анить как PDF...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Экспортировать</translation>
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
        <source>Cu&amp;t</source>
        <translation>&amp;Вырезать</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Скопировать</translation>
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
        <source>Select &amp;All</source>
        <translation>Выделить в&amp;сё</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Найти/Заменить</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>Ц&amp;вета...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>Стили &amp;абзаца...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>Стили &amp;линий...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation>&amp;Шаблоны...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;Javascripts...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Продублировать</translation>
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
        <translation>&amp;Сгруппировать</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>&amp;Разгруппировать</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>&amp;Заблокировать</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>Послать на самый &amp;низкий уровень</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation>Послать на самый &amp;высокий уровень</translation>
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
        <source>Distribute/&amp;Align...</source>
        <translation>Распространить / Выр&amp;овнять...</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Очертания</translation>
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
        <source>C&amp;onvert to Outlines</source>
        <translation>Преобразовать в &amp;контур</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Вставить...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Удалить...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Переместить</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation>Применить &amp;шаблон...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation>&amp;Уместить в окне</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation>&amp;100&amp;</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation>&amp;Миниатюры</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Показать страничную &amp;сетку</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Привязать к н&amp;аправляющим</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Показать опорную &amp;сетку</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Свойства</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation>С&amp;труктура</translation>
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
        <source>P&amp;age Palette</source>
        <translation>&amp;Палитра страниц</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Закладки</translation>
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
        <source>Toolti&amp;ps</source>
        <translation>&amp;Подсказки</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>PDF-&amp;инструменты</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation>Подсказки</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation>&amp;Шрифты...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation>&amp;Модуль переносов...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation>&amp;Горячие клавиши...</translation>
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
        <source>Scribus &amp;Manual...</source>
        <translation>Ру&amp;ководство по Scribus...</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>Ст&amp;иль</translation>
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
        <source>&amp;Other...</source>
        <translation>Друг&amp;ое...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Каскадом</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Название</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Цвет</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation>&amp;Инвертировать</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>&amp;Получить текст...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Шрифт</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Кегль</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Эффекты</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Выключка</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Тень</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>Т&amp;абуляторы...</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>&amp;Разблокировать</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Показывать &amp;изображения</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Показать &amp;поля</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Показывать &amp;обрамление</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Показать &amp;направляющие</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation type="obsolete">Ghostscript : Вы не можете использовать изображения в формате EPS</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation>Импортировать &amp;страницы...</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Происходит импорт страниц...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Импорт страниц</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation>&lt;p&gt;Вы пытаетесь импортировать большее число страниц, чем существует в данном документе начиная с текущей.&lt;/p&gt;Выберите одно из следующих вариантов:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Создать&lt;/b&gt; недостающие страницы&lt;/li&gt;&lt;li&gt;&lt;b&gt;Импортировать&lt;/b&gt; страницы до последней существующей&lt;/li&gt;&lt;li&gt;&lt;b&gt;Отменить&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Создать</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Импортировать</translation>
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
        <source>Getting ICC Profiles</source>
        <translation>Получение ICC-профилей</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation>Информация о д&amp;окументе...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>&amp;Настроить направляющие...</translation>
    </message>
    <message>
        <source>100%</source>
        <translation>100%</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Привязать к стран&amp;ичной сетке</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>&amp;Настроить Scribus...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Кегль:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Тень:</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation>Отменить уд&amp;аление объекта</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation>Отменить &amp;перемещение объекта</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation>Отменить &amp;изменение объекта</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation>Изменить &amp;очертания</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation>Файл %1 не имеет формат Scribus</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Африкаанс</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Шрифтовая подсистема инициализирована</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Португальский (Бр.)</translation>
    </message>
    <message>
        <source>There are no suitable Fonts on your System</source>
        <translation>В вашей системе подходящих шрифтов нет</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management is supported but can not currently be enabled. Make sure you have ICC color profiles installed and that the profile path in the preferences points to where they&apos;re installed.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Управление цветом поддерживается, но не включено. Проверьте, установлены ли ICC-профили и корректно ли указан путь к ним.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management is not supported by this Scribus build (not compiled in).&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Управление цветом не поддерживается в этой сборке Scribus.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Хорватский</translation>
    </message>
    <message>
        <source>JavaScripts...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>GhostScript : You cannot use EPS Images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation type="unfinished">Норвежский</translation>
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
        <source>All</source>
        <translation>Все</translation>
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
        <source>Copy of</source>
        <translation>Копия</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Документ:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>изменился с момента последнего сохранения.</translation>
    </message>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation>Всё равно в&amp;ыйти</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation>Всё равно &amp;закрыть</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation>&amp;Сохранить</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Сценарии Scribus</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Выполнить сценарий...</translation>
    </message>
    <message>
        <source>Run a Python script from a file.</source>
        <comment>scripter</comment>
        <translation>Выполнить сценарий на Python из файла.</translation>
    </message>
    <message>
        <source>&amp;Load Extension Script...</source>
        <translation>&amp;Загрузить сценарий расширения...</translation>
    </message>
    <message>
        <source>Load a Python script as an extension. Used for loading macros and for advanced Python scripts that extend the Scribus user interface.</source>
        <comment>scripter</comment>
        <translation>Загрузить сценарий на Python как расширение. Используется для загрузки макросов и расширенных сценариев на Python, которые дополняют пользовательский интерфейс Scribus.</translation>
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
        <source>Display an interactive Python console where you can write and run Python programs that use the Scripter tools.</source>
        <comment>scripter</comment>
        <translation>Показать интерактивную консоль Python, из которой можно записывать и выполнять сценарии на Python, использующие инструменты Сценариста.</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>О сц&amp;енарии...</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation>С&amp;ценарии</translation>
    </message>
    <message>
        <source>Scripter &amp;Settings</source>
        <comment>script menu</comment>
        <translation>Настроить &amp;Сценарист</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Открыть</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Сценарии на Python (*.py);; Все файлы (*)</translation>
    </message>
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
        <source>Scribus - Script Plugin</source>
        <translation>Scribus - Сценарный модуль</translation>
    </message>
    <message>
        <source>The &apos;Load Script&apos; function of the script plugin is currently disabled.
If you just want to run a normal script, you probably want to use
&apos;Execute Script...&apos; instead.

If you do actually want to load a Python extension script or macro, you
need to go into the Scripter Settings in the Script menu and enable
scripter extensions there.

Please read the documentation on extension scripts first.
</source>
        <translation>Функция &apos;Загрузить сценарий&apos; сценарного модуля в настоящее 
время отключена. Если вы хотите выполнить обычный сценарий, 
скорее всего вам захочется вместо неё использовать команду 
&apos;Выполнить сценарий...&apos;, доступную из меню &apos;Сценарии&apos;.

Еслм вам нужно загрузить сценарий расширения или макрос, то 
нужно вызвать диалог настрйоки Сценариста и включить в нём 
сценарные расширения.

Настоятельно рекомендуется предварительно прочитать 
документацию о сценариях расширений.
</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation>Спрятать &amp;консоль</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>О сценарии</translation>
    </message>
</context>
<context>
    <name>ScripterPreferences</name>
    <message>
        <source>Scribus - Scripter Preferences</source>
        <translation>Scribus - Настройки Сценариста</translation>
    </message>
    <message>
        <source>Enable Scripter Extensions</source>
        <translation>Включить расширения Сценариста</translation>
    </message>
    <message>
        <source>Turn on extension scripts and macros</source>
        <translation>Включить сценарии-расширения и макросы</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;Enabling scripter extensions turns on additional scripter functionality including Python macros and the option of loading a Python script at start-up. Turning on this option unlocks the &lt;tt&gt;Load Extension Script&lt;/tt&gt; item in the Script menu.&lt;/p&gt;
&lt;p&gt;
Only scripts written to be run as extension scripts should be used with &lt;tt&gt;Load Extension Script&lt;/tt&gt; or as start-up scripts. See the scripter documentation for more details.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;Включение сценариев расширений обеспечивает подключение дополнительной функциональности, включая макросы на языке Python и возможность запускать сценарий на Python при запуске программы. Включение этой возможности делает доступным пункт &lt;tt&gt;Загрузить сценарий расширения&lt;/tt&gt; в меню &quot;Сценарии&quot;.&lt;/p&gt;

Сценарии расширений можно вызывать только через &lt;tt&gt;Загрузить сценарий расширения&lt;/tt&gt; или в качестве сценария, запускаемого при старте программы. Настоятельно рекомендуется предварительно прочитать документацию по Сценаристу.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Use a Startup Script</source>
        <translation>Выполнить сценарий при запуске программы</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;If &lt;tt&gt;Use a Startup Script&lt;/tt&gt; is checked, Scribus will load the script file specified here as an extension script at start-up. It is important that the script be written as an extension script, as if not written carefully it can potentially cause problems.&lt;/p&gt;
&lt;p&gt;&lt;tt&gt;Use a Startup Script&lt;/tt&gt; will be disabled if scripter extensions are off, as extension scripts cannot be loaded without scripter extensions enabled.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;Если флажок &lt;tt&gt;Выполнить сценарий при запуске программы&lt;/tt&gt; выставлен, при запуске Scribus будет загружать и выполнять указанный сценарий на Python как сценарий расширения. Важно помнить, что сценарий будет записан именно как сценарий расширения, поэтому если он написан некорректно, это может стать источником головной боли у пользователя.&lt;/p&gt;
&lt;p&gt;Флажок &lt;tt&gt;Выполнить сценарий при запуске программы&lt;/tt&gt; автоматически снимается при отключении сценарных расширений, поскольку сценарии расширения не могут быть загружены при отключенных сценарных расширениях.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation>Указать...</translation>
    </message>
    <message>
        <source>Browse for a new script file</source>
        <translation>Указать файл сценария</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Browse for a new script file&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Указать файл сценария&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>What script file to load at start-up</source>
        <translation>Какой сценарий выполнить при запуске программы</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;The file containing the Python script to run as an extension script at start-up.&lt;/p&gt;
&lt;p&gt;Note that when this script is run, Scribus has not completely started up and the workspace does not yet exist.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;Файл содержит сценарий на Python, исполняемый как сценарий расширения при запуске программы.&lt;/p&gt;
&lt;p&gt;Помните, что запуск Scribus приостанавливается на время исполнения этого сценария и рабочего пространства по сути ещё не создаётся.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Script File:</source>
        <translation>Файл сценария:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+т</translation>
    </message>
    <message>
        <source>Close without saving changes</source>
        <translation>Закрыть без сохранения изменений</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+о</translation>
    </message>
    <message>
        <source>Save changes and close</source>
        <translation>Сохранить и выйти</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Дополнительные параметры</translation>
    </message>
    <message>
        <source>Import All Names at Startup</source>
        <translation>Импортировать все имена при запуске</translation>
    </message>
    <message>
        <source>Run &apos;from scribus import *&apos; in the script console at start-up</source>
        <translation>Запускать &apos;from scribus import *&apos; в консоли для сценариев при запуске</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Import All Names at Startup&lt;/tt&gt; is an advanced option. You should probably leave it checked unless you have read the documentation and know what you are doing.&lt;/p&gt;
&lt;p&gt;Unchecking this option will prevent the scripter from running its usual &lt;tt&gt;from scribus import *&lt;/tt&gt; command when it initializes the main interpreter (used for the script console and extension scripts) at start-up.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Импортировать все имена при запуске&lt;/tt&gt; - расширенная функция. Если вы не знаете, зачем она, не отключайте её, либо сначала прочитайте документацию.&lt;/p&gt;
&lt;p&gt;Отключение этой функции запретить сценаристу запускать его обычную команду &lt;tt&gt;from scribus import *&lt;/tt&gt; при инициализации основного интерпретатора  (используемого для консоли сценариев и сценариев расширения) во время запуска программы.&lt;/p&gt;
&lt;p&gt;Включение и отключение этой функции учитывается только при повторном запуске Scribus.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Enable Legacy Name Aliases</source>
        <translation>Разрешить устаревшие имена функций</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Enable the use of OldStyle function names&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Разрешить использование имён функций в старом стиле&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Enable Legacy Aliases&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;If checked, this option will cause the scripter to create a large number of function and constant name aliases for 1.2.0 script compatibility. It defaults to checked.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Использовать унаследованные имена функций&lt;/tt&gt; - это расширенная функция. Лучше оставить её включённой.&lt;/p&gt;
&lt;p&gt;Пока она включена, сценарист будет создавать много функций и постоянных псевдонимов имён для совместимости со сценариями Scribus 1.2.0. По умолчанию функция включена.&lt;/p&gt;
&lt;p&gt;Включение и отключение этой функции учитывается только при повторном запуске Scribus.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Use Fake Stdin</source>
        <translation>Эмулировать Stdin</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace sys.stdin with a fake file to prevent Scribus hanging when a script tries to read from stdin.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Заменить sys.stdin псевдофайлом, чтобы Scribus не зависал при попытке чтения из stdin.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Use Fake Stdin&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;Normally, scribus will provide Python with a fake file object for &lt;tt&gt;sys.stdin&lt;/tt&gt;, so that reads from stdin always return an empty string. If the real &lt;tt&gt;sys.stdin&lt;/tt&gt; is left in place, scripts that try to read from it will block - and in turn block scribus&apos;s execution, making the app appear to hang - until input arrives on stdin. It&apos;s unusual for GUI apps to expect anything on stdin, so mostly users will think scribus has crashed.&lt;/p&gt;
&lt;p&gt;You can disable this option if you want to accept input on stdin. Generally you should use &lt;tt&gt;os.popen&lt;/tt&gt; to make a pipe instead, or use some other input mechanism, but this option is here just in case.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Эмулировать Stdin&lt;/tt&gt; - это расширенная функция. Лучше оставить её включённой.&lt;/p&gt;
&lt;p&gt;Обычно Scribus передаёт Python ложный файловый объект для &lt;tt&gt;sys.stdin&lt;/tt&gt;, так что при чтении из stdin всегда возвращается пустая строка.Если настоящий &lt;tt&gt;sys.stdin&lt;/tt&gt; на месте,сценарии, пытающиеся читать из него, блокируются, что приводит к блокированию и зависанию Scribus до тех пор, пока на stdin что-нибудь не попадёт. Для программ с графическим интерфейсом весьма необычно получать что-либо с stdin, поэтому большинство пользователей подумает, что Scribus завис намертво.&lt;/p&gt;
&lt;p&gt;Эту функцию можно отключить, чтобы передавать что-то на stdin. Обычно вместо этого настоятельно рекомендуется использовать &lt;tt&gt;os.popen&lt;/tt&gt; для создания pipe, либо иной механизм ввода, но в реальности эта опция сделана на всякий &quot;пожарный&quot; случай.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Enable Legacy Name Aliases&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;If checked, this option will cause the scripter to create a large number of function and constant name aliases for 1.2.0 script compatibility. It defaults to checked.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
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
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation>Показать имена шаблонов</translation>
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
        <source>OK</source>
        <translation>ОК</translation>
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
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Arrange Pages</source>
        <translation>Палитра страниц</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation>Доступные шаблоны:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Страницы документа:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation>Парные страницы</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation>Левая страница первой</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation>Перетащите страницы или шаблоны 
на пиктограмму корзины для их удаления.</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation>Здесь находятся все ваши шаблоны. 
Для создания новой страницы перетащите 
шаблон в область просмотра страниц внизу.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation>Просмотр всех страниц в документе</translation>
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
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Без стиля</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Стилей нет</translation>
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
        <source>Warning</source>
        <translation>Предупреждение</translation>
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
        <source>&amp;Append</source>
        <translation>&amp;Из файла</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation>Вы действительно хотите удалить этот стиль?</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Редактор текста</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation>Вы действительно хотите потерять все изменения?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Вы действительно хотите удалить весь текст?</translation>
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
        <translation>Вы хотите сохранить изменения:</translation>
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
        <source>&amp;Insert Special...</source>
        <translation>В&amp;ставить символ...</translation>
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
        <translation>Показать &amp;шрифт...</translation>
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
        <source>Clear all Text</source>
        <translation>Удалить весь текст</translation>
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
        <source>Outline Text</source>
        <translation>Контурный текст</translation>
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
        <source>Update Paragraph Styles</source>
        <translation>Обновить абзацные стили</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Включение этой функции приведёт к перезаписи существующих стилей в текущем документе</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Объединить абзацные сили</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Объединить абзацные стили по атрибутам. В результате схожих абзацных стилей станет меньше, атрибуты будут унаследованы, даже если в оригинальном документе стили называются иначе.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Добавить название документа в начало абзацного стиля документа Scribus.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Сделать эти настройки стандартными и больше никогда про них не спрашивать.</translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Настройка табуляторов</translation>
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
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Положение:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation>&amp;Красная строка:</translation>
    </message>
    <message>
        <source>Left Ind&amp;ent:</source>
        <translation>Отступ с&amp;лева:</translation>
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
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Информация</translation>
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
        <source>Font:</source>
        <translation>Шрифт:</translation>
    </message>
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
        <source>Page</source>
        <translation>Страница</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Имя &quot;%1&quot; уже использовано.
Выберите другое.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Группа</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
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
    <name>VlnaDialog</name>
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
        <source>&amp;OK</source>
        <comment>short words plugin</comment>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>short words plugin</comment>
        <translation>О&amp;тменить</translation>
    </message>
    <message>
        <source>&amp;Info and
Languages</source>
        <comment>short words plugin</comment>
        <translation>О &amp;модуле и языках</translation>
    </message>
    <message>
        <source>Replace defaults by user config</source>
        <comment>short words plugin</comment>
        <translation>Заменить стандартные настройки</translation>
    </message>
    <message>
        <source>When the user config file exists 
(%1)
you can choose if you want to append your config
to the global configuration by unchecked button.

You can replace predefined values by yours
with checked button too.</source>
        <comment>short words plugin</comment>
        <translation>Если существует свой файл настроек этого модуля
(%1),
вы можете добавить свои настройки к общесистемным, 
держа флажок напротив этой метки снятым.
Вы также можете заменить общесистемные 
настройки своими собственными, для чего нужно 
держать флажок напротив этой метки включенным.</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Обработаны только выбранные рамки.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Обработана только текущая страница.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Обработан весь документ.</translation>
    </message>
    <message>
        <source>Short Words for Scribus</source>
        <comment>short words plugin</comment>
        <translation>Короткие Слова для Scribus</translation>
    </message>
    <message>
        <source>Available in the following languages</source>
        <comment>short words plugin</comment>
        <translation>Работает для следующих языков</translation>
    </message>
    <message>
        <source>About Short Words</source>
        <comment>short words plugin</comment>
        <translation>О модуле Короткие Слова</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Инструменты</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation>Выбрать объекты</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Повернуть объекты</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Изменить содержимое рамки</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Вставить тестовую рамку</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation>Вставить изображение</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Свойства...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation>Вставить многоугольник</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation>Вставить линии</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation>Вставить кривые Безье</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Вставить линию от руки</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Связать текстовые рамки</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Разъединить тестовые рамки</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Вставить таблицу</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Изменить масштаб</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Изменить текст в Текстовом Редакторе</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation>Нарисовать различные очертания</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation>Измерить</translation>
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
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation>Выбор символа:</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Вставить символы под курсор в тексте</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Удалить текуее выделение(ия).</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation>Закрыть это диалог и вернуться к правке текста.</translation>
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
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation>Запомнить расширение файла для импорта и 
больше не спрашивать о файлах этого типа</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>ОК</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;ОК</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>О&amp;тменить</translation>
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
        <translation>Создатель шаблона должен убедиться в том, что раздел &quot;Установка шаблонов&quot; действителен и для его шаблонов. Это означает, что у пользователя не должно возникнуть проблем при скачивании пакета, извлечении файлов шаблонов из него и использовании этих шаблонов.</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Удаление шаблона из данного диалога означает лишь удаление записи из template.xml, а сами файлы с документами удалены не будут. Всплывающее меню с пунктом &quot;Удалить&quot; появится только в том случае, если у вас есть права на запись в файл template.xml.</translation>
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
        <translation>собственный</translation>
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
