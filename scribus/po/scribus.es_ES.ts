<!DOCTYPE TS><TS>
<context>
    <name></name>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
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
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation type="unfinished"></translation>
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
        <source>rendeFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size) -&gt; bool

Creates an image preview of font &quot;name&quot; with given text &quot;sample&quot; and size.
Image is saved into &quot;filename&quot;. Returns true when success.

May raise NotFoundError if the specified font can&apos;t be found.
May raise ValueError if an empty sample or filename is passed.
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
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns wether the Layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns wether the layer &quot;layer&quot; is printable or not, a value of True means
that the layer &quot;layer&quot; can be printed, a value of False means that printing
the layer &quot;layer&quot; is disabled.

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
        <source>getPageMargins()

Returns the page margins as a (left, right, top, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
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
        <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame. Text
must be UTF encoded (see setText() as reference) The first character has an
index of 0. &quot;name&quot; If &quot;name&quot; is not given the currently selected Item is
used.

May throw IndexError for an insertion out of bounds.
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
</context>
<context>
    <name>About</name>
    <message>
        <source>About Scribus %1</source>
        <translation>Acerca de Scribus %1</translation>
    </message>
    <message>
        <source>%1. %2 %3 </source>
        <translation>%1. %2 %3 </translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation>Versión de Scribus %1
%2 %3</translation>
    </message>
    <message>
        <source>Build-ID:</source>
        <translation>Build-ID:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;Acerca de</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Equipo de Desarrollo:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Contribuciones de:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation>Versión de Windows:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Documentación Oficial:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Otra Documentación:</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>A&amp;utores</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Traducciones Oficiales y Traductores:</translation>
    </message>
    <message>
        <source>Catalan:</source>
        <translation>Catalán:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation>Checo:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation>Danés:</translation>
    </message>
    <message>
        <source>English (British):</source>
        <translation>Inglés (Británico):</translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation>Esperanto:</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation>Finlandés:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation>Francés:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation>Alemán:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation>Italiano:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation>Coreano:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation>Noruego:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation>Polaco:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation>Ruso:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation>Serbio:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation>Eslovaco:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation>Esloveno:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation>Español:</translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation>Sueco:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation>Ucraniano:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation>Galés:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Anteriores Colaboradores en la Traducción:</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation>Vasco:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation>Brasileño:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation>Búlgaro:</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation>Gallego:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation>Hungaro:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation>Lituano:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation>Turco:</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;Traducciones</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Página inicial</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Referencia en línea</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Errores y Peticiones de Características</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Lista de Correo</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;En línea</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Este panel muestra la versión, fecha de construcción y
soporte de librerías con el que se compiló Scribus.
C-C-T significa soporte de C=CUPS C=littlecms T=TIFF
Si falta el soporte de una librería se indica con un *</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=littlecms C=CUPS T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation>Opciones Avanzadas</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation>Voltear página(s) &amp;horizontalmente</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation>Voltear página(s) &amp;verticalmente</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation>Aplicar &amp;eliminación del color subyacente (UCR)</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation>Aplicar perfiles &amp;ICC</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation>PostScript Nivel &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation>PostScript Nivel &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation>PostScript Nivel &amp;3</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation>Crea PostScript Nivel 3</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation>Crea PostScript Nivel 2 solamente, ten en
cuenta que puede crear archivos enormes.</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation>Crea PostScript Nivel 1 solamente, ten en
cuenta que puede crear archivos enormes.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Una forma de desactivar algunas de las saturaciones grises que están compuestas
de cian, amarillo y magenta y en lugar de eso usar negro.
UCR afecta en su mayoría a partes de images que son de tonos neutrales u
oscuros cercanos al gris. Esto puede mejorar la impresión de algunas imágenes
y se necesita hacer pruebas en cada caso.
UCR reduce la posibilidad de sobresaturación con tintas CMY (cian, magenta, amarillo).</translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Distribute/Align</source>
        <translation>Distribuir/Alinear</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Alinear</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation>Lados izquierdos</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation>Centro</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation>Lados derechos</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation>&amp;Entre:</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation>&amp;No cambiar</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation>A&amp;linear</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation>&amp;Desplazamiento</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation>Distribuir e&amp;quitativamente</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation>Lados superiores</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation>Lados inferiores</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation>En&amp;tre:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation>N&amp;o cambiar</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation>Al&amp;inear</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation>De&amp;splazamiento</translation>
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
        <source>Distribute E&amp;venly</source>
        <translation>Distribuir eq&amp;uitativamente</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation>A&amp;plicar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation>Alinear Texto a la Izquierda</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Alinear Texto a la Derecha</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Centrar Texto</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Justificar Texto</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Justificación forzada del Texto</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Propiedades de campo</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Tipo:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Botón</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Campo de texto</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Casilla de selección</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Cuadro combinado</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Lista</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Propiedades</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Consejo:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Fuente a usar para PDF-1.3:</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Borde</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Color:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Anchura:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Delgado</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Ancho</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Estilo:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Sólido</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>A trazos</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Subrayado</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Biselado</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Recuadro</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Otro</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Sólo Lectura</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Requerido</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>No exportar valor</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Visibilidad:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Visible</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Oculto</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>No Imprimir</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>No Mostrar</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Apariencia</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Texto al Presionar el Botón</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Texto al Pasar por Encima</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Iconos</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Usar Iconos</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Borrar</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Pulsado</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Cursor encima</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Disposición del icono...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Realzar</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertir</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Contorneado</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Pulsar</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Multilínea</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Contraseña</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Límite de</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>caracteres</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>No desplazar</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>No revisar ortografía</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Estilo de marca:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>V</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Cruz</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Diamante</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Círculo</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Estrella</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Cuadrado</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Seleccionada por defecto</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Editable</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation>Java Script</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Ir a</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Enviar formulario</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Reiniciar formulario</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Importar datos</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Evento:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Se suelta el ratón</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Se presiona el ratón</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>El cursor entra</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>El cursor sale</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Se obtiene el foco</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Se pierde el foco</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Script:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Editar...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Enviar a URL:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Enviar dato como HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Importar datos desde:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Destino</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>A archivo:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Cambiar...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Página:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>Posición X:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Posición Y:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Acción</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Campo formateado como:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Sin formato</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Número</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Porcentaje</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Fecha</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Hora</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Formato de Número</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Decimales:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Usar Símbolo de Moneda</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Símbolo de Moneda Delante</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Formato</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Formato de Porcentaje</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Formato de Fecha</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Formato de Hora</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Scripts Propios</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Formato:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Pulsación:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Formato</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>El valor no se valida</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>El valor debe ser mayor o igual que:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>y menor o igual que:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Script propio de validación:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Validar</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>El valor no se calcula</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Valor es el</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>total</translation>
    </message>
    <message>
        <source>product</source>
        <translation>producto</translation>
    </message>
    <message>
        <source>average</source>
        <translation>promedio</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>mínimo</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>máximo</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>de los siguientes campos:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Elegir...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Script de cálculo propio:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Calcular</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Opción ignorada por PDF 1.3</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Introducir aquí una lista de campos separados por comas</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Se necesita como mínimo el icono Normal para usar iconos en los botones</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation>Imágenes (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Ejemplo:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Se cambia la selección</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Archivos PDF (*.pdf);;Todos los archivos (*)</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Propiedades de Anotación</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Enlace</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Enlace externo</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Enlace web externo</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Tipo:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Destino</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Cam&amp;biar...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Página:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>Posición &amp;X:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Posicón &amp;Y:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>Documentos PDF (*.pdf);; Todos los archivos (*)</translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Apply Template</source>
        <translation>Aplicar Plantilla</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Plantilla:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation>Aplicar a la página ac&amp;tual</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation>Aplicar a todas las páginas &amp;pares</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation>Aplicar a todas las páginas &amp;impares</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation>Aplicar &amp;desde la página:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>Hasta:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>Álbum de recortes</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>C&amp;argar...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Gu&amp;ardar como...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>&amp;Pequeño</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>&amp;Mediano</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>&amp;Grande</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Previsualizar</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>Álbum de recortes (*.scs);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Renombrar</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Borrar</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>El nombre &quot;%1&quot; no es único.
Por favor elige otro.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objeto</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nombre:</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Nueva entrada</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>Marcadores</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>Mover Marcador</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Insertar Marcador</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Marcadores</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Disposición del icono</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Disposición:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Sólo texto</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Sólo icono</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Texto debajo del icono</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Texto encima del icono</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Texto a la derecha del icono</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Texto a la izquierda del icono</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Texto superpuesto al icono</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Escalado:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Siempre</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Cuando el icono es demasiado pequeño</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Cuando el icono es demasiado grande</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Nunca</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Modo de escalado:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proporcional</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>No proporcional</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Icono</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Reiniciar</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Color Management Settings</source>
        <translation>Configuración de los Ajustes de Color</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>Ac&amp;tivar Ajustes de Color</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Perfiles del Sistema</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation>&amp;Imágenes:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>Colores &amp;Sólidos:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Monitor:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>Im&amp;presora:</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Modos de Visualización</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptivo</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <comment>#, fuzzy</comment>
        <translation type="obsolete">Colormetraje Relativo</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturación</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <comment>#, fuzzy</comment>
        <translation type="obsolete">Colormetraje Absoluto</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>M&amp;onitor:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>Imp&amp;resora:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Sim&amp;ular impresora en la pantalla</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Indicar colores fuera de &amp;gama</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Usar compensación de punto &amp;negro</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation>Perfil de color por defecto para imágenes importadas</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Perfil de color por defecto para colores sólidos en la página</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>El perfil de color que hayas generado o que te haya dado el fabricante.
Debería ser específico para tu monitor y no uno genérico (como sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>El perfil de color del fabricante para tu modelo de impresora.
Debería ser específico para tu impresora y no uno genérico (como sRGB).</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <comment>#, fuzzy</comment>
        <translation type="obsolete">Modo de visualización por defecto para tu monitor. A menos que sepas
porqué cambiarlo, deberías elegir Colormetraje Relativo o Perceptivo.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <comment>#, fuzzy</comment>
        <translation type="obsolete">Modo de visualización por defecto para tu impresora. A menos que sepas
porqué cambiarlo, deberías elegir Colormetraje Relativo o Perceptivo.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Activar comprobación software (soft proofing) de cómo se imprimirán 
los colores del documento, basada en el perfil de impresora seleccionado.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Una forma de mostrar en la pantalla los colores que no se imprimirán apropiadamente.
Esto requiere perfiles muy precisos y sólo sirve como un aviso.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Un método de mejorar el contraste en las fotos. Se recomienda
que lo actives si tienes fotos en el documento</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="unfinished">Colormetraje Relativo</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="unfinished">Colormetraje Absoluto</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="unfinished">Modo de visualización por defecto para tu monitor. A menos que sepas
porqué cambiarlo, deberías elegir Colormetraje Relativo o Perceptivo.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="unfinished">Modo de visualización por defecto para tu impresora. A menos que sepas
porqué cambiarlo, deberías elegir Colormetraje Relativo o Perceptivo.</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Editar color</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nombre:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>&amp;Modelo de color</translation>
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
        <translation>RGB Seguro para la Red</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Nuevo</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Viejo</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>Mapa de color TSI (HSV)</translation>
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
        <translation>Barras de Color Dinámicas</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Barras de Color Estáticas</translation>
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
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation>No puedes crear un color llamado &quot;%1&quot;.
Es un nombre reservado para el color transparente.</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation>El nombre del color ya existe</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Shade:</source>
        <translation>Saturación:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Opacidad:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Degradado horizontal</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Degradado vertical</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Degradado diagonal</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Degradado diagonal cruzado</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Degradado radial</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Degradado lineal libre</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Degradado radial libre</translation>
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
        <source>Edit Line Color Properties</source>
        <translation>Editar color de línea</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Editar color de relleno</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Saturación del color</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Método de relleno normal o gradiente</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Establece la transparencia del color seleccionado</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation>Color del objeto seleccionado</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
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
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Opciones de importación de CSV</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Delimitador de campo:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Delimitador de valor:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>La primera fila es una cabecera</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Opciones de impresión</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Selección de páginas</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Todas</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Sólo pares</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Sólo impares</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Voltear</translation>
    </message>
    <message>
        <source>No</source>
        <translation>No</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sí</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Orientación:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Apaisado</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Páginas por hoja</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Página por hoja</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Páginas por hoja</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Opción</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Valor</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Este panel muestra varias opciones de CUPS cuando se imprime.
Los parámetros exactos disponibles dependerán del driver de tu impresora.
Puedes confirmar el qué soporta CUPS seleccionando Ayuda &gt; Acerca de Scribus.
Busca los listados: C-C-T Esto significa soporte de C=CUPS C=littlecms T=TIFF
Las bibliotecas que falten se indican con un *</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Comprimir el archivo</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Incluir fuentes</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Codificación:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Ir al Directorio Documentos.
Se puede definir en las Preferencias.</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Borrar color</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation>Color a borrar:</translation>
    </message>
    <message>
        <source>?</source>
        <translation>?</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation>Reemplazarlo con:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Borrar páginas</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation>Borrar desde:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>hasta:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>Missing Font</source>
        <translation>Fuente ausente</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>La fuente %1 no está instalada.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Usar</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>en su lugar.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Información del documento</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Título:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>A&amp;utor:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Palabras clave</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>De&amp;scripción:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Editor:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>Cola&amp;boradores:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>&amp;Fecha:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>&amp;Tipo:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>&amp;Formato:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>&amp;Identificador:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Fuente:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Idioma:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;Relación:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>&amp;Portada:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>De&amp;rechos:</translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation>&amp;Documento</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>&amp;Información adicional</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>La persona u organización responsable principal de hacer el contenido del documento.
Este campo se puede empotrar en el documento de Scribus como referencia,
además de en los metadados de un PDF.</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Un nombre dado al documento.
Este campo se puede empotrar en el documento de Scribus como referencia,
además de en los metadados de un PDF.</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Un informe del contenido del documento.
Este campo es para una breve descripción o resumen del documento.
Se empotra en el PDF al exportar.</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>El tema del contenido del documento.
Este campo es para palabras clave del documento que deseas que se
empotren en un PDF, para ayudar a buscar e indexar archivos PDF.</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Una persona u organización responsable de hacer disponible el documento</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Una persona u organización responsable de hacer contribuciones al contenido del documento.</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Una fecha asociada con un evento en el ciclo de vida del documento, en formato AAAA-MM-DD, de acuerdo a ISO 8601.</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>La naturaleza o género del contenido del documento, p.ej. categorías, funciones, géneros, etc.</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>El formato físico o digital del documento. El tipo de medio y las dimensiones serían interesantes.
También son útiles los tipos MIME según RFC2045 y RFC2046.</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Una referencia ambigua al documento dentro de un contexto dado, como un ISBN o URI.</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Una referencia a un documento del que deriva el documento actual, p.ej. un ISBN o URI.</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>El idioma en que está escrito el contenido del documento, normalmente un código de idioma ISO-639
opcionalmente seguido con un guión y un código de país ISO-3166, p.ej. en-GB, fr-CH.</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Una referencia a un documento relacionado, posiblemente usando un identificador formal como un ISBN o URI.</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>La extensión o alcance del contenido del documento, posiblemente incluyendo localización, tiempo y rangos de jurisdicción.</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Información sobre los derechos mantenidos sobre el documento, p.ej. derechos de autor, patentes o marcas registradas.</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation>Configuración de la impresora</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Destino de la impresión</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Archivo</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Opciones</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Cam&amp;biar...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>Comando de impresión a&amp;lternativo</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation>Usar un administrador de impresión alternativo, como kprinter
o gtklp, para utilizar opciones de impresión adicionales.</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>Co&amp;mando:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Rango</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Imprimir &amp;todo</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Imprimir &amp;página actual</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Imprimir el ran&amp;go</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Inserta una lista de elementos separados por comas,
donde un elemento puede ser * para todas las páginas,
1-5 para un intervalo de páginas o un único número.</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>N&amp;úmero de copias:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation>Impresión &amp;normal</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation>Imprimir &amp;separaciones</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Todos</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cian</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Amarillo</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Negro</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation>Imprimir en colo&amp;r si está disponible</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation>Imprimir en &amp;escala de grises</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation>Opciones A&amp;vanzadas...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Imprimir</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Guardar como</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation>Archivos Postscript (*.ps);; Todos los archivos (*)</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Error importando el archivo:
%1</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Error fatal</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Editar Estilo</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nombre:</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Carácter</translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation>&amp;Fuente:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation>&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation>Efectos:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation>Al&amp;ineación:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation>Letra ca&amp;pital</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Lineas:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Color de &amp;relleno:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation>Color de tra&amp;zo:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation>Espaciado vertical</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation>A&amp;justar a las líneas de base</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation>Int&amp;erlineado:</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation>Arri&amp;ba:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation>&amp;Debajo:</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabuladores y sangría</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Nombre del estilo de párrafo</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Fuente del texto u objeto seleccionado</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamaño de la fuente</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Color de relleno del texto</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Color del trazo del texto</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation>Proporciona una primera letra de párrafo de mayor tamaño. Se usa para efecto estilístico.</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Determina la altura general, en números de línea, de las letras capitales.</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation>Alinear texto a las líneas de base</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Espaciado por encima del párrafo</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Espacio por debajo del párrafo</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Espacio entre líneas</translation>
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
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation>El nombre del estilo ya existe</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Editor</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Abrir...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Gu&amp;ardar como...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Guardar y Salir</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Salir sin Guardar</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Deshacer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Re&amp;hacer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Cor&amp;tar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>B&amp;orrar</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Nombres de los campos</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation>Javascripts (*.js);; Todos los archivos (*)</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Elige un Directorio donde Exportar</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Exportar como Imagen(es)</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Exportar al Directorio</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Cam&amp;biar...</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>&amp;Tipo de Imagen:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>Cali&amp;dad:</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>Resol&amp;ución:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaño:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> ppp</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Rango</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>Página &amp;Actual</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>Todas las &amp;Páginas</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Intervalo:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Exportar un intervalo de páginas</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Inserta una lista de elementos separados por comas,
donde un elemento puede ser * para todas las páginas,
1-5 para un intervalo de páginas o un único número.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Exportar todas las páginas</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Exportar sólo la página actual</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Resolución de las imágenes
Usa 72 ppp para imágenes dirigidas a verse en pantalla</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Tamaño de las imágenes. 100% para que no cambie, 200% para el doble de grande, etc.</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>La calidad de las imágenes - 100% es la  mejor calidad, 1% la peor</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Formatos de exportación disponibles</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>El directorio de salida - donde se almacenarán las imágenes.
El nombre del archivo será &apos;documento-página.tipo&apos;</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Cambiar el directorio de salida</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Tamaño:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Título:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Sin título</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Desconocido</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Documento de Scribus</translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Colors</source>
        <translation>Colores</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>A&amp;ñadir</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>Borrar no &amp;usados</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Conjuntos de Colores</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Conjunto actual:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Guardar Conjunto de Colores</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Elige el conjunto de colores a cargar</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Guarda el conjunto de colores actual</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Eliminar del conjunto de colores del documento los que no se usen</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation>Añadir colores de un documento existente al conjunto actual</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Crear un nuevo color en el actual conjunto</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Editar el color seleccionado</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Hacer una copia del color seleccionado</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Borrar el color seleccionado</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Establecer el conjunto actual como el conjunto de colores por defecto</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nombre:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Elige un nombre</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copia de %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Nuevo color</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Global Font Settings</source>
        <translation>Configuraciones globales de las fuentes</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Fuentes Disponibles</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Sustituciones de Fuentes</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Rutas Adicionales</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sí</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Nombre de fuente</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation>Usar fuente</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation>Empotrar en:</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Subconjunto</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation>Ruta al archivo de la fuente</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>Fuentes &amp;Disponibles</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Reemplazo</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Sustituciones de Fuentes</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Cam&amp;biar...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>A&amp;ñadir...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>&amp;Rutas Adicionales</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Elige un directorio</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Fonts Preview</source>
        <translation>Previsualizar Fuentes</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <translation>Añadir fuente seleccionada al menú Estilo, Fuente</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation>Salir de la previsualización</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Woven silk pyjamas exchanged for blue quartz</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Posición:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Aquí puedes añadir, cambiar o eliminar límites de colores.</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Administrar Guías</translation>
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
        <source>Horizontal Guides</source>
        <translation>Guías horizontales</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Posicón &amp;Y:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>A&amp;ñadir...</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Guías verticales</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>Posición &amp;X:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>Aña&amp;dir...</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>B&amp;orrar</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>B&amp;loquear las guías</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Contents</source>
        <translation>Contenidos</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Enlace</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Ayuda de Scribus en línea</translation>
    </message>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Lo siento, no hay ningún manual disponible. Por favor mira: http://docs.scribus.net para documentación actualizada y www.scribus.net para descargas.</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Posible División</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Saltar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Hyphenator Settings</source>
        <translation>Configuraciones del Divisor de Palabras</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Sugerencias de &amp;División</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Dividir Texto &amp;Automáticamente Mientras se Escribe</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Idioma:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>Palabra más &amp;pequeña:</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>Divisiones Consecutivas Per&amp;mitidas:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Cuando use la opción Extras, Dividir en Sílabas, se mostrará para cada palabra un cuadro de diálogo con las posibles divisiones.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Activa la división silábica automática mientras escribe.</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Longitud mínima que debe tener una palabra para dividirla.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Máximo número de divisiones seguidas.
Un valor de 0 significa divisiones ilimitadas.</translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Insertar página</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation>&amp;Insertar</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>página(s)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>antes de la página</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>después de la página</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>al final</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation>Insertando</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation>&amp;Plantilla (página izquierda):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Plantilla:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation>Plantilla (página derecha):</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Insertar tabla</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation>Número de filas:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation>Número de columnas:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Editar JavaScripts</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Editar...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Añadir...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Nuevo Script:</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation>Nuevo Script</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation>¿Realmente desea borrar este Script?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;No</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sí</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation>Configurar accesos rápidos</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Acción</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Tecla Actual</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Acceso rápido para la acción seleccionada</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Ninguno</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>Definido por el &amp;usuario</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>&amp;Definir</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
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
        <translation>Mays</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation>Mays+</translation>
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
        <translation>Atención</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation>Ésta combinación de teclas ya está en uso</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation>Capas</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation>Añadir una nueva Capa</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Borrar Capa</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation>Subir Capa</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation>Bajar Capa</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation>Nueva Capa</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation>¿Desea borrar también todos los objetos de esta capa?</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Editar Estilos de Línea</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>A&amp;ñadir</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copia de %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nuevo Estilo</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation>¿Realmente desea borrar este Estilo?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;No</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sí</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);; Todos los archivos (*)</translation>
    </message>
</context>
<context>
    <name>MSpinBox</name>
    <message>
        <source> pt</source>
        <translation> pt</translation>
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
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Duplicación múltiple</translation>
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
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Número de copias:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>Desplazamiento &amp;horizontal:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>Desplazamiento &amp;vertical:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Distancias</translation>
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
        <translation>Ángulo:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Longitud:</translation>
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
</context>
<context>
    <name>MenuTest</name>
    <message>
        <source>Script error</source>
        <translation>Error en el Script</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Si estás ejecutando un script oficial, por favor informalo en &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Este mensaje está en tu portapapeles también. Usa Ctrl+V para pegarlo en el seguimiento de errores</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Mostrar &amp;Consola</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation>Ocultar &amp;Consola</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Import Template</source>
        <translation>Importar Plantilla</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importar Página(s)</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation>Desde el documento:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Cambiar...</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation>Importar página(s):</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Inserta una lista de elementos separados por comas,
donde un elemento puede ser * para todas las páginas,
1-5 para un intervalo de páginas o un único número.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> de 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Crear página(s)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>antes de la página</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>después de la página</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>al final</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Importar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> de %1</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Mover Páginas</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Copiar Página</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Mover página(s):</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>hasta:</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>antes de la página</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>después de la página</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>al final</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>Propiedades</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Forma</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Texto</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Imagen</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Linea</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Colores</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometría</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>Posición &amp;X:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Posicón &amp;Y:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Anc&amp;hura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>Al&amp;tura:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotación:</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Punto base:</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Nivel</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Forma:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Editar forma...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>Esquinas &amp;redondeadas:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Distancia del texto:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Colu&amp;mnas:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>Es&amp;pacio:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Superior:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>I&amp;nferior:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>Iz&amp;quierda:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Derecha:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>Ta&amp;buladores...</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Propiedades del Texto sobre Trazado</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Mostrar curva</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Desplazamiento inicial:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Distancia desde curva:</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>El texto flu&amp;ye alrededor del marco</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Usar cuadro circ&amp;unscrito</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>Usar línea de c&amp;ontorno</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Ta&amp;maño de fuente:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Saturación:</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation>Espaciado personalizado</translation>
    </message>
    <message>
        <source>&amp;Kerning:</source>
        <translation>A&amp;juste entre caracteres:</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation>Es&amp;pacio entre lineas:</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>&amp;Estilo:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>I&amp;dioma:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Escalado Li&amp;bre</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>Esc&amp;ala en X:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>&amp;Escalar en Y:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Escalar al tamaño del &amp;marco</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>&amp;Proporcional</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Perfil de entrada:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Modo de Visualización:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptivo</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <comment>#, fuzzy</comment>
        <translation type="obsolete">Colormetraje Relativo</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturación</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <comment>#, fuzzy</comment>
        <translation type="obsolete">Colormetraje Absoluto</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Punto izquierdo</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Extremos</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>P&amp;unto base:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>Ti&amp;po de línea:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>&amp;Ancho de linea:</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Unión de inglete</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Unión biselada</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Unión redondeada</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>&amp;Bordes:</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Plano</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Cuadrado</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Redondeado</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Extremos:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Sin estilo</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Bordes de celda</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Borde superior</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Borde izquierdo</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Borde derecho</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Borde inferior</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Nombre del objeto seleccionado</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Posición horizontal del punto base actual</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Posición vertical del punto base actual</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Anchura</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Altura</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Rotación del objeto sobre el punto base actual</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Punto de referencia para las medidas o ángulos de rotación</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Seleccionar la esquina superior izquierda como punto base</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Seleccionar la esquina superior derecha como punto base</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Seleccionar la esquina inferior izquierda como punto base</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Seleccionar la esquina inferior derecha como punto base</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Seleccionar el centro como punto base</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Voltear horizontalmente</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Voltear verticalmente</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Subir un nivel</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Bajar un nivel</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Traer al frente</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Envíar al fondo</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Indica en qué nivel está el objeto, 0 significa que está debajo</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Bloquear o desbloquear el objeto</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Bloquear o desbloquear el tamaño del objeto</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Activar o desactivar la impresión del objeto</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Hacer que el texto en los marcos de debajo fluya alrededor de la forma del objeto</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation>Usar el cuadro circunscrito en vez de la forma del marco para que fluya el texto</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Usar una segund línea basada originalmente en la forma del marco para que fluya el texto</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Fuente del texto u objeto seleccionado</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamaño de la fuente</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Escalar la anchura de los caracteres</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Color del trazo del texto</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Color de relleno del texto</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Saturación del color de trazo</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Saturación del color de relleno</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Escritura de Izquierda a Derecha</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation>Ajuste entre caracteres manual</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Espacio entre líneas</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Estilo del párrafo</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Idioma del marco para la división de palabras</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Cambiar configuraciones para el punto izquierdo o los extremos</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Patrón de línea</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Grosor de línea</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Tipo de uniones de las líneas</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Tipo de extremos</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Estilo de línea del objeto</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Elegir la forma del marco...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Editar la forma del marco...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Definir el radio de las esquinas redondeadas</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Número de columnas de texto del marco</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Cambia entre espacio o ancho de las columnas</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Distancia entre columnas</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Distancia del texto desde el extremo superior</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Distancia del texto desde el extremo inferior</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Distancia del texto desde el extremo izquierdo</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Distancia del texto desde el extremo derecho</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Editar las configuraciones de los tabuladores...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Permite que la imagen sea de un tamaño distinto al del marco</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Desplazamiento horizontal de la imagen dentro del marco</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Desplazamiento vertical de la imagen dentro del marco</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Redimensionar la imagen horizontalmente</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Redimensionar la imagen verticalmente</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Mantener la escala en X e Y igual</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Mantener la proporción</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Hacer que la imagen encaje dentro del marco</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Usar proporciones de la imagen en vez de las del marco</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Perfil de entrada de la imagen</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Modo de representación de la imagen</translation>
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
        <source>Column width</source>
        <translation>Anchura de las columnas</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>El nombre &quot;%1&quot; no es único.
Por favor elige otro.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="unfinished">Colormetraje Relativo</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="unfinished">Colormetraje Absoluto</translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation>Editar Estilo</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Plano</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Cuadrado</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Redondeado</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Unión de inglete</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Unión biselada</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Unión redondeada</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Ancho de linea:</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source> pt </source>
        <translation> pt </translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Línea sólida</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Línea a trazos</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Línea a puntos</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Línea raya punto</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Línea raya punto punto</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>El nombre &quot;%1&quot; no es único.
Por favor elige otro.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Edit Templates</source>
        <translation>Editar Plantillas</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>A&amp;ñadir</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation>¿Realmente desea borrar esta Plantilla?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;No</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sí</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nombre:</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation>Nueva Plantilla</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copia de %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copia #%1 de </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
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
        <source>New Document</source>
        <translation>Nuevo documento</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Formato de página</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Executive</source>
        <comment>#, fuzzy</comment>
        <translation type="obsolete">Ejecutivo</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation>Folio</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation>Libro de contabilidad</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Carta</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloide</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientación:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Apaisado</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Anc&amp;hura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>Al&amp;tura:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>Páginas en&amp;frentadas</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Primero pá&amp;gina izquierda</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Guías de los márgenes</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>Iz&amp;quierda:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Derecha:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Superior:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>I&amp;nferior:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Núme&amp;ro de la primera página:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>&amp;Unidad por defecto:</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation>Puntos (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Milímetros (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Pulgadas (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picas (p)</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>Marcos de te&amp;xto automáticos</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation>Columnas</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>Es&amp;pacio:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Colu&amp;mnas:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Tamaño de página del documento, elige un tamaño estándar o uno personalizado</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Orientación de las páginas del documento</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Anchura de las páginas, editable si se elige un tamaño de página personalizado</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Altura de las páginas, editable si se elige un tamaño de página personalizado</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Esquema de página única o doble</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Hacer la primera página del documento la página izquierda</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Distancia entre la guía del margen superior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Distancia entre la guía del margen inferior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distancia entre la guía del margen izquierdo y el borde de la página.
Si las páginas enfrentadas están activadas, este espacio se puede usar
para lograr los márgenes correctos para la encuadernación.</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distancia entre la guía del margen derecho y el borde de la página.
Si las páginas enfrentadas están activadas, este espacio se puede usar
para lograr los márgenes correctos para la encuadernación.</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>Número de la primera página del documento</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Unidad de medida por defecto</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Crear marcos de texto automáticamente cuando se añaden nuevas páginas</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Número de columnas de los marcos de texto creados automáticamente</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Distancia entre columnas de los marcos de texto creados automáticamente</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Interior:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>&amp;Exterior:</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="unfinished">Ejecutivo</translation>
    </message>
</context>
<context>
    <name>NewTm</name>
    <message>
        <source>Left Page</source>
        <translation>Página izquierda</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Página derecha</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Nodos</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>Coordenadas &amp;Absolutas</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>Posición &amp;X:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Posicón &amp;Y:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>Editar &amp;contorno</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>&amp;Reiniciar puntos de control</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Fin de edición</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Mover nodos</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Mover puntos de control</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Añadir nodos</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Borrar nodos</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Mover puntos de control independientemente</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Mover puntos de control simétricamente</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Reiniciar puntos de control</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Reinicia este punto de control</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Abrir un polígono o cortar una curva de Bézier</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Cierra esta curva de Bézier</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Voltear horizontalmente</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Voltear verticalmente</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Cizallar horizontalmente a la derecha</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Cizallar horizontalmente a la izquierda</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Cizallar verticalmente hacia arriba</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Cizallar verticalmente hacia abajo</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Rotar en el sentido contrario a las agujas del reloj</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Rotar en el sentido de las agujas del reloj</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation>Reducir el tamaño en el porcentaje mostrado</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Agrandar el tamaño en el porcentaje mostrado</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Ángulo de rotación</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation>Porcentaje en que agrandar o reducir</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Activar el modo de edición del contorno</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Reiniciar el contorno a la forma original del marco</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation>Cuando está marcada usa coordenadas relativas a la página,
de otro modo las coordenadas son relativas al objeto.</translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation>Consola de Script</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Create PDF File</source>
        <translation>Crear archivo PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>&amp;Archivo de salida:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Cam&amp;biar...</translation>
    </message>
    <message>
        <source>Export Range</source>
        <translation>Rango de exportación</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>&amp;Todas las páginas</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>&amp;Rango</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotación:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Opciones del Archivo</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>Compat&amp;ibilidad:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>E&amp;ncuadernación:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Margen Izquierdo</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Margen Derecho</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Generar &amp;miniaturas</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Guardar marcos enla&amp;zados como artículos PDF</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>Incluir &amp;marcadores</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> ppp</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>Resol&amp;ución:</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation>Comprimir Texto y Gráficos &amp;Vectoriales</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation>Configuración de imágenes</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automático</translation>
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
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation>Mét&amp;odo:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>Cali&amp;dad:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Máxima</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Alta</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Media</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Baja</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Mínima</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation>Reba&amp;jar las imágenes a:</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>G&amp;eneral</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>&amp;Empotrar todas las fuentes</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation>Empotrar &amp;subconjunto de todas las fuentes</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Empotrar</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Fuentes Disponibles:</translation>
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
        <translation>Fuentes a empotrar:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation>Fuentes a empotrar un subconjunto:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Fuentes</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation>Acti&amp;var Efectos de Presentación</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Página</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>&amp;Mostrar vista previa</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efectos</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>Duración de la vis&amp;ualización:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Duración del efect&amp;o:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>&amp;Tipo de efecto:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>Lí&amp;neas:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>&amp;Desde:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>Di&amp;rección:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> seg</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Sin efecto</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Bandas</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Caja</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Disolver</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Brillo</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>División</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Cortina</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Interior</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Exterior</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Izquierda a Derecha</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Arriba a Abajo</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Abajo a Arriba</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Derecha a Izquierda</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Arriba-Izda a Abajo-Dcha</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>Apl&amp;icar efectos en todas las páginas</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation>&amp;Presentación</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>Usar cif&amp;rado</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Contraseñas</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Usuario:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>&amp;Propietario:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Configuraciones</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>Permitir &amp;imprimir el documento</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Permitir ca&amp;mbiar el documento</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Permitir c&amp;opiar texto y gráficos</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Permitir a&amp;ñadir anotaciones y campos</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>&amp;Seguridad</translation>
    </message>
    <message>
        <source>General</source>
        <translation>General</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Salida &amp;dirigida a:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Pantalla / Internet</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Impresora</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Usar configuraciones de renderizado personalizadas</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Configuraciones de Renderizado</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>F&amp;recuencia:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>Á&amp;ngulo:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>Función de &amp;punto:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Punto simple</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Línea</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Redondeado</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Elipse</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Colores Sólidos:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Usar perfil ICC</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Perfil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Modo de Visualización:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptivo</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <comment>#, fuzzy</comment>
        <translation type="obsolete">Colormetraje Relativo</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturación</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <comment>#, fuzzy</comment>
        <translation type="obsolete">Colormetraje Absoluto</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Imágenes:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>No usar perfiles ICC empotrados</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>Co&amp;lor</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>Modo de salida de PDF/X-3</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>Texto &amp;informativo:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>&amp;Perfil de salida:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Cuadro de recorte</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
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
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Exportar todas las páginas a PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Exportar un rango de páginas a PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Inserta una lista de elementos separados por comas,
donde un elemento puede ser * para todas las páginas,
1-5 para un intervalo de páginas o un único número.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation>Determina la compatibilidad del PDF. La opción por defecto es Acrobat 4.0 que da la mayor compatibilidad.
Elige Acrobat 5.0 si tu archivo tiene características de PDF 1.4, como transparencia, o si requieres cifrado de 128 bits.
PDF/X-3 es para exportar PDF para impresión profesional y se puede seleccionar si tienes activados los ajustes de color.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation>Determinar la encuadernación de las páginas en el PDF. Si no sabes
que elegir deja la opción por defecto - Izquierda.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation>Genera miniaturas de cada página en el PDF.
Algunos visores pueden usar las miniaturas para la navegación.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Generar artículos en el PDF, lo que es útil para navegar artículos enlazados en un PDF.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation>Empotrar los marcadores que has creado en tu documento.
Son útils para navegar en documentos PDF largos.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation>Resolución del testo y gráficos vectoriales exportados.
Esto no afecta a la resolución de las imágenes como las fotos.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation>Compresión de texto y gráficos.
Si no tienes una razón para no usarlo, deja esta opción.
Reduce el tamaño del PDF.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation>Método de compresión para las imágenes.
Automático permite a Scribus elegir el mejor método.
ZIP es bueno para imágenes con colores sólidos.
JPEG crea archivos PDF más pequeños que tienen muchas fotos
(posiblemente con una leve pérdida de calidad).
Dejalo puesto a automático, excpeto si necesitas opciones de compresión especiales.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation>Niveles de compresión: Mínimo (25%), Bajo (50%), Medio (75%), Alto (85%), Máximo (95%)</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation>Rebaja las imágnes a la resolución seleccionada.
Dejar esta opción sin seleccionar las renderizada con su propia resolución.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>PPP (puntos por pulgada) con que exportar las imágenes.</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation>Empotrar las fuentes en el PDF. Esto preservará
el diseño y la apariencia del documento.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation>Activar los efectos de presentación cuando se usa Acrobat Reader en modo de pantalla completa.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Mostrar la previsualización de cada página listada arriba.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>Cuanto tiempo se muestra la página antes de que la presentación empiece en la página actual.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>La duración del efecto.
Cuanto menor tiempo más rápido será el efecto, y viceversa.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Tipo del efecto que se mostrará.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Dirección del movimiento de las líneas para los efectos de división y bandas.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Posición inicial de los efectos de caja y división.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Dirección de los efectos de brillo y cortina.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Aplicar el efecto actual en todas las páginas.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Activar las características de securidad en el PDF.
Si seleccionas Acrobat 4.0, el PDF estará protegido por un cifrado de 40 bits.
Si seleccionas Acrobat 5.0, el PDF estará protegido por un cifrado de 128 bits.
Atención: el cifrado PDF no es tan fiable como el cifrado GPG o PGP y tiene algunas limitaciones.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation>Elige una contraseña maestra que activará o desactivará todas
las características de seguridad del PDF.</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Elige una contraseña para que los usuarios puedan leer el PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Permitir imprimir el PDF, si no se selecciona no se permitirá.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Permitir modificar el PDF. Si no se selecciona no se permitirá.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation>Permitir copiar el texto o los gráficos del PDF.
Si no se selecciona no se permtirá.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation>Permitir añadir anotaciones y campos al PDF.
Si no se selecciona, no se permitirá.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation>Modelo de color para la salidad del PDF.
Elige Pantalla / Web para PDFs que se usarán para mostrar en pantalla e imprimir en impresoras de tinta.
Elige Impresora para imprimir en impresoras de 4 colores CMYK.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Esta es una opción avanzada que no se activa por defecto. Sólo se debería activar
cuando lo pida específicamente tu imprenta y te den los detalles exactos que necesitan.
De otra forma, puede que el PDF no se imprima apropiadamente y no será portable entre distintos sistemas.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Empotrar un perfil de color para colores sólidos</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Perfil de color para colores sólidos</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Modo de representación para colores sólidos</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Empotrar un perfil de color para las imágenes</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>No usar los perfiles de color que estén empotrados en las imágenes</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Perfil de color para imágenes</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Modos de representación para las imágenes</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Perfil de salida para impresión. Si es posible, pide consejo a tu imprenta sobre la elección del perfil.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Cadena obligatoria para PDF/X-3 o el PDF no será conforme a PDF/X-3.
Recomendamos que uses el título del documento.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Distancia del margen desde el extremo superior de la página real</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Distancia del margen desde el extremo inferior de la página real</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Distancia del margen desde el extremo izquierdo de la página real</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Distancia del margen desde el extremo derecho de la página real</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Voltear página(s) horizontalmente</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Voltear página(s) verticalmente</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Guardar como</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Archivos PDF (*.pdf);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="unfinished">Colormetraje Relativo</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="unfinished">Colormetraje Absoluto</translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Previsualización de la Impresión</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>Suavizar &amp;Texto</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Suavizar &amp;Gráficos</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Mostrar Trans&amp;parencia</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>&amp;Eliminación de Color Subyacente</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;Mostrar CMYK</translation>
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
        <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation>Proporciona una vista más agradable del texto, al coste de una ligera
ralentización en la previsualización. Sólo afecta a fuentes Type 1</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Proporciona una vista más agradable de las fuentes TrueType y OpenType, EPS, PDF y gráficos vectoriales, al coste de una ligera ralentización en la previsualización</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Muestra transparencias y objetos transparentes. Requiere Ghostscript 7.07 o superior</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Da una previsualización de la impresión simulando tintas CMYK genéricas, en vez de colores RGB</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Activar/desactivar la tinta C (Cian)</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Activar/desactivar la tinta M (Magenta)</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Activar/desactivar la tinta Y (Amarillo)</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Activar/desactivar la tinta K (Negro)</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Una forma de desactivar algunas de las saturaciones grises que están compuestas
de cian, amarillo y magenta y en lugar de eso usar negro.
UCR afecta en su mayoría a partes de images que son de tonos neutrales u
oscuros cercanos al gris. Esto puede mejorar la impresión de algunas imágenes
y se necesita hacer pruebas en cada caso.
UCR reduce la posibilidad de sobresaturación con tintas CMY (cian, magenta, amarillo).</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Todos</translation>
    </message>
</context>
<context>
    <name>Page</name>
    <message>
        <source>Copy Here</source>
        <translation>Copiar aquí</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Mover aquí</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Mostrar Má&amp;rgenes</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Mostrar &amp;Marcos</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Mostrar &amp;Imágenes</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Mostrar &amp;Cuadrícula</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Mostrar &amp;Guías</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Mostrar Líneas de &amp;Base</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>&amp;Ajustar a la Cuadrícula</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>A&amp;justar a las Guías</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Imagen</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Archivo: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>PPP originales: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>PPP actuales: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Texto enlazado</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Marco de Texto</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Texto sobre trazado</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Párrafos: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Palabras: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Caracteres: </translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Imprimir: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Activado</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Desactivado</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>In&amp;formación</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation>Car&amp;gar imagen...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>&amp;Imagen visible</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation>Actuali&amp;zar imagen</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation>&amp;Editar imagen</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation>&amp;Ajustar marco a imagen</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>Car&amp;gar texto...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation>&amp;Añadir texto...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation>&amp;Editar texto...</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>&amp;Marcador PDF</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>&amp;Anotación PDF</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>&amp;Propiedades de las Anotaciones</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>&amp;Propiedades de Campo</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>Opciones de &amp;PDF</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>&amp;Editar texto...</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>Blo&amp;quear</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>Desblo&amp;quear</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation>Bloquear tama&amp;ño</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation>Desbloquear tama&amp;ño</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Enviar al Álbu&amp;m de Recortes</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>E&amp;nviar a Capa</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation>Insertar texto de e&amp;jemplo</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>Agr&amp;upar</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation>Desagr&amp;upar</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>Nive&amp;l</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>&amp;Enviar al fondo</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation>&amp;Traer al frente</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Bajar</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;Subir</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation>Marco de &amp;imagen</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation>&amp;Polígono</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation>&amp;Contorno</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>Marco de &amp;texto</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>Curva de &amp;Bézier</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>Con&amp;vertir a</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Cor&amp;tar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>B&amp;orrar contenidos</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation>Mostrar p&amp;ropiedades...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation>Ocultar p&amp;ropiedades...</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>¿Realmente desea borrar todo el texto?</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation>El programa</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation>no se encuentra</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Copia de </translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Imagen</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Línea</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polígono</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polilínea</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>TextoTrazado</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation>Página </translation>
    </message>
    <message>
        <source> of %1</source>
        <translation> de %1</translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Result</source>
        <translation>Resultado</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Resultados de la búsqueda para: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Previsualizar</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Seleccionar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Pictures</source>
        <translation>Imágenes</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Ruta</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Página</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Imprimir</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Estado</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Ir a</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sí</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Ausente</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Buscar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Propiedades del polígono</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>&amp;Vértices:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotación:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Aplicar &amp;Factor</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>Fac&amp;tor:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Número de vértices de los polígonos.</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Grados de rotación de los polígonos</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Aplicar el factor convexo/cóncavo para cambiar la forma de los polígonos</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Polígono de ejemplo</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation>Con un valor negativo el polígono será cóncavo (o con forma de estrella),
con un valor positivo será convexo</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Preferencias</translation>
    </message>
    <message>
        <source>General</source>
        <translation>General</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Documento</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Guías</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Tipografía</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Herramientas</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Álbum de recortes</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Visualización</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Herramientas Externas</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation>Varios</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>GUI</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Tema:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Tama&amp;ño de fuente:</translation>
    </message>
    <message>
        <source>Units</source>
        <translation>Unidades</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation>Puntos (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Milímetros (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Pulgadas (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picas (p)</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation>Configuración del ratón</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>Desplazamiento de la r&amp;ueda:</translation>
    </message>
    <message>
        <source> px</source>
        <translation> px</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation>Radio de s&amp;elección:</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menús</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>D&amp;ocumentos recientes:</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Rutas</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Documentos:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>Ca&amp;mbiar...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;Perfiles ICC:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Cam&amp;biar...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Scripts:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>Camb&amp;iar...</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation>P&amp;lantillas:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Cambia&amp;r...</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Formato de página</translation>
    </message>
    <message>
        <source>Executive</source>
        <comment>#, fuzzy</comment>
        <translation type="obsolete">Ejecutivo</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation>Folio</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation>Libro de contabilidad</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Carta</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloide</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Apaisado</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientación:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Anc&amp;hura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>Al&amp;tura:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>Páginas en&amp;frentadas</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>&amp;Primero página izquierda</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Guías de los márgenes</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>I&amp;nferior:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Superior:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>D&amp;erecha:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Izquierda:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Autoguardado</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation>Acti&amp;vado</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>Interva&amp;lo:</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation>Disposición de la Cuadrícula</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation>Espacio entre Líneas Me&amp;nores:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation>Espacio entre Líneas Ma&amp;yores:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation>Distancia de A&amp;juste a las Guías:</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation>Colores de la Cuadrícula</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation>Color de las Líneas M&amp;enores:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation>Color de las Líneas May&amp;ores:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation>Color de las Guías del &amp;Usuario:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation>Color de las Líneas de &amp;Base:</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation>Posición:</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation>Det&amp;rás</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation>&amp;Delante</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation>Líneas de Base</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation>Ac&amp;tivadas</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation>Desacti&amp;vadas</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Subíndice</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Desplazamiento</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;Escala:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Superíndice</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>Des&amp;plazamiento</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>E&amp;scala:</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Versalitas</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Esca&amp;la:</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Otro</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>Líneas de &amp;Base:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>Mar&amp;gen de las Líneas de Base:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation>&amp;Interlineado automático:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation>&amp;Fuente por defecto:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation>&amp;Tamaño por defecto:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation>C&amp;olor del texto:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Colu&amp;mnas:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>Es&amp;pacio:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Woven silk pyjamas exchanged for blue quartz</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>Color de &amp;línea:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Saturación:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>Color de &amp;Relleno:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>Sat&amp;uración:</translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation>&amp;Tipo de Línea</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>A&amp;ncho de linea:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>Mí&amp;nimo</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>Má&amp;ximo</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>&amp;Incremento:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Escalado Li&amp;bre</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>Escalado &amp;Horizontal</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>Escalado &amp;Vertical</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>&amp;Escalar Imagen al Tamaño del Marco</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>&amp;Mantener la Relación de Aspecto</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Color de &amp;relleno:</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>&amp;Vértices:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotación:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Aplicar &amp;Factor</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>Fac&amp;tor:</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Otras Opciones</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>&amp;Guardar los Contenidos cuando Cambian</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Previsualizar</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>Pequeño</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Mediano</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>Grande</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation>Mostrar &amp;páginas una al lado de otra</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation>Colores de la Página</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation>&amp;Fondo:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation>&amp;Márgenes:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Mostrar el Área &amp;No Imprimible con el Color del Margen</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation>Usar las Características de &amp;Transparencia de PDF 1.4</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>A&amp;justar Tamaño de la Pantalla</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation>Para ajustar la pantalla arrastra el deslizador de abajo y ajusta la regla.</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation>Intérprete Postscript</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Nombre del Ejecutable:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Suavizar &amp;Texto</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Suavizar &amp;Gráficos</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Herramienta de Procesado de Imágenes</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Nombre del &amp;Ejecutable:</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Impresión</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation>Recortar a los &amp;Márgenes de la Página</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Aplicar &amp;Eliminación del Color Subyacente</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation>Elige la apariencia y decoración de ventanas por defecto.
Scribus hereda cualquier tema de KDE o Qt disponible.</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Tamaño de fuente por defecto para los menús y ventanas</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Unidad de medida por defecto</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Número de líneas que Scribus desplazará con cada movimiento de la rueda del ratón</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Radio del área donde Scribus permitirá que se seleccione un objeto</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Número de documentos editados recientemente a mostrar en el menú Archivo</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Directorio de documentos por defecto</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation>Directorio de perfiles ICC por defecto</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Directorio de scripts por defecto</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation>Directorio adicional para Plantillas de Documentos</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Tamaño de página por defecto, ya sea un tamaño estándar o uno personalizado</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Orientación de las páginas por defecto</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Anchura de las páginas, edtable si has elegido un tamaño de página personalizado</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Altura de las páginas, edtable si has elegido un tamaño de página personalizado</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Esquema de página única o doble</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation>Hacer que la primera página esté a la izquierda</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Distancia entre la guía del margen superior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Distancia entre la guía del margen inferior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distancia entre la guía del margen izquierdo y el borde de la página.
Si las páginas enfrentadas están activadas, este espacio se puede usar
para lograr los márgenes correctos para la encuadernación.</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distancia entre la guía del margen derecho y el borde de la página.
Si las páginas enfrentadas están activadas, este espacio se puede usar
para lograr los márgenes correctos para la encuadernación.</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation>Cuando se activa, Scribus guarda una copia de seguridad del archivo
con la extensión .bak cada vez que pasa el período indicado</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Período de tiempo para el autoguardado</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Distancia entre las líneas menores de la cuadrícula</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Distancia entre las líneas mayores de la cuadrícula</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Distancia dentro de la que un objeto se ajustará a las guías</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Color de las líneas menores de la cuadrícula</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Color de las líneas mayores de la cuadrícula</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Color de las guías que se insertan</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation>Poner la cuadrícula detrás de los objetos de la página</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation>Poner la cuadrícula delante de los objetos de la página</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation>Activar las líneas de base</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation>Desactivar las líneas de base</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Desplazamiento sobre la línea de la fuente en una línea</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Tamaño relativo del superíndice comparado a la fuente normal</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Desplazamiento por debajo de la línea base de la fuente en una línea</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Tamaño relativo del subíndice comparado a la fuente normal</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Tamaño relativo de las versalitas comparado a la fuente normal</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Porcentaje de incremente sobre el tamaño de fuente para el interlineado</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Propiedades de Marco de Texto</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Propiedades de Marco de Imagen</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Propiedades de Dibujo de Formas</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Niveles de Zoom por Defecto</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Propiedades de Dibujo de Líneas</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Propiedades de Dibujo de Polígonos</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Fuente de los marcos de texto nuevos</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Tamaño de la fuente de los marcos de texto nuevos</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Color de la fuente</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Número de columnas en un marco de texto</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Hueco entre las columnas del marco de texto</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Ejemplo de la fuente</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Los marcos de imagen permiten el escalado de la imagen a cualquier tamaño</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Escalado horizontal de las imágenes</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Escalado vertical de las imágenes</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Mantener iguales los escalados horizontal y vertical</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Las imágenes se escalan al tamaño del marco</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Mantener las proporciones originales de las imágenes escaladas automáticamente</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Color de relleno de los marcos de imagen</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Intensidad del color de relleno</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Color de línea de las formas</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Saturación del color de las líneas</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Color de relleno de las formas</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Estilo de línea de las formas</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Ancho de línea de las formas</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Reducción mínima permitida</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Ampliación máxima permitida</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Cambio en el nivel de ampliación por cada operación de zoom</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Color de las líneas</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Saturación del color</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Estilo de las líneas</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Ancho de las líneas</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Número de vértices de los polígonos.</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Grados de rotación de los polígonos</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Aplicar el factor convexo/cóncavo para cambiar la forma de los polígonos</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Polígono de ejemplo</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation>Con un valor negativo el polígono será cóncavo (o con forma de estrella),
con un valor positivo será convexo</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Elige el tamaño de la previsualización en el album de recortes</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Guardar los contenidos del album de recortes después de un cambio</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation>Cuando se activa las páginas enfrentadas, muestra las dos páginas juntas</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Color del papel</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Color de las líneas de margen</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Enmascarar el área fuera de los margenes con el color del margen</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation>Activar las característcias de transparencia con la exportación de PDF 1.4</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Establecer el nivel de ampliación de la regla</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation>Localización del intérprete Ghostscript</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Suavizar (antialiasing) texto para la visualización en pantalla de EPS y PDF</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Suavizar (antialiasing) gráficos para la visualización en pantalla de EPS y PDF</translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation>Localización del editor de gráficos</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>No mostrar objetos fuera de los margenes en la página imprimida o el archivo exportado</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Una forma de desactivar algunas de las saturaciones grises que están compuestas
de cian, amarillo y magenta y en lugar de eso usar negro.
UCR afecta en su mayoría a partes de images que son de tonos neutrales u
oscuros cercanos al gris. Esto puede mejorar la impresión de algunas imágenes
y se necesita hacer pruebas en cada caso.
UCR reduce la posibilidad de sobresaturación con tintas CMY (cian, magenta, amarillo).</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Elige un directorio</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Interior:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>&amp;Exterior:</translation>
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
        <source>Executive</source>
        <translation type="unfinished">Ejecutivo</translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>T&amp;ono:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Sat:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>Va&amp;l:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Rojo:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Verde:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>A&amp;zul:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>Canal al&amp;fa:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>Colores &amp;básicos</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>Colores &amp;propios</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Definir colores propios &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Añadir a Colores Propios</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Seleccionar color</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Copiar o Mover un Archivo</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <comment>#, fuzzy</comment>
        <translation type="obsolete">Leer: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <comment>#, fuzzy</comment>
        <translation type="obsolete">Escribir: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>&amp;Nombre del fichero:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>&amp;Tipo de fichero:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Subir un directorio</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Tamaño</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Fecha</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Atributos</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Mirar &amp;en:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Atrás</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Crear Nueva Carpeta</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Vista de Lista</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Vista Detallada</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Previsualizar Información de Archivos</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Previsualizar Contenidos de Archivos</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Lectura-Escritura</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Sólo Lectura</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Sólo Escritura</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Inaccesible</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Enlace a Archivo</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Enlace a Directorio</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Enlace a Especial</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Archivo</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Dir</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Especial</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Guardar como</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Abrir</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Renombrar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>&amp;Recargar</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Ordenar por &amp;Nombre</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Ordenar por &amp;Tamaño</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Ordenar por &amp;Fecha</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Sin ordenar</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Ordenar</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Mostrar archivos &amp;ocultos</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>el archivo</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>el directorio</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>el enlace simbólico</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Borrar %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;¿Está seguro de que desea borrar %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sí</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;No</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Nuevo Carpeta 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Nueva Carpeta</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Nueva Carpeta %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Encontrar Directorio</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Directorios</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Guardar</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Error</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Archivo no encontrado.
Comprueba la ruta y el nombre del archivo.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Todos los Archivos (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Selecciona un Directorio</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Directorio:</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation type="unfinished">Leer: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation type="unfinished">Escribir: %1</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Fuente</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>&amp;Estilo de fuente</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Tamaño</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efectos</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>Tac&amp;hado</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Subrayado</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Color</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Ejemplo</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>Escr&amp;itura:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Aplicar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Cerrar</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Seleccionar fuente</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Borrar</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Seleccionar todo</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Deshacer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Re&amp;hacer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Cor&amp;tar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Preparar</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Personalizar...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Acerca de Qt&lt;/h3&gt;&lt;p&gt;Este programa usa la versión de Qt %1.&lt;/p&gt;&lt;p&gt;Qt es un conjunto de herramientas para el desarrollo de aplicaciones e interfaces gráficas multiplataforma.&lt;/p&gt;&lt;p&gt;Qt proporciona portabilidad entre MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux y la mayoría de las variantes de Unix.&lt;br&gt;Qt también está disponible para dispositivos empotrados.&lt;/p&gt;&lt;p&gt;Qt es un producto de Trolltech. Ver &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; para más información.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Importing text</source>
        <translation>Importando texto</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Todos los formatos soportados</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Albanés</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Vasco</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Bulgaro</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation>Brasileño</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Catalán</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Chino</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Checo</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Danés</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Holandés</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Inglés</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Inglés (Británico)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Alemán</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finlandés</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Francés</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Gallego</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Griego</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Húngaro</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Indonesio</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italiano</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Coreano</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Lituano</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>Noruego (Bokmål)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Noruego (Nynorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Noruego</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polaco</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Ruso</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Sueco</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Español</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Español (Latinoamérica)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Eslovaco</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Esloveno</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Serbio</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>La fuente %1 está rota, descartándola</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Iniciando...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Documento</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Fondo</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>¿Realmente quieres sobreescribir el archivo:
%1 ?</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation>Previsualizar &amp;Fuentes</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation>&amp;Insertar Carácter Especial</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Nuevo &amp;desde Plantilla...</translation>
    </message>
    <message>
        <source>Template: </source>
        <translation>Plantilla: </translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Boletines</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Folletos</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Catálogos</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Cuartillas</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Letreros</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Tarjetas</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Membretes</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Sobres</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Tarjetas profesionales</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Calendarios</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Anuncios</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Etiquetas</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menús</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programas</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>Formularios PDF</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>Presentaciones PDF</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Revistas</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Posters</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Anuncios</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Documentos de Texto</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Dípticos</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation>Cajas de discos</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Plantillas Propias</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Guardar como &amp;Imagen...</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Guardar como Imagen</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation>Se produjo un error escribiendo los archivos de salida.</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Se produjo un error escribiendo los archivos de salida.</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Exportación realizada con éxito.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>El archivo existe. ¿Sobreescribir?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>ya exste. ¿Sobreescribir?</translation>
    </message>
    <message>
        <source>No</source>
        <translation>No</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sí</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Sí a todo</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation>Previsualización de la Impresión</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>Pre&amp;visualizar</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Importar &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Todos los formatos soportados (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Guardar como P&amp;lantilla...</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>No se puede obtener un color sin nombre</translation>
    </message>
    <message>
        <source>Color not found</source>
        <comment>python error</comment>
        <translation>Color no encontrado</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>No se puede cambiar un color sin nombre</translation>
    </message>
    <message>
        <source>Color not found in document</source>
        <comment>python error</comment>
        <translation>Color no encontrado en el documento</translation>
    </message>
    <message>
        <source>Color not found in default colors</source>
        <comment>python error</comment>
        <translation>Color no encontrado en los colores por defecto</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>No se puede crear un color sin nombre</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>No se puede borrar un color sin nombre</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>No se puede reemplazar un color sin nombre</translation>
    </message>
    <message>
        <source>Failed to open document</source>
        <comment>python error</comment>
        <translation>Error abriendo el documento</translation>
    </message>
    <message>
        <source>Failed to save document</source>
        <comment>python error</comment>
        <translation>Error guardando el documento</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Unidad fuera de rango. Usa una de las constantes scribus.UNIT_*</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>El destino no es un marco de imagen.</translation>
    </message>
    <message>
        <source>Can&apos;t scale by 0%</source>
        <comment>python error</comment>
        <translation>No se puede escalar a 0%</translation>
    </message>
    <message>
        <source>Font not found</source>
        <comment>python error</comment>
        <translation>Fuente no encontrada</translation>
    </message>
    <message>
        <source>Can&apos;t render an empty sample</source>
        <comment>python error</comment>
        <translation>No se puede visualizar una cadena vacía</translation>
    </message>
    <message>
        <source>Can&apos;t save to a blank filename</source>
        <comment>python error</comment>
        <translation>No se puede guardar a un archivo sin nombre</translation>
    </message>
    <message>
        <source>Can&apos;t have an empty layer name</source>
        <comment>python error</comment>
        <translation>No se puede tener una capa sin nombre</translation>
    </message>
    <message>
        <source>Layer not found</source>
        <comment>python error</comment>
        <translation>Capa no encontrada</translation>
    </message>
    <message>
        <source>Can&apos;t remove the last layer</source>
        <comment>python error</comment>
        <translation>No se puede eliminar la última capa</translation>
    </message>
    <message>
        <source>Can&apos;t create layer without a name</source>
        <comment>python error</comment>
        <translation>No se puede crear una capa sin nombre</translation>
    </message>
    <message>
        <source>An object with the requested name already exists</source>
        <comment>python error</comment>
        <translation>Ya existe un objeto con el nombre solicitado</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values)</source>
        <comment>python error</comment>
        <translation>La lista de puntos debe tener al menos 2 puntos (4 valores)</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values</source>
        <comment>python error</comment>
        <translation>La lista de puntos debe tener un número par de valores</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values)</source>
        <comment>python error</comment>
        <translation>La lista de puntos debe tener al menos 3 puntos (6 valores)</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values)</source>
        <comment>python error</comment>
        <translation>La lista de puntos debe tener al menos 4 puntos (8 valores)</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values</source>
        <comment>python error</comment>
        <translation>La lista de puntos debe tener un múltiplo de 6 valores</translation>
    </message>
    <message>
        <source>Object not found</source>
        <comment>python error</comment>
        <translation>Objeto no encontrado</translation>
    </message>
    <message>
        <source>Style not found</source>
        <comment>python error</comment>
        <translation>Estilo no encontrado</translation>
    </message>
    <message>
        <source>Can&apos;t set style on a non-text frame</source>
        <comment>python error</comment>
        <translation>No puedo asignar un estilo a un marco que no es de texto</translation>
    </message>
    <message>
        <source>Failed to save EPS</source>
        <comment>python error</comment>
        <translation>Error guardando EPS</translation>
    </message>
    <message>
        <source>Page number out of range</source>
        <comment>python error</comment>
        <translation>Número de página fuera de rango</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values</source>
        <comment>python error</comment>
        <translation>El argumento no es una lista: debe ser una lista de valores float</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values</source>
        <comment>python error</comment>
        <translation>El argumento contiene valores no numéricos: debe ser una lista de valores float</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12</source>
        <comment>python error</comment>
        <translation>Ancho de línea fuera de rango, debe ser 0 &lt;= ancho_de_línea &lt;= 12</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation>Saturación de la línea fuera de rango, debe ser 0 &lt;= saturación &lt;= 100</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation>Saturación del relleno fuera de rango, debe ser 0 &lt;= saturación &lt;= 100</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>El radio de las esquinas debe ser un número positivo.</translation>
    </message>
    <message>
        <source>Line style not found</source>
        <comment>python error</comment>
        <translation>Estilo de línea no encontrado</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el tamaño de fuente de un marco que no es de texto</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener la fuente de un marco que no es de texto</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el tamaño del texto de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el número de columnas de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el interlineado de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener la distancia entre columnas de un marco que no es de texto</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el texto de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede establecer el texto de un marco que no es de texto</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede insertar texto en un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Insert index out of bounds</source>
        <comment>python error</comment>
        <translation>Índice de inserción fuera de rango</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Alineamiento fuera de rango. Usa una de las constantes scribus.ALIGN*.</translation>
    </message>
    <message>
        <source>Can&apos;t set text alignment on a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede establecer el alineamiento de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512</source>
        <comment>python error</comment>
        <translation>Tamaño de fuente fuera de rango - debe ser 1 &lt;= tamaño &lt;= 512</translation>
    </message>
    <message>
        <source>Can&apos;t set font size on a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede establecer el tamaño de fuente de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Can&apos;t set font on a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede establecer la fuente de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1</source>
        <comment>python error</comment>
        <translation>Interlineado fuera de rango debe ser &gt;= 0.1</translation>
    </message>
    <message>
        <source>Can&apos;t line spacing on a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede establecer el interlineado en un marco que no es de texto</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive</source>
        <comment>python error</comment>
        <translation>Distancia entre columnas fuera de rango, debe ser positiva</translation>
    </message>
    <message>
        <source>Can&apos;t column gap on a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede establecer una distancia entre columnas en un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1</source>
        <comment>python error</comment>
        <translation>Número de columnas fuera de rango, debe ser mayor de 1</translation>
    </message>
    <message>
        <source>Can&apos;t number of columns on a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede establecer el número de columnas de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Índice de selección fuera de rango</translation>
    </message>
    <message>
        <source>Can&apos;t select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede seleccionar texto de un marco que no es de texto</translation>
    </message>
    <message>
        <source>Can&apos;t delete text from a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede borrar texto de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Can&apos;t set text fill on a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede establecer el color de relleno del texto de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Can&apos;t set text stroke on a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede tachar texto en un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Can&apos;t set text shade on a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede establecer la saturación del texto de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Can only link text frames</source>
        <comment>python error</comment>
        <translation>Sólo se puede enlazar marcos de texto</translation>
    </message>
    <message>
        <source>Target frame must be empty</source>
        <comment>python error</comment>
        <translation>El marco de destino debe estar vacío</translation>
    </message>
    <message>
        <source>Target frame links to another frame</source>
        <comment>python error</comment>
        <translation>El marco de destino enlaza con otro marco</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame</source>
        <comment>python error</comment>
        <translation>El marco de destino está enlazado por otro marco</translation>
    </message>
    <message>
        <source>Source and target are the same object</source>
        <comment>python error</comment>
        <translation>El origen y el destino son el mismo objeto</translation>
    </message>
    <message>
        <source>Can&apos;t unlink a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede desenlazar un marco que no es de texto</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>El objeto no es un marco de texto enlazado, no se puede desenlazar.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>El objeto es el último marco de una serie, no se puede desenlazar. Desenlaza el marco anterior.</translation>
    </message>
    <message>
        <source>Can&apos;t convert a non-text frame to outlines</source>
        <comment>python error</comment>
        <translation>No se puede convertir un marco que no es de texto a contornos</translation>
    </message>
    <message>
        <source>Tried to set progress &gt; maximum progress</source>
        <translation>Se intentó establecer el progreso mayor que el máximo</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation>Manual de S&amp;cripter...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scripts de Scribus</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Ejecutar Script...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>Scripts &amp;Recientes</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Mostrar &amp;Consola</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>&amp;Acerca de un Script...</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation>S&amp;cript</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Referencia en línea</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Scripts de Python (*.py);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>Acerca del Script</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Guardar la página como &amp;SVG...</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Guardar como</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>Imágenes SVG (*.svg *.svgz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>Imágenes SVG (*.svg);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Importar &amp;SVG...</translation>
    </message>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Importar &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation>OpenOffice.org Draw (*.sxd);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Archivos de Valores Separados por Coma</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>datos_CSV</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>cabecera_CSV</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>Archivos HTML</translation>
    </message>
    <message>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation>
Enlaces externos</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>Documentos de OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Filtros de Texto</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Archivos de texto</translation>
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
      the text colour on a graphics frame doesn&apos;t make sense, and will result
      in this exception being raised.
    - Errors resulting from calls to the underlying Python API will be
      passed through unaltered. As such, the list of exceptions thrown by
      any function as provided here and in its docstring is incomplete.
Details of what exceptions each function may throw are provided on the
function&apos;s documentation.
</source>
        <translation type="obsolete">Módulo de interfaz de Python de Scribus 
Este módulo es la interfaz de Python para Scribus. Proporciona funciones
para controlar scribus y manipular objetos del lienzo. Cada función está
documentada individualmente abajo.
Hay unas pocas cosas comunes a través de gran parte de la interfaz.
La mayoría de las funciones trabajan sobre marcos. Los marcos se identifican
por su nombre, una cadena - no son objetos de Python reales. Muchas funciones
reciben como parámetro opcional (no clave) un nombre de marco.
También hay muchas excepciones comunes en la mayoría de las funciones.
Actualmente no están documentadas para cada función.
    - Muchas funciones lanzarán una excepción NoDocOpenError si intentas
      usarlas sin un documento en el que trabajar.
    - Si no se pasa un nombre de marco a una función que requiera uno,
      la función usara el marco seleccionado, si lo hay, o lanzará una excepción
      NoValidObjectError si no puede encontrar nada sobre lo que trabajar
    - Muchas funciones lanzarán una excpeción WrongFrameTypeError si se intentan
      sobre un tipo de marco sobre el que no tenga sentido. Por ejemplo, establecer
      el color del texto en un marco de imagen no tiene sentido, y dará lugar al
      lanzamiento de esta excepción.
    - Los errores resultantes de llamadas al API de Python no se alterarán.
      De esta forma, la lista de excepciones lanzadas por cualquier función,
      tal como se indica aquí y en su documentación, está incompleta. 
Los detalles de qué excepciones puede lanzar cada función se proporcionan
en la documentación de la función.
</translation>
    </message>
    <message>
        <source>Scribus Python interface module
<byte value="x9"/><byte value="x9"/>
<byte value="x9"/><byte value="x9"/>This module is the Python interface for Scribus. It provides functions
<byte value="x9"/><byte value="x9"/>to control scribus and to manipulate objects on the canvas. Each
<byte value="x9"/><byte value="x9"/>function is documented individually below.
<byte value="x9"/><byte value="x9"/>
<byte value="x9"/><byte value="x9"/>A few things are common across most of the interface.
<byte value="x9"/><byte value="x9"/>
<byte value="x9"/><byte value="x9"/>Most functions operate on frames. Frames are identified by their name,
<byte value="x9"/><byte value="x9"/>a string - they are not real Python objects. Many functions take an
<byte value="x9"/><byte value="x9"/>optional (non-keyword) parameter, a frame name.
<byte value="x9"/><byte value="x9"/>Many exceptions are also common across most functions. These are
<byte value="x9"/><byte value="x9"/>not currently documented in the docstring for each function.
<byte value="x9"/><byte value="x9"/>
<byte value="x9"/><byte value="x9"/>    - Many functions will raise a NoDocOpenError if you try to use them
<byte value="x9"/><byte value="x9"/>      without a document to operate on.
<byte value="x9"/><byte value="x9"/>
<byte value="x9"/><byte value="x9"/>    - If you do not pass a frame name to a function that requires one,
<byte value="x9"/><byte value="x9"/>      the function will use the currently selected frame, if any, or
<byte value="x9"/><byte value="x9"/>      raise a NoValidObjectError if it can&apos;t find anything to operate
<byte value="x9"/><byte value="x9"/>      on.
<byte value="x9"/><byte value="x9"/>
<byte value="x9"/><byte value="x9"/>    - Many functions will raise WrongFrameTypeError if you try to use them
<byte value="x9"/><byte value="x9"/>      on a frame type that they do not make sense with. For example, setting
<byte value="x9"/><byte value="x9"/>      the text colour on a graphics frame doesn&apos;t make sense, and will result
<byte value="x9"/><byte value="x9"/>      in this exception being raised.
<byte value="x9"/><byte value="x9"/>
<byte value="x9"/><byte value="x9"/>    - Errors resulting from calls to the underlying Python API will be
<byte value="x9"/><byte value="x9"/>      passed through unaltered. As such, the list of exceptions thrown by
<byte value="x9"/><byte value="x9"/>      any function as provided here and in its docstring is incomplete.
<byte value="x9"/><byte value="x9"/>
<byte value="x9"/><byte value="x9"/>Details of what exceptions each function may throw are provided on the
<byte value="x9"/><byte value="x9"/>function&apos;s documentation.
<byte value="x9"/><byte value="x9"/></source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Borrar</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Seleccionar todo</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Deshacer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Re&amp;hacer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Cor&amp;tar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Menú del Sistema</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Saturación</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Desenrollar</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Normalizar</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimizar</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maximizar</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Cerrar</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Restaurar</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Mover</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Tamaño</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Mi&amp;nimizar</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ma&amp;ximizar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>&amp;Mantener sobre el resto</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimizar</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Restaurar Tamaño</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Cerrar</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>Rec&amp;oger</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>&amp;Desenrollar</translation>
    </message>
</context>
<context>
    <name>Query</name>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
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
        <source>Document Setup</source>
        <translation>Configuración del documento</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Formato de página</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Tamaño:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation>Orientación:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Apaisado</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Anchura:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation>Altura:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Guías de los márgenes</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Superior:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Izquierda:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>I&amp;nferior:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>D&amp;erecha:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>Páginas en&amp;frentadas</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>&amp;Primero página izquierda</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Núme&amp;ro de la primera página:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Esquema de página única o doble</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Hacer la primera página del documento la página izquierda</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Distancia entre la guía del margen superior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Distancia entre la guía del margen inferior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distancia entre la guía del margen izquierdo y el borde de la página.
Si las páginas enfrentadas están activadas, este espacio se puede usar
para lograr los márgenes correctos para la encuadernación.</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distancia entre la guía del margen derecho y el borde de la página.
Si las páginas enfrentadas están activadas, este espacio se puede usar
para lograr los márgenes correctos para la encuadernación.</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Interior:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation>&amp;Exterior:</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style Settings</source>
        <translation>Configuración de Estilo</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Estilo del párrafo</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Fill Color Settings</source>
        <translation>Configuración del Color de Relleno</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Color de relleno del texto</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Saturación del color de relleno</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Configuración del Color de Trazo</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Color del trazo del texto</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Saturación del color de trazo</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source>Font Settings</source>
        <translation>Configuración de Fuente</translation>
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
        <source>Font of selected text</source>
        <translation>Fuente del texto seleccionado</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamaño de la fuente</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Escalar la anchura de los caracteres</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Configuración de Carácter</translation>
    </message>
    <message>
        <source>Kerning:</source>
        <translation>Ajuste entre caracteres:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation>Ajuste entre caracteres manual</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copia #%1 de </translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Fondo</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>File</source>
        <translation>Archivo</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation>Crear un nuevo documento</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation>Abrir un documento</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation>Guardar el documento actual</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation>Cerrar el documento actual</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation>Imprimir el documento actual</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation>Guardar el documento actual como PDF</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Buscando fuentes</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation>No hay ninguna fuente Postscript en el sistema</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation>Saliendo</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Error fatal</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Sistema de Fuentes Iniciado</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation>División Silábica Inteligente</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation>Alinear a la izquierda</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation>Alinear a la derecha</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation>Centrar</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Insertar Número de Página</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation>Adjuntar Texto al Trazado</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation>Mostrar Capas</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation>JavaScripts...</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation>Deshacer</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation>Mostrar Paleta de Páginas</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation>Bloquear/Desbloquear</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation>Espacio no separable</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Leyendo las Preferencias</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation>Obteniendo los Perfiles ICC</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation>Iniciar el Divisor Silábico</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Configurando los Accesos Rápidos</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation>Ajustes de &amp;Color...</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Leyendo el Álbum de recortes</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Iniciando las extensiones</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Abrir...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Abrir &amp;reciente</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Nuevo</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation>Abrir...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Cerrar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Guardar</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Gu&amp;ardar como...</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation>Guardar como...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>Revertir al guardado</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Rec&amp;olectar para salida...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation>&amp;Cargar texto/imagen...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>&amp;Añadir texto...</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation>Importar &amp;página(s)...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importar</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Guardar el &amp;texto...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Guardar la página como &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Guardar como P&amp;DF...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Exportar</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation>In&amp;formación del documento...</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation>Información del documento...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>Co&amp;nfiguración del documento...</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation>Configuración del documento...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Imprimir...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Imprimir...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Salir</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Salir</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Deshacer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Cor&amp;tar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>B&amp;orrar</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Seleccionar &amp;todo</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Cortar</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Copiar</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Pegar</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Borrar</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation>Seleccionar todo</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Buscar/Reemplazar...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>C&amp;olores...</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation>Colores...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>Estilos de &amp;párrafo...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>Estilos de &amp;línea...</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation>Estilos...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation>&amp;Plantillas...</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation>Plantillas...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation>&amp;JavaScripts...</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation>Seleccionar nueva fuente</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplicar</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation>Duplicar</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>Duplicación &amp;múltiple</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Duplicación múltiple</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Borrar</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>Agr&amp;upar</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Agrupar</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>&amp;Desagrupar</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation>Desagrupar</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>Blo&amp;quear</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>&amp;Enviar al fondo</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation>Enviar al fondo</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation>&amp;Traer al frente</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation>Traer al frente</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Bajar</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation>Bajar</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;Subir</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation>Subir</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation>Distribuir/&amp;Alinear...</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation>Distribuir/Alinear...</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation>&amp;Editar forma...</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Forma</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>&amp;Adjuntar Texto al Trazado</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>&amp;Desadjuntar Texto del Trazado</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>&amp;Combinar Polígonos</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Dividir &amp;Polígonos</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation>C&amp;onvertir a Contorno</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Insertar...</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation>Insertar...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Borrar...</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation>Borrar...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Mover...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation>Mover...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation>&amp;Aplicar Plantilla...</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation>Aplicar Plantilla ...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>Administrar &amp;Guías...</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation>Administrar Guías...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation>&amp;Ajustar a la ventana</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation>Ajustar a la ventana</translation>
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
        <source>&amp;100%</source>
        <translation>&amp;100%</translation>
    </message>
    <message>
        <source>100%</source>
        <translation>100%</translation>
    </message>
    <message>
        <source>200%</source>
        <translation>200%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation>&amp;Miniaturas</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation>Miniaturas</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Mostrar Má&amp;rgenes</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation>Ocultar Márgenes</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Mostrar &amp;Marcos</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation>Ocultar Marcos</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Mostrar &amp;Imágenes</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation>Ocultar Imágenes</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Mostrar &amp;Cuadrícula</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation>Mostrar Cuadrícula</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Mostrar &amp;Guías</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Mostrar Líneas de &amp;Base</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>&amp;Ajustar a la Cuadrícula</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation>Ajustar a la Cuadrícula</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>A&amp;justar a las Guías</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Propiedades</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Propiedades</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation>&amp;Esquema</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Esquema</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>Álb&amp;um de recortes</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Álbum de recortes</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>&amp;Capas</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation>Ocultar Paleta de P&amp;ágina</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Marcadores</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>Ad&amp;ministrar imágenes</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Administrar imágenes</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;Dividir en sílabas</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation>Dividir en sílabas</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>Ayuda &amp;emergente</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Herramientas</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>Herramientas de P&amp;DF</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Herramientas</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation>Ayuda emergente</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>&amp;Preferencias...</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation>&amp;Fuentes...</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation>Fuentes...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation>&amp;División silábica...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation>&amp;Accesos rápidos...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>&amp;Acerca de Scribus</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation>Acerca de Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>Acerca de &amp;Qt</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Acerca de Qt</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>&amp;Manual de Scribus</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation>Ayuda en línea...</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>E&amp;stilo</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Objeto</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Página</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Ver</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtras</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Preferencias</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>&amp;Ventanas</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>A&amp;yuda</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Izquierda</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Centro</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>&amp;Derecha</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Justificado</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Forzado</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Otro...</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Subrayado</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Tachar</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Versalitas</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Superíndice</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Subíndice</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Contorneado</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>Posición X:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Posición Y:</translation>
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
        <source>Ready</source>
        <translation>Preparado</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Cascada</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Mosaico</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation>Cargar texto/imagen...</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation>Cargar imagen...</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Color</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation>&amp;Invertir</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>Car&amp;gar texto...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Fuente</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Tamaño</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Efectos</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Alineación</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Saturación</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>Ta&amp;buladores...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Cargar texto...</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Fuente</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Tamaño</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Estilo</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Color</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Saturación</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>Desblo&amp;quear</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Importar páginas...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importar Página(s)</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation>&lt;p&gt;Estás intentando importar más páginas que las disponibles en este documento a patir de la página actual.&lt;/p&gt;Elige una de las siguientes opciones:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Crear&lt;/b&gt; las páginas que falten&lt;/li&gt;&lt;li&gt;&lt;b&gt;Importar&lt;/b&gt; las páginas hasta la últma&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancelar&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Crear</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Importar</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Importación realizada</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>No hay nada que importar</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation>El archivo %1 no está en formato de Scribus</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Cargando...</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Todos los formatos soportados</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation>No puedo escribir el archivo: 
%1</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Guardar como</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Guardando...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Imprimiendo...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Documento</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>La impresión falló</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Manual de Scribus</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Archivos de texto (*.txt);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaño:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Saturación:</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation>Ocultar Líneas de Base</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Mostrar Líneas de Base</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation>Faltan los siguientes programas:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation>Ghostscript : No puedes usar imágenes EPS</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Todos</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation>Archivos EPS (*.eps);;Todos los archivos (*)</translation>
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
        <source>Some Objects are locked.</source>
        <translation>Algunos objetos están bloqueados.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation>Bloquear todo</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation>Desbloquear todo</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Desbloquear</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Bloquear</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation>Cargando:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Ajustando Colores</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation>&amp;Deshacer borrar objeto</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation>&amp;Deshacer mover objeto</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation>&amp;Deshacer cambios en el objeto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Alemán</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polaco</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Inglés</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Español</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italiano</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Francés</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Ruso</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Danés</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Eslovaco</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Húngaro</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Checo</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Holandés</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugués</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation>Ucraniano</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Griego</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Catalán</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finlandés</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation>Irlandés</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Lituano</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Sueco</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Esloveno</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikaans</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Elige un directorio</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertir</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Scribus se ha cerrado</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus se ha cerrado debido a la señal #%1</translation>
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
        <translation>Capa</translation>
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
        <source>All</source>
        <translation>Todos</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation>&amp;Dejar como está</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation>C&amp;errar sin guardar</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Documento:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>El documento se ha modificado desde la última vez que lo guardó.</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation>&amp;Guardar ahora</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Mostrar Previsualización de Páginas</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation>Mostrar nombres de plantilla</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Buscar/Reemplazar</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Buscar:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Estilo de Párrafo</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Fuente</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamaño de la fuente</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Efectos de Fuente</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Color de Relleno</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Saturación de Relleno</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Color de Trazo</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Saturación de Trazo</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Izquierda</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centrado</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Derecha</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Justificado</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Forzado</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Reemplazar con:</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Palabras Completas</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Ignorar mayúsculas/minúsculas</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Buscar</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Reemplazar</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Reemplazar &amp;Todo</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>B&amp;orrar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Búsqueda finalizada</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Arrange Pages</source>
        <translation>Ordenar Páginas</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation>Plantillas Disponibles:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Páginas del Documento:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation>Páginas enfrentadas</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation>Primero la página izquierda</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation>Arrastrar páginas o plantillas a la papelera para borrarlas.</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation>Aquí puedes ver todas las páginas del documento.</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation>Aquí están todas tus plantillas; para crear una nueva página
arrastra una plantilla en la vista de páginas de abajo.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Seleccionar Campos</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Campos disponibles</translation>
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
        <source>Selected Fields</source>
        <translation>Campos Seleccionados</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Otro...</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Saturación:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Saturación</translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Sin estilo</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Sin estilo</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Editar Estilos</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>A&amp;ñadir</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copia de %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nuevo Estilo</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation>¿Realmente desea borrar este Estilo?</translation>
    </message>
    <message>
        <source>No</source>
        <translation>No</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sí</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);; Todos los archivos (*)</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Editor Interno</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Recargar Texto del Marco</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Guardar a archivo...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Cargar de archivo...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Guardar &amp;documento</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Actualizar el Marco de Texto y Salir</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>&amp;Salir sin Actualizar el Marco de Texto</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Seleccionar &amp;todo</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Cor&amp;tar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>B&amp;orrar</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Buscar/Reemplazar...</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation>&amp;Insertar Carácter Especial...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>&amp;Editar Estilos...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>Previsualizar &amp;Fuentes...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Actualizar el Marco de Texto</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>Fondo del &amp;editor...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Fuente a usar...</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Selección de texto inteligente</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Preferencias</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Archivo</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation>Borrar todo el texto</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation>Cargar Texto de Archivo</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>Guardar Texto a Archivo</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation>Actualizar el Marco de Texto y Salir</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>Salir sin Actualizar el Marco de Texto</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>Recargar Texto del Marco</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation>Actualizar el Marco de Texto</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Buscar/Reemplazar</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Párrafo actual:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Palabras: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Caracteres: </translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Totales:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Párrafos: </translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>¿Desea guardar los cambios?</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation>¿Realmente desea perder todos los cambios?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>¿Realmente desea borrar todo el texto?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Archivos de texto (*.txt);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Guardar como</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation>Subrayado</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Versalitas</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Subíndice</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Superíndice</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Tachado</translation>
    </message>
    <message>
        <source>Outline Text</source>
        <translation>Contorno</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Opciones de la Importación de OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Update paragraph styles</source>
        <translation>Actualizar estilos de párrafo</translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation>Si ya existe un estilo de párrafo con el mismo nombre que el de un párrafo
del documento de OpenOffice.org, el estilo de Scribus se debería modificar
de acuerdo al importado, o dejarse intacto.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Usa nombre del documento como un prefijo para los estilos de párrafo</translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation>Si se debe añadir el nombre del documento delante
del nombre del estilo de párrafo al importarlo a Scribus</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>No preguntar de nuevo</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OpenOffice.org document and
never ask your confirmation again</source>
        <translation>Si se deben usar siempre las opciones elegidas ahora cuando se
importa un documento de OpenOffice.org y no preguntar de nuevo</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Administrar los tabuladores</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Izquierda</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Derecha</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Punto</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Coma</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centrado</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>P&amp;osición:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation>Pri&amp;mera Línea:</translation>
    </message>
    <message>
        <source>Left Ind&amp;ent:</source>
        <translation>&amp;Sangría izquierda:</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Borrar Todos</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Sangría de la primera línea del párrafo</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Sangría izquierda de todo el párrafo</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Borrar todos los tabuladores</translation>
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
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>Esquema</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Elemento</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Información</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>El nombre &quot;%1&quot; no es único.
Por favor elige otro.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
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
        <translation>Fuente:</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Grupo </translation>
    </message>
    <message>
        <source>Image</source>
        <translation>Imagen</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Línea</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polígono</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polilínea</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>TextoTrazado</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Página</translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation>Insertar valor</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Introducir un valor y presionar Aceptar.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Introducir un valor y presionar Aceptar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Enviar el valor al script</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Herramientas</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation>Seleccionar objetos</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Insertar marco de texto</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation>Insertar imagen</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Insertar tabla</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Propiedades...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation>Insertar polígonos</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation>Insertar lineas</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation>Insertar Curva de Bézier</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Insertar linea a mano alzada</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Rotar objeto</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Acercar o alejar</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Editar contenidos del marco</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Editar el texto con el editor interno</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Enlazar marcos de texto</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Desenlazar marcos de texto</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation>Medir</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation>Dibujar varias formas</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>PDF Tools</source>
        <translation>Herramientas de PDF</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Botón</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Campo de texto</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Casilla de selección</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Cuadro combinado</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Lista</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Insertar campos de PDF</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Enlace</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Insertar anotaciones de PDF</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation>Seleccionar Carácter:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Insertar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>B&amp;orrar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Inserta los caracteres en el texto en la posición del cursor</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Borra la selección actual</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation>Cierra este diálogo y vuelve a la edición de texto</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Elige que importación usar</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automática</translation>
    </message>
    <message>
        <source>Get text only</source>
        <translation>Sólo obtener texto</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Importar texto sin ningún formato</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Importación:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Codificación:</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Elige que importación usar</translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Recordar asociación</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation>Recordar la asociación extensión - importación y no preguntar
de nuevo que importación hacer para archivos de este tipo.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Nuevo desde Plantilla</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Abrir</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Todos</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Formato de página</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Colores</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Descripción:</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Uso</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Creado con</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Fecha</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Descargando Plantillas</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Se pueden encontrar plantillas de documento en http://www.scribus.net/ en la sección descargas (Downloads).</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Instalando Plantillas</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Extrae el paquete al directorio de plantillas ~/.scribus/templates para el usuario actual o en PREFIX/usr/share/scribus/templates para todos los usuarios del sistema.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Preparando una pantilla</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Asegurate que las imágenes y fuentes que usas se pueden usar libremente. Si las fuentes no se pueden compartir no las recolectes cuando lo guardes como una plantilla.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>El creador de la plantilla debería asegurarse también de que la sección de Instalar Plantillas de arriba se aplica a sus plantillas también. Esto quiere decir que un usuario debería poder descargar la plantilla y extraerla al directorio de plantillas y usarla.</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Eliminando una plantilla</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Eliminar una plantilla del diálogo Nueva desde Plantilla sólo eliminará la entrada de template.xml, no borrará los archivos. Sólo se muestra un menú contextual con la opción de eliminar si tiene permiso de escritura en el archivo template.xml</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Traduciendo template.xml</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Copia un archivo template.xml que ya exista a un archivo llamado template.idioma_PAÍS.xml (usa el mismo código de idioma que en el archivo qm de tu idioma), por ejemplo template.fi.xml para Finlandés. La copia debe estar en el mismo directorio que el original template.xml para que Scribus pueda cargarlo.</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Guardar como Plantilla</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Categoría</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Formato de página</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Colores</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Descripción:</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Uso</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>Correo electrónico</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Más detalles</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Menos detalles</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Carta</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloide</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>Apaisado</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>Personalizado</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Crear filtro</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>B&amp;orrar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Elegir un filtro guardado previamente</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Dar un nombre a este filtro para guardarlo</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Darle un nombre para guardarlo</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Desactivar o activar esta fila del filtro</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Eliminar esta fila del filtro</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Añadir una nueva fila de filtro</translation>
    </message>
    <message>
        <source>to</source>
        <translation>a</translation>
    </message>
    <message>
        <source>and</source>
        <translation>y</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>eliminar las coincidencias</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>no eliminar las coincidencias</translation>
    </message>
    <message>
        <source>words</source>
        <translation>palabras</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Borrar</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Reemplazar</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Aplicar</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>El valor de la izquierda es una expresión regular</translation>
    </message>
    <message>
        <source>with</source>
        <translation>con</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>estilo de párrafo</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>todas las instacias de</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>todos los párrafos</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>los párrafos que empiezan con</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>los párrafos con menos de</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>los párrafos con más de</translation>
    </message>
</context>
</TS>
