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
</context>
<context>
    <name>About</name>
    <message>
        <source>Build-ID:</source>
        <translation>Build-ID:</translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation>Versão do Scribus %1
%2 %3</translation>
    </message>
    <message>
        <source>%1. %2 %3 </source>
        <translation>%1. %2 %3</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation>Galego:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation>Tcheco</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation>Galês:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Contribuições de:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation>Polonês:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation>Ucraniano</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation>Eslovaco:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation>Italiano</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation>Dinamarquês:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation>Lituano:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation>Turco</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation>Húngaro:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation>Francês:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation>Búlgaro:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation>Alemão:</translation>
    </message>
    <message>
        <source>About Scribus%1%2</source>
        <translation type="obsolete">Sobre Scribus%1%2</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation>Windows port:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation>Norueguês:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation>Russo:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation>Português do Brasil:
</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation>Finlandês:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Fechar</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation type="unfinished"></translation>
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
        <source>Homepage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation type="unfinished">Referncia Online</translation>
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
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation>Ooções Avançadas</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation type="unfinished"></translation>
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
</context>
<context>
    <name>Align</name>
    <message>
        <source> p</source>
        <translation>paicas</translation>
    </message>
    <message>
        <source> in</source>
        <translation>pol</translation>
    </message>
    <message>
        <source> mm</source>
        <translation> mm</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Alinhar</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation>Lado Superior</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation>Centro</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation>Lado Direito</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation>Lado Esquerdo</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation>Lado Inferior</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Distribute/Align</source>
        <translation>Distribuir/Alinhar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation>Alinhar texto a esquerda</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Alinhar texto a direita</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Alinhar texto ao Centro</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Texto Justificado</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Forçar Texto Justificado</translation>
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
        <translation>soma</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Destino</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Data</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Push</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Star</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Thin</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Time</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Largo</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Prepend Currency Symbol</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Submit Form</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Limite de</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Verificar</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Cruzado</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>On Focus</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Ir Para</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Ícones</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Inset</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nome:</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Outro</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Plain</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Sólido</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Fonte:</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Caixa de Lista</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Usar Símbolo de Moeda</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Formato de Número</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Verificar Estilo:</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>Para Arquivo:</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Sem Imprimir</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Submit to URL:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Calcular</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Roll Over</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Valor não foi validado</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>máximo</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Formatando</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Combo Box</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Custom validate script:</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>mínimo</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Ação</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Borda</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Formato de Data</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Botão</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Círculo</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Côr:</translation>
    </message>
    <message>
        <source>Field Properties</source>
        <translation>Propriedades do Campo</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Pontilhado</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Custom</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Time Format</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Evento:</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Valor é o</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Format</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Oculto</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Custom calculation script:</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Custom Scripts</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Inverter</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Invisível</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Número</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Remover</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Square</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Style:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-Pos:</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Campo formatado como:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-Pos:</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation>Java Script</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Largura:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Tool-Tip:</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>On Blur</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Não Exportar Valores</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Submit Data as HTML</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Text Field</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opções</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Posicionamento do ícone...</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Mouse Up</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Pick...</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Mouse Enter</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Usar ícones</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Read Only</translation>
    </message>
    <message>
        <source>product</source>
        <translation>produto</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Text for Roll Over</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Validar</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Mudar...</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Percent Format</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Caracteres</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Password</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Visibilidade:</translation>
    </message>
    <message>
        <source>average</source>
        <translation>média</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Bizotado</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Importar Dados</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Mouse Exit</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Mouse Down</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Script</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Percentage</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Decimais:</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>dos seguintes campos:</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Importar Dados de:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation> e menor ou igual a: </translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Keystroke</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Multi-Line</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Underline</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Diamante</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Editável</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Aparência</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Check Box</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Valor não foi calculado</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Editar...</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Reset Form</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Required</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Não Verifique Ortografia</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Outlined</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Visível</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Valor deve ser maior ou igual a:</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Formato:</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Highlight</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Default foi Selecionado</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Exemplo:</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Não Exibir Rolagem</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Text for Button Down</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Necessário o ícone para Normal, para usar ícones para Botões</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Selection Change</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Propriedades</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Pressionado</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Página:</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Entre com uma lista de campos separados por virgulas aqui</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation>Imagens (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Destino</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Link</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Annotation Properties</source>
        <translation>Annotation Properties</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF- (*.pdf);;Todos (*)</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Link Externo</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Web-Link Externo</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Apply Template</source>
        <translation>Aplicar Modelo</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>To:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Delete</source>
        <translation>Deletar</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objeto</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>Scrapbooks (*.scs);;All Files (*)</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Scrapbook</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Nova Entrada</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Salvar</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Fechar</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Arquivo</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rename</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Move Bookmark</source>
        <translation>Mover Bookmark</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Bookmarks</source>
        <translation>Bookmarks</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Inserir Bookmark</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Bookmarks</translation>
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
        <translation>Ícone</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Nunca</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Reset</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Caption overlays Icon</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Escalar como:</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Layout:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Caption somente</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Caption abaixo do ícone</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Sempre</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Escala:</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Ícone Somente</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation type="unfinished">Quando o êcone for muito pequeno</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Caption á esquerda do ícone</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Caption á direita do ícone</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Quando o ícone for muito grande</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Sem proporção</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proporcional</translation>
    </message>
    <message>
        <source>Icon Placement</source>
        <translation>Posicionamento do ícone</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Caption acima do ícone</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Colorimétrico Absoluto</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Perfis Icc do Sistema</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Rendering Intents</translation>
    </message>
    <message>
        <source>Color Management Settings</source>
        <translation>Configurar Gerenciamento de Côr</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptual</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Colorimétrico Relativo</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturação</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation>Perfil de côr padrão para imagens importadas</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Perfil de côr padrão para cores sólidas na página </translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Color profile that you have generated or received from the manufacturer.(new line)
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Color profile for your printer model from the manufacturer.(new line)
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Black Point Compensation is a method of improving contrast in photos.(new line)
It is recommended that you enable this if you have photos in your document.</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation type="unfinished"></translation>
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
        <source>OK</source>
        <translation>OK</translation>
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
        <translation>Nova</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Antigo</translation>
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
        <translation>RGB Seguro para Web</translation>
    </message>
    <message>
        <source>Edit Color</source>
        <translation>Editar Côr</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Barras de Côr Estáticas</translation>
    </message>
    <message>
        <source>Dynamic Color Bars</source>
        <translation>Barras de Côr Dinâmicas</translation>
    </message>
    <message>
        <source> %</source>
        <translation> % </translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
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
        <source> %</source>
        <translation> % </translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Gradiente Radial</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Gradiente Horizontal</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Opacidade:</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Gradiente Vertical</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Atravessar Gradiente Diagonal</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Gradiente Diagonal</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Gradação:</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Editar Propriedades de Côr da Linha</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Editar Propriedades de Côr do Preenchimento</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Saturação da côr</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Método de Preenchimento Normal ou Gradiente</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Definir transparencia para a côr selecionada</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation>Côr do Objeto Selecionado</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X1:</source>
        <translation type="unfinished">X1:</translation>
    </message>
    <message>
        <source>Y1:</source>
        <translation type="unfinished">Y1:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X2:</source>
        <translation type="unfinished">X2:</translation>
    </message>
    <message>
        <source>Y2:</source>
        <translation type="unfinished">Y2:</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Cancelar</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>No</source>
        <translation>Não</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sim</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Somente Páginas Pares</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Valor</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Página por Folha</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Paisagem</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Espelho</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Opções</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Orientação</translation>
    </message>
    <message>
        <source>Printer Options</source>
        <translation>Opções de Impressora</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Somente Páginas Ímpares</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Retrato</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Page Set</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Páginas por Folha</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Impressão N-Up</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Todas Páginas</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>Encoding:</source>
        <translation>Codificação:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Move para seu Diretório de Documento. 
Isto pode ser definido nas Preferencias.</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>?</source>
        <translation>?</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Delete Color</source>
        <translation>Deletar Côr</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation>Substituí-la por:</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Cancelar</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation>Deletar Côr:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>to:</source>
        <translation>até:</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation>Deletar de:</translation>
    </message>
    <message>
        <source>Delete Pages</source>
        <translation>Deletar Páginas</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Usar</translation>
    </message>
    <message>
        <source>instead</source>
        <translation> ao invés de</translation>
    </message>
    <message>
        <source>Missing Font</source>
        <translation>Fonte Ausente</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>A Fonte %1 não está instalada.</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>All</source>
        <translation>Todas</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Arquivo</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation>Arquivos PostScript (*.ps);; Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opções</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Salvar como</translation>
    </message>
    <message>
        <source>Setup Printer</source>
        <translation>Confiruração da Impressora</translation>
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
        <translation>Amarelo</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Preto</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation type="unfinished">Erro Fatal</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation>O Nome do Estilo não é unico</translation>
    </message>
    <message>
        <source>Edit Style</source>
        <translation>Editar Estilo</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Character</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation>Espaços Verticais</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atenção</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Entrelinhas</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation>Efeito:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation type="unfinished">Fonte de texto selecionado ou objeto</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation type="unfinished">Tamanho da fonte</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation type="unfinished">Côr de preenchimento de texto</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation type="unfinished">Côr da borda de texto</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Editor</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation>Javascripts (*.js);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="unfinished">&amp;Desfazer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">&amp;Refazer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished">Corta&amp;r</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Arquivo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Editar</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="unfinished">Opções</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="unfinished"> dpi</translation>
    </message>
    <message>
        <source>Range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C</source>
        <translation type="unfinished">C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished"></translation>
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
        <translation>Tamanho:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Sem Título</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Título:</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Desconhecido</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Cores</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copia de %1</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Configuração das Cores</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Configuração de Côr Atual</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Definir set de Côr para carregar</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Salvar set de Côr atual</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Remover cores não utilizadas no set de côr do documento atual</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation>Adicionar cores ao set atual de um documento existente</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Criar uma côr nova dentro do ser atual</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Editar a côr selecionada</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Fazer uma cópia da côr selecionada</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Deletar côr a selecionada</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Definir o set de côr atual como set de côr default</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Escolha um nome</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Nova côr</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Deletar</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Yes</source>
        <translation>Sim</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Nome da fonte</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Substituies de Fontes</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Sub-sistema</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Reposio</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation>Inclusa em</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation>Usar Fonte</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Escolha um diretrio</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Global Font Settings</source>
        <translation>Configuraao Global de Fontes</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Caminhos Adicionais</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Fontes Disponíveis</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation>&quot;Path&quot; para os Arquivos de Fontes </translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Deletar</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="unfinished">Pijamas de seda trocados para quartzo azul</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source> p</source>
        <translation type="unfinished"> p</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished"> in</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished"> mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Guias Horizontais***</translation>
    </message>
    <message>
        <source>Manage Guides</source>
        <translation>Adminitrar Guias***</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Guia Vertical***</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Link</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Skip</source>
        <translation>Skip</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Aceitar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Hifenao Possvel</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Hyphenator Settings</source>
        <translation>Configuraes do hifenador</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation type="unfinished"></translation>
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
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>ao Final</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>antes da Pgina</translation>
    </message>
    <message>
        <source>Insert Page</source>
        <translation>Inserir Pgina</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation>Inserindo</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>ap¢s Pgina</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation>Template (Pagina Direita):</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Inserir Tabela</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation>Número de Linhas:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation>Número de Colunas:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>New Script</source>
        <translation>Novo Script</translation>
    </message>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Editar Javascripts</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Deletar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Fechar</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished">&amp;Não</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Sim</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation type="unfinished"></translation>
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
        <translation>Ação</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation>Shift+</translation>
    </message>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation>Gerenciar Atalhos de Teclado</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Selecione uma Tecla para esta Ação</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Tecla Atual</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atenção</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Delete Layer</source>
        <translation>Deletar Camada</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation>Abaixar Camada</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Camadas</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation>Elevar Camada</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation>Nova Camada</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation>Adicionar uma nova Camada</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation>Você quer deletar todos Objetos nessa camada também?</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Editar Estilos de Linhas</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Novo Estilo</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copia de %1</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atenção</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Deletar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Salvar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished">&amp;Não</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Sim</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="unfinished"> mm</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="unfinished">pol</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="unfinished">p</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source> p</source>
        <translation type="unfinished"> paicas</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished"> in</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished">mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Duplicao múltipla</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X1:</source>
        <translation type="unfinished">X1:</translation>
    </message>
    <message>
        <source>Y1:</source>
        <translation type="unfinished">Y1:</translation>
    </message>
    <message>
        <source>X2:</source>
        <translation type="unfinished">X2:</translation>
    </message>
    <message>
        <source>Y2:</source>
        <translation type="unfinished">Y2:</translation>
    </message>
    <message>
        <source>DX:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>DY:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Angle:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Length:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MenuTest</name>
    <message>
        <source>Script error</source>
        <translation>Erro de Script</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Se você estiver rodando um script oficial, transpásse-o para &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; por favor.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla .sla.gz .scd .scd.gz);;All Files (*)</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Imporar</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla ,scd);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Alterar...</translation>
    </message>
    <message>
        <source>Import Template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> from 0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="unfinished">ao Final</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>to:</source>
        <translation>para:</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Mover Página(s):</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>ao Final</translation>
    </message>
    <message>
        <source>Move Pages</source>
        <translation>Mover Páginas</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Copiar Página</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>Antes da Página</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>depois da Página</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
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
        <source>Name</source>
        <translation>Nome</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Nível</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Colorimétrico Absoluto</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Sem Estilo</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Distância da curva:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Rendering Intent:</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamanho da fonte</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Nome &quot;%1&quot; já existe.
