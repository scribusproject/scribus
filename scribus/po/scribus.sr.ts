<!DOCTYPE TS><TS>
<context>
    <name></name>
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
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
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
</context>
<context>
    <name>@default</name>
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
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yellow and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
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
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
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
</context>
<context>
    <name>About</name>
    <message>
        <source>About Scribus%1%2</source>
        <translation type="obsolete">О Scribus-у%1%2</translation>
    </message>
    <message>
        <source>%1. %2 %3 </source>
        <translation type="obsolete">%1. %2 %3 </translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation>Scribus верзија %1
%2 %3</translation>
    </message>
    <message>
        <source>Build-ID:</source>
        <translation>ID градње:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>О прогр&amp;аму</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Развојни тим:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Прилози од стране:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation>Порт за windows:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Званична документација:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Остала документација:</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>А&amp;утори</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation>Баскијски:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation>Бразилски:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation>Бугарски:</translation>
    </message>
    <message>
        <source>Catalan:</source>
        <translation>Каталански:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation>Чешки:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation>Дански:</translation>
    </message>
    <message>
        <source>English (British):</source>
        <translation>Енглески (Британски):</translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation>Есперанто:</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation>Фински:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation>Француски:</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation>Галицијски:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation>Немачки:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation>Мађарски:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation>Италијански:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation>Корејски:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation>Литвански:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation>Норвешки:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation>Пољски:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation>Руски:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation>Словачки:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation>Словеначки:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation>Шпански:</translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation>Шведски:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation>Турски:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation>Украјински:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation>Велшки:</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;Преводи</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Почетна страница</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Веб референца</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Грешке и захтеви за нове могућности</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Дискусиона листа</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>На &amp;вези</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Затвори</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Овај панел приказује верзију, датум прављења и
 укомпајлирану подршку за библиотеке у Scribus-у
C-C-T означава: C=CUPS C=littlecms T=TIFF подршку.
Подршка за библиотеке које недостају је обележена *</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Званични преводи и преводиоци:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation>Српски:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Они који су претходно допринели превођењу:</translation>
    </message>
    <message>
        <source>About Scribus %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=littlecms C=CUPS T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 %2 %3 </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Afrikaans:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Portuguese (Brazilian):</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation>Напредне опције</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation>Окрени страницу(е) &amp;водоравно</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation>Окрени страницу(е) &amp;усправно</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation>Примени под &amp;уклањањем боја</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation>Примени &amp;ICC профиле</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation>PostScript ниво &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation>PostScript ниво &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation>PostScript ниво &amp;3</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation>Прави PostScript ниво 3</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation>Прави PostScript ниво 2 само, опрез,
