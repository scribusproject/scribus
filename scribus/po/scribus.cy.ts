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
</context>
<context>
    <name>About</name>
    <message>
        <source>About Scribus%1%2</source>
        <translation type="obsolete">Ynglyn â Scribus%1%2</translation>
    </message>
    <message>
        <source>%1. %2 %3 </source>
        <translation>%1. %2 %3 </translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation>Scribus Fersiwn %1(new line)
%2 %3</translation>
    </message>
    <message>
        <source>Build-ID:</source>
        <translation>Dynodiad-Adeiladu:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Cyfraniadau o:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation>Addasiad Windows:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation>Almaeneg:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation>Ffrangeg:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation>Eidaleg:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation>Hwngareg:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation>Wcraineg:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation>Bwlgareg:</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation>Galiseg:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation>Twrceg:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation>Lithwaneg:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation>Pwyleg:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation>Tsieceg:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation>Slofaceg:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation>Daneg:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation>Norwyeg:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation>Cymraeg:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation>Rwsieg:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation>Portiwgaleg Brasil:</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation>Ffinneg:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation>Slofeneg: </translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation>Basg:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Mae&apos;r panel yma yn dangos y fersiwn, dyddiad adeiladu a&apos;r