Por favor escolha outro.</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Ponto Esquerdo</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation>Custom Spacing</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Propriedades</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Square Cap</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Mostrar Curva</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation>Manual Kerning</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Percepção</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Colorimétrico Relativo</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Junta bizotada</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Miter Join</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometria</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturação</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atenção</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Inserir Perfil:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Distância de texto</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Inicio de deslocamento:</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Basepoint:</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Forma:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Gradação:</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>End Points</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Arredondar Juntas</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Achatar Pontas</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Arredondar Pontas</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Linhas da Célula</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Linha no topo</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Linha á esquerda</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Linha á direita</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Linha na Base</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Nome do objeto selecionado</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Posição horizontal da basepoint atual</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Posição vertical da basepoint atual</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Largura</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Altura</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Rotação de objeto na basepoint atual</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Ponto de referência para medição e rotação</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Selecionar canto superior esquerdo como basepoint</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Selecionar canto superior direito como basepoint</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Selecionar canto inferior esquerdo como basepoint</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Selecionar canto inferior direito como basepoint</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Selecionar centro como basepoint</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Flip Horizontal</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Flip Vertical</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Mover um nível acima</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Mover um nível abaixo</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Mover para frente</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Mover para trás</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Travar ou Destravar um objeto</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Travar ou Destravar o tamanho de um objeto</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Habilitar ou desabilitar impressão de um objeto</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Fonte de texto selecionado ou objeto</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Dimensão da largura de caracteres</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Côr da borda de texto</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Côr de preenchimento de texto</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Saturação de côr da borda de texto</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Saturação de côr do preenchimento de texto</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Entrelinhas</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Estilo do parágrafo atual</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Alterar configurações para esquerda ou final de objetos</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Modelo da linha</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Largura da linha</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Tipo de junta de linha</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Tipo do final da linha</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Estilo de linha do objeto atual</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Selecione a forma do frame...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Editar forma do frame...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Configurar radius do arredondamento de canto</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Número de colunas no frame de texto</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Distância entre colunas</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Distância de texto da borda superior do frame</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Distância de texto da borda inferior do frame</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Distância de texto da borda esquerda do frame</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Distância de texto da borda direita do frame</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Editar configuração de tab do frame de texto...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Permitir que a imagem seja de tamanho diferente do frame</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Deslocamento horizontal da imagem dentro do frame</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Deslocamento vertical da imagem dentro do frame</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Redimensionar a imagem horizontalmente</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Redimensionar a imagem verticalmente</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Manter a escala X e Y iguais</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Posicionar a imagem de acordo com o tamanho do frame</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Usar proporções de imagens e não a do frame </translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>pol</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Column width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Kerning:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;X1:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X&amp;2:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y&amp;1:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Y2:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation type="unfinished"></translation>
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
        <translation>Achatar Pontas</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Dash Dot Dot Line</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Arredondar Juntas</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Espessura da linha</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Nome &quot;%1&quot; já existe.Por favor escolha outra.</translation>
    </message>
    <message>
        <source>Edit Style</source>
        <translation>Editar Estilo</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Esquadriar Ponta</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Linha separada</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Linha pontuada</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Junta bizotada</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Miter Join</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Arredondar Pontas</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Dash Dot Line</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Linha Sólida</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Cópia de %1</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atenção</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation>Novo Template</translation>
    </message>
    <message>
        <source>Edit Templates</source>
        <translation>Editar Template</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nome:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copiar #%1 de </translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Deletar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Fechar</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished">&amp;Não</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Sim</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>Points (pts)</source>
        <translation>Pontos (pts)</translation>
    </message>
    <message>
        <source>New Document</source>
        <translation>Novo Documento</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Customizar</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Paisagem</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opções</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Retrato</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Guias de Margem</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picas (P)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Polegadas (pol)</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Tamanho da Página</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation>Guia de colunas</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Milímetros (mm)</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="unfinished">Habilita layout single ou based</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="unfinished">Distância entre o topo da margem e a beirada da página</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="unfinished">Distância entre a base da margem e a beirada da página</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished">Distância entre a margem esquerda e a beirada da página</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished">Distância entre a margem direita e a beirada da página</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation type="unfinished">Unidade de medida padrão para edição de documento</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
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
        <translation>Página Esquerda</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Página Direita</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Ligações</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Resetar esse Ponto de Controle</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Deletar ponto de ligações</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Adicionar pontos de ligações</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Mover pontos ligação</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Mover pontos de controle</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Resetar Pontos de Controle</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation>Quando selecionado Coordenadas relativas a Página,