ово може направити огромне фајлове</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation>Прави PostScript ниво 1 само, опрез, 
oво може направити огромне фајловеНиво огроман</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Начин за искључивање неких сивих сенки које су састављене
од цијан, жуте, и магенте и коришћења црне уместо тога.
UCR највише утиче на делове слике који имају неутралне и/или тамне тонове
који су блиски сивој. Коришћење овога може поправити штампу неких слика
и потребно је мало експериментисања у зависности од случаја.
UCR смањује могућност презасићености CMY мастилима</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This enables you to explicitely set,
the media size of the postscript file.
Not recommended unless
 requested by your printer.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Distribute/Align</source>
        <translation>Распореди/Поравнај</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Поравнај</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Водоравно</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation>Леве стране</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation>Средине</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation>Десне стране</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation>&amp;Између:</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation>&amp;Немој да мењаш</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation>П&amp;оравнај</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation>Погрешно &amp;постављање</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation>Распореди &amp;равномерно</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Усправно</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation>Горње стране</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation>Доње стране</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation>Изм&amp;еђу:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation>&amp;Немој да мењаш</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation>По&amp;равнај</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation>Погрешно &amp;постављање</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
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
        <translation>Распореди рав&amp;номерно</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Примени</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation>Поравнај текст лево</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Поравнај текст десно</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Поравнај текст по средини</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Поравнај текст развучено</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Поравнај текст насилно развучено</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Својства поља</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Врста:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Дугме</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Поље за текст</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Кућица</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Комбо кутија</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Кутија листе</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Својства</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Име:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Алат-Савет:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Фонт за коришћење са PDF 1.3:</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Оквир</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Боја:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ниједан</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Ширина:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Танак</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Нормално</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Широк</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Стил:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Пуно</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Испрекидано</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Подвучено</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Искошено</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Уметак</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Остало</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Само за читање</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Захтевано</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Немој да извозиш вредност</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Видљивост:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Видљиво</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Скривен</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Не штампај</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Без приказа</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Изглед</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Текст за Дугме Доле</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Текст за превртање</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Иконе</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Користи иконе</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Уклони</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Притиснуто</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Превртање</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Постављање икона...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Истицање</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Инвертуј</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Уоквирено</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Гурни</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Мулти-линија</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Лозинка</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Лимит од</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Знакови</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Немој да скролујеш</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Немој да провераваш правопис</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Провери стил:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Провери</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Крст</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Дијамант</translation>
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
        <translation>Подразумевано је обележено</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Може се уређивати</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Опције</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation>Java Script</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Иди на</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Пошаљи формулар</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Ресетуј формулар</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Увези податке</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Догађај:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Миш горе</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Миш доле</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Миш унеси</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Миш изађи</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>На фокус</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>На замућење</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Скрипта:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Уреди...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Пошаљи до URL-а:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Пошаљи податке као HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Увези податке од:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Одредиште</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>У фајл:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Измени...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Страна:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X Поз:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y Поз:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Акција</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Поље је форматирано као:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Обичан</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Број</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Проценат</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Датум</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Време</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Произвољно</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Формат бројева</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Децимале:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Користи симбол валуте</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Валута Симбол</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Форматирање</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Процентни формат</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Формат датума</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Формат времена</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Произвољне скрипте</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Формат:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Притисак дугмета:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Формат</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Вредност није оверена</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Вредност мора бити већа или једнака са:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>и мања или једнака са:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Произвољне скрипте за оверавање:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Овери</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Вредност није израчуната</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Вредност је</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>сума</translation>
    </message>
    <message>
        <source>product</source>
        <translation>производ</translation>
    </message>
    <message>
        <source>average</source>
        <translation>средња вредност</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>минимум</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>максимум</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>од следећих поља:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Изабери...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Произвољна скрипта за израчунавање:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Израчунај</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Поништи</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Застава се игнорише за PDF 1.3</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Овде унесите листу поља одвојених зарезима</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Потребна Вам је барем икона за Нормално да бисте користили икону за дугмад</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Отвори</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation>Слике (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Пример:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Промена избора</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF фајлови (*.pdf);;Сви фајлови (*)</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Својства анотације</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Веза</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Спољашња веза</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Спољашња веб-веза</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Одредиште</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Отвори</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF-Документи (*.pdf);;Сви фајлови (*)</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Врста:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>И&amp;змени...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Страница:</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X-Поз</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y Поз:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Apply Template</source>
        <translation>Примени шаблон</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>Ша&amp;блон:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Нормално</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation>Примени на &amp;текућу страницу</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation>Примени на све &amp;парне странице</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation>Примени на све &amp;непарне стране</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation>Примени од &amp;стране:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>За:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>Албум</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Нови</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Учитај...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Сними</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Сними &amp;као...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Затвори</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>&amp;Мала</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>&amp;Средње</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>&amp;Велики</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Фајл</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Преглед</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>Албуми (*.scs);;Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Преименуј</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Обриши</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Име „%1“ није јединствено.
Молим Вас изаберите друго.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Објекат</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Име:</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Нов унос</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>Маркери</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>Помери маркер</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Уметни маркер</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Поништи</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Маркери</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Постављање икона</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Распоред:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Само натпис</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Само икона</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Натпис испод иконе</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Натпис изнад иконе</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Натпис десно од иконе</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Натпис лево од иконе</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Натпис преко иконе</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Размера:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Увек</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Када је икона превише мала</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Када је икона превише велика</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Никад</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Скалирај како:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Пропорционалан</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Непропорционално</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Икона</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Поништи</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Ресетуј</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Color Management Settings</source>
        <translation>Подешавање управљања бојама</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Активирај управљање бојама</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Системски профили</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation>&amp;Слике:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>&amp;Пуне боје:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Надгледач:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>Ш&amp;тампач:</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Намере рендеровања</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Перцептуално</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Релативна колорметрика</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Засићење</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Апсолутна колорметрика</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>М&amp;онитор:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>Шт&amp;ампач:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Симу&amp;лирај штампач на екрану</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Означи боје изван &amp;gamut-а</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Користи компензацију &amp;црне тачке</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation>Подразумевани профил боја за увезене слике</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Подразумевани профил боја за пуне боје на страници</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Профил боја који сте направили или добили од произвођача.
Овај профил би требало да буде баш за Ваш монитор, а не генерички профил (тј. sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Профил боја за Ваш модел штампача од произвођача.
Овај профил би требало да буде баш за Ваш штампач, а не генерички профил (тј. sRGB).</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Подразумеване намере рендеровања за Ваш монитор. Уколико не знате зашто ово да промените,
требало би да буде изабрано „Релативна колорметрика“ или „Перцептуално“.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Подразумеване намере рендеровања за Ваш штампач. Уколико не знате зашто ово да промените,
требало би да буде изабрано „Релативна колорметрика“ или „Перцептуално“.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Укључи „меко проверавање“ како ће се штампати боје у Вашем документу,
на основу изабраног профила за штампач.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Метод за приказивање боја на екрану које можда неће бити добро одштампане.
Ово захтева веома прецизне профиле и служи само као упозорење.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Компензација црне тачке је метод поправљања контраста на фотографијама.
Препоручљиво је да ово укључите ако имате фотографије у документу.</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Уређивање боја</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Име:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>&amp;Модел боја</translation>
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
        <translation>Веб сигуран RGB</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Нови</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Стари</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>HSV мапа боја</translation>
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
        <translation>Динамичке линије боја</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Статичне линије боја</translation>
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
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation>Име боје није јединствено</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ниједан</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation>Не можете направити боју под називом „%1“. 
То име је резервисано за провидну боју</translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">У реду</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Поништи</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Shade:</source>
        <translation>Сенка:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Непрозирност:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Нормално</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Водоравни прелаз</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Усправни прелаз</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Дијагонални прелаз</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Укрштени дијагонални прелаз</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Радијални прелаз</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Слободни линијски прелаз</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Слободни радијални прелаз</translation>
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
        <translation> тач.</translation>
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
        <translation>Уреди поставке линије са бојама</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Уреди поставке попуњености бојама</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Засићење боје</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Нормални или прелазни метод попуњавања</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Постави прозирност за изабрану боју</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation>Боја изабраног објекта</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ниједан</translation>
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
        <translation>Опције CSV увозника</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Границе поља:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Граничне вредности:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ниједан</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Први ред је заглавље</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Поништи</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Опције штампача</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Поставка странице</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Све странице</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Само парне странице</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Само непарне странице</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Огледало</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Не</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Усмерење</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Усправно</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Водоравно</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>N-Горе штампање</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Једна страница по листу</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Број страница на листу</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Опција</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Вредност</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Овај панел приказује различите опције CUPS-а током штампања. 
Доступни параметри ће зависити од управљачког програма Вашег штампача.
Можете потврдити CUPS подршку избором „Помоћ &gt; О програму“.
Тражите ставке: C-C-T Оне означавају C=CUPS C=littlecms T=TIFF подршку.
Непостојећа подршка за библиотеке је означена *</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Компресуј фајл</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Укључи фонтове</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Кодирање:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Премешта у Ваш директоријум за документе.
Ово се може наместити у поставкама.</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Обриши боју</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation>Обриши боју:</translation>
    </message>
    <message>
        <source>?</source>
        <translation>?</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation>Замени је са:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">У реду</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Поништи</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ниједан</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Обриши странице</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation>Обриши од:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>у:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>Missing Font</source>
        <translation>Недостаје фонт</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Фонт %1 није инсталиран.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Користи</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>уместо</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Информације о документу</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Наслов:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Аутор:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Кључне речи:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>Оп&amp;ис:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>И&amp;здавач:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;Помагачи:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>&amp;Датум:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>В&amp;рста:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>Ф&amp;ормат:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>Означ&amp;ивач:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Извор:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Језик:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;Однос:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>Покр&amp;ивеност:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>Пр&amp;ава:</translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation>&amp;Документ</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Додатне &amp;информације</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Особа или организација примарно одговорна за прављење садржаја документа.
Ово поље се може уметнути у scribus документ као референца, као и у meta податке PDF-а</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Име дато документу.
Ово поље се може уметнути у scribus документ као референца, као и у meta податке PDF-а</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Извод из садржаја документа.
Ово поље служи за кратак опис или апстракт документа. Умеће се у PDF приликом изласка</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Тема садржаја документа.
Ово поље служи за кључне речи документа које желите да уметнете у PDF, да би побољшали индексирање и претрагу PDF фајлова</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Особа или организација одговорни за издавање документа</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Особа или организација одговорни за допринос садржају документа</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Датум повезан са неким догађајем у животном циклусу документа, у ГГГГ-ММ-ДД формату, као по ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Природа или жанр садржаја документа, нпр. категорије, функције, жанрови, итд.</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Физичка или дигитална манифестација документа. Врста медија и димензије не би ничему служили.
RFC2045,RFC2046 за MIME врсте су такође корисни овде</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Прецизна референца на документ у оквиру датог контекста као што је ISBN 
или URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Референца на документ из кога је тренутни документ изведен, нпр. 
ISBN или URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Језик којим је написан садржај документа, обично ISO-
639 језички код
коме опционо претходи цртица и ISO-3166 код земље, нпр. en-GB, fr-
CH</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Референца на повезани документ, вероватно коришћењем формалног идентификатора као што је ISBN или URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Степен или делокруг садржаја документа, вероватно укључујући место, време и опсеге надлежности</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Информације о правима у и над документом, нпр. copyright, патент или бренд</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation>Подеси штампач</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Одредиште за штампу</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Фајл</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Опције...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Фајл:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>И&amp;змени...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>А&amp;лтернативна наредба штампача</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation>Користи или до</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>&amp;Наредба:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Опсег</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Штампај &amp;све</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Штампај текућу стр&amp;аницу</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>&amp;Опсег штампања</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Унесите листу обележја раздвојених зарезима где
обележје може бити * за све странице, 1-5 за
опсег страница или број једне странице.</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>Б&amp;рој копија:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Опције</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation>Штампај &amp;нормално</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation>Штампај &amp;раздвајања</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Све</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Цијан</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Магента</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Жута</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Црна</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation>Шт&amp;ампај у боји ако је доступно</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation>Штампај у &amp;сивим тоновима</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation>&amp;Напредне опције...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Штампај</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Сними као</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation>Postscript-фајлови (*.ps);;Сви фајлови (*)</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Увожење фајла: 
%1 
није успело!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Фатална грешка</translation>
    </message>
</context>
<context>
    <name>EditMacroDialog</name>
    <message>
        <source>Editing Macro: &lt;b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; already exists.
Are you sure you want to overwrite it?
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You have already edited this macro.
Are you sure you want to discard all your changes?
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A full traceback follows:

%1
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compilation of the macro failed, so it can not 
be saved in its current form. The error was:
%1
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;This is the Edit Macro / New Macro dialog box. Here you can change the source code to macros. Edit the source code to the macro in the text editing area below the &quot;Source Code&quot; label and click OK to save your changes to the macro.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Source Code:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Editing Macro:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The name of the macro being edited.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;/qt&gt;This item displays the name of the macro you are currently editing.&lt;qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Откажи</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Discard all changes and exit.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Exit the editing dialog, discarding all changes you have made. If you want to exit without saving the macro but don&apos;t want to lose your changes, save your changes with &quot;Save Source As...&quot;.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes and exit.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes to the macro and exit. If there is a problem with the macro, a message will be displayed and the editing dialog will not close.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;This text area contains the source code of the macro. If you&apos;re creating a new macro there won&apos;t be anything in it, and if you&apos;re editing an existing macro the source code the macro was defined with will be shown here.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Load Source ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace the current source code with code from a file.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Load new source code into the editing area from &quot;file&quot;. Any source code in the editing area is replaced. The loaded source must be a Scribus macro function. If you load any other script, you&apos;ll need to edit it so that it&apos;ll work as a scripter macro before saving it.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save Source As...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save the source code being edited to a file.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save the source code - the text - of the macro to a file. You can edit the saved source and load it again with &quot;Load Source...&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Python source files (.py)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save File Dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save macro source</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open File Dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select the source file to load</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Уреди стил</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Име:</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Знак</translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation>&amp;Фонт:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation>&amp;Величина:</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation>Ефекат:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation>&amp;Поравнање:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation>&amp;Испусти велика слова</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Линије:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Боја за поп&amp;уњавање:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation>Боја за црт&amp;ање:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ниједан</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation>Усправни размаци</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation>Прилагоди основној &amp;мрежи</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation>&amp;Проред:</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation>Изн&amp;ад:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation>&amp;Испод:</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Табулатори и увлачење</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Име Вашег стила пасуса</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Фонт изабраног текста или објекта</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Величина фонта</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Боја попуњеног текста</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Боја текста</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation>Обезбеђује увеличано прво слово за пасус. Користи се ради стилског ефекта</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Утврђује свеукупну висину, бројеве у низу, испуштених великих слова</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation>Поравнај текст са основном мрежом</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Проред изнад пасуса</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Проред испод пасуса</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Проред</translation>
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
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation>Име стила није јединствено</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Уређивач</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Нови</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Отвори...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Сними &amp;као...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Сними и изађи</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Изађи без снимања</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Опозови</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Поно&amp;ви</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Исеци</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Копирај</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Пренеси</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Оч&amp;исти</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Добави имена поља</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Фајл</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Уреди</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation>Javascripts (*.js);;Сви фајлови (*)</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Изаберите директоријум за извоз</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Извези као слику(е)</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Извези у директоријум:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>И&amp;змени...</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Опције</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>&amp;Врста слике:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Квалитет:</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Резолуција:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> тпи</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Опсег</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>&amp;Текућа страница</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Све странице</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Опсег</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Извези опсег страница</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Унесите листу обележја раздвојених зарезима где
обележје може бити * за све странице, 1-5 за
опсег страница или број једне странице.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Извези све странице</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Извези само текућу страницу</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Резолуција слика
Користи 72 тпи за слике предвиђене за приказивање на екрану</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Квалитет Ваших слика - 100\\% је најбољи, 1\\% је најлошији</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Доступни формати за извоз</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Излазни директоријум - место за смештање Ваших слика.
Име извезеног фајла ће бити „имедокумента-бројстране.врстафајла“</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Промени излазни директоријум</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Величина:</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Величина:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Наслов:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Без наслова</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Аутор:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Непознат</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus документ</translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Colors</source>
        <translation>Боје</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Примени</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Нови</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Уреди</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Д&amp;уплирај</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Обриши</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>&amp;Уклони некоришћено</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Скупови боја</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Текући скуп боја:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Сними скуп боја</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Изаберите скуп боја за учитавање</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Сними текући скуп боја</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Уклони некоришћене боје из скупа боја текућег документа</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation>Примени боје на текући скуп из постојећег документа</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Направи нову боју у оквиру текућег скупа</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Уреди тренутно изабрану боју</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Направи копију тренутно изабране боје</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Обриши тренутно изабрану боју</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Учини да тренутни скуп боја буде подразумевани</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Име:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Изаберите име</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Отвори</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Документи (*.sla *.sla.gz *.scd *.scd.gz);;Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Документи (*.sla *.scd);;Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Копија од %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Нова боја</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ниједан</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Global Font Settings</source>
        <translation>Глобалне поставке фонтова</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Доступни фонтови</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Замене за фонтове</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Додатне путање</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Име фонта</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation>Користи фонт</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation>Уметни у:</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Подсет</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Врста</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation>Путања до фајла фонта</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>&amp;Доступни фонтови</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Замена</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Обриши</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Замене за фонтове</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>И&amp;змени...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>&amp;Додај...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Уклони</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>Додатне &amp;путање</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Изаберите директоријум</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Fonts Preview</source>
        <translation>Преглед фонтова</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Плетене свилене пиџаме замењене за плави кварц</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Size of the Font</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Позиција:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Овде можете додавати, мењати или уклањати зауставне боје.</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Управљање водиљама</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
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
        <translation>Водоравне водиље</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y Поз:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Додај</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>&amp;Обриши</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Усправне водиље</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X Поз:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>&amp;Додај</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>&amp;Обриши</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Закључај водиље</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Извините, нема доступног упутства! Молим Вас погледајте: http://docs.scribus.net за освежене документе
и www.scribus.net за преузимање.</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Link</source>
        <translation type="unfinished">Веза</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="unfinished">&amp;Тражи</translation>
    </message>
    <message>
        <source>S&amp;earch</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ctrl+F</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>F3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>unknown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Find</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Могуће спајање цртицом</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Прихвати</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Прескочи</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Поништи</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Hyphenator Settings</source>
        <translation>Поставке спајања цртицом</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Предлози за &amp;спајање цртицом</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Аутоматски спајај текст цртицама &amp;приликом куцања</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Језик:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;Најмања реч:</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>&amp;Дозвољена су узастопна спајања цртицом:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Појавиће се прозор који приказује сва могућа спајања цртицом за сваку реч када користите опцију „Екстра&gt;Спајај текст цртицом“.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Укључује аутоматско спајање текста цртицама у току куцања.</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Дужина најмање речи која се може спајати цртицом.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Максималан број спајања цртицом који могу ићи једно за другим.
Вредност 0 означава неограничено спајање цртицом.</translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Убаци страну</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation>&amp;Умећем</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Страна(е)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>пре странице</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>после странице</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>на крају</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation>Умећем</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation>&amp;Шаблон (Лева страна):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>Ша&amp;блон:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Нормално</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation>Шаблон (Десна страна):</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Убаци табелу</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation>Број редова:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation>Број колона:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Поништи</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Уређивање JavaScript-а</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Уреди...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Додај...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Обриши</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Затвори</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Нови скрипта:</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation>Нова скрипта</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation>Да ли стварно желите да обришете ову скрипту?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Не</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Да</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation>Управљање пречицама на тастатури</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Акција</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Текуће дугме</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Изаберите дугме за ову акцију</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Нема дугмета</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>&amp;Дугме дефинисано од стране корисника</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Постави &amp;дугме</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
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
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation>Та комбинација дугмади је већ у употреби</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation>Слојеви</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation>Додај нови слој</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Обриши слој</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation>Подигни слој</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation>Спусти слој</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation>Нови слој</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation>Да ли такође желите да обришете све објекте на овом слоју?</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Уређивање стилова линија</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Примени</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Нови</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Уреди</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Д&amp;упликуј</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Обриши</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Сними</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Копија од %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Нови стил</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation>Да ли стварно желите да обришете овај стил?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Не</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Да</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Отвори</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Документи (*.sla *.sla.gz *.scd *.scd.gz);;Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Документи (*.sla *.scd);;Сви фајлови (*)</translation>
    </message>
</context>
<context>
    <name>MSpinBox</name>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
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
    <name>Macro</name>
    <message>
        <source>Passed object is not callable</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MacroManager</name>
    <message>
        <source>Manage Macros</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Brings up a graphical window for creating, deleting, editing, saving and loading macros.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create, edit and delete macros</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Macro</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unable to open the requested file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus - Edit Macro</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;The macro name you requested is already taken  by another macro.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Macro creation failed. The macro manager was unable to set up the macro.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; has reported a minor error.
The error is: %2
A full traceback follows:

%3
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; failed to execute correctly.
The error is: %2
A full traceback follows:

%3
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter name for new macro: </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ManageMacrosDialog</name>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Renaming the macro failed because the name is already in use.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus - Manage Macros</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;This window is the Scribus Macro Manager. Here you can create macros, edit macros, etc. All changes are made using the buttons on the right hand side of the window.&lt;/p&gt;
&lt;p&gt;All changes made in this dialog take effect instantly - you cannot cancel the actions you make here.
The table in the center of the dialog lists what macros are currently loaded and some information about them. Use &quot;What&apos;s this&quot; on the table for more information.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;Нови</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro by prompting for the macro name then bringing up the edit macro dialog box.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <source>Macro</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Accel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Description</source>
        <translation type="unfinished">Опис</translation>
    </message>
    <message>
        <source>&lt;p&gt;This table lists the macros that are currently defined.&lt;/p&gt;

&lt;p&gt;&lt;b&gt;Name:&lt;/b&gt; The name of the macro, as shown in the menu bar and in other places around Scribus.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Edit:&lt;/b&gt; If the macro can be edited, &quot;Yes&quot; appears in this column. Usually if a macro cannot be edited it was created using the register_macro command in a script.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Accel:&lt;/b&gt; The menu shortcut key sequence, if any, associated with the macro. For example, CTRL-F8 means that you can press Control-F8 when in Scribus to run the macro.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Description:&lt;/b&gt; If the macro contains a &quot;docstring&quot;, a special string at the start of its definition that describes it, that is shown here. If the docstring is long, only the beginning is shown - use &quot;What&apos;s This&quot; on the macro&apos;s entry in the Macro menu to see the full description.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rena&amp;me</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rename the selected macro.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Rename the selected macro. You will be prompted for the new name.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation type="unfinished">&amp;Уреди...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Edit the source of the selected macro, if the source is availible.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Обриши</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Delete the currently selected macro.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete the selected macro. This is instant, and there is no way to recover the macro once deleted. If the macro is created by a start-up script, it will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Set Accel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Set the keyboard shortcut for the selected macro.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;Set the keyboard shortcut (accelerator) key of the selected macro. You will be prompted for the new shortcut in a dialog box.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>E&amp;xport</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+X</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export macros to a file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;Export macros to an external file. The file will be a Python script containing the scripter commands to re-create the macros. It can be run using &lt;tt&gt;Load extension script&lt;/tt&gt; from the &lt;tt&gt;Script&lt;/tt&gt; menu, or the import button in the macro manager.&lt;/p&gt;
&lt;p&gt;If you want a nice, human readable version of your macros, select the macro you want, press the &lt;tt&gt;Edit&lt;/tt&gt;  button, and use the &lt;tt&gt;Save source&lt;/tt&gt; button in the &lt;tt&gt;Edit Macro&lt;/tt&gt; dialog. You won&apos;t be able to load that version with &lt;tt&gt;Load extension script&lt;/tt&gt; - instead, create a new macro with the&lt;tt&gt; New&lt;/tt&gt; button and use &lt;tt&gt;Load source&lt;/tt&gt;.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete &amp;All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete all macros.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete all registered macros. This is instant, and there is no way to recover the deleted macros. Any macros created by your start-up script will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Увези</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import macros from a file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;Loads macros from an external file.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close this dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Return to Scribus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;Edit the selected macro. &lt;/p&gt;
&lt;p&gt;If this button is greyed out, either there is no selected macro or the macro manager does not have the source code for the macro you have selected (in which case &lt;tt&gt;No&lt;/tt&gt; will be shown in the &lt;tt&gt;Edit &lt;/tt&gt;column of the macro).&lt;/p&gt;
&lt;p&gt;If Scribus doesn&apos;t have the source, the macro was probably created by a script.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus Macro Files (*.pymacro)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open File Dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select the macro file to load.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save File Dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save all macros</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus - Rename Macro</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter new name: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus - Set Macro Shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter new shortcut: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Да</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Не</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Многоструки дупликат</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
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
        <translation>&amp;Број копија:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Водоравна промена:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Усправна промена:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Удаљеност</translation>
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
        <translation>Угао:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Дужина:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
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
        <translation type="obsolete">Грешка скрипте</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation type="obsolete">Ако радите са званичном скриптом молим Вас пријавите грешку на &lt;a href=\&quot;http://bugs.scribus.net\&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation type="obsolete">Ова порука је и у Вашем клипборду. Користите Ctrl+V да је пренесете у пратиоца грешака.</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">Прикажи &amp;конзолу</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">Сакриј &amp;конзолу</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Import Template</source>
        <translation>Увези шаблон</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Увези страницу(е)</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation>Из документа:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Измени...</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation>Увези страницу(е):</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Унесите листу обележја раздвојених зарезима где\n
обележје може бити * за све странице, 1-5 за\n
опсег страница или број једне странице.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> из 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Направи страницу(е)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>пре странице</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>после странице</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>на крају</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Увези</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Поништи</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Отвори</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Документи (*.sla *.sla.gz *.scd *.scd.gz);;Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Документи (*.sla *.scd);;Сви фајлови (*)</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> из %1</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Помери странице</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Копирај страницу</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Помери страницу(е):</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>у:</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>пре странице</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>после странице</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>на крају</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>Својства</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Облик</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Текст</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>Сл&amp;ика</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Линија</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Боје</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Име</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Геометрија</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X Поз:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y Поз:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Ш&amp;ирина:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Висина:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Ротација:</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Почетна тачка:</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Ниво</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Облик:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Уређивање облика...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>За&amp;обљени углови:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Растојање текста</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Колоне:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Процеп:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>Вр&amp;х:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Дно:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Лево:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Десно:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>Т&amp;абулатори...</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Својства путање текста</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Прикажи криву</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Покрени померај:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Растојање од криве:</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>Текст &amp;плута око оквира</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Користи кутија за &amp;везивање</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>&amp;Користи контуру</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Величина &amp;фонта:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Сенка:</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation>Произвољан проред</translation>
    </message>
    <message>
        <source>&amp;Kerning:</source>
        <translation type="obsolete">&amp;Кернинг:</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation>&amp;Проред:</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>Ст&amp;ил:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>Је&amp;зик:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Слободно скалирање</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>X Ска&amp;лирај:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>Y Ска&amp;лирај:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Скалирај &amp;до величине оквира</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>Пр&amp;опорционалан</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Улазни профил:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Намера рендеровања:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Перцептуално</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Релативна колорметрика</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Засићење</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Апсолутна колорметрика</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Лева тачка</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Крајње тачке</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Основна тачка:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>В&amp;рста линије:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>&amp;Ширина линије:</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Угаони спој</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Коси спој</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Кружни спој</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>Ив&amp;ице:</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Раван поклопац</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Квадратни поклопац</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Кружни поклопац</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Завршеци:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Нема стила</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Линије ћелије</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Линија на врху</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Линија лево</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Линија десно</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Линија на дну</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Име изабраног објекта</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Водоравни положај тренутне почетне тачке</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Усправни положај тренутне почетне тачке</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Ширина</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Висина</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Ротација објекта у тренутној почетној тачки</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Тачка од које се референцирају мере или углови ротације</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Изабери врх лево за почетну тачку</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Изабери врх десно за почетну тачку</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Изабери дно лево за почетну тачку</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Изабери дно десно за почетну тачку</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Изабери центар за почетну тачку</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Преврни водоравно</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Преврни усправно</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Помери за један ниво на горе</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Помери за један ниво на доле</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Помери ка напред</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Помери у позадину</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Указује на ком је нивоу објекат, 0 значи да је објекат на дну</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Закључај или откључај објекат</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Закључај или откључај величину објекта</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Укључи или искључи штампање објекта</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Натерај текст у нижим оквирима да плута око објекта</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation>Користи окружујућу кутију уместо облика оквира за плутање текста</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Користи другу линију оригинално постављену на облик оквира за плутање текста</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Фонт изабраног текста или објекта</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Величина фонта</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Скалирање ширине карактера</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Боја текста</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Боја попуњеног текста</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Засићење боје текста</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Засићење боје испуњеног текста</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="obsolete">Ручни кернинг</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Проред</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Стил тренутног пасуса</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Језик оквира за спајање цртицом</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Измени поставке за леве или крајње тачке</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Узорак линије</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Дебљина линије</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Врста спојева линија</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Врста краја линије</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Стил линија тренутног објекта</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Изаберите облик оквира...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Уређивање облика оквира...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Постави полупречник за заобљавање углова</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Број колона у тексталном оквиру</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Пребацује између ширине процепа и ширине колоне</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Растојање између колона</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Растојање текста од врха оквира</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Растојање текста од дна оквира</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Растојање текста од леве стране оквира</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Растојање текста од десне стране оквира</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Уређивање поставки језичака текстуалног оквира...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Дозволи да слика буде различите величине од оквира</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Водоравно одступање слике унутар оквира</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Усправно одступање слике унутар оквира</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Промени величину слике водоравно</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Промени величину слике усправно</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Задржи исто X и Y скалирање</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Задржи аспектни однос</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Натерај слику да стане у оквир</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Радије користи пропорције слике него оквира</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Изворни профил слике</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Намера рендеровања за слику</translation>
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
        <translation>Ширина колона</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ниједан</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Име „%1“ није јединствено.
Молим вас изаберите друго.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Писање с десна на лево</translation>
    </message>
    <message>
        <source>Trac&amp;king:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation>Уреди стил</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Раван поклопац</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Квадратни поклопац</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Кружни поклопац</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Угаони спој</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Коси спој</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Кружни спој</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Ширина линије:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source> pt </source>
        <translation> pt </translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Непрекидна линија</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Испрекидана линија</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Тачкаста линија</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Црта-тачка</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Црта-тачка-тачка</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Име „%1“ није јединствено.
Молим вас изаберите друго.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Edit Templates</source>
        <translation>Уређивање шаблона</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Примени</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Нови</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Д&amp;уплирај</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Обриши</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Затвори</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation>Да ли стварно желите да обришете овај шаблон?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Не</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Да</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Име:</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation>Нови шаблон</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Копија од %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Име:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Копија #%1 од</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Нормално</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
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
        <translation>Нови документ</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Величина папира</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Величина:</translation>
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
        <source>Custom</source>
        <translation>Произвољно</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Усм&amp;ерење:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Усправно</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Водоравно</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Ш&amp;ирина:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Висина:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>&amp;Упоредне странице</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Прво лева &amp;страница</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Водиље маргина</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Лево:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Десно:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Врх:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Дно:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Опције</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>П&amp;рви број странице:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>&amp;Подразумевана јединица:</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation>Тачке (pts)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Милиметри (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Инчи (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Пика (p)</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>&amp;Аутоматски текстуални оквир</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation>Водиље колоне</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Процеп:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Колоне:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Величина странице документа, или стандардна величина или произвољна величина</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Усмерење страница документа</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Ширина страница документа, подложна изменама ако изаберете произвољну величину странице</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Висина страница документа, подложна изменама ако изаберете произвољну величину странице</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Укључи једноструки распоред или раширени распоред</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Учини да прва страница буде лева</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Растојање између водиље горње маргине и обода странице</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Растојање између водиље доње маргине и обода странице</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Растојање између водиље леве маргине и обода странице.
Ако је изабрано „Упоредне странице“, овај размак маргина се може користити за постизање исправних маргина за повезивање</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Растојање између водиље десне маргине и обода странице.\n
Ако је изабрано „Упоредне странице“, овај размак маргина се може користити за постизање исправних маргина за повезивање</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>Први број стране документа</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Подразумевана јединица мере за уређивање документа</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Направи текстуалне оквире аутоматски при додавању нових страница</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Број колона у аутоматски направљеним текстуалним оквирима</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Растојање између аутоматски направљених колона</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Унутра:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>Спо&amp;ља:</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Folio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NewTm</name>
    <message>
        <source>Left Page</source>
        <translation>Лева страница</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Десна страница</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Нодови</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Апсолутне координате</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X Поз:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y Поз:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>Уреди &amp;контуру</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>&amp;Ресетуј контуру</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Крај уређивања</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Помери нодове</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Помери контролне тачке</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Додај нодове</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Обриши нодове</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Помери контролне тачке независно</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Помери контролне тачке симетрично</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Ресетуј контролне тачке</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Ресетуј ову контролну тачку</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Отвори полигон или исеци bezier криву</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Затвори ову bezier криву</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Окрени путању водоравно</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Окрени путању усправно</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Развуци путању водоравно на десно</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Развуци путању водоравно на лево</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Развуци путању усправно на горе</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Развуци путању усправно на доле</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Ротирај путању супротно од смера казаљке на часовнику</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Ротирај путању у смеру казаљке на часовнику</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation>Умањи величину путање за приказаних %</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Повећај величину путање за приказаних %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Угао ротације</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation>% to Enlarge or Reduce By</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Активирај режим уређивања контура</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Ресетуј контуре на оригинални облик оквира</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation>Када је штиклирано користи координате у односу на страну,
иначе, координате се односе на објекат.</translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>Update paragraph styles</source>
        <translation type="obsolete">Ажурирај стилове пасуса</translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation type="obsolete">Ако стил пасуса са истим именом као текући пасус OpenOffice.org-а већ постоји, да ли би требало изменити стил у Scribus-у да се поклапа са оним који се увози, или би требало да остане непромењен</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation type="unfinished">Користи име документа као префикс за стилове пасуса</translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation type="obsolete">Да ли би увозник требало да дода име документа 
испред имена стила параграфа у Scribus-у</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation type="unfinished">Не питај опет</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">У реду</translation>
    </message>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Update paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing stlyes in the current Scribus document</source>
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
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation>Конзола за скрипте</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Create PDF File</source>
        <translation type="obsolete">Направи PDF фајл</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>Из&amp;лаз у фајл:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Изме&amp;ни...</translation>
    </message>
    <message>
        <source>Export Range</source>
        <translation>Опсег извожења</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>&amp;Све странице</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>Изаб&amp;ерите странице</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Опције за фајлове</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>Компатибилнос&amp;т:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>&amp;Повезивање</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Лева маргина</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Десна маргина</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Направи &amp;сличице</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Сними &amp;повезане текстуалне оквире као PDF чланке</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>&amp;Обухвати маркере</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> тпи</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Резолуција:</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation>Поставке слике</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Аутоматски</translation>
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
        <translation>Ниједан</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation>&amp;Метод:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Квалитет:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Максимално</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Висок</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Средњи</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Низак</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Минимално</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation>&amp;Смањи резолуцију слике на:</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;Опште</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>&amp;Уметни све фонтове</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Уметање</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Доступни фонтови:</translation>
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
        <translation>Фонтови за уметање:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Фонтови</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Ротација:</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Страница</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>Прикажи пред&amp;приказе страница</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Ефекти</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>&amp;Прикажи трајање:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Ефека&amp;т трајање:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>Ефекат в&amp;рста:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>&amp;Премештам линије:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>&amp;Од:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>С&amp;мера:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> сек.</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Без ефеката</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Ролетне</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Кутија</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Растварање</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Сјај</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Раздвоји</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Обриши</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Водоравно</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Усправно</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Унутра</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Споља</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>С лева на десно</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Од врха до дна</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Од дна до врха</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>С десна на лево</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Врх-лево до дно-десно</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>&amp;Примени ефекат на све странице</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>&amp;Користи шифровање</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Лозинке</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Корисник:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>&amp;Власник:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Поставке</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>Дозволи &amp;штампање документа</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Дозволи &amp;измену документа</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Дозволи &amp;копирање текста и графике</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Дозволи додавање &amp;анотација и поља</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>&amp;Безбедност</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Опште</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Излаз &amp;намењен за:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Екран / веб</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Штампач</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Користи произвољне поставке рендеровања</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Поставке рендеровања</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>Уче&amp;сталост:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Угао:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>Та&amp;чка функција:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Једноставна тачка</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Линија</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Кружна</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Елипса</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Пуне боје:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Користи ICC профил</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Профил:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Намера рендеровања:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Перцептуално</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Релативна колорметрика</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Засићење</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Апсолутна колорметрика</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Слике:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Немој да користиш уметнуте ICC профиле</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>Б&amp;оја</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>PDF/X-3 на излазу</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Инфо знаковни низ:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Излазни &amp;профил:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Подсеци кутију</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
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
        <translation>&amp;Сними</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Извези све странице у PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Извези опсег страница у PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Унесите листу обележја раздвојених зарезима где
обележје може бити * за све странице, 1-5 за
опсег страница или број једне странице.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation>Утврђује PDF компатибилност. Подразумевана је Acrobat 4.0 који даје најширу компатибилност.
Изаберите Acrobat 5.0 ако Ваши фајлови поседују PDF 1.4 карактеристике као што су провидност или Вам је потребна 128. битно шифровање.
PDF/X-3 служи за извоз у PDF за комерцијалну штампу и могуће га је изабрати када активирате управљање бојама.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation>Утврђује повезивање страна у PDF-у. Отавите подразумевани
избор - Лево, осим ако знате да ово треба да промените.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation>Прави сличицу сваке стране у PDF-у.
Неки прегледачи могу користити сличице за навигацију.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Прави PDF чланке, што је корисно за навигацију по повезаним чланцима у PDF-у.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation>Умеће маркере које сте направили у Ваш документ.
Ово је корисно при навигацији кроз дугачке PDF документе.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation>Извози резолуцију текста и векторске графике.
Ово не утиче на резолуцију bitmap слика као што су фотографије.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation>Компресија текста и графике.
Оставите ово штиклирано, осим ако немате разлога за супротно. Ово смањује величину PDF-а.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation>Верзија компресије за слике.
„Аутоматска“ дозвољава Scribus-у да изабере најбољи метод.
ZIP је добар за слике са пуним бојама.
JPEG је бољи за прављење PDF фајлова који садрже много фотографија (са могућим малим губитком квалитета).
Оставите поставку на „Аутоматска“, уколико немате потребу за специјалним опцијама компресије.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation>Нивои компресије: Минималан (25\\%), Низак (50\\%), Средњи (75\\%), Висок (85\\%), Максималан (95\\%)</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation>Смањите резолуцију Ваших битмап слика на изабрани резолуцију (тпи).
Ако ово оставите нештиклирано рендероваће се у матичној резолуцији.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>ТПИ (Тачака По Инчу) за извоз слика.</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation>Уметни фонтове у PDF. Уметање фонтова
ће сачувати распоред и изглед Вашег документа.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation>Укључује ефекте презентације када се користи Acrobat Reader у режиму преко целог екрана.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Прикажи преглед стране за сваку од горе излистаних страница.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>Временски интервал за који се приказује страница пре него што почне презентација на изабраној страници.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Дужина трајања ефекта.
Краће време ће убрзати ефекат, а дуже ће га успорити.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Врста ефекта приказа.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Смер ефекта крећућих линија за ефекте „подели“ и „ролетна“.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Почетна позиција за ефекте „подели“ и „кутија“.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Смер ефеката „сјај“ и „брисање“.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Примени изабрани ефекат на све странице.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Укључи сигурносна својства у PDF-у.
Ако сте изабрали Acrobat 4.0, PDF ће бити заштићен 40 битном 
енкрипцијом.
Ако сте изабрали Acrobat 5.0, PDF ће бити заштићен 128 битном 
енкрипцијом.
Обавештење: PDF енкрипција није тако поуздана као GPG или PGP енкрипција и има неких ограничења.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation>Изаберите главну лозинку која укључује или искључује сва
сигурносна својства и Вашем извезеном PDF-у</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Изаберите лозинку помоћу које ће корисници моћи да читају Ваш PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Дозволи штампање PDF-а. Ако није штиклирано, штампање није омогућено. </translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Дозволи измену PDF-а. Ако није штиклирано, измена PDF-а није омогућена.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation>Дозволи копирање текста и графике из PDF-а. 
Ако није штиклирано, текст и графика се не могу копирати.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="obsolete">Дозволи додавање анотација и поља у PDF. 
Ако није штиклирано, уређивање анотација и поља је онемогућено.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation>Модел боја за излазни PDF.
Изаберите Екран/Веб за PDF-ове који се користе за приказ на екрану и штампање на обичним штампачима.
Изабери штампач када се штампа на штампачу са CMYK четворобојном штампом.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>јОво је напредно поешавање које није укључено као подразумевано. Ово би требало да буде укључено само
када се то експлицитно захтева од стране Вашег штампача и имате прецизне податке који су Вам потребни.
Иначе, Ваш извезени PDF може да има грешке при штампању и да не буде преносив између различитих система.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Уметни профил боја за пуне боје</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Профил боја за пуне боје</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Намера рендеровања за пуне боје</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Уметни профил боја за слик</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Немој да користиш профиле боја који су уметнути у изворне слике</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Профил боја за слике</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Намера рендеровања за слике</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Излазни профил за штампу. Ако је могуће, добавите нека упутства од Вашег штампача везана за избор профила.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Обавезан знаковни низ за PDF/X-3 или неће успети прилагођавање
PDF-а PDF/X-3-у. Препоручујемо да користите наслов документа.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Растојање за цурење од врха физичке странице</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Растојање за цурење од дна физичке странице</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Растојање за цурење с лева физичке странице</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Растојање за цурење с десна физичке странице</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Сними као</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF фајлови (*.pdf);;Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation>Компресуј текст и &amp;векторску графику</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation>Ук&amp;ључи ефекте за презентацију</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation>&amp;Презентација</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation>Сви фонтови у &amp;подсет</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation>Фонтови за подсет:</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Окрени страницу(е) водоравно</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Окрени страницу(е) усправно</translation>
    </message>
    <message>
        <source>Save as PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management must be enabled to use PDF/X-3. You can enable color management from the Settings menu.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF/X-3 is supported and enabled, but can not be used for the selected PDF version. If you want to use PDF/X-3, you need to set PDF/X-3 as your PDF version (compatibility level).&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF/X-3 is not supported by this Scribus build (CMS support not present).&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fields is prevented.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF security settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF/X-3 settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF security can not be used with PDF/X-3. If you want to turn on security, change your PDF version (compatibility level) to something other than PDF/X-3.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Преглед пре штампања</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>Омекшај &amp;текст</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Омекшај &amp;графику</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Прикажи провидн&amp;ост</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>&amp;Под уклањањем боја</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;Прикажи CMYK</translation>
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
        <translation>Обезбеђује пријатнији приказ текстуалних ставки у прегледачу, на рачун
незнатног успорења при прегледању. Ово утиче само на Type 1 фонтове</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of True Type Fonts, Open Type Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Обезбеђује пријатнији приказ True Type фонтова, Open Type фонтова, EPS, PDF 
и 
векторске графике у прегледу, на рачун незнатног успорења при прегледању</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Приказује провидност и провидне ставке у Вашем документу. Захтева Ghostscript 7.07 или новији</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Даје преглед за штампу користећи симулацију генеричких CMYK мастила, уместо RGB боја</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Укључи/искључи C (Цијан) палету мастила</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Укључи/искључи М (Магента) палету мастила</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Укључи/искључи Y (Жута) палету мастила</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Укључи/искључи К (Црна) палету мастила</translation>
    </message>
    <message>
        <source>A way of switching some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Начин за замену неких од сивих нијанси које се састоје од
цијан, жуте и магенте и коришћење црне уместо тога.
UCR највише утиче на делове слике који су неутрални и/или тамни тонови
блиски сивој. Коришћење овога може поправити штампу неких слика,
а потребно је мало експериментисања и тестирања у зависности од случаја.
UCR смањује могућност презасићења CMY мастилима.</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Све</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Page</name>
    <message>
        <source>Copy Here</source>
        <translation>Копирај овде</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Помери овде</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Поништи</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Пренеси</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Прикажи &amp;маргине</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Прикажи &amp;оквире</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Прикажи &amp;слике</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Прикажи &amp;мрежу</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Прикажи &amp;водиље</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Прикажи мрежу &amp;основне линије</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Пре&amp;баци на мрежу</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Преб&amp;аци на водиље</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Слика</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Фајл: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Оригинални PPI: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Стварни PPI: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Повезан текст</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Текстуални оквир</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Текст на путањи</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Пасуси: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Речи: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Карактери: </translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Штампај: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Укључено</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Искључено</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>Ин&amp;фо</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation>&amp;Добави слику...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>С&amp;лика видљива</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation>&amp;Ажурирај слику</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation>&amp;Уређивање слике</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation>&amp;Подеси оквир према слици</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>&amp;Добави текст...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation>&amp;Примени текст...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation>&amp;Уређивање текста...</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Је PDF &amp;маркер</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Је PDF ан&amp;отација</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>С&amp;војства анотације</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>С&amp;војства поља</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF опције</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Уреди текст...</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>&amp;Закључај</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>От&amp;кључај</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation>Закључај &amp;величину објекта</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation>Откључај &amp;величину објекта</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Пошаљи у св&amp;еску</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Пошаљи у сл&amp;ој</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation>&amp;Уметни пример текста</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Група</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation>Од&amp;групиши</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>Ни&amp;во</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>&amp;Пошаљи иза свега</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation>&amp;Доведи испред свега</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Спусти</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;Подигни</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation>Оквир &amp;слике</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation>Пол&amp;игон</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation>&amp;Оквирне линије</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Текстуални оквир</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>&amp;Bezier крива</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>Претв&amp;ори у</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Исеци</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Копирај</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Обриши</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>О&amp;чисти садржај</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation>Прикажи с&amp;војства...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation>Сакриј св&amp;ојства...</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ниједан</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Да ли стварно желите да очистите сав текст?</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation>Програм</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation>недостаје!</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">Копија</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Слика</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Линија</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Полигон</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Полилинија</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Текста путање</translation>
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
        <translation> од %1</translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Result</source>
        <translation>Резултат</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Тражи резултате за: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Преглед</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Изабери</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Поништи</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Pictures</source>
        <translation>Слике</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Име</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Путања</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Страница</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Штампај</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Статус</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Иди на</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Недостаје</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Тражи</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Својства полигона</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Угл&amp;ови:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Ротација:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Примени &amp;фактор</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>&amp;Фактор:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Број углова за полигоне</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Степени ротације за полигоне</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Примени конвексно/конкавно фактор да би променио облик полигона</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Пример полигона</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation>Негативна вредност ће полигон учинити конкавним 
(или у облику звезде),
 позитивна вредност ће га учинити конвексним</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Подешавања</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Опште</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Документ</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Водиље</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Типографија</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Алати</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Албум</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Приказ</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Спољашњи алати</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation>Разно</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>Кориснички интерфејс</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Тема:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Величина &amp;фонта:</translation>
    </message>
    <message>
        <source>Units</source>
        <translation>Јединице</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation>тачке (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Милиметри (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Инчи (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Пика (p)</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation>Поставке миша</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Скок помоћу точкића:</translation>
    </message>
    <message>
        <source> px</source>
        <translation> px</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation>&amp;Зграби полупречник:</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Менији</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>&amp;Скорашњи документи:</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Путање</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Документи:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Измени...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC профили:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>И&amp;змени...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>С&amp;крипте:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>Изм&amp;ени...</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation>Ш&amp;аблони:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Изме&amp;ни...</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Величина папира</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Произвољно</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Величина:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Усправно</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Водоравно</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Усм&amp;ерење:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Ш&amp;ирина:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Висина:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>&amp;Упоредне странице</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Прво лева &amp;страница</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Водиље маргина</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Дно:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Врх:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Десно:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Лево:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Аутоматско снимање</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation>Укључ&amp;ено</translation>
    </message>
    <message>
        <source>min</source>
        <translation>мин</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Период:</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation>Распоред мреже</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation>С&amp;итни проред мреже:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation>Озб&amp;иљни проред мреже:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation>Растојање &amp;пресецања водиља:</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation>Боје мреже</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation>Си&amp;тне боје мреже:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation>Озб&amp;иљне боје мреже:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation>Боје &amp;корисничких водиља:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation>Боја осн&amp;овне мреже:</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation>Постављање</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation>У &amp;позадини</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation>&amp;Спреда</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation>Основна мрежа</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation>&amp;Укључи</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation>И&amp;скључи се</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Индекс</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Погрешно постављање:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;Скалирање:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Изложилац</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>По&amp;грешно постављање:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>С&amp;калирање:</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Мала верзална слова</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Ска&amp;лирање:</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Остало</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>Основна &amp;мрежа:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>Основни &amp;померај:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation>Аутоматски проред &amp;линија:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation>Подразумевани &amp;фонт:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation>Подразумевана &amp;величина:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation>Боја &amp;текста:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Колоне:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Процеп:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Плетене свилене пиџаме замењене за плави кварц</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ниједан</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>Боја &amp;линије:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Сенчење:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>Боја &amp;испуњавања:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>Се&amp;нчење:</translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation>&amp;Врста линије:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>&amp;Ширина линије:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>Мин&amp;имално:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>Мак&amp;симално:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>&amp;Кораци:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Слободно скалирање</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>&amp;Водоравно скалирање:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>&amp;Усправно скалирање:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>&amp;Скалирај слику на величину оквира</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Задржи аспектну &amp;сразмеру</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Боја за поп&amp;уњавање:</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Угл&amp;ови:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Ротација:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Примени &amp;фактор</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>&amp;Фактор:</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Остале опције</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>Сн&amp;ими садржај приликом измена</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Преглед</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>Мали</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Средњи</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>Велики</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation>Прикажи странице &amp;једну до друге</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation>Боје странице</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation>&amp;Позадина:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation>&amp;Маргине:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Прикажи област која се не може одштампати у боји маргине</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation>Користи опције &amp;провидности PDF 1.4</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>&amp;Подеси величину приказа</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation>Да бисте подесили приказ вуците лењир испод помоћу клизача.</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation>Postscript интерпретатор</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Име извршног фајла:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Омекшај &amp;текст</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Омекшај &amp;графику</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Алат за обраду слика</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Имена &amp;извршних фајлова:</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Штампање</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation>Исеци у &amp;маргину странице</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Примени &amp;под уклањање боја</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation>Изаберите подразумевано украшавање прозора и изглед.
Scribus наслеђује било које доступне KDE или Qt теме</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Подразумевана величина за меније и прозоре</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Подразумевана јединица мере за уређивање документа</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Број линија које ће се скроловати при сваком померању точкића на мишу</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Радијус области где ће Вам scribus дозволити да хватате објекте</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Број скорије измењених докумената који се приказују у менију „Фајл“</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Подразумевани директоријум за документе</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation type="obsolete">Подразумевани директоријум за ICC профиле</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Подразумевани директоријум за scripter скрипте</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation>Додатни директоријум за шаблоне докумената</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Подразумевана величина странице, или стандардна или произвољна величина</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Подразумевана оријентација страница документа</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Ширина страница документа, измењива ако сте изабрали произвољну величину странице</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Висина страница документа, измењива ако сте изабрали произвољну величину странице</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Укључи једноструки распоред или раширени распоред</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation>Учини да прва страица документа буде лева</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Растојање између водиље горње маргине и обода странице</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Растојање између водиље доње маргине и обода странице</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Растојање између водиље леве маргине и обода странице.
Ако је изабрано „Упоредне странице“, овај размак маргина се може користити за постизање исправних маргина за повезивање</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Растојање између водиље десне маргине и обода странице.
Ако је изабрано „Упоредне странице“, овај размак маргина се може користити за постизање исправних маргина за повезивање</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation>Када је укључено, Scribus снима резервну копију Вашег документа са наставком .bak
сваки пут када истекне временски период</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Временски период између аутоматског снимања</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Растојање између мањих линија мреже</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Растојање између озбиљних линија мреже</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Растојање у оквиру кога ће се објекат пребацити на постављене водиље</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Боја мањих линија мреже</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Боја озбиљнијих линија мреже</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Боја водиља које Ви умећете</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation>Постави мрежу иза Ваших објеката на страници</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation>Постави мрежу испред Ваших објеката на страници</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation type="obsolete">Укључује основну мрежу</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation type="obsolete">Искључује основну мрежу</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Погрешно постављање изнад основне линије фонта на линији</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Релативна величина superscript-а у поређењу са нормалним фонтом</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Погрешно постављање испод основне линије фонта на линији</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Релативна величина subscript-а у поређењу са нормалним фонтом</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Релативна величина фонта малих слова у поређењу са нормалним фонтом</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Процентуално повећање величине фонта за проред линија</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Својства текстуалног оквира</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Својства оквира за слике</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Својства исцртавања облика</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Подразумеване вредности нивоа увеличавања</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Својства исцртавања линија</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Својства исцртавања полигона</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Фонт за нове текстуалне оквире</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Величина фонта за нове текстуалне оквире</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Боја фонта</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Број колона у текстуалном оквиру</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Размак између колона текстуалног оквира</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Пример Вашег фонта</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Оквири за слике дозвољавају скалирање слика на било коју величину</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Водоравно скалирање слика</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Усправно скалирање слика</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Задржи подједнако водоравно и усправно скалирање</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Слике у оквирима за слике се скалирају према величини оквира</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Аутоматски скалиране слике задржавају своје оригиналне пропорције</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Боја за испуњавање оквира за слике</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Засићење боје за испуњавање</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Боја линије за облике</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Засићење боје за линије</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Боја испуњавања за облике</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Стил линије за облике</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Ширина линије за облике</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Минимално увећање дозвољено</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Максимално увећање дозвољено</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Измена увеличања за сваку операцију увеличавања/умањивања</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Боја линија</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Засићење боје</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Стил линија</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Ширина линија</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Број углова за полигоне</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Степени ротације за полигоне</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Примени конвексно/конкавно фактор да би променио облик полигона</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Пример полигона</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation>Негативна вредност ће полигон учинити конкавним 
(или у облику звезде),
 позитивна вредност ће га учинити конвексним</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Изаберите величину прегледа у албумској палети</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Сними садржај албума после сваке измене</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation>Када се користе наспрамне странице, прикажи две странице једну поред друге</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Боја папира</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Боја линија маргина</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Маскирај област изван линија маргина у боју тих линија</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation>Укључи својства провидности у оквиру PDF 1.4 извоза</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Постави подразумевани ниво увеличања/умањења</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation type="obsolete">Локација фајл система за Ghostscript интепретатор</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Омекшај текст за EPS и PDF рендеровање на екрану</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Омекшај графику за EPS и PDF рендеровање на екрану</translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation type="obsolete">Локација фајл система за измењивач графике</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Немој да приказујеш објекте изван маргина на одштампаној страници или извезеном фајлу</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Начин за искључивање неких сивих сенки које су састављене
од цијан, жуте, и магенте и коришћења црне уместо тога.
UCR највише утиче на делове слике који имају неутралне и/или тамне тонове
који су блиски сивој. Коришћење овога може поправити штампу неких слика
и потребно је мало експериментисања у зависности од случаја.
UCR смањује могућност презасићености CMY мастилима.</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Изаберите директоријум</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Унутра:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>Спо&amp;ља:</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Folio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="unfinished">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="unfinished">Letter</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="unfinished">Таблоид</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot
be changed with documents open.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns on the base grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns off the base grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File system location for the Ghostscript interpreter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend &apos;gimp-remote&apos;,
as it allows you to edit the image in an already running
instance of gimp.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>Нијансира&amp;ј:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Зас:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>&amp;Вре:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Црвена:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Зелена:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>Пл&amp;ава:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>А&amp;лфа канал:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>&amp;Основне боје</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Произвољне боје</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Дефинишите произвољне боје &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Поништи</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Додај у биране боје</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Изаберите боју</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Копирај или помери фајл</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Прочитај: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Упиши: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>&amp;Име фајла:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>&amp;Врста фајла:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Један директоријум горе</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Поништи</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Име</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Величина</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Врста</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Датум</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Атрибути</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Т&amp;ражи у:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Назад</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Направи нови директоријум</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Приказ листе</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Приказ детаља</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Преглед информација о фајлу</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Преглед садржаја фајла</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Читање-упис</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Само за читање</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Само за упис</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Није приступачан</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Симболичка веза до фајла</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Симболичка веза до директоријума</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Симболичка веза до специјалног</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Фајл</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Дир.</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Специјално</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Отвори</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Сними као</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Отвори</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Сними</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Преименуј</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Обриши</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>У&amp;читај поново</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Поређај по &amp;имену</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Поређај по &amp;величини</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Поређај по &amp;датуму</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Није поређано</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Поређај</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Приказуј &amp;скривене фајлове</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>тај фајл</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>тај директоријум</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>та симболичка веза</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Обриши %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Да ли сте сигурни да желите да обришете %1 „%2“?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Да</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Не</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Нови директоријум 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Нови директоријум</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Нови директоријум %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Нађи директоријум</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Директоријуми</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Сними</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Грешка</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Фајл није пронађен.
Проверите путању и име фајла.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Сви фајлови (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Изаберите директоријум</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Директоријум:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Фонт</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>Ст&amp;ил фонта</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Величина</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Ефекти</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>&amp;Прецртано</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Подвуци</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Боја</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Пример</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>Скр&amp;ипт</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Примени</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Поништи</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Затвори</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Изаберите фонт</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Очисти</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Изабери све</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Опозови</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Поно&amp;ви</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Исеци</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Копирај</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Пренеси</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Поравнај</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Подеси...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;О Qt-у&lt;/h3&gt;
&lt;p&gt;Овај програм користи Qt верзију %1.&lt;/p&gt;
&lt;p&gt;Qt је  вишеплатформски C++ GUI скуп алата од 
фирме Trolltech.&lt;/p&gt;
&lt;p&gt;Qt пружа портабилност на нивоу изворног 
кода кроз MS Windows, Mac OS X, Linux, и све главне 
комерцијалне Unix варијанте.
&lt;br&gt;Qt такође, постоји и за преносне и 
уградиве уређаје.&lt;/p&gt;
&lt;p&gt;Погледајте &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; за више 
информација.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Oook! Wrong arguments! Call: </source>
        <translation type="obsolete">Упс! Погрешни аргументи! Позови: </translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Увозим текст</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Сви подржани формати</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>Фонт %1 није исправан, одбацујем га</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Иницијализује се...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Документ</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Позадина</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Да ли стварно желите да пребришете фајл: 
%1 ?</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation>Преглед &amp;фонтова</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation>&amp;Уметни специјално</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Нови &amp;из шаблона...</translation>
    </message>
    <message>
        <source>Template: </source>
        <translation>Шаблон:  </translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Кружно писмо</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Брошуре</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Каталози</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Флајери</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Знаци</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Картице</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Заглавља</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Коверте</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Подсетнице</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Календари</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Огласи</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Ознаке</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Менији</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Програми</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>PDF формулари</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF презентације</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Часописи</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Постери</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Објаве</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Текстуални документи</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Фасцикле</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Властити шаблони</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Сними као &amp;слику...</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Сними као слику</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation>Грешка при писању излазног фајла(ова).</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Грешка при писању излазног фајла(ова).</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Извоз успешан.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Фајл постоји. Да пребришем?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>већ постоји Да пребришем?</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Не</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Да све</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation>Преглед пре штампања</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>Прегл&amp;ед за штампање</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Увези &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Сви подржани формати (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Отвори</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Сними као &amp;шаблон...</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to load image into an object doesn&apos;t exist or isn&apos;t selected!</source>
        <translation type="obsolete">Упс! Покушавате да учитате слику у објекат који не постоји или није изабран.</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to (un)lock an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Упс! Покушавате да (откључате)закључате објекат који не постоји! Такође, ниједан није изабран.</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to query an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Упс! Покушавате да позовете објекат који не постоји! Такође, ниједан није изабран.</translation>
    </message>
    <message>
        <source>Oook! You&apos;re calling an object doesn&apos;t exist!</source>
        <translation type="obsolete">Упс! Позивате објекат који не постоји!</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to erase an object doesn&apos;t exist!</source>
        <translation type="obsolete">Упс! Покушавате да обришете објекат који не постоји!</translation>
    </message>
    <message>
        <source>Oook! An object you&apos;re trying to textflow doesn&apos;t exist!</source>
        <translation type="obsolete">Упс! Објекат који покушавате да придружите тексту не постоји!</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation>S&amp;cripter приручник...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation type="obsolete">&amp;Scribus скрипте</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="obsolete">&amp;Изврши скрипту...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="obsolete">&amp;Скорашње скрипте</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">Прикажи &amp;конзолу</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation type="obsolete">С&amp;крипта</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Веб референца</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Python скрипте (*.py);; Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Сними страницу као &amp;SVG...</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Сними као</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG-слике (*.svg *.svgz);;Сви фајлови (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG-слике (*.svg);;Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Увези &amp;SVG...</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Фајлови са вредностима одвојеним запетом</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>CSV_подаци</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>CSV_заглавље</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>HTML фајлови</translation>
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
Спољашње везе</translation>
    </message>
    <message>
        <source>OO.o Writer Documents</source>
        <translation type="obsolete">OO.o Writer документи</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Текст филтери</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Текстуални фајлови</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation>Медисјки случајеви</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Basque</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation type="unfinished">Каталански</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Czech</source>
        <translation type="unfinished">Чешки</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation type="unfinished">Дански</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation type="unfinished">Холандски</translation>
    </message>
    <message>
        <source>English</source>
        <translation type="unfinished">Енглески</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>German</source>
        <translation type="unfinished">Немачки</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation type="unfinished">Фински</translation>
    </message>
    <message>
        <source>French</source>
        <translation type="unfinished">Француски</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="unfinished">Грчки</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation type="unfinished">Мађарски</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Italian</source>
        <translation type="unfinished">Италијански</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="unfinished">Литвански</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Polish</source>
        <translation type="unfinished">Пољски</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation type="unfinished">Руски</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="unfinished">Шведски</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation type="unfinished">Шпански</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation type="unfinished">Словачки</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="unfinished">Словеначки</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tried to set progress &gt; maximum progress</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <source>Failed to open document</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to save document</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
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
        <source>An object with the requested name already exists</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Style not found</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set style on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to save EPS</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page number out of range</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found - python error</source>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short &amp;Words...</source>
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
        <source>Afrikaans</source>
        <translation type="unfinished">Африканерски</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <source>OpenDocument Text Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Очисти</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Изабери све</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Опозови</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Поно&amp;ви</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Исеци</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Копирај</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Пренеси</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Системски мени</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Намотај</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Одмотај</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Нормализуј</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Минимизуј</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Максимизуј</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Затвори</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Обнови</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Помери</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Величина</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Ми&amp;нимизуј</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ма&amp;ксимизуј</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Затвори</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Ос&amp;тани на врху</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Минимизуј</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Врати доле</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Затвори</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>Намот&amp;ај</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>&amp;Одмотај</translation>
    </message>
</context>
<context>
    <name>Query</name>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
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
        <translation>Поставке документа</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Водиље маргина</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Врх:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Лево:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Дно:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Десно:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>&amp;Упоредне странице</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Прво лева &amp;страница</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Укључи једноструки распоред или раширени распоред</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Учини да прва страница буде лева</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Растојање између водиље горње маргине и обода странице</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Растојање између водиље доње маргине и обода странице</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Растојање између водиље леве маргине и обода странице.
Ако је изабрано „Упоредне странице“, овај размак маргина се може користити за постизање исправних маргина за повезивање</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Растојање између водиље десне маргине и обода странице.\n
Ако је изабрано „Упоредне странице“, овај размак маргина се може користити за постизање исправних маргина за повезивање</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Унутра:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation>&amp;Споља:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Величина папира</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Величина:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Произвољно</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation>Оријентација:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Усправно</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Водоравно</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Ширина:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation>Висина:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="unfinished">П&amp;рви број странице:</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style Settings</source>
        <translation>Поставке стила</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Стил тренутног пасуса</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Fill Color Settings</source>
        <translation>Поставке боје за испуњавање</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ниједан</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Боја попуњеног текста</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Засићење боје испуњеног текста</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Поставке боје за потезе</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ниједан</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Боја текста</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Засићење боје текста</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source>Font Settings</source>
        <translation>Поставке фонта</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Font of selected text</source>
        <translation>Фонт изабраног текста</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Величина фонта</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Скалирање ширине карактера</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Поставке карактера</translation>
    </message>
    <message>
        <source>Kerning:</source>
        <translation type="obsolete">Кернинг:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="obsolete">Ручни кернинг</translation>
    </message>
    <message>
        <source>Tracking:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Копија #%1 од</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Позадина</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>Normal</source>
        <translation>Нормално</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Фајл</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation>Направи нови документ</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation>Отвори документ</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation>Сними текући документ</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation>Затвори текући документ</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation>Штампај текући документ</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation>Сними текући документ као PDF</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Тражим фонтове</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation type="obsolete">Нема postscript фонтова на Вашем систему</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation>Излазим сада</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Фатална грешка</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation>Паметно спајање цртицом</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation>Поравнај лево</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation>Поравнај десно</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation>Поравнај по средини</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Уметни број странице</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation>Прикачи текст уз путању</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation>Прикажи слојеве</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation>Javascript-ови...</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation>Опозови</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation>Прикажи палету странице</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation>Закључај/Откључај</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation>Непреломиви размак</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Читам подешавања</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation>Добави ICC профиле</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation>Покрени спајача</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Постављам пречице</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation>&amp;Управљање бојама...</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Читам албум</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Покрећем прикључке</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Нови</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Отвори...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Отво&amp;ри скорашњи</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Нови</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation>Отвори...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Затвори</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Затвори</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Сними</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Сними</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Сними &amp;као...</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation>Сними као...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>Вр&amp;ати на снимљено</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Сакупи за и&amp;злаз...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation>&amp;Добави текст/слику...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Примени &amp;текст...</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation>Увези &amp;страницу(е)...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Увези</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Сними &amp;текст...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Сними страницу као &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Сними као P&amp;DF...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Извези</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation>&amp;Информације о документу...</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation>Информације о документу...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>&amp;Поставке документа...</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation>Поставке документа...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>Ш&amp;тампај...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Штампај...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Заврши</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Заврши</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Опозови</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Исеци</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Копирај</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Пренеси</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Оч&amp;исти</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Изабери &amp;све</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Исеци</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Копирај</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Пренеси</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Очисти</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation>Изабери све</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Тражи/Замени...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>Б&amp;оје...</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation>Боје...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>Стилови &amp;пасуса....</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>Стилови &amp;линија...</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation>Стилови...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation>&amp;Шаблони...</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation>Шаблони...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation>&amp;Javascript-ови...</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation>Изаберите нови фонт</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Д&amp;упликуј</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation>Дуплирај</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>&amp;Умножено дуплирање</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Многоструки дупликат</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Обриши</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Обриши</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Група</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Група</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>&amp;Растури групу</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation>Од-групиши</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>&amp;Закључај</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>&amp;Пошаљи иза свега</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation>Пошаљи иза свега</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation>&amp;Доведи испред свега</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation>Доведи испред свега</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Спусти</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation>Спусти</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;Подигни</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation>Подигни</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation>Распореди/&amp;Поравнај...</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation>Распореди/Поравнај...</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation>&amp;Уређивање облика</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Облик</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>&amp;Прикачи текст уз путању</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>&amp;Откачи текст од путање</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>&amp;Комбинуј полигоне</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Раздвоји &amp;полигоне</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation>Прет&amp;вори у контуре</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>Уметн&amp;и...</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation>Уметни...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Обриши...</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation>Обриши...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Помери...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation>Помери...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation>&amp;Примени шаблон...</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation>Примени шаблон...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>Управљање &amp;водиљама...</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation>Управљање водиљама...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation>&amp;Смести у прозор</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation>Смести у прозор</translation>
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
        <translation>&amp;Сличице</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation>Сличице</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Прикажи &amp;маргине</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation>Сакриј маргине</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Прикажи &amp;оквире</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation>Сакриј оквире</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Прикажи &amp;слике</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation>Сакриј слике</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Прикажи &amp;мрежу</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation>Прикажи мрежу</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Прикажи &amp;водиље</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Прикажи мрежу &amp;основне линије</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Пре&amp;баци на мрежу</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation>Уклапај уз мрежу</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Преб&amp;аци на водиље</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Својства</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Својства</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation>&amp;Контура</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Контура</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>&amp;Албум</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Албум</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>&amp;Слојеви</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation>Палета стр&amp;анице</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Маркери</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>&amp;Управљање сликама</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Управљање сликама</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;Споји текст цртицама</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation>Споји текст цртицама</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>Облач&amp;ићи</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Алати</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>P&amp;DF алати</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Алати</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation>Облачићи</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>П&amp;одешавања...</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation>&amp;Фонтови...</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation>Фонтови...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation>&amp;Спајач...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation>&amp;Пречице на тастатури...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>&amp;О Scribus-у</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation>О Scribus-у</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>О &amp;Qt-у</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>О Qt-у</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>Scribus &amp;Приручник...</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation>Веб помоћ...</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Фајл</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Уреди</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>Ст&amp;ил</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Ставка</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Страна</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Прикажи</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>Пр&amp;оширења</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Подешавања</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>&amp;Прозори</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>По&amp;моћ</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Лево</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Средина</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>&amp;Десно</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Блокирај</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Приморано</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Остало...</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Подвучено</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Право напред</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Мала верзална слова</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Изложилац</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Индекс</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Уоквирено</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X Поз:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y Поз:</translation>
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
        <translation>Спреман</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Наслажи</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>Поп&amp;лочај</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ниједан</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation>Добави Текст/Слику...</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation>Добави слику...</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Боја</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation>&amp;Изврни</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>&amp;Добави текст...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Фонт</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Величина</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Ефекти</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Поравнање</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Намотај</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Табулатори...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Добави текст...</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Фонт</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Величина</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Стил</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Боја</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Намотај</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>От&amp;кључај</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Отвори</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Документи (*.sla *.sla.gz *.scd *.scd.gz);;Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Документи (*.sla *.scd);;Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Увозим странице...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Увези страницу(е)</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation>&lt;p&gt;Покушавате да увезете више страница него што је доступно у 
текућем документу ако се броји од активне странице.&lt;/p&gt;Изаберите нешто од следећег:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Направи&lt;/b&gt; стране које 
недостају&lt;/li&gt;&lt;li&gt;&lt;b&gt;Увези&lt;/b&gt; 
странице до последње&lt;/li&gt;&lt;li&gt;&lt;b&gt;Поништи&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Направи</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Увези</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Увези готово</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Нисам пронашао ништа да увезем</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation>Фајл %1 није у scribus-овом формату</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Учитавам...</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Сви подржани формати</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation>t Фајл 1</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Сними као</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Документи (*.sla *.sla.gz *.scd *scd.gz);;Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Снимам...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Штампам...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Документ</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Штампање није успело!</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Scribus приручник</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Текстуални фајлови (*.txt);;Сви фајлови(*)</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Величина:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Сенка:</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation>Сакриј основну мрежу</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Прикажи основну мрежу</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation>Недостају следећи програми:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation>Ghostscript : Не можете да користите EPS слике</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Све</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation>EPS-фајлови (*.eps);;Сви фајлови (*)</translation>
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
        <translation>Неки објекти су закључани.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Поништи</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation>Закључај све</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation>Откључај све</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Откључај</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Закључај</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation>Учитавање:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Прилагођавам боје</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation>&amp;Опозови брисање објекта</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation>&amp;Опозови померање објекта</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation>&amp;Опозови измену објекта</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Немачки</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Пољски</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Енглески</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Шпански</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Италијански</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Француски</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Руски</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Дански</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Словачки</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Мађарски</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Чешки</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Холандски</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Португалски</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation>Украјински</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Грчки</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Каталански</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Фински</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation>Ирски</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Литвански</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Шведски</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Словеначки</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Африканерски</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Изаберите директоријум</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Инвертуј</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Пуцање scribus-а</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus пуца због сигнала #%1</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>There are no suitable Fonts on your System</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management is supported but can not currently be enabled. Make sure you have ICC color profiles installed and that the profile path in the preferences points to where they&apos;re installed.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management is not supported by this Scribus build (not compiled in).&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Croatian</source>
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
        <translation>Слој</translation>
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
        <translation>Све</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="unfinished">Копија</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation>&amp;Напусти свакако</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation>За&amp;твори свакако</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Документ:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>је измењен од последњег снимања.</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation>&amp;Сними сада</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation type="unfinished">&amp;Scribus скрипте</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="unfinished">&amp;Изврши скрипту...</translation>
    </message>
    <message>
        <source>Run a Python script from a file.</source>
        <comment>scripter</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Load Extension Script...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Load a Python script as an extension. Used for loading macros and for advanced Python scripts that extend the Scribus user interface.</source>
        <comment>scripter</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="unfinished">&amp;Скорашње скрипте</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="unfinished">Прикажи &amp;конзолу</translation>
    </message>
    <message>
        <source>Display an interactive Python console where you can write and run Python programs that use the Scripter tools.</source>
        <comment>scripter</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation type="unfinished">С&amp;крипта</translation>
    </message>
    <message>
        <source>Scripter &amp;Settings</source>
        <comment>script menu</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Отвори</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="unfinished">Python скрипте (*.py);; Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Script error</source>
        <translation type="unfinished">Грешка скрипте</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation type="unfinished">Ако радите са званичном скриптом молим Вас пријавите грешку на &lt;a href=\&quot;http://bugs.scribus.net\&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation type="unfinished">Ова порука је и у Вашем клипборду. Користите Ctrl+V да је пренесете у пратиоца грешака.</translation>
    </message>
    <message>
        <source>Scribus - Script Plugin</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="unfinished">Сакриј &amp;конзолу</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScripterPreferences</name>
    <message>
        <source>Scribus - Scripter Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable Scripter Extensions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turn on extension scripts and macros</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;Enabling scripter extensions turns on additional scripter functionality including Python macros and the option of loading a Python script at start-up. Turning on this option unlocks the &lt;tt&gt;Load Extension Script&lt;/tt&gt; item in the Script menu.&lt;/p&gt;
&lt;p&gt;
Only scripts written to be run as extension scripts should be used with &lt;tt&gt;Load Extension Script&lt;/tt&gt; or as start-up scripts. See the scripter documentation for more details.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use a Startup Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;If &lt;tt&gt;Use a Startup Script&lt;/tt&gt; is checked, Scribus will load the script file specified here as an extension script at start-up. It is important that the script be written as an extension script, as if not written carefully it can potentially cause problems.&lt;/p&gt;
&lt;p&gt;&lt;tt&gt;Use a Startup Script&lt;/tt&gt; will be disabled if scripter extensions are off, as extension scripts cannot be loaded without scripter extensions enabled.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Browse for a new script file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Browse for a new script file&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>What script file to load at start-up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;The file containing the Python script to run as an extension script at start-up.&lt;/p&gt;
&lt;p&gt;Note that when this script is run, Scribus has not completely started up and the workspace does not yet exist.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Script File:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Откажи</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>Close without saving changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <source>Save changes and close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation type="unfinished">Напредне опције</translation>
    </message>
    <message>
        <source>Import All Names at Startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Run &apos;from scribus import *&apos; in the script console at start-up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Import All Names at Startup&lt;/tt&gt; is an advanced option. You should probably leave it checked unless you have read the documentation and know what you are doing.&lt;/p&gt;
&lt;p&gt;Unchecking this option will prevent the scripter from running its usual &lt;tt&gt;from scribus import *&lt;/tt&gt; command when it initializes the main interpreter (used for the script console and extension scripts) at start-up.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable Legacy Name Aliases</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Enable the use of OldStyle function names&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Enable Legacy Aliases&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;If checked, this option will cause the scripter to create a large number of function and constant name aliases for 1.2.0 script compatibility. It defaults to checked.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use Fake Stdin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace sys.stdin with a fake file to prevent Scribus hanging when a script tries to read from stdin.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Use Fake Stdin&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;Normally, scribus will provide Python with a fake file object for &lt;tt&gt;sys.stdin&lt;/tt&gt;, so that reads from stdin always return an empty string. If the real &lt;tt&gt;sys.stdin&lt;/tt&gt; is left in place, scripts that try to read from it will block - and in turn block scribus&apos;s execution, making the app appear to hang - until input arrives on stdin. It&apos;s unusual for GUI apps to expect anything on stdin, so mostly users will think scribus has crashed.&lt;/p&gt;
&lt;p&gt;You can disable this option if you want to accept input on stdin. Generally you should use &lt;tt&gt;os.popen&lt;/tt&gt; to make a pipe instead, or use some other input mechanism, but this option is here just in case.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Прикажи прегледе страница</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation>Прикажи имена шаблона</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Тражи/Замени</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Тражи:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Стил пасуса</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Фонт</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Величина фонта</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Ефекти за фонтове</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Боја испуњавања</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Сенка испуњавања</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Боја потеза</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Сенка потеза</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Лева</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Средина</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Десна</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Блок</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Приморано</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ниједан</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Замени са:</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Цела реч</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>Игнориши величину слова</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Тражи</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Замени</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Замени &amp;све</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Затвори</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Претрага је завршена</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">Оч&amp;исти</translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Arrange Pages</source>
        <translation>Намести странице</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation>Доступни шаблони:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Странице документа:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation>Страница окренуте једна према другој</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation>Прво лева страница</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation>Превуците странице или их шаблонирајте у канту за отпатке да бисте их обрисали.</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation>Даје преглед свих страница у Вашем документу.</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation>Ово су сви Ваши шаблони, да бисте напавили нову страницу
превуците шаблон у поглед странице испод.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Нормално</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Изаберите поља</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Доступна поља</translation>
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
        <translation>Изабрана поља</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Остало...</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Сенка:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Намотај</translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Нема стила</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Нема стила</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Уреди стилове</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Примени</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Нови</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Уреди</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Д&amp;уплирај</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Обриши</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Сними</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Копија од %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Нови стил</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation>Да ли стварно желите да обришете овај стил?</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Не</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Отвори</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Документи (*.sla *.sla.gz *.scd *.scd.gz);;Сви фајлови (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Документи (*.sla *.scd);;Сви фајлови (*)</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Уређивач приче</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Нови</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Учитај поново текст из оквира</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Сними у фајл...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Учитај из фајла...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Сними &amp;документ</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Ажурирај текстуални оквир и изађи</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>&amp;Изађи без ажурирања текстуалног оквира</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Изабери &amp;све</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Исеци</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Копирај</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Пренеси</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Оч&amp;исти</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Тражи/Замени...</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation>&amp;Уметни специјално...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>&amp;Уређивање стилова...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>Преглед &amp;фонтова...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Ажурирај текстуални оквир</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Позадина...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Прикажи фонт...</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Фајл</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Уреди</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Подешавања</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Фајл</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation>Очисти сав текст</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation>Учитај текст из фајла</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>Сними текст у фајл</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation>Ажурирај текстуални оквир и изађи</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>Изађи без ажурирања текстуалног оквира</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>Учитај поново текст из оквира</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation>Ажурирај текстуални оквир</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Тражи/Замени</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Текући пасус:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Речи: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Карактери: </translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Укупно:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Пасуси: </translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Да ли желите да снимите измене?</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">&amp;Уметни специјално</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">Преглед &amp;фонтова</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation>Да ли стварно желите да се изгубе све измене?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Да ли стварно желите да очистите сав текст?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Отвори</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Текстуални фајлови (*.txt);;Сви фајлови(*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Сними као</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>Избор &amp;паметног текста</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation>Подвучено</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Мала верзална слова</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Индекс</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Изложилац</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Прецртано</translation>
    </message>
    <message>
        <source>Outline Text</source>
        <translation>Контура текста</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>OO.o Writer Importer Options</source>
        <translation type="obsolete">Опције OO.o Writer увозника</translation>
    </message>
    <message>
        <source>Update paragraph styles</source>
        <translation type="obsolete">Ажурирај стилове пасуса</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Користи име документа као префикс за стилове пасуса</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Не питај опет</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OO.o document and
never ask your confirmation again</source>
        <translation type="obsolete">Да ли би увозник увек требало да користи текућу
постављену вредност приликом увоза OO.o докумената и
да никада више не пита за потврду</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation type="obsolete">Да ли би увозник требало да дода име документа 
испред имена стила параграфа у Scribus-у</translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation type="obsolete">Ако стил пасуса са истим именом као текући пасус OpenOffice.org-а већ постоји, да ли би требало изменити стил у Scribus-у да се поклапа са оним који се увози, или би требало да остане непромењен</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Update Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing stlyes in the current Scribus document</source>
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
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Управљање табулаторима</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Лева</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Десна</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Тачка</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Запета</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Средина</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Положај:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation>Прва &amp;линија:</translation>
    </message>
    <message>
        <source>Ind&amp;ent:</source>
        <translation type="obsolete">Увла&amp;чење:</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Обриши све</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Увлачење за прву линију пасуса</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Увлачење од леве стране за цео пасус</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Обриши све табулаторе</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> тач.</translation>
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
        <source>Left Ind&amp;ent:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>Контура</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Елемент</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Врста</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Информација</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Упозорење</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Име „%1“ није јединствено. 
Молим Вас изаберите друго.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
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
        <translation>Фонт:</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Група </translation>
    </message>
    <message>
        <source>Image</source>
        <translation>Слика</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Линија</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Полигон</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Полилинија</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Текста путање</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Страница</translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation>Уметни вредност</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Унесите вредност а затим притисните „У реду“.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Унесите вредност а затим притисните „У реду“</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Пошаљите Вашу вредност скрипти</translation>
    </message>
</context>
<context>
    <name>VlnaDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">&amp;Откажи</translation>
    </message>
    <message>
        <source>&amp;Info and
Languages</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Replace defaults by user config</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>When the user config file exists 
(%1)
you can choose if you want to append your config
to the global configuration by unchecked button.

You can replace predefined values by yours
with checked button too.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words for Scribus</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Available in the following languages</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>About Short Words</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Алати</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation>Изаберите ставке</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Уметни текстуални оквир</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation>Убаци слику</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Убаци табелу</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Својства...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation>Уметни полигоне</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation>Уметни линије</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation>Уметни bezier криве</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Уметни слободоручну линију</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Ротирај ставку</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Увеличај или умањи</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Уређивање садржаја оквира</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Уређивање текста помоћу уређивача</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Повежи текстуалне оквире</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Прекини везу између текстуалних оквира</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation>Одради премеравање</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation>Исцртај различите облике</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>PDF Tools</source>
        <translation>PDF алати</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Дугме</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Поље за текст</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Кућица</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Комбо кутија</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Кутија листе</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Уметни PDF поља</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Веза</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Уметни PDF анотације</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation>Изаберите карактер:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>Уметн&amp;и</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Оч&amp;исти</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Затвори</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Уметни карактере на месту курсора у тексту</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Обриши текући избор(е).</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation>Затвори овај прозор и врати се на уређивање текста.</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Изаберите увозника</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Аутоматски</translation>
    </message>
    <message>
        <source>Get text only</source>
        <translation type="obsolete">Добави само текст</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Увези текст без било каквог форматирања</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Увозник:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Кодирање:</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Изаберите увозника</translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Запамти везе</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation>Запамти наставак фајла - везе увозника
и немој опет да ме питаш да изаберем увозника за
фајлове ове врсте.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Нови из шаблона</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Уклони</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Отвори</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Све</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Име</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Величина папира</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Боје</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Опис</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Употреба</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Направљен са</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Датум</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Аутор</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Преузимам шаблоне</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Шаблони докумената могу се наћи на http://www.scribus.net/ у одељку Downloads.</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Инсталирам шаблоне</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Распакујте пакет у директоријум за шаблоне ~/.scribus/templates за 
текућег корисника или PREFIX/share/scribus/templates за све кориснике на систему.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Спремам шаблон</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Уверите се да се слике и фонтови које користите могу слободно користити. Ако фонтови не могу бити дељени немојте их скупљати када снимате као шаблон.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Онај који прави шаблоне би такође требало да се увери да се одељак „Инсталирање шаблона“ изнад може применити и на њихове шаблоне. Ово значи да би корисник требало да буде у могућности да преузме пакет шаблона и да би требало да их распакује у директоријум за шаблоне и да почне да их користи.</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Уклањам шаблон</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Уклањање шаблона из прозора „Нови из шаблона“ ће само уклонити унос из фајла template.xml, неће обрисати фајлове документа. Искачући прозор за уклањање ће се појавити само ако имате права за уписивање у фајл template.xml.</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Преводим template.xml</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Копирајте постојећи template.xml у фајл под називом template.lang_COUNTRY.xml (користите исти код језика који је присутан у qm фајлу за Ваш језик), на пример template.fi.xml за Фински језик. Копија се мора налазити у истом директоријуму као и оригинални template.xml да би Scribus могао да га учита.</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Сними као шаблон</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Име</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Категорија</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Величина папира</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Боје</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Опис</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Употреба</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Аутор</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>Е-пошта</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Више детаља</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>У реду</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Мање детаља</translation>
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
        <translation>пејзаж</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>портрет</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>произвољно</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Направи филтер</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Оч&amp;исти</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Обриши</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Изаберите претходно снимљен филтер</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Дајте име за снимање овом филтеру</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Дајте име за снимање</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;У реду</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Откажи</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Искључи или укључи овај ред филтера</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Уклони овај ред филтера</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Додај нови ред филтера</translation>
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
        <translation>уклони поклапање</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>немој да уклониш поклапање</translation>
    </message>
    <message>
        <source>words</source>
        <translation>речи</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Уклони</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Замени</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Примени</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>Вредност са леве стране је регуларни израз</translation>
    </message>
    <message>
        <source>with</source>
        <translation>са</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>стил пасуса</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>сви делови</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>сви пасуси</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>пасуси који почињу са</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>пасуси са мање од</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>пасуси са више од</translation>
    </message>
</context>
</TS>