gynhaliaeth rhaglengelloedd a ymgrynhowyd yn Scribus.
Mae&apos;r C-C-T yn golygu cynhaliaeth CUPS - littlecms - TIFF.
Dynodir cynhaliaeth raglengelloedd sydd ar goll gan *</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;Ynglyn â</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>&amp;Awduron</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;Cyfieithiadau</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;Arlein</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cau</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>English (British):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation type="unfinished">Cyfeireb Arlein</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Catalan:</source>
        <translation type="unfinished"></translation>
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
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation>Dewisiadau Uwch</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation>Creu PostScript Lefel 3</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation>Creu Postscript Lefel 2 yn unig;
gofaler - gally hyn greu ffeiliau enfawr</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation>Creu Postscript Lefel 1 yn unig;
gofaler - gally hyn greu ffeiliau enfawr</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation>Adlewyrchu Tudalen(nau) yn &amp;Llorweddol</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation>Adlewyrchu Tudalen(nau) yn &amp;Fertigol</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation>Gweithredu &amp;Proffilau ICC</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation>PostScript Lefel &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation>PostScript Lefel &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation>PostScript Lefel &amp;3</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation>Gweithredoli Gwaredu &amp;Dan-liw</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Ffordd o atal rhai o&apos;r arlliwiau llwyd a gyfansoddir o wyrddlas, melyn 
a magenta, a defnyddio du yn eu lle.  Mae UCR efo&apos;r effaith mwyaf ar 
ddarnau delweddau sy&apos;n amhendant a/neu sy&apos;n cynnwys graddliwiau tywyll
sy&apos;n agosaf i&apos;r llwyd.  Gall defnyddio hyn wella argraffu ar rai delweddau,
ond mae angen tipyn o brofi ac arbrofi yn ôl y galw.  Mae UCR yn lleihau&apos;r 
bosibilrwydd o or-ddirlawnder efo inciau CMY.</translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Distribute/Align</source>
        <translation>Dosbarthu/Alinio</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Alinio</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Llorweddol</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation>Ochrau Chwith</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation>Canolau</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation>Ochrau Dde</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Fertigol</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation>Ochrau Pen</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation>Ochrau Gwaelod</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>mod</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Gweithredu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation>&amp;Rhwng:</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation>A&amp;linio</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation>Dad&amp;leoliad</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation>Dosbarthu yn &amp;Gyson</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation>R&amp;hwng:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation>Peidio â &amp;Newid</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation>Al&amp;inio</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation>Dadl&amp;eoliad</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation>Dosbarthu yn G&amp;yson</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation>&amp;Peidio â Newid</translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation>Alinio Testun i&apos;r Chwith</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Alinio Testun i&apos;r Dde</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Alinio Testun yn y Canol</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Alinio Testun Unioni</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Alinio Testun Gorfod Unioni</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Priodweddau Meysydd</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Math:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Botwm</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Maes Testun</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Blwch Gwirio</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Blwch Cyfun</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Blwch Rhestr</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Priodweddau</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Enw:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Cynghoryn Offer:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Testun</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Ffin</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Lliw:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Dim</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Lled:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Tenau</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Arferol</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Tew</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Arddull:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Solet</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Toredig</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Tanlinell</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Befeledig</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Mewnosodedig</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Arall</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Darllen yn Unig</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Hanfodol</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Peidiwch ac Allforio Gwerth</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Gwelededd:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Gweladwy</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Cuddiedig</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Dim Argraffu</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Dim Golwg</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Ymddangosiad</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Testun am Fotwm i Lawr</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Testun am Droslen</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Eiconau</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Defnyddio Eiconau</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Gwaredu</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Gwasgedig</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Rholio Drosodd</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Lleoliad Eiconau...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Amlygu</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Gwrthdroi</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Amlinedig</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Gwthio</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Aml-Linell</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Cyfrinair</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Terfyn o</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Nodau</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Peidwich â Sgrolio</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Peidiwch â Gwirio Sillafu</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Gwirio Arddull:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Gwirio</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Croes</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Diamwnt</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Cylch</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Seren</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Sgwâr</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Mae&apos;r Rhagosod wedi&apos;i Ddewis</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Golygadwy</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Dewisiadau</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation>Sgript Java</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Ewch I</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Anfon Ffurflen</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Ailosod  Ffurflen</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Mewnforio Data</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Digwyddiad:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Llygoden i Fyny</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Llygoden i Lawr</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Llygoden Mynd i Mewn</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Llygoden Mynd Allan</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Ar Ganolbwyntio</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Ar Bylu</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Sgript:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Golygu...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Anfon i URL:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Anfon Data fel HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Mewnforio Data oddiwrth:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Cyrchfan</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>I Ffeil:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Newid...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Tudalen:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>Lleoliad X:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Lleoliad Y:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Gweithred</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Mae&apos;r maes wedi&apos;i fformatio fel:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Plaen</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Rhif</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Canran</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Dyddiad</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Amser</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Addasiedig</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Fformat Rhif</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Degolion:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Defnyddio Symbol Arian</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Rhagddodi Symbol Arian</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Fformatio</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Fformat Canran</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Fformat Dyddiad</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Fformat Amser</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Sgriptiau Addasiedig</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Fformat:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Gwasgiad Bysell:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Fformat</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Nid yw&apos;r gwerth wedi&apos;i ddilysu</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Rhaid i&apos;r gwerth fod yn fwy na neu gyfartal i:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>a llai na neu gyfartal i:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Sgript dilysu addasiedig:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Dilysu</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Ni chyfrifir y gwerth</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Y gwerth yw</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>cyfanswm</translation>
    </message>
    <message>
        <source>product</source>
        <translation>lluoswm</translation>
    </message>
    <message>
        <source>average</source>
        <translation>cyfartaledd</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>lleiafswm</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>uchafswm</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>o&apos;r meyseydd dilynol:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Dewis...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Sgript cyfrifo addasiedig:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Cyfrifo</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Iawn</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Diddymu</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Rhowch rhestr o feyseydd wedi&apos;i gwahanu gan atalnod</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Rhaid i chi gael o leiaf yr Eicon am Arferol er mwyn defnyddio Eiconau fel Botymau</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Agor</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation>Delweddau (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Enghraifft:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Newid Dewisiad</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Wynebfath i&apos;w ddefnyddio efo PDF-1.3:</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Anwybyddir y faner ar gyfer PDF-1.3</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Dogfennau-PDF (*.pdf);;Pob Ffeil (*)</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Priodweddau Anodiad</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Testun</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Cyswllt</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Cyswllt Allanol</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Cyswllt Gwê Allanol</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Cyrchfan</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Agor</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>Dogfennau-PDF (*.pdf);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation type="unfinished">Lleoliad &amp;Y:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Apply Template</source>
        <translation>Gweithredu Patrymlun</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Arferol</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Patrymlun:</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation>Gweithredu i&apos;r Dudalen &amp;Gyfredol</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation>Gweithredu o &amp;Dudalen:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>I:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation>Gweithredu i bob Tudalen &amp;eilrif</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation>Gweithredu i bob Tudalen &amp;odrif</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>Llyfr Lloffion</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>Llyfrau Lloffion (*.scs);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Dileu</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Gwrthrych</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Cofnod Newydd</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Ail-enwi</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Nid yw&apos;r enw &quot;%1&quot; yn unigryw.
Dewiswch un arall.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Iawn</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Newydd</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Llwytho...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Cadw</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Cadw &amp;Fel...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cau</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>&amp;Bach</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>&amp;Canolig</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>&amp;Mawr</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Ffeil</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Rhagolwg</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Enw:</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>Tudnodau</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>Symud Tudnod</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Mewnosod Tudnod</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Diddymu</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Tudnodau</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Lleoliad Eiconau</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Haenlun:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Egluryn yn unig</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Eicon yn unig</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Egluryn dan yr Eicon</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Egluryn uwchben yr Eicon</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Egluryn ar ochr de yr Eicon</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Egluryn ar ochr chwith yr Eicon</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Egluryn yn troshaenu&apos;r Eicon</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Graddfa:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>O Hyd</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Pan mae&apos;r Eicon yn rhy fach</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Pan mae&apos;r Eicon yn rhy fawr</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Byth</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Sut i Raddio:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Cyfrannol</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Di-Gyfrannol</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Eicon</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Iawn</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Diddymu</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Ailosod</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Color Management Settings</source>
        <translation>Gosodiadau Rheoli Lliw</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Proffilau Cysawd</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Bwriadau Llunio</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Canfyddiadol</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Lliwfesurol Perthnasol</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Dirlawnder</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Lliwfesurol Absoliwt</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation>Proffil rhagosod lliwiau ar gyfer delweddau a fewnforir</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Proffil rhagosod lliwiau ar gyfer lliwiau solet ar y dudalen</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Proffil lliw a greuwyd gennych, neu a dderbynnir gan y gwneuthurwr.
Dylai&apos;r proffil yma fod yn benodol i&apos;ch monitr, a dim yn broffil cyffredinol (h.y. sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Proffil lliw ar gyfer eich argraffydd, a ddarperir gan y gwneuthurwr.
Dylai&apos;r proffil yma fod yn benodol i&apos;ch monitr, a dim yn broffil cyffredinol (h.y. sRGB).</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Bwriad llunio rhagosodod ar gyfer eich monitr.  Onibai i chi wybod sut i&apos;w newid,
dyler dewis Lliwfesurol Perthnasol neu Ganfyddiadol.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Bwriad llunio rhagosodod ar gyfer eich argraffydd.  Onibai i chi wybod sut i&apos;w newid,
dyler dewis Lliwfesurol Perthnasol neu Ganfyddiadol.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Alluogi &apos;profi meddal&apos; o sut bydd lliwiau eich dogfen yn argraffu,
wedi&apos;i seilio ar y proffil argraffydd a ddewisir.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Dull o ddangos lliwiau ar y sgrîn, er gall fod na fyddant yn argraffu yn gywir.
Mae proffilau manwl iawn yn hanfodol, a dylai hyn fod yn rybudd yn unig.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Mae Cyfadferiad Pwynt-du yn ddull o wella&apos;r gyferbyniad mewn ffotos.
Argymhellir alluogi hyn os oes gennych ffotos yn eich dogfen.</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Gweithredoli Rheoli Lliw</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation>&amp;Lluniau:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>Lliwiau &amp;Solet:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Dangosydd:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>A&amp;rgraffydd:</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>D&amp;angosydd:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>Ar&amp;graffydd:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>&amp;Efelychu Argraffydd ar y Sgrîn</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Nodi Lliwiau allan o &amp;Resaid</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Defnyddio Cyfadferiad Pwynt-&amp;du</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Golygu Lliw</translation>
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
        <translation>RGB Gwê</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Newydd</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Hen</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Iawn</translation>
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
        <translation>Bariau Lliw Dynamig</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Bariau Lliw Sefydlog</translation>
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
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation>Ni yw enw&apos;r Lliw yn unigryw</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>Map-lliw HSV</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Enw:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>Model &amp;Lliw</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Dim</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Normal</source>
        <translation>Arferol</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Graddiant Llorweddol </translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Graddiant Fertigol</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Graddiant Croeslinol</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Graddiant Croes-groeslinol</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Graddiant Rheiddiol</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Didreiddiad:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Dim</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Arlliw:</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Golygu Priodweddau&apos;r Lliw Llinell</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Golygu Priodweddau&apos;r Lliw Llenwad</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Dirlawnder lliw</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Dull arferol neu lenwad graddfa</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Gosod tryloywder y lliw penodol</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation>Lliw y gwrthrych penodol</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Graddiant llinellol rhydd</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Graddiant rheiddiol rhydd</translation>
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
        <translation>mod</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">Iawn</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Diddymu</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Dim</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Dewisiadau Argraffydd</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Dewisiad</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Gwerth</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Set Tudalennau</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Pob Tudalen</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Tudalennau Ailrif yn unig</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Tudalennau Odrif yn unig</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Adlewyrchu</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nac ydw</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ydw</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Cyfeiriadaeth</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Darlun</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Tirlun</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Argraffu o N i Fyny</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Tudalen ar bob Dalen</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Tudalennau ar bob Dalen</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Mae&apos;r panel yma yn dangos amrywiaeth o ddewisiadau CUPS, i&apos;w
defnyddio wrth argraffu.  Bydd y paramedrau penodol sydd ar gael
yn dibynnu ar eich gyrrydd argraffu.  Gallwch gadarnhau cynhaliaeth
CUPS gan dewis Cymorth -&gt; Ynglyn â.  Chwiliwch am y rhestriadau:
C-C-T.  Mae hyn yn golygu cynhaliaeth CUPS - littlecms - TIFF.
Dynodir cynhaliaeth raglengelloedd sydd ar goll gan *</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>Encoding:</source>
        <translation>Amgodiad:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Symud i&apos;ch Cyfeiriadur Dogfennau.
  Gellir gosod hyn yn yr Hoffterau.</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Cywasgu Ffeil</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>Cy&amp;nnwys Wynebfathau</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Dileu Lliw</translation>
    </message>
    <message>
        <source>?</source>
        <translation>?</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation>Amnewid efo:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Iawn</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Diddymu</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation>Dileu lliw:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Dim</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Dileu Tudalennau</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation>Dileu o:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>i:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>Missing Font</source>
        <translation>Wynebfath ar Goll</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Nid yw&apos;r wynebfath %1 wedi&apos;i sefydlu.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Defnyddio</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>yn lle</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Iawn</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Gwybodaeth Ddogfen</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Teitl:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>Awd&amp;ur:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Allweddeiriau:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>D&amp;isgrifiad:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>Cyho&amp;eddwr:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>C&amp;yfranwyr:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>&amp;Dyddiad:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>Ma&amp;th:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>F&amp;ormat:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>Dyn&amp;odiad:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Tarddiad:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Iaith:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;Perthynas:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>Cy&amp;nnwys:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>&amp;Hawliau:</translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation>D&amp;ogfen</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Gwybodaeth &amp;Bellach</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Y person neu sefydliad sydd efo&apos;r prif gyfrifoldeb ar gyfer creu cynnwys y ddogfen.
Gellir caethiwo&apos;r maes yma yn y ddogfen Scribus fel cyfeirnod, ac hefyd ym metadata PDF</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Enw a roddir i&apos;r ddogfen.
Gellir caethiwo&apos;r maes yma yn y ddogfen Scribus fel cyfeirnod, ac hefyd ym metadata PDF</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Crynodeb o gynnwys y ddogfen.
Mae&apos;r maes yma ar gyfer disgrifiad byr neu grynodeb y ddogfen.  Caethiwir yn y PDF wrth allforio</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Pwnc cynnwys y ddogfen.
Mae&apos;r maes yma ar gyfer allweddeiriau y ddogfen yr hoffech gaethiwo mewn PDF, er mwyn cynorthwyo chwilio a mynegeio ffeiliau PDF</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Person neu sefydliad sy&apos;n gyfrifol ar gyfer cyhoeddi&apos;r ddogfen</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Person neu sefydliad sy&apos;n gyfrifol ar gyfer gweud cyfraniadau i gynnwys y ddogfen</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Dyddiad sy&apos;n gysylltiedig efo digwyddiad yng nghylch bywyd y ddogfen, mewn fformat BBBB-MM-DD, fel yn ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Natur neu math cynnwys y ddogfen, e.e. categoriau, swyddogaethau, genrau, ayyb</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Amlygiad corfforol neu ddigidol y ddogfen.  Byddai math cyfryngau a dimensiynau yn werth eu nodi.
Mae RFC2045 ac RFC2046 ar gyfer mathau MIME yn ddefnyddiol yma hefyd</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Cyfeirnod diamwys i&apos;r ddogfen tu mewn cyd-destun penodol fel ISBN neu URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Cyfeirnod i ddogfen y mae&apos;r ddogfen bresennol yn tarddu ohoni, e.e. ISBN neu URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Yr iaith yr ysgrifennir cynnwys y ddogfen ynddi, fel rheol côd iaith ISO-639
efo ôl-ddodiad dewisol o gysylltnod a côd gwlad ISO-3166, e.e. en-GB, fr-CH</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Cyfeirnod i ddogfen berthynol, efallai yn defnyddio dynodiad ffurfiol fel ISBN neu URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Lledaeniad neu gwmpas cynnwys y ddogfen, efallai yn cynnwys lleoliad, amser ac amrediadau awdurdodaethol</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Gwybodaeth am hawliau yn neu dros y ddogfen, e.e. hawlfraint, breinlen, neu nod masnach</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation>Gosod Argraffydd</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Ffeil</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Dewisiadau</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Popeth</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Cadw fel</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation>Ffeiliau Postscript (*.ps);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Gwyrddlas</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Melyn</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Du</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Mewnosod rhestr wedi&apos;i gwahanu gan atalnodau o docynnau, lle
gall tocyn fod yn * ar gyfer pob tudalen, 1-5 ar gyfer
amrediad tudalennau, neu rhif tudalen unigol.</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Cyrchfan Argraffu </translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Dewisiadau...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Ffeil:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Newid...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>Gorchymyn Argraffydd &amp;Eiledol</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>Go&amp;rchymyn:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Amrediad</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Argraffu &amp;Popeth</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Argraffu &amp;Tudalen Gyfredol</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Argraffu Am&amp;rediad</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>Nifer o &amp;gopïau:</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation>Argraffu yn &amp;Arferol</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation>Argraffu &amp;Gwahaniadau</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation>Argraffu mewn &amp;Lliw os ar Gael</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation>Argraffu mewn Gra&amp;ddlwyd</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation>Dewisiadau &amp;Uwch...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Argraffu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation>Defnyddio rheolydd argraffu arall, megis kprinter neu gtklp,
i gael dewisiadau argraffu ychwanegol</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Mewnforio Ffeil:
methodd
%1!
</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Gwall Angheuol</translation>
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
        <translation type="unfinished">&amp;Diddymu</translation>
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
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Golygu Arddull</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Nod</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation>Effaith:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Dim</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation>Bylchau Fertigol</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Bylchu Llinellau</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Iawn</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation>Nid yw enw&apos;r Ardull yn unigryw</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Enw eich arddull paragraff</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Wynebfath y testun neu wrthrych penodol</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Maint yr Wynebfath</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Lliw llenwad y testun</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Lliw trawiad y testun</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation>Darparu llythyren gyntaf orfaint ar gyfer paragraff.  Defnyddir ar gyfer effaith arddulliol</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Penodi&apos;r uchder cyffredinol, mewn rhifau llinellau, o&apos;r Priflythrennau Cwymp</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation>Alinio testun i grid gwaelodlin</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Bylchu cyn y paragraff</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Bylchu ar ôl y paragraff</translation>
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
        <source>Tabulators and Indentation</source>
        <translation>Tablyddion a Mewnoliad</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Enw:</translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation>&amp;Wynebfath:</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation>Ma&amp;int:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation>&amp;Aliniad:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation>Priflythrennau &amp;Hir</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>Lli&amp;nellau:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Lliw &amp;Llenwad:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation>Lliw &amp;Trawiad:</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation>Addasu i Grid &amp;Gwaelodlin</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation>&amp;Bylchu Llinellau:</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation>&amp;Uwchlaw:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation>&amp;Islaw:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Golygydd</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation>Javascriptiau (*.js);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Newydd</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Agor...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Cadw &amp;Fel...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Cadw a Terfynu</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Terfynu heb Gadw</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Dadwneud</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Ail-wneud</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>To&amp;rri</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copïo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Gludo</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>G&amp;waredu</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>Nôl Enwau&apos;r &amp;Meysydd</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Ffeil</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Golygu</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Dewis Cyfeiriadur Allforio</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Pob Tudalen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Newid y cyfeiriadur allforio</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Y cyfeiriadur allforio - y lle i gadw eich delweddau.
&apos;Enw_dogfen-rhif_tudalen.math_ffeil&apos; fydd enw&apos;r ffeil a allforir</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Allforio&apos;r dudalen gyfredol yn unig</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Fformatau allforio ar gael</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Newid...</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>Allforio i &amp;Gyfeiriadur:</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>Math o &amp;Ddelwedd:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Ansawdd:</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Allforio fel Delwedd(au)</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Dewisiadau</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Cydraniad:</translation>
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
        <translation>Amrediad</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>&amp;Tudalen Gyfredol</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>Am&amp;rediad</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Allforio amrediad o dudalennau</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Mewnosod rhestr wedi&apos;i gwahanu gan atalnodau o docynnau, lle
gall tocyn fod yn * ar gyfer pob tudalen, 1-5 ar gyfer
amrediad tudalennau, neu rhif tudalen unigol.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Allforio pob tudalen</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Cydraniad y Delweddau
Defnyddiwch 72 dpi ar gyfer Delweddau a fwriedir i&apos;r Sgrin</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Ansawdd eich delweddau - y gorau yw 100%, yr ansawdd isaf yw 1%</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Maint:</translation>
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
        <translation>Maint:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Teitl:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Di-deitl</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Awdur:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Anhysbys</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Dogfen Scribus</translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Colors</source>
        <translation>Lliwiau</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Setiau Lliw</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Set Lliw Cyfredol:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Dewis Enw</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Agor</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dogfennau (*.sla *.sla.gz *.scd *.scd.gz);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dogfennau (*.sla *.scd);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Lliw Newydd</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copi o %1</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Dewis casgliad o liwiau i&apos;w llwytho</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Cadw&apos;r casgliad cyfredol o liwiau</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Gwaredu lliwiau heb eu defnyddio o gasgliad y lliwiau yn y  ddogfen cyfredol</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation>Atodi lliwiau i&apos;r casgliad cyfredol o ddogfen sydd yn bodoli</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Creu lliw newydd tu fewn y casgliad cyfredol</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Golygu&apos;r lliw a ddewisir ar hyn o bryd</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Gwneud copi o&apos;r lliw a ddewisir ar hyn o bryd</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Dileu&apos;r lliw a ddewisir ar hyn o bryd</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Gwneud i&apos;r casgliad cyfredol o liwiau fod yn gasgliad rhagosod</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Atodi</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Newydd</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Golygu</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;yblygu</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Dileu</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>&amp;Gwaredu nas Defnyddiwyd</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>Cadw&apos;r &amp;Set Lliw</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Enw:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Dim</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Global Font Settings</source>
        <translation>Gosodiadau Wynebfath Eang</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Wynebfathau ar Gael</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Amnewidiadau Wynebfath</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Llwybrau Ychwanegol</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ydw</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Enw&apos;r Wynebfath</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation>Defnyddio Wynebfath</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation>Caetháu yn:</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Is-set</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Math</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation>Llwybr i&apos;r Ffeil Wynebfath</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Amnewidiad</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Dewis Cyfeiriadur</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>Wynebfathau ar &amp;Gael</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>Amnewidiadau &amp;Wynebfath</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>Llwybrau &amp;Ychwanegol</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Dileu</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Newid...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>&amp;Ychwanegu...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Gwaredu</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Ddydd Sul, bûm i&apos;n caniatáu i Jac, y gŵr o Fôn, gopïo llun y tŷ haf</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <translation>Rhagolwg Wynebfathau</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Lleoliad:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Yma, gallwch ychwanegu, newid, neu waredu Ataliadau Lliw.</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Trefnu Cyfeiryddion</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Cyfeiryddion Llorweddol</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Cyfeiryddion Fertigol</translation>
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
        <translation>Lleoliad &amp;Y:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Ychwanegu</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>&amp;Dileu</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>Lleoliad &amp;X:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>Ych&amp;wanegu</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>Dil&amp;eu</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>Clo&amp;i Cyfeiryddion</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Link</source>
        <translation type="unfinished">Cyswllt</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Cysylltnodi Posibl</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Derbyn</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Hepgor</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Diddymu</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Hyphenator Settings</source>
        <translation>Gosodiadau Cysylltnodydd</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Hyd y gair lleiaf i&apos;w gysylltnodi.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Y nifer mwyaf o Gysylltnodiadau sy&apos;n dilyn eu gilydd.
Mae gwerth o 0 yn golygu cysylltnodiadau heb derfyn. </translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Iaith:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>Gair &amp;Lleiaf:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Mewnosod Tudalen</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation>Mewnosod</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>cyn Tudalen</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>wedi Tudalen</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>wrth Ddiwedd</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Arferol</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation>Patrymlun (Tudalen Dde):</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation>&amp;Mewnosod</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Tudalen(nau)</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation>Patrymlun (Tudalen &amp;Chwith):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Patrymlun:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Mewnosod Tabl</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation>Nifer o Resi:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation>Nifer o Golognau:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Iawn</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Diddymu</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>New Script</source>
        <translation>Sgript Newydd</translation>
    </message>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Golygu Sgriptiau Java</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Golygu...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Ychwanegu...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Dileu</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cau</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>S&amp;gript Newydd:</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nac ydw</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ydw</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation>Rheoli Byrlwybrau Bysellfwrdd</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Gweithred</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Bysell Gyfredol</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Dewis Bysell am y Weithred yma</translation>
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
        <translation>Syflyd</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation>Syflyd+</translation>
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
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>Dim &amp;Bysell</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>Bysell wedi&apos;i &amp;Diffinio gan y Defnyddiwr</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>&amp;Gosod Bysell</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation>Mae&apos;r Dilyniant Bysell yma mewn defnydd eisoes</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation>Haenau</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation>Ychwanegu Haen newydd</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Dileu Haen</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation>Codi Haen</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation>Gostwng Haen</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation>Haen Newydd</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation>Ydych eisiau dileu&apos;r Gwrthrychau i gyd ar yr Haen yma hefyd?</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Golygu Arddulliau Llinell</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copi o %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Arddull Newydd</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Agor</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dogfennau (*.sla *.sla.gz *.scd *.scd.gz);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dogfennau (*.sla *.scd);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Atodi</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Newydd</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Golygu</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;yblygu</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Dileu</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Cadw</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nac ydw</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ydw</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="unfinished"></translation>
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
        <translation>mod</translation>
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
        <translation>mod</translation>
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
        <translation type="unfinished">&amp;Newydd</translation>
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
        <source>Close this dialog.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Return to Scribus.</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Disgrifiad</translation>
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
        <translation type="unfinished">&amp;Golygu...</translation>
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
        <source>&lt;p&gt;Edit the selected macro. &lt;/p&gt;
&lt;p&gt;If this button is greyed out, either there is no selected macro or the macro manager does not have the source code for the macro you have selected (in which case &lt;tt&gt;No&lt;/tt&gt; will be shown in the &lt;tt&gt;Edit &lt;/tt&gt;column of the macro).&lt;/p&gt;
&lt;p&gt;If scribus doesn&apos;t have the source, the macro was probably created by a script.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Dileu</translation>
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
        <translation type="unfinished">&amp;Mewnforio</translation>
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
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Dyblygiad Lluosol</translation>
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
        <translation>mod</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Nifer o Gopïau:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>Syfliad &amp;Llorweddol:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>Syfliad &amp;Fertigol:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Pellterau</translation>
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
        <translation>Ongl:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Hyd:</translation>
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
        <translation>mod</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
</context>
<context>
    <name>MenuTest</name>
    <message>
        <source>Script error</source>
        <translation>Gwall Sgript</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Os ydych yn rhedeg sgript swyddogol, rhowch adrodd wrth &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>&amp;Dangos Terfynell</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation>&amp;Cuddio Terfynell</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Mae&apos;r neges yma ar eich gludfwrdd hefyd.  Defnyddiwch CTRL+V i&apos;w gludo i mewn i&apos;r dilynnydd namau.</translation>
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
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Change...</source>
        <translation>Newid...</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Mewnforio</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Diddymu</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Agor</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dogfennau (*.sla *.sla.gz *.scd *.scd.gz);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dogfennau (*.sla *.scd);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Import Template</source>
        <translation>Mewnforio Patrymlun</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Mewnforio Tudalen(nau)</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation>O Ddogfen:</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation>Mewnforio Tudalen(nau):</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Mewnosod rhestr wedi&apos;i gwahanu gan atalnodau o docynnau, lle
gall tocyn fod yn * ar gyfer pob tudalen, 1-5 ar gyfer
amrediad tudalennau, neu rhif tudalen unigol.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation>o 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Creu Tudalen(nau)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>cyn Tudalen</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>wedi Tudalen</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>wrth Ddiwedd</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation>o %1</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Symud Tudalennau</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Copïo Tudalen</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Symud Tudalen(nau):</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>i:</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>cyn Tudalen</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>wedi Tudalen</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>wrth Ddiwedd</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>Priodweddau</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Enw</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometreg</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Sailbwynt:</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Lefel</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Siâp:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Pellter y Testun</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Dangos Cromlin</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Dechrau Atred:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Pellter o&apos;r Cromlin:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation>Bylchu Addasiedig</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Mewnosod Proffil:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Bwriad Llunio:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Canfyddiadol</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Lliwfesurol Perthnasol</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Dirlawnder</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Lliwfesurol Absoliwt</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Pwynt Chwith</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Pwyntiau Diwedd</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Uniad Meitrog</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Uniad Befel</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Uniad Crwn</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Pen Gwastad</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Pen Sgwâr</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Pen Crwn</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Dim Arddull</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Maint yr Wynebfath</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Bylchu Llinellau</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation>Gorgyffyrddiad â Llaw</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Dim</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Nid yw&apos;r enw &quot;%1&quot; yn unigryw.
 Dewiswch un arall.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Iawn</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Priodweddau Testun Llwybr</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Arlliw:</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Llinellau Cell</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Llinell wrth y Pen</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Llinell wrth y Chwith</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Llinell wrth y De</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Llinell wrth y Gwaelod</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Enw&apos;r gwrthrych penodol</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Lleoliad llorweddol y gwaelodbwynt cyfredol</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Lleoliad fertigol y gwaelodbwynt cyfredol</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Lled</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Taldra</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Cylchdroi y gwrthrych wrth y gwaelodbwynt cyfredol</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Pwynt y cyfeirir mesuriadau neu onglau cylchdroi ohono</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Dewis pen chwith y gwaelodbwynt</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Dewis pen de y gwaelodbwynt</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Dewis gwaelod chwith y gwaelodbwynt</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Dewis gwaelod de y gwaelodbwynt</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Dewis canolbwynt y gwaelodbwynt</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Troi drosodd yn Llorweddol</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Troi drosodd yn Fertigol</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Symud un lefel i fyny</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Symud un lefel i lawr</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Symud i&apos;r blaen</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Symud i&apos;r cefn</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Dynodi&apos;r lefel y mae&apos;r gwrthrych arno, efo 0 yn golygu bod y gwrthrych wrth y gwaelod</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>(Dad)gloi&apos;r gwrthrych</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>(Dad)gloi maint y gwrthrych</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>(An)alluogi argraffu&apos;r gwrthrych</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Gwneud i&apos;r testun mewn fframau is lifo o gwmpas siâp y gwrthrych</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Wynebfath y testun neu wrthrych penodol</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Lled graddio nodau</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Lliw trawiad y testun</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Lliw llenwad y testun</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Dirlawnder lliw trawiad y testun</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Dirlawnder lliw llenwad y testun</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Arddull y paragraff cyfredol</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Newid gosodiadau ar gyfer pwyntiau chith neu derfyn</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Patrwm y llinell</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Trwch y llinell</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Math uniadau&apos;r llinellau</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Math terfyn y llinellau</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Arddull llinell y gwrthrych cyfredol</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Dewis siâp y ffrâm...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Golygu siâp y ffrâm...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Gosod radiws y crynder corneli</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Nifer o golofnau yn y ffrâm testun</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Newid rhwng Bwlch neu led Colofn</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Pellter rhwng colofnau</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Pellter y testun o ben y ffrâm</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Pellter y testun o waelod y ffrâm</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Pellter y testun o chwith y ffrâm</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Pellter y testun o dde y ffrâm</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Golygu gosodiadau tab y ffrâm testun...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Caniatáu i&apos;r ddelwedd fod yn faint gwahanol i&apos;r ffrâm</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Atred llorweddol y ddelwedd tu mewn y ffrâm</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Atred fertigol y ddelwedd tu mewn y ffrâm</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Newid maint y ddelwedd yn llorweddol</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Newid maint y ddelwedd yn fertigol</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Cadw&apos;r graddio X a Y yn hafal</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Cadw&apos;r gymhareb agwedd</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Gwneud i&apos;r ddelwedd ffitio tu mewn maint y ffrâm</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Defnyddio cyfraneddau&apos;r ddelwedd yn lle rhai y ffrâm</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Proffil tarddiad y ddelwedd</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Bwriad llunio ar gyfer y ddelwedd</translation>
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
        <source>Column width</source>
        <translation>Lled y colofn</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Siâp</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>T&amp;estun</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Delwedd</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>Lli&amp;nell</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Lliwiau</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>Lleoliad &amp;X:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Lleoliad &amp;Y:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Lle&amp;d:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Taldra:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>C&amp;ylchdro:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>Golygu &amp;Siâp...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>Talgrynnu
Co&amp;rneli:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Colo&amp;fnau:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Bwlch:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Pen:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Gwaelod:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Chwith:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;De:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>&amp;Tablyddion...</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>Testun yn Llifo o Gwmpas y &amp;Ffrâm</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Defnyddio &amp;Blwch Ffinio</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>Defnyddio Llinell &amp;Amlin</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Maint yr &amp;Wynebfath:</translation>
    </message>
    <message>
        <source>&amp;Kerning:</source>
        <translation>&amp;Gorgyffyrddiad:</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation>&amp;Bylchu Llinellau:</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>&amp;Arddull:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>&amp;Iaith:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Graddfa &amp;Rydd</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>&amp;Graddfa X:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>&amp;Graddfa Y:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Graddio i &amp;Faint y Ffrâm</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>Cy&amp;frannol</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Sailbwynt:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>Mat&amp;h o Linell:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Ll&amp;ed y Llinell:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>Och&amp;rau:</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>D&amp;iweddiadau:</translation>
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
        <translation>Defnyddio blwch amgylchynnol yn lle siâp y ffrâm ar gyfer llifo testun</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Defnyddio ail linell a seiliwyd yn wreiddiol ar siâp y ffrâm ar gyfer llifo testun</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Iaith cyplysnodi y ffrâm</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation>Golygu Arddull</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Pen Gwastad</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Pen Sgwâr</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Pen Crwn</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Uniad Meitrog</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Uniad Befel</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Uniad Crwn</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Lled y Llinell:</translation>
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
        <translation>Iawn</translation>
    </message>
    <message>
        <source> pt </source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Llinell Solet</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Llinell Doredig</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Llinell Dotiog</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Llinell Doriad Dot</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Llinell Doriad Dot Dot</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Nid yw&apos;r enw &quot;%1&quot; yn unigryw.
 Dewiswch un arall.</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Edit Templates</source>
        <translation>Golygu Patrymluniau</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Enw:</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation>Patrymlun Newydd</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copi o %1</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copi #%1 o</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Arferol</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Atodi</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Newydd</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;yblygu</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Dileu</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cau</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nac ydw</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ydw</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Enw:</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>New Document</source>
        <translation>Dogfen Newydd</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Maint y Dudalen</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Addasiedig</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Darlun</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Tirlun</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Cyfeiryddion Ymylon</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Dewisiadau</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation>Pwyntiau (pts)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Milimedrau (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Modfeddi (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picau (p)</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation>Cyfeiryddion Colofnau</translation>
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
        <translation>Maint tudalennau&apos;r ddogfen, un ai maint arferol neu faint addasiedig</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Cyfeiriadaeth tudalennau&apos;r ddogfen</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Lled tudalennau&apos;r ddogfen, a ellir ei olygu os ydych wed dewis maint dudalen addasiedig</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Uchder tudalennau&apos;r ddogfen, a ellir ei olygu os ydych wed dewis maint dudalen addasiedig</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Alluogi haenlun sengl neu seiliedig ar daeniad</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Gwneud y dudalen gyntaf yn dudalen chwith y ddogfen</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Pellter rhwng y cyfeirydd ymyl pen ac ochr y dudalen</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Pellter rhwng y cyfeirydd ymyl gwaelod ac ochr y dudalen</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Pellter rhwng y cyfeirydd ymyl chwith ac ochr y dudalen.
Os dewisir Tudalennau Wynebu, gellir defnyddio&apos;r lle ymylon yma i gael yr ymylon cywir ar gyfer rhwymo</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Pellter rhwng y cyfeirydd ymyl de ac ochr y dudalen.
Os dewisir Tudalennau Wynebu, gellir defnyddio&apos;r lle ymylon yma i gael yr ymylon cywir ar gyfer rhwymo</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>Rhif tudalen gyntaf y ddogfen</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Uned mesur rhagosod ar gyfer golygu dogfennau</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Creu fframiau testun yn ymysgogol pan ychwanegir tudalennau newydd</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Nifer colofnau i&apos;w creu mewn fframiau testun a greir yn ymysgogol</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Pellter rhwng colofnau a greir yn ymysgogol</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Cyfreithiol</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Llythyr</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloid</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Maint:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>C&amp;yfeiriadaeth:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Lle&amp;d:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Taldra:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>Tudalennau &amp;Wynebu</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Tudalen &amp;Chwith yn Gyntaf</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Chwith:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;De:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Pen:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>G&amp;waelod:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Rhif y Dudalen &amp;Gyntaf:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Uned &amp;Rhagosod:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>&amp;Fframiau Testun Ymysgogol</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Bwlch:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Colo&amp;fnau:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>Tu &amp;Mewn:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>Tu &amp;Allan:</translation>
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
        <translation>Tudalen Chwith</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Tudalen Dde</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Nodau</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Symud Nodau</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Symud Pwyntiau Rheoli</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Ychwanegu Nodau</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Dileu Nodau</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Ailosod Pwyntiau Rheoli</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Ailosod y Pwynt Rheoli yma</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation>Pan frithir hyn, defnyddio Cyfesurynnau sy&apos;n berthnasol i&apos;r Dudalen;
os arall, mae Cyfesurynnau yn berthnasol i&apos;r Gwrthrych.</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>Cyfesurynnau &amp;Absoliwt</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>Lleoliad &amp;X:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Lleoliad &amp;Y:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>Golygu Llinell A&amp;mlin</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>Ail&amp;osod Llinell Amlin</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Gorffen Golygu</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Symud Pwyntiau Rheoli yn Annibynnol</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Symud Pwyntiau Rheoli yn Gymesurol</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Agor Polygon neu Dorri Cromlin Bezier</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Cau&apos;r Cromlin Bezier yma</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Adlewyrchu&apos;r Llwybr yn Llorweddol</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Adlewyrchu&apos;r Llwybr yn Fertigol</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Croesrwygo&apos;r Llybr yn Llorweddol i&apos;r Chwith</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Croesrwygo&apos;r Llybr yn Fertigol i Fyny</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Croesrwygo&apos;r Llybr yn Fertigol i Lawr</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Cylchdroi&apos;r Llwybr yn Wrth-glocwedd</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Cylchdroi&apos;r Llwybr yn Glocwedd</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation>Lleihau Maint y Llwybr gan y % a ddangosir</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Cynyddu Maint y Llwybr gan y % a ddangosir</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Ongl Cylchdroi</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation>% i Gynyddu neu Leihau</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Gweithredoli Modd Golygu Llinellau Amlin</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Ailosod y Llinell Amlin i Siâp Gwreiddiol y Ffrâm</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Croesrwygo&apos;r Llybr yn Llorweddol i&apos;r Dde</translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation>Terfynell Sgript</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Export Range</source>
        <translation>Allforio Amrediad</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Dewisiadau Ffeil</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Ymyl Chwith</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Ymyl Dde</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Cyffredinol</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Caethiwo</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Wynebfathau ar Gael:</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation>Wynebfathau i&apos;w caethiwo:</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Tudalen</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effeithiau</translation>
    </message>
    <message>
        <source> sec</source>
        <translation>eil</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Dim Effaith</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Cysgodlenni</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Blwch</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Hydoddi</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Gwreichioni</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Hollti</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Disodli</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Llorweddol</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Fertigol</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Tu Mewn</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Tu Allan</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Chwith i Dde</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Pen i Waelod</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Gwaelod i Ben</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Dde i&apos;r Chwith</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Pen-Chwith i Waelod-De</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Cyfrineiriau</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Gosodiadau</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Sgrîn / Gwê</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Argraffydd</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Lliwiau Solet:</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Proffil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Bwriad Llunio:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Canfyddiadol</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Lliwfesurol Perthnasol</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Dirlawnder</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Lliwfesurol Absoliwt</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Delweddau:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Peidio â defnyddio proffilau ICC caeth</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>Bwriad Allbwn PDF/X-3</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Blwch Tocio</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Cadw fel</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation>Gosodiadau Delwedd</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Ymysgogol</translation>
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
        <translation>Dim</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Uchafswm</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Uchwl</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Canolig</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Isel</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Lleiafswm</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;Cyffredinol</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Wynebfathau</translation>
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
        <source>Export all pages to PDF</source>
        <translation>Allforio pob tudalen i PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Allforio amrediad o dudalennau i PDF</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation>Penodi&apos;r cytunedd PDF.  Y rhagososiad yw Acrobat 4.0, sy&apos;n rhoi&apos;r cytunedd lletaf.  Dewiswch Acrobat 
5.0 os oes gan eich ffeil nodweddion PDF 1.4 fel tryloywder, neu os oes angen cêl-ysgrifaeth 128-did.
Mae PDF/X-3 ar gyfer allforio&apos;r PDF ar gyfer argraffu masnachol, a gellir ei ddewis os weithredir rheoli lliwiau.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation>Penodi rhwymo&apos;r tudalennau yn y PDF.  Onibai i chi wybod
bod angen ei newid, gadewch y dewisiad rhagosod - Chwith.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation>Creu rhagolygon cryno o bob tudalen yn y PDF.
Gall rhai gwelyddion ddefnyddio&apos;r rhagolygon cryno i lywio.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Creu Erthyglau PDF, sydd o ddefnydd ar gyfer llywio drwy erthyglau cysylltiedig mewn PDF.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation>Caethiwo y tudnodau a greuwyd yn eich dogfen.
Maent o ddefnydd ar gyfer llywio drwy ddogfennau PDF hir.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation>Allforio cydraniad o destun a graffeg fector.
Nid yw hyn yn effeithio ar gydraniad delweddau didfap fel ffotos.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation>Cywasgiad testun a graffeg.
Onibai i chi fod efo rheswm da, gadewch hwn wedi&apos;i fritho.  Mae&apos;n lleihau maint y PDF.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation>Fersiwn cywasgiad ar gyfer delweddau.
Mae Ymysgogol yn gadael i Scribus ddewis y dull gorau.
Mae XIP yn dda ar gyfer delweddau efo lliwiau solet.
Mae JPEG yn well wrth greu ffeiliau PDF llai sydd efo llawer o ffotos (mae tipyn o golled ddelwedd yn bosibl).
Gadewch y gosodiad wrth Ymysgogol, onibai bod angen dewisiadau cywasgu arbennig.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation>Lefelau cywasgiad: Lleiaf (25%), Isel (50%), Canolig (75%), Uchel (85%), Uchaf (95%)</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation>Lawrsamplu eich delweddau didfap i&apos;r DPI penodol.
Bydd gadael hwn heb ei fritho yn eu llunio wrth eu cydraniad gwreiddiol.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (Dotiau/Modfedd) ar gyfer allforio delwedd.</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation>Caethiwo wynebfathau yn y PDF.  Bydd hyn yn 
amddiffyn haenlun a golwg eich dogfen.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation>Alluogi effeithiau cyflwyniad wrth ddefnyddion Acrobat Reader mewn modd sgrîn lawn.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Dangos rhagolygon tudalen ar gyfer pob tudalen a restrir uchod.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>Y maint o amser y dangosir y dudalen cyn i&apos;r cyflwyniad ddechrau ar y dudalen benodol.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Y maint o amser y bydd yr effaith yn rhedeg.
Bydd amser byrrach yn cyflymu&apos;r effaith, ac amser hirach yn ei arafu.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Math yr effaith arddangos.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Cyfeiriad y  llinellau symudol ar gyfer yr effeithiau hollti a cysgodlenni.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Lleoliad dechrau ar gyfer yr effeithiau blwch a hollti.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Cyfeiriad yr effeithiau disgleirio neu sychu.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Gweithredu&apos;r effaith penodol i bob tudalen.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Alluogi&apos;r nodweddion diogelwch yn eich PDF allforedig.
Os dewiswyd Acrobat 4.0, amddifynnir y PDF gan gêl-ysgrifaeth 40-did.
Os dewiswyd Acrobat 5.0, amddifynnir y PDF gan gêl-ysgrifaeth 128-did.
Ymwadiad: Nid yw cêl-ysgrifaeth PDF mor ddibynadwy â chêl-ysgrifaeth GPG neu PGP, ac mae ganddo rhai cyfyngiadau.
</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation>Dewis prif gyfrinair sy&apos;n alluogi neu analluogi yr holl
nodweddion diogelwch yn eich PDF allforedig</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation>Model lliw ar gyfer allbwn eich PDF.
Dewiswch Sgrîn/Gwê ar gyfer PDF a ddangosir ar sgrîn, neu a argraffir ar argraffyddion inkjet cyffredinol.
Dewiswch Argraffydd wrth argraffu i argraffydd CMYK 4-lliw go iawn.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Caethiwo proffil lliw ar gyfer lliwiau solet</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Proffil lliw ar gyfer lliwiau solet</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Bwriad llunio ar gyfer lliwiau solet</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Caethiwo proffil lliw ar gyfer delweddau</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Peidiwch â defnyddio proffilau lliw a gaethiwir yn nelweddau tarddiad</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Proffil lliw ar gyfer delweddau</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Bwriad llunio ar gyfer delweddau</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Proffil allbwn ar gyfer argraffu.  Os bosibl, gofynnwch i&apos;ch argraffydd am ei gyngor ynglyn â dewis proffil.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Llinyn gorfodol ar gyfer PDF/X-3, neu fydd y PDF yn methu
cydymffurfiad PDF/X-3.  Argymhellir defnyddio teitl y ddogfen.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Pellter ar gyfer ymdoddi o ben y dudalen gorfforol</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Pellter ar gyfer ymdoddi o waelod y dudalen gorfforol</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Pellter ar gyfer ymdoddi o chwith y dudalen gorfforol</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Pellter ar gyfer ymdoddi o dde y dudalen gorfforol</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Dewis cyfrinair i ddefnyddwyr allu darllen eich PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Caniatáu argraffu&apos;r PDF.  Os na frithir, atelir argraffu.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Caniatáu addasu&apos;r PDF.  Os na frithir, atelir addasu&apos;r PDF.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation>Caniatáu copïo testun a graffeg i&apos;r PDF.
Os na frithir, atelir copïo testun a graffeg.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation>Caniatáu ychwanegu anodiadau a meyseydd i&apos;r PDF.
Os na frithir, atelir golygu anodiadau a ffeiliau.</translation>
    </message>
    <message>
        <source>Create PDF File</source>
        <translation>Creu Ffeil PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>Allosod i &amp;Ffeil:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Newid...</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>&amp;Pob Tudalen</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>D&amp;ewis Tudalennau</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>Cy&amp;tunedd:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>&amp;Rhwymiad:</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Creu &amp;Rhagolygon Cryno</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Cadw Fframiau Testun &amp;Cysylltiedig fel Erthyglau PDF</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>Cynnwys &amp;Tudnodau</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Cydraniad:</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation>&amp;Dull:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Ansawdd:</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation>&amp;Lawr-enghreifftio Delweddau i:</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>&amp;Caethiwo pob Wynebfath</translation>
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
        <translation>Dangos Rhag&amp;olygon Tudalen</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>&amp;Parhad Dangosiad:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Parhad &amp;Effaith:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>Math E&amp;ffaith:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>Llinellau sy&apos;n &amp;Symud :</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>&amp;O&apos;r:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>Cyfei&amp;riad:</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>Gweithredu&apos;r &amp;Effaith ar Bob Tudalen</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>Defnyddio &amp;Cêl-ysgrifaeth</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Defnyddiwr:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>Perch&amp;ennog:</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>Caniatáu Ar&amp;graffu&apos;r Ddogfen</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Caniatáu &amp;Newid y Ddogfen</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Caniatáu Copïo &amp;Testun a Graffeg</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Caniatáu Ychwanegu &amp;Anodiadau a Meyseydd</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>&amp;Diogelwch</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Allbwn ar &amp;Gyfer:</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Defnyddio Gosodiadau Llunio wedi&apos;u Addasu</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Gosodiadau Llunio</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>A&amp;mledd:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Ongl:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>&amp;Swyddogaeth Smotyn:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Dot Syml</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Llinell</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Crwn</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Hirgylch</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Defnyddio Proffil ICC</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Lliw</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Llinyn Gwybodaeth:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>&amp;Proffil Allbwn:</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Cadw</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Mewnosod rhestr wedi&apos;i gwahanu gan atalnodau o docynnau, lle
gall tocyn fod yn * ar gyfer pob tudalen, 1-5 ar gyfer
amrediad tudalennau, neu rhif tudalen unigol.</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Ffeiliau-PDF (*.pdf);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Dyma osodiad uwch nas alluogir yn rhagosodol.  Dylid ei alluogi pan gwneir cais gan eich 
cwmni argraffu yn unig, ac nid ydynt wedi rhoi i chi y manylion llawn sy&apos;n hanfodol.  Os arall,
gall ddigwydd bod eich PDF allforiedig yn peidio ag argraffu yn iawn, neu beidio â bod yn wir
gludadwy dros gysodau.</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">C&amp;ylchdro:</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Rhagolwg Argraffu</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Popeth</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation>Rhoi golwg ddeliach o eitemau testun yn y gwelydd, ar draul
arafu&apos;r rhagolygu rywfaint.  Mae hyn yn effeithio ar wynebfathau Type 1 yn unig</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of True Type Fonts, Open Type Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Rhoi golwg ddeliach o wynebfathau True Type ac Open Type, EPS, PDF a 
graffeg fector yn y rhagolwg, ar draul arafu&apos;r rhagolygu rywfaint</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Dangos tryloywder ac eitemau tryloyw yn eich dogfen.  Angen Ghostscript 7.07 neu hwyrach</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Dangos rhagolwg argraffu gan ddefnyddio efelychiadau o inciau CMYK cyffredinol, yn lle lliwiau RGB</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>(An)alluogi y plât inc C (Gwyrddlas)</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>(An)alluogi y plât inc M (Magenta)</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>(An)alluogi y plât inc Y (Melyn)</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>(An)alluogi y plât inc K (Du)</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>Gwrthamgenu &amp;Testun</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Gwrthamgenu &amp;Graffeg</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Dangos &amp;Tryloywder</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>Dangos CMY&amp;K</translation>
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
        <translation>Gwaredu &amp;Dan-liw</translation>
    </message>
    <message>
        <source>A way of switching some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Ffordd o atal rhai o&apos;r arlliwiau llwyd a gyfansoddir o wyrddlas, melyn 
a magenta, a defnyddio du yn eu lle.  Mae UCR efo&apos;r effaith mwyaf ar 
ddarnau delweddau sy&apos;n amhendant a/neu sy&apos;n cynnwys graddliwiau tywyll
sy&apos;n agosaf i&apos;r llwyd.  Gall defnyddio hyn wella argraffu ar rai delweddau,
ond mae angen tipyn o brofi ac arbrofi yn ôl y galw.  Mae UCR yn lleihau&apos;r 
bosibilrwydd o or-ddirlawnder efo inciau CMY.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished">Ffordd o atal rhai o&apos;r arlliwiau llwyd a gyfansoddir o wyrddlas, melyn 
a magenta, a defnyddio du yn eu lle.  Mae UCR efo&apos;r effaith mwyaf ar 
ddarnau delweddau sy&apos;n amhendant a/neu sy&apos;n cynnwys graddliwiau tywyll
sy&apos;n agosaf i&apos;r llwyd.  Gall defnyddio hyn wella argraffu ar rai delweddau,
ond mae angen tipyn o brofi ac arbrofi yn ôl y galw.  Mae UCR yn lleihau&apos;r 
bosibilrwydd o or-ddirlawnder efo inciau CMY.</translation>
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
        <translation>Copïo Yma</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Symud Yma</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Diddymu</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Llun</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Feil:</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Testun Cysylltiedig</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Ffrâm Testun</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Testun ar Lwybr</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Paragraffau:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Geiriau:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Nodau:</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Argraffu:</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Alluogir</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Analluogir</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Golygu Testun...</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Dim</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation>Mae&apos;r rhaglen</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation>ar goll!</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Copi o</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Gludo</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Dangos &amp;Ymylon</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Dangos &amp;Fframiau</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Dangos &amp;Delweddau</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Dangos &amp;Grid</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Dangos &amp;Cyfeiryddion</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Dangos Grid G&amp;waelodlin</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>N&amp;eidio at y Grid</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Ne&amp;idio at y Cyfeiryddion</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>PPI gwreiddiol:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>PPI gwir:</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>Gwy&amp;bodaeth</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation>&amp;Nôl Llun...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>Delwedd yn &amp;Weladwy</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation>Diweddaru &amp;Llun</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation>Golygu &amp;Llun</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation>Addasu &amp;Ffrâm i Lun</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>Nôl T&amp;estun...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation>Atodi &amp;Testun...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation>Golygu &amp;Testun...</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation>Mewnosod Testun &amp;Enghreifftiol</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Yn &amp;Dudnod PDF</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Yn A&amp;nodiad PDF</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>&amp;Priodweddau Anodiad</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>Priodweddau &amp;Meysydd</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;Dewisiadau PDF</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>Cl&amp;oi</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>&amp;Dadgloi</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation>Cloi Maint y Gw&amp;rthrych</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation>D&amp;adgloi Maint y Gwrthrych</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>An&amp;fon i Lyfr Lloffion</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Anfon i &amp;Haen</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>Cas&amp;glu</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation>Dadga&amp;sglu</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>Le&amp;fel</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>Anfon i&apos;r C&amp;efn</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation>Anfon i&apos;r B&amp;laen</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Iselhau</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>C&amp;odi</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation>Ff&amp;râm Llun</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation>Pol&amp;ygon</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation>&amp;Amlinellau</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>Ffrâm &amp;Testun</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>Cromlin &amp;Bezier</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>&amp;Trosi i</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>To&amp;rri</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copïo</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Dileu</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>&amp;Gwaredu Cynnwys</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation>&amp;Dangos Priodweddau...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation>&amp;Cuddio Priodweddau...</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Ydych wir eisiau gwaredu eich holl Testun?</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Delwedd</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Testun</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Llinell</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polygon</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polylinell</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Testun Llwybr</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation>Tudalen</translation>
    </message>
    <message>
        <source> of %1</source>
        <translation>o %1</translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Result</source>
        <translation>Canlyniad</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Chwilio Canlyniadau am:</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Rhagolwg</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Dewis</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Diddymu</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Pictures</source>
        <translation>Lluniau</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Ewch i</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ydw</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Iawn</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Ar goll</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Chwilio</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Enw</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Llwybr</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Tudalen</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Argraffu</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Cyflwr</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Priodweddau Polygon</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Co&amp;rneli:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>C&amp;ylchdro:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>&amp;Ffactor:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Rhif corneli ar gyfer polygonau</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Graddau cylchdroi ar gyfer polygonau</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Polygon enghreifftiol</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Hoffterau</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Cyffredinol</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dogfen</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Cyfeiryddion</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Argraffwaith</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Offer</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Llyfr Lloffion</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Dangos</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>Rhyngwyneb Graffegol (GUI)</translation>
    </message>
    <message>
        <source>Units</source>
        <translation>Unedau</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation>Pwyntiau (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Milimedrau (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Modfeddi (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picau (p)</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Dewislenni</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Llwybrau</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Maint y Dudalen</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Addasiedig</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Darlun</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Tirlun</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Cyfeiryddion Ymylon</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Hunan-gadw</translation>
    </message>
    <message>
        <source>min</source>
        <translation>mun</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation>Haenlun Grid</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation>Lliwiau&apos;r Grid</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation>Lleoli</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Isysgrif</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Uwchysgrif</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Priflythrennau Bach</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Arall</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Ddydd Sul, bûm i&apos;n caniatáu i Jac, y gŵr o Fôn, gopïo llun y tŷ haf</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Dim</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Dewisiadau Eraill</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Rhagolwg</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>Bach</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Canolig</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation>I addasu&apos;r dangosiad llusgwch y rheolydd isod efo&apos;r Llithrydd.</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Dewis Cyfeiriadur</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>mod</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Offer allanol</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation>Amrywiol.</translation>
    </message>
    <message>
        <source> px</source>
        <translation>px</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation>Grid Gwaelodlin</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation>Dehonglydd Postscript</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Erfyn Prosesu Delwedd</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Argraffu</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation>Dewis yr addurniad ffenestri a golwg rhagosod.
Mae Scribus yn etifeddu unrhyw themau KDE neu Qt sydd ar gael</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Maint yr wynebfathau rhagosod ar gyfer y dewislenni a&apos;r ffenestri</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Uned mesur rhagosod ar gyfer golygu dogfennau</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Y nifer o linellau y bydd Scribus yn sgrolio am bob symudiad o&apos;r olwyn lygoden</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Radiws y cylch lle bydd Scribus yn gadael i chi gipio ar dolenni gwrthrych</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Y nifer o ddogfenni a olygwyd yn ddiweddar i ddangos yn y ddewislen Ffeil</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Cyfeiriadur rhagosod dogfenni</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation>Cyfeiriad rhagosod proffilau ICC</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Cyfeiriad rhagosod sgriptiau Scripter</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Maint rhagosod y dudalen, un ai maint arferol neu faint addasiedig</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Cyfeiriadaeth rhagosod tudalennau&apos;r ddogfen</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Lled tudalennau&apos;r ddogfen, a ellir ei olygu os ydych wedi dewis maint dudalen addasiedig</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Uchder tudalennau&apos;r ddogfen, a ellir ei olygu os ydych wedi dewis maint dudalen addasiedig</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Alluogi haenlun sengl neu seiliedig ar daeniad</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation>Gwneud y dudalen gyntaf yn dudalen chwith y ddogfen</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Pellter rhwng y cyfeirydd ymyl pen ac ochr y dudalen</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Pellter rhwng y cyfeirydd ymyl gwaelod ac ochr y dudalen</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Pellter rhwng y cyfeirydd ymyl chwith ac ochr y dudalen.
Os dewisir Tudalennau Wynebu, gellir defnyddio&apos;r lle ymylon yma i gael yr ymylon cywir ar gyfer rhwymo</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Pellter rhwng y cyfeirydd ymyl de ac ochr y dudalen.
Os dewisir Tudalennau Wynebu, gellir defnyddio&apos;r lle ymylon yma i gael yr ymylon cywir ar gyfer rhwymo</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation>Pan alluogir, mae Scribus yn cadw copi wrth gefn o&apos;ch ffeil efo&apos;r estyniad .bak
bob tro mae&apos;r cyfnod yn gorffen</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Cyfnod rhwng cadw yn ymysgogol</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Pellter rhwng llinellau lleiaf y grid</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Pellter rhwng llinellau mwyaf y grid</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Pellter y bydd gwrthrcyh yn neidio i&apos;ch cyfeiryddion tu mewn iddo</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Lliw llinellau lleiaf y grid</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Lliw llinellau mwyaf y grid</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Lliw y llinellau cyfeirydd a fewnosodir</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation>Gosod y grid tu ôl eich gwrthrychau tudalen</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation>Gosod y grid o flaen eich gwrthrychau tudalen</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation>Dangos y grid gwaelod</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation>Cuddio&apos;r grid gwaelod</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Afleoliad dros waelodlin yr wynebfath ar linell</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Maint perthynol yr uwchysgrif wrth gymharu â&apos;r wynebfath arferol</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Afleoliad dan waelodlin yr wynebfath arferol ar linell</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Maint perthynol yr isysgrif wrth gymharu â&apos;r wynebfath arferol</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Maint perthynol wynebfath y priflythrennau bach wrth gymharu â&apos;r wynebfath arferol</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Cynydd canrannol dros faint yr wynebfath ar gyfer y bylchu llinell</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Priodweddau Ffrâm Testun</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Priodweddau Ffrâm Llun</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Priodweddau Lluniadu Siapau</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Rhagosodiadau Lefel Chwyddo</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Priodweddau Lluniadu Llinellau</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Priodweddau Lluniadu Polygonau</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Wynebfath ar gyfer fframiau testun newydd</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Maint yr wynebfath ar gyfer fframiau testun newydd</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Lliw&apos;r wynebfath</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Nifer o golofnau mewn ffrâm testun</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Bwlch rhwng colofnau ffrâm testun</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Enghraifft o&apos;ch wynebfath</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Mae fframiau llun yn caniatáu graddio lluniau i unrhyw faint</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Graddio llorweddol o ddelweddau</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Graddio fertigol o ddelweddau</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Cadw graddio llorweddol a fertigol yn hafal</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Graddir lluniau mewn fframiau llun i faint y ffrâm</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Mae lluniau a raddir yn ymysgogol yn cadw eu cyfrannedd wreiddiol</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Lliw llenwad fframiau llun</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Dirlawnder y lliw llenwad</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Lliw llinell siapau</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Dirlawnder y lliw llinellau</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Lliw llenwad siapau</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Arddull llinell siapau</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Lled llinell siapau</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Chwyddo lleiaf a ganiateir</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Chwyddo mwyaf a ganiateir</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Newid y chwyddo ar gyfer pob gweithrediad chwyddo</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Lliw llinellau</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Dirlawnder lliw</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Arddull llinellau</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Lled Llinellau</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Rhif corneli ar gyfer polygonau</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Graddau cylchdroi ar gyfer polygonau</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Polygon enghreifftiol</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Dewis maint y rhagolwg yn y palet llyfr lloffion</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Cadw cynnwys y llyfr lloffion bob amser ar ôl newid</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation>Pan ddefyddio tudalennau wynebu, dangos y ddwy dudalen ochr wrth ochr</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Lliw y papur</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Lliw y llinellau ymyl</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Mygydu&apos;r lle tu allan yr ymylon efo lliw yr ymylon</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation>Alluogi nodweddion tryloywder wrth allforio i PDF 1.4</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Gosod y lefel chwyddo rhagosodiadol</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation>Lleoliad y dehonglydd Ghostscript yn y cysawd ffeiliau</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Gwrthamgenu testun ar gyfer llunio ar-sgrin EPS a PDF</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Gwrthamgenu graffeg ar gyfer llunio ar-sgrin EPS a PDF</translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation>Lleoliad y golygydd graffegau yn y cysawd ffeiliau</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Peidio â dangos gwrthrychau tu allan yr ymylon ar y dudalen argraffedig neu yn y ffeil a allforir</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Thema:</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Maint &amp;Wynebfath:</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation>Gosodiadau Llygoden</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>Naid &amp;Olwyn:</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation>&amp;Radiws Cipio:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>D&amp;ogfennau Diweddar:</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Dogfennau:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Newid...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;Proffilau ICC:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Ne&amp;wid...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Sgriptiau:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>New&amp;id...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Maint:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>C&amp;yfeiriadaeth:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Lle&amp;d:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Taldra:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>Tudalennau &amp;Wynebu</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Tudalen &amp;Chwith yn Gyntaf</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Gwaelod:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Pen:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;De:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Chwith:</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation>&amp;Alluogir</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Ysbaid:</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation>B&amp;ylchu Grid Lleiaf:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation>By&amp;lchu&apos;r Prif Grid:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation>Pellter S&amp;nap y Cyfeirydd:</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation>Lliw&apos;r Grid &amp;Lleiaf:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation>Lliw&apos;r P&amp;rif Grid:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation>Lliw C&amp;yfeiryddion Defnyddiwr:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation>Lliw&apos;r Grid G&amp;waelodlin:</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation>Yn y &amp;Cefndir</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation>Yn y &amp;Blaendir</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation>A&amp;rnodd</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation>I &amp;Ffwrdd</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>Dadle&amp;oliad:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>G&amp;raddio:</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>Dadle&amp;oliad:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>Gr&amp;addio:</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Gra&amp;ddio:</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>Grid Gwaelod&amp;lin:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>A&amp;tred Gwaelodlin:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation>&amp;Bylchu Llinellau Ymysgogol:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation>&amp;Wynebfath Rhagosod:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation>&amp;Maint Rhagosod:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation>Lliw &amp;Testun:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Colo&amp;fnau:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Bwlch:</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>Lliw&apos;r Ll&amp;inell:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Arlliwio:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>Lliw &amp;Llenwad:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>Arlli&amp;wio:</translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation>Mat&amp;h o Linell:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Ll&amp;ed y Llinell:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>&amp;Lleiafswm:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>&amp;Uchafswm:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>&amp;Camu:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Graddfa &amp;Rydd</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>Graddio &amp;Llorweddol:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>Graddio &amp;Fertigol:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>Graddio Llun i &amp;Faint y Ffrâm</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Cadw &amp;Cymhareb Agwedd</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Lliw Ll&amp;enwad:</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Co&amp;rneli:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>C&amp;ylchdro:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>&amp;Ffactor:</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>C&amp;adw Cynnwys ar ôl Newidiadau</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>Mawr</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation>Dangos Tudalennau &amp;Ochr wrth Ochr</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation>Lliwiau Tudalen</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation>&amp;Cefndir:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation>&amp;Ymylon:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Dangos yr Ardal &amp;Anargraffadwy yn Lliw&apos;r Ymyl</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation>Defnyddio &amp;Nodweddion Tryloywder PDF-1.4</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>Addasu Maint y &amp;Dangosiad</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Enw&apos;r Gweithredadwyn:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Gwrthamgenu &amp;Testun</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Gwrthamgenu &amp;Graffeg</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Enw&apos;r G&amp;weithredadwyn:</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation>Tocio i &amp;Ochrau&apos;r Dudalen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>Tu &amp;Mewn:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>Tu &amp;Allan:</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Gweithredoli Gwaredu &amp;Dan-liw</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="unfinished">&amp;Newid...</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished">Ffordd o atal rhai o&apos;r arlliwiau llwyd a gyfansoddir o wyrddlas, melyn 
a magenta, a defnyddio du yn eu lle.  Mae UCR efo&apos;r effaith mwyaf ar 
ddarnau delweddau sy&apos;n amhendant a/neu sy&apos;n cynnwys graddliwiau tywyll
sy&apos;n agosaf i&apos;r llwyd.  Gall defnyddio hyn wella argraffu ar rai delweddau,
ond mae angen tipyn o brofi ac arbrofi yn ôl y galw.  Mae UCR yn lleihau&apos;r 
bosibilrwydd o or-ddirlawnder efo inciau CMY.</translation>
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
        <translation type="unfinished">Cyfreithiol</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="unfinished">Llythyr</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="unfinished">Tabloid</translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>&amp;Arlliw:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Dirlawnder:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>&amp;Gwerth:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Coch:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Gwyrdd:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>G&amp;las:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>Sianel A&amp;lffa:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>Lliwiau &amp;sylfaenol</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>Lliwiau adda&amp;siedig</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>Diffinio &amp;Lliwiau Addasiedig &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Iawn</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Diddymu</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Ychwanegu i&apos;r Lliwiau Addasiedig</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Dewis lliw</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Copïo neu Symud Ffeil</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Darllen: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Ysgrifennu: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>En&amp;w&apos;r ffeil:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>Math y &amp;ffeil:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Un cyfeiriadur i fyny</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Diddymu</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Enw</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Maint</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Math</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Dyddiad</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Priodweddau</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Iawn</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Edrych &amp;yn:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Yn ôl</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Creu Plygell Newydd</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Golwg Rhestr</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Golwg Manwl</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Rhagolygu Gwybodaeth y Ffeil</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Rhagolygu Cynnwys y Ffeil</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Darllen-ysgrifennu</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Darllen-yn-unig</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Ysgrifennu-yn-unig</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Anhygyrch</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Cyswllt Symbolaidd i Ffeil</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Cyswllt Symbolaidd i Gyfeiriadur</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Cyswllt Symbolaidd i Arbennig</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Ffeil</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Cyfeiriadur</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Arbennig</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Agor</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Cadw Fel</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Agor</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Cadw</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>Ail-&amp;enwi</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Dileu</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>Ail-lwy&amp;tho</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Trefnu gan &amp;Enw</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Trefnu gan &amp;Faint</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Trefnu gan &amp;Ddyddiad</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Heb eu Trefnu</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Trefnu</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Dangos &amp;ffeiliau cudd</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>y ffeil</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>y cyfeiriadur</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>y cyswllt symbolaidd</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Dileu %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Ydych wir eisiau dileu %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ydw</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nac ydw</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Plygell Newydd 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Plygell Newydd</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Plygell Newydd %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Canfod Cyfeiriadur</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Cyfeiriaduron</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Cadw</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Gwall</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Ni chanfuwyd y ffeil.
Gwiriwch y llwybr ac enw&apos;r ffeil.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Dewis Cyfeiriadur</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Cyfeiriadur:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation type="unfinished">&amp;Wynebfath</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation type="unfinished">Ar&amp;ddull wynebfath</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="unfinished">Effeithiau</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation type="unfinished">Trwyli&amp;nellu</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation type="unfinished">&amp;Tanlinellu</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation type="unfinished">&amp;Lliw</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation type="unfinished">Enghraifft</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation type="unfinished">Sgr&amp;ipt</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">Iawn</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation type="unfinished">Gweithredu</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Diddymu</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="unfinished">Cau</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation type="unfinished">Dewis Wynebfath</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Gwaredu</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Dewis Popeth</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Dadwneud</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Ail-wneud</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>To&amp;rri</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copïo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Gludo</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Alinio</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Addasu...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Ynglyn â Qt&lt;/h3&gt;&lt;p&gt;Mae&apos;r rhaglen hon yn defnyddio Qt fersiwn %1.&lt;/p&gt;&lt;p&gt;Blwch offer ar gyfer datblygu rhyngwynebau graffegol &amp;amp; cymhwysiadau aml-blatfform yw Qt.&lt;/p&gt;&lt;p&gt;Mae Qt yn gadael i chi ddefnyddio&apos;r un goeden o gôd dros MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, a phob amrywiad pwysig o Unix masnachol.&lt;br&gt;Mae Qt ar gael hefyd am ddyfeisiau caeth.&lt;/p&gt;&lt;p&gt;Cynnyrch Trolltech yw Qt. Gweler &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; am fwy o wybodaeth.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Initializing...</source>
        <translation>Ymgychwyn...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dogfen</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Cefndir</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Ydych wir eisiau ysgrifennu dros y Ffeil: 
%1?</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation>Rhagolwg Argraffu</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Cyfeireb Arlein</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Agor</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Sgriptiau Python (*.py);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Cadw fel</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>Delweddau SVG (*.svg *.svgz);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>Delweddau SVG (*.svg);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ydw</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nac ydw</translation>
    </message>
    <message>
        <source>Oook! You&apos;re calling an object doesn&apos;t exist!</source>
        <translation type="obsolete">Ew!  Rydych yn galw gwrthrych sy ddim yn bodoli!</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to erase an object doesn&apos;t exist!</source>
        <translation type="obsolete">Ew!  Rydych yn ceisio dileu gwrthrych sy ddim yn bodoli!</translation>
    </message>
    <message>
        <source>Oook! An object you&apos;re trying to textflow doesn&apos;t exist!</source>
        <translation type="obsolete">Ew!  Nid yw&apos;r gwrthrych yr ydych yn ceisio llifo i mewn i&apos;r testun yn bodoli!</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation>S&amp;gript</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation>Rhagolwg &amp;Wynebfathau</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation>Mewnosod &amp;Arbennig</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Newydd o &amp;Batrymlun...</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Cylchlythyrau</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Pamffledi</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Catalogau</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Taflenni</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Arwyddion</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Cerdiau</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Penawdau Llythyrau</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Amlenni</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Cerdiau Busnes</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Calendrau</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Hybysebion</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Labeli</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Dewislenni</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Rhaglenni</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>Ffurflenni PDF</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>Cyflwyniadau PDF</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Cylchgronau</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Posteri</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Hybysiadau</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Dogfenni Testun</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Plygiadau</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Patrymluniau eich hunain</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Cadw fel &amp;Delwedd...</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Cadw fel Delwedd</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation>Gwall wrth ysgrifennu&apos;r ffeil(iau) allbwn.</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Gwall wrth ysgrifennu&apos;r ffeil(iau) allbwn.</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Wedi allforio yn llwyddiannus.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Mae&apos;r ffeil yn bodoli.  Trosysgrifo?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>mewn bod eisoes.  Trosysgrifo?</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Iawn i bopeth</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>Rhag&amp;olwg Argraffu</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Mewnforio &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Pob Fformat a Gynhelir (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Cadw fel &amp;Patrymlun...</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation>Llawlyfr S&amp;griptio...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>Sgriptiau S&amp;cribus</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Gweithredu Sgript...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Sgriptiau Diweddar</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>&amp;Dangos Terfynell</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Cadw Tudalen fel &amp;SVG...</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Mewnforio &amp;SVG...</translation>
    </message>
    <message>
        <source>Oook! Wrong arguments! Call: </source>
        <translation type="obsolete">Ew!  Ymresymiadau anghywir!  Galwad:</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to load image into an object doesn&apos;t exist or isn&apos;t selected!</source>
        <translation type="obsolete">Ew! Rydych yn ceisio llwytho delwedd i mewn i wrthrych sy ddim yn bodoli neu nas dewisir!</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to (un)lock an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Ew! Rydych yn ceisio cloi/dadgloi gwrthrych sy ddim yn bodoli!  Ni ddewisir un chwaith.</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to query an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Ew! Rydych yn ceisio ymholi gwrthrych sy ddim yn bodoli!  Ni ddewisir un chwaith.</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Mewnforio testun</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Pob Fformat a Gynhelir</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>Ffeiliau HTML</translation>
    </message>
    <message>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Ffeiliau Testun</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Template: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Catalaneg</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Czech</source>
        <translation type="unfinished">Tsieceg</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation type="unfinished">Daneg</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation type="unfinished">Iseldireg</translation>
    </message>
    <message>
        <source>English</source>
        <translation type="unfinished">Saesneg</translation>
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
        <translation type="unfinished">Almaeneg</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation type="unfinished">Ffinneg</translation>
    </message>
    <message>
        <source>French</source>
        <translation type="unfinished">Ffrangeg</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="unfinished">Groeg</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation type="unfinished">Hwngareg</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Italian</source>
        <translation type="unfinished">Eidaleg</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="unfinished">Lithwaneg</translation>
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
        <translation type="unfinished">Pwyleg</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation type="unfinished">Rwsieg</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="unfinished">Swedeg</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation type="unfinished">Sbaeneg</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation type="unfinished">Slofaceg</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="unfinished">Slofeneg</translation>
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
        <source>&amp;About Script...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>About Script</source>
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
        <source>Insert index out of bounds</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set text alignment on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set font size on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set font on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font not found</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t line spacing on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t column gap on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t number of columns on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t select text in a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t delete text from a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set text fill on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set text stroke on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set text shade on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can only link text frames</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target frame must be empty</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target frame links to another frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Source and target are the same object</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t unlink a non-text frame</source>
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
        <source>Can&apos;t convert a non-text frame to outlines</source>
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
        <source>Can&apos;t scale by 0%</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t render an empty sample</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t save to a blank filename</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t have an empty layer name</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Layer not found</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t remove the last layer</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t create layer without a name</source>
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
        <source>Color not found</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found in document</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found in default colors</source>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Portugese (Brazilian)</source>
        <translation type="unfinished"></translation>
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
        <source>Specified item not an image frame</source>
        <comment>python error</comment>
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
        <source>Display an interactive Python console where you can write and run Python programs that use the Scripter tools.</source>
        <comment>scripter</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scripter &amp;Settings</source>
        <comment>script menu</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Gwaredu</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Dewis Popeth</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Dadwneud</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Ail-wneud</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>To&amp;rri</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copïo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Gludo</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Dewislen Cysawd</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Rholio</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Dadrolio</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Normaleiddio</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Lleihau</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Ehangu</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Cau</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Adfer</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Symud</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Maint</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Llei&amp;hau</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ehan&amp;gu</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cau</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Bob amser ar y &amp;Pen</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Lleihau</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Adfer i Lawr</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Cau</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>&amp;Rholio</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>&amp;Dadrolio</translation>
    </message>
</context>
<context>
    <name>Query</name>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Document Setup</source>
        <translation>Gosodiadau&apos;r Ddogfen</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Cyfeiryddion Ymylon</translation>
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
        <translation>mod</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Alluogi haenlun sengl neu seiliedig ar daeniad</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Gwneud y dudalen gyntaf yn dudalen chwith y ddogfen</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Pellter rhwng y cyfeirydd ymyl pen ac ochr y dudalen</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Pellter rhwng y cyfeirydd ymyl gwaelod ac ochr y dudalen</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Pellter rhwng y cyfeirydd ymyl chwith ac ochr y dudalen.
Os dewisir Tudalennau Wynebu, gellir defnyddio&apos;r lle ymylon yma i gael yr ymylon cywir ar gyfer rhwymo</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Pellter rhwng y cyfeirydd ymyl de ac ochr y dudalen.
Os dewisir Tudalennau Wynebu, gellir defnyddio&apos;r lle ymylon yma i gael yr ymylon cywir ar gyfer rhwymo</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Pen:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Chwith:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>G&amp;waelod:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;De:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>Tudalennau &amp;Wynebu</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Tudalen &amp;Chwith yn Gyntaf</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>Tu &amp;Mewn:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation>Tu &amp;Allan:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Maint y Dudalen</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="unfinished">Maint:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Addasiedig</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="unfinished">Cyfeiriadaeth:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Darlun</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Tirlun</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Lled:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="unfinished">Taldra:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="unfinished">Rhif y Dudalen &amp;Gyntaf:</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style of current paragraph</source>
        <translation>Arddull y paragraff cyfredol</translation>
    </message>
    <message>
        <source>Style Settings</source>
        <translation>Gosodiadau Arddull</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>None</source>
        <translation>Dim</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Lliw llenwad y testun</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Dirlawnder lliw llenwad y testun</translation>
    </message>
    <message>
        <source>Fill Color Settings</source>
        <translation>Gosodiadau Lliw Llenwad</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>None</source>
        <translation>Dim</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Lliw trawiad y testun</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Dirlawnder lliw trawiad y testun</translation>
    </message>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Gosodiadau Lliw Trawiad</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
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
        <translation>Wynebfath y testun penodol</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Maint yr Wynebfath</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Lled graddio nodau</translation>
    </message>
    <message>
        <source>Font Settings</source>
        <translation>Gosodiadau Wynebfath</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Kerning:</source>
        <translation>Gorgyffyrddiad:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation>Gorgyffyrddiad â Llaw</translation>
    </message>
    <message>
        <source>Character Settings</source>
        <translation>Gosodiadau Nodau</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copi #%1 o</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Cefndir</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>File</source>
        <translation>Ffeil</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Chwilio am Wynebfathau</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation>Nid oes wynebfathau PostScript ar eich cysawd</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation>Terfynu rwan</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Gwall Angheuol</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation>Cysylltnod Deallus</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation>Alinio i&apos;r Chwith</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation>Alinio i&apos;r Dde</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation>Alinio i&apos;r Canol</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Mewnosod Rhif Tudalen</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation>Atodi Testun i&apos;r Llwybr</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation>Dangos Haenau</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation>Sgriptiau Java...</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation>Dadwneud</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation>Dangos Palet Tudalen</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation>Cloi/Datgloi</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation>Gofodnod Di-dor</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Darllen Hoffterau</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation>Ymgychwyn Cysylltnodydd</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Gosod Byrlwybrau</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Darllen Llyfr Lloffion</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Ymgychwyn Ategion</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Newydd</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation>Agor...</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Cau</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Cadw</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation>Cadw fel...</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation>Nôl Testun/Llun...</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation>Gwybodaeth am y Ddogfen...</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation>Gosodiadau&apos;r Ddogfen...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Argraffu...</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Terfynu</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Torri</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Copïo</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Gludo</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Gwaredu</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation>Dewis popeth</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation>Lliwiau...</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation>Arddulliau...</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation>Patrymluniau...</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation>Wynebfathau...</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation>Dewis Wynebfath Newydd</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation>Dyblygu</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Dyblygiad Lluosol</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Dileu</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Casglu</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation>Datgasglu</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Cloi</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation>Anfon i&apos;r Cefn</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation>Anfon i&apos;r Blaen</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation>Iselhau</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation>Codi</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation>Dosbarthu/Alinio...</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation>Mewnosod...</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation>Dileu...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation>Symud...</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation>Gweithredu Patrymlun...</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation>Trefnu Cyfeiryddion...</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation>Ffitio yn y Ffenest</translation>
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
        <translation>Rhagolygon cryno</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation>Cuddio Ymylon</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation>Cuddio Fframiau</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation>Cuddio Delweddau</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation>Dangos Grid</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation>Neidio at y Grid</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Offer</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Priodweddau</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Amlinell</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Llyfr Lloffion</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Trefnu Lluniau</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation>Cysylltnodi Testun</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation>Ynglyn â Scribus</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Ynglyn â Qt</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation>Cymorth ar-lein...</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Arddull</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Arferol</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Tanlinellu</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Trwylinellu</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Priflythrennau Bach</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Uwchysgrif</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Isysgrif</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Amlinedig</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>Lleoliad X:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Lleoliad Y:</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Parod</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Dim</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation>Nôl Llun...</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Lliw</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Gwrthdroi</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Nôl Testun...</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Wynebfath</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Maint</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Arlliw</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Datgloi</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Agor</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dogfennau (*.sla *.sla.gz *.scd *.scd.gz);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dogfennau (*.sla *.scd);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Llwytho...</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Pob Fformat a Gynhelir</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Ffeiliau Testun (*.txt);;Pob Ffeil(*)</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation>Methu ysgrifennu&apos;r Ffeil:
 %1</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Iawn</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Cadw fel</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dogfennau (*.sla *.sla.gz *.scd *.scd.gz);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Cadw...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Argraffu...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dogfen</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Methodd argraffu!</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Llawlyfr Scribus</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation>Mae&apos;r rhaglenni dilynnol ar goll:</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Popeth</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation>Ffeiliau EPS (*.eps);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation>Llwytho:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Addasu Lliwiau</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Saesneg</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Almaeneg</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Sbaeneg</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Eidaleg</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Ffrangeg</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Rwsieg</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Daneg</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slofaceg</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Hwngareg</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tsieceg</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Iseldireg</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portiwgaleg</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation>Wcraineg</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Pwyleg</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Groeg</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Catalaneg</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Ffinneg</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation>Gwyddeleg</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Dewis Cyfeiriadur</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Chwalfa Scribus</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Chwalodd Scribus o achos Arwydd #%1</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation>Creu Dogfen newydd</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation>Agor Dogfen</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation>Cadw&apos;r Ddogfen gyfredol</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation>Cau&apos;r Ddogfen gyfredol</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation>Argraffu&apos;r Ddogfen gyfredol</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation>Cadw&apos;r Ddogfen gyfredol fel PDF</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Dangos Grid Gwaelodlin</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Ffeil</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Golygu</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Eitem</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Tudalen</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Golwg</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Offer</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>&amp;Ychwanegol</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>&amp;Ffenestri</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Cymorth</translation>
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
        <translation>mod</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation>Cuddio Grid Gwaelodlin</translation>
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
        <translation>mod</translation>
    </message>
    <message>
        <source>p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation>Cloir rhai Gwrthrychau.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Diddymu</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation>Cloi popeth</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation>Dadgloi popeth</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Lithwaneg</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Swedeg</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Slofeneg</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation>&amp;Rheoli Lliw...</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Newydd</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Agor...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Agor &amp;Diweddar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cau</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Cadw</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Cadw &amp;Fel...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>Dychwelyd i&apos;r Ffeil a &amp;Gadwyd</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Casglu ar gyfer &amp;Allbwn...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation>&amp;Nôl Testun/Llun...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Atodi &amp;Testun...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Mewnforio</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Cadw T&amp;estun...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Cadw Tudalen fel &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Cadw fel P&amp;DF...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Allforio</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>&amp;Gosodiadau&apos;r Ddogfen...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Argraffu...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Terfynu</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Dadwneud</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>To&amp;rri</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copïo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Gludo</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>G&amp;waredu</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Dewis &amp;Popeth</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Chwilio/Amnewid...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>Lli&amp;wiau...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>Arddulliau &amp;Paragraff...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>&amp;Arddulliau Llinell...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation>&amp;Patrymluniau...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation>Sgriptiau &amp;Java...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;yblygu</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>&amp;Dyblygiad Lluosol</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Dileu</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>Cas&amp;glu</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>Dadga&amp;sglu</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>Cl&amp;oi</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>Anfon i&apos;r C&amp;efn</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation>Anfon i&apos;r B&amp;laen</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Iselhau</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>C&amp;odi</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation>Dosbarthu/&amp;Alinio...</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Siâp</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>At&amp;odi Testun i&apos;r Llwybr</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>D&amp;atglymu&apos;r Testun o&apos;r Llwybr</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>Cyfuno &amp;Polygonau</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Hollti &amp;Polygonau</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation>&amp;Trosi i Amlinellau</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Mewnosod...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Dileu...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Symud...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation>Gwei&amp;thredu Patrymlun...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation>&amp;Ffitio yn y Ffenest</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation>&amp;100%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation>&amp;Rhagolygon cryno</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Dangos &amp;Grid</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Ne&amp;idio at y Cyfeiryddion</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Dangos Grid G&amp;waelodlin</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>Priod&amp;weddau</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation>&amp;Amlinell</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>Llyfr Ll&amp;offion</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>&amp;Haenau</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation>&amp;Palet Tudalen</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Tudnodau</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>Trefnu &amp;Lluniau</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>Cy&amp;sylltnodi Testun</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>Cynghorion &amp;Offer</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>Offer P&amp;DF</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation>Cynghorion Offer</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation>W&amp;ynebfathau...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation>C&amp;ysylltnodydd...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation>By&amp;rlwybrau Bysellfwrdd...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>Ynglyn â S&amp;cribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>Yn&amp;glyn â Qt</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>Llawlyfr &amp;Scribus...</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>&amp;Arddull</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Gosodiadau</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Chwith</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Canoli</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>&amp;De</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Bloc</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>G&amp;orfodedig</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Arall...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Rhaeadru</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Teilio</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Lliw</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation>Gwr&amp;thdroi</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>Nôl T&amp;estun...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Wynebfath</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Maint&amp;Maint</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Effeithiau</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Aliniad</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Arlliw</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tablyddion...</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>&amp;Dadgloi</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Dangos &amp;Delweddau</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Dangos &amp;Ymylon</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Dangos &amp;Fframiau</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Dangos &amp;Cyfeiryddion</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation>Ghostscript: Ni allwch ddefnyddio delweddau EPS</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation>M&amp;ewnforio Tudalen(nau)...</translation>
    </message>
    <message>
        <source>100%</source>
        <translation>100%</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>N&amp;eidio at y Grid</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>&amp;Hoffterau...</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Mewnforio Tudalennau...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Mewnforio Tudalen(nau)</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation>&lt;p&gt;Rydych yn ceisio mewnforio mwy o dudalennau na sydd ar gael yn y ddogfen gyfredol, yn cyfrif o&apos;r dudalen weithredol.&lt;/p&gt;Dewiswch un o&apos;r dilynol:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Creu&lt;/b&gt; tudalennau sydd ar goll&lt;/li&gt;&lt;li&gt;&lt;b&gt;mewnforio&lt;/b&gt; tudalennau tan y dudalen olaf&lt;/li&gt;&lt;li&gt;&lt;b&gt;Diddymu&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Creu</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Mewnforio</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Wedi gorffen mewnforio</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Canfuwyd dim byd i fewnforio</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation>Nôl Proffilau ICC</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>&amp;Trefnu Cyfeiryddion...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Maint:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Arlliw:</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation>&amp;Gwybodaeth Ddogfen...</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation>Dadwneud &amp;Dileu Gwrthrych</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation>Dadwneud &amp;Symud Gwrthrych</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation>Dadwneud &amp;Newid Gwrthrych</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation type="unfinished"></translation>
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
        <translation>Haen</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Popeth</translation>
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
        <translation>mod</translation>
    </message>
    <message>
        <source>p</source>
        <translation>p</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>Warning</source>
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Dogfen:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>gafodd ei newid ers y cadw diwethaf.</translation>
    </message>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation>Gadael Serch H&amp;ynny</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation>Cau Serch &amp;Hynny</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation>&amp;Cadw Rwan</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
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
        <translation type="unfinished">&amp;Diddymu</translation>
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
        <translation type="unfinished">Dewisiadau Uwch</translation>
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
        <translation>Dangos Rhagolygon Tudalen</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation>Dangos Enw Patrymlun</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Chwilio/Amnewid</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Chwilio am:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Testun</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Arddull Paragraff</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Wynebfath</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Maint yr Wynebfath</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Effeithiau Wynebfath</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Lliw Llenwad</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Arlliw Llenwad</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Lliw Trawiad</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Arlliw Trawiad</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Chwith</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Canol</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>De</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Bloc</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Gorfodedig</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Dim</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Amnewid efo:</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Chwiliad wedi&apos;i Orffen</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Iawn</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Gair Cyfan</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Anwybyddu Priflythrennau</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Chwilio</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Amnewid</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Amnewid &amp;Popeth</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cau</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">G&amp;waredu</translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Arrange Pages</source>
        <translation>Trefnu Tudalennau</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation>Patrymluniau ar Gael:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Tudalennau&apos;r Ddogfen:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation>Tudalennau Wynebu</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation>Tudalen Chwith yn gyntaf</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation>Llusgo Tudalennau neu Tudalennau Patrymlun i&apos;r Bin Sbwriel i&apos;w dileu.</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation>Dyma&apos;ch Patrymluniau i gyd; er mwyn creu Tudalen newydd
 llusgwch Patrymlun i&apos;r Golwg Tudalen isod.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Arferol</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation>Rhagolygu pob tudalen eich dogfen.</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Dewis Meysydd</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Meysydd ar Gael</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>Meysydd wedi&apos;u Dewis</translation>
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
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Arall...</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Arlliw</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Arlliw:</translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation type="unfinished">Dim Arddull</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Dim Arddull</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Golygu Ardulliau</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copi o %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Arddull Newydd</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nac ydw</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ydw</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Agor</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dogfennau (*.sla *.sla.gz *.scd *.scd.gz);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dogfennau (*.sla *.scd);;Pob Ffeil (*)</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Atodi</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Newydd</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Golygu</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Dyblygu</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Dileu</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Cadw</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Golygydd Stori</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation>Diweddaru Ffrâm Testun</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Ffeil</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Paragraff Cyfredol:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Geiriau:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Nodau:</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Cyfansymiau:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Paragraffau:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation>Ydych wir eisiau colli eich holl Newidiadau?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Ydych wir eisiau gwaredu eich holl Testun?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Agor</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Ffeiliau Testun (*.txt);;Pob Ffeil(*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Cadw fel</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Ydych eisiau cadw eich newidiadau?</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">Mewnosod &amp;Arbennig</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Newydd</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Ail-lwytho Testun o Ffrâm</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Cadw i Ffeil...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Llwytho Testun o Ffeil...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Cadw &amp;Dogfen</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>Di&amp;weddaru Ffrâm Testun a Terfynu</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>&amp;Terfynu Heb Ddiweddaru&apos;r Ffrâm Destun</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>To&amp;rri</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copïo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Gludo</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>G&amp;waredu</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation>Mewnosod &amp;Arbennig...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>Di&amp;weddaru Ffrâm Testun</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Ffeil</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Golygu</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Dewis &amp;Popeth</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>Golygu &amp;Arddulliau...</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation>Llwytho Testun o Ffeil</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>Cadw Testun i Ffeil</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation>Diweddaru Ffrâm Testun a Terfynu</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>Terfynu Heb Ddiweddaru&apos;r Ffrâm Destun</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>Ail-lwytho Testun o Ffrâm</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Chwilio/Amnewid...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>Rhagolwg &amp;Wynebfathau...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Cefndir...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Dangos Wynebfath...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Gosodiadau</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Chwilio/Amnewid</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">Rhagolwg &amp;Wynebfathau</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation>Gwaredu pob Testun</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation>Tanlinellu</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Priflythrennau Bach</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Isysgrif</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Uwchysgrif</translation>
    </message>
    <message>
        <source>Outline Text</source>
        <translation>Amlinellu Testun</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Trwylinellu</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>Update paragraph styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">Iawn</translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OpenOffice.org document and
never ask your confirmation again</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Pack paragraph styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Group paragraph styles by attributes.
Less paragraph styles but controlling them may be hard.
Should be used if it is known that text must not be edited
after importing.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Rheoli Tablyddion</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Chwith</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>De</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Atalnod Llawn</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Atalnod</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Canoli</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Dileu Popeth</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Mewnoliad ar gyfer llinell gyntaf y paragraff</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Mewnoliad o&apos;r chwith ar gyfer yr holl paragraff</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Dileu pob Tablydd</translation>
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
        <translation>mod</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Lleoliad:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation>Llinell &amp;Gyntaf:</translation>
    </message>
    <message>
        <source>Ind&amp;ent:</source>
        <translation type="obsolete">Mewn&amp;oli:</translation>
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
        <translation>Amlinell</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Elfen</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Math</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Gwybodaeth</translation>
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
        <translation>Wynebfath:</translation>
    </message>
    <message>
        <source>Image</source>
        <translation>Delwedd</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Testun</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Llinell</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polygon</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polylinell</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Testun Llwybr</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Tudalen</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Rhybudd</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Nid yw&apos;r enw &quot;%1&quot; yn unigryw.
 Dewiswch un arall.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Iawn</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Grŵp</translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Iawn</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">&amp;Diddymu</translation>
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
        <translation>Offer</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation>Dewis Eitemau</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Mewnosod Ffrâm Testun</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation>Mewnosod Llun</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Priodweddau...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation>Mewnosod Polygonau</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation>Mewnosod Llinellau</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation>Mewnosod Cromlinau Bezier</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Mewnosod Llinell Llaw Rydd</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Cylchdroi Eitem</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Golygu Cynnwys y Ffrâm</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Cysylltu Fframiau Testun</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Dadgysylltu Fframiau Testun</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Mewnosod Tabl</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Chwyddo neu bellhau</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Golygu&apos;r testun efo&apos;r Golygydd Stori</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation>Lluniadu amrywiol Siapau</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation>Gwneud mesuriadau</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>Button</source>
        <translation>Botwm</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Maes Testun</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Blwch Gwirio</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Blwch Cyfun</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Blwch Rhestr</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Testun</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Cyswllt</translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation>Offer PDF</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Mewnosod Meyseydd PDF</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Mewnosod Anodiadau PDF</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation>Dewis Nod:</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Mewnosod y nodau wrth y cyrchydd yn y testun</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Dileu&apos;r detholiad(au) cyfredol.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation>Cau&apos;r ymgom yma a dychwelyd i olygu testun.</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Mewnosod</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>G&amp;waredu</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cau</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Dewis y mewnforydd i ddefnyddio</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Ymysgogol</translation>
    </message>
    <message>
        <source>Get text only</source>
        <translation>Nôl y testun yn unig</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Mewnforio&apos;r testun heb unrhyw fformatio</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Mewnforydd:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Amgodiad:</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation type="unfinished">Dewis y mewnforydd i ddefnyddio</translation>
    </message>
    <message>
        <source></source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">Iawn</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Newydd o Batrymlun</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Popeth</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Enw</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Maint y Dudalen</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Lliwiau</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Disgrifiad</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Defnydd</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Creuir efo</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Awdur</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Gwaredu</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Agor</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Diddymu</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Lawrlwytho Patrymluniau</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Arsefydlu Patrymluniau</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Echdynnu&apos;r pecyn i gyfeiriadur patrymluniau - ~/.scribus/templates ar gyfer y defnyddiwr cyfredol, neu PREFIX/share/scribus/templates ar gyfer pob defnyddiwr ar y cysawd.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Paratoi patrymlun</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Gwaredu patrymlun</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Cyfieithu template.xml</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Gellir cael patrymluniau dogfen yn http://www.scribus.net/ yn yr adran Lawrlwythiadau.</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Sicrhewch y gellir defnyddio eich delweddau ac wynebfathau yn rhydd.  Os ni ellir rhannu wynebfathau, peidiwch â&apos;u casglu wrth gadw fel patrymlun.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Dylai creuwr y patrymlun sicrhau bod yr adran Arsefydlu Patrymluniau yn gweithredu i&apos;w patrymluniau hefyd.  Hynny yw, dylai defnyddiwr allu lawrlwytho pecyn patrymlun a&apos;i echdynnu i&apos;r cyfeiriadur patrymluniau a dechrau ei ddefnyddio.</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Bydd gwaredu patrymlun o&apos;r ymgom Newydd o Batrymlun yn gwaredu&apos;r cofnod o&apos;r template.xml yn unig - ni fydd yn dileu y ffeiliau dogfen.  Dangosir naidlen efo gwaredu os oes gennych freintiau ysgrifennu i&apos;r ffeil template.xml yn unig.</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Copïo template.xml (ffeil patrymlun) sydd yn bodoli eisoes i ffeil o&apos;r enw template.iaith_GWLAD.xml (defnyddiwch y côd iaith sydd yn y ffeil qm ar gyfer eich iaith), er enghraifft template.fi.xml ar gyfer template.xml yn yr iaith Fffinneg.  Rhaid i&apos;r gopi fod yn yr un cyfeiriadur a&apos;r template.xml gwreiddiol, i Scribus gael ei lwytho.</translation>
    </message>
    <message>
        <source>Date</source>
        <translation type="unfinished">Dyddiad</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Cadw fel Patrymlun</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Enw</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Categori</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Maint y Dudalen</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Lliwiau</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Disgrifiad</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Defnydd</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Awdur</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>Ebost</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Rhagor o Fanylion</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Iawn</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Llai o Fanylion</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Cyfreithiol</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Llythyr</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloid</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>tirlun</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>darlun</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>addasiedig</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">G&amp;waredu</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Dileu</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Iawn</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Diddymu</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>and</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>remove match</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>words</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove</source>
        <translation type="unfinished">Gwaredu</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation type="unfinished">Amnewid</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation type="unfinished">Gweithredu</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>with</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