caso contrário Coordenadas serão relativas ao Objeto.</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation>Script Console</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source> p</source>
        <translation type="unfinished"> p</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished"> in</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished"> mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Caixa</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> sec</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Página</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Limpar</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Imagens:</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Distribuir</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Dissolver</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Colorimetrado absoluto</translation>
    </message>
    <message>
        <source>Export Range</source>
        <translation>Exportar Intervalo***</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Opes de Arquivo</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Margem Direita</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation>Fontes para incluir:</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Esquerda para Direita</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Direita para Esquerda</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Cores S¢lidas</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Restituo Intensionada***</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Cortinas***</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Sem Efeito</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Fontes Disponveis</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Dentro</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Margem Esquerda</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Fora</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Impressora</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Incluindo</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Caixa de Adornos</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Perfil</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Salvar como</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Compreensivo</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Colorimetria Relativa***</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Senhas</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>PDF/X-3 Sada Intencionada***</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efeitos</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturao</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Configuraes</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Geral</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Iluminar</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Base ao Topo***</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Topo para Base</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation>Configurações de Imagem </translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automática</translation>
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
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Máximo</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Alto</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Médio</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Baixo</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Mínimo</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Canto superior esquerdo ao Canto inferior direito</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Screen / Web</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Não usar Perfís ICC linkados</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Exportar todas páginas para PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Exportar várias páginas para PDF </translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>Período de tempo que a página é exibida antes que a apresentação comece na página selecionada.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Período em que o efeito acontece.
Quanto menor o tempo mais rápido será o efeito.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Aplicar os efeitos selecionados a todas as páginas.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation>Seleciona uma senha que habilita ou impede o uso de recursos
de segurança em se PDF exportado</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Adiciona um perfil de côr a uma côr sólida</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Adiciona um perfil de côr em imagens </translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Não usar perfís de cores que estejam adicionadas em sua fonte de imagens</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Distância para sangria do topo da página</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Distância para sangria da base da página</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Distância para sangria no lado esquerdo da página</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Distância para sangria no lado direito da página</translation>
    </message>
    <message>
        <source>Create PDF File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;&gt;&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;&lt;&lt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="unfinished">Linha</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Salvar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <source>All</source>
        <translation>Tudo</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation>Visualizar Impressão</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Y</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation type="unfinished"></translation>
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
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Text Frame</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>Copiar Aqui</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Mover Aqui</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Editar Texto...</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Imagem</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Arquivo:</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Texto linkado</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Texto ao Caminho</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Parágrafos:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Palavras:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Carácteres:</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Imprimir:</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Habilitado</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Desabilitado</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation>O Programa</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation>está faltando!!!</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Copia de</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished">Corta&amp;r</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Deletar</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="unfinished">Você quer realmente apagar todo o seu texto?</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation type="unfinished">Imagem</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="unfinished">Texto</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="unfinished">Linha</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="unfinished">Polígono</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="unfinished">Polyline</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation type="unfinished">Caminho de Texto</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation>Página</translation>
    </message>
    <message>
        <source> of %1</source>
        <translation>de %1</translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Result</source>
        <translation>Resultado</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Selecionar</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Pesquisar Resultados para: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Visualizar</translation>
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
        <translation>Sim</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Ir para</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nome</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Página</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Caminho</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Imprimir</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Faltando</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Pesquisar</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Status</translation>
    </message>
    <message>
        <source>Pictures</source>
        <translation>Figuras</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Polygon Properties</source>
        <translation>Propriedades de Polígono</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="unfinished">Número de cantos para polígonos</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="unfinished">Grau de rotação nos polígonos</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="unfinished">Exemplo do Polígono</translation>
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
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source> p</source>
        <translation> p</translation>
    </message>
    <message>
        <source> in</source>
        <translation> in</translation>
    </message>
    <message>
        <source> mm</source>
        <translation> mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>GUI</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menus</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Caminhos</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Outro</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>Pequeno</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Ferramentas</translation>
    </message>
    <message>
        <source>Units</source>
        <translation>Unidades</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Outras Opções</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Letras Minúsculas</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Custom</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Paisagem</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Guias</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation>Cores de Grid</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Médio</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation>Layout da Grid</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Documento</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Tipografia</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Portrait</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation>Posicionando</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Guia de Margens</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Visualizar</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Pijamas de seda trocados para quartzo azul</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Scrapbook</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation>Para ajustar o display arraste a régua abaixo com o controle.</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picas (p)</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Subscrito</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Superscrito</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation>Pontos (pt)</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Apresentar</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Escolher um Diretório</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Polegadas (pol)</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Salvar Automaticamente</translation>
    </message>
    <message>
        <source>Preferences</source>
        <translation>Preferências</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Tamanho da Página</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Geral</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Ferramentas Externas</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation>Misc.</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Milímetros (mm)</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation>Interpretador Postscript</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Ferramenta de Processamento de Imagem</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Impressão</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation>Selecione um padrão de layout.
Scribus aceita qualquer tema de KDE e QT disponível</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Tamanho de fonte padrão para menus e janela</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Unidade de medida padrão para edição de documento</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Número de linha movidas pelo programa a cada movimento do &quot;wheel&quot; do mouse</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Raio da área donde um manozeador de objetos pode ser selecionado</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Número de documentos recentes a serem mostrados no menu Arquivo</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Diretório de documento padrão</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation>Diretório de Perfil ICC padrão</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Diretório de scripts padrão</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Tamanho de Página Padrão</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Orientação Padrão das Páginas do Documento</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Largura das Páginas do Documento, editável se você escolheu uma página costumizada</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Altura da Página no documento</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Habilita layout single ou based</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation>Faz com que a primeira página seja página esquerda do documento</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Distância entre o topo da margem e a beirada da página</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Distância entre a base da margem e a beirada da página</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distância entre a margem esquerda e a beirada da página</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distância entre a margem direita e a beirada da página</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation>Quando habilitado, Scribus salva uma ćopia de seu arquivo com a extensão .bak
cada vez que se passar o período definido</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Período de tempo para salvar automaticamente</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Distância entre  as menores linhas de Grids</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Distância entre  as maiores linhas de Grids</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Distância que um objeto se posicionará da guia inserida no documento</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Côr da menor linha de Grid</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Côr da maior linha de Grid</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Côr da linha guia inserida</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation>Posicione o grid atrás dos objetos em sua página </translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation>Posicione o grid a frente dos objetos em sua página </translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Deslocamento acima da linha de base de uma fonte na linha </translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Tamanho relativo do Superscript comparado a uma fonte normal </translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Deslocamento abaixo da linha de base de uma fonte na linha </translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Tamanho relativo do Subscript comparado a uma fonte normal </translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Tamanho relativo de Small Cap comparado a uma fonte normal </translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Porcentagem de ampliação sobre o tamanho de fonte para o espçamento de linha</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Propriedades de Frame de Texto</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Propriedades do Frame de Figura</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Propriedades do Shape Drawing</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Nível de Magnificação Padrão</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Propriedades de Line Drawing</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Propriedades de Polygon Drawing</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Fonte para novos frames de texto</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Tamanho de fonte para novos frames de texto</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Côr da Fonte</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Número de Colunas em um Frame de Texto</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Espaço entre colunas no frame de texto </translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Exemplo de Fonte</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Frames de Figuras permitem que as mesmas sejam redimensionadas a qualquer tamanho</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Escala Horizontal das imagens</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Escala Vertical das imagens</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Manter escala horizontal e vertical proporcionais</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Figuras num frame de figuras são dimensionadas ao mesmo tamanho do Frame</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Figuras dimensionadas manteêm suas proporções originais automaticamente</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Preencher côr dos frames de figura</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Saturação da côr de preenchimento</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Côr da linha dos shapes</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Saturação da côr das linhas</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Preencher côr dos Shapes</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Estilo de linha dos Shapes </translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Espessura da linha dos Shapes</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Magnificação mínima disponível</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Magnificação máxima disponível</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Alterar magnificação a cada operação de zoom</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Côr das linhas</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Saturação de côr</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Estilo de linha </translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Espessura de linhas</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Número de cantos para polígonos</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Grau de rotação nos polígonos</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Exemplo do Polígono</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Selecionar o tamanho do preview na paleta scrapbook</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation>Mostrar páginas lado a lado quando for facing pages</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Côr para o papel</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Côr para as linhas da margem
</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Mascarar a área fora das margens na côr de margens</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation>Habilitar recursos de tranparências dentro da exportação PDF 1.4</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Definir nível padrão do zoom</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation>Local dos arquivos do sistema para o interpretador Ghostscript</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Texto antialias para EPS e PDF na renderização da tela</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Gráfico antialias para EPS e PDF na renderização da tela</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Não mostrar objetos fora das margens numa página impressa ou arquivo exportado</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> px</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Large</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
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
        <source>Apply &amp;Under Color Removal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tabloid</source>
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
        <translation>&amp;Red:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Green:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>Bl&amp;ue:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>Canal A&amp;lpha:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>&amp;Cores básicas</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Cores customizadas</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Definir cores costumizadas &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Adicionar a Cores Customizadas</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Selecionar Côr</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Copiar ou mover um arquivo</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Ler: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Escrever: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>&amp;Nome do arquivo:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>&amp;Tipo de arquivo:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Um driretório acima</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nome</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Tamanho</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Data</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Atributos</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Olhar &amp;em:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Voltar</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Crear nova pasta</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Visualizar Lista</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Visualizar detalhe</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Preview informação de arquivo</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Preview Conteúdo de Arquivo</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Read-write</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Read-only</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Write-only</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Inacessível</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Symlink para Arquivo</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Symlink para Diretório</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Symlink para Especial</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Arquivo</translation>
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
        <translation>Salvar como</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Abrir</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Salvar</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Renomear</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Deletar</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>R&amp;eload</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Organizar por &amp;Nome</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Organizar por &amp;Tamanho</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Organizar por &amp;Data</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Aleatório</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Organizar</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Exibir arquivos &amp;ocultos</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>o arquivo</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>o diretório</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>o symlink</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Deletar %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Você realmete deseja deletar %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sim</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Não</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Nova Pasta 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Nova Pasta</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Nova Pasta %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Encontrar Diretório</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Diretórios</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Salvar</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Erro</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Arquivo não encontrado.
Cheque o caminho e o nome do arquivo.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Todos Arquivos (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Selecione um diretório</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Diretório:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="unfinished">&amp;Tamanho</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="unfinished">Efeitos</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sample</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation type="unfinished">Aplicar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Cancelar</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation type="unfinished">Selecione Fonte</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Limpar</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Selecionar Tudo</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Desfazer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Refazer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Corta&amp;r</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Colar</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Alinhar</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Customizar...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Sobre Qt&lt;/h3&gt;&lt;p&gt;Esse programa usa Qt versão %1.&lt;/p&gt;&lt;p&gt;Qt é um utilitário C++ multiplataforma GUI &amp;amp; aplicativos de desenvolvimento.&lt;/p&gt;&lt;p&gt;Qt suporta MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, e a maioria de variantes Unix.&lt;br&gt;Qt também é disponível para dispositivos acoplados.&lt;/p&gt;&lt;p&gt;Qt é um produto Trolltech. Acesse &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; para mais informações.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>No</source>
        <translation>No</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sim</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Voc deseja realmente substituir o Arquivo:
%1 ?</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Salvar Como</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Referncia Online</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Inicializando...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Documento</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Plano de Fundo</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation type="unfinished">Visualizar Impressão</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation>S&amp;cript</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Python Scripts (*.py);; Todos os Arquivos (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>Imagens-SVG (*.svg *.svgz);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>Imagens-SVG (*.svg);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Signs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cards</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Labels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Menus</source>
        <translation type="unfinished">Menus</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Posters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Folds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="unfinished">Todos formatos Suportados</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>html</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation type="unfinished"></translation>
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
        <source>Font %1 is broken, discarding it</source>
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
        <translation type="unfinished">Catalão</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Czech</source>
        <translation type="unfinished">Tcheco</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation type="unfinished">Dinamarquês</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation type="unfinished">Holandês</translation>
    </message>
    <message>
        <source>English</source>
        <translation type="unfinished">Inglês</translation>
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
        <translation type="unfinished">Alemão</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation type="unfinished">Terminado</translation>
    </message>
    <message>
        <source>French</source>
        <translation type="unfinished">Francês</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="unfinished">Grego</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation type="unfinished">Húngaro</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Italian</source>
        <translation type="unfinished">Italiano</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Polonês</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation type="unfinished">Russo</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation type="unfinished">Espanhol</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation type="unfinished">Eslovaco</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="unfinished"></translation>
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
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Limpar</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Selecionar Tudo</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Desfazer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Refazer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Corta&amp;r</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;ColarLimpar</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Menu do sistema</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Sombra</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Remover Sombra</translation>
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
        <translation>FecharFechar</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Refazer</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Mover</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Tamanho</translation>
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
        <translation>&amp;Fechar</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Ficar no &amp;Topo</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimizar</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Refazer Abaixo</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Fechar</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>So&amp;mbra</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>&amp;Remover Sombra</translation>
    </message>
</context>
<context>
    <name>Query</name>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Document Setup</source>
        <translation>Configuração do Documento</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Guia de Margens</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="unfinished">Habilita layout single ou based</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="unfinished">Distância entre o topo da margem e a beirada da página</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="unfinished">Distância entre a base da margem e a beirada da página</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished">Distância entre a margem esquerda e a beirada da página</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished">Distância entre a margem direita e a beirada da página</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Tamanho da Página</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="unfinished">Tamanho:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="unfinished">Orientação:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Paisagem</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Largura:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style of current paragraph</source>
        <translation type="unfinished">Estilo do parágrafo atual</translation>
    </message>
    <message>
        <source>Style Settings</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation type="unfinished">Côr de preenchimento de texto</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation type="unfinished">Saturação de côr do preenchimento de texto</translation>
    </message>
    <message>
        <source>Fill Color Settings</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation type="unfinished">Côr da borda de texto</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation type="unfinished">Saturação de côr da borda de texto</translation>
    </message>
    <message>
        <source>Stroke Color Settings</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font of selected text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation type="unfinished">Tamanho da fonte</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation type="unfinished">Dimensão da largura de caracteres</translation>
    </message>
    <message>
        <source>Font Settings</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Kerning:</source>
        <translation type="unfinished">Kerning:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="unfinished">Manual Kerning</translation>
    </message>
    <message>
        <source>Character Settings</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Background</source>
        <translation>Plano de Fundo</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copiar #%1 de </translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>OK</source>
        <translation>OK</translation>
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
        <source>All</source>
        <translation>Todo</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Cortar</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Novo</translation>
    </message>
    <message>
        <source>200%</source>
        <translation>200%</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Copiar</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Arquivo</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Fonte</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Trancar</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Sair</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Salvar</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Tamanho</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation>Saindo Agora</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation>Desfazer</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation>Alinhamento á Direita</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Strikethru</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation>Alinhamento ao Centro</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation>Estilos...</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation>Distribuir/Alinhar...</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Limpar</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Fechar</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Côr</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tcheco</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Holandês</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Grego</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Grupo</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation>Abaixar</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Colar</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation>Subir</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Pronto</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Sombra</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>EStilo</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Ferramentas</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation>Enviar devolta</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation>Os Programas seguintes estão faltando:</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italiano</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation>Snap to Grid</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Inicializando Plugins</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation>Desagrupar</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation>Smart Hífen</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation>EPS-Files (*.eps);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation>Deletar...</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Minúsculas</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Duplicão Múltipla</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation>Mostrar Paleta da Página</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation>Cores...</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation>Thumbnails</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Pesquisando Fontes</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation>Document Info...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation>Mover...</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation>Ocultar Frames</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Dinamarquês</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Deletar</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation>Sem quebra de espaço</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation>Ocultar Imagens</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Húngaro</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Francês</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Alemão</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation>Trancar/Destrancar</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Inverter</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation>Não há Fontes Postscript no seu Sistema</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation>Mostrar Camadas</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polonês</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation>Carregando:</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);;Todos Arquivos (*) </translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Eslovaco</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Documento</translation>
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
        <source>Unlock</source>
        <translation>Destravar</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation>Fontes...</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation>Abrir...</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation>Mostrar Grid</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation>Dupliicar</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation>Ocultar Margens</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation>Salvar como...</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation>Trazer Figura...</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation>Atachar Texto ao Caminho</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Inserir Número de Página</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation>Trazer á Frente</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Ajustando Cores</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation>Gerenciar Guias</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation>Ukraniano</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Lendo Scrapbook</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation>Configuração do Documento...</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation>Javascripts...</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation>Ajustar a Janela</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Imprimir...</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Sobre Qt</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation>Trazer Texto/Figura...</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Catalão</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Salvar como</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation>Aplicar Template...</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Subscript</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Superscript</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation>Inserir...</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Russo</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Falha na impressão!</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation>Hifenar Texto</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd .scd.gz);; Todos Arquivos(*) </translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Espanhol</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Trazer Text...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Imprindo...</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Sublinhar</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Carregando...</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Escolha um diretório</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation>Ajuda Online</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation>Selecionar Tudo</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Português</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Gerenciar Figuras</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Inglês</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Delineado</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation>Templates...</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Salvando...</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Lendo Preferências</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Configurando teclas de atalho</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Erro Fatal</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation>Alinhamento a Esquerda</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation>Selecionar Nova Fonte</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation>Sobre Scribus</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation>Inicializar Hifenador</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation>Não pode escrever o Arquivo: 
%1</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Manual do Scribus</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation>Criar um novo documento</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation>Abrir um Documento</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation>Salvar Documento Atual</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation>Fechar Documento Atual</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation>Imprimir esse documento</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation>Salvar Documento Atual como PDF</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Propriedades</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Outline</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Scrapbook</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Arquivo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Item</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Página</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Visualizar</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Ferramentas</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtras</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>&amp;Janelas</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Ajuda</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Todos formatos Suportados</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Arquivos de Texto (*.txt);;Todos Arquivos(*)</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Terminado</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation>Irlandês</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Scribus Crash</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus trava devido ao Sinal #%1</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Fechar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Salvar</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="unfinished">&amp;Desfazer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished">Corta&amp;r</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Deletar</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>100%</source>
        <translation type="unfinished">100%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="unfinished">&amp;Tamanho</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="unfinished">Imporar</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="unfinished"> mm</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="unfinished">pol</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="unfinished">p</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Cancelar</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation type="unfinished"></translation>
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
</context>
<context>
    <name>ScribusView</name>
    <message>
        <source> %</source>
        <translation>Visualizao do Scribus:: %</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Todo</translation>
    </message>
    <message>
        <source>Layer</source>
        <translation>Camada</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="unfinished"> mm</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="unfinished">pol</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="unfinished">p</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>has been changed since the last save.</source>
        <translation>foi feita uma alteração desde a última vez que foi salvo.</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Documento:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Mostrar  Preview da Página</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation>Mostrar nome de Templates</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Pesquisar/Substituir</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Pesquisar por:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Estilo do Parágrafo</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Fonte</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamanho da fonte</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Efeitos de Fonte</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Preencher Côr</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Preencher Sombra</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Côr do Contorno</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Sombra do Contorno</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Esquerda</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centro</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Direita</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Bloquear</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Forçado</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nada</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Substituir com:</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Pesquisa Terminada</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Fechar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation>Templates Disponíveis:</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation>Página esquerda primeiro</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation>Adornando Páginas</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Páginas do Documento:</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Organizar Páginas</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation>Arrastar páginas ou páginas do Template para a lata de lixo para deletá-las.</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation>Aqui estão todos Templates, para criar uma página nova
arraste o Template para a Pageview abaixo.</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Available Fields</source>
        <translation>Campos Disponíveis</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>Campos Selecionados</translation>
    </message>
    <message>
        <source>Select Fields</source>
        <translation>Selecionar Campos</translation>
    </message>
    <message>
        <source>&amp;&gt;&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;&lt;&lt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Outro...</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Sombra</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation type="unfinished">Sem Estilo</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Sem Estilo</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>No</source>
        <translation>Não</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sim</translation>
    </message>
    <message>
        <source>Edit Styles</source>
        <translation>Editar Estilos</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Novo Estilo</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copia de %1</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Deletar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Salvar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>File</source>
        <translation type="unfinished">Arquivo</translation>
    </message>
    <message>
        <source>Story Editor</source>
        <translation>Editor do Histórico</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Você quer realmente apagar todo o seu texto?</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation type="unfinished">Atualizar Frame de Texto</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Parágrafo Atual:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Palavras:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Carácteres:</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Total:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Parágrafos:</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation>Você quer realmente deseja perder todas alterações?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Arquivos de Texto (*.txt);;Todos Arquivos(*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Salvar como</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished">Corta&amp;r</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Arquivo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Editar</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation type="unfinished">Pesquisar/Substituir</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation type="unfinished"></translation>
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
        <translation>Sublinhado</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Mínusculas</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Subscrito</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Superscrito</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Strike Out</translation>
    </message>
    <message>
        <source>Outline Text</source>
        <translation>Texto Outline</translation>
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
        <translation type="unfinished">OK</translation>
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
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Gerenciar Tabuladores</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation type="unfinished">Esquerda</translation>
    </message>
    <message>
        <source>Right</source>
        <translation type="unfinished">Direita</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation type="unfinished">Ponto</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation type="unfinished">Vírgula</translation>
    </message>
    <message>
        <source>Center</source>
        <translation type="unfinished">Centro</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left Ind&amp;ent:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>X:</source>
        <translation>X:</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation>Y:</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linha</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Página</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Fonte:</translation>
    </message>
    <message>
        <source>Image</source>
        <translation>Imagem</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polyline</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Outline</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polígono</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Caminho de Texto</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Informação</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Elemento</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Group </source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Ferramentas</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation>Inserir Linhas</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation>Inserir Figura</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation>Selecionar Itens</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Editar Conteúdo do Frame</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation>Inserir Curvas Bezier</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Rotacionar Ítem</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Propriedades...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation>Inserir Polígonos</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Inserir Frame de Texto</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Inserir Tabela</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Inserir Freehand Line</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Zoom in ou out</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Editar o Texto com o editor de Histórico</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Linkar Frames de Texto</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Separar Frames de Texto</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation>Desenhar Várias Formas</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>Link</source>
        <translation>Link</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Caixa de Lista</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Combo Box</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Base</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Campo de Texto</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Check Box</translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation>Selecionar letra:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Fechar</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="unfinished">Automática</translation>
    </message>
    <message>
        <source>Get text only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation type="unfinished">Codificação:</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">OK</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation type="unfinished">&amp;Abrir</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Nome</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Tamanho da Página</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation type="unfinished">Cores</translation>
    </message>
    <message>
        <source>Description</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Usage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Created with</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Author</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Date</source>
        <translation type="unfinished">Data</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Nome</translation>
    </message>
    <message>
        <source>Category</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Tamanho da Página</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation type="unfinished">Cores</translation>
    </message>
    <message>
        <source>Description</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Usage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Author</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Email</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>More Details</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>landscape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>portrait</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>custom</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Deletar</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Remover</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation type="unfinished">Substituir</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation type="unfinished">Aplicar</translation>
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
